// paq1.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

/* PAQ1 - File archiver and compressor.
(C) 2002, Matt Mahoney, mmahoney@cs.fit.edu

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
or see http://www.gnu.org/licenses/gpl.txt

USAGE

To compress:      PAQ1 archive file file...  (1 or more file names), or
  or (MSDOS):     dir/b | PAQ1 archive       (read file names from input)
  or (UNIX):      ls    | PAQ1 archive
To decompress:    PAQ1 archive
To list contents: more < archive

Compression:  The files listed are compressed and stored in the archive,
which is created.  The archive must not already exist.  File names may
specify a path, which is stored.  If there are no file names on the command
line, then PAQ1 prompts for them, reading until the first blank line or
end of file.

Decompression:  No file names are specified.  The archive must exist.
If a path is stored, the file is extracted to the appropriate directory,
which must exist.  PAQ1 does not create directories.  If the file to be
extracted already exists, it is not replaced; rather it is compared with
the archived file, and the offset of the first difference is reported.

It is not possible to add, remove, or update files in an existing archive.
If you want to do this, extract the files, delete the archive, and
create a new archive with just the files you want.

FILE FORMAT

The archived file names are stored in a readable header as follows:

PAQ1\r\n
size name\r\n
size name\r\n
\032\f\0

where "size" is the original number of bytes, as a 10 digit decimal number,
right justified with leading spaces, "name" is the file name (as input,
including path if any), \r = carriage return, \n = linefeed, \f = formfeed,
\032 is the DOS end of file character, and \0 is a NUL.

The header is followed by the compressed data in binary format.  The
input files are concatenated and treated as a single data stream.  Some
improvement in compression is possible by grouping related files together,
for instance, all text files followed by all binary files.

Data is compressed using a bit stream predictive arithmetic encoder.
An input string s is encoded as a binary fraction x such that for any
randomly chosen string r of the same length:

  p(r < s) <= x < p(r <= s)                               (1)

Such an x with length at most log2 p(s) + 1 bits can always be
found.  It is convenient to estimate p(s) = p(s1 s2 s3 ... sn)
 = p(s1) p(s2 | s1) p(s3 | s1 s2) ... p(sn | s1 s2...sn-1)
and to narrow the range of possible x (initially 0 to 1) as each
probability term is computed.  As the leading bits of x become
known (because they are the same at the lower and upper bounds), they
can be output.  The archive is a binary representation of x in base
256 with the most significant digit (256^-1) first.  The ordering
of s is as a binary number of n bits (n/8 bytes) with the most
significant bit as the most significant bit of the first byte.

The archive is decompressed by finding s that satifies (1) given x.
We make an identical series of probability estimates p(si | s1...si-1)
where each si is one bit (0 or 1), then use our knowledge of x
to decide which it is.  After 8 predictions, one byte of uncompressed
data is output.

Compression performance depends on the quality of prediction.
A predictor consists of a set of models that output a weighted
probability that the next bit (si) will be a 0 or 1, given all of the
previous input (s1 s2 ... si-1).  The weighted probability is
expressed as counts of 0s and 1s (n0 and n1), where p(1) = n1/(n0+n1)
with weight (confidence) n0+n1.  Models are combined by adding
their respective counts, which has the effect of a weighted averaging.

PAQ1 combines the outputs of 5 models:

m0 - A bland model in which p(1) = 1/2 with confidence 2, i.e.
n0 = 1, n1 = 1.  By itself, this model would result in no compression.

m1 - NonstatioaryPPM.  This is a combination of N = 8 models of
order n = 1 through N.  In an order n model, the next bit is
predicted in the context of the last n - 1 bytes, plus the last
0 to 7 bits of the current byte.  Each time a 0 or 1 occurs in some
context, the counts n0 and n1 associated with that context is incremented.
If the opposite count is more than 2, then the excess is halved.  This
is a compromise between a stationary model, in which all outcomes in
the same context are equally significant, and an inverse temporal
model, in which the probability of an event is inversely proportional
to the time since it last occurred, regardless of anything that happened
previous to that.

For example, suppose that a context occurs 10 times, and the sequence
of bits that follow the context is 1111111000.  If the outcomes are
independent, then n0 = 7, n1 = 3, and p(1) = 7/10 with confidence 10.
In an inverse temporal model, the last 1 was 4 events ago and the last
0 was 1 event ago, so 0 is 4 times more likely than 1, for p(1) = 1/5.
PAQ1 uses a compromise between these two positions:

  1111111     n0 = 0  n1 = 7  p(1) = 1   with confidence 7
  11111110    n0 = 1  n1 = 4  p(1) = 4/5 with confidence 5
  111111100   n0 = 2  n1 = 3  p(1) = 3/5 with confidence 5
  1111111000  n0 = 3  n1 = 2  p(1) = 2/5 with confidence 5

The counts are weighted by n^2 (order squared).  The rationale is
that if the last n bytes of context match, then the next byte will
match with probability (n - 1)/n (in an inverse temporal model),
so we weight the counts by n.  Also, the expected number of matches
of length n is proportional to 1/n (given the above probability), so
we weight the counts by n again.

The counts are stored in a hash table with 16M entries.  Each entry
takes 2 bytes for a total of 32 MB of memory: an 8 bit hash checksum
to detect collisions, and an 8 bit state representing the two counts
n0 and n1.  The context is hashed to 32 bits, with 24 bits used to
index the hash table and 8 bits used to detect collisions.  If a
collision occurs, then the next two table elements are tried.
If all 3 are occupied, then the one with the lowest n0 + n1 is removed.

The 8-bit state represents counts of 0 to 10 exactly, and counts
up to 512 as an approximation with probabilistic increments.
Since n0 and n1 can't both be large at the same time, no states are
needed for such pairs.

m2 - MatchModel.  This is an approximation of the NonstationaryPPM
model for orders longer than n = 8.  It searches the previous
4M bytes of input for a matching context of 8 or more bytes (plus
the current 0-7 bits), and if found, predicts whatever bit followed next
with weight 3n^2 (instead of n^2 since there might be more matches,
but only the last one is used).  It uses a hash table of 1M 3-byte
pointers (always overwriting the previous entry) into the 4MB buffer
to find matching contexts, for a total 7 MB of memory.

m3 - WordModel.  This is an optimization for English text.  There
are two models, and order n = 1 word model and an order n = 2 word model.
In the first, the context is the most recent word or partial word,
plus the previous and current bytes (8 to 15 bits).  A word is defined
as a sequence of letters (a-z), case insensitive.  The second model
includes the previous word in the context.  The weight is
(n + w)^2 where w is the length of the word or words (i.e. the
square of the length of the context in bytes as in m1).  The counts
are stored in a 4M hash table (8 MB memory) as in model m1.

m4 - CyclicModel.  This is an optimization for data with fixed length
records or rows, such as databases, tables, and binary numeric data.
If the record length can be determined, then two models are used.
The first is an order 1 bit model (no context) with weight 4 in which
the history is all bits in the same column.  The second is an order
9-bit model with weight 16 in which the context is the previous 8 bits
(spanning a byte boundary), i.e. the history is all those bits which
have the same 8 bit context as the current bit being predicted.  The
bit counts are stored in a 32K (64 KB) hash table.

The record length is determined by finding a pattern of 4 consecutive
8 bit sequences at equal intervals of at least 9 bits.  (It does not
have to be an integral number of bytes).  If a pattern of n equally
spaced sequences is found at interval r, then n more records are
assumed before the model is expired.  A new cycle length of r
may replace an existing one if the new product rn exceeds the old
one (with n rows remaining until the model expires).

PERFORMANCE

The following results were obtained by PAQ1 and various popular
and top ranked compression programs on the Calgary corpus.  For
each program, two tests were performed.  In the first, the 14 files
were compressed and the total size is reported (either as an archive
or 14 compressed files, depending on the program).  In the second
test, the 14 files were concatenated (alphabetically by file name)
into a single file of size 3,141,622 bytes and compressed.  The
resulting sizes are shown.  Run time is shown for the first test
but are about the same for both.  The test was performed on
a 750 MHz Duron with 256 MB memory, 128K L2 cache, running Windows Me.
PAQ1 was compiled using DJGPP (g++) 2.95.2 with optimization (gxx -O).
For all other programs, options are selected for maximum compression
within 64 MB of memory.

Program         Options        14 files   Seconds  Concatenated 
-------         -------        --------   -------  ------------
compress                       1,272,772     1.5   1,318,269
pkzip 2.04e                    1,032,290     1.5   1,033,217
gzip 1.2.4      -9             1,017,624     2     1,021,863
P5                               992,902    31.8     same
P6                               841,717    38.4     same
P12                              831,341    38.8     same
bzip2 1.0.0     -9               828,347     5       859,448
acb             u                766,322   110       769,363
boa 0.58b       -m15             751,413    44       769,196
ppmd H          e -m64 -o16      744,057     5       759,674
sbc 0.910       c -m3            740,161     4.1     819,016
ppmonstr H      e -m64 -o1       719,922    13       736,899
rk 1.04         -mx3 -M64 -ts    712,188    36       755,872
rk 1.02 b5      -mx3             707,144    64       750,740

PAQ1
m1, 1 MB                         843,819    46.3     same for all
m1, 8 MB                         768,463    45.9
m1, 16 MB                        758,456    45.6
m1, 32 MB                        751,734    45.3
m1-m2, 32+7 MB                   731,637    48.5
m1-m3, 32+7+8 MB                 723,092    62.1
m1-m4, 16+7+8 MB                 720,310    71.2
m1-m4, 32+7+4 MB                 717,766    68.0
m1-m4, 32+7+8 MB  (as coded)     716,704    68.1

compress, pkzip, and gzip are LZ encoders, encoding strings as pointers
to previous occurrences or to dictionary entries.  bzip2 and sbc are
Burrows-Wheeler (context sorting) encoders.  P5, P6, and P12 use
neural network prediction and arithmetic encoding.  acb, boa, ppmd,
ppmonstr, and rk are stationary PPM (longest context matching) encoders.
rk is the best of 157 compressors rated by ACT as of 6/24/2001 (Gilchrist,
see http://compression.ca/act-calgary.html ), followed by sbc, boa, and acb.
On the Canterbury corpus, ppmonstr, ppmd, and rk are the top three
(see http://www.geocities.com/SiliconValley/Bay/1995/texts22.html ).

PAQ1 beats all other comprssors when the Calgary corpus is concatenated
into a single file.  Compression is the same with or without
concatenation because PAQ1 (and P5, P6, P12) always concatenate the
files to take advantage of cross file statistics.  Concatenation tends to
hurt stationary models such as PPM and Burrows-Wheeler because the
Calgary corpus contains a mix of text and binary files
with different statistics and the models do not adapt.

Results for RK -mx3 and PAQ1.  RK is a PPMZ-based program which
rearranges the file order to optimize compression across files.
For consistency, the PAQ1 test was run with the files in the same
order as RK rather than alphabetically as previously.

File        Size   RK -mx3  Ratio    PAQ1   Ratio
------    ------   -------  -----   ------  -----
PAPER2     82199     22111   26.8    22323  27.16
PAPER1     53161     13085   24.6    12783  24.05
BOOK1     768771    201950   26.2   206539  26.87
BOOK2     610856    139591   22.8   132151  21.63
OBJ1       21504      9397   43.6    10027  46.63
GEO       102400     47960   46.8    52244  51.02
OBJ2      246814     63754   25.8    66416  26.91
PIC       513216     30595    5.9    46975   9.15
BIB       111261     24146   21.7    23011  20.68
PROGL      71646     13692   19.1    12661  17.67
PROGC      39611     11614   29.3    10571  26.69
PROGP      49379     10207   20.6     9048  18.32
TRANS      93695     14273   15.2    13465  14.37
NEWS      377109    104435   27.6    97917  25.97
------    ------   -------  -----   ------  -----
14       3141622    706810   22.4   716386  22.80

rk outperforms PAQ1 on binary files such as GEO (seismic data, 32 bit
numbers), PIC (bitmapped 200 dpi FAX image, page from a textbook), OBJ1
(VAX executable), and OBJ2 (Macintosh executable).  PAQ1 outperforms
rk on most text files, especially toward the end as more statistics are
collected.

rk also outperforms PAQ1 on random data, with practically no expansion
vs. 6.6% (8.5 bpc) for PAQ1.  Slowing the aging of statistics (making the
model more stationary) decreases this expansion but hurts compression for
almost all other files.  It is surprisingly difficult to detect
randomness embedded in compressible data without sacrificing the
ability to adapt to changing statistics.

ORIGINAL CODE: 1/6/01.  Compiled using DJGPP 2.95.2.

REVISED 1/21/2001 to compile in Borland C++ 5.5.1.
- Removed unused typedef U64 (Borland does not support long long).
- In the CyclicModel constructor, changed cpos(255)
to cpos(256) to fix a vector out of bounds bug.
- In main(), added a test for filesize.size()==0 before calling
max_element().  The last two bugs caused PAQ1 to crash when compiled
with Borland but not g++.  Archive compatibility is not affected.
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
#include <map>
using namespace std;

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

/* Hashtable<T, N> is a hash table of 2^N elements of type T
(derived from HashElement) with linear search
of 3 elements in case of collision.  If all elements collide,
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
  inline T& operator[](U32 i);
  ~Hashtable() {delete[] table;}
};

template<class T, int N, int M>
T& Hashtable<T, N, M>::operator[](U32 i) {
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
observations.  The aged counts are stored in a hash table of 8M
contexts.  */

class NonstationaryPPM: public Model {
  enum {N=8};  // Number of contexts
  int c0;  // Current 0-7 bits of input with a leading 1
  int c1;  // Previous whole byte
  int cn;  // c0 mod 53 (low bits of hash)
  vector<Counter> counter0;  // Counters for context lengths 0 and 1
  vector<Counter> counter1;
  Hashtable<Counter, 24> counter2;  // for lengths 2 to N-1
  Counter *cp[N];  // Pointers to current counters
  U32 hash[N];   // Hashes of last 0 to N-1 bytes
public:
  inline void predict(int& c0, int& c1) const;  // Add to counts of 0s and 1s
  inline void update(int y);   // Append bit y (0 or 1) to model
  NonstationaryPPM();
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
  for (int i=0; i<N; ++i)
    if (cp[i])
      cp[i]->add(y);

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
  MatchModel(): buf(0x400000), ptr(0x100000), hash(0), pos(0),
    bpos(0), begin(0), end(0) {}
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
  // If there is nn current match, search for one.
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
  Hashtable<Counter, 22> t1;  // Model
  int c1;  // Previous char, lower case
  int c0;  // 0-7 bits of current char with leading 1 bit
  int c0h; // Small hash of c0
  Counter *cp0, *cp1;  // Points into t1 current context
public:
  WordModel(): word1(0), word0(0), ww1(0), ww0(0),
      c1(0), c0(1), c0h(1), cp0(&t1[0]), cp1(&t1[0]) {}

  void predict(int& n0, int& n1) const {
    const int wt0=(ww0+1)*(ww0+1);
    n0+=cp0->get0()*wt0;
    n1+=cp0->get1()*wt0;
    const int wt1=(ww0+ww1+2)*(ww0+ww1+2);
    n0+=cp1->get0()*wt1;
    n1+=cp1->get1()*wt1;
  }

  void update(int y) {
    cp0->add(y);
    cp1->add(y);

    // Update contexts with next bit
    c0+=c0+y;
    c0h+=c0h+y;
    if (c0h>=59) c0h-=59;
    if (c0>=256) {
      c0-=256;
      if (isalpha(c0)) {
        word0=(word0+tolower(c0)+1)*234577751*16;
        if (++ww0>8) ww0=8;
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
    U32 h=word0*123456791+c1*345689647+c0h+(c0<<24);
    cp0=&t1[h];
    cp1=&t1[word1+h];
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
public:
  U16 p() const {
    int n0=1, n1=n0;
    m1.predict(n0, n1);
    m2.predict(n0, n1);
    m3.predict(n0, n1);
    m4.predict(n0, n1);
    return U16(65535.0*n1/(n0+n1));
  }
  void update(int y) {
    m1.update(y);
    m2.update(y);
    m3.update(y);
    m4.update(y);
  }
};

/* An Encoder does arithmetic encoding.  Methods:
   Encoder(COMPRESS, f) creates encoder for compression to archive f, which
     must be open for writing in binary mode
   Encoder(DECOMPRESS, f) creates encoder for decompression from archive f,
     which must be open for reading in binary mode
   encode(bit) in COMPRESS mode compresses bit to file f.
   encode() in DECOMPRESS mode returns the next decompressed bit from file f.
   print() prints compression statistics
*/

typedef enum {COMPRESS, DECOMPRESS} Mode;
class Encoder {
private:
  Predictor predictor;
  const Mode mode;       // Compress or decompress?
  FILE* archive;         // Compressed data file
  U32 x1, x2;            // Range, initially [0, 1), scaled by 2^32
  U32 x;                 // Last 4 input bytes of archive.
  int eofs;              // Number of EOF's read from data
  long xchars;           // Number of bytes of compressed data
  long encodes;          // Number of bits of uncompressed data
  int start_time;        // Clocks at start of compression
  long total_encodes;    // Sum of encodes
  int total_time;        // Sum of compression times
public:
  Encoder(Mode m, FILE* f);
  int encode(int bit=0);
  void print();
  ~Encoder();
};

// Constructor
Encoder::Encoder(Mode m, FILE* f): predictor(), mode(m), archive(f), x1(0),
    x2(0xffffffff), x(0), eofs(0), xchars(0), encodes(0), start_time(0),
    total_encodes(0), total_time(0) {
  start_time=clock();

  // In DECOMPRESS mode, initialize x to the first 4 bytes of the archive
  if (mode==DECOMPRESS) {
    for (int i=0; i<4; ++i) {
      int c=getc(archive);
      if (c==EOF) {
        c=0;
        ++eofs;
      }
      else
        ++xchars;
      x=(x<<8)+c;
    }
  }
}

/* encode(bit) -- Split the range [x1, x2] at x in proportion to predictor
   P(y = 1).  In COMPRESS mode, make the lower or upper subrange
   the new range according to y.  In DECOMPRESS mode, return 0 or 1
   according to which subrange x is in, and make this the new range.

   Maintain x1 <= x <= x2 as the last 4 bytes of compressed data:
   In COMPRESS mode, write the leading bytes of x2 that match x1.
   In DECOMPRESS mode, shift out these bytes and shift in an equal
   number of bytes into x from the archive.
*/
int Encoder::encode(int y) {
  ++encodes;

  // Split the range
  U32 p=65535-predictor.p(); // Probability P(0) * 64K rounded down
  const U32 xdiff=x2-x1;
  U32 xmid=x1;  // = x1+p*(x2-x1) multiply without overflow, round down
  if (xdiff>=0x10000000) xmid+=(xdiff>>16)*p;
  else if (xdiff>=0x1000000) xmid+=((xdiff>>12)*p)>>4;
  else if (xdiff>=0x100000) xmid+=((xdiff>>8)*p)>>8;
  else if (xdiff>=0x10000) xmid+=((xdiff>>4)*p)>>12;
  else xmid+=(xdiff*p)>>16;

  // Update the range
  if (mode==COMPRESS) {
    if (y)
      x1=xmid+1;
    else
      x2=xmid;
  }
  else {
    if (x<=xmid) {
      y=0;
      x2=xmid;
    }
    else {
      y=1;
      x1=xmid+1;
    }
  }
  predictor.update(y);

  // Shift equal MSB's out
  while (((x1^x2)&0xff000000)==0) {
    if (mode==COMPRESS) {
      putc(x2>>24, archive);
      ++xchars;
    }
    x1<<=8;
    x2=(x2<<8)+255;
    if (mode==DECOMPRESS) {
      int c=getc(archive);
      if (c==EOF) {
        c=0;
        if (++eofs>5) {
          printf("Premature end of archive\n");
          print();
          exit(1);
        }
      }
      else
        ++xchars;
      x=(x<<8)+c;
    }
  }
  return y;
}

// Destructor
Encoder::~Encoder() {

  // In COMPRESS mode, write out the remaining bytes of x, x1 < x < x2
  if (mode==COMPRESS) {
    while (((x1^x2)&0xff000000)==0) {
      putc(x2>>24, archive);
      x1<<=8;
      x2=(x2<<8)+255;
    }
    putc(x2>>24, archive);  // First unequal byte
  }
  if (total_encodes>0) {
    long total_xchars=ftell(archive);
    printf("%ld/%ld = %6.4f bpc (%4.2f%%) in %1.2f sec\n",
      total_xchars, total_encodes/8,
      total_xchars*64.0/total_encodes, total_xchars*800.0/total_encodes,
      double(total_time)/CLOCKS_PER_SEC);
  }
}

// Print Encoder stats
void Encoder::print() {
  int now=clock();
  if (encodes>0)
    printf("%ld/%ld = %6.4f bpc (%4.2f%%) in %1.2f sec\n",
      xchars, encodes/8,
      xchars*64.0/encodes, xchars*800.0/encodes,
      double(now-start_time)/CLOCKS_PER_SEC);
  else
    printf("0 bytes\n");
  total_time+=now-start_time;
  start_time=now;
  total_encodes+=encodes;
  encodes=0;
  xchars=0;
}

// Read one byte from encoder e
int decompress(Encoder& e) {  // Decompress 8 bits, MSB first
  int c=0;
  for (int i=0; i<8; ++i)  
    c=(c<<1)+e.encode();
  return c;
}

// Write one byte c to encoder e
void compress(Encoder& e, int c) {
  for (int i=0; i<8; ++i) {  // Compress 8 bits, MSB first
    e.encode((c&128)?1:0);
    c<<=1;
  }
}

// Fail if out of memory
void handler() {
  printf("Out of memory\n");
  exit(1);
}

// Read and return a line of input from FILE f (default stdin) up to
// first control character.  Skips CR in CR LF.
/*string getline(FILE* f=stdin) {
  int c;
  string result="";
  while ((c=getc(f))!=EOF && c>=32)
    result+=char(c);
  if (c=='\r')
    (void) getc(f);
  return result;
}*/

// User interface
/*int main(int argc, char** argv) {
  clock();
  set_new_handler(handler);

  // Check arguments
  if (argc<2) {
    printf(
      "PAQ1 file compressor/archiver, (C) 2002, Matt Mahoney, mmahoney@cs.fit.edu\n"
      "This program is free software distributed without warranty under the terms\n"
      "of the GNU General Public License, see http://www.gnu.org/licenses/gpl.txt\n"
      "\n"
      "To compress:         PAQ1 archive filenames...  (archive will be created)\n"
      "  or (MSDOS):        dir/b | PAQ1 archive  (reads file names from input)\n"
      "To extract/compare:  PAQ1 archive  (does not clobber existing files)\n"
      "To view contents:    more < archive\n");
    return 1;
  }

  // File names and sizes from input or archive
  vector<string> filename; // List of names
  vector<long> filesize;   // Size or -1 if error

  // Extract files
  FILE* archive=fopen(argv[1], "rb");
  if (archive) {
    if (argc>2) {
      printf("File %s already exists\n", argv[1]);
      return 1;
    }
    printf("Extracting archive %s ...\n", argv[1]);

    // Read "PAQ1\r\n" at start of archive
    if (getline(archive) != "PAQ1") {
      printf("Archive file %s not in PAQ1 format\n", argv[1]);
      return 1;
    }

    // Read "size filename" in "%10d %s\r\n" format
    while (true) {
      string s=getline(archive);
      if (s.size()>10) {
        filesize.push_back(atol(s.c_str()));
        filename.push_back(s.substr(11));
      }
      else
        break;
    }

    // Test end of header for "\f\0"
    {
      int c1=0, c2=0;
      if ((c1=getc(archive))!='\f' || (c2=getc(archive))!=0) {
        printf("%s: Bad PAQ1 header format %d %d\n", argv[1],
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
        if (s.size()==0 || s=="")
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
    if (filesize.size()==0
        || *max_element(filesize.begin(), filesize.end())<0) {
      printf("No files to compress, no archive created.\n");
      return 1;
    }

    // Write header
    archive=fopen(argv[1], "wb");
    if (!archive) {
      printf("Cannot create archive: %s\n", argv[1]);
      return 1;
    }
    fprintf(archive, "PAQ1\r\n");
    for (int i=0; i<int(filename.size()); ++i) {
      if (filesize[i]>=0)
        fprintf(archive, "%10ld %s\r\n", filesize[i], filename[i].c_str());
    }
    putc(032, archive);
    putc('\f', archive);
    putc(0, archive);

    // Write data
    Encoder e(COMPRESS, archive);
    for (int i=0; i<int(filename.size()); ++i) {
      const int size=filesize[i];
      if (size>=0) {
        printf("%s: ", filename[i].c_str());
        FILE* f=fopen(filename[i].c_str(), "rb");
        int c;
        for (long j=0; j<size; ++j) {
          if (f)
            c=getc(f);
          else
            c=0;
          compress(e, c);
        }
        if (f) {
          fclose(f);
          e.print();
        }
      }
    }
  }
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
		case DLL_PROCESS_DETACH: if(e != NULL) e->~Encoder(); break;
	}
	
	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

