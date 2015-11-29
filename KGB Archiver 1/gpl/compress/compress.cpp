/* PAQ6v2 - File archiver and compressor.
(C) 2004, Matt Mahoney, mmahoney@cs.fit.edu

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation at
http://www.gnu.org/licenses/gpl.txt or (at your option) any later version.
This program is distributed without any warranty.


USAGE

To compress:      PAQ6 -3 archive file file...  (1 or more file names), or
  or (MSDOS):     dir/b | PAQ6 -3 archive       (read file names from input)
  or (UNIX):      ls    | PAQ6 -3 archive
To decompress:    PAQ6 archive                  (no option)
To list contents: more < archive

Compression:  The files listed are compressed and stored in the archive,
which is created.  The archive must not already exist.  File names may
specify a path, which is stored.  If there are no file names on the command
line, then PAQ6 prompts for them, reading until the first blank line or
end of file.

The -3 is optional, and is used to trade off compression vs. speed
and memory.  Valid options are -0 to -9.  Higher numbers compress better
but run slower and use more memory.  -3 is the default, and gives a
reasonable tradeoff.  Recommended options are:

  -0 to -2 for fast (2X over -3) but poor compression, uses 2-6 MB memory
  -3 for reasonably fast and good compression, uses 18 MB (default)
  -4 better compression but 3.5X slower, uses 64 MB
  -5 slightly better compression, 6X slower than -3, uses 154 MB 
  -6 about like -5, uses 202 MB memory
  -7 to -9 use 404 MB, 808 MB, 1616 MB, about the same speed as -5

Decompression:  No file names are specified.  The archive must exist.
If a path is stored, the file is extracted to the appropriate directory,
which must exist.  PAQ6 does not create directories.  If the file to be
extracted already exists, it is not replaced; rather it is compared with
the archived file, and the offset of the first difference is reported.
The decompressor requires as much memory as was used to compress.
There is no option.

It is not possible to add, remove, or update files in an existing archive.
If you want to do this, extract the files, delete the archive, and
create a new archive with just the files you want.


TO COMPILE

gxx -O PAQ6.cpp        DJGPP 2.95.2
bcc32 -O2 PAQ6.cpp     Borland 5.5.1
sc -o PAQ6.cpp         Digital Mars 8.35n

g++ -O produces the fastest executable among free compilers, followed
by Borland and Mars.  However Intel 8 will produce the fastest and smallest
Windows executable overall, followed by Microsoft VC++ .net 7.1 /O2 /G7


PAQ6 DESCRIPTION

1. OVERVIEW

A PAQ6 archive has a header, listing the names and lengths of the files
it contains in human-readable format, followed by the compressed data.
The first line of the header is "PAQ6 -m" where -m is the memory option.
The data is compressed as if all the files were concatenated into one
long string.

PAQ6 uses predictive arithmetic coding.  The string, y, is compressed
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

To simplify coding, PAQ6 uses a binary string alphabet.  Thus, the
output of a model is an estimate of P(y_i = 1 | context) (henceforth p),
where y_i is the i'th bit, and the context is the previous i - 1 bits of
uncompressed data.


2.  PAQ6 MODEL

The PAQ6 model consists of a weighted mix of independent submodels which
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

  Fig. 2.  PAQ6 Model details for compression.  The model is identical for
  decompression, but the encoder is replaced with a decoder.

In Sections 2-6, the description applies to the default memory option
(-5, or MEM = 5).  For smaller values of MEM, some components are
omitted and the number of contexts is less.


3.  MIXER

The mixers compute a probability by a weighted summation of the N
models.  Each model outputs two numbers, n0 and n1 represeting the
relative probability of a 0 or 1, respectively.  These are
combined using weighted summations to estimate the probability p
that the next bit will be a 1:

      SUM_i=1..N w_i n1_i                                               (3)
  p = -------------------,  n_i = n0_i + n1_i
      SUM_i=1..N w_i n_i

The weights w_i are adjusted after each bit of uncompressed data becomes
known in order to reduce the cost (code length) of that bit.  The cost
of a 1 bit is -log(p), and the cost of a 0 is -log(1-p).  We find the
gradient of the weight space by taking the partial derivatives of the
cost with respect to w_i, then adjusting w_i in the direction
of the gradient to reduce the cost.  This adjustment is:

  w_i := w_i + e[ny_i/(SUM_j (w_j+wo) ny_j) - n_i/(SUM_j (w_j+wo) n_j)]

where e and wo are small constants, and ny_i means n0_i if the actual
bit is a 0, or n1_i if the bit is a 1.  The weight offset wo prevents
the gradient from going to infinity as the weights go to 0.  e is set
to around .004, trading off between faster adaptation (larger e)
and less noise for better compression of stationary data (smaller e).

There are two mixers, whose outputs are averaged together before being
input to the SSE stage.  Each mixer maintains a set of weights which
is selected by a context.  Mixer 1 maintains 16 weight vectors, selected
by the 3 high order bits of the previous byte and on whether the data
is text or binary.  Mixer 2 maintains 16 weight vectors, selected by the
2 high order bits of each of the previous 2 bytes.

To distinguish text from binary data, we use the heuristic that space
characters are more common in text than NUL bytes, while NULs are more
common in binary data.  We compare the position of the 4th from last
space with the position of the 4th from last 0 byte.


4.  CONTEXT MODELS

Individual submodels output a prediction in the form of two numbers,
n0 and n1, representing relative probabilities of 0 and 1.  Generally
this is done by storing a pair of counters (c0,c1) in a hash table
indexed by context.  When a 0 or 1 is encountered in a context, the
appropriate count is increased by 1.  Also, in order to favor newer
data over old, the opposite count is decreased by the following
heuristic:

  If the count > 25 then replace with sqrt(count) + 6 (rounding down)
  Else if the count > 1 then replace with count / 2 (rounding down)

The outputs are derived from the counts in a way that favors highly
predictive contexts, i.e. those where one count is large and the
other is small.  For the case of c1 >= c0 the following heuristic
is used.

  If c0 = 0 then n0 = 0, n1 = 4 c0
  Else n0 = 1, n1 = c1 / c0

For the case of c1 < c0 we use the same heuristic swapping 0 and 1.

In the following example, we encounter a long string of zeros followed
by a string of ones and show the model output.  Note how n0 and n1 predict
the relative outcome of 0 and 1 respectively, favoring the most recent
data, with weight n = n0 + n1

  Input                 c0  c1  n0  n1
  -----                 --  --  --  --
  0000000000            10   0  40   0
  00000000001            5   1   5   1
  000000000011           2   2   1   1
  0000000000111          1   3   1   3
  00000000001111         1   4   1   4

  Table 1.  Example of counter state (c0,c1) and outputs (n0,n1)

In order to represent (c0,c1) as an 8-bit state, counts are restricted
to the values 0-40, 44, 48, 56, 64, 96, 128, 160, 192, 224, or 255.
Large counts are incremented probabilistically.  For example, if
c0 = 40 and a 0 is encountered, then c0 is set to 44 with
probability 1/4.  Decreases in counter values are deterministic,
and are rounded down to the nearest representable state.

Counters are stored in a hash table indexed by contexts starting
on byte boundaries and ending on nibble (4-bit) boundaries.  Each
hash element contains 15 counter states, representing the 15 possible
values for the 0-3 remaining bits of the context after the last nibble
boundary.  Hash collisions are detected by storing an 8-bit checksum of
the context.

Each bucket contains 4 elements in a move-to-front queue.  When a
new element is to be inserted, the priority of the two least recently
accessed elements are compared by using n (n0+n1) of the initial
counter as the priority, and the lower priority element is discarded.
Hash buckets are aligned on 64 byte addresses to minimize cache misses.


5.  RUN LENGTH MODELS

A second type of model is used to efficiently represent runs of
up to 255 identical bytes within a context.  For example, given the
sequence "abc...abc...abc..." then a run length model would map
"ab" -> ("c", 3) using a hash table indexed by "ab".  If a new
value is seen, e.g. "abd", then the state is updated to the new
character and a count of 1, i.e. "ab" -> ("d", 1).

A run length context is accessed 8 times, once for each bit.  If the
bits seen so far are consistent with the modeled character, then the output
of a run length model is (n0,n1) = (0,n) if the next bit is a 1,
or (n,0) if the next bit is a 0, where n is the count (1 to 255).
If the bits seen so far are not consistent with the predicted byte,
then the output is (0,0).  These counts are added to the counter state
counts to produce the model output.

Run lengths are stored in a hash table without collision detection,
so an element occupies 2 bytes.  Generally, most models store one run
length for every 8 counter pairs, so 20% of the memory is allocated to
them.  Run lengths are used only for memory option (-MEM) of 5 or higher.


6.  SUBMODEL DETAILS

Submodels differ mainly in their contexts.  These are as follows:

a. DefaultModel.  (n0,n1) = (1,1) regardless of context.

b. CharModel (N-gram model).  A context consists of the last 0 to N whole
bytes, plus the 0 to 7 bits of the partially read current byte.
The maximum N depends on the -MEM option as shown in the table below.
The order 0 and 1 contexts use a counter state lookup table rather
than a hash table.

  Order  Counters               Run lengths
  -----  --------               -----------
   0     2^8
   1     2^16
   2     2^(MEM+15)             2^(MEM+12), MEM >= 5
   3     2^(MEM+17)             2^(MEM+14), MEM >= 5
   4     2^(MEM+18)             2^(MEM+15), MEM >= 5
   5     2^(MEM+18), MEM >= 1   2^(MEM+15), MEM >= 5
   6     2^(MEM+18), MEM >= 3   2^(MEM+15), MEM >= 5
   7     2^(MEM+18), MEM >= 3   2^(MEM+15), MEM >= 5
   8     2^20, MEM = 5          2^17, MEM = 5
         2^(MEM+14), MEM >= 6   2^(MEM+14), MEM >= 6
   9     2^20, MEM = 5          2^17, MEM = 5
         2^(MEM+14), MEM >= 6   2^(MEM+14), MEM >= 6

  Table 2.  Number of modeled contexts of length 0-9

c.  MatchModel (long context).  A context is the last n whole bytes
(plus extra bits) where n >=8.  Up to 4 matching contexts are found by
indexing into a rotating input buffer whose size depends on MEM.  The
index is a hash table of 32-bit pointers with 1/4 as many elements as
the buffer (and therefore occupying an equal amount of memory).  The
table is indexed by a hashes of 8 byte contexts.  No collision detection
is used.  In order to detect very long matches at a long distance
(for example, versions of a file compressed together), 1/16 of the
pointers (chosen randomly) are indexed by a hash of a 32 byte context.

For each match found, the output is (n0,n1) = (w,0) or (0,w) (depending on
the next bit) with a weight of w = length^2 / 4 (maximum 511), depending
on the length of the context in bytes.  The four outputs are added together.

d.  RecordModel.  This models data with fixed length records, such as
tables.  The model attempts to find the record length by searching for
characters that repeat in the pattern x..x..x..x where the interval
between 4 successive occurrences is identical and at least 2.  Because
of uncertainty in this method, the two most recent values (which must
be different) are used.  The following 5 contexts are modeled;

  1. The two bytes above the current bit for each repeat length.
  2. The byte above and the previous byte (to the left) for each repeat
     length.
  3. The byte above and the current position modulo the repeat length,
     for the longer of the two lengths only.

e.  SparseModel.  This models contexts with gaps.  It considers the
following contexts, where x denotes the bytes considered and ? denotes
the bit being predicted (plus preceding bits, which are included in
the context).

       x.x?  (first and third byte back)
      x..x?
     x...x?
    x....x?
       xx.?
      x.x.?
      xx..?
  c ...  c?, gap length
  c ... xc?, gap length

  Table 3.  Sparse model contexts

The last two examples model variable gap lengths between the last byte
and its previous occurrence.  The length of the gap (up to 255) is part
of the context.

e.  AnalogModel.  This is intended to model 16-bit audio (mono or stereo),
24-bit color images, 8-bit data (such as grayscale images).  Contexts drop
the low order bits, and include the position within the file modulo
2, 3, or 4.  There are 8 models, combined into 4 by addition before
mixing.  An x represents those bits which are part of the context.

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

  CCITT images (1 bit per pixel, 216 bytes wide, e.g. calgary/pic)
    xxxxxxxx (skip 215 bytes...) xxxxxxxx (skip 215 bytes...) ?

  Table 4.  Analog models.

f.  WordModel.  This is intended to model text files.  There are
3 contexts:

  1.  The current word
  2.  The previous and current words
  3.  The second to last and current words (skipping a word)

A word is defined in two different ways, resulting in a total of 6
different contexts:

  1.  Any sequence of characters with ASCII code > 32 (not white space).
      Upper case characters are converted to lower case.
  2.  Any sequence of A-Z and a-z (case sensitive).

g.  ExeModel.  This models 32-bit Intel .exe and .dll files by changing
relative 32-bit CALL addresses to absolute.  These instructions have the
form (in hex) "E8 xx yy zz 00" or "E8 xx yy zz FF" where the 32-bit
operand is stored least significant byte first.  These are converted
to absolute addresses by adding the position of the E8 byte, and then
stored in a 256 element table indexed by the low order byte (xx) along
with an 8-bit count.  If another E8 xx ... 00/FF with the same value of
xx is encountered, then the old value is replaced and the count set back
to 1.

During modeling, when "E8 xx" is encountered, the bytes yy, zz, and 00/FF
are predicted by adjusting xx to absolute address, then looking up
the address in the table indexed by xx.  If the context matches the table
entry up to the current bit, then the next bit from the table is
predicted with weight n for yy, 4n for zz, and 16n for 00/FF, where n
is the count.


7.  SSE

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

SSE is context sensitive.  There are 2048 separately maintained SSE
functions, selected by the 0-7 bits of the current (partial) byte and
the 2 high order bits of the previous byte, and on whether the data
is text or binary, using the same heuristic as for selecting the mixer
context.

The final output to the encoder is a weighted average of the SSE
input and output, with the output receiving 3/4 of the weight:

  p := (3 SSE(p) + p) / 4.                                              (4)


8.  MEMORY USAGE

The -m option (MEM = 0 through 9) controls model and memory usage.  Smaller
numbers compress faster and use less memory, while higher numbers compress
better.

For MEM < 5, only one mixer is used.  For MEM < 4, bit counts are stored
in nonstationary counters, but no run length is stored (decreasing
memory by 20%).  For MEM < 1, SSE is not used.  For MEM < 5, the record,
sparse, and analog models are not used.  For MEM < 4, the word model is
not used.  The order of the char model ranges from 4 to 9 depending on
MEM for MEM as shown in Table 6.

             Run        Memory used by........................   Total
 MEM  Mixers Len  Order Char Match Record Sparse Analog Word SSE Memory (MB)
 ---  ------ ---  ----- ---- ----- ------ ------ ------ ---- --- -----------
  0     1    no   4      .5    1                                      1.5
  1     1    no   5       1    2                             .12      3
  2     1    no   5       2    4                             .12      6
  3     1    no   7      10    8                             .12     18
  4     1    no   7      20   16       6     6      1    15  .12     64
  5     2   yes   9      66   32      13    11      2    30  .12    154
  6     2   yes   9     112   32      13    11      4    30  .12    202
  7     2   yes   9     224   64      25    22      9    60  .12    404
  8     2   yes   9     448  128      50    45     18   120  .12    808
  9     2   yes   9     992  256     100    90     36   240  .12   1616

  Table 5.  Memory usage depending on MEM (-0 to -9 option).
  

9.  EXPERIMENTAL RESULTS

Results on the Calgary corpos are shown below for some top data compressors
as of Dec. 30, 2003.  Options are set for maximum compression.  When
possible, the files are all compressed into a single archive.  Run times
are on a 705 MHz Duron with 256 MB memory, and include 3 seconds to run
WRT when applicable.  PAQ6 was compiled with DJGPP (g++) 2.95.2 -O.

  Original size   Options        3,141,622  Time   Author
  -------------   -------        ---------  ----   ------
  gzip 1.2.4      -9             1,017,624     2   Jean Loup Gailly
  epm r9          c                668,115    49   Serge Osnach
  rkc             a -M80m -td+     661,102    91   Malcolm Taylor
  slim 20         a                659,213   159   Serge Voskoboynikov
  compressia 1.0 beta              650,398    66   Yaakov Gringeler
  durilca v.03a (as in README)     647,028    30   Dmitry Shkarin
  PAQ5                             661,811   361   Matt Mahoney
  WRT11 + PAQ5                     638,635   258   Przemyslaw Skibinski +
  PAQ6            -0               858,954    52
                  -1               750,031    66
                  -2               725,798    76
                  -3               709,806    97
                  -4               655,694   354
                  -5               648,951   625
                  -6               648,892   636
  WRT11 + PAQ6    -6               626,395   446
  WRT20 + PAQ6    -6               617,734   439

  Table 6.  Compressed size of the Calgary corpus.

WRT11 is a word reducing transform written by Przemyslaw Skibinski.  It
uses an external English dictionary to replace words with 1-3 byte
symbols to improve compression.  rkc, compressia, and durilca use a
similar approach.  WRT20 is a newer version of WRT11.


10.  ACKNOWLEDGMENTS

Thanks to Serge Osnach for introducing me to SSE (in PAQ1SSE/PAQ2) and
the sparse models (PAQ3N).  Also, credit to Eugene Shelwein,
Dmitry Shkarin for suggestions on using multiple character SSE contexts.
Credit to Eugene, Serge, and Jason Schmidt for developing faster and
smaller executables of previous versions.  Credit to Werner Bergmans
and Berto Destasio for testing and evaluating them, including modifications
that improve compression at the cost of more memory.  Credit to
Alexander Ratushnyak who found a bug in PAQ4 decompression, and also
in PAQ6 decompression for very small files (both fixed).

Thanks to Berto for writing PAQ5-EMILCONT-DEUTERIUM from which this
program is derived (which he derived from PAQ5).  His improvements to
PAQ5 include a new Counter state table and additional contexts for
CharModel and SparseModel.  I refined the state table by adding
more representable states and modified the return counts to give greater
weight when there is a large difference between the two counts.

I expect there will be better versions in the future.  If you make any
changes, please change the name of the program (e.g. PAQ7), including
the string in the archive header by redefining PROGNAME below.
This will prevent any confusion about versions or archive compatibility.
Also, give yourself credit in the help message.
*/

#define PROGNAME "KGB_arch"  // Please change this if you change the program

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
#include <map>
#undef hash
using namespace std;

const int PSCALE=4096;  // Integer scale for representing probabilities
int MEM=3;        // Use about 6 MB * 2^MEM bytes of memory                      

template <class T> inline int size(const T& t) {return t.size();}

// 8-32 bit unsigned types, adjust as appropriate
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;
//typedef unsigned long u32;

// Fail if out of memory
void handler() {
  printf("Out of memory\n");
  exit(1);
}

// A ProgramChecker verifies some environmental assumptions and sets the
// out of memory handler.  It also gets the program starting time.
// The global object programChecker should be initialized before any
// other global objects.

/*class ProgramChecker {
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
  }
  clock_t start_time() const {return start;}  // When the program started
} programChecker;*/

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

  get0() -- returns p(0) with weight n = get0()+get1()
  get1() -- returns p(1) with weight n
  add(y) -- increments n_y, where y is 0 or 1 and decreases n_1-y
  priority() -- Returns a priority (n) for hash replacement such that
    higher numbers should be favored.
*/

class Counter {
  U8 state;
  struct E {      // State table entry
    U16 n0, n1;   // get0(), get1()
    U8 s00, s01;  // Next state on input 0 without/with probabilistic incr.
    U8 s10, s11;  // Next state on input 1
    U32 p0, p1;   // Probability of increment x 2^32 on inputs 0, 1
  };
  static E table[];  // State table
public:
  Counter(): state(0) {}
  int get0() const {return table[state].n0;}
  int get1() const {return table[state].n1;}
  int priority() const {return get0()+get1();}
  void add(int y) {
    if (y) {
      if (state<208 || rnd()<table[state].p1)
        state=table[state].s11;
      else
        state=table[state].s10;
    }
    else {
      if (state<208 || rnd()<table[state].p0)
        state=table[state].s01;
      else
        state=table[state].s00;
    }
  }
};

// State table generated by stgen6.cpp
Counter::E Counter::table[] = {
//  get0 get1 s00 s01 s10 s11  p(s01)       p(s11)    state n0,n1
//  ---- ---- --- --- --- --- ---------  ----------   ----- -- --
    {  0,  0,  0,  2,  0,  1,4294967295u,4294967295u}, // 0 (0,0)
    {  0,  4,  1,  4,  1,  3,4294967295u,4294967295u}, // 1 (0,1)
    {  4,  0,  2,  5,  2,  4,4294967295u,4294967295u}, // 2 (1,0)
    {  0,  8,  1,  4,  3,  6,4294967295u,4294967295u}, // 3 (0,2)
    {  1,  1,  4,  8,  4,  7,4294967295u,4294967295u}, // 4 (1,1)
    {  8,  0,  5,  9,  2,  4,4294967295u,4294967295u}, // 5 (2,0)
    {  0, 12,  1,  4,  6, 10,4294967295u,4294967295u}, // 6 (0,3)
    {  1,  2,  4,  8,  7, 11,4294967295u,4294967295u}, // 7 (1,2)
    {  2,  1,  8, 13,  4,  7,4294967295u,4294967295u}, // 8 (2,1)
    { 12,  0,  9, 14,  2,  4,4294967295u,4294967295u}, // 9 (3,0)
    {  0, 16,  3,  7, 10, 15,4294967295u,4294967295u}, // 10 (0,4)
    {  1,  3,  4,  8, 11, 16,4294967295u,4294967295u}, // 11 (1,3)
    {  1,  1,  8, 13,  7, 11,4294967295u,4294967295u}, // 12 (2,2)
    {  3,  1, 13, 19,  4,  7,4294967295u,4294967295u}, // 13 (3,1)
    { 16,  0, 14, 20,  5,  8,4294967295u,4294967295u}, // 14 (4,0)
    {  0, 20,  3,  7, 15, 21,4294967295u,4294967295u}, // 15 (0,5)
    {  1,  4,  7, 12, 16, 22,4294967295u,4294967295u}, // 16 (1,4)
    {  1,  1,  8, 13, 11, 16,4294967295u,4294967295u}, // 17 (2,3)
    {  1,  1, 13, 19,  7, 11,4294967295u,4294967295u}, // 18 (3,2)
    {  4,  1, 19, 26,  8, 12,4294967295u,4294967295u}, // 19 (4,1)
    { 20,  0, 20, 27,  5,  8,4294967295u,4294967295u}, // 20 (5,0)
    {  0, 24,  6, 11, 21, 28,4294967295u,4294967295u}, // 21 (0,6)
    {  1,  5,  7, 12, 22, 29,4294967295u,4294967295u}, // 22 (1,5)
    {  1,  2, 12, 18, 16, 22,4294967295u,4294967295u}, // 23 (2,4)
    {  1,  1, 13, 19, 11, 16,4294967295u,4294967295u}, // 24 (3,3)
    {  2,  1, 19, 26, 12, 17,4294967295u,4294967295u}, // 25 (4,2)
    {  5,  1, 26, 34,  8, 12,4294967295u,4294967295u}, // 26 (5,1)
    { 24,  0, 27, 35,  9, 13,4294967295u,4294967295u}, // 27 (6,0)
    {  0, 28,  6, 11, 28, 36,4294967295u,4294967295u}, // 28 (0,7)
    {  1,  6, 11, 17, 29, 37,4294967295u,4294967295u}, // 29 (1,6)
    {  1,  2, 12, 18, 22, 29,4294967295u,4294967295u}, // 30 (2,5)
    {  1,  1, 18, 25, 16, 22,4294967295u,4294967295u}, // 31 (3,4)
    {  1,  1, 19, 26, 17, 23,4294967295u,4294967295u}, // 32 (4,3)
    {  2,  1, 26, 34, 12, 17,4294967295u,4294967295u}, // 33 (5,2)
    {  6,  1, 34, 43, 13, 18,4294967295u,4294967295u}, // 34 (6,1)
    { 28,  0, 35, 44,  9, 13,4294967295u,4294967295u}, // 35 (7,0)
    {  0, 32, 10, 16, 36, 45,4294967295u,4294967295u}, // 36 (0,8)
    {  1,  7, 11, 17, 37, 46,4294967295u,4294967295u}, // 37 (1,7)
    {  1,  3, 17, 24, 29, 37,4294967295u,4294967295u}, // 38 (2,6)
    {  1,  1, 18, 25, 22, 29,4294967295u,4294967295u}, // 39 (3,5)
    {  1,  1, 25, 33, 23, 30,4294967295u,4294967295u}, // 40 (4,4)
    {  1,  1, 26, 34, 17, 23,4294967295u,4294967295u}, // 41 (5,3)
    {  3,  1, 34, 43, 18, 24,4294967295u,4294967295u}, // 42 (6,2)
    {  7,  1, 43, 53, 13, 18,4294967295u,4294967295u}, // 43 (7,1)
    { 32,  0, 44, 54, 14, 19,4294967295u,4294967295u}, // 44 (8,0)
    {  0, 36, 10, 16, 45, 55,4294967295u,4294967295u}, // 45 (0,9)
    {  1,  8, 16, 23, 46, 56,4294967295u,4294967295u}, // 46 (1,8)
    {  1,  3, 17, 24, 37, 46,4294967295u,4294967295u}, // 47 (2,7)
    {  1,  2, 24, 32, 29, 37,4294967295u,4294967295u}, // 48 (3,6)
    {  1,  1, 25, 33, 30, 38,4294967295u,4294967295u}, // 49 (4,5)
    {  1,  1, 33, 42, 23, 30,4294967295u,4294967295u}, // 50 (5,4)
    {  2,  1, 34, 43, 24, 31,4294967295u,4294967295u}, // 51 (6,3)
    {  3,  1, 43, 53, 18, 24,4294967295u,4294967295u}, // 52 (7,2)
    {  8,  1, 53, 61, 19, 25,4294967295u,4294967295u}, // 53 (8,1)
    { 36,  0, 54, 62, 14, 19,4294967295u,4294967295u}, // 54 (9,0)
    {  0, 40, 15, 22, 55, 63,4294967295u,4294967295u}, // 55 (0,10)
    {  1,  9, 16, 23, 56, 64,4294967295u,4294967295u}, // 56 (1,9)
    {  1,  4, 23, 31, 46, 56,4294967295u,4294967295u}, // 57 (2,8)
    {  1,  2, 24, 32, 37, 46,4294967295u,4294967295u}, // 58 (3,7)
    {  2,  1, 43, 53, 24, 31,4294967295u,4294967295u}, // 59 (7,3)
    {  4,  1, 53, 61, 25, 32,4294967295u,4294967295u}, // 60 (8,2)
    {  9,  1, 61, 69, 19, 25,4294967295u,4294967295u}, // 61 (9,1)
    { 40,  0, 62, 70, 20, 26,4294967295u,4294967295u}, // 62 (10,0)
    {  0, 44, 15, 22, 63, 71,4294967295u,4294967295u}, // 63 (0,11)
    {  1, 10, 22, 30, 64, 72,4294967295u,4294967295u}, // 64 (1,10)
    {  1,  4, 23, 31, 56, 64,4294967295u,4294967295u}, // 65 (2,9)
    {  1,  2, 31, 40, 46, 56,4294967295u,4294967295u}, // 66 (3,8)
    {  2,  1, 53, 61, 32, 40,4294967295u,4294967295u}, // 67 (8,3)
    {  4,  1, 61, 69, 25, 32,4294967295u,4294967295u}, // 68 (9,2)
    { 10,  1, 69, 77, 26, 33,4294967295u,4294967295u}, // 69 (10,1)
    { 44,  0, 70, 78, 20, 26,4294967295u,4294967295u}, // 70 (11,0)
    {  0, 48, 21, 29, 71, 79,4294967295u,4294967295u}, // 71 (0,12)
    {  1, 11, 22, 30, 72, 80,4294967295u,4294967295u}, // 72 (1,11)
    {  1,  5, 30, 39, 64, 72,4294967295u,4294967295u}, // 73 (2,10)
    {  1,  3, 31, 40, 56, 64,4294967295u,4294967295u}, // 74 (3,9)
    {  3,  1, 61, 69, 32, 40,4294967295u,4294967295u}, // 75 (9,3)
    {  5,  1, 69, 77, 33, 41,4294967295u,4294967295u}, // 76 (10,2)
    { 11,  1, 77, 85, 26, 33,4294967295u,4294967295u}, // 77 (11,1)
    { 48,  0, 78, 86, 27, 34,4294967295u,4294967295u}, // 78 (12,0)
    {  0, 52, 21, 29, 79, 87,4294967295u,4294967295u}, // 79 (0,13)
    {  1, 12, 29, 38, 80, 88,4294967295u,4294967295u}, // 80 (1,12)
    {  1,  5, 30, 39, 72, 80,4294967295u,4294967295u}, // 81 (2,11)
    {  1,  3, 39, 49, 64, 72,4294967295u,4294967295u}, // 82 (3,10)
    {  3,  1, 69, 77, 41, 50,4294967295u,4294967295u}, // 83 (10,3)
    {  5,  1, 77, 85, 33, 41,4294967295u,4294967295u}, // 84 (11,2)
    { 12,  1, 85, 91, 34, 42,4294967295u,4294967295u}, // 85 (12,1)
    { 52,  0, 86, 92, 27, 34,4294967295u,4294967295u}, // 86 (13,0)
    {  0, 56, 28, 37, 87, 93,4294967295u,4294967295u}, // 87 (0,14)
    {  1, 13, 29, 38, 88, 94,4294967295u,4294967295u}, // 88 (1,13)
    {  1,  6, 38, 48, 80, 88,4294967295u,4294967295u}, // 89 (2,12)
    {  6,  1, 85, 91, 42, 51,4294967295u,4294967295u}, // 90 (12,2)
    { 13,  1, 91, 97, 34, 42,4294967295u,4294967295u}, // 91 (13,1)
    { 56,  0, 92, 98, 35, 43,4294967295u,4294967295u}, // 92 (14,0)
    {  0, 60, 28, 37, 93, 99,4294967295u,4294967295u}, // 93 (0,15)
    {  1, 14, 37, 47, 94,100,4294967295u,4294967295u}, // 94 (1,14)
    {  1,  6, 38, 48, 88, 94,4294967295u,4294967295u}, // 95 (2,13)
    {  6,  1, 91, 97, 42, 51,4294967295u,4294967295u}, // 96 (13,2)
    { 14,  1, 97,103, 43, 52,4294967295u,4294967295u}, // 97 (14,1)
    { 60,  0, 98,104, 35, 43,4294967295u,4294967295u}, // 98 (15,0)
    {  0, 64, 36, 46, 99,105,4294967295u,4294967295u}, // 99 (0,16)
    {  1, 15, 37, 47,100,106,4294967295u,4294967295u}, // 100 (1,15)
    {  1,  7, 47, 58, 94,100,4294967295u,4294967295u}, // 101 (2,14)
    {  7,  1, 97,103, 52, 59,4294967295u,4294967295u}, // 102 (14,2)
    { 15,  1,103,109, 43, 52,4294967295u,4294967295u}, // 103 (15,1)
    { 64,  0,104,110, 44, 53,4294967295u,4294967295u}, // 104 (16,0)
    {  0, 68, 36, 46,105,111,4294967295u,4294967295u}, // 105 (0,17)
    {  1, 16, 46, 57,106,112,4294967295u,4294967295u}, // 106 (1,16)
    {  1,  7, 47, 58,100,106,4294967295u,4294967295u}, // 107 (2,15)
    {  7,  1,103,109, 52, 59,4294967295u,4294967295u}, // 108 (15,2)
    { 16,  1,109,113, 53, 60,4294967295u,4294967295u}, // 109 (16,1)
    { 68,  0,110,114, 44, 53,4294967295u,4294967295u}, // 110 (17,0)
    {  0, 72, 45, 56,111,115,4294967295u,4294967295u}, // 111 (0,18)
    {  1, 17, 46, 57,112,116,4294967295u,4294967295u}, // 112 (1,17)
    { 17,  1,113,119, 53, 60,4294967295u,4294967295u}, // 113 (17,1)
    { 72,  0,114,120, 54, 61,4294967295u,4294967295u}, // 114 (18,0)
    {  0, 76, 45, 56,115,121,4294967295u,4294967295u}, // 115 (0,19)
    {  1, 18, 56, 65,116,122,4294967295u,4294967295u}, // 116 (1,18)
    {  1,  8, 57, 66,112,116,4294967295u,4294967295u}, // 117 (2,17)
    {  8,  1,113,119, 60, 67,4294967295u,4294967295u}, // 118 (17,2)
    { 18,  1,119,125, 61, 68,4294967295u,4294967295u}, // 119 (18,1)
    { 76,  0,120,126, 54, 61,4294967295u,4294967295u}, // 120 (19,0)
    {  0, 80, 55, 64,121,127,4294967295u,4294967295u}, // 121 (0,20)
    {  1, 19, 56, 65,122,128,4294967295u,4294967295u}, // 122 (1,19)
    {  1,  9, 65, 74,116,122,4294967295u,4294967295u}, // 123 (2,18)
    {  9,  1,119,125, 68, 75,4294967295u,4294967295u}, // 124 (18,2)
    { 19,  1,125,131, 61, 68,4294967295u,4294967295u}, // 125 (19,1)
    { 80,  0,126,132, 62, 69,4294967295u,4294967295u}, // 126 (20,0)
    {  0, 84, 55, 64,127,133,4294967295u,4294967295u}, // 127 (0,21)
    {  1, 20, 64, 73,128,134,4294967295u,4294967295u}, // 128 (1,20)
    {  1,  9, 65, 74,122,128,4294967295u,4294967295u}, // 129 (2,19)
    {  9,  1,125,131, 68, 75,4294967295u,4294967295u}, // 130 (19,2)
    { 20,  1,131,137, 69, 76,4294967295u,4294967295u}, // 131 (20,1)
    { 84,  0,132,138, 62, 69,4294967295u,4294967295u}, // 132 (21,0)
    {  0, 88, 63, 72,133,139,4294967295u,4294967295u}, // 133 (0,22)
    {  1, 21, 64, 73,134,140,4294967295u,4294967295u}, // 134 (1,21)
    {  1, 10, 73, 82,128,134,4294967295u,4294967295u}, // 135 (2,20)
    { 10,  1,131,137, 76, 83,4294967295u,4294967295u}, // 136 (20,2)
    { 21,  1,137,143, 69, 76,4294967295u,4294967295u}, // 137 (21,1)
    { 88,  0,138,144, 70, 77,4294967295u,4294967295u}, // 138 (22,0)
    {  0, 92, 63, 72,139,145,4294967295u,4294967295u}, // 139 (0,23)
    {  1, 22, 72, 81,140,146,4294967295u,4294967295u}, // 140 (1,22)
    {  1, 10, 73, 82,134,140,4294967295u,4294967295u}, // 141 (2,21)
    { 10,  1,137,143, 76, 83,4294967295u,4294967295u}, // 142 (21,2)
    { 22,  1,143,147, 77, 84,4294967295u,4294967295u}, // 143 (22,1)
    { 92,  0,144,148, 70, 77,4294967295u,4294967295u}, // 144 (23,0)
    {  0, 96, 71, 80,145,149,4294967295u,4294967295u}, // 145 (0,24)
    {  1, 23, 72, 81,146,150,4294967295u,4294967295u}, // 146 (1,23)
    { 23,  1,147,151, 77, 84,4294967295u,4294967295u}, // 147 (23,1)
    { 96,  0,148,152, 78, 85,4294967295u,4294967295u}, // 148 (24,0)
    {  0,100, 63, 72,149,153,4294967295u,4294967295u}, // 149 (0,25)
    {  1, 24, 80, 89,150,154,4294967295u,4294967295u}, // 150 (1,24)
    { 24,  1,151,155, 85, 90,4294967295u,4294967295u}, // 151 (24,1)
    {100,  0,152,156, 70, 77,4294967295u,4294967295u}, // 152 (25,0)
    {  0,104, 63, 72,153,157,4294967295u,4294967295u}, // 153 (0,26)
    {  1, 25, 72, 81,154,158,4294967295u,4294967295u}, // 154 (1,25)
    { 25,  1,155,159, 77, 84,4294967295u,4294967295u}, // 155 (25,1)
    {104,  0,156,160, 70, 77,4294967295u,4294967295u}, // 156 (26,0)
    {  0,108, 63, 72,157,161,4294967295u,4294967295u}, // 157 (0,27)
    {  1, 26, 72, 81,158,162,4294967295u,4294967295u}, // 158 (1,26)
    { 26,  1,159,163, 77, 84,4294967295u,4294967295u}, // 159 (26,1)
    {108,  0,160,164, 70, 77,4294967295u,4294967295u}, // 160 (27,0)
    {  0,112, 63, 72,161,165,4294967295u,4294967295u}, // 161 (0,28)
    {  1, 27, 72, 81,162,166,4294967295u,4294967295u}, // 162 (1,27)
    { 27,  1,163,167, 77, 84,4294967295u,4294967295u}, // 163 (27,1)
    {112,  0,164,168, 70, 77,4294967295u,4294967295u}, // 164 (28,0)
    {  0,116, 63, 72,165,169,4294967295u,4294967295u}, // 165 (0,29)
    {  1, 28, 72, 81,166,170,4294967295u,4294967295u}, // 166 (1,28)
    { 28,  1,167,171, 77, 84,4294967295u,4294967295u}, // 167 (28,1)
    {116,  0,168,172, 70, 77,4294967295u,4294967295u}, // 168 (29,0)
    {  0,120, 63, 72,169,173,4294967295u,4294967295u}, // 169 (0,30)
    {  1, 29, 72, 81,170,174,4294967295u,4294967295u}, // 170 (1,29)
    { 29,  1,171,175, 77, 84,4294967295u,4294967295u}, // 171 (29,1)
    {120,  0,172,176, 70, 77,4294967295u,4294967295u}, // 172 (30,0)
    {  0,124, 63, 72,173,177,4294967295u,4294967295u}, // 173 (0,31)
    {  1, 30, 72, 81,174,178,4294967295u,4294967295u}, // 174 (1,30)
    { 30,  1,175,179, 77, 84,4294967295u,4294967295u}, // 175 (30,1)
    {124,  0,176,180, 70, 77,4294967295u,4294967295u}, // 176 (31,0)
    {  0,128, 63, 72,177,181,4294967295u,4294967295u}, // 177 (0,32)
    {  1, 31, 72, 81,178,182,4294967295u,4294967295u}, // 178 (1,31)
    { 31,  1,179,183, 77, 84,4294967295u,4294967295u}, // 179 (31,1)
    {128,  0,180,184, 70, 77,4294967295u,4294967295u}, // 180 (32,0)
    {  0,132, 63, 72,181,185,4294967295u,4294967295u}, // 181 (0,33)
    {  1, 32, 72, 81,182,186,4294967295u,4294967295u}, // 182 (1,32)
    { 32,  1,183,187, 77, 84,4294967295u,4294967295u}, // 183 (32,1)
    {132,  0,184,188, 70, 77,4294967295u,4294967295u}, // 184 (33,0)
    {  0,136, 63, 72,185,189,4294967295u,4294967295u}, // 185 (0,34)
    {  1, 33, 72, 81,186,190,4294967295u,4294967295u}, // 186 (1,33)
    { 33,  1,187,191, 77, 84,4294967295u,4294967295u}, // 187 (33,1)
    {136,  0,188,192, 70, 77,4294967295u,4294967295u}, // 188 (34,0)
    {  0,140, 63, 72,189,193,4294967295u,4294967295u}, // 189 (0,35)
    {  1, 34, 72, 81,190,194,4294967295u,4294967295u}, // 190 (1,34)
    { 34,  1,191,195, 77, 84,4294967295u,4294967295u}, // 191 (34,1)
    {140,  0,192,196, 70, 77,4294967295u,4294967295u}, // 192 (35,0)
    {  0,144, 71, 80,193,197,4294967295u,4294967295u}, // 193 (0,36)
    {  1, 35, 72, 81,194,198,4294967295u,4294967295u}, // 194 (1,35)
    { 35,  1,195,199, 77, 84,4294967295u,4294967295u}, // 195 (35,1)
    {144,  0,196,200, 78, 85,4294967295u,4294967295u}, // 196 (36,0)
    {  0,148, 71, 80,197,201,4294967295u,4294967295u}, // 197 (0,37)
    {  1, 36, 80, 89,198,202,4294967295u,4294967295u}, // 198 (1,36)
    { 36,  1,199,203, 85, 90,4294967295u,4294967295u}, // 199 (36,1)
    {148,  0,200,204, 78, 85,4294967295u,4294967295u}, // 200 (37,0)
    {  0,152, 71, 80,201,205,4294967295u,4294967295u}, // 201 (0,38)
    {  1, 37, 80, 89,202,206,4294967295u,4294967295u}, // 202 (1,37)
    { 37,  1,203,207, 85, 90,4294967295u,4294967295u}, // 203 (37,1)
    {152,  0,204,208, 78, 85,4294967295u,4294967295u}, // 204 (38,0)
    {  0,156, 71, 80,205,209,4294967295u,4294967295u}, // 205 (0,39)
    {  1, 38, 80, 89,206,210,4294967295u,4294967295u}, // 206 (1,38)
    { 38,  1,207,211, 85, 90,4294967295u,4294967295u}, // 207 (38,1)
    {156,  0,208,212, 78, 85,4294967295u,4294967295u}, // 208 (39,0)
    {  0,160, 71, 80,209,215,4294967295u,1073741823u}, // 209 (0,40)
    {  1, 39, 80, 89,210,213,4294967295u,4294967295u}, // 210 (1,39)
    { 39,  1,211,214, 85, 90,4294967295u,4294967295u}, // 211 (39,1)
    {160,  0,212,216, 78, 85,1073741823u,4294967295u}, // 212 (40,0)
    {  1, 40, 80, 89,213,217,4294967295u,1073741823u}, // 213 (1,40)
    { 40,  1,214,218, 85, 90,1073741823u,4294967295u}, // 214 (40,1)
    {  0,176, 71, 80,215,219,4294967295u,1073741823u}, // 215 (0,44)
    {176,  0,216,220, 78, 85,1073741823u,4294967295u}, // 216 (44,0)
    {  1, 44, 80, 89,217,221,4294967295u,1073741823u}, // 217 (1,44)
    { 44,  1,218,222, 85, 90,1073741823u,4294967295u}, // 218 (44,1)
    {  0,192, 71, 80,219,223,4294967295u, 536870911u}, // 219 (0,48)
    {192,  0,220,224, 78, 85, 536870911u,4294967295u}, // 220 (48,0)
    {  1, 48, 80, 89,221,225,4294967295u, 536870911u}, // 221 (1,48)
    { 48,  1,222,226, 85, 90, 536870911u,4294967295u}, // 222 (48,1)
    {  0,224, 79, 88,223,227,4294967295u, 536870911u}, // 223 (0,56)
    {224,  0,224,228, 86, 91, 536870911u,4294967295u}, // 224 (56,0)
    {  1, 56, 88, 95,225,229,4294967295u, 536870911u}, // 225 (1,56)
    { 56,  1,226,230, 91, 96, 536870911u,4294967295u}, // 226 (56,1)
    {  0,256, 87, 94,227,231,4294967295u, 134217727u}, // 227 (0,64)
    {256,  0,228,232, 92, 97, 134217727u,4294967295u}, // 228 (64,0)
    {  1, 64, 94,101,229,233,4294967295u, 134217727u}, // 229 (1,64)
    { 64,  1,230,234, 97,102, 134217727u,4294967295u}, // 230 (64,1)
    {  0,384, 93,100,231,235,4294967295u, 134217727u}, // 231 (0,96)
    {384,  0,232,236, 98,103, 134217727u,4294967295u}, // 232 (96,0)
    {  1, 96,100,107,233,237,4294967295u, 134217727u}, // 233 (1,96)
    { 96,  1,234,238,103,108, 134217727u,4294967295u}, // 234 (96,1)
    {  0,512,105,112,235,239,4294967295u, 134217727u}, // 235 (0,128)
    {512,  0,236,240,110,113, 134217727u,4294967295u}, // 236 (128,0)
    {  1,128,112,117,237,241,4294967295u, 134217727u}, // 237 (1,128)
    {128,  1,238,242,113,118, 134217727u,4294967295u}, // 238 (128,1)
    {  0,640,111,116,239,243,4294967295u, 134217727u}, // 239 (0,160)
    {640,  0,240,244,114,119, 134217727u,4294967295u}, // 240 (160,0)
    {  1,160,116,123,241,245,4294967295u, 134217727u}, // 241 (1,160)
    {160,  1,242,246,119,124, 134217727u,4294967295u}, // 242 (160,1)
    {  0,768,115,122,243,247,4294967295u, 134217727u}, // 243 (0,192)
    {768,  0,244,248,120,125, 134217727u,4294967295u}, // 244 (192,0)
    {  1,192,122,129,245,249,4294967295u, 134217727u}, // 245 (1,192)
    {192,  1,246,250,125,130, 134217727u,4294967295u}, // 246 (192,1)
    {  0,896,121,128,247,251,4294967295u, 138547332u}, // 247 (0,224)
    {896,  0,248,252,126,131, 138547332u,4294967295u}, // 248 (224,0)
    {  1,224,128,135,249,253,4294967295u, 138547332u}, // 249 (1,224)
    {224,  1,250,254,131,136, 138547332u,4294967295u}, // 250 (224,1)
    { 0,1020,127,134,251,251,4294967295u,         0u}, // 251 (0,255)
    {1020, 0,252,252,132,137,         0u,4294967295u}, // 252 (255,0)
    {  1,255,134,141,253,253,4294967295u,         0u}, // 253 (1,255)
    {255,  1,254,254,137,142,         0u,4294967295u}  // 254 (255,1)
};

//////////////////////////// ch ////////////////////////////

/* ch is a global object that provides common services to models.
It stores all the input so far in a rotating buffer of the last N bytes

  ch -- Global object
  ch.init() -- Initialize (after MEM is set)
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
  U32 N;  // Buffer size
  U8 *buf;  // [N] last N bytes
  U32 p;  // pos()
  U32 bp;  // bpos()
  U32 hi_nibble, lo_nibble;  // hi(), lo()
  U32 lpos[256][4];  // pos(c, i)
public:
  Ch(): N(0), buf(0), p(0), bp(0), hi_nibble(0), lo_nibble(1) {
    memset(lpos, 0, 256*4*sizeof(U32));
  }
  void init() {
    N = 1 << 19+MEM-(MEM>=6);
    buf=(U8*)calloc(N, 1);
    if (!buf)
      handler();
    buf[0]=1;
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
    HashElement(): checksum(0) {}
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
Hashtable<T>::Hashtable(U32 n): N(n>4?n-4:1), table(0), cxt(0) {
  assert(sizeof(HashElement)==16);
  assert(sizeof(char)==1);

  // Align the hash table on a 64 byte cache page boundary
  char *p=(char*)calloc((16<<N)+64, 1);
  if (!p)
    handler();
  p+=64-(((int)p)&63);  // Aligned
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
  enum {N=64};  // Max writes before update
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
  while (sum>2000000000/PSCALE) sum/=4, n1/=4;
  assert(sum>0);
  return (PSCALE-1)*n1/sum;
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
  if (n==0)
    return;
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
  enum {MINMEM=5};  // Lowest MEM to use 2 mixers
  Mixer m1, m2;
public:
  MultiMixer(): m1(16), m2(16) {}
  void write(int n0, int n1) {
    m1.write(n0, n1);
    if (MEM>=MINMEM)
      m2.write(n0, n1);
  }
  void add(int n0, int n1) {
    if (MEM>=MINMEM) {
      m1.add(n0, n1);
      m2.add(n0, n1);
    }
    else
      m1.add(n0, n1);
  }
  int predict() {
    U32 p1=m1.predict((ch(1) >> 5) + 8*(ch.pos(0, 3) < ch.pos(32, 3)));
    if (MEM>=MINMEM) {
      U32 p2=m2.predict((ch(1) >> 6)+4*(ch(2) >> 6));
      return (p1+p2)/2;
    }
    else
      return p1;
  }
  void update(int y) {
    m1.update(y);
      if (MEM>=MINMEM)
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

// Stores only the most recent byte and its count per context (run length)
// in a hash table without collision detection
class CounterMap1 {
  const int N;
  struct S {
    U8 c;  // char
    U8 n;  // count
  };
  S* t;  // cxt -> c repeated last n times
  U32 cxt;
public:
  CounterMap1(int n): N(n>1?n-1:1), cxt(0) {
    assert(sizeof(S)==2);
    t=(S*)calloc(1<<N, 2);
    if (!t)
      handler();
  }
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

// Uses a nibble-oriented hash table of contexts (counter state)
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
  enum {MINMEM=5};  // Smallest MEM to use cm1
  CounterMap1 cm1;
  CounterMap2 cm2;
public:
  CounterMap3(int n): cm1(MEM>=MINMEM ? n-2 : 0), cm2(n) {}
  void update(U32 h) {
    if (MEM>=MINMEM)
      cm1.update(h);
    cm2.update(h);
  }
  void write() {
    cm2.write();
    if (MEM>=MINMEM)
      cm1.add();
  }
  void add() {
    cm2.add();
    if (MEM>=MINMEM)
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
};

//////////////////////////// charModel ////////////////////////////

// A CharModel contains n-gram models from 0 to 9

class CharModel: public Model {
  enum {N=10};        // Number of models
  Counter *t0, *t1;   // Model orders 0, 1 [256], [65536]
  CounterMap t2, t3, t4, t5, t6, t7, t8, t9;  // Model orders 2-9
  U32 *cxt;           // Context hashes [N]
  Counter *cp0, *cp1; // Pointers to counters in t0, t1
public:
  CharModel(): t0(new Counter[256]), t1(new Counter[65536]),	
               t2(MEM+15), t3(MEM+17), t4(MEM+18), t5((MEM>=1)*(MEM+18)),
               t6((MEM>=3)*(MEM+18)), t7((MEM>=3)*(MEM+18)),
               t8((MEM>=5)*(MEM+18-(MEM>=6))),
               t9((MEM>=5)*(MEM+18-(MEM>=6))),
               cxt(new U32[N]) {
    cp0=&t0[0];
    cp1=&t1[0];
    memset(cxt, 0, N*sizeof(U32));
    memset(t0, 0, 256*sizeof(Counter));
    memset(t1, 0, 65536*sizeof(Counter));
  }
  void model();         // Update and predict
};

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
    if (MEM>=1)
      t5.update(cxt[5]);
    if (MEM>=3) {
      t6.update(cxt[6]);
      t7.update(cxt[7]);
    }
    if (MEM>=5) {
      t8.update(cxt[8]);
      t9.update(cxt[9]);
    }
  }
  cp0=&t0[ch()];
  cp1=&t1[ch()+256*ch(1)];

  // Write predictions to the mixer
  mixer.write(cp0->get0(), cp0->get1());
  mixer.write(cp1->get0(), cp1->get1());
  t2.write();
  t3.write();
  t4.write();
  if (MEM>=1)
    t5.add();
  if (MEM>=3) {
    t6.write();
    t7.add();
  }
  if (MEM>=5) {
    t8.write();
    t9.add();
  }
}

//////////////////////////// matchModel ////////////////////////////

/* A MatchModel looks for a match of length n >= 8 bytes between
the current context and previous input, and predicts the next bit
in the previous context with weight n.  If the next bit is 1, then
the mixer is assigned (0, n), else (n, 0).  Matchies are found using
an index (a hash table of pointers into ch). */

class MatchModel: public Model {
  const int N;      // 2^N = hash table size
  enum {M=4};       // Number of strings to match
  U32 hash[2];      // Hashes of current context up to pos-1
  U32 begin[M];     // Points to first matching byte
  U32 end[M];       // Points to last matching byte + 1, 0 if no match
  U32 *ptr;         // Hash table of pointers [2^(MEM+17)]
public:
  MatchModel(): N(17+MEM-(MEM>=6)), ptr(new U32[1 << N]) {
    memset(ptr, 0, (1 << N)*sizeof(U32));                             
    hash[0]=hash[1]=0;
    for (int i=0; i<M; ++i)
      begin[i]=end[i]=0;
  }
  void model();
};

inline void MatchModel::model() {
  if (ch.bpos()==0) {  // New byte
    hash[0]=hash[0]*(16*56797157)+ch(1)+1;  // Hash last 8 bytes
    hash[1]=hash[1]*(2*45684217)+ch(1)+1;   // Hash last 32 bytes
    U32 h=hash[0] >> (32-N);
    if ((hash[0]>>28)==0)
      h=hash[1] >> (32-N);  // 1/16 of 8-contexts are hashed to 32 bytes   
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
    if (end[i]) { 
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
"x..x..x..x" would imply a record size of 3.  There are models for
the 2 most recent, different record lengths of at least 2. */

class RecordModel: public Model {
  const int SIZE;
  enum {N=2};           // Number of models
  CounterMap t0, t1, t2, t3, t4;  // Model
  int repeat1, repeat2;  // 2 last cycle lengths
public:
  RecordModel(): SIZE((MEM>=4)*(16+MEM-(MEM>=6))),
                 t0(SIZE), t1(SIZE), t2(SIZE), t3(SIZE), t4(SIZE),
                 repeat1(2), repeat2(3) {}
  void model(); 
};

// Update the model with bit y, then put predictions of the next update
// as 0 counts in n0[0..N-1] and 1 counts in n1[0..N-1]
inline void RecordModel::model() {
  if (ch.bpos()==0) {

    // Check for a repeating pattern of interval 3 or more
    const int c=ch(1);
    const int d1=ch.pos(c,0)-ch.pos(c,1);
    const int d2=ch.pos(c,1)-ch.pos(c,2);
    const int d3=ch.pos(c,2)-ch.pos(c,3);
    if (d1>1 && d1==d2 && d2==d3) {
      if (d1==repeat1)
        swap(repeat1, repeat2);
      else if (d1!=repeat2) {
        repeat1=repeat2;
        repeat2=d1;
      }
    }

    // Compute context hashes
    int r1=repeat1, r2=repeat2;
    if (r1>r2)
      swap(r1, r2);
    t0.update(hash(ch(r1), ch(r1*2), r1));  // 2 above (shorter repeat)
    t1.update(hash(ch(1), ch(r1), r1));     // above and left
    t2.update(hash(ch(r1), ch.pos()%r1));   // above and pos
    t3.update(hash(ch(r2), ch(r2*2), r2));  // 2 above (longer repeat)
    t4.update(hash(ch(1), ch(r2), r2));     // above and left
  }
  t0.write();
  t1.write();
  t2.write();
  t3.write();
  t4.write();
}

//////////////////////////// sparseModel ////////////////////////////

// A SparseModel models several order-2 contexts with gaps

class SparseModel: public Model {
  const int SIZE;
  enum {N=10};   // Number of models
  CounterMap t0, t1, t2, t3, t4, t5, t6, t7, t8;  // Sparse models
public:
  SparseModel(): SIZE((MEM>=4)*(MEM+15-(MEM>=6))),
                 t0(SIZE), t1(SIZE), t2(SIZE), t3(SIZE), t4(SIZE),
                 t5(SIZE), t6(SIZE), t7(SIZE), t8(SIZE) {}
  void model();  // Update and predict
};

inline void SparseModel::model() {

  // Update context
  if (ch.bpos()==0) {
    t0.update(hash(ch(1), ch(3)));
    t1.update(hash(ch(1), ch(4)));
    t2.update(hash(ch(1), ch(5)));
    t3.update(hash(ch(1), ch(6)));
    t4.update(hash(ch(2), ch(3)));
    t5.update(hash(ch(2), ch(4)));
    t6.update(hash(ch(3), ch(4)));
    const int g=min(255, int(ch.pos()-ch.pos(ch(1), 2))); // gap to prior ch1
    t7.update(hash(ch(1), g));
    t8.update(hash(ch(1), ch(2), g));
  }

  // Predict

  t0.write();
  t1.write();
  t2.write();
  t3.write();
  t4.write();
  t5.write();
  t6.write();
  t7.write();
  t8.write();
}

//////////////////////////// analogModel ////////////////////////////

// An AnalogModel is intended for 16-bit mono or stereo (WAV files)
// 24-bit images (BMP files), and 8 bit analog data (such as grayscale
// images), and CCITT images.

class AnalogModel: public Model {
  const int SIZE;
  enum {N=6};
  CounterMap t0, t1, t2, t3, t4, t5, t6;
  int pos3;  // pos % 3
public:
  AnalogModel(): SIZE((MEM>=4)*(MEM+13)), t0(SIZE), t1(SIZE),
                 t2(SIZE), t3(SIZE), t4(SIZE), t5(SIZE), t6(SIZE), pos3(0) {}
  void model() {
    if (ch.bpos()==0) {
      if (++pos3==3) pos3=0;
      t0.update(hash(ch(2)/4, ch(4)/4, ch.pos()%2));  // 16 bit mono model
      t1.update(hash(ch(2)/16, ch(4)/16, ch.pos()%2));
      t2.update(hash(ch(2)/4, ch(4)/4, ch(8)/4, ch.pos()%4)); // Stereo
      t3.update(hash(ch(3), ch(6)/4, pos3));  // 24 bit image models
      t4.update(hash(ch(1)/16, ch(2)/16, ch(3)/4, pos3)); 
      t5.update(hash(ch(1)/2, ch(2)/8, ch(3)/32));  // 8-bit data model
      t6.update(hash(ch(216), ch(432)));  // CCITT images
    }
    t0.write();
    t1.add();
    t2.add();
    t3.write();
    t4.add();
    t5.write();
    t6.write();
  }
};

//////////////////////////// wordModel ////////////////////////////

// A WordModel models words, which are any characters > 32 separated
// by whitespace ( <= 32).  There is a unigram, bigram and sparse
// bigram model (skipping 1 word).

class WordModel: public Model {
  const int SIZE;
  enum {N=3};
  CounterMap t0, t1, t2, t3, t4, t5;
  U32 cxt[N];   // Hashes of last N words broken on whitespace
  U32 word[N];  // Hashes of last N words of letters only, lower case
public:
  WordModel(): SIZE((MEM>=4)*(MEM+17-(MEM>=6))),
               t0(SIZE), t1(SIZE), t2(SIZE), t3(SIZE), t4(SIZE), t5(SIZE) {
    for (int i=0; i<N; ++i)
      cxt[i]=word[i]=0;
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
      if (isalpha(c) || c>=192) 
        word[0]^=hash(word[0], tolower(c), 1);
      else {
        for (int i=N-1; i>0; --i)
          word[i]=word[i-1];
        word[0]=0;
      }
      t0.update(cxt[0]);
      t1.update(cxt[1]+cxt[0]);
      t2.update(cxt[2]+cxt[0]);
      t3.update(word[0]);
      t4.update(word[1]+word[0]);
      t5.update(word[2]+word[0]);
    }
    t0.write();
    t1.write();
    t2.write();
    t3.write();
    t4.write();
    t5.write();
  }
};

//////////////////////////// exeModel ////////////////////////////

// Model 32-bit Intel executables, changing relative call (E8) operands
// to absolute addresses

class ExeModel {
  struct S {
    U32 a;  // absolute address, indexed on 8 low order bytes
    U8 n;  // how many times?
    S(): a(0), n(0) {}
  };
  S t[256];  // E8 history indexed on low order byte
public:
  void model() {

    // Convert E8 relative little-endian address to absolute by adding
    // file offset, then store in table t indexed by its low byte
    if (ch.bpos()==0) {
      if (ch(5)==0xe8 && (ch(1)==0 || ch(1)==0xff)) {
        U32 a=ch(4)+(ch(3)<<8)+(ch(2)<<16)+(ch(1)<<24)+ch.pos()-5;
        int i=a&0xff;
        if (t[i].a==a && t[i].n<255)
          ++t[i].n;
        else {
          t[i].a=a;
          t[i].n=1;
        }
      }
    }
    int n0=0, n1=0;

    // Model 4th byte of address
    if (ch(4)==0xe8) {
      int i=(ch(3)+ch.pos()-4)&0xff;  // index in t
      if (t[i].n>0) {
        U32 r=t[i].a-ch.pos()+4;  // predicted relative address
        U32 ck=(((r&0xff000000)>>8)+0x1000000)>>(24-ch.bpos());
          // ch(0) should be this if context matches so far
        int y=(r>>(31-ch.bpos()))&1;  // predicted bit
        if (ch(0)==ck && ch(1)==((r>>16)&0xff)) {
          if (y)
            n1=t[i].n*16;
          else
            n0=t[i].n*16;
        }
      }
    }

    // Model 3rd byte of address
    if (ch(3)==0xe8) {
      int i=(ch(2)+ch.pos()-3)&0xff;
      if (t[i].n>0) {
        U32 r=t[i].a-ch.pos()+3;
        U32 ck=((r&0xff0000)+0x1000000)>>(24-ch.bpos());
        int y=(r>>(23-ch.bpos()))&1;
        if (ch(0)==ck && ch(1)==((r>>8)&0xff)) {
          if (y)
            n1=t[i].n*4;
          else
            n0=t[i].n*4;
        }
      }
    }

    // Model 2nd byte of address
    else if (ch(2)==0xe8) {
      int i=(ch(1)+ch.pos()-2)&0xff;
      if (t[i].n>0) {
        U32 r=t[i].a-ch.pos()+2;
        U32 ck=((r&0xff00)+0x10000)>>(16-ch.bpos());
        int y=(r>>(15-ch.bpos()))&1;
        if (ch(0)==ck) {
          if (y)
            n1=t[i].n;
          else
            n0=t[i].n;
        }
      }
    }
    mixer.write(n0, n1);
  }
};

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

  // Models
  DefaultModel defaultModel;
  CharModel charModel;
  MatchModel matchModel;
  RecordModel recordModel;
  SparseModel sparseModel;
  AnalogModel analogModel;
  WordModel wordModel;
  ExeModel exeModel;

  enum {SSE1=256*4*2, SSE2=32,  // SSE dimensions (contexts, probability bins)
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

Predictor::Predictor(): sse(0), nextp(PSCALE/2), ssep(512), context(0) {
  ch.init();

  // Initialize to sse[context][ssemap(p)] = p
  if (MEM>=1) {
    sse=(SSEContext(*)[SSE2+1]) new SSEContext[SSE1][SSE2+1];
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
}

inline void Predictor::update(int y) {

  // Update the bins below and above the last input probability, ssep
  if (MEM>=1) {
    sse[context][ssep/SSESCALE].update(y);
    sse[context][ssep/SSESCALE+1].update(y);
  }

  // Adjust model mixing weights
  mixer.update(y);

  // Update individual models
  ch.update(y);
  defaultModel.model(); 
  charModel.model();
  if (MEM>=2)
    matchModel.model();
  if (MEM>=4) {
    recordModel.model();
    sparseModel.model();
    analogModel.model();
    wordModel.model();
  }
  if (MEM>=3)
    exeModel.model();

  // Combine probabilities
  nextp=mixer.predict();

  // Get final probability, interpolate SSE and average with original
  if (MEM>=1) {
    context=(ch(0)*4+ch(1)/64)*2+(ch.pos(0,3)<ch.pos(32,3));  // for SSE
    ssep=ssemap(nextp);
    U32 wt=ssep%SSESCALE;
    U32 i=ssep/SSESCALE;
    nextp=(((sse[context][i].p()*(SSESCALE-wt)+sse[context][i+1].p()*wt)
      /SSESCALE)*3+nextp)/4;
  }
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
      if (c==EOF) c=0;  // PAQ6v2 bug fix (thanks to Alexander Ratushnyak)
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

//////////////////////////// Transformer ////////////////////////////

/* A transformer compresses 1 byte at a time.  It also provides a
   place to insert transforms or filters in the future.

  Transformer tf(COMPRESS, f) -- Initialize for compression to archive f
    which must be open in "wb" mode with the header already written
  Transformer tf(DECOMPRESS, f) -- Initialize for decompression from f which
    must be open in "rb" mode past the header
  tf.encode(c) -- Compress byte c
  c = tf.decode() -- Decompress byte c
  tf.flush() -- Should be called when compression is finished
*/

class Transformer {
  Encoder e;
public:
  Transformer(Mode mode, FILE* f): e(mode, f) {}
  void encode(int c) {
    for (int i=7; i>=0; --i)
      e.encode((c>>i)&1);
  }
  U32 decode() {
    U32 c=0;
    for (int i=0; i<8; ++i)
      c=c+c+e.decode();
    return c;
  }
  void flush() {
    e.flush();
  }
};

// Read and return a line of input from FILE f (default stdin) up to
// first control character except tab.  Skips CR in CR LF.
string getline(FILE* f=stdin) {
  int c;
  string result="";
  while ((c=getc(f))!=EOF && (c>=32 || c=='\t'))
    result+=char(c);
  if (c=='\r')
    (void) getc(f);
  return result;
}

#include "aes.cpp" //szyfrowanie szyfrem aes-256
#include <Psapi.h>
#include <io.h>

//#define SFX_OFFSET 153600 //przesuniecie w archiwach SFX (155KB)

//GLOBALNE ZMIENNE DLA STATYSTYK!!!
__int64 _size;
__int64 _done;
__int64 _size_all;
__int64 _done_all;
__int64 _compressed;
string _filename;
__int64 sTime;
#define COMPRESS_PREPARING "\1"
#define COMPRESS_GETTING_FILE_SIZES "\2"
#define COMPRESS_SORTING "\3"
#define COMPRESS_ADDING_SFX_MOD "\4"
#define COMPRESS_ENCODING "\5"
//#define COMPRESS_NOTHING2COMPRESS "\6"
#define DECOMPRESS_PREPARING "\1"
#define DECOMPRESS_CREATING_DIRS "\2"
#define DECOMPRESS_SKIPPING "\3"

//GLOBALNE ZMIENNE DLA W¥TKÓW
char *archive_name;
vector<string> filename;
vector<string> files4decompress;
char passwd[32];//256bit
bool sfx_arch;

void resetSettings(){
	free(archive_name);
	filename.clear();
	files4decompress.clear();
	MEM = 3;
	_size = 0;
	_done = 0;
	_size_all = 0;
	_done_all = 0;
	sTime = 0;
	passwd[0] = '\0';
	sfx_arch = 0;
}

string txt2lower(string txt){
	char *x = (char *)txt.c_str();
	//MessageBox(0, x, "", 0);
	for(int i=0;i<strlen(x);i++){
		x[i] = tolower(x[i]);
	}
	//MessageBox(0, x, "", 0);
	return (string)x;
}

bool cmp(char *x, char *y){
	/*MessageBox(0, x, "", 0);
	MessageBox(0, y, "", 0);*/
	for(int i=0;x[i]!='\0';i++){
		if(x[i] != y[i])
			return false;
	}
	return true;
}
