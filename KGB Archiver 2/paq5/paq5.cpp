// paq5.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif

/* PAQ5 - File archiver and compressor.
(C) 2003, Matt Mahoney, mmahoney@cs.fit.edu

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation at
http://www.gnu.org/licenses/gpl.txt or (at your option) any later version.
This program is distributed without any warranty.


USAGE

To compress:      PAQ5 archive file file...  (1 or more file names), or
  or (MSDOS):     dir/b | PAQ5 archive       (read file names from input)
  or (UNIX):      ls    | PAQ5 archive
To decompress:    PAQ5 archive
To list contents: more < archive

Compression:  The files listed are compressed and stored in the archive,
which is created.  The archive must not already exist.  File names may
specify a path, which is stored.  If there are no file names on the command
line, then PAQ5 prompts for them, reading until the first blank line or
end of file.

Decompression:  No file names are specified.  The archive must exist.
If a path is stored, the file is extracted to the appropriate directory,
which must exist.  PAQ5 does not create directories.  If the file to be
extracted already exists, it is not replaced; rather it is compared with
the archived file, and the offset of the first difference is reported.

It is not possible to add, remove, or update files in an existing archive.
If you want to do this, extract the files, delete the archive, and
create a new archive with just the files you want.


TO COMPILE

gxx -O PAQ5.cpp        DJGPP 2.95.2  (gives fastest executable)
bcc32 -O2 PAQ5.cpp     Borland 5.5.1
sc -o PAQ5.cpp         Digital Mars 8.35n


PAQ5 DESCRIPTION

1. OVERVIEW

A PAQ5 archive has a header, listing the names and lengths of the files
it contains in human-readable format, followed by the compressed data.
The data is compressed as if all the files were concatenated into one
long string.

PAQ5 uses predictive arithmetic coding.  The string, y, is compressed
by representing it as a base 256 number, x, such that:

  P(s < y) <= x < P(s <= y)                                             (1)

where s is chosen randomly from the probability distribution P, and x
has the minimum number of digits (bytes) needed to satisfy (1).
Such coding is within 1 byte of the Shannon limit, log 1/P(y), so
compression depends almost entirely on the goodness of the model, P,
i.e. how well it estimates the probability distribution of strings that
might be input to the compressor.

Coding and decoding are illustrated in Fig. 1.  An encoder, given P and
y, outputs x.  A decoder, given P and x, outputs y.  Note that given
P in equation (1), that you can find either x from y or y from x.
Note also that both computations can be done incrementally.  As the
leading characters of y are known, the range of possible x narrows, so
the leading digits can be output as they become known.  For decompression,
as the digits of x are read, the set of possible y satisfying (1)
is restricted to an increasingly narrow lexicographical range containing y.
All of the strings in this range will share a growing prefix.  Each time
the prefix grows, we can output a character.

            y
          +--------------------------+
  Uncomp- |                          V
  ressed  |    +---------+  p   +----------+  x   Compressed
  Data  --+--->|  Model  |----->| Encoder  |----+ Data
               +---------+      +----------+    |
                                                |
                                     +----------+
                                     V
            y  +---------+  p   +----------+  y       Uncompressed
          +--->|  Model  |----->| Decoder  |----+---> Data
          |    +---------+      +----------+    |
          |                                     |
          +-------------------------------------+

  Fig. 1.  Predictive arithmetic compression and decompression


Note that the model, which estimates P, is identical for compression
and decompression.  Modeling can be expressed incrementally by the
chain rule:

  P(y) = P(y_1) P(y_2|y_1) P(y_3|y_1 y_2) ... P(y_n|y_1 y_2 ... y_n-1)  (2)

where y_i means the i'th character of the string y.  The output of the
model is a distribution over the next character, y_i, given the context
of characters seen so far, y_1 ... y_i-1.

To simplify coding, PAQ5 uses a binary string alphabet.  Thus, the
output of a model is an estimate of P(y_i = 1 | context) (henceforth p),
where y_i is the i'th bit, and the context is the previous i - 1 bits of
uncompressed data.


2.  PAQ5 MODEL

The PAQ5 model consists of a weighted mix of independent submodels which
make predictions based on different contexts.  The submodels are weighted
adaptively to favor those making the best predictions.  The output of
two independent mixers (which use sets of weights selected by different
contexts) are averaged.  This estimate is then adjusted by secondary
symbol estimation (SSE), which maps the probability to a new probability
based on previous experience and the current context.  This final
estimate is then fed to the encoder as illustrated in Fig. 2.


  Uncompressed input
  -----+--------------------+-------------+-------------+
       |                    |             |             |
       V                    V             |             |
  +---------+  n0, n1  +----------+       |             |
  | Model 1 |--------->| Mixer 1  |\ p    |             |
  +---------+ \      / |          | \     V             V
               \    /  +----------+  \ +-----+    +------------+
  +---------+   \  /                  \|     | p  |            |    Comp-
  | Model 2 |    \/                  + | SSE |--->| Arithmetic |--> ressed
  +---------+    /\                    |     |    | Encoder    |    output
      ...       /  \                  /|     |    |            |
               /    \  +----------+  / +-----+    +------------+
  +---------+ /      \ | Mixer 2  | /
  | Model N |--------->|          |/ p
  +---------+          +----------+

  Fig. 2.  PAQ5 Model details for compression.  The model is identical for
  decompression, but the encoder is replaced with a decoder.


3.  SUBMODELS

Individual submodels output a prediction in the form of 2 counts (n0, n1)
representing counts of past 0's and 1's that have occurred previously
in the same context.  After the result of the prediction becomes known
the appropriate count is updated.  A pair of counts represents a
probability p:

  p = n1 / n = n1 / (n0 + n1)                                           (3)

with confidence n = n0 + n1.

A pair of counts is associated with each context.  By context, we mean
a partition of all previous input, i.e. y_1 y_2 ... y_i-1 when predicting
the i'th bit y_i.  For example, one might choose the last k bits to be
the context.  In this case, there would be 2^k possible contexts, with
one pair of counters associated with each.

After the i'th bit is encoded and becomes known to the model to be a
0 or 1, we update the appropriate counter associated with the current
context.  However, the counts are decayed in order to favor newer data
over old by the following algorithm:

  n_y := n+y + 1
  if n_1-y > 2 then n_1-y := 1 + n_1-y / 2                              (4)

In other words, when we observe a 1, we discard half of the 0 count
over 2.  This favors newer data, but also gives greater weight to
long runs of 0s or 1s.  For example:

  Input             n0  n1   p    Confidence = n  Stationary model
  0000000000        10   0   0    10              0/10
  00000000001        6   1   1/7   7              1/11
  000000000011       4   2   2/6   6              2/12
  0000000000111      3   3   3/6   6              3/13
  00000000001111     2   4   4/6   6              4/14
  000000000011111    2   5   5/7   7              5/15
  0000000000111111   2   6   6/8   8              6/16

  Table. 1.  Nonstationary counter example.  For comparison, the predictions
  of a stationary model are also shown.

This technique allows nonstationary sources to be modeled.  Had we
assumed that the input are independent (as some models do), we would
have estimated the 1 bits incorrectly.

Because both n0 and n1 cannot be large, it is possible to represent a
pair of counts compactly.  PAQ5 represents both counts as a single
8-bit state.  Large counts are approximated with probabilistic increments.
The allowable values are 0-10, 12, 14, 16, 20, 24, 28, 32, 48, 64, 128,
256, and 512.  For example, if n0 = 10 and we update with y = 0, then
instead of assigning n = 11, we assign n = 12 with 50% probability.

A second type of model considers only runs of identical byte values
within a context.  If a byte c occurs within a context m times in a row,
then we add m to either n0 or n1 depending on the corresponding bit within
c that we wish to predict.  This run length model requires 2 bytes to
represent, one for c and one for the 8-bit counter m, which is limited
to 255.


4.  SUBMODEL DETAILS

Submodels differ only in their contexts.  These are as follows:

a. DefaultModel.  (n0, n1) = (1, 1) regardless of context.

b. CharModel (n-gram model).  A context consists of the last n whole
bytes, plus the 0 to 7 bits of the partially read current byte.  There
are 8 models with n ranging from 0 to 7.  For n > 2, the context is
hashed and the counters are stored in a hash table of the following size:

  n  Number of contexts  (2^k means 2 to the power of k)
  -  ------------------
  0  2^8
  1  2^16
  2  2^20
  3  2^22
  4  2^24
  5  2^24
  6  2^24
  7  2^24

  Table 2.  Maximum number of contexts of length n bytes.

In addition, run length models are stored for 2 <= n <= 7.  There are
1/8 as many contexts for run length models as for nonstationary counters.

c.  MatchModel (long context).  A context is the last n whole bytes
(plus extra bits) where n >=8.  Up to 4 matching contexts are found by
indexing into a rotating input buffer of length 2^24 bytes.  The index
is a hash table of 2^22 pointers (32-bit, although 24-bit would be
sufficient) without collision detection.  Hashes are indexed using a
hash of the last 8 bytes, except that in 1/16 of the cases the last
32 bytes are hashed in order to find very long contexts.  For each
context match of length n bytes, the next bit (n0 or n1) is given
a weight of n^2/4 (maximum 511).

For mixing, the following models are added together: n = 2 and 3, 4 and 5,
6 and 7.  Thus, the output appears as 5 submodel predictions.

d.  RecordModel.  This models data with fixed length records, such as
tables.  The model attempts to find the record length by searching for
characters that repeat in the pattern x..x..x..x where the interval
between 4 successive occurrences is identical and at least 3.  If a
pattern is found, the record length remains fixed until another is found.
Two contexts are modeled:

  1. The two bytes above the current bit.
  2. The byte above and the previous byte (to the left).

Both contexts also include the record length and the current 0-7 bits,
which are all hashed together.  For each context, there are 2^22
pairs of counters plus 2^19 run length counts.  The counts from the
two contexts are added prior to mixing.

e.  SparseModel.  This models contexts with gaps.  It considers the
following contexts, where x denotes the bytes considered and ? denotes
the bit being predicted (plus preceding bits, which are included in
the context).

     x.x?      x..x?
    x.x.?  x...x...?
     xx.?      xx..?

  Table 3.  Sparse models.

For example, the first model considers the last and third to last bytes.
There are three outputs fed to the mixer, combined by adding the two
counter pairs in each of the three rows.  There are 2^18 counter pairs
and 2^15 run length counts associated with each of the 6 contexts.

e.  AnalogModel.  This is intended to model 16-bit audio (mono or stereo),
24-bit color images, and 8-bit data (such as grayscale images).  Contexts
drop the low order bits, and include the position within the file
modulo 2, 3, or 4.  There are 6 models, combined into 3 by addition
before mixing.  An x represents those bits which are part of the context.

  16 bit audio:
    xxxxxx.. ........ xxxxxx.. ........ ?  (position mod 2)
    xxxx.... ........ xxxxxx.. ........ ?  (position mod 2)
    xxxxxx.. ........ ........ ........ xxxxxx.. ........ xxxxxx.. ........ ?
      (position mod 4 for stereo audio)

  24 bit color:
    xxxx.... ........ ........ xxxxxxxx ........ ........ ? (position mod 3)
    xxxxxx.. xxxx.... xxxx.... ? (position mod 3)

  8 bit data:
    xxx..... xxxxx... xxxxxxx. ?

  Table 4.  Analog models.

Each of the 6 contexts is modeled by 2^16 counter pairs and 2^13 run
length counters.

f.  WordModel.  This is intended to model text files.  The string
is parsed into words by splitting on white space, which is considered
to be any character with an ASCII code of 32 or less.  The remaining
characters are hashed and form the context.  There is one sparse model
which skips a word.  In the table below, x now represents a whole
word.  The context also includes all of the bits of the current word
so far.  There are 3 contexts grouped into 2 prior to mixing:

    ?  (unigram model, 2^22 counter pairs and 2^19 run lengths)
   x?  (bigram model, 2^21 counter pairs and 2^18 run lengths)
  x.?  (sparse bigram model, 2^21 counter pairs and 2^18 run lengths,
        grouped with bigram model)

  Table 5.  Word models.


5.  MIXER

The mixers compute a probability by a weighted summation of the N = 15
counter pairs from the submodels.

      SUM_i=1..N w_i n1_i                                               (5)
  p = -------------------
      SUM_i=1..N w_i n_i

where w_i is the i'th weight, n1_i is the 1 bit count from the i'th
submodel, and n_i = n0_i + n1_i.

The weights w_i are adjusted after each bit of uncompressed data becomes
known in order to reduce the cost (code length) of that bit.  The
adjustment is along the gradient of the cost in weight space, which is

  w_i := w_i + e[ny_i/(SUM_j (w_j+wo) ny_j) - n_i/(SUM_j (w_j+wo) n_j)]

where e and wo are small constants, and ny_i is the count for bit y
(either n0 or n1) from the i'th submodel when the actual bit is y.
The weight offset wo prevents the gradient from going to infinity as the
weights go to 0.

There are two mixers, whose outputs are averaged together before being
input to the SSE stage.  Each mixer maintains a set of weights which
is selected by a context.  Mixer 1 maintains 8 weight vectors, selected
by the 3 high order bits of the previous byte.  Mixer 2 maintains 16
weight vectors, selected by the 2 high order bits of each of the
previous 2 bytes.


6.  SSE

The purpose of the SSE stage is to further adjust the probability
output from the mixers to agree with actual experience.  Ideally this
should not be necessary, but in reality this can improve compression.
For example, when "compressing" random data, the output probability
should be 0.5 regardless of what the models say.  SSE will learn this
by mapping all input probabilities to 0.5.


    | Output   __
    | p      /
    |       /
    |    __/
    |   /
    |  /
    |  |
    | /
    |/   Input p
    +-------------

  Fig. 3.  Example of an SSE mapping.

SSE maps the probability p back to p using a piecewise linear function
with 32 segments.  Each vertex is represented by a pair of 8-bit counters
(n0, n1) except that now the counters use a stationary model.  When the
input is p and a 0 or 1 is observed, then the corresponding count (n0
or n1) of the two vertices on either side of p are incremented.  When
a count exceeds the maximum of 255, both counts are halved.  The output
probability is a linear interpolation of n1/n between the vertices on
either side.

The vertices are scaled to be longer in the middle of the graph and short
near the ends.  The intial counts are set so that p maps to itself.

SSE is context sensitive.  There are 1024 separately maintained SSE
functions, selected by the 0-7 bits of the current (partial) byte and
the 2 high order bits of the previous byte.

The final output to the encoder is a weighted average of the SSE
input and output, with the output receiving 3/4 of the weight:

  p := (3 SSE(p) + p) / 4.                                              (6)


7.  EXPERIMENTAL RESULTS

Results on the Calgary corpos are shown below for some top data compressors
as of Dec. 2003.  Options are set for maximum compression.  When possible,
the files are all compressed into a single archive.

  Original size   Options        3,141,622  Time   Author
  -------------   -------        ---------  ----   ------
  gzip 1.2.4      -9             1,017,624     2   Jean Loup Gailly
  epm r9          c                668,115    49   Serge Osnach
  rkc             a -M80m -td+     661,102    91   Malcolm Taylor
  slim 19         a                659,358   153   Serge Voskoboynikov
  compressia 1.0 beta              650,398    66   Yaakov Gringeler
  durilca v.03a (as in README)     647,028    30   Dmitry Shkarin
  PAQ5                             661,811   361   Matt Mahoney
  WRT11 + PAQ5                     638,635   258   Przemyslaw Skibinski +

  Table 6.  Compressed size of the Calgary corpus.

WRT11 is a word reducing transform written by Przemyslaw Skibinski.  It
uses an external English dictionary to replace words with 1-3 byte
symbols to improve compression.  rkc, compressia, and durilca use a
similar approach.  Among dictionary compressors, WRT + PAQ5 gives the
best compression.  Among non-dictionary compressors, slim slightly
outperforms PAQ5.

PAQ5 was designed for maximum compression rather than speed, and is
the slowest compressor listed.  Compression is in seconds and was timed
on a 750 MHz Duron with 256 MB memory under Windows Me.  To obtain this
compression, it was often necessary to sacrifice 25% speed and memory
to gain 1% of compression.


8.  ACKNOWLEDGMENTS

Thanks to Serge Osnach for introducing me to SSE (in PAQ1SSE/PAQ2) and
the sparse models (PAQ3N).  Also, credit to Eugene Shelwein,
Dmitry Shkarin for suggestions on using multiple character SSE contexts.
Credit to Eugene, Serge, and Jason Schmidt for developing faster and
smaller executables of previous versions.  Credit to Werner Bergmans
and Berto Destasio for testing and evaluating them, including modifications
that improve compression at the cost of more memory.  Credit to
Alexander Ratushnyak who found a bug in PAQ4 decompression (now fixed).

I expect there will be better versions in the future.  If you make any
changes, please change the name of the program (e.g. PAQ6), including
the string in the archive header by redefining PROGNAME below.
This will prevent any confusion about versions or archive compatibility.
Also, give yourself credit in the help message.
*/

#define PROGNAME "PAQ5"  // Please change this if you change the program

#define hash ___hash  // To avoid Digital MARS name collision
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
#undef hash

const int PSCALE=2048;  // Integer scale for representing probabilities
const int MEM=9;        // Use about 0.6 MB * 2^MEM bytes of memory

template <class T> inline int size(const T& t) {return t.size();}

// 8-32 bit unsigned types, adjust as appropriate
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;

// Fail if out of memory
void handler() {
  printf("Out of memory\n");
  exit(1);
}

// A ProgramChecker verifies some environmental assumptions and sets the
// out of memory handler.  It also gets the program starting time.
// The global object programChecker should be initialized before any
// other global objects.

class ProgramChecker {
  clock_t start;
public:
  ProgramChecker() {
    start=clock();
    set_new_handler(handler);

    // Test the compiler for common but not guaranteed assumptions
    assert(sizeof(U8)==1);
    assert(sizeof(U16)==2);
    assert(sizeof(U32)==4);
    assert(sizeof(int)==4);
    assert(sizeof(char*)==sizeof(unsigned long));
  }
  clock_t start_time() const {return start;}  // When the program started
} programChecker;

//////////////////////////// rnd ////////////////////////////

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

//////////////////////////// hash ////////////////////////////

// Hash functoid, returns 32 bit hash of 1-4 chars

class Hash {
  U32 table[8][256];  // Random number table
public:
  Hash() {
    for (int i=7; i>=0; --i)
      for (int j=0; j<256; ++j)
        table[i][j]=rnd();
    assert(table[0][255]==3610026313LU);
  }
  U32 operator()(U8 i0) const {
    return table[0][i0];
  }
  U32 operator()(U8 i0, U8 i1) const {
    return table[0][i0]+table[1][i1];
  }
  U32 operator()(U8 i0, U8 i1, U8 i2) const {
    return table[0][i0]+table[1][i1]+table[2][i2];
  }
  U32 operator()(U8 i0, U8 i1, U8 i2, U8 i3) const {
    return table[0][i0]+table[1][i1]+table[2][i2]+table[3][i3];
  }
} hash;

//////////////////////////// Counter ////////////////////////////

/* A Counter represents a pair (n0, n1) of counts of 0 and 1 bits
in a context.

  get0() -- returns n0
  get1() -- returns n1
  add(y) -- increments ny, where y is 0 or 1
  priority() -- Returns a priority for hash replacement such that
    higher numbers should be favored.

A counter uses a nonstationary model favoring newer data.  When
ny (n0 or n1) is incremented and the opposite count is > 2, then
the excess over 2 is halved (rounding down).  For instance:

  Input         n0  n1
  -----         --  --
  00000000       8   0
  000000001      5   1
  0000000011     3   2
  00000000111    2   3
  000000001111   2   4
  0000000011111  2   5

To represent both counts using only 8 bits, the representable counts
(n0, n1) are limited to 0-10, 12, 14, 16, 20, 24, 28, 32, 48, 64, 128,
256, 512.  For counts above 10, incrementing is probabilistic and the
count is approximate.  The state represening (n0, n1) is updated using
a state table generated by stategen.cpp.  */

class Counter {
  U8 state;
  struct E {      // State table entry
    U16 n0, n1;   // Counts represented by state
    U8 s00, s01;  // Next state on input 0 without/with probabilistic incr.
    U8 s10, s11;  // Next state on input 1
    U32 p0, p1;   // Probability of increment x 2^32-1 on inputs 0, 1
  };
  static E table[244];  // State table
public:
  Counter(): state(0) {}
  int get0() const {return table[state].n0;}
  int get1() const {return table[state].n1;}
  int priority() const {return get0()+get1();}
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
Counter::E Counter::table[] = {
//    n0  n1 s00 s01 s10 s11          p0          p1
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

//////////////////////////// ch ////////////////////////////

/* ch is a global object that provides common services to models.
It stores all the input so far in a rotating buffer of the last N bytes

  ch -- Global object
  ch(i) -- Returns i'th byte from end
  ch(0) -- Returns the 0-7 bits of the partially read byte with a leading 1
  ch()  -- ch(0)
  ch.update(y) -- Appends bit y to the buffer
  ch.pos() -- The number of whole bytes appended, possibly > N
  ch.bpos() -- The number of bits (0-7) of the current partial byte at (0)
  ch[i] -- ch(pos()-i)
  ch.lo() -- Low order nibble so far (1-15 with leading 1)
  ch.hi() -- Previous nibble, 0-15 (no leading 1 bit)
  ch.pos(c) -- Position of the last occurrence of byte c (0-255)
  ch.pos(c, i) -- Position of the i'th to last occurrence, i = 0 to 3
*/
class Ch {
  enum {N = 1 << (16+MEM)};  // Buffer size
  //U32 N;
  U8 *buf;  // [N] last N bytes
  U32 p;  // pos()
  U32 bp;  // bpos()
  U32 hi_nibble, lo_nibble;  // hi(), lo()
  U32 lpos[256][4];  // pos(c, i)
public:
  Ch(): buf(new U8[N]), p(0), bp(0), hi_nibble(0), lo_nibble(1) {
    //N = 1 << (16+MEM);
	memset(buf, 0, N);
    buf[0]=1;
    memset(lpos, 0, 256*4*sizeof(U32));
  }
  U32 operator()(int i) const {return buf[(p-i)&(N-1)];}
  U32 operator()() const {return buf[p&(N-1)];}
  void update(int y) {
    U8& r=buf[p&(N-1)];
    r+=r+y;
    if (++bp==8) {
      lpos[r][3]=lpos[r][2];
      lpos[r][2]=lpos[r][1];
      lpos[r][1]=lpos[r][0];
      lpos[r][0]=p;
      bp=0;
      ++p;
      buf[p&(N-1)]=1;
    }
    if ((lo_nibble+=lo_nibble+y)>=16) {
      hi_nibble=lo_nibble-16;
      lo_nibble=1;
    }
  }
  U32 pos() const {return p;}
  U32 pos(U8 c, int i=0) const {return lpos[c][i&3];}
  U32 bpos() const {return bp;}
  U32 operator[](int i) const {return buf[i&(N-1)];}
  U32 hi() const {return hi_nibble;}
  U32 lo() const {return lo_nibble;}
} ch;  // Global object

//////////////////////////// Hashtable ////////////////////////////

/* A Hashtable stores Counters.  It is organized to minimize cache
misses for 64-byte cache lines.  The size is fixed at 2^n bytes.  It
uses LRU replacement for buckets of size 4, except that the next to
oldest element is replaced if it has lower priority than the oldest.
Each bucket represents 15 counters for a context on a half-byte boundary.

  Hashtable<Counter> ht(n) -- Create hash table of 2^n bytes (15/16 of
    these are 1-byte Counters).
  ht.set(h) -- Set major context to h, a 32 bit hash of a context ending on a
    nibble (4-bit) boundary.
  ht(c) -- Retrieve a reference to counter associated with partial nibble c
    (1-15) in context h.

Normally there should be 4 calls to ht(c) after each ht.set(h).
*/

template<class T>
class Hashtable {
private:
  const U32 N;  // log2 size in bytes
  struct HashElement {
    U8 checksum;  // Checksum of context, used to detect collisions
    T c[15];  // 1-byte counters in minor context c
  };
  HashElement *table;  // [2^(N-4)]
  U32 cxt;  // major context
public:
  Hashtable(U32 n);

  // Set major context to h, a 32 bit hash.  Create a new element if needed.
  void set(U32 h) {

    // Search 4 elements for h within a 64-byte cache line
    const U8 checksum=(h>>24)^h;
    const U32 lo= (h>>(32-N)) & -4;
    const U32 hi=lo+4;
    U32 i;
    for (i=lo; i<hi; ++i) {
      U32 pri=table[i].c[0].priority();
      if (table[i].checksum==checksum) { // found
        cxt=i;
        break;
      }
      else if (pri==0) {  // empty bucket
        table[i].checksum=checksum;
        cxt=i;
        break;
      }
    }

    // Put new element in front, pushing the lower priority of the two
    // oldest off the back
    if (i==hi) {
      cxt=lo;
      if (table[lo+2].c[0].priority()<table[lo+3].c[0].priority())
        memmove(table+lo+1, table+lo, 32);
      else
        memmove(table+lo+1, table+lo, 48);
      memset(table+lo, 0, 16);
      table[cxt].checksum=checksum;
    }

    // Move newest to front
    else if (cxt!=lo) {
      HashElement he=table[cxt];
      memmove(table+lo+1, table+lo, (cxt-lo)*16);
      table[lo]=he;
      cxt=lo;
    }
  }

  // Get element c (1-15) of bucket cxt
  T& operator()(U32 c) {
    --c;
    assert(c<15);
    return table[cxt].c[c];
  }
};

template <class T>
Hashtable<T>::Hashtable(U32 n): N(n-4) {
  assert(sizeof(HashElement)==16);

  // Align the hash table on a 64 byte cache page boundary
  char *p=(char*)malloc((16<<N)+64);
  if (!p)
    handler();
  memset(p, 0, (16<<N)+64);
  char *q=(char*)((unsigned long)(p+63)&-64);  // Aligned
  if (q>=p && q<p+64)  // Portability check
    p=q;
  table=(HashElement*)p;
}

//////////////////////////// mixer ////////////////////////////

/* A Mixer combines a weighted set of probabilities (expressed as 0 and
1 counts) into a single probability P(1) that the next bit will be a 1.

  Mixer m(C);      -- Create Mixer with C sets of N weights (N is fixed)
  m.write(n0, n1); -- Store a prediction P(1) = n1/(n0+n1), with confidence
                      0 <= n0+n1 < 1024.  There should be at most N calls
                      to write() followed by predict() and update().
                      Write order should be consistent.
  m.add(n0, n1);   -- Adds to a previous write.
  m.predict(c);    -- Return P(1)*PSCALE (range 0 to PSCALE-1) for
                      weight set c (0 to C-1).
  m.update(y);     -- Tune the N internal weights for set c such that
                      predict(c) would return a result closer to y*PSCALE,
                      y = 0 or 1.
*/
class Mixer {
  enum {N=32};  // Max writes before update
  const int C;
  U32 *bc0, *bc1;  // 0,1 counts for N models
  U32 (*wt)[N];  // wt[c][n] is n'th weight in context c
  int n;  // number of bit count pairs written
  int c;  // weight set context
public:
  Mixer(int C_);
  ~Mixer();
  U32 getN() const {return N;}
  U32 getC() const {return C;}

  // Store next counts n0, n1 from model
  void write(int n0, int n1) {
    bc0[n]=n0;
    bc1[n]=n1;
    ++n;
  }

  // Add to the last write
  void add(int n0, int n1) {
    bc0[n-1]+=n0;
    bc1[n-1]+=n1;
  }
  int predict(int c_);
  void update(int y);
};

// Return weighted average of models in context c_
int Mixer::predict(int c_) {
  assert(n>0 && n<=N);
  assert(c_>=0 && c_<C);
  c=c_;
  int n0=1, n1=n0;
  for (int j=0; j<n; ++j) {
    U32 w=wt[c][j];
    n0+=bc0[j]*w;
    n1+=bc1[j]*w;
  }
  int sum=n0+n1;
  assert(sum>0);
  while (sum>2000000000/PSCALE) sum/=4, n1/=4;
  return int((PSCALE-1)*n1/sum);
}

// Adjust the weights by gradient descent to reduce cost of bit y
void Mixer::update(int y) {
  U32 s0=0, s1=0;
  for (int i=0; i<n; ++i) {
    s0+=(wt[c][i]+48)*bc0[i];
    s1+=(wt[c][i]+48)*bc1[i];
  }
  if (s0>0 && s1>0) {
    const U32 s=s0+s1;
    const U32 sy=y?s1:s0;
    const U32 sy1=0xffffffff/sy+(rnd()&1023) >> 10;
    const U32 s1 =0xffffffff/s +(rnd()&1023) >> 10;
    for (int i=0; i<n; ++i) {
      const int dw=int((y?bc1[i]:bc0[i])*sy1-(bc0[i]+bc1[i])*s1
         + (rnd()&255)) >> 8;
      wt[c][i]=min(65535, max(1, int(wt[c][i]+dw)));
    }
  }
  n=0;
}

Mixer::Mixer(int C_): C(C_), bc0(new U32[N]), bc1(new U32[N]),
                      wt(new U32[C_][N]), n(0), c(0) {
  for (int i=0; i<C; ++i) {
    for (int j=0; j<N; ++j)
      wt[i][j]=1;
  }
  for (int i=0; i<N; ++i)
    bc0[i]=bc1[i]=0;
}

Mixer::~Mixer() {
/*
  // Uncomment this to print the weights.  This is useful for testing
  // new models or weight vector contexts.
  printf("  ");
  for (int i=0; i<n; ++i)
    printf("%4d", i);
  printf("\n");
  fflush(stdout);
  for (int i=0; i<C && i<16; ++i) {
    printf("%2d", i);
    for (int j=0; j<n; ++j)
      printf("%4d", wt[i][j]/10);
    printf("\n");
    fflush(stdout);
  } */
}

// A MultiMixer averages the output of 2 mixers using different contexts
class MultiMixer {
  Mixer m1, m2;
public:
  MultiMixer(): m1(8), m2(16) {}
  void write(int n0, int n1) {
    m1.write(n0, n1);
    m2.write(n0, n1);
  }
  void add(int n0, int n1) {
    m1.add(n0, n1);
    m2.add(n0, n1);
  }
  int predict() {
    U32 p1=m1.predict(ch(1)*m1.getC()/256);
    U32 p2=m2.predict((ch(1) >> 6)+4*(ch(2) >> 6));
    return (p1+p2)/2;
  }
  void update(int y) {
    m1.update(y);
    m2.update(y);
  }
  U32 getC() const {return 256;}
  U32 getN() const {return m1.getN();}
};

MultiMixer mixer;

//////////////////////////// CounterMap ////////////////////////////

/* CounterMap maintains a model and one context

  Countermap cm(N); -- Create, size 2^N bytes
  cm.update(h);     -- Update model, then set next context hash to h
  cm.write();       -- Predict next bit and write counts to mixer
  cm.add();         -- Predict and add to previously written counts

There should be 8 calls to either write() or add() between each update(h).
h is a 32-bit hash of the context which should be set after a whole number
of bytes are read. */

// Stores only the most recent byte and its count per context
// in a hash table without collision detection
class CounterMap1 {
  const int N;
  struct S {
    U8 c;  // char
    U8 n;  // count
    S(): c(0), n(0) {}
  };
  S* t;  // cxt -> c repeated last n times
  U32 cxt;
public:
  CounterMap1(int n): N(n-1), t(new S[1<<N]), cxt(0) {}
  void update(U32 h) {
    if (ch.bpos()==0) {
      if (t[cxt].n==0) {
        t[cxt].n=1;
        t[cxt].c=ch(1);
      }
      else if (U32(t[cxt].c)==ch(1)) {
        if (t[cxt].n<255)
          ++t[cxt].n;
      }
      else {
        t[cxt].c=ch(1);
        t[cxt].n=1;
      }

    }
    cxt = h >> 32-N;
  }
  void add() {
    if ((U32)((t[cxt].c+256) >> 8-ch.bpos())==ch()) {
      if ((t[cxt].c >> 7-ch.bpos()) & 1)
        mixer.add(0, t[cxt].n);
      else
        mixer.add(t[cxt].n, 0);
    }
  }
  void write() {
    mixer.write(0, 0);
    add();
  }
};

// Uses a nibble-oriented hash table of contexts
class CounterMap2 {
private:
  const U32 N2;  // Size of ht2 in elements
  U32 cxt;  // Major context
  Hashtable<Counter> ht2;  // Secondary hash table
  Counter* cp[8];  // Pointers into ht2 or 0 if not used
public:
  CounterMap2(int n);  // Use 2^n bytes memory
  void add();
  void update(U32 h);
  void write() {
    mixer.write(0, 0);
    add();
  }
};

CounterMap2::CounterMap2(int n): N2(n), cxt(0), ht2(N2) {
  for (int i=0; i<8; ++i)
    cp[i]=0;
}

// Predict the next bit given the bits so far in ch()
void CounterMap2::add() {
  const U32 bcount = ch.bpos();
  if (bcount==4) {
    cxt^=hash(ch.hi(), cxt);
    ht2.set(cxt);
  }
  cp[bcount]=&ht2(ch.lo());
  mixer.add(cp[bcount]->get0(), cp[bcount]->get1());
}

// After 8 predictions, update the models with the last input char, ch(1),
// then set the new context hash to h
void CounterMap2::update(U32 h) {
  const U32 c=ch(1);

  // Update the secondary context
  for (int i=0; i<8; ++i) {
    if (cp[i]) {
      cp[i]->add((c>>(7-i))&1);
      cp[i]=0;
    }
  }
  cxt=h;
  ht2.set(cxt);
}

// Combines 1 and 2 above.
class CounterMap3 {
  CounterMap1 cm1;
  CounterMap2 cm2;
public:
  CounterMap3(int n): cm1(n-2), cm2(n) {}
  void update(U32 h) {
    cm1.update(h);
    cm2.update(h);
  }
  void write() {
    cm2.write();
    cm1.add();
  }
  void add() {
    cm2.add();
    cm1.add();
  }
};

#define CounterMap CounterMap3

//////////////////////////// Model ////////////////////////////

// All models have a function model() which updates the model with the
// last bit of input (in ch) then writes probabilities for the following
// bit into mixer.
class Model {
public:
  virtual void model() = 0;
  virtual ~Model() {}
};

//////////////////////////// defaultModel ////////////////////////////

// DefaultModel predicts P(1) = 0.5

class DefaultModel: public Model {
public:
  void model() {mixer.write(1, 1);}
} defaultModel;

//////////////////////////// charModel ////////////////////////////

// A CharModel contains n-gram models from 0 to 7

class CharModel: public Model {
  enum {N=8};                // Number of models
  Counter *t0, *t1;          // Model orders 0, 1 [256], [65536]
  CounterMap t2, t3, t4, t5, t6, t7;
  U32 *cxt;                  // Context hashes [N]
  Counter *cp0, *cp1;        // Pointers to counters in t0, t1
public:
  CharModel(): t0(new Counter[256]), t1(new Counter[65536]),
               t2(12+MEM), t3(14+MEM), t4(16+MEM), t5(16+MEM),
               t6(16+MEM), t7(16+MEM), cxt(new U32[N]) {
    cp0=&t0[0];
    cp1=&t1[0];
    memset(cxt, 0, N*sizeof(U32));
    memset(t0, 0, 256*sizeof(Counter));
    memset(t1, 0, 65536*sizeof(Counter));
  }
  void model();         // Update and predict
} charModel;

// Update with bit y, put array of 0 counts in n0 and 1 counts in n1
inline void CharModel::model() {

  // Update models
  int y = ch(ch.bpos()==0)&1;  // last input bit
  cp0->add(y);
  cp1->add(y);

  // Update context
  if (ch.bpos()==0) {  // Start new byte
    for (int i=N-1; i>0; --i)
      cxt[i]=cxt[i-1]^hash(ch(1), i);
    t2.update(cxt[2]);
    t3.update(cxt[3]);
    t4.update(cxt[4]);
    t5.update(cxt[5]);
    t6.update(cxt[6]);
    t7.update(cxt[7]);
  }
  cp0=&t0[ch()];
  cp1=&t1[ch()+256*ch(1)];

  // Write predictions to the mixer
  mixer.write(cp0->get0(), cp0->get1());
  mixer.write(cp1->get0(), cp1->get1());
  t2.write();
  t3.add();
  t4.write();
  t5.add();
  t6.write();
  t7.add();
}

//////////////////////////// matchModel ////////////////////////////

/* A MatchModel looks for a match of length n >= 8 bytes between
the current context and previous input, and predicts the next bit
in the previous context with weight n.  If the next bit is 1, then
the mixer is assigned (0, n), else (n, 0).  Matchies are found using
an index (a hash table of pointers into ch). */

class MatchModel: public Model {
  enum {M=4};       // Number of strings to match
  U32 hash[2];      // Hashes of current context up to pos-1
  U32 begin[M];     // Points to first matching byte
  U32 end[M];       // Points to last matching byte + 1, 0 if no match
  U32 *ptr;         // Hash table of pointers [2^(MEM+14)]
public:
  MatchModel(): ptr(new U32[1 << (MEM+14)]) {
    memset(ptr, 0, (1 << (MEM+14))*sizeof(U32));
    hash[0]=hash[1]=0;
    for (int i=0; i<M; ++i)
      begin[i]=end[i]=0;
  }
  void model();
} matchModel;

inline void MatchModel::model() {
  if (ch.bpos()==0) {  // New byte
    hash[0]=hash[0]*(16*56797157)+ch(1)+1; // Hash last 8 bytes
    hash[1]=hash[1]*(2*45684217)+ch(1)+1; // Hash last 32 bytes
    U32 h=hash[0] >> (18-MEM);
    if ((hash[0]>>28)==0)
      h=hash[1] >> (18-MEM);  // 1/16 of 8-contexts are hashed to 32 bytes
    for (int i=0; i<M; ++i) {
      if (end[i] && ch(1)==ch[end[i]])
        ++end[i];
    }
    for (int i=0; i<M; ++i) {
      if (!end[i]) { // Search for a matching context
        int j;
        for (j=0; j<M; ++j)  // Search for duplicate match
          if (ptr[h]==end[j])
            break;
        if (j!=M)  // Context already matched?
          break;
        end[i]=ptr[h];
        if (end[i]>0) {
          begin[i]=end[i];
          U32 p=ch.pos();
          while (begin[i]>0 && p>0 && begin[i]!=p+1
              && ch[begin[i]-1]==ch[p-1]) {
            --begin[i];
            --p;
          }
        }
        if (end[i]==begin[i])  // No match found
          begin[i]=end[i]=0;
        break;
      }
    }
    ptr[h]=ch.pos();
  }

  // Test whether the current context is valid in the last 0-7 bits
  for (int i=0; i<M; ++i) {
    if (end[i] && ((ch[end[i]]+256) >> (8-ch.bpos())) != ch())
      begin[i]=end[i]=0;
  }

  // Predict the bit found in the matching contexts
  int n0=0, n1=0;
  for (int i=0; i<M; ++i) {
    if (end) { 
      U32 wt=(end[i]-begin[i]);
      wt=wt*wt/4;
      if (wt>511)
        wt=511;
      int y=(ch[end[i]]>>(7-ch.bpos()))&1;
      if (y)
        n1+=wt;
      else
        n0+=wt;
    }
  }
  mixer.write(n0, n1);
}

//////////////////////////// recordModel ////////////////////////////

/* A RecordModel finds fixed length records and models bits in the context
of the two bytes above (the same position in the two previous records)
and in the context of the byte above and to the left (the previous byte).
The record length is assumed to be the interval in the most recent
occurrence of a byte occuring 4 times in a row equally spaced, e.g.
"x..x..x..x" would imply a record size of 3 (the mimimum). */

class RecordModel: public Model {
  enum {N=2};           // Number of models
  CounterMap t0, t1;    // Model
  int repeat;           // Cycle length
public:
  RecordModel(): t0(14+MEM), t1(14+MEM), repeat(1) {}
  void model(); 
} recordModel;

// Update the model with bit y, then put predictions of the next update
// as 0 counts in n0[0..N-1] and 1 counts in n1[0..N-1]
inline void RecordModel::model() {
  if (ch.bpos()==0) {

    // Check for a repeating pattern of interval 3 or more
    const int c=ch(1);
    const int d1=ch.pos(c,0)-ch.pos(c,1);
    const int d2=ch.pos(c,1)-ch.pos(c,2);
    const int d3=ch.pos(c,2)-ch.pos(c,3);
    if (d1>2 && d1==d2 && d2==d3)
      repeat=d1;

    // Compute context hashes
    t0.update(hash(ch(repeat), ch(repeat*2), repeat&255));  // 2 above
    t1.update(hash(ch(1), ch(repeat), repeat&255));  // above and left
  }
  t0.write();
  t1.add();
}

//////////////////////////// sparseModel ////////////////////////////

// A SparseModel models several order-2 contexts with gaps

class SparseModel: public Model {
  enum {N=6};                      // Number of models
  CounterMap t0, t1, t2, t3, t4, t5;  // Sparse models
public:
  SparseModel(): t0(12+MEM), t1(12+MEM), t2(12+MEM),
                 t3(12+MEM), t4(12+MEM), t5(12+MEM) {}
  void model();  // Update and predict
} sparseModel;

// Update with bit y, put array of 0 counts in n0 and 1 counts in n1
inline void SparseModel::model() {

  // Update context
  if (ch.bpos()==0) {  // Start new byte
    t0.update(hash(ch(1), ch(3)));
    t1.update(hash(ch(1), ch(4)));
    t2.update(hash(ch(2), ch(4)));
    t3.update(hash(ch(4), ch(8)));
    t4.update(hash(ch(2), ch(3)));
    t5.update(hash(ch(3), ch(4)));
  }

  // Predict
  t0.write();
  t1.add();
  t2.write();
  t3.add();
  t4.write();
  t5.add();
}

//////////////////////////// analogModel ////////////////////////////

// An AnalogModel is intended for 16-bit mono or stereo (WAV files)
// 24-bit images (BMP files), and 8 bit analog data (such as grayscale
// images).

class AnalogModel: public Model {
  enum {N=6};
  CounterMap t0, t1, t2, t3, t4, t5;
  int pos3;  // pos % 3
public:
  AnalogModel(): t0(10+MEM), t1(10+MEM), t2(10+MEM), t3(10+MEM),
      t4(10+MEM), t5 (10+MEM), pos3(0) {}
  void model() {
    if (ch.bpos()==0) {
      if (++pos3==3) pos3=0;
      t0.update(hash(ch(2)/4, ch(4)/4, ch.pos()%2));  // 16 bit mono model
      t1.update(hash(ch(2)/16, ch(4)/16, ch.pos()%2));
      t2.update(hash(ch(2)/4, ch(4)/4, ch(8)/4, ch.pos()%4)); // Stereo
      t3.update(hash(ch(3), ch(6)/4, pos3));  // 24 bit image models
      t4.update(hash(ch(1)/16, ch(2)/16, ch(3)/4, pos3)); 
      t5.update(hash(ch(1)/2, ch(2)/8, ch(3)/32));  // 8-bit data model
    }
    t0.write();
    t1.add();
    t2.add();
    t3.write();
    t4.add();
    t5.write();
  }
} analogModel;

//////////////////////////// wordModel ////////////////////////////

// A WordModel models words, which are any characters > 32 separated
// by whitespace ( <= 32).  There is a unigram, bigram and sparse
// bigram model (skipping 1 word).

class WordModel: public Model {
  enum {N=3};
  CounterMap t0, t1, t2;
  U32 cxt[N];  // Hashes of last N words
public:
  WordModel(): t0(16+MEM), t1(15+MEM), t2(15+MEM) {
    for (int i=0; i<N; ++i)
      cxt[i]=0;
  }
  void model() {
    if (ch.bpos()==0) {
      int c=ch(1);
      if (c>32) {
        cxt[0]^=hash(cxt[0], c);
      }
      else if (cxt[0]) {
        for (int i=N-1; i>0; --i)
          cxt[i]=cxt[i-1];
        cxt[0]=0;
      }
      t0.update(cxt[0]);
      t1.update(cxt[1]+cxt[0]);
      t2.update(cxt[2]+cxt[0]);
    }
    t0.write();
    t1.write();
    t2.add();
  }
} wordModel;

//////////////////////////// Predictor ////////////////////////////

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

  SSEContext (*sse)[SSE2+1];  // [SSE1][SSE2+1] context, mapped probability
  U32 nextp;   // p()
  U32 ssep;    // Output of sse
  U32 context; // SSE context
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

Predictor::Predictor():
    sse((SSEContext(*)[SSE2+1]) new SSEContext[SSE1][SSE2+1]),
    nextp(PSCALE/2), ssep(512), context(0) {

  // Initialize to sse[context][ssemap(p)] = p
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

  // Adjust model mixing weights
  mixer.update(y);

  // Update individual models
  ch.update(y);
  defaultModel.model();   // 0
  charModel.model();      // 1-5
  matchModel.model();     // 6
  recordModel.model();    // 7
  sparseModel.model();    // 8-10
  analogModel.model();    // 11-13
  wordModel.model();      // 14-15

  // Combine probabilities
  nextp=mixer.predict();

  // Get final probability, interpolate SSE and average with original
  context=ch(0)*4+ch(1)/64;  // for SSE
  ssep=ssemap(nextp);
  U32 wt=ssep%SSESCALE;
  U32 i=ssep/SSESCALE;
  nextp=(((sse[context][i].p()*(SSESCALE-wt)+sse[context][i+1].p()*wt)
    /SSESCALE)*3+nextp)/4;

}

//////////////////////////// Encoder ////////////////////////////

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
    if (c==EOF) c=0;
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

//////////////////////////// main ////////////////////////////

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

  // Check arguments
  if (argc<2) {
    printf(
      PROGNAME " file compressor/archiver, (C) 2003, Matt Mahoney, mmahoney@cs.fit.edu\n"
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
  const double elapsed_time =
    double(clock()-programChecker.start_time())/CLOCKS_PER_SEC;
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
	/*if(mode >= 0 && mode <= 8){
		MEM = mode;
	}*/
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

