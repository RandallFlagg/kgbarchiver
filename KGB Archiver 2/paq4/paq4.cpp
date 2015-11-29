// paq4.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

/* PAQ4 v2 - File archiver and compressor.
(C) 2003, Matt Mahoney, mmahoney@cs.fit.edu

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation at
http://www.gnu.org/licenses/gpl.txt or (at your option) any later version.
This program is distributed without any warranty.


USAGE

To compress:      PAQ4 archive file file...  (1 or more file names), or
  or (MSDOS):     dir/b | PAQ4 archive       (read file names from input)
  or (UNIX):      ls    | PAQ4 archive
To decompress:    PAQ4 archive
To list contents: more < archive

Compression:  The files listed are compressed and stored in the archive,
which is created.  The archive must not already exist.  File names may
specify a path, which is stored.  If there are no file names on the command
line, then PAQ4 prompts for them, reading until the first blank line or
end of file.

Decompression:  No file names are specified.  The archive must exist.
If a path is stored, the file is extracted to the appropriate directory,
which must exist.  PAQ4 does not create directories.  If the file to be
extracted already exists, it is not replaced; rather it is compared with
the archived file, and the offset of the first difference is reported.

It is not possible to add, remove, or update files in an existing archive.
If you want to do this, extract the files, delete the archive, and
create a new archive with just the files you want.


TO COMPILE

gxx -O paq4.cpp        DJGPP 2.95.2  (gives fastest executable)
bcc32 -O2 paq4.cpp     Borland 5.5.1
sc -o paq4.cpp         Digital Mars 8.35n


FILE FORMAT

A PAQ4 archive has the following format.  The first 6 bytes are
"PAQ4\r\n" followed by a list of file sizes and names in
"%ld\t%s\r\n" format, e.g. 768771<TAB>book1<CR><LF>.  The last filename
is followed by "\x1A\f\0" (<MSDOS-EOF><FORMFEED><NUL>) and the
compressed data.

The compressor uses predictive arithmetic coding.  The compressed data
is a base 256 represenation of a number x (e.g. the first byte has
weight 1/256, second is 1/65536, etc.) such that x has as few digits as
possible and satisfies P(s > y) <= x < P(s >= y), where y is the concatenated
files, s is a random string with probability distribution P, and (s > y)
means that s follows y in a lexicographical ordering over strings.
As y is read in, the bounds of possible x narrow, allowing the leading
digits to be written as they become known.  Computation of x
is approximated using integer arithmetic, which expands the
data less than 0.001% over the Shannon limit of log2 1/P(y) bits.

Prediction (estimation of P(y)) is independent of coding.  The model
by which P is estimated is built from the previous uncompressed data
regardless of whether it is being input or output, so the model
is identical for compression and decompression.  The model consists
of an adaptive weighting of submodels, followed by context sensitive
probability adjustment using SSE (secondary symbol estimation).  It
differs from PAQ3 and PAQ3N that the weighting of models is adaptive
rather than fixed.  The cyclic model for fixed length records has also
been improved.

There are N = 19 submodels, which consist of counts of 0 and 1 bits that
appear in different contexts.  These are combined to a probability P(1)
that the next bit of input will be a 1 as follows:

  P(1) = SUM(i=1..N) wi (n1i / ni)

where wi is the i'th weight (determined adaptively), n1i is the number
of 1 bits observed in the i'th context, and ni = n0i + n1i is the
total number of times the i'th context was observed.

The weights wi are adjusted after each bit of uncompressed data to
reduce the cost (code length) of that bit.  The adjustment is along
the gradient of the cost in weight space, which is

  wi := wi + e[nyi/(SUM(j) (wj+wo) nyj) - ni/(SUM(j) (wj+wo) nj)]

where e and wo are small constants, and nyi is the count for bit y (0 or 1)
in the i'th context.  The weight offset wo prevents the gradient from
going to infinity as the weights go to 0.

The weight vector is context sensitive.  There are 8 vectors which
are trained and used depending on the context of the 3 high order
bits of the previous whole byte.

In order to favor recent data over older data, counts are discarded
in a way that gives higher weights to probabilities near 0 or 1.
After bit y is observed, the count ny is incremented, but half of the
opposite count over 2 is discarded.  For example, if n0=3, n1=10, then
after a 0 is observed, then half of the 8 bits over 2 in n1 are discarded,
so n0=4, n1=6.

The 19 submodels are as follows.  The current 0-7 bits of the partially
compressed byte is always included in the context except in the fixed
model.
- Fixed (n0 = n1 = 1)
- Order 0.  The context is the 0-7 bits of the current byte.
- Order 1.  The context is the last byte plus the current 0-7 bits.
- Order 2.  The context is the last 2 bytes plus the current 0-7 bits.
- Order 3.
- Order 4.
- Order 5.
- Order 6.
- Order 7.
- Word.  The context is the last whole word, defined as the last 0 to 8
  contiguous bytes with values of 33 or more, plus the current bits.
- Sparse order 2, skipping the next to last byte (x.x)
- Sparse order 2, skipping 2 bytes (x..x), the 4th to last and last bytes.
- Sparse order 2, the 2nd and 4th last bytes (x.x.)
- Sparse order 2, the 4th and 8th last bytes (x...x...)
- Sparse order 2, the 2nd and 3rd last (xx.)
- Sparse order 2, the 3rd and 4th last (xx..)
- A long string model consisting of the last match of 8 or more bytes.
- A fixed record model consisting of the two bytes above (as a table)
- A fixed record model consisting of the byte above and to the left.

Counts for the order 2-7 and word models share a 32M element hash table,
where both counts are represented by an 8 bit state.  Hash collisions
are detected (usually) by using an 8 bit checksum of the context,
with linear search over 3 buckets.  If all 3 are full, then the element
with the lowest priority is replaced,  where priority = n0 + n1.
The order 0 and 1 models use unhashed tables.  The sparse order 2 models
share a 4M hash table.  The two fixed record models share a 2M hash table

The long string model uses a 1M hash table to index a 4MB rotating buffer
with 8 and 32 bute hashes to find the most recent context match of
8 bytes or more.  The count is set to nu = the length of the
match, where y is the bit that occured in that context, and the other
count to 0.  For example, given "abcdefghij1...abcdefghij" then
n0=0, n1=10.

The fixed record model determines the record size (table width) by
searching for 4 consecutive appearances of the same character equally
spaced with an interval of at least 3.  For instance, if the
string "a..a..a..a" is observed, then the record length is set to 3.
It remains set until another sequence is observed.

The probability P(1) calculated by the weighted sum of models is
further adjusted by SSE in the context of the current
0-7 bits of the partially compressed byte and the 2 high order bits of
the previous byte (1024 contexts).  In each SSE context, P(1) is divided
into 32 bins, and the actual counts of 0s and 1s observed in each bin
are used to compute the new probability.  When P(1) falls between bins,
the counts in the bins on both sides are incremented, and the output
probability is interpolated between the bins.  The bin spacing is
closer together for output probabilities near 0 or 1.  The counts are
not discounted to favor recent data as in the 19 submodels.  Instead,
when the counters (8 bits) overflow, both n0 and n1 are halved.  The
SSE mapping is initialized to SSE(P) = P.  The final probability Pf
is then averaged to Pf = (3 SSE(P) + P) / 4.

Thanks to Serge Osnach for introducing me to SSE (in PAQ1SSE/PAQ2) and
the sparse models (PAQ3N).  Also, credit to Eugene Shelwein,
Dmitry Shkarin for suggestions on using multiple character SSE contexts.
Credit to Eugene, Serge, and Jason Schmidt for developing faster and
smaller executables of previous versions.  Credit to Werner Bergmans
and Berto Destasio for testing and evaluating them, including modifications
that improve compression at the cost of more memory.

I expect there will be better versions in the future.  If you make any
changes, please change the name of the program (e.g. PAQ5), including
the string in the archive header by redefining PROGNAME below.
This will prevent any confusion about versions or archive compatibility.
Also, give yourself credit in the help message.
*/

#define PROGNAME "PAQ4"  // Please change this if you change the program

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <ctime>
#include <cassert>
#include <new>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

const int PSCALE=2048;  // Integer scale for representing probabilities
const int MEM=8;        // 6 = 88 MB, increase by 1 to double it

template <class T> inline int size(const T& t) {return t.size();}

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

Random::Random(): i(0) {  // Seed the table
  table[0]=123456789;
  table[1]=987654321;
  for (int j=2; j<55; ++j)
    table[j]=table[j-1]*11+table[j-2]*19/16;
}


/* 3 byte counter, shown for reference only.  It implements a
nonstationary pair of counters of 0s and 1s such that preference is
given to recent history by discarding old bits. */

class Counter3 {
  U8 n[2];  // n[y] is the counts of ys (0s or 1s)
public:
  Counter3() {n[0]=n[1]=0;}
  int get0() const {return n[0];}  // Return count of 0s
  int get1() const {return n[1];}  // Return count of 1s
  int priority() const {return get0()+get1();}  // For hash replacement
  void add(int y) {  // Add y (0 or 1) to n[y] and age the opposite count
    if (n[y]<255) ++n[y];
    if (n[1-y]>2) n[1-y]/=2, n[1-y]+=1;
  }
};

/* Approximately equivalent 2 byte counter implementing the above.
The representable counts (n0, n1) are 0-10, 12, 14, 16, 20, 24, 28,
32, 48, 64, 128, 256, 512.  Both counts are represented by a single
8-bit state.  Counts larger than 10 are incremented probabilistically.
Although it uses 1/3 less memory, it is 8% slower and gives 0.05% worse
compression than the 3 byte counter. */

class Counter2 {
  U8 state;
  struct E {      // State table entry
    U16 n0, n1;   // Counts represented by state
    U8 s00, s01;  // Next state on input 0 without/with probabilistic incr.
    U8 s10, s11;  // Next state on input 1
    U32 p0, p1;   // Probability of increment x 2^32-1 on inputs 0, 1
  };
  static E table[244];  // State table
public:
  Counter2(): state(0) {}
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
Counter2::E Counter2::table[244] = {
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

// Counter2 (less memory) or Counter3 (faster and better compression)
typedef Counter2 Counter;

/* Hashtable<T, N> is a hash table of 2^N elements of type T
with linear search of M=3 elements in case of collision.  If all elements
collide, then the one with the lowest T.priority() is replaced.  
Hashtable[i] returns a T& indexed by the lower N bits of i whose
checksum matches bits 31-24 of i, creating or replacing if needed.
The user must supply T::priority() and a 32-bit hash.  */

template<class T, int N, int M=3>
class Hashtable {
private:
  struct HashElement {
    U8 checksum;
    T value;
    HashElement(int ch=0): checksum(ch), value() {}
  };
  vector<HashElement> table;  // Array of 2^N+M elements
public:
  Hashtable(): table((1<<N)+M) {}
  inline T& operator[](U32 i);
};

template<class T, int N, int M>
T& Hashtable<T, N, M>::operator[](U32 i) {
  const U32 lb=i&((1<<N)-1);  // Lower bound of search range
  const U32 ub=lb+M;  // Upper bound + 1
  const int checksum=i>>24; 
  int bj=lb;
  int bget=1000000000;
  for (U32 j=lb; j<ub; ++j) {
    HashElement& c=table[j];
    int g=c.value.priority();
    if (g==0) {
      c=HashElement(checksum);
      return c.value;
    }
    if (checksum==c.checksum)
      return c.value;
    if (g<bget) {
      bget=g;
      bj=j;
    }
  }
  table[bj]=HashElement(checksum);
  return table[bj].value;
}
/* MatchModel, RecordModel and CharModel all have a function
model(y, n0, n1) which updates the model with bit y (0 or 1), then
returns an array of counts in n0[] and n1[].  Each pair of elements
represents a probability n1[i]/n with weight n that the next value of
y will be 1, where n = n0[i]+n1[i].  The number of elements written
depends on the model, and is usually defined as N in the class.

A MatchModel looks for a match of length n >= 8 bytes between
the current context and previous input, and predicts the next bit
in the previous context with weight n.  If the next bit is 1, then
(n0[0], n1[0]) is assigned (0, n), else (n, 0).  Matchies are found in
a 4MB rotating buffer using a 1M hash table of pointers. */

class MatchModel {
  enum {N=1};       // Number of submodels
  vector<U8> buf;   // Input buffer, wraps at end
  vector<U24> ptr;  // Hash table of pointers
  U32 hash[2];      // Hashes of current context up to pos-1
  int pos;          // Element of buf where next bit will be stored
  int bpos;         // Number of bits (0-7) stored at buf[pos]
  int begin;        // Points to first matching byte (does not wrap)
  int end;          // Points to last matching byte + 1, 0 if no match
public:
  MatchModel(): buf(0x10000<<MEM), ptr(0x4000<<MEM),
      pos(0), bpos(0), begin(0), end(0) {
    hash[0]=hash[1]=0;
  }
  void model(int y, int* n0, int* n1);
} matchModel;

void MatchModel::model(int y, int* n0, int* n1) {
  buf[pos]+=buf[pos]+y;  // Store bit
  ++bpos;
  if (end && (buf[end]>>(8-bpos))!=buf[pos])  // Does it match?
    begin=end=0;  // no
  if (bpos==8) {  // New byte
    bpos=0;
    hash[0]=hash[0]*(16*56797157)+buf[pos]+1; // Hash last 8 bytes
    hash[1]=hash[1]*(2*45684217)+buf[pos]+1; // Hash last 32 bytes
    U32 h=hash[0] >> (18-MEM);
    if ((hash[0]>>28)==0)
      h=hash[1] >> (18-MEM);  // 1/16 of 8-contexts are hashed to 32 bytes
    if (++pos==int(buf.size()))
      pos=0;
    if (end)
      ++end;
    else {  // Search for a matching context
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
    }
    ptr[h]=U24(pos);
  }
  *n0=*n1=0;
  if (end) { 
    int wt=end-begin;
    if (wt>255) wt=255;
    int y=(buf[end]>>(7-bpos))&1;
    if (y) *n1=wt;
    else *n0=wt;
  }
}

/* A RecordModel finds fixed length records and models bits in the context
of the two bytes above (the same position in the two previous records)
and in the context of the byte above and to the left (the previous byte).
The record length is assumed to be the interval in the most recent
occurrence of a byte occuring 4 times in a row equally spaced, e.g.
"x..x..x..x" would imply a record size of 3 (the mimimum). */

class RecordModel {
  enum {N=2};                    // Number of models
  static int lpos[256][4];       // Position of last 4 bytes
  vector<U8> buf;                // Rotating history [4K]
  Hashtable<Counter, 15+MEM> t;  // Model
  int pos;                       // Byte counter
  int c0;                        // Leading 1 and 0-7 bits
  int repeat;                    // Cycle length
  int hash[N];                   // of c0, context and repeat
  Counter* cp[N];                // Pointer to current counter
public:
  RecordModel();
  void model(int y, int* n0, int* n1);  // Update and predict
} recordModel;
int RecordModel::lpos[256][4]={{0}};

RecordModel::RecordModel(): buf(4096), pos(0), c0(1), repeat(1) {
  hash[0]=hash[1]=0;
  cp[0]=cp[1]=&t[0];
}

// Update the model with bit y, then put predictions of the next update
// as 0 counts in n0[0..N-1] and 1 counts in n1[0..N-1]
void RecordModel::model(int y, int* n0, int* n1) {

  // Update the counters with bit y
  cp[0]->add(y);
  cp[1]->add(y);
  c0+=c0+y;
  if (c0>=256) {
    c0-=256;

    // Save positions of last 4 occurrences of current byte c0
    int (&lp)[4]=lpos[c0];
    for (int j=3; j>0; --j)
      lp[j]=lp[j-1];
    lp[0]=pos;

    // Check for a repeating pattern of interval 3 or more
    if (lp[2]-lp[3]==lp[1]-lp[2] && lp[1]-lp[2]==lp[0]-lp[1] && lp[0]-lp[1]>2)
      repeat=lp[0]-lp[1];

    // Save c0 
    buf[pos&(buf.size()-1)]=c0;
    ++pos;

    // Compute context hashes
    hash[0]=buf[(pos-repeat)&(buf.size()-1)]*578997481
      +buf[(pos-repeat*2)&(buf.size()-1)]*878996291
      +repeat*378997811;  // Context is 2 bytes above
    hash[1]=buf[(pos-repeat)&(buf.size()-1)]*236399113
      +buf[(pos-1)&(buf.size()-1)]*736390141
      +repeat*984388129;  // Context is the byte above and to the left
    c0=1;
  }

  // Set up counter pointers
  cp[0]=&t[hash[0]+(c0<<24)+((c0*3)&255)];
  cp[1]=&t[hash[1]+(c0<<24)+((c0*3)&255)];

  // Predict the next value of y
  n0[0]=cp[0]->get0();
  n1[0]=cp[0]->get1();
  n0[1]=cp[1]->get0();
  n1[1]=cp[1]->get1();
}

/* A CharModel contains a fixed model, n-gram models from 0 to 7,
and several order-2 sparse models which skip over parts of the context. */

class CharModel {
  enum {N=16};                     // Number of models
  vector<Counter> t0;              // Order 0 counts [256]
  vector<Counter> t1;              // Order 1 counts [64K]
  Hashtable<Counter, MEM+16> t2;   // Sparse models
  Hashtable<Counter, MEM+19> t;    // Order 2-7 models
  int c0;                          // Current partial byte with leading 1 bit
  int c1, c2, c3, c4, c5, c6, c7;  // Previous bytes
  vector<U32> cxt;                 // Context hashes [N]
  vector<Counter*> cp;             // Pointers to current counts [N]
public:
  CharModel();
  void model(const int y, int* n0, int* n1);  // Update and predict
  int getc0() const {return c0;}
  int getc1() const {return c1;}
  int getc2() const {return c2;}
} charModel;

CharModel::CharModel(): t0(256), t1(65536), t(),
    c0(1), c1(0), c2(0), c3(0), c4(0), c5(0), c6(0), c7(0),
    cxt(N), cp(N) {
  for (int i=0; i<N; ++i)
    cp[i]=&t[0];
}

// Update with bit y, put array of 0 counts in n0 and 1 counts in n1
void CharModel::model(const int y, int* n0, int* n1) {

  // Update models
  for (int i=1; i<N; ++i)
    cp[i]->add(y);

  // Update context
  c0+=c0+y;
  if (c0>=256) {  // Start new byte
    for (int i=8; i>1; --i)
      cxt[i]=(cxt[i-1]+c0)*987660757;
    c0-=256; 
    if (c0>32)
      cxt[9]=(cxt[9]+c0)*34609027*4;  // Last whole word (8 chars max)
    else
      cxt[9]=0;
    cxt[10]=c0*837503159+c2*537496093;  // Sparse models
    cxt[11]=c0*840101893+c3*850090301;
    cxt[12]=c1*769377353+c3*653589317; 
    cxt[13]=c3*368910977+c7*568909763;
    cxt[14]=c1*950087393+c2*970092001;
    cxt[15]=c2*629495183+c3*649492307;
    c7=c6;
    c6=c5;
    c5=c4;
    c4=c3;
    c3=c2;
    c2=c1;
    c1=c0; 
    c0=1;
  }

  // Set up pointers to next counters
  cp[1]=&t0[c0];
  cp[2]=&t1[c0+c1*256];
  for (int i=3; i<10; ++i)
    cp[i]=&t[cxt[i]+(c0<<24)+((c0*3)&255)];
  for (int i=10; i<N; ++i)
    cp[i]=&t2[cxt[i]+(c0<<24)+((c0*3)&255)];

  // Return bit counts
  n0[0]=n1[0]=1;
  for (int i=1; i<N; ++i) {
    n0[i]=cp[i]->get0();
    n1[i]=cp[i]->get1();
  }
}

/* A MixModel mixes an array of counts to guess the next bit.  The counts
are adaptively weighted to minimize cost.  model(y, pr) updates all the
models with y, gets their bit counts, weights them, and returns the
probability P(1) * PSCALE in pr.  The weights are adjusted to minimize
the cost of encoding y.  There are C sets of weights, selected by
a context (the 3 upper bits of the last whole byte). */

class MixModel {
  enum {N=19, C=8};    // Number of models, number of weight contexts
  vector<int> wt;      // Context weights [C, N]
  int bc0[N], bc1[N];  // Bit counts concatenated from various models
  int cxt2;            // Secondary context to select a weight vector
public:
  MixModel();
  ~MixModel();
  void model(int y, int& pr); // Update with bit y, then put next guess in pr
} mixModel;

MixModel::MixModel(): wt(C*N), cxt2(0) {
  for (int i=0; i<C*N; ++i)
    wt[i]=1;
  for (int i=0; i<N; ++i)
    bc0[i]=bc1[i]=0;
}

MixModel::~MixModel() {
/*
  // Uncomment this to print the weights.  This is useful for testing
  // new models or weight vector contexts.
  printf("  ");
  for (int i=0; i<N; ++i)
    printf("%4d", i);
  printf("\n");
  for (int i=0; i<C; ++i) {
    printf("%2d", i);
    for (int j=0; j<N; ++j)
      printf("%4d", wt[i*N+j]/10);
    printf("\n");
  } */
}


void MixModel::model(int y, int& pr) {
  // Adjust the weights by gradient descent to reduce cost
  {
    const int cn=cxt2*N;
    int s0=0, s1=0;
    for (int i=0; i<N; ++i) {
      s0+=(wt[cn+i]+48)*bc0[i];
      s1+=(wt[cn+i]+48)*bc1[i];
    }
    if (s0>0 && s1>0) {
      const int s=s0+s1;
      const int sy=y?s1:s0;
      const int sy1=0xffffffff/sy+(rnd()&1023) >> 10;
      const int s1 =0xffffffff/s +(rnd()&1023) >> 10;
      for (int i=0; i<N; ++i) {
        const int dw=int((y?bc1[i]:bc0[i])*sy1-(bc0[i]+bc1[i])*s1
           + (rnd()&255)) >> 8;
        wt[cn+i]=min(65535, max(1, wt[cn+i]+dw));
      }
    }
  }

  // Get counts
  charModel.model(y, bc0, bc1);
  recordModel.model(y, bc0+16, bc1+16);
  matchModel.model(y, bc0+18, bc1+18);

  // Update secondary context
  cxt2=charModel.getc1()/(256/C);

  // Predict next bit
  int n0=1, n1=n0;
  for (int j=0; j<N; ++j) {
    int w=wt[cxt2*N+j];
    n0+=bc0[j]*w;
    n1+=bc1[j]*w;
  }
  int n=n0+n1;
  assert(n>0);
  while (n>2000000000/PSCALE) n/=4, n1/=4;
  pr=int((PSCALE-1)*n1/n);
}

/* A Predictor adjusts the model probability using SSE and passes it
to the encoder.  An SSE model is a table of counters, sse[SSE1][SSE2]
which maps a context and a probability into a new, more accurate
probability.  The context, SSE1, consists of the 0-7 bits of the current
byte and the 2 leading bits of the previous byte.  The probability
to be mapped, SSE2 is first stretched near 0 and 1 using SSEMap, then
quantized into SSE2=32 intervals.  Each SSE element is a pair of 0
and 1 counters of the bits seen so far in the current context and
probability range.  Both the bin below and above the current probability
is updated by adding 1 to the appropriate count (n0 or n1).  The
output probability for an SSE element is n1/(n0+n1) interpolated between
the bins below and above the input probability.  This is averaged
with the original probability with 25% weight to give the final
probability passed to the encoder. */

class Predictor {
  enum {SSE1=256*4, SSE2=32,  // SSE dimensions (contexts, probability bins)
    SSESCALE=1024/SSE2};      // Number of mapped probabilities between bins

  // Scale probability p into a context in the range 0 to 1K-1 by
  // stretching the ends of the range.
  class SSEMap {
    U16 table[PSCALE];
  public:
    int operator()(int p) const {return table[p];}
    SSEMap();
  } ssemap;  // functoid

  // Secondary source encoder element
  struct SSEContext {
    U8 c1, n;  // Count of 1's, count of bits
    int p() const {return PSCALE*(c1*64+1)/(n*64+2);}
    void update(int y) {
      if (y)
        ++c1;
      if (++n>254) {  // Roll over count overflows
        c1/=2;
        n/=2;
      }
    }
    SSEContext(): c1(0), n(0) {}
  };

  vector<vector<SSEContext> > sse;  // [SSE1][SSE2+1] context, mapped prob
  int nextp;   // p()
  int ssep;    // Output of sse
  int context; // SSE context
public:
  Predictor();
  int p() const {return nextp;}  // Returns pr(y = 1) * PSCALE
  void update(int y);  // Update model with bit y = 0 or 1
};

Predictor::SSEMap::SSEMap() {
  for (int i=0; i<PSCALE; ++i) {
    int p=int(64*log((i+0.5)/(PSCALE-0.5-i))+512);
    if (p>1023) p=1023;
    if (p<0) p=0;
    table[i]=p;
  }
}

Predictor::Predictor(): sse(SSE1), nextp(PSCALE/2), ssep(512), context(0) {

  // Initialize to sse[context][ssemap(p)] = p
  for (int i=0; i<SSE1; ++i)
    sse[i].resize(SSE2+1);
  int N=PSCALE;
  int oldp=SSE2+1;
  for (int i=N-1; i>=0; --i) {
    int p=(ssemap(i*PSCALE/N)+SSESCALE/2)/SSESCALE;
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

inline void Predictor::update(int y) {

  // Update the bins below and above the last input probability, ssep
  sse[context][ssep/SSESCALE].update(y);
  sse[context][ssep/SSESCALE+1].update(y);

  // Update/predict all the models
  mixModel.model(y, nextp);

  // Get the SSE context
  context=charModel.getc0()*4+charModel.getc1()/64;

  // Get final probability, interpolate and average with original
  ssep=ssemap(nextp);
  int wt=ssep%SSESCALE;
  int i=ssep/SSESCALE;
  nextp=(((sse[context][i].p()*(SSESCALE-wt)+sse[context][i+1].p()*wt)
    /SSESCALE)*3+nextp)/4;
}

/* An Encoder does arithmetic encoding.  Methods:
   Encoder(COMPRESS, f) creates encoder for compression to archive f, which
     must be open past the header for writing in binary mode
   Encoder(DECOMPRESS, f) creates encoder for decompression from archive f,
     which must be open past the header for reading in binary mode
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
  void encode(int y);    // Compress bit y
  int decode();          // Uncompress and return bit y
  void flush();          // Call when done compressing
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
subrange.  Output leading bytes of the range as they become known. */

inline void Encoder::encode(int y) {

  // Split the range
  const U32 p=predictor.p()*(4096/PSCALE)+2048/PSCALE; // P(1) * 4K
  assert(p<4096);
  const U32 xdiff=x2-x1;
  U32 xmid=x1;  // = x1+p*(x2-x1) multiply without overflow, round down
  if (xdiff>=0x4000000) xmid+=(xdiff>>12)*p;
  else if (xdiff>=0x100000) xmid+=((xdiff>>6)*p)>>6;
  else xmid+=(xdiff*p)>>12;

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
  const U32 p=predictor.p()*(4096/PSCALE)+2048/PSCALE; // P(1) * 4K
  assert(p<4096);
  const U32 xdiff=x2-x1;
  U32 xmid=x1;  // = x1+p*(x2-x1) multiply without overflow, round down
  if (xdiff>=0x4000000) xmid+=(xdiff>>12)*p;
  else if (xdiff>=0x100000) xmid+=((xdiff>>6)*p)>>6;
  else xmid+=(xdiff*p)>>12;

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
  while (((x1^x2)&0xff000000)==0) {
    x1<<=8;
    x2=(x2<<8)+255;
    int c=getc(archive);
    if (c==EOF) c=0;  // Fix for version 2
    x=(x<<8)+c;
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

  // Test the compiler
  assert(sizeof(U8)==1);
  assert(sizeof(U16)==2);
  assert(sizeof(U24)==3);
  assert(sizeof(U32)==4);
  assert(sizeof(int)==4);

  // Check arguments
  if (argc<2) {
    printf(
      PROGNAME " v2 file compressor/archiver, (C) 2003, Matt Mahoney, mmahoney@cs.fit.edu\n"
      "This program is free software distributed without warranty under the terms\n"
      "of the GNU General Public License, see http://www.gnu.org/licenses/gpl.txt\n"
      "\n"
      "To compress:         " PROGNAME " archive filenames...  (archive will be created)\n"
      "  or (MSDOS):        dir/b | " PROGNAME " archive  (reads file names from input)\n"
      "To extract/compare:  " PROGNAME " archive  (does not clobber existing files)\n"
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

    // Read PROGNAME "\r\n" at start of archive
    if (getline(archive) != PROGNAME) {
      printf("Archive file %s not in " PROGNAME " format\n", argv[1]);
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
        printf("%s: Bad " PROGNAME " header format %d %d\n", argv[1],
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
    fprintf(archive, PROGNAME "\r\n");
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

