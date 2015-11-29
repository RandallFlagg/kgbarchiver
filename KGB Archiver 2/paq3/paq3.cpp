// paq3.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

/* PAQ3 - File archiver and compressor.
(C) 2003, Matt Mahoney, mmahoney@cs.fit.edu

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation at
http://www.gnu.org/licenses/gpl.txt or (at your option) any later version.
This program is distributed without any warranty.

USAGE

To compress:      PAQ3 archive file file...  (1 or more file names), or
  or (MSDOS):     dir/b | PAQ3 archive       (read file names from input)
  or (UNIX):      ls    | PAQ3 archive
To decompress:    PAQ3 archive
To list contents: more < archive

Compression:  The files listed are compressed and stored in the archive,
which is created.  The archive must not already exist.  File names may
specify a path, which is stored.  If there are no file names on the command
line, then PAQ3 prompts for them, reading until the first blank line or
end of file.

Decompression:  No file names are specified.  The archive must exist.
If a path is stored, the file is extracted to the appropriate directory,
which must exist.  PAQ3 does not create directories.  If the file to be
extracted already exists, it is not replaced; rather it is compared with
the archived file, and the offset of the first difference is reported.

It is not possible to add, remove, or update files in an existing archive.
If you want to do this, extract the files, delete the archive, and
create a new archive with just the files you want.

PAQ3 is an improved version of PAQ1SSE/PAQ2 by Serge Osnach, who added
SSE to my PAQ1 (both available at http://cs.fit.edu/~mmahoney/compression/ ).
PAQ3 uses an improved SSE implementation, and adds update exclusion to
the NonstationaryPPM and WordModel models.
*/

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <ctime>
#include <new>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

const int MEM=8;  // 0-8 = 1M, 2M ... 256M memory model.  6 = 64M

// 8-32 bit unsigned types, adjust as appropriate
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;

class U24 {  // 24-bit unsigned int
  U8 b0, b1, b2;  // Low, mid, high byte
public:
  explicit U24(int x=0): b0(x), b1(x>>8), b2(x>>16) {}
  operator int() const {return (((b2<<8)|b1)<<8)|b0;}
}; 

// 32-bit random number generator based on r(i) = r(i-24) ^ r(i-55)
class Random {
  U32 table[55];  // Last 55 random values
  int i;  // Index of current random value in table
public:
  Random();
  U32 operator()() {  // Return 32-bit random number
    if (++i==55) i=0;
    if (i>=24) return table[i]^=table[i-24];
    else return table[i]^=table[i+31];
  }
} rnd;

Random::Random(): i(0) {
  for (int j=0; j<55; ++j)
    table[j]=314159265*j;
  for (int j=0; j<10000; ++j)
    operator()();
}

/* Model interface.  A Predictor is made up of a collection of various
models, whose outputs are summed to yield a prediction.  Methods:

   Model.predict(int& n0, int& n1) - Adds to counts n0 and n1 such that
     it predicts the next bit will be a 1 with probability n1/(n0+n1)
     and confidence n0+n1.
   Model.update(int y) - Appends bit y (0 or 1) to the model.
*/
class Model {
public:
  virtual void predict(int& n0, int& n1) const = 0;
  virtual void update(int y) = 0;
  virtual ~Model() {}
};

/* Hash table element base class.  It contains an 8-bit checksum to
detect collisions, and a priority() method which is used to control
replacement when full by replacing the element with the lowest priority
(0 = unused).  The derived class should supply the data
to be stored and override priority().  */

class HashElement {
  U8 ch;  // Checksum
public:
  HashElement(int c=0): ch(c) {}  // Initialize the checksum, 0 = unused
  int checksum() const {return ch;}  // Collision if not matched
  int priority() const {return ch!=0;}  // Override: lowest replaced first
};

/* 3 byte counter, shown for reference only.  It implements a
nonstationary pair of counters of 0s and 1s such that preference is
given to recent history by discarding old bits. */

class Counter3: public HashElement {
  U8 n[2];  // n[y] is the counts of ys (0s or 1s)
public:
  Counter3(int c=0): HashElement(c) {n[0]=n[1]=0;}
  int get0() const {return n[0];}  // Return count of 0s
  int get1() const {return n[1];}  // Return count of 1s
  int priority() const {return get0()+get1();}  // For hash replacement
  void add(int y) {  // Add y (0 or 1) to n[y] and age the opposite count
    if (n[y]<255) ++n[y];
    if (n[1-y]>2) (n[1-y]/=2)+=1;
  }
};

/* Approximately equivalent 2 byte counter implementing the above.
The representable counts (n0, n1) are 0-10, 12, 14, 16, 20, 24, 28,
32, 48, 64, 128, 256, 512.  Both counts are represented by a single
8-bit state.  Counts larger than 10 are incremented probabilistically.
Although it uses 1/3 less memory, it is 8% slower and gives 0.05% worse
compression than the 3 byte counter. */

class Counter: public HashElement {
  U8 state;
  struct E {      // State table entry
    U16 n0, n1;   // Counts represented by state
    U8 s00, s01;  // Next state on input 0 without/with probabilistic incr.
    U8 s10, s11;  // Next state on input 1
    U32 p0, p1;   // Probability of increment x 2^32-1 on inputs 0, 1
  };

  static E table[244];  // State table
public:
  Counter(int c=0): HashElement(c), state(0) {}
  int get0() const {return table[state].n0;}
  int get1() const {return table[state].n1;}
  int priority() const {return state;}
  void add(int y) {
    if (y) {
      if (state<94 || rnd()<table[state].p1)
        state=table[state].s11;
      else
        state=table[state].s10;
    }
    else {
      if (state<94 || rnd()<table[state].p0)
        state=table[state].s01;
      else
        state=table[state].s00;
    }
  }
};

// State table generated by stategen.cpp
Counter::E Counter::table[244] = {
 //   n0  n1 s00 s01 s10 s11     p0          p1        state
    {  0,  0,  0,  2,  0,  1,4294967295u,4294967295u}, // 0
    {  0,  1,  1,  4,  1,  3,4294967295u,4294967295u}, // 1
    {  1,  0,  2,  5,  2,  4,4294967295u,4294967295u}, // 2
    {  0,  2,  3,  7,  3,  6,4294967295u,4294967295u}, // 3
    {  1,  1,  4,  8,  4,  7,4294967295u,4294967295u}, // 4
    {  2,  0,  5,  9,  5,  8,4294967295u,4294967295u}, // 5
    {  0,  3,  3,  7,  6, 10,4294967295u,4294967295u}, // 6
    {  1,  2,  7, 12,  7, 11,4294967295u,4294967295u}, // 7
    {  2,  1,  8, 13,  8, 12,4294967295u,4294967295u}, // 8
    {  3,  0,  9, 14,  5,  8,4294967295u,4294967295u}, // 9
    {  0,  4,  6, 11, 10, 15,4294967295u,4294967295u}, // 10
    {  1,  3,  7, 12, 11, 16,4294967295u,4294967295u}, // 11
    {  2,  2, 12, 18, 12, 17,4294967295u,4294967295u}, // 12
    {  3,  1, 13, 19,  8, 12,4294967295u,4294967295u}, // 13
    {  4,  0, 14, 20,  9, 13,4294967295u,4294967295u}, // 14
    {  0,  5,  6, 11, 15, 21,4294967295u,4294967295u}, // 15
    {  1,  4, 11, 17, 16, 22,4294967295u,4294967295u}, // 16
    {  2,  3, 12, 18, 17, 23,4294967295u,4294967295u}, // 17
    {  3,  2, 18, 25, 12, 17,4294967295u,4294967295u}, // 18
    {  4,  1, 19, 26, 13, 18,4294967295u,4294967295u}, // 19
    {  5,  0, 20, 27,  9, 13,4294967295u,4294967295u}, // 20
    {  0,  6, 10, 16, 21, 28,4294967295u,4294967295u}, // 21
    {  1,  5, 11, 17, 22, 29,4294967295u,4294967295u}, // 22
    {  2,  4, 17, 24, 23, 30,4294967295u,4294967295u}, // 23
    {  3,  3, 18, 25, 17, 23,4294967295u,4294967295u}, // 24
    {  4,  2, 25, 33, 18, 24,4294967295u,4294967295u}, // 25
    {  5,  1, 26, 34, 13, 18,4294967295u,4294967295u}, // 26
    {  6,  0, 27, 35, 14, 19,4294967295u,4294967295u}, // 27
    {  0,  7, 10, 16, 28, 36,4294967295u,4294967295u}, // 28
    {  1,  6, 16, 23, 29, 37,4294967295u,4294967295u}, // 29
    {  2,  5, 17, 24, 30, 38,4294967295u,4294967295u}, // 30
    {  3,  4, 24, 32, 23, 30,4294967295u,4294967295u}, // 31
    {  4,  3, 25, 33, 24, 31,4294967295u,4294967295u}, // 32
    {  5,  2, 33, 42, 18, 24,4294967295u,4294967295u}, // 33
    {  6,  1, 34, 43, 19, 25,4294967295u,4294967295u}, // 34
    {  7,  0, 35, 44, 14, 19,4294967295u,4294967295u}, // 35
    {  0,  8, 15, 22, 36, 45,4294967295u,4294967295u}, // 36
    {  1,  7, 16, 23, 37, 46,4294967295u,4294967295u}, // 37
    {  2,  6, 23, 31, 38, 47,4294967295u,4294967295u}, // 38
    {  3,  5, 24, 32, 30, 38,4294967295u,4294967295u}, // 39
    {  4,  4, 32, 41, 31, 39,4294967295u,4294967295u}, // 40
    {  5,  3, 33, 42, 24, 31,4294967295u,4294967295u}, // 41
    {  6,  2, 42, 52, 25, 32,4294967295u,4294967295u}, // 42
    {  7,  1, 43, 53, 19, 25,4294967295u,4294967295u}, // 43
    {  8,  0, 44, 54, 20, 26,4294967295u,4294967295u}, // 44
    {  0,  9, 15, 22, 45, 94,4294967295u,4294967295u}, // 45
    {  1,  8, 22, 30, 46, 55,4294967295u,4294967295u}, // 46
    {  2,  7, 23, 31, 47, 56,4294967295u,4294967295u}, // 47
    {  3,  6, 31, 40, 38, 47,4294967295u,4294967295u}, // 48
    {  4,  5, 32, 41, 39, 48,4294967295u,4294967295u}, // 49
    {  5,  4, 41, 51, 31, 39,4294967295u,4294967295u}, // 50
    {  6,  3, 42, 52, 32, 40,4294967295u,4294967295u}, // 51
    {  7,  2, 52, 62, 25, 32,4294967295u,4294967295u}, // 52
    {  8,  1, 53, 63, 26, 33,4294967295u,4294967295u}, // 53
    {  9,  0, 54, 95, 20, 26,4294967295u,4294967295u}, // 54
    {  1,  9, 22, 30, 55, 96,4294967295u,4294967295u}, // 55
    {  2,  8, 30, 39, 56, 64,4294967295u,4294967295u}, // 56
    {  3,  7, 31, 40, 47, 56,4294967295u,4294967295u}, // 57
    {  4,  6, 40, 50, 48, 57,4294967295u,4294967295u}, // 58
    {  5,  5, 41, 51, 39, 48,4294967295u,4294967295u}, // 59
    {  6,  4, 51, 61, 40, 49,4294967295u,4294967295u}, // 60
    {  7,  3, 52, 62, 32, 40,4294967295u,4294967295u}, // 61
    {  8,  2, 62, 71, 33, 41,4294967295u,4294967295u}, // 62
    {  9,  1, 63, 97, 26, 33,4294967295u,4294967295u}, // 63
    {  2,  9, 30, 39, 64, 99,4294967295u,4294967295u}, // 64
    {  3,  8, 39, 49, 56, 64,4294967295u,4294967295u}, // 65
    {  4,  7, 40, 50, 57, 65,4294967295u,4294967295u}, // 66
    {  5,  6, 50, 60, 48, 57,4294967295u,4294967295u}, // 67
    {  6,  5, 51, 61, 49, 58,4294967295u,4294967295u}, // 68
    {  7,  4, 61, 70, 40, 49,4294967295u,4294967295u}, // 69
    {  8,  3, 62, 71, 41, 50,4294967295u,4294967295u}, // 70
    {  9,  2, 71,100, 33, 41,4294967295u,4294967295u}, // 71
    {  3,  9, 39, 49, 64, 99,4294967295u,4294967295u}, // 72
    {  4,  8, 49, 59, 65, 72,4294967295u,4294967295u}, // 73
    {  5,  7, 50, 60, 57, 65,4294967295u,4294967295u}, // 74
    {  6,  6, 60, 69, 58, 66,4294967295u,4294967295u}, // 75
    {  7,  5, 61, 70, 49, 58,4294967295u,4294967295u}, // 76
    {  8,  4, 70, 78, 50, 59,4294967295u,4294967295u}, // 77
    {  9,  3, 71,100, 41, 50,4294967295u,4294967295u}, // 78
    {  4,  9, 49, 59, 72,103,4294967295u,4294967295u}, // 79
    {  5,  8, 59, 68, 65, 72,4294967295u,4294967295u}, // 80
    {  6,  7, 60, 69, 66, 73,4294967295u,4294967295u}, // 81
    {  7,  6, 69, 77, 58, 66,4294967295u,4294967295u}, // 82
    {  8,  5, 70, 78, 59, 67,4294967295u,4294967295u}, // 83
    {  9,  4, 78,104, 50, 59,4294967295u,4294967295u}, // 84
    {  5,  9, 59, 68, 72,103,4294967295u,4294967295u}, // 85
    {  7,  7, 69, 77, 66, 73,4294967295u,4294967295u}, // 86
    {  9,  5, 78,104, 59, 67,4294967295u,4294967295u}, // 87
    {  6,  9, 68, 76, 79,108,4294967295u,4294967295u}, // 88
    {  9,  6, 84,109, 67, 74,4294967295u,4294967295u}, // 89
    {  7,  9, 76, 83, 79,108,4294967295u,4294967295u}, // 90
    {  9,  7, 84,109, 74, 80,4294967295u,4294967295u}, // 91
    {  8,  9, 83, 87, 85,114,4294967295u,4294967295u}, // 92
    {  9,  8, 87,115, 80, 85,4294967295u,4294967295u}, // 93
    {  0, 10, 21, 29, 94, 98,4294967295u,2147483647u}, // 94
    { 10,  0, 95,101, 27, 34,2147483647u,4294967295u}, // 95
    {  1, 10, 29, 38, 96,102,4294967295u,2147483647u}, // 96
    { 10,  1, 97,105, 34, 42,2147483647u,4294967295u}, // 97
    {  0, 12, 28, 37, 98,106,4294967295u,2147483647u}, // 98
    {  2, 10, 38, 48, 99,107,4294967295u,2147483647u}, // 99
    { 10,  2,100,110, 42, 51,2147483647u,4294967295u}, // 100
    { 12,  0,101,111, 35, 43,2147483647u,4294967295u}, // 101
    {  1, 12, 37, 47,102,112,4294967295u,2147483647u}, // 102
    {  3, 10, 48, 58, 99,107,4294967295u,2147483647u}, // 103
    { 10,  3,100,110, 51, 60,2147483647u,4294967295u}, // 104
    { 12,  1,105,117, 43, 52,2147483647u,4294967295u}, // 105
    {  0, 14, 36, 46,106,118,4294967295u,2147483647u}, // 106
    {  2, 12, 47, 57,107,119,4294967295u,2147483647u}, // 107
    {  4, 10, 58, 67,103,113,4294967295u,2147483647u}, // 108
    { 10,  4,104,116, 60, 68,2147483647u,4294967295u}, // 109
    { 12,  2,110,122, 52, 61,2147483647u,4294967295u}, // 110
    { 14,  0,111,123, 44, 53,2147483647u,4294967295u}, // 111
    {  1, 14, 46, 56,112,124,4294967295u,2147483647u}, // 112
    {  3, 12, 57, 66,107,119,4294967295u,2147483647u}, // 113
    {  5, 10, 67, 75,103,113,4294967295u,2147483647u}, // 114
    { 10,  5,104,116, 68, 75,2147483647u,4294967295u}, // 115
    { 12,  3,110,122, 61, 69,2147483647u,4294967295u}, // 116
    { 14,  1,117,129, 53, 62,2147483647u,4294967295u}, // 117
    {  0, 16, 45, 55,118,138,4294967295u,1073741823u}, // 118
    {  2, 14, 56, 65,119,130,4294967295u,2147483647u}, // 119
    {  4, 12, 66, 74,113,125,4294967295u,2147483647u}, // 120
    { 12,  4,116,128, 69, 76,2147483647u,4294967295u}, // 121
    { 14,  2,122,135, 62, 70,2147483647u,4294967295u}, // 122
    { 16,  0,123,141, 54, 63,1073741823u,4294967295u}, // 123
    {  1, 16, 55, 64,124,142,4294967295u,1073741823u}, // 124
    {  3, 14, 65, 73,119,130,4294967295u,2147483647u}, // 125
    {  5, 12, 74, 81,113,125,4294967295u,2147483647u}, // 126
    { 12,  5,116,128, 76, 82,2147483647u,4294967295u}, // 127
    { 14,  3,122,135, 70, 77,2147483647u,4294967295u}, // 128
    { 16,  1,129,145, 63, 71,1073741823u,4294967295u}, // 129
    {  2, 16, 64, 72,130,146,4294967295u,1073741823u}, // 130
    {  4, 14, 73, 80,125,136,4294967295u,2147483647u}, // 131
    {  6, 12, 81, 86,120,131,4294967295u,2147483647u}, // 132
    { 12,  6,121,134, 82, 86,2147483647u,4294967295u}, // 133
    { 14,  4,128,137, 77, 83,2147483647u,4294967295u}, // 134
    { 16,  2,135,149, 71, 78,1073741823u,4294967295u}, // 135
    {  3, 16, 72, 79,130,146,4294967295u,1073741823u}, // 136
    { 16,  3,135,149, 78, 84,1073741823u,4294967295u}, // 137
    {  0, 20, 94, 96,138,154,4294967295u,1073741823u}, // 138
    {  4, 16, 79, 85,136,150,4294967295u,1073741823u}, // 139
    { 16,  4,137,153, 84, 87,1073741823u,4294967295u}, // 140
    { 20,  0,141,157, 95, 97,1073741823u,4294967295u}, // 141
    {  1, 20, 96, 99,142,158,4294967295u,1073741823u}, // 142
    {  5, 16, 85, 88,136,150,4294967295u,1073741823u}, // 143
    { 16,  5,137,153, 87, 89,1073741823u,4294967295u}, // 144
    { 20,  1,145,159, 97,100,1073741823u,4294967295u}, // 145
    {  2, 20, 99,103,146,160,4294967295u,1073741823u}, // 146
    {  6, 16, 88, 90,139,155,4294967295u,1073741823u}, // 147
    { 16,  6,140,156, 89, 91,1073741823u,4294967295u}, // 148
    { 20,  2,149,161,100,104,1073741823u,4294967295u}, // 149
    {  3, 20,103,108,146,160,4294967295u,1073741823u}, // 150
    {  7, 16, 90, 92,139,155,4294967295u,1073741823u}, // 151
    { 16,  7,140,156, 91, 93,1073741823u,4294967295u}, // 152
    { 20,  3,149,161,104,109,1073741823u,4294967295u}, // 153
    {  0, 24, 98,102,154,164,4294967295u,1073741823u}, // 154
    {  4, 20,108,114,150,162,4294967295u,1073741823u}, // 155
    { 20,  4,153,163,109,115,1073741823u,4294967295u}, // 156
    { 24,  0,157,167,101,105,1073741823u,4294967295u}, // 157
    {  1, 24,102,107,158,168,4294967295u,1073741823u}, // 158
    { 24,  1,159,171,105,110,1073741823u,4294967295u}, // 159
    {  2, 24,107,113,160,172,4294967295u,1073741823u}, // 160
    { 24,  2,161,173,110,116,1073741823u,4294967295u}, // 161
    {  3, 24,113,120,160,172,4294967295u,1073741823u}, // 162
    { 24,  3,161,173,116,121,1073741823u,4294967295u}, // 163
    {  0, 28,106,112,164,176,4294967295u,1073741823u}, // 164
    {  4, 24,120,126,162,174,4294967295u,1073741823u}, // 165
    { 24,  4,163,175,121,127,1073741823u,4294967295u}, // 166
    { 28,  0,167,177,111,117,1073741823u,4294967295u}, // 167
    {  1, 28,112,119,168,178,4294967295u,1073741823u}, // 168
    {  5, 24,126,132,162,174,4294967295u,1073741823u}, // 169
    { 24,  5,163,175,127,133,1073741823u,4294967295u}, // 170
    { 28,  1,171,179,117,122,1073741823u,4294967295u}, // 171
    {  2, 28,119,125,172,180,4294967295u,1073741823u}, // 172
    { 28,  2,173,181,122,128,1073741823u,4294967295u}, // 173
    {  3, 28,125,131,172,180,4294967295u,1073741823u}, // 174
    { 28,  3,173,181,128,134,1073741823u,4294967295u}, // 175
    {  0, 32,118,124,176,190,4294967295u, 268435455u}, // 176
    { 32,  0,177,191,123,129, 268435455u,4294967295u}, // 177
    {  1, 32,124,130,178,192,4294967295u, 268435455u}, // 178
    { 32,  1,179,193,129,135, 268435455u,4294967295u}, // 179
    {  2, 32,130,136,180,194,4294967295u, 268435455u}, // 180
    { 32,  2,181,195,135,137, 268435455u,4294967295u}, // 181
    {  3, 32,136,139,180,194,4294967295u, 268435455u}, // 182
    { 32,  3,181,195,137,140, 268435455u,4294967295u}, // 183
    {  4, 32,139,143,182,196,4294967295u, 268435455u}, // 184
    { 32,  4,183,197,140,144, 268435455u,4294967295u}, // 185
    {  5, 32,143,147,182,196,4294967295u, 268435455u}, // 186
    { 32,  5,183,197,144,148, 268435455u,4294967295u}, // 187
    {  6, 32,147,151,184,198,4294967295u, 268435455u}, // 188
    { 32,  6,185,199,148,152, 268435455u,4294967295u}, // 189
    {  0, 48,154,158,190,200,4294967295u, 268435455u}, // 190
    { 48,  0,191,201,157,159, 268435455u,4294967295u}, // 191
    {  1, 48,158,160,192,202,4294967295u, 268435455u}, // 192
    { 48,  1,193,203,159,161, 268435455u,4294967295u}, // 193
    {  2, 48,160,162,194,204,4294967295u, 268435455u}, // 194
    { 48,  2,195,205,161,163, 268435455u,4294967295u}, // 195
    {  3, 48,162,165,194,204,4294967295u, 268435455u}, // 196
    { 48,  3,195,205,163,166, 268435455u,4294967295u}, // 197
    {  4, 48,165,169,196,206,4294967295u, 268435455u}, // 198
    { 48,  4,197,207,166,170, 268435455u,4294967295u}, // 199
    {  0, 64,176,178,200,212,4294967295u, 134217727u}, // 200
    { 64,  0,201,213,177,179, 134217727u,4294967295u}, // 201
    {  1, 64,178,180,202,214,4294967295u, 134217727u}, // 202
    { 64,  1,203,215,179,181, 134217727u,4294967295u}, // 203
    {  2, 64,180,182,204,216,4294967295u, 134217727u}, // 204
    { 64,  2,205,217,181,183, 134217727u,4294967295u}, // 205
    {  3, 64,182,184,204,216,4294967295u, 134217727u}, // 206
    { 64,  3,205,217,183,185, 134217727u,4294967295u}, // 207
    {  4, 64,184,186,206,218,4294967295u, 134217727u}, // 208
    { 64,  4,207,219,185,187, 134217727u,4294967295u}, // 209
    {  5, 64,186,188,206,218,4294967295u, 134217727u}, // 210
    { 64,  5,207,219,187,189, 134217727u,4294967295u}, // 211
    {  0, 96,190,192,212,220,4294967295u, 134217727u}, // 212
    { 96,  0,213,221,191,193, 134217727u,4294967295u}, // 213
    {  1, 96,192,194,214,222,4294967295u, 134217727u}, // 214
    { 96,  1,215,223,193,195, 134217727u,4294967295u}, // 215
    {  2, 96,194,196,216,224,4294967295u, 134217727u}, // 216
    { 96,  2,217,225,195,197, 134217727u,4294967295u}, // 217
    {  3, 96,196,198,216,224,4294967295u, 134217727u}, // 218
    { 96,  3,217,225,197,199, 134217727u,4294967295u}, // 219
    {  0,128,200,202,220,230,4294967295u,  33554431u}, // 220
    {128,  0,221,231,201,203,  33554431u,4294967295u}, // 221
    {  1,128,202,204,222,232,4294967295u,  33554431u}, // 222
    {128,  1,223,233,203,205,  33554431u,4294967295u}, // 223
    {  2,128,204,206,224,234,4294967295u,  33554431u}, // 224
    {128,  2,225,235,205,207,  33554431u,4294967295u}, // 225
    {  3,128,206,208,224,234,4294967295u,  33554431u}, // 226
    {128,  3,225,235,207,209,  33554431u,4294967295u}, // 227
    {  4,128,208,210,226,236,4294967295u,  33554431u}, // 228
    {128,  4,227,237,209,211,  33554431u,4294967295u}, // 229
    {  0,256,220,222,230,238,4294967295u,  16777215u}, // 230
    {256,  0,231,239,221,223,  16777215u,4294967295u}, // 231
    {  1,256,222,224,232,240,4294967295u,  16777215u}, // 232
    {256,  1,233,241,223,225,  16777215u,4294967295u}, // 233
    {  2,256,224,226,234,242,4294967295u,  16777215u}, // 234
    {256,  2,235,243,225,227,  16777215u,4294967295u}, // 235
    {  3,256,226,228,234,242,4294967295u,  16777215u}, // 236
    {256,  3,235,243,227,229,  16777215u,4294967295u}, // 237
    {  0,512,230,232,238,238,4294967295u,         0u}, // 238
    {512,  0,239,239,231,233,         0u,4294967295u}, // 239
    {  1,512,232,234,240,240,4294967295u,         0u}, // 240
    {512,  1,241,241,233,235,         0u,4294967295u}, // 241
    {  2,512,234,236,242,242,4294967295u,         0u}, // 242
    {512,  2,243,243,235,237,         0u,4294967295u}  // 243
};

/* Hashtable<T, N, M> is a hash table of 2^N elements of type T
(derived from HashElement) with linear search
of M elements in case of collision.  If all elements collide,
then the one with the lowest .priority() is replaced.  
Hashtable[i] returns a T& indexed by the lower bits of i whose
checksum matches the upper bits of i, creating or replacing if needed.
*/

template<class T, int N, int M=3>
class Hashtable {
private:
  T* table;  // Array of 2^N+M elements
  Hashtable(const Hashtable&);  // No copy
  Hashtable& operator=(const Hashtable&);  // No assignment
public:
  Hashtable(): table(new Counter[(1<<N)+M]) {}
  T& operator[](U32 i);
  ~Hashtable() {delete[] table;}
};

template<class T, int N, int M>
inline T& Hashtable<T, N, M>::operator[](U32 i) {
  const U32 lb=i&((1<<N)-1);  // Lower bound of search range
  const U32 ub=lb+M;  // Upper bound + 1
  const int checksum=i>>24; 
  int bj=lb;
  int bget=1000000000;
  for (U32 j=lb; j<ub; ++j) {
    T& c=table[j];
    int g=c.priority();
    if (g==0)
      return c=T(checksum);
    if (checksum==c.checksum())
      return c;
    if (g<bget) {
      bget=g;
      bj=j;
    }
  }
  return table[bj]=T(checksum);
}

/* A NonstationaryPPM model guesses the next bit by finding all
matching contexts of n = 1 to 8 bytes (including the last partial
byte of 0-7 bits) and guessing for each match that the next bit
will be the same with weight n^2/f(age).  The function f(age) decays
the count of 0s or 1s for each context by half whenever there are
more than 2 and the opposite bit is observed.  This is an approximation
of the nonstationary model, weight = 1/(t*variance) where t is the
number of subsequent observations and the variance is tp(1-p) for
t observations and p the probability of a 1 bit given the last t
observations.  The aged counts are stored in a hash table of
contexts.  */

class NonstationaryPPM: public Model {
  enum {N=8};  // Number of contexts
  int c0;  // Current 0-7 bits of input with a leading 1
  int c1;  // Previous whole byte
  int cn;  // c0 mod 53 (low bits of hash)
  vector<Counter> counter0;  // Counters for context lengths 0 and 1
  vector<Counter> counter1;
  Hashtable<Counter, 18+MEM> counter2;  // for lengths 2 to N-1
  Counter *cp[N];  // Pointers to current counters
  U32 hash[N];   // Hashes of last 0 to N-1 bytes
public:
  inline void predict(int& c0, int& c1) const;  // Add to counts of 0s and 1s
  inline void update(int y);   // Append bit y (0 or 1) to model
  NonstationaryPPM();
  int getc0() const {return c0;}
};

NonstationaryPPM::NonstationaryPPM(): c0(1), c1(0), cn(1),
     counter0(256), counter1(65536) {
  for (int i=0; i<N; ++i) {
    cp[i]=&counter0[0];
    hash[i]=0;
  }
}

void NonstationaryPPM::predict(int& n0, int& n1) const {
  for (int i=0; i<N; ++i) {
    const int wt=(i+1)*(i+1);
    n0+=cp[i]->get0()*wt;
    n1+=cp[i]->get1()*wt;
  }
}

// Add bit y (0 or 1) to model
void NonstationaryPPM::update(int y) {

  // Count y by context
  for (int i=N-1; i>=0; --i) {
    cp[i]->add(y);
    if ((cp[i]->get0()+cp[i]->get1())*i > 100) {  // Update exclusion
      cp[i]->add(y);
      break;
    }
  }

  // Store bit y
  cn+=cn+y;
  if (cn>=53) cn-=53;
  c0+=c0+y;
  if (c0>=256) {  // Start new byte
    for (int i=N-1; i>0; --i)
      hash[i]=(hash[i-1]+c0)*987660757;
    c1=c0-256;
    c0=1;
    cn=1;
  }

  // Set up pointers to next counters
  cp[0]=&counter0[c0];
  cp[1]=&counter1[c0+(c1<<8)];
  for (int i=2; i<N; ++i)
    cp[i]=&counter2[hash[i]+cn+(c0<<24)];
}

/* A MatchModel looks for a match of length n = 8 bytes or more between
the current context and previous input and guesses that the next bit
will be the same with weight 3n^2.  Matches are found in a 4MB rotating
buffer using a 1M hash table of pointers. */

class MatchModel: public Model {
  vector<U8> buf;   // Input buffer, wraps at end
  vector<U24> ptr;  // Hash table of pointers
  U32 hash;         // Hash of current context up to pos-1
  int pos;          // Element of buf where next bit will be stored
  int bpos;         // Number of bits (0-7) stored at buf[pos]
  int begin;        // Points to first matching byte (does not wrap)
  int end;          // Points to last matching byte + 1, 0 if no match
public:
  MatchModel(): buf(0x10000*(1<<MEM)), ptr(0x4000*(1<<MEM)),
    hash(0), pos(0), bpos(0), begin(0), end(0) {}
  void predict(int& n0, int& n1) const {
    if (end) {
      int wt=end-begin;
      if (wt>1000)
        wt=3000000;
      else
        wt*=wt*3;
      if ((buf[end]>>(7-bpos))&1)
        n1+=wt;
      else
        n0+=wt;
    }
  }

  // Append bit y to buf and check that it matches at the end.
  // After a byte is completed, compute a new hash and store it.
  // If there is no current match, search for one.
  void update(int y) {
    (buf[pos]<<=1)+=y;  // Store bit
    ++bpos;
    if (end && (buf[end]>>(8-bpos))!=buf[pos])  // Does it match?
      begin=end=0;  // no
    if (bpos==8) {  // New byte
      bpos=0;
      hash=hash*(16*123456791)+buf[pos]+1;
      if (++pos==int(buf.size()))
        pos=0;
      if (end)
        ++end;
      else {  // If no match, search for one
        U32 h=(hash^(hash>>16))&(ptr.size()-1);
        end=ptr[h];
        if (end>0) {
          begin=end;
          int p=pos;
          while (begin>0 && p>0 && begin!=p+1 && buf[begin-1]==buf[p-1]) {
            --begin;
            --p;
          }
        }
        if (end==begin)  // No match found
          begin=end=0;
        ptr[h]=U24(pos);
      }
    }
  }
};

/* A WordModel predicts in the context of whole words (a-z, case
insensitive) using a trigram model. */

class WordModel {
private:
  U32 word1, word0;  // Hash of previous and current word
  int ww1, ww0;  // Word weights (lengths)
  Hashtable<Counter, 16+MEM> t1;  // Model
  int c1;  // Previous char, lower case
  int c0;  // 0-7 bits of current char with leading 1 bit
  int c0h; // Small hash of c0
  Counter *cp0, *cp1;  // Points into t1 current context
  int lettercount;  // For detecting English
  enum {THRESHOLD=5};  // lettercount threshold for English
public:
  WordModel(): word1(0), word0(0), ww1(0), ww0(0),
      c1(0), c0(1), c0h(1), cp0(&t1[0]), cp1(&t1[0]), lettercount(0) {}

  void predict(int& n0, int& n1) const {
    if (lettercount>=THRESHOLD) {
      const int wt0=(ww0+1)*(ww0+1);
      n0+=cp0->get0()*wt0;
      n1+=cp0->get1()*wt0;
      const int wt1=(ww0+ww1+2)*(ww0+ww1+2);
      n0+=cp1->get0()*wt1;
      n1+=cp1->get1()*wt1;
    }
  }

  void update(int y) {
    if (lettercount>=THRESHOLD) {
      cp1->add(y);
      if (cp1->get0()+cp1->get1() < 100)
        cp0->add(y);
    }

    // Update contexts with next bit
    c0+=c0+y;
    c0h+=c0h+y;
    if (c0h>=59) c0h-=59;
    if (c0>=256) {
      c0-=256;
      if (lettercount>0 &&
          (c0>127 || (c0<32 && c0!='\n' && c0!='\r' && c0!='\t'))) 
        --lettercount;
      if (isalpha(c0)) {
        word0=(word0+tolower(c0)+1)*234577751*16;
        if (++ww0>8) ww0=8;
        if (lettercount<THRESHOLD*2)
          ++lettercount;
      }
      else if (word0) {
        ww1=ww0;
        ww0=0;
        word1=word0;
        word0=0;
      }
      c1=c0;
      c0=1;
      c0h=1;
    }

    // Set up pointers to new contexts
    if (lettercount>=THRESHOLD) {
      U32 h=word0*123456791+c1*345689647+c0h+(c0<<24);
      cp0=&t1[h];
      cp1=&t1[word1+h];
    }
  }
};

/* CyclicModel models data with fixed length records such as tables,
databases, images, audio, binary numeric data, etc.  It models
runs of 0s or 1s in bit columns.  A table is detected when an 8-bit
pattern occurs 4 or more times in a row spaced by the same interval.  The
table ends after no repetition detection for the same number of bits
as were in the table. */

class CyclicModel: public Model {
  struct E {
    int p, n, r;  // Position of last match, number of matches, interval
    E(): p(0), n(0), r(0) {}
  };
  vector<E> cpos;  // Table of repeat patterns by char
  int pos;  // Current bit position in input
  int c0;  // Last 8 bits
  int cycle;  // Most likely number of columns
  int column;  // Column number, 0 to cycle-1
  int size;  // Number of bits before the table expires, 0 to 3*cycle
  Hashtable<Counter, 15> t;  // Context is last 8 bits in column
  vector<Counter> t1;  // Context is the column number only
  Counter *cp, *cp1;  // Points to t, t1
public:
  CyclicModel(): cpos(256), pos(0), c0(1), cycle(0), column(0), size(0),
                 t1(2048), cp(&t[0]), cp1(&t1[0]) {}

  void predict(int& n0, int& n1) const {
    if (cycle>0) {
      int wt=16;
      n0+=cp->get0()*wt;
      n1+=cp->get1()*wt;
      wt=4;
      n0+=cp1->get0()*wt;
      n1+=cp1->get1()*wt;
    }
  }

  void update(int y) {
    if (++column>=cycle) column=0;
    if (size>0 && --size==0) cycle=0;
    cp->add(y);
    cp1->add(y);
    c0=(c0+c0+y)&0xff;
    ++pos;
    E& e=cpos[c0];
    if (e.p+e.r==pos) {
      ++e.n;
      if (e.n>3 && e.r>8 && e.r*e.n>size) {
        size=e.r*e.n;
        if (cycle!=e.r) {
          cycle=e.r;
          column=pos%cycle;
        }
      }
    }
    else {
      e.n=1;
      e.r=pos-e.p;
    }
    e.p=pos;
    int h=column*3+c0*876546821;
    cp=&t[h];
    cp1=&t1[column&2047];
  }
};

// Secondary source encoder
struct SSEContext {
  U8 c1, n;  // Count of 1's, count of bits
  U16 p() const {return U32(65535)*(c1*64+1)/(n*64+2);}
  void update(int y) {
    if (y)
      ++c1;
    if (++n>254) {
      c1/=2;
      n/=2;
    }
  }
};

const int SSE1=256*3*3*3*3, SSE2=32; // dimensions
const int SSESCALE=1024/SSE2;
SSEContext sse[SSE1][SSE2+1];  // [context][mapped probability]

// Scale probability p (0 to 64K-1) into a context in the range 0 to 1K-1
class SSEMap {
  enum {N=4096};
  U16 table[N];
public:
  int operator()(int p) const {return table[p/(65536/N)];}
  SSEMap();
} ssemap;  // global functoid

SSEMap::SSEMap() {
  for (int i=0; i<N; ++i) {
    int p=int(64*log((i+0.5)/(N-0.5-i))+512);
    if (p>1023) p=1023;
    if (p<0) p=0;
    table[i]=p;
  }
}

/* A Predictor predicts the next bit given the bits so far using a
collection of models.  Methods:

   p() returns probability of a 1 being the next bit, P(y = 1)
     as a 16 bit number (0 to 64K-1).
   update(y) updates the models with bit y (0 or 1)
*/

class Predictor {
  NonstationaryPPM m1;
  MatchModel m2;
  WordModel m3;
  CyclicModel m4;
  mutable int context, ssep; 
public:
  Predictor();
  ~Predictor();
  U16 p() const {
    int n0=1, n1=n0;
    context=m1.getc0();
    m4.predict(n0, n1); context=context*3+(n0*2>n1)+(n0>n1*2);
    m2.predict(n0, n1); context=context*3+(n0*4>n1)+(n0>n1*4);
    m3.predict(n0, n1); context=context*3+(n0*8>n1)+(n0>n1*8);
    m1.predict(n0, n1); context=context*3+(n0*16>n1)+(n0>n1*16);
    int n=n0+n1;
    while (n>32767) {
      n1/=16;
      n/=16;
    }
    U16 pr=65535*n1/n;
    ssep=ssemap(pr);
    int wt=ssep%SSESCALE;
    int i=ssep/SSESCALE;
    return (((sse[context][i].p()*(SSESCALE-wt)+sse[context][i+1].p()*wt)
      /SSESCALE)*3+pr)/4;
  }
  void update(int y) {
    m1.update(y);
    m2.update(y);
    m3.update(y);
    m4.update(y);
    sse[context][ssep/SSESCALE].update(y);
    sse[context][ssep/SSESCALE+1].update(y);
  }
};

Predictor::Predictor() {
  int N=4096;
  int oldp=SSE2+1;
  for (int i=N-1; i>=0; --i) {
    int p=(ssemap(i*65536/N)+SSESCALE/2)/SSESCALE;
    int n=1+N*N/((i+1)*(N-i));
    if (n>254) n=254;
    int c1=(i*n+N/2)/N;
    for (int j=oldp-1; j>=p; --j) {
      for (int k=0; k<SSE1; ++k) {
        sse[k][j].n=n;
        sse[k][j].c1=c1;
      }
    }
    oldp=p;
  }
}


Predictor::~Predictor() {
/*
  for (int i=0; i<SSE1; ++i) {
    for (int j=0; j<SSE2; ++j) {
      printf("%d/%d ", sse[i][j].c1, sse[i][j].n);
    }
    printf("\n");
  } */
}

/* An Encoder does arithmetic encoding.  Methods:
   Encoder(COMPRESS, f) creates encoder for compression to archive f, which
     must be open for writing in binary mode
   Encoder(DECOMPRESS, f) creates encoder for decompression from archive f,
     which must be open for reading in binary mode
   encode(bit) in COMPRESS mode compresses bit to file f.
   decode() in DECOMPRESS mode returns the next decompressed bit from file f.
   flush() should be called when there is no more to compress
*/

typedef enum {COMPRESS, DECOMPRESS} Mode;
class Encoder {
private:
  Predictor predictor;
  const Mode mode;       // Compress or decompress?
  FILE* archive;         // Compressed data file
  U32 x1, x2;            // Range, initially [0, 1), scaled by 2^32
  U32 x;                 // Last 4 input bytes of archive.
public:
  Encoder(Mode m, FILE* f);
  void encode(int bit);
  int decode();
  void flush();
};

// Constructor
Encoder::Encoder(Mode m, FILE* f): predictor(), mode(m), archive(f), x1(0),
                                   x2(0xffffffff), x(0) {

  // In DECOMPRESS mode, initialize x to the first 4 bytes of the archive
  if (mode==DECOMPRESS) {
    for (int i=0; i<4; ++i) {
      int c=getc(archive);
      x=(x<<8)+(c&0xff);
    }
  }
}

/* encode(y) -- Encode bit y by splitting the range [x1, x2] in proportion
to P(1) and P(0) as given by the predictor and narrowing to the appropriate
subrange.  Output leading bytes of the range as they become known.
*/
inline void Encoder::encode(int y) {

  // Split the range
  const U32 p=predictor.p(); // Probability P(1) * 64K rounded down
  const U32 xdiff=x2-x1;
  U32 xmid=x1;  // = x1+p*(x2-x1) multiply without overflow, round down
  if (xdiff>=0x10000000) xmid+=(xdiff>>16)*p;
  else if (xdiff>=0x1000000) xmid+=((xdiff>>12)*p)>>4;
  else if (xdiff>=0x100000) xmid+=((xdiff>>8)*p)>>8;
  else if (xdiff>=0x10000) xmid+=((xdiff>>4)*p)>>12;
  else xmid+=(xdiff*p)>>16;

  // Update the range
  if (y)
    x2=xmid;
  else
    x1=xmid+1;
  predictor.update(y);

  // Shift equal MSB's out
  while (((x1^x2)&0xff000000)==0) {
    putc(x2>>24, archive);
    x1<<=8;
    x2=(x2<<8)+255;
  }
}

/* Decode one bit from the archive, splitting [x1, x2] as in the encoder
and returning 1 or 0 depending on which subrange the archive point x is in.
*/
inline int Encoder::decode() {

  // Split the range
  const U32 p=predictor.p(); // Probability P(1) * 64K rounded down
  const U32 xdiff=x2-x1;
  U32 xmid=x1;  // = x1+p*(x2-x1) multiply without overflow, round down
  if (xdiff>=0x10000000) xmid+=(xdiff>>16)*p;
  else if (xdiff>=0x1000000) xmid+=((xdiff>>12)*p)>>4;
  else if (xdiff>=0x100000) xmid+=((xdiff>>8)*p)>>8;
  else if (xdiff>=0x10000) xmid+=((xdiff>>4)*p)>>12;
  else xmid+=(xdiff*p)>>16;

  // Update the range
  int y=0;
  if (x<=xmid) {
    y=1;
    x2=xmid;
  }
  else
    x1=xmid+1;
  predictor.update(y);

  // Shift equal MSB's out
  while ((x1>>24)==(x2>>24)) {
    x1<<=8;
    x2=(x2<<8)+255;
    x=(x<<8)+getc(archive);
  }
  return y;
}

// Should be called when there is no more to compress
void Encoder::flush() {

  // In COMPRESS mode, write out the remaining bytes of x, x1 < x < x2
  if (mode==COMPRESS) {
    while (((x1^x2)&0xff000000)==0) {
      putc(x2>>24, archive);
      x1<<=8;
      x2=(x2<<8)+255;
    }
    putc(x2>>24, archive);  // First unequal byte
  }
}

// Read one byte from encoder e
int decompress(Encoder& e) {  // Decompress 8 bits, MSB first
  int c=0;
  for (int i=0; i<8; ++i)
    c=c+c+e.decode();
  return c;
}

// Write one byte c to encoder e
void compress(Encoder& e, int c) {
  for (int i=7; i>=0; --i)
    e.encode((c>>i)&1);
}

// Fail if out of memory
void handler() {
  printf("Out of memory\n");
  exit(1);
}

// Read and return a line of input from FILE f (default stdin) up to
// first control character except tab.  Skips CR in CR LF.
/*string getline(FILE* f=stdin) {
  int c;
  string result="";
  while ((c=getc(f))!=EOF && (c>=32 || c=='\t'))
    result+=char(c);
  if (c=='\r')
    (void) getc(f);
  return result;
}*/

// User interface
/*int main(int argc, char** argv) {
  set_new_handler(handler);

  // Check arguments
  if (argc<2) {
    printf(
      "PAQ3 file compressor/archiver, (C) 2003, Matt Mahoney, mmahoney@cs.fit.edu\n"
      "This program is free software distributed without warranty under the terms\n"
      "of the GNU General Public License, see http://www.gnu.org/licenses/gpl.txt\n"
      "\n"
      "To compress:         PAQ3 archive filenames...  (archive will be created)\n"
      "  or (MSDOS):        dir/b | PAQ3 archive  (reads file names from input)\n"
      "To extract/compare:  PAQ3 archive  (does not clobber existing files)\n"
      "To view contents:    more < archive\n");
    return 1;
  }

  // File names and sizes from input or archive
  vector<string> filename; // List of names
  vector<long> filesize;   // Size or -1 if error
  int start_time=clock();
  int uncompressed_bytes=0, compressed_bytes=0;  // Input, output sizes

  // Extract files
  FILE* archive=fopen(argv[1], "rb");
  if (archive) {
    if (argc>2) {
      printf("File %s already exists\n", argv[1]);
      return 1;
    }
    printf("Extracting archive %s ...\n", argv[1]);

    // Read "PAQ3\r\n" at start of archive
    if (getline(archive) != "PAQ3") {
      printf("Archive file %s not in PAQ3 format\n", argv[1]);
      return 1;
    }

    // Read "size filename" in "%d\t%s\r\n" format
    while (true) {
      string s=getline(archive);
      if (s.size()>1) {
        filesize.push_back(atol(s.c_str()));
        string::iterator tab=find(s.begin(), s.end(), '\t');
        if (tab!=s.end())
          filename.push_back(string(tab+1, s.end()));
        else
          filename.push_back("");
      }
      else
        break;
    }

    // Test end of header for "\f\0"
    {
      int c1=0, c2=0;
      if ((c1=getc(archive))!='\f' || (c2=getc(archive))!=0) {
        printf("%s: Bad PAQ3 header format %d %d\n", argv[1],
          c1, c2);
        return 1;
      }
    }

    // Extract files from archive data
    Encoder e(DECOMPRESS, archive);
    for (int i=0; i<int(filename.size()); ++i) {
      printf("%10ld %s: ", filesize[i], filename[i].c_str());

      // Compare with existing file
      FILE* f=fopen(filename[i].c_str(), "rb");
      const long size=filesize[i];
      uncompressed_bytes+=size;
      if (f) {
        bool different=false;
        for (long j=0; j<size; ++j) {
          int c1=decompress(e);
          int c2=getc(f);
          if (!different && c1!=c2) {
            printf("differ at offset %ld, archive=%d file=%d\n",
              j, c1, c2);
            different=true;
          }
        }
        if (!different)
          printf("identical\n");
        fclose(f);
      }

      // Extract to new file
      else {
        f=fopen(filename[i].c_str(), "wb");
        if (!f)
          printf("cannot create, skipping...\n");
        for (long j=0; j<size; ++j) {
          int c=decompress(e);
          if (f)
            putc(c, f);
        }
        if (f) {
          printf("extracted\n");
          fclose(f);
        }
      }
    }
    compressed_bytes=ftell(archive);
    fclose(archive);
  }

  // Compress files
  else {

    // Read file names from command line or input
    if (argc>2)
      for (int i=2; i<argc; ++i)
        filename.push_back(argv[i]);
    else {
      printf(
        "Enter names of files to compress, followed by blank line or EOF.\n");
      while (true) {
        string s=getline(stdin);
        if (s=="")
          break;
        else
          filename.push_back(s);
      }
    }

    // Get file sizes
    for (int i=0; i<int(filename.size()); ++i) {
      FILE* f=fopen(filename[i].c_str(), "rb");
      if (!f) {
        printf("File not found, skipping: %s\n",
          filename[i].c_str());
        filesize.push_back(-1);
      }
      else {
        fseek(f, 0L, SEEK_END);
        filesize.push_back(ftell(f));
        fclose(f);
      }
    }
    if (filesize.empty() || *max_element(filesize.begin(), filesize.end())<0){
      printf("No files to compress, no archive created.\n");
      return 1;
    }

    // Write header
    archive=fopen(argv[1], "wb");
    if (!archive) {
      printf("Cannot create archive: %s\n", argv[1]);
      return 1;
    }
    fprintf(archive, "PAQ3\r\n");
    for (int i=0; i<int(filename.size()); ++i) {
      if (filesize[i]>=0)
        fprintf(archive, "%ld\t%s\r\n", filesize[i], filename[i].c_str());
    }
    putc(032, archive);  // MSDOS EOF
    putc('\f', archive);
    putc(0, archive);

    // Write data
    Encoder e(COMPRESS, archive);
    long file_start=ftell(archive);
    for (int i=0; i<int(filename.size()); ++i) {
      const long size=filesize[i];
      if (size>=0) {
        uncompressed_bytes+=size;
        printf("%-23s %10ld -> ", filename[i].c_str(), size);
        FILE* f=fopen(filename[i].c_str(), "rb");
        int c;
        for (long j=0; j<size; ++j) {
          if (f)
            c=getc(f);
          else
            c=0;
          compress(e, c);
        }
        if (f)
          fclose(f);
        printf("%ld\n", ftell(archive)-file_start);
        file_start=ftell(archive);
      }
    }
    e.flush();
    compressed_bytes=ftell(archive);
    fclose(archive);
  }

  // Report statistics
  const double elapsed_time=double(clock()-start_time)/CLOCKS_PER_SEC;
  printf("%d/%d in %1.2f sec.", compressed_bytes, uncompressed_bytes,
    elapsed_time);
  if (uncompressed_bytes>0 && elapsed_time>0) {
    printf(" (%1.4f bpc, %1.2f%% at %1.0f KB/s)",
      compressed_bytes*8.0/uncompressed_bytes,
      compressed_bytes*100.0/uncompressed_bytes,
      uncompressed_bytes/(elapsed_time*1000.0));
  }
  printf("\n");
  return 0;
}*/

extern "C" __declspec(dllexport) void kgbcompress(FILE *, char *, unsigned __int64);
extern "C" __declspec(dllexport) void setMode(char);
extern "C" __declspec(dllexport) void kgbdecompress(FILE *, char *, unsigned __int64);

Encoder *e = NULL;

void kgbcompress(FILE *archive, char *data, unsigned __int64 size){
	if(e == NULL)
		e = new Encoder(COMPRESS, archive);

	/*for(unsigned __int64 i=0;i<size;i++){
		compress(*e, data[i]);
	}*/
	char *last = &data[size];
	for(;data!=last;data++){
		compress(*e, *data);
	}
}

void setMode(char mode){
	//ignored
}

void kgbdecompress(FILE *archive, char *data, unsigned __int64 size){
	/*static Encoder e(DECOMPRESS, archive);

	for(unsigned __int64 i=0;i<size;i++){
		data[i] = decompress(e);
	}*/
	if(e == NULL)
		e = new Encoder(DECOMPRESS, archive);

	/*for(unsigned __int64 i=0;i<size;i++){
		data[i] = decompress(*e);
	}*/
	char *last = &data[size];
	for(;data!=last;data++){
		*data = decompress(*e);
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call){
		case DLL_PROCESS_DETACH: if(e != NULL) e->flush(); break;
	}
	
	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

