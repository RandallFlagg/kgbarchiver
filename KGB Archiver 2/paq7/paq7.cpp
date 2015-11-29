// paq7.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif


/* PAQ7 file compressor/archiver.
(C) 2005, Matt Mahoney under GPL, http://www.gnu.org/licenses/gpl.txt

PAQ7 is a file compressor and archiver.  To use:

  paq7 archive files...

If the archive does not exist, then it is created and the files are
compresed and stored in the archive.  File names are stored in the
archive exactly as entered (with or without a path).

If the archive already exists, then the files are extracted in the
same order as they were stored.  If the file names are omitted,
then the stored names are used instead.

PAQ7 never clobbers files.  If the file to be extracted already
exists, then PAQ7 compares it with the archived copy and reports
whether they differ.  Example:

  paq7 xyz foo bar      Compresses foo and bar to archive xyz.
  paq7 xyz baz          Extracts baz as a copy of foo, compares bar.

If PAQ7 is run under UNIX or Linix, or compiled with g++ for Windows,
then wildcards may be used.  You can also enter the file names, one
per line to standard input ending with a blank line or EOF.  Example:

  dir/b *.txt | paq7 archive

is equivalent to:

  paq7 archive *.txt

The archive has a human-readable header.  To view the stored file
names and sizes:

  more < archive

PAQ7 cannot update an archive.  You must create a new one instead.
You cannot extract a subset of the stored files, but you can interrupt
it with ^C after the files you want are extracted.

Neither the archive nor the total size of all files can be larger than
2 gigabytes, even if your file system supports such large files.  File
names (including path) cannot be longer than 511 characters or contain
carriage returns, linefeeds, ^Z (ASCII 26) or NUL (ASCII 0).  If any file
names contain 8-bit (European) extended ASCII, then do not pipe from
dir/b, use wildcards.  File names with spaces on the command line
should be quoted "like this".


TO COMPILE

  MINGW g++:
    nasm paq7asm.asm -f win32 --prefix _
    g++ paq7.cpp -O2 -s -o paq7.exe paq7asm.obj

  DJGPP g++:
    nasm paq7asm.asm -f coff --prefix _
    gxx paq7.cpp -O2 -s -o paq7.exe paq7asm.o

  Borland:
    nasm paq7asm.asm -f obj --prefix _
    bcc32 -O -w-8027 paq7.cpp paq7asm.obj

  Mars:
    nasm paq7asm.asm -f obj --prefix _
    dmc paq7.cpp paq7asm.obj

  UNIX/Linux (PC):
    nasm -f elf paq7asm.asm
    g++ paq7.cpp -O2 -s -o paq7 paq7asm.o

  Non PC (UNIX):
    g++ paq7.cpp -O2 -DNOASM -s -o paq7

The program runs a little faster compiled under g++ than Borland or Mars.

I did not test with Microsoft or Intel C++.  Either nasm "-f obj" or
"-f win32" should link.  Always use "--prefix _" under Windows.
Some older versions of Linux would use "-f aout" instead of "-f elf".

You can compile without NASM in any of the above examples by adding
-DNOASM, but it will run about 1/3 slower, so you really should get it.
(http://nasm.sourceforge.net).  It is easy to install, just one
executable program put in your path.  The NASM code requires a
Pentium-MMX or higher, so it should work on any Windows or Linux PC
made since about 1997.


ARCHIVE FILE FORMAT

An archive has the following format.  It is intended to be both
human and machine readable.  The header ends with CTRL-Z (Windows EOF)
so that the binary compressed data is not displayed on the screen.

  paq7 CR LF
  size TAB filename CR LF
  size TAB filename CR LF
  ...
  CTRL-Z
  compressed binary data

Decompression will fail if the first 4 bytes are not "paq7".  Sizes
are stored as decimal numbers.  CR, LF, TAB, CTRL-Z are ASCII codes
13, 10, 9, 26 respectively.

The binary data is arithmetic coded as the shortest base 256 fixed point
number x = SUM_i x_i 256^-1-i such that p(<y) <= x < p(<=y), where y is the
input string, x_i is the i'th coded byte, p(<y) (or p(<=y)) means the
probability that a string is lexicographcally less than (less tha
or equal to) y according to the model, _ denotes subscript, and ^ denotes
exponentiation.

The model p(y) for y is a conditional bit stream,
p(y) = PROD_j p(y_j | y_0..j-1) where y_0..j-1 denotes the first j
bits of y, and y_j is the next bit.  Compression depends almost entirely
on the ability to predict the next bit accurately.


MODEL MIXING

PAQ7 uses a neural network to combine a large number of models that
independently predict p1_i = p(y_j = 1 | y_0..j-1), p0_i = 1 - p1_i.
The network computes the next bit probabilty

  p1 = squash(SUM_i w_i t_i), p0 = 1 - p1                        (1)

where t_i = stretch(p1_i) is the i'th input, p1_i is the prediction of
the i'th model, p1 is the output prediction, stretch(p) = ln(p/(1-p)),
and squash(s) = 1/(1+exp(-s)).  Note that squash() and stretch() are
inverses of each other.

After bit y_j (0 or 1) is received, the network is trained:

  w_i := w_i + eta t_i (y_j - p1)                                (2)

where eta is an ad-hoc learning rate, t_i is the i'th input, (y - p1)
is the prediction error, and w_i is the i'th weight.  Note that this
differs from back propagation:

  w_i := w_i + eta t_i (y_j - p1) p0 p1                          (3)

which is a gradient descent in weight space to minimize root mean square
error.  Rather, the goal in compression is to minimize coding cost,
which is -log(p0) if y = 1 or -log(p1) if y = 0.  Taking
the partial derivative of cost with respect to w_i yields (2).


MODELS

Most models are context models.  A function of the context (last few
bytes) is mapped by a lookup table or hash table to a state which depends
on the bit history (prior sequence of 0 and 1 bits seen in this context).
The bit history is then mapped to p1_i by a fixed or adaptive function.
There are several types of bit history states:

- Run Map. The state is (b,n) where b is the last bit seen (0 or 1) and
  n is the number of consecutive times this value was seen.  The initial
  state is (0,0).  The output is computed directly:

    t_i = (2b - 1)K log(n + 1).

  where K is ad-hoc, around 4 to 10.  When bit y_j is seen, the state
  is updated:

    (b,n) := (b,n+1) if y_j = b, else (y_j,1).

- Stationary Map.  The state is p, initially 1/2.  The output is
  t_i = stretch(p).  The state is updated at ad-hoc rate K (around 0.01):

    p := p + K(y_j - p)

- Nonstationary Map.  This is a compromise between a stationary map, which
  assumes uniform statistics, and a run map, which adapts quickly by
  discarding old statistics.  An 8 bit state represents (n0,n1,h), initially
  (0,0,0) where:

    n0 is the number of 0 bits seen (for small n).
    n1 is the number of 1 bits seen (for small n).
    n = n0 + n1.
    h is the full bit history for 0 <= n <= 4,
      the last bit seen (0 or 1) if 5 <= n <= 7,
      0 for n >= 8.
    Furthermore there are no states where both n0 and n1 are large:
      n0 < 64.                  n1 < 64.
      If n0 = 1 then n1 < 32.   If n1 = 1 then n0 < 32.
      If n0 = 2 then n1 < 16.   If n1 = 2 then n0 < 16.
      If n0 = 3 then n1 < 8.    If n1 = 3 then n0 < 8.

  The primaty output is t_i := stretch(sm(n0,n1,h)), where sm(.) is
  a stationary map with K = 1/256, initiaized to sm(n0,n1,h) = (n1+1)/(n+2).
  Four additional inputs may also be computed to improve compression
  slightly:

    p1_i = sm(n0,n1,h)
    p0_i = 1 - p1_i
    t_i   := stretch(p_1)
    t_i+1 := K1 (p1_i - p0_i)
    t_i+2 := K2 (-p0_i if n1 = 0, p1_i if n0 = 0, else 0)
    t_i+3 := K3 (t_i if n1 = 0, -t_i if n0 = 0, else 0)
    t_i+4 := K4 (-p0_i if lb(h) = 0, p1_i if lb(h) = 1)

  where K1..K4 are ad-hoc constants, and lb(h) is the last bit
  as stored in h.

  The update rule discards counts from n1 when y_j = 0 and vice versa.
  If n1 > 2 then n1 := 2 log2(n1) and vice versa.  This rule tends
  to favor newer data over old.

  If an increment leads to a nonexistent state, then go to the
    nearest state with about the same ratio (n0+1/4)/(n1+1/4).
    For example, state (7,3) on bit 0 would lead to nonexistent
    state (8,3), so go to (5,2) instead.
    State (31,1) on 0 would go to nonexistent state (32,1) so
    go to (8,0) instead.

  h is updated as follows: for n < 4, store y_j in h.  For n < 8,
  set h := y_j.  For n >= 8, h = 0.

- Match Model.  The state is (c,b), initially (0,0), where c is 1 if
  the context was previously seen, else 0, and b is the next bit in
  this context.  The prediction is:

    t_i := (2b - 1)Kc log(m + 1)

  where m is the length of the context.  The update rule is c := 1,
  b := y_j.  A match model can be implemented efficiently by storing
  input in a buffer and storing pointers into the buffer into a hash
  table indexed by context.  Then c is indicated by a hash table entry
  and b can be retrieved from the buffer.


CONTEXTS

High compression is achieved by combining a large number of contexts.
Most (not all) contexts start on a byte boundary and end on the bit
immediately preceding the predicted bit.  The contexts below are
modeled with both a run map and a nonstationary map unless indicated.

- Order n.  The last n bytes, up to about 16.  For general purpose data.
  Most of the compression occurs here for orders up to about 6.
  An order 0 context includes only the 0-7 bits of the partially coded
  byte and the number of these bits (255 possible values).

- Sparse.  Usually 1 or 2 of the last 8 bytes preceding the byte containing
  the predicted bit, e.g (2), (3),..., (8), (1,3), (1,4), (1,5), (1,6),
  (2,3), (2,4), (3,6), (4,8).  The ordinary order 1 and 2 context, (1)
  or (1,2) are included above.  Useful for binary data.

- Text.  Contexts consists of whole words (a-z, converted to lower case
  and skipping other values).  Contexts may be sparse, e.g (0,2) meaning
  the current (partially coded) word and the second word preceding the
  current one.  Useful contexts are (0), (0,1), (0,1,2), (0,2), (0,3),
  (0,4).  The preceding byte may or may not be included as context in the
  current word.

- Formatted text.  The column number (determined by the position of
  the last linefeed) is combined with other contexts: the charater to
  the left and the character above it.

- Fixed record length.  The record length is determined by searching for
  byte sequences with a uniform stride length.  Once this is found, then
  the record length is combined with the context of the bytes immediately
  preceding it and the corresponding byte locations in the previous
  one or two records (as with formatted text).

- Context gap.  The distance to the previous occurrence of the order 1
  or order 2 context is combined with other low order (1-2) contexts.

- FAX.  For 2-level bitmapped images.  Contexts are the surrounding
  pixels already seen.  Image width is assumed to be 1728 bits (as
  in calgary/pic).

- Image.  For uncompressed 24-bit color BMP and TIFF images.  Contexts
  are the high order bits of the surrounding pixels and linear
  combinations of those pixels, including other color planes.  The
  image width is detected from the file header.  When an image is
  detected, other models are turned off to improve speed.

- JPEG.  Files are further compressed by partially uncompressing back
  to the DCT coefficients to provide context for the next Huffman code.
  Only baseline DCT-Huffman coded files are modeled.  (This ia about
  90% of images, the others are usually progresssive coded).  JPEG images
  embedded in other files (quite common) are detected by headers.  The
  baseline JPEG coding process is:
  - Convert to grayscale and 2 chroma colorspace.
  - Sometimes downsample the chroma images 2:1 or 4:1 in X and/or Y.
  - Divide each of the 3 images into 8x8 blocks.
  - Convert using 2-D discrete cosine transform (DCT) to 64 12-bit signed
    coefficients.
  - Quantize the coefficients by integer division (lossy).
  - Split the image into horizontal slices coded independently, separated
    by restart codes.
  - Scan each block starting with the DC (0,0) coefficient in zigzag order
    to the (7,7) coefficient, interleaving the 3 color components in
    order to scan the whole image left to right starting at the top.
  - Subtract the previous DC component from the current in each color.
  - Code the coefficients using RS codes, where R is a run of R zeros (0-15)
    and S indicates 0-11 bits of a signed value to follow.  (There is a
    special RS code (EOB) to indicate the rest of the 64 coefficients are 0).
  - Huffman code the RS symbol, followed by S literal bits.
  The most useful contexts are the current partially coded Huffman code
  (including S following bits) combined with the coefficient position
  (0-63), color (0-2), and last few RS codes.

- Match.  When a context match of 400 bytes or longer is detected,
  the next bit of the match is predicted and other models are turned
  off to improve speed.


ARCHITECTURE

The context models are mixed by up to 3 of several hundred neural networks
selected by a low-order context.  The outputs of these networks are
combined by weighted averaging with fixed weights, then fed through
two stages of adaptive probability maps (APM) before arithmetic coding.
The three neural networks are selected by the following contexts:

- NN1: Order 0: the current (partially coded) byte, and the number of
       bits coded (0-7).
- NN2: The previous whole byte (does not include current byte).
- NN3: The second from last byte.
- NN4: The file type (image, long match, or normal), order (0-7), 3 high bits
       of the previous byte, and whether the last 2 bytes are equal.

The outputs are combined by another neural network with the 4 predicitons
as inputs.

For images and long matches, only one neural network is used and its
context is fixed.

An APM is a stationary map combining a context and an input probability.
The input probability is stretched and divided into 32 segments to
combine with other contexts.  The output is interpolated between two
adjacent quantized values of stretch(p1).  There are 2 APM stages in series:

  p1 := (p1 + 3 APM(order 0, p1)) / 4.
  p1 := (APM(order 1, p1) + 2 APM(order 2, p1) + APM(order 3, p1)) / 4.


IMPLEMENTATION

Hash tables are designed to minimize cache misses, which consume most
of the CPU time.

Most of the memory is used by the nonstationary context models.
Contexts are hashed to 32 bits.  Hash tables use the low order
bits to index a table and an 8 bit checksum of the hash to detect
collisions, which have a 1/256 chance of a false match.  A context
hash is updated every 4 bits to index a table entry containing 15
states (1 byte each) for each of the possible combinations of 0-3
bits that follow.  If a collision is detected, 4 adjacent elements
within the same 64-byte block are tested (trying within a 32-byte
block first).  Hash elements are aligned on 64 byte address boundaries
so as not to cross cache lines.

If all 4 hash elements are full, then the state of the first context
(the one on the 4 bit boundary) is used as a priority to select a
replacement.  The states are ordered by increasing n so that the least
used state is replaced first.  A zero state is replaced immediately.

Run models use 3 byte hash elements consisting of a checksum, a
repeat count (0-255) and the byte value.  The count also serves as
a priority.

Stationary models are most appropriate for small contexts, so the
context is used as a direct table lookup without hashing.

The match model maintains a pointer to the last match until a mismatching
bit is found.  At the start of the next byte, the hash table is referenced
to find another match.  The hash table of pointers is updated after each
whole byte.  There is no checksum.  Collisions are detected by comparing
the current and matched context in a rotating buffer.

The inner loops of the neural network prediction (1) and training (2)
algorithms are implemented in MMX assembler, which computes 4 elements
at a time.  Using assembler is 8 times faster than C++ for this code
and 1/3 faster overall.  (However I found that SSE2 code on an AMD-64,
which computes 8 elements at a time, is not any faster).

*/

#define PROGNAME "paq7"  // Please change this if you change the program.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#define NDEBUG  // uncomment for debugging (turns on Array bound checks)
#include <assert.h>
namespace std {}
using namespace std;

// 8, 16, and 32 bit unsigned types (adjust as appropriate)
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

// min, max functions
/*template <class T> inline T max(const T& a, const T& b) {return a<b?b:a;}
template <class T> inline T min(const T& a, const T& b) {return a<b?a:b;}*/
#ifndef min
inline int min(int a, int b) {return a<b?a:b;}
inline int max(int a, int b) {return a<b?b:a;}
#endif

//////////////////////// Program Checker /////////////////////

class ProgramChecker {
  int memused;  // bytes allocated by Array<T>
  clock_t start_time;  // in ticks
public:
  void alloc(int n) {memused+=n;}  // report memory allocated
  ProgramChecker(): memused(0) {
    start_time=clock();
    assert(sizeof(U8)==1);
    assert(sizeof(U16)==2);
    assert(sizeof(U32)==4);
    assert(sizeof(short)==2);
    assert(sizeof(int)==4);
  }
  ~ProgramChecker() {
    /*printf("Time %1.2f sec, memory %d bytes\n\n",
      double(clock()-start_time)/CLOCKS_PER_SEC, memused);*/
  }
} programChecker;

//////////////////////////// Array ////////////////////////////

// Array<T> a(n); creates n elements of T initialized to 0 bits.
// Constructors for T are not called.
// Indexing is bounds checked if assertions are on.
// a.size() returns n.
// Copy, assignment, resize are not supported.
// Memory is aligned on a 64 byte boundary.

template <class T> class Array {
  int n;     // size
  char *ptr; // allocated memory, zeroed
  T* data;   // start of n elements of aligned data
public:
  explicit Array(int i);
//  ~Array() {free(ptr);}
  T& operator[](int i) {
#ifndef NDEBUG
    if (i<0 || i>=n) fprintf(stderr, "%d out of bounds %d\n", i, n), exit(1);
#endif
    return data[i];
  }
  const T& operator[](int i) const {
#ifndef NDEBUG
    if (i<0 || i>=n) fprintf(stderr, "%d out of bounds %d\n", i, n), exit(1);
#endif
    return data[i];
  }
  int size() const {return n;}
private:
  Array(const Array&);  // no copy or assignment
  Array& operator=(const Array&);
};

template<class T> Array<T>::Array(int i): n(i) {
  const int sz=64+n*sizeof(T);
  programChecker.alloc(sz);
  ptr = (char*)calloc(sz, 1);
  if (!ptr) fprintf(stderr, "Out of memory\n"), exit(1);
  data = (T*)(ptr+64-(((long)ptr)&63));
}

////////////////////////////// Buf /////////////////////////////

// Buf(n) buf; creates an array of n bytes (must be a power of 2).
// buf[i] returns a reference to the i'th byte with wrap (no out of bounds).
// buf(i) returns i'th byte back from pos (i > 0) 
// buf.size() returns n.

int pos;  // Number of input bytes in buf (not wrapped)

class Buf {
  Array<U8> b;
public:
  Buf(int i): b(i) {assert((i&(i-1))==0);}
  U8& operator[](int i) {return b[i & (b.size()-1)];}
  int operator()(int i) const {
    assert(i>0);
    return b[(pos-i) & (b.size()-1)];
  }
  int size() const {return b.size();}
};

//////////////////////////// context ///////////////////////////

// Global context.  This information is available to all models.
int MEM=0x100000;  // Memory usage scale (power of 2), set by command option
int y=0;  // Last bit, 0 or 1, set by encoder
int c0=1; // Last 0-7 bits of the partial byte with a leading 1 bit (1-255)
U32 c4=0; // Last 4 whole bytes, packed.  Last byte is bits 0-7.
int bpos=0; // bits in c0 (0 to 7)
Buf buf(MEM*8);  // Rotating input queue

///////////////////////////// ilog //////////////////////////////

// ilog(x) = round(log2(x) * 16), 0 <= x < 64K
class Ilog {
  Array<U8> t;
public:
  int operator()(U16 x) const {return t[x];}
  Ilog();
} ilog;

// Compute lookup table by numerical integration of 1/x
Ilog::Ilog(): t(65536) {
  U32 x=14155776;
  for (int i=2; i<65536; ++i) {
    x+=774541002/(i*2-1);  // numerator is 2^29/ln 2
    t[i]=x>>24;
  }
}

// llog(x) accepts 32 bits
inline int llog(U32 x) {
  if (x>=0x1000000)
    return 256+ilog(x>>16);
  else if (x>=0x10000)
    return 128+ilog(x>>8);
  else
    return ilog(x);
}

///////////////////////// state table ////////////////////////

// State table:
//   nex(state, 0) = next state if y = 0, 0 <= state < 256
//   nex(state, 1) = next state if y = 1
//   nex(state, 2) = number of zeros in bit history represented by state
//   nex(state, 3) = number of ones represented

// States represent a bit history within some context.
// State 0 is the starting state (no bits seen).
// States 1-30 represent all possible sequences of 1-4 bits.
// States 31-255 represent a pair of counts, (n0,n1), the number
//   of 0 and 1 bits respectively.
// For states 31-67 where n0+n1 < 8 and n0 > 0 and n1 > 0, there
//   are two states for each (n0,n1) pair depending on whether
//   the last bit was a 0 or 1.
// There are no states where both n0 and n1 are large:
//   n0 < 64.                  n1 < 64.
//   If n0 = 1 then n1 < 32.   If n1 = 1 then n0 < 32.
//   If n0 = 2 then n1 < 16.   If n1 = 2 then n0 < 16.
//   If n0 = 3 then n1 < 8.    If n1 = 3 then n0 < 8.
// On bit 0, n0 is incremented and if n1 > 2 then n1 := 2 log2(n1).
//   (This rule tends to favor newer data over old).
// On bit 1, then n1 is incremented and n0 is reduced as above.
// If an increment leads to a nonexistent state, then go to the
//   nearest state with about the same ratio (n0+1/4)/(n1+1/4).
//   For example, state (7,3) on bit 0 would lead to nonexistent
//   state (8,3), so go to (5,2) instead.
//   State (31,1) on 0 would go to nonexistent state (32,1) so
//   go to (8,0) instead.

// State -> last bit: 1 = last bit is 0, 2 = last bit is 1, 0=unknown
static const U8 LB[256] = {
  0, 1,2, 1,2,1,2, 1,1,1,2,1,2,2,2, 1,1,1,1,2,1,1,1,2,2,2,1,2,2,2,2,
  1,1,2,1,2,1,2,1,2,2, 1,1,2,1,2,1,2,1,2,1,2,2, 1,1,2,1,2,1,2,1,2,1,2,1,2,2}; 

// Initialize nex()

class StateTable {
  Array<U8> ns;  // state*4 -> next state if 0, if 1, n0, n1
  enum {B=5, N=64};
  static const int b[B];  // n0 -> max n1, n1 -> max n0
  static int t[N+1][N+1][2];  // n0,n1 -> state number, number of states
  int num_states(int x, int y);
  void discount(int& x);
  void next_state(int& x, int& y, int b);
public:
  int operator()(int state, int sel) {return ns[state*4+sel];}
  StateTable();
} nex;

const int StateTable::b[B]={N,32,16,8,5};  // n0 -> max n1, n1 -> max n0
int StateTable::t[N+1][N+1][2];

int StateTable::num_states(int x, int y) {
  if (x<y) return num_states(y, x);
  if (x<0 || y<0 || x>=N || y>=N || y>=B || x>=b[y]) return 0;

  // States 0-30 are a history of the last 0-4 bits
  if (x+y<=4) {  // x+y choose x = (x+y)!/x!y!
    int r=1;
    for (int i=x+1; i<=x+y; ++i) r*=i;
    for (int i=2; i<=y; ++i) r/=i;
    return r;
  }

  // States 31-255 represent a 0,1 count and possibly the last bit
  // if the state is reachable by either a 0 or 1.
  else
    return 1+(y>0 && x+y<8);
}

// New value of count x if the opposite bit is observed
void StateTable::discount(int& x) {
  if (x>2) x=ilog(x) >> 3;
}

// compute next x,y (0 to N) given input b (0 or 1)
void StateTable::next_state(int& x, int& y, int b) {
  if (x<y)
    next_state(y, x, 1-b);
  else {
    if (b) {
      ++y;
      discount(x);
    }
    else {
      ++x;
      discount(y);
    }
    if (!t[x][y][1]) {
      if (y==0) --x;
      else if (y==1) {
        y=0;
        x/=4;
      }
      else {
        x=(x*(y-1)+(y/2))/y;
        --y;
      }
    }
  }
}

StateTable::StateTable(): ns(1024) {

  // Assign states
  int state=0;
  for (int i=0; i<256; ++i) {
    for (int y=0; y<=i; ++y) {
      int x=i-y;
      int n=num_states(x, y);
      if (n) {
        t[x][y][0]=state;
        t[x][y][1]=n;
        state+=n;
      }
    }
  }

  // Print next state table
  state=0;
  for (int i=0; i<N; ++i) {
    for (int y=0; y<=i; ++y) {
      int x=i-y;
      for (int k=0; k<t[x][y][1]; ++k) {
        int x0=x, y0=y, x1=x, y1=y;  // next x,y for input 0,1
        int ns0=0, ns1=0;
        if (state<15) {
          ++x0;
          ++y1;
          ns0=t[x0][y0][0]+state-t[x][y][0];
          ns1=t[x1][y1][0]+state-t[x][y][0];
          if (x>0) ns1+=t[x-1][y+1][1];
          ns[state*4]=ns0;
          ns[state*4+1]=ns1;
          ns[state*4+2]=x;
          ns[state*4+3]=y;
        }
        else if (t[x][y][1]) {
          next_state(x0, y0, 0);
          next_state(x1, y1, 1);
          ns[state*4]=ns0=t[x0][y0][0];
          ns[state*4+1]=ns1=t[x1][y1][0]+(t[x1][y1][1]>1);
          ns[state*4+2]=x;
          ns[state*4+3]=y;
        }
        assert(state>=0 && state<256);
        assert(t[x][y][1]>0);
        assert(t[x][y][0]<=state);
        assert(t[x][y][0]+t[x][y][1]>state);
        assert(t[x][y][1]<=6);
        assert(t[x0][y0][1]>0);
        assert(t[x1][y1][1]>0);
        assert(ns0-t[x0][y0][0]<t[x0][y0][1]);
        assert(ns0-t[x0][y0][0]>=0);
        assert(ns1-t[x1][y1][0]<t[x1][y1][1]);
        assert(ns1-t[x1][y1][0]>=0);
        ++state;
      }
    }
  }
}

///////////////////////////// Squash //////////////////////////////

// return p = 1/(1 + exp(-d)), d scaled by 8 bits, p scaled by 12 bits
int squash(int d) {
  static const int t[33]={
    1,2,3,6,10,16,27,45,73,120,194,310,488,747,1101,
    1546,2047,2549,2994,3348,3607,3785,3901,3975,4022,
    4050,4068,4079,4085,4089,4092,4093,4094};
  if (d>2047) return 4095;
  if (d<-2047) return 0;
  int w=d&127;
  d=(d>>7)+16;
  return (t[d]*(128-w)+t[(d+1)]*w+64) >> 7;
}

//////////////////////////// Stretch ///////////////////////////////

// Inverse of squash. d = ln(p/(1-p)), d scaled by 8 bits, p by 12 bits.
// d has range -2047 to 2047 representing -8 to 8.  p has range 0 to 4095.

class Stretch {
  Array<short> t;
public:
  Stretch();
  int operator()(int p) const {
    assert(p>=0 && p<4096);
    return t[p];
  }
} stretch;

Stretch::Stretch(): t(4096) {
  int pi=0;
  for (int x=-2047; x<=2047; ++x) {  // invert squash()
    int i=squash(x);
    for (int j=pi; j<=i; ++j)
      t[j]=x;
    pi=i+1;
  }
  t[4095]=2047;
}

//////////////////////////// Mixer /////////////////////////////

// Mixer m(N, M, S) combines models using M neural networks with
//   N inputs each, of which up to S may be selected.  If S > 1 then
//   the outputs of these neural networks are combined using another
//   neural network (with parameters S, 1, 1).  If S = 1 then the
//   output is direct.  It is used as follows:
// m.update() trains the network where the expected output is the
//   last bit (in the global variable y).
// m.add(stretch(p)) inputs prediction from one of N models.  The
//   prediction should be positive to predict a 1 bit, negative for 0,
//   nominally +-256 to +-2K.  The maximum allowed value is +-32K but
//   using such large values may cause overflow if N is large.
// m.set(cxt, range) selects cxt as one of 'range' neural networks to
//   use.  0 <= cxt < range.  Should be called up to S times such
//   that the total of the ranges is <= M.
// m.p() returns the output prediction that the next bit is 1 as a
//   12 bit number (0 to 4095).

// dot_product returns dot product t*w of n elements.  n is rounded
// up to a multiple of 8.  Result is scaled down by 8 bits.
#ifdef NOASM  // no assembly language
int dot_product(short *t, short *w, int n) {
  int sum=0;
  n=(n+7)&-8;
  for (int i=0; i<n; i+=2)
    sum+=(t[i]*w[i]+t[i+1]*w[i+1]) >> 8;
  return sum;
}
#else  // The NASM version uses MMX and is about 8 times faster.
extern "C" int dot_product(short *t, short *w, int n);  // in NASM
#endif

// Train neural network weights w[n] given inputs t[n] and err.
// w[i] += t[i]*err, i=0..n-1.  t, w, err are signed 16 bits (+- 32K).
// err is scaled 16 bits (representing +- 1/2).  w[i] is clamped to +- 32K
// and rounded.  n is rounded up to a multiple of 8.
#ifdef NOASM
void train(short *t, short *w, int n, int err) {
  n=(n+7)&-8;
  for (int i=0; i<n; ++i) {
    int wt=w[i]+((t[i]*err*2>>16)+1>>1);
    if (wt<-32768) wt=-32768;
    if (wt>32767) wt=32767;
    w[i]=wt;
  }
}
#else
extern "C" void train(short *t, short *w, int n, int err);  // in NASM
#endif

class Mixer {
  const int N, M, S;  // max inputs, max contexts, max context sets
  Array<short> tx; // N inputs from add()
  Array<short> wx; // N*M weights
  Array<int> cxt;  // S contexts
  int ncxt;        // number of contexts (0 to S)
  int base;        // offset of next context
  int nx;          // Number of inputs in tx, 0 to N
  Array<int> pr;   // last result (scaled 12 bits)
  Mixer* mp;       // points to a Mixer to combine results
public:
  Mixer(int n, int m, int s=1);

  // Adjust weights to minimize coding cost of last prediction
  void update() {
    for (int i=0; i<ncxt; ++i) {
      int err=((y<<12)-pr[i])*7;
      assert(err>=-32768 && err<32768);
      train(&tx[0], &wx[cxt[i]*N], nx, err);
    }
    nx=base=ncxt=0;
  }

  // Input x (call up to N times)
  void add(int x) {
    assert(nx<N);
    tx[nx++]=x;
  }

  // Set a context (call S times, sum of ranges <= M)
  void set(int cx, int range) {
    assert(range>=0 && ncxt<S && cx>=0 && base+cx<M);
    cxt[ncxt++]=base+cx;
    base+=range;
  }

  // predict next bit
  int p() {
    while (nx&7) tx[nx++]=0;  // pad
    if (mp) {  // combine outputs
      mp->update();
      for (int i=0; i<ncxt; ++i) {
        pr[i]=squash(dot_product(&tx[0], &wx[cxt[i]*N], nx)>>5);
        mp->add(stretch(pr[i]));
      }
      mp->set(0, 1);
      return mp->p();
    }
    else {  // S=1 context
      return pr[0]=squash(dot_product(&tx[0], &wx[0], nx)>>8);
    }
  }
};

Mixer::Mixer(int n, int m, int s): N((n+7)&-8), M(m), S(s), tx(N), wx(N*M),
    cxt(S), ncxt(0), base(0), nx(0), pr(S), mp(0) {
  assert(N>0 && (N&7)==0 && M>0);
  for (int i=0; i<S; ++i)
    pr[i]=2048;
  if (S>1) mp=new Mixer(S, 1, 1);
}

//////////////////////////// APM //////////////////////////////

// A ProbMap map maps a probability and a context into a new probability
// that bit y will next be 1.  After each guess it updates
// its state to improve future guesses.  Methods:
//
// ProbMap pm(N) creates with N contexts, uses 66*N bytes memory.
// pm.p(pr, cx, rate=8) returned adjusted probability in context cx (0 to
//   N-1).  rate determines the learning rate (smaller = faster, default 8).
//   Probabilities are scaled 12 bits (0-4095).

class ProbMap {
private:
  int index;     // last p, context
protected:
  const int N;   // number of contexts
  Array<U16> t;  // [N][33]:  p, context -> p
public:
  ProbMap(int n): index(0), N(n), t(n*33) {}
  int p(int pr=2048, int cxt=0, int rate=8) {
    assert(pr>=0 && pr<4096 && cxt>=0 && cxt<N && rate>0 && rate<32);
    pr=stretch(pr);
    int g=(y<<16)+(y<<rate)-y-y;
    t[index] += g-t[index] >> rate;
    t[index+1] += g-t[index+1] >> rate;
    const int w=pr&127;  // interpolation weight (33 points)
    index=((pr+2048) >> 7)+cxt*33;
    return (t[index]*(128-w)+t[index+1]*w) >> 11;
  }
};

// An APM is a ProbMap initialized such that p(pr, cxt) = pr for all cxt.
class APM: public ProbMap {
public:
  APM(int n);
};

APM::APM(int n): ProbMap(n) {
  // initialize t[p] -> p
  for (int i=0; i<N; ++i)
    for (int j=0; j<33; ++j)
      t[i*33+j] = i==0 ? squash((j-16)*128)*16 : t[j];
}

//////////////////////////// StateMap //////////////////////////

// A StateMap maps a nonstationary counter state to a probability.
// After each mapping, the mapping is adjusted to improve future
// predictions.  Methods:
//
// sm.p(cx) converts state cx (0-255) to a probability (0-4095).

// Counter state -> probability * 256
class StateMap {
  int cxt;
protected:
  Array<U16> t; // 256 states -> probability * 64K
public:
  StateMap(): cxt(0), t(256) {
    for (int i=0; i<256; ++i)
      t[i] = 65536*(nex(i,3)+1)/(nex(i,2)+nex(i,3)+2);
  }
  int p(int cx) {
    assert(cx>=0 && cx<256);
    t[cxt]+=(y<<16)-t[cxt]+128 >> 8;
    return t[cxt=cx] >> 4;
  }
};

//////////////////////////// hash //////////////////////////////

// Hash 2-5 ints.
inline U32 hash(U32 a, U32 b, U32 c=~0, U32 d=~0, U32 e=~0) {
  U32 h=a*200002979u+b*30005491u+c*50004239u+d*70004807u+e*110002499u;
  return h^h>>9^a>>2^b>>3^c>>4^c>>5^e>>6;
}

///////////////////////////// BH ////////////////////////////////

// A BH maps a 32 bit hash to an array of B bytes (checksum and B-1 values)
//
// BH bh(N); creates N element table with B bytes each.
//   N must be a power of 2.  The first byte of each element is
//   reserved for a checksum to detect collisions.  The remaining
//   B-1 bytes are values, prioritized by the first value.  This
//   byte is 0 to mark an unused element.
//   
// bh[i] returns a pointer to the i'th element, such that
//   bh[i][0] is a checksum of i, bh[i][1] is the priority, and
//   bh[i][2..B-1] are other values (0-255).
//   The low lg(n) bits as an index into the table.
//   If a collision is detected, up to M nearby locations in the same
//   cache line are tested and the first matching checksum or
//   empty element is returned.
//   If no match or empty element is found, then the lowest priority
//   element is replaced.

template <int B> class BH {
  enum {M=4};  // search limit
  Array<U8> t; // elements
  const U32 n; // size-1
  int replaced;
public:
  BH(int i): t(i*B), n(i-1), replaced(0) {
    assert(B>=2 && i>0 && (i&(i-1))==0); // size a power of 2?
  }
  U8* operator[](U32 i) {
    int chk=(i>>24^i>>12^i)&255;
    i&=n;
    int bi=i, b=1024;  // best replacement so far
    U8 *p;
    for (int j=0; j<M; ++j) {
      p=&t[(i^j)*B];
      if (p[0]==chk) return p;  // match
      else if (p[1]==0) return p[0]=chk, p;  // empty
      else if (p[1]<b) b=p[1], bi=i^j;  // best replacement so far
    }
    ++replaced;
    p=&t[bi*B];  // replacement element
    memset(p, 0, B);
    p[0]=chk;
    return p;
  }
  ~BH();
};

// Show memory usage for debugging
template <int B> BH<B>::~BH() {
/*
  int empty=0, once=0;
  for (int i=1; i<t.size(); i+=B) {
    if (t[i]==0) ++empty;
    else if (t[i]<2) ++once;
  }
  printf("BH<%d> %d empty, %d once, %d replaced of %d\n", B,
    empty, once, replaced, t.size()/B);
*/
}

/////////////////////////// ContextMap /////////////////////////
//
// A ContextMap maps contexts to a bit histories and makes predictions
// to a Mixer.  Methods common to all classes:
//
// ContextMap cm(M); creates using about M bytes (a power of 2).
// cm.set(cx);  sets the i'th whole-byte context to cx,
//   a 32 bit hash of a context ending on a byte boundary.
//   It should be called before predicting the first bit of each byte.
// cm.mix(m) updates Mixer m with the next prediction.  Returns 1
//   if context cx is found, else 0.  It should be called for every bit:
//
//     if (bpos==0) cm.set(cxt).
//     cm.mix(m)
//
// The different types are as follows:
//
// - NonstationaryContextMap.  Uses one byte per context (state map).
// - SmallNonstationaryContextMap.  The context is not a hash, but
//     must be 0 <= cx < M/256.
// - RunContextMap.  The bit history is a count of 0-255 consecutive
//     zeros or ones.  Uses 3 bytes per whole byte context.
// - ContextMap.  Combines a NonstationaryContextMap and a RunContextMap.
// - SmallContextMap.  Combines a SmallNonstationaryContextMap and an
//     RunContextMap.
// - SmallStationaryContextMap.  The context is not hashed.  0 <= cx < M/512.
//     The state is a 16-bit probability that is adjusted after each
//     prediction.

class NonstationaryContextMap {
  U32 cxt;  // Whole byte context hash.
  U8* cp;   // Pointer to bit-oriented context.
  BH<16> t;  // Array of hash tables mapping context to bit history.
  StateMap sm;
public:
  int bh() {  // return bit history
    *cp=nex(*cp,y);
    cp=bpos&3?cp+(y+1<<((bpos&3)-1)):t[cxt^c0*9]+1;
    return *cp;
  }
  NonstationaryContextMap(int m): cxt(0), t(m/16) {cp=t[0]+1;}
  void set(U32 cx) {cxt=cx;}

  // Put prediction in m
  int mix(Mixer& m) {
    int s=bh();
    int p1=sm.p(s);
    int n0=nex(s,2);
    int n1=nex(s,3);
    int st=stretch(p1)>>2;
    m.add(st);
    p1>>=4;
    int p0=255-p1;
    m.add(p1-p0);
    m.add(st*(!n1-!n0));
    m.add((p0&-!n1)-(p1&-!n0));
    m.add((-(LB[s]==1)&p1)-(-(LB[s]==2)&p0));
    return s>0;
  }
};

// Context is not hashed
class SmallNonstationaryContextMap {
  U32 cxt;  // Whole byte context.
  U8* cp;   // Pointer to bit-oriented context.
  Array<U8> t;  // Array of bit histories.
  StateMap sm;
public:
  int bh() {  // return bit history
    *cp=nex(*cp,y);
    cp=&t[cxt+c0];
    return *cp;
  }
  SmallNonstationaryContextMap(int m): cxt(0), t(m/2) {cp=&t[0];}
  void set(U32 cx) {cxt=cx*256&t.size()-256;}

  // Put prediction in m
  int mix(Mixer& m) {
    int s=bh();
    int p1=sm.p(s);
    int n0=nex(s,2);
    int n1=nex(s,3);
    int st=stretch(p1)>>2;
    m.add(st);
    p1>>=4;
    int p0=255-p1;
    m.add(p1-p0);
    m.add(st*(!n1-!n0));
    m.add((p0&-!n1)-(p1&-!n0));
    m.add((-(LB[s]==1)&p1)-(-(LB[s]==2)&p0));
    return s>0;
  }
};

// A RunContextMap maps a context into the next byte and a repeat
// count up to M.  Size should be a power of 2.  Memory usage is 3M/4

class RunContextMap {
  BH<3> t;
  U8* cp;
public:
  RunContextMap(int m): t(m/4) {cp=t[0]+1;}
  void set(U32 cx) {  // update count
    if (cp[0]==0 || cp[1]!=buf(1)) cp[0]=1, cp[1]=buf(1);
    else if (cp[0]<255) ++cp[0];
    cp=t[cx]+1;
  }
  int p() {  // predict next bit
    if (cp[1]+256>>8-bpos==c0)
      return ((cp[1]>>7-bpos&1)*2-1)*ilog(cp[0]+1)*8;
    else
      return 0;
  }
  int mix(Mixer& m) {  // return run length
    m.add(p());
    return cp[0]!=0;
  }
};

// ContextMap combines a NonstationaryContextMap and RunContextMap.
// Memory usage is 19/16 M.

class ContextMap {
  RunContextMap rcm;
  NonstationaryContextMap cm;
public:
  ContextMap(int m): rcm(m/4), cm(m) {}
  void set(U32 cx) {
    rcm.set(cx);
    cm.set(cx);
  }
  int mix(Mixer& m) {
    rcm.mix(m);
    return cm.mix(m);
  }
};

// Combines SmallNonstationaryMap and RunContextMap.
// Suports M/512 byte contexts. Memory usage is M bytes.

class SmallContextMap {
  RunContextMap rcm;
  SmallNonstationaryContextMap scm;
public:
  SmallContextMap(int m): rcm(m/128), scm(m) {}
  void set(U32 cx) {
    rcm.set(cx);
    scm.set(cx);
  }
  int mix(Mixer& m) {
    scm.mix(m);
    return rcm.mix(m);
  }
};

// Context is looked up directly.  m=size is power of 2 in bytes.
// Context should be < m/512.  High bits are discarded.
class SmallStationaryContextMap {
  Array<U16> t;
  int cxt;
  U16 *cp;
public:
  SmallStationaryContextMap(int m): t(m/2), cxt(0) {
    assert((m/2&m/2-1)==0); // power of 2?
    for (int i=0; i<t.size(); ++i)
      t[i]=32768;
    cp=&t[0];
  }
  void set(U32 cx) {
    cxt=cx*256&t.size()-256;
  }
  void mix(Mixer& m, int rate=7) {
    *cp += (y<<16)-*cp+(1<<rate-1) >> rate;
    cp=&t[cxt+c0];
    m.add(stretch(*cp>>4));
  }
};

//////////////////////////// Models //////////////////////////////

// All of the models below take a Mixer as a parameter and write
// predictions to it.

//////////////////////////// matchModel ///////////////////////////

// matchModel() finds the longest matching context and returns its length

int matchModel(Mixer& m) {
  const int MAXLEN=2047;  // longest allowed match + 1
  static Array<int> t(MEM);  // hash table of pointers to contexts
  static int h=0;  // hash of last 7 bytes
  static int ptr=0;  // points to next byte of match if any
  static int len=0;  // length of match, or 0 if no match
  static int result=0;

  if (!bpos) {
    h=h*997*8+buf(1)+1&t.size()-1;  // update context hash
    if (len) ++len, ++ptr;
    else {  // find match
      ptr=t[h];
      if (ptr && pos-ptr<buf.size())
        while (buf(len+1)==buf[ptr-len-1] && len<MAXLEN) ++len;
    }
    t[h]=pos;  // update hash table
    result=len;
  }

  // predict
  if (len>MAXLEN) len=MAXLEN;
  int sgn;
  if (len && buf(1)==buf[ptr-1] && c0==buf[ptr]+256>>8-bpos) {
    if (buf[ptr]>>7-bpos&1) sgn=1;
    else sgn=-1;
  }
  else sgn=len=0;
  m.add(sgn*4*ilog(len));
  m.add(sgn*64*min(len, 32));
  return result;
}

//////////////////////////// picModel //////////////////////////

// Model a 1728 by 2376 2-color CCITT bitmap image, left to right scan,
// MSB first (216 bytes per row, 513216 bytes total).  Insert predictions
// into m.

void picModel(Mixer& m) {
  static U32 r0, r1, r2, r3;  // last 5 rows, bit 8 is over current pixel
  static Array<U8> t(0x10200);  // model: cxt -> state
  const int N=3;  // number of contexts
  static int cxt[N];  // contexts
  static StateMap sm[N];

  // update the model
  for (int i=0; i<N; ++i)
    t[cxt[i]]=nex(t[cxt[i]],y);

  // update the contexts (pixels surrounding the predicted one)
  r0+=r0+y;
  r1+=r1+((buf(215)>>(7-bpos))&1);
  r2+=r2+((buf(431)>>(7-bpos))&1);
  r3+=r3+((buf(647)>>(7-bpos))&1);
  cxt[0]=r0&0x7|r1>>4&0x38|r2>>3&0xc0;
  cxt[1]=0x100+(r0&1|r1>>4&0x3e|r2>>2&0x40|r3>>1&0x80);
  cxt[2]=0x200+(r0&0x3f^r1&0x3ffe^r2<<2&0x7f00^r3<<5&0xf800);

  // predict
  for (int i=0; i<N; ++i)
    m.add(stretch(sm[i].p(t[cxt[i]])));
}

//////////////////////////// wordModel /////////////////////////

// Model English text (words and columns/end of line)

void wordModel(Mixer& m) {
  static U32 word0=0, word1=0, word2=0, word3=0, word4=0;  // hashes
  static ContextMap cm1(MEM*4), cm2(MEM*4), cm3(MEM*8), cm4(MEM*8),
    cm5(MEM*8), cm6(MEM*8), cm7(MEM*8), cm8(MEM*8), wcm(MEM*8);
  static NonstationaryContextMap lcm1(MEM), lcm2(MEM), lcm3(MEM);
  static Array<int> wpos(MEM);  // last position of word
  static int nl1=-3, nl=-2;  // previous, current newline position

  // Update word hashes
  if (bpos==0) {
    int c=c4&255;
    if (c>='A' && c<='Z')
      c+='a'-'A';
    if (c>='a' && c<='z')
      word0^=hash(word0, c);
    else if (word0) {
      word4=word3;
      word3=word2;
      word2=word1;
      word1=word0;
      word0=0;
    }
    if (c==10) nl1=nl, nl=pos-1;
    int i=0;
    int col=min(255, pos-nl), above=buf[nl1+col]; // text column context
    U32 h=hash(++i, word0, buf(1));
    int w=h&wpos.size()-1;
    cm1.set(h);
    cm2.set(word0);
    cm3.set(hash(++i, h, word1));
    cm4.set(hash(++i, word0, word1));
    cm5.set(hash(++i, h, word2));
    cm6.set(hash(++i, h, word1, word2));
    cm7.set(hash(++i, h, word3));
    cm8.set(hash(++i, h, word4));
    wcm.set(hash(++i, w, w>>8, llog(pos-wpos[w])>>4));
    wpos[w]=pos;

    // Text column models
    lcm1.set(h=hash(++i, col, buf(1)));
    lcm2.set(h=hash(++i, buf(1), above));
    lcm3.set(h=hash(++i, col, above));
  }

  // Predict
  cm1.mix(m);
  cm2.mix(m);
  cm3.mix(m);
  cm4.mix(m);
  cm5.mix(m);
  cm6.mix(m);
  cm7.mix(m);
  cm8.mix(m);
  wcm.mix(m);
  lcm1.mix(m);
  lcm2.mix(m);
  lcm3.mix(m);
}

//////////////////////////// recordModel ///////////////////////

// Model 2-D data with fixed record length.  Also order 1-2 models
// that include the distance to the last match.

void recordModel(Mixer& m) {
  const int MAXRUN=1024;
  static Array<U8> run(MAXRUN);  // candidate record lengths
  static Array<int> cpos1(256), cpos2(256); // buf(1) -> last 2 positions
  static Array<int> wpos1(0x10000);  // last position of order 2 context
  static Array<int> wpos3(MEM);  // last position of order 3 hash
  static int rlen=2;  // current, previous run length
  static ContextMap cm1(MEM), cm2(MEM), cm3(MEM), cm4(MEM),
    cm5(MEM), cm6(MEM);

  // Find record length
  if (!bpos) {
    int c=buf(1);
    int w=c4&0xffff;
    int r=pos-cpos1[c];
    if (r<MAXRUN && r>1 && r==cpos1[c]-cpos2[c]) {
      if (++run[r] > 15) {
        rlen=r;
        memset(&run[0], 0, MAXRUN);
      }
    }

    // Set 2 dimensional contexts
    int col=pos%rlen;
    cm1.set(hash(1, rlen, buf(1), buf(rlen)));
    cm2.set(hash(2, rlen, buf(rlen), buf(rlen*2)));
    cm3.set(hash(3, rlen, buf(rlen), col));
    cm4.set(hash(4, rlen, buf(1), col));
    cm5.set(hash(5, buf(1), min(255, pos-cpos1[buf(1)])));
    cm6.set(hash(6, buf(1), buf(2), llog(pos-wpos1[w])>>2));

    // update last context positions
    cpos2[c]=cpos1[c];
    cpos1[c]=pos;
    wpos1[w]=pos;
  }
  cm1.mix(m);
  cm2.mix(m);
  cm3.mix(m);
  cm4.mix(m);
  cm5.mix(m);
  cm6.mix(m);
}

//////////////////////////// sparseModel ///////////////////////

// Model order 1-2 contexts with gaps.

void sparseModel(Mixer& m) {
  static ContextMap cm13(MEM/4), cm14(MEM/4), cm15(MEM/4),
    cm16(MEM/4), cm23(MEM/4), cm24(MEM/4), cm36(MEM/4), cm48(MEM/4);
  static SmallContextMap scm1(0x20000), scm2(0x20000), scm3(0x20000),
    scm4(0x20000), scm5(0x20000), scm6(0x20000), scm7(0x20000), scm8(0x20000);
  if (bpos==0) {
    int i=0;
    cm13.set(hash(++i, buf(1), buf(3)));
    cm14.set(hash(++i, buf(1), buf(4)));
    cm15.set(hash(++i, buf(1), buf(5)));
    cm16.set(hash(++i, buf(1), buf(6)));
    cm23.set(hash(++i, buf(2), buf(3)));
    cm24.set(hash(++i, buf(2), buf(4)));
    cm36.set(hash(++i, buf(3), buf(6)));
    cm48.set(hash(++i, buf(4), buf(8)));
    scm1.set(buf(1));
    scm2.set(buf(2));
    scm3.set(buf(3));
    scm4.set(buf(4));
    scm5.set(buf(5));
    scm6.set(buf(6));
    scm7.set(buf(7));
    scm8.set(buf(8));
  }
  cm13.mix(m);
  cm14.mix(m);
  cm15.mix(m);
  cm16.mix(m);
  cm23.mix(m);
  cm24.mix(m);
  cm36.mix(m);
  cm48.mix(m);
  scm1.mix(m);
  scm2.mix(m);
  scm3.mix(m);
  scm4.mix(m);
  scm5.mix(m);
  scm6.mix(m);
  scm7.mix(m);
  scm8.mix(m);
}

//////////////////////////// bmpModel /////////////////////////////////

// Model a 24-bit color uncompressed .bmp or .tif file.  Return
// width in pixels if an image file is detected, else 0.

// 32-bit little endian number at buf(i)..buf(i-3)
inline U32 i4(int i) {
  return buf(i)+256*buf(i-1)+65536*buf(i-2)+16777216*buf(i-3);
}

// 16-bit
inline int i2(int i) {
  return buf(i)+256*buf(i-1);
}

// Square buf(i)
inline int sqrbuf(int i) {return buf(i)*buf(i);}

int bmpModel(Mixer& m) {
  static int width=0, w=0;  // width of image in pixels, bytes
  static int eoi=0;     // end of image
  static U32 tiff=0;  // offset of tif header
  const int SC=0x20000;
  static SmallStationaryContextMap scm1(SC), scm2(SC),
    scm3(SC), scm4(SC), scm5(SC), scm6(SC*2);
  static ContextMap cm1(MEM/2), cm2(MEM/2), cm3(MEM/2),
    cm4(MEM/2), cm5(MEM/2), cm6(0x40000), cm7(0x40000), cm8(MEM/2);

  // Detect .bmp file header (24 bit color, not compressed)
  if (!bpos && pos>=eoi && buf(54)=='B' && buf(53)=='M'
      && i4(44)==54 && i4(40)==40 && i4(24)==0) {
    const int iwidth=i4(36);  // image width
    const int height=i4(32);
    eoi=pos+i4(20);  // image size in bytes
    w=(eoi-pos)/height;  // width in bytes
    width=w/3;  // width includes pixels on right edge not displayed
    if (eoi-pos>=iwidth*height*3 && eoi-pos<=(iwidth+3)*height*3
        && width>=1 && height>=1) {
      printf("BMP %dx%d ", width, height);
    }
    else
      eoi=pos;
  }

  // Detect .tif file header (24 bit color, not compressed).
  // Parsing is crude, won't work with weird formats.
  if (!bpos && pos>=eoi) {
    if (c4==0x49492a00) tiff=pos;  // Intel format only
    if (pos-tiff==4 && c4!=0x08000000) tiff=0; // 8=normal offset to directory
    if (tiff && pos-tiff==200) {  // most of directory should be read by now
      int dirsize=i2(pos-tiff-4);  // number of 12-byte directory entries
      width=w=0;
      int bpp=0, compression=0, height=0;
      for (int i=tiff+6; i<pos-12 && --dirsize>0; i+=12) {
        int tag=i2(pos-i);  // 256=width, 257==height, 259: 1=no compression
          // 277=3 samples/pixel
        int tagfmt=i2(pos-i-2);  // 3=short, 4=long
        int taglen=i4(pos-i-4);  // number of elements in tagval
        int tagval=i4(pos-i-8);  // 1 long, 1-2 short, or points to array
        if ((tagfmt==3||tagfmt==4) && taglen==1) {
          if (tag==256) width=tagval;
          if (tag==257) height=tagval;
          if (tag==259) compression=tagval; // 1 = no compression
          if (tag==277) bpp=tagval;  // should be 3
        }
      }
      if (width>0 && height>0 && width*height>50 && compression==1
          && (bpp==1||bpp==3))
        eoi=tiff+width*height*bpp, w=width*bpp;
      if (eoi>pos)
        printf("TIFF %dx%dx%d ", width, height, bpp);
      else
        width=tiff=w=0;
    }
  }
  if (pos>eoi) return w=width=0;

  // Select nearby pixels as context
  if (!bpos) {
    int color=pos%3;
    int mean=buf(3)+buf(w-3)+buf(w)+buf(w+3);
    const int var=sqrbuf(3)+sqrbuf(w-3)+sqrbuf(w)+sqrbuf(w+3)-mean*mean/4>>2;
    mean>>=2;
    const int logvar=ilog(var);
    int i=0;
    cm1.set(hash(++i, buf(3)>>2, buf(w)>>2, color));
    cm2.set(hash(++i, buf(3)>>2, buf(1)>>2, color));
    cm3.set(hash(++i, buf(3)>>2, buf(2)>>2, color));
    cm4.set(hash(++i, buf(w)>>2, buf(1)>>2, color));
    cm5.set(hash(++i, buf(w)>>2, buf(1)>>2, color));
    cm6.set(hash(++i, buf(3)+buf(w)>>1, color));
    cm7.set(hash(++i, buf(3)+buf(w)>>3, buf(1)>>5, buf(2)>>5, color));
    cm8.set(hash(++i, mean, logvar>>5, color));
    scm1.set(buf(3)+buf(w)>>1);
    scm2.set(buf(3)+buf(w)-buf(w+3)>>1);
    scm3.set(buf(3)*2-buf(6)>>1);
    scm4.set(buf(w)*2-buf(w*2)>>1);
    scm5.set(buf(3)+buf(w)-buf(w-3)>>1);
    scm6.set(mean>>1|logvar<<1&0x180);
  }

  // Predict next bit
  scm1.mix(m);
  scm2.mix(m);
  scm3.mix(m);
  scm4.mix(m);
  scm5.mix(m);
  scm6.mix(m);
  cm1.mix(m);
  cm2.mix(m);
  cm3.mix(m);
  cm4.mix(m);
  cm5.mix(m);
  cm6.mix(m);
  cm7.mix(m);
  cm8.mix(m);

  return w;
}

//////////////////////////// jpegModel /////////////////////////

// Model JPEG. Return 1 if a JPEG file is detected or else 0.
// Only the baseline and 8 bit extended Huffman coded DCT modes are
// supported.  The model partially decodes the JPEG image to provide
// context for the Huffman coded symbols.

// Print a JPEG segment at buf[p...] for debugging
void dump(const char* msg, int p) {
  printf("%s:", msg);
  int len=buf[p+2]*256+buf[p+3];
  for (int i=0; i<len+2; ++i)
    printf(" %02X", buf[p+i]);
  printf("\n");
}

// Detect invalid JPEG data.  The proper response is to silently
// fall back to a non-JPEG model.
#define jassert(x) if (!(x)) { \
/*  printf("JPEG error at %d, line %d: %s\n", pos, __LINE__, #x); */ \
  jpeg=0; \
  return next_jpeg;}

struct HUF {U32 min, max; int val;}; // Huffman decode tables
  // huf[Tc][Th][m] is the minimum, maximum+1, and pointer to codes for
  // coefficient type Tc (0=DC, 1=AC), table Th (0-3), length m+1 (m=0-15)

int jpegModel(Mixer& m) {

  // State of parser
  enum {SOF0=0xc0, SOF1, SOF2, SOF3, DHT, RST0=0xd0, SOI=0xd8, EOI, SOS, DQT,
    DNL, DRI, APP0=0xe0, COM=0xfe, FF};  // Second byte of 2 byte codes
  static int jpeg=0;  // 1 if JPEG is detected
  static int next_jpeg=0;  // updated with jpeg on next byte boundary
  static int app;  // Bytes remaining to skip in APPx or COM field
  static int sof=0, sos=0, data=0;  // pointers to buf
  static Array<int> ht(8);  // pointers to Huffman table headers
  static int htsize=0;  // number of pointers in ht

  // Huffman decode state
  static U32 huffcode=0;  // Current Huffman code including extra bits
  static int huffbits=0;  // Number of valid bits in huffcode
  static int huffsize=0;  // Number of bits without extra bits
  static int rs=-1;  // Decoded huffcode without extra bits.  It represents
    // 2 packed 4-bit numbers, r=run of zeros, s=number of extra bits for
    // first nonzero code.  huffcode is complete when rs >= 0.
    // rs is -1 prior to decoding incomplete huffcode.
  static int mcupos=0;  // position in MCU (0-639).  The low 6 bits mark
    // the coefficient in zigzag scan order (0=DC, 1-63=AC).  The high
    // bits mark the block within the MCU, used to select Huffman tables.

  // Decoding tables
  static Array<HUF> huf(128);  // Tc*64+Th*16+m -> min, max, val
  static int mcusize=0;  // number of coefficients in an MCU
  static int linesize=0; // width of image in MCU
  static int hufsel[2][10];  // DC/AC, mcupos/64 -> huf decode table

  // Image state
  static Array<int> color(10);  // block -> component (0-3)
  static Array<int> pred(4);  // component -> last DC value
  static int dc=0;  // DC value of the current block
  static int width=0;  // Image width in MCU
  static int row=0, column=0;  // in MCU (column 0 to width-1)
  static Buf cbuf(0x20000); // Rotating buffer of coefficients, coded as:
    // DC: level shifted absolute value, low 4 bits discarded, i.e.
    //   [-1023...1024] -> [0...255].
    // AC: as an RS code: a run of R (0-15) zeros followed by an S (0-15)
    //   bit number, or 00 for end of block (in zigzag order).
    //   However if R=0, then the format is ssss11xx where ssss is S,
    //   xx is the first 2 extra bits, and the last 2 bits are 1 (since
    //   this never occurs in a valid RS code).
  static int cpos=0;  // position in cbuf
  static U32 huff1=0, huff2=0, huff3=0, huff4=0;  // hashes of last codes
  static int rs1, rs2, rs3, rs4;  // last 4 RS codes
  static int ssum=0, ssum1=0, ssum2=0, ssum3=0, ssum4=0;
    // sum of S in RS codes in block and last 4 values

  // Be sure to quit on a byte boundary
  if (!bpos) next_jpeg=jpeg;
  if (bpos && !jpeg) return next_jpeg;
  if (!bpos && app>0) --app;
  if (app>0) return next_jpeg;
  if (!bpos) {

    // Parse.  Baseline DCT-Huffman JPEG syntax is:
    // SOI APPx... misc... SOF0 DHT... SOS data EOI
    // SOI (= FF D8) start of image.
    // APPx (= FF Ex) len ... where len is always a 2 byte big-endian length
    //   including the length itself but not the 2 byte preceding code.
    //   Application data is ignored.  There may be more than one APPx.
    // misc codes are DQT, DNL, DRI, COM (ignored).
    // SOF0 (= FF C0) len 08 height width Nf [C HV Tq]...
    //   where len, height, width (in pixels) are 2 bytes, Nf is the repeat
    //   count (1 byte) of [C HV Tq], where C is a component identifier
    //   (color, 0-3), HV is the horizontal and vertical dimensions
    //   of the MCU (high, low bits, packed), and Tq is the quantization
    //   table ID (not used).  An MCU (minimum compressin unit) consists
    //   of 64*H*V DCT coefficients for each color.
    // DHT (= FF C4) len [TcTh L1...L16 V1,1..V1,L1 ... V16,1..V16,L16]...
    //   defines Huffman table Th (1-4) for Tc (0=DC (first coefficient)
    //   1=AC (next 63 coefficients)).  L1..L16 are the number of codes
    //   of length 1-16 (in ascending order) and Vx,y are the 8-bit values.
    //   A V code of RS means a run of R (0-15) zeros followed by S (0-15)
    //   additional bits to specify the next nonzero value, negative if
    //   the first additional bit is 0 (e.g. code x63 followed by the
    //   3 bits 1,0,1 specify 7 coefficients: 0, 0, 0, 0, 0, 0, 5.
    //   Code 00 means end of block (remainder of 63 AC coefficients is 0).
    // SOS (= FF DA) len Ns [Cs TdTa]... 0 3F 00
    //   Start of scan.  TdTa specifies DC/AC Huffman tables (0-3, packed
    //   into one byte) for component Cs matching C in SOF0, repeated
    //   Ns (1-4) times.
    // EOI (= FF D9) is end of image.
    // Huffman coded data is between SOI and EOI.  Codes may be embedded:
    // RST0-RST7 (= FF D0 to FF D7) mark the start of an independently
    //   compressed region.
    // DNL (= FF DC) 04 00 height
    //   might appear at the end of the scan (ignored).
    // FF 00 is interpreted as FF (to distinguish from RSTx, DNL, EOI).

    // Detect JPEG (SOI, APPx)
    if (!jpeg && buf(4)==FF && buf(3)==SOI && buf(2)==FF && buf(1)>>4==0xe) {
      jpeg=1;
      app=sos=sof=htsize=data=mcusize=linesize=0;
      huffcode=huffbits=huffsize=mcupos=cpos=0, rs=-1;
      memset(&huf[0], 0, huf.size()*sizeof(HUF));
      memset(&pred[0], 0, pred.size()*sizeof(int));
    }

    // Detect end of JPEG when data contains a marker other than RSTx
    // or byte stuff (00).
    if (jpeg && data && buf(2)==FF && buf(1) && (buf(1)&0xf8)!=RST0) {
      jassert(buf(1)==EOI);
      jpeg=0;
    }
    if (!jpeg) return next_jpeg;

    // Detect APPx or COM field
    if (!data && !app && buf(4)==FF && (buf(3)>>4==0xe || buf(3)==COM))
      app=buf(2)*256+buf(1)+2;

    // Save pointers to sof, ht, sos, data,
    if (buf(5)==FF && buf(4)==SOS) {
      int len=buf(3)*256+buf(2);
      if (len==6+2*buf(1) && buf(1) && buf(1)<=4)  // buf(1) is Ns
        sos=pos-5, data=sos+len+2;
    }
    if (buf(4)==FF && buf(3)==DHT && htsize<8) ht[htsize++]=pos-4;
    if (buf(4)==FF && buf(3)==SOF0) sof=pos-4;

    // Restart
    if (buf(2)==FF && (buf(1)&0xf8)==RST0) {
      huffcode=huffbits=huffsize=mcupos=0, rs=-1;
      memset(&pred[0], 0, pred.size()*sizeof(int));
    }
  }

  {
    // Build Huffman tables
    // huf[Tc][Th][m] = min, max+1 codes of length m, pointer to byte values
    if (pos==data && bpos==1) {
      jassert(htsize>0);
      for (int i=0; i<htsize; ++i) {
        int p=ht[i]+4;  // pointer to current table after length field
        int end=p+buf[p-2]*256+buf[p-1]-2;  // end of Huffman table
        int count=0;  // sanity check
        while (p<end && end<pos && end<p+2100 && ++count<10) {
          int tc=buf[p]>>4, th=buf[p]&15;
          if (tc>=2 || th>=4) break;
          jassert(tc>=0 && tc<2 && th>=0 && th<4);
          HUF* h=&huf[tc*64+th*16]; // [tc][th][0]; 
          int val=p+17;  // pointer to values
          int code=0;
          for (int j=0; j<16; ++j) {
            h[j].min=code;
            h[j].max=code+=buf[p+j+1];
            h[j].val=val;
            val+=buf[p+j+1];
            code*=2;
          }
          p=val;
        }
        jassert(p==end);
      }
      huffcode=huffbits=huffsize=0, rs=-1;

      // Build Huffman table selection table (indexed by mcupos).
      // Get image width.
      if (!sof && sos) return next_jpeg;
      int ns=buf[sos+4];
      int nf=buf[sof+9];
      jassert(ns<=4 && nf<=4);
      mcusize=0;  // blocks per MCU
      int hmax=0;  // MCU horizontal dimension
      for (int i=0; i<ns; ++i) {
        for (int j=0; j<nf; ++j) {
          if (buf[sos+2*i+5]==buf[sof+3*j+10]) { // Cs == C ?
            int hv=buf[sof+3*j+11];  // packed dimensions H x V
            if (hv>>4>hmax) hmax=hv>>4;
            hv=(hv&15)*(hv>>4);  // number of blocks in component C
            jassert(hv>=1 && hv+mcusize<=10);
            while (hv) {
              jassert(mcusize<10);
              hufsel[0][mcusize]=buf[sos+2*i+6]>>4&15;
              hufsel[1][mcusize]=buf[sos+2*i+6]&15;
              jassert (hufsel[0][mcusize]<4 && hufsel[1][mcusize]<4);
              color[mcusize]=i;
              --hv;
              ++mcusize;
            }
          }
        }
      }
      jassert(hmax>=1 && hmax<=10);
      width=buf[sof+7]*256+buf[sof+8];  // in pixels
      int height=buf[sof+5]*256+buf[sof+6];
      printf("JPEG %dx%d ", width, height);
      width=(width-1)/(hmax*8)+1;  // in MCU
      jassert(width>0);
      mcusize*=64;  // coefficients per MCU
      row=column=0;
    }
  }


  // Decode Huffman
  {
    if (mcusize && buf(1+(!bpos))!=FF) {  // skip stuffed byte
      jassert(huffbits<=32);
      huffcode+=huffcode+y;
      ++huffbits;
      if (rs<0) {
        jassert(huffbits>=1 && huffbits<=16);
        const int ac=(mcupos&63)>0;
        jassert(mcupos>=0 && (mcupos>>6)<10);
        jassert(ac==0 || ac==1);
        const int sel=hufsel[ac][mcupos>>6];
        jassert(sel>=0 && sel<4);
        const int i=huffbits-1;
        jassert(i>=0 && i<16);
        const HUF *h=&huf[ac*64+sel*16]; // [ac][sel];
        jassert(h[i].min<=h[i].max && h[i].val<pos && huffbits>0);
        if (huffcode<h[i].max) {
          jassert(huffcode>=h[i].min);
          rs=buf[h[i].val+huffcode-h[i].min];
          huffsize=huffbits;
        }
      }
      if (rs>=0) {
        if (huffsize+(rs&15)==huffbits) { // done decoding
          huff4=huff3;
          huff3=huff2;
          huff2=huff1;
          huff1=hash(huffcode, huffbits);
          rs4=rs3;
          rs3=rs2;
          rs2=rs1;
          rs1=rs;
          int x=0;  // decoded extra bits
          if (mcupos&63) {  // AC
            if (rs==0) { // EOB
              mcupos=mcupos+63&-64;
              jassert(mcupos>=0 && mcupos<=mcusize && mcupos<=640);
              while (cpos&63) cbuf[cpos++]=0;
            }
            else {  // rs = r zeros + s extra bits for the next nonzero value
                    // If first extra bit is 0 then value is negative.
              jassert((rs&15)<=10);
              const int r=rs>>4;
              const int s=rs&15;
              jassert(mcupos>>6==mcupos+r>>6);
              mcupos+=r+1;
              x=huffcode&(1<<s)-1;
              if (s && !(x>>s-1)) x-=(1<<s)-1;
              for (int i=r; i>=1; --i) cbuf[cpos++]=i<<4|s;
              cbuf[cpos++]=s<<4|huffcode<<2>>s&3|12;
              ssum+=s;
            }
          }
          else {  // DC: rs = 0S, s<12
            jassert(rs<12);
            ++mcupos;
            x=huffcode&(1<<rs)-1;
            if (rs && !(x>>rs-1)) x-=(1<<rs)-1;
            jassert(mcupos>=0 && mcupos>>6<10);
            const int comp=color[mcupos>>6];
            jassert(comp>=0 && comp<4);
            dc=pred[comp]+=x;
            jassert((cpos&63)==0);
            cbuf[cpos++]=dc+1023>>3;
            ssum4=ssum3;
            ssum3=ssum2;
            ssum2=ssum1;
            ssum1=ssum;
            ssum=rs;
          }
          jassert(mcupos>=0 && mcupos<=mcusize);
          if (mcupos>=mcusize) {
            mcupos=0;
            if (++column==width) column=0, ++row;
          }
          huffcode=huffsize=huffbits=0, rs=-1;
        }
      }
    }
  }

  // Estimate next bit probability
  if (!jpeg || !data) return next_jpeg;

  // Context model
  const int N=15;  // size of t, number of contexts
  static BH<8> t(MEM);  // context hash -> bit history
    // As a cache optimization, the context does not include the last 1-2
    // bits of huffcode if the length (huffbits) is not a multiple of 3.
    // The 7 mapped values are for context+{"", 0, 00, 01, 1, 10, 11}.
  static Array<U32> cxt(N);  // context hashes
  static Array<U8*> cp(N);  // context pointers
  static StateMap sm[N];
  static Mixer m1(N+1, 800, 4);
  static APM a1(1024), a2(0x10000);

  // Update model
  if (cp[N-1]) {
    for (int i=0; i<N; ++i)
      *cp[i]=nex(*cp[i],y);
  }
  m1.update();

  // Update context
  static int hbcount=0;
  if (++hbcount>2 || huffbits==0) hbcount=0;
  const int hc=huffcode|1<<huffbits;
  const int coef=(mcupos&63)|color[mcupos>>6]<<6;
  jassert(coef>=0 && coef<256);
  if (hbcount==0) {
    const int mpos=mcupos>>4|!(mcupos&-64)<<7;
    int n=0;
    cxt[0]=hash(++n, hc, mcupos>>2, min(3, mcupos&63));
    cxt[1]=hash(++n, hc, mpos>>4, cbuf[cpos-mcusize]);
    cxt[2]=hash(++n, hc, mpos>>4, cbuf[cpos-width*mcusize]);
    cxt[3]=hash(++n, hc, ilog(ssum3), mcupos);
    cxt[4]=hash(++n, hc, mcupos, column>>3);
    cxt[5]=hash(++n, hc, mcupos, column>>1);
    cxt[6]=hash(++n, hc, rs1, mpos);
    cxt[7]=hash(++n, hc, rs1, rs2);
    cxt[8]=hash(++n, hc, rs1, rs2, rs3);
    cxt[9]=hash(++n, hc, ssum>>4, mcupos);
    cxt[10]=hash(++n, hc, mpos, cbuf[cpos-1]);
    cxt[11]=hash(++n, hc, dc);
    cxt[12]=hash(++n, hc, rs1, coef);
    cxt[13]=hash(++n, hc, rs1, rs2, coef);
    cxt[14]=hash(++n, hc, mcupos>>3, ssum3>>3);
  }

  // Predict next bit
  m1.add(128);
  assert(hbcount<=2);
  for (int i=0; i<N; ++i) {
    if (hbcount==0) cp[i]=t[cxt[i]]+1;
    else if (hbcount==1) cp[i]+=1+(huffcode&1)*3;
    else cp[i]+=1+(huffcode&1);
    m1.add(stretch(sm[i].p(*cp[i])));
  }
  m1.set(0, 1);
  m1.set(coef, 64);
  m1.set(mcupos, 640);
  int pr=m1.p();
  pr=a1.p(pr, hc&1023);
  pr=a2.p(pr, hc&255|coef<<8);
  m.add(stretch(pr));
  return 1;
}

//////////////////////////// contextModel //////////////////////

// This combines all the context models with a Mixer.

int contextModel2() {
  static SmallContextMap cm0(0x200), cm1(0x20000);
  static ContextMap cm2(MEM), cm3(MEM), cm4(MEM),
    cm5(MEM*2), cm6(MEM*2), cm7(MEM*2), cm8(MEM*2);
  static RunContextMap rcm7(MEM), rcm9(MEM), rcm10(MEM);
  static Mixer m(512, 1024, 4);

  m.update();
  m.add(256);

  // Test for special file types
  int isjpeg=jpegModel(m);  // 1 if JPEG is detected, else 0
  int ismatch=matchModel(m);  // Length of longest matching context
  int isbmp=bmpModel(m);  // Image width (bytes) if BMP or TIFF detected, or 0
  if (ismatch>400) {   // Model long matches directly
    m.set(0, 8);
    return m.p();
  }
  else if (isjpeg) {
    m.set(1, 8);
    m.set(c0, 256);
    m.set(buf(1), 256);
    return m.p();
  }
  else if (isbmp) {
    static int col=0;
    if (++col>=24) col=0;
    m.set(2, 8);
    m.set(col, 24);
    m.set(buf(isbmp)+buf(3)>>4, 32);
    m.set(c0, 256);
    return m.p();
  }

  // Normal model
  if (bpos==0) {
    U32 h;
    int i=0;
    cm0.set(0);
    cm1.set(buf(1));
    cm2.set(hash(++i, buf(1), buf(2)));
    cm3.set(hash(++i, buf(1), buf(2), buf(3)));
    cm4.set(hash(++i, buf(1), buf(2), buf(3), buf(4)));
    cm5.set(h=hash(buf(1), buf(2), buf(3), buf(4), buf(5)));
    cm6.set(h^=hash(++i, h, buf(6)));
    rcm7.set(h^=hash(++i, h, buf(7)));
    cm7.set(h^=hash(++i, h, buf(8)));
    rcm9.set(h^=hash(++i, h, buf(9), buf(10)));
    rcm10.set(h^=hash(++i, h, buf(11), buf(12)));
    cm8.set(h^=hash(++i, h, buf(13), buf(14)));
  }
  int order=0;
  order+=cm0.mix(m);
  order+=cm1.mix(m);
  order+=cm2.mix(m);
  order+=cm3.mix(m);
  order+=cm4.mix(m);
  order+=cm5.mix(m);
  order+=cm6.mix(m);
  rcm7.mix(m);
  cm7.mix(m);
  rcm9.mix(m);
  rcm10.mix(m);
  cm8.mix(m);
  picModel(m);
  wordModel(m);
  recordModel(m);
  sparseModel(m);

  m.set(buf(1)+8, 264);
  m.set(c0, 256);
  m.set(order+8*(c4>>5&7)+64*(buf(1)==buf(2)), 128);
  m.set(buf(2), 256);
  int pr=m.p();
  return pr;
}

//////////////////////////// Predictor /////////////////////////

// A Predictor estimates the probability that the next bit of
// uncompressed data is 1.  Methods:
// p() returns P(1) as a 12 bit number (0-4095).
// update(y) trains the predictor with the actual bit (0 or 1).

class Predictor {
  int pr;  // next prediction
public:
  Predictor();
  int p() const {assert(pr>=0 && pr<4096); return pr;}
  void update();
};

Predictor::Predictor(): pr(2048) {}

void Predictor::update() {
  static APM a1(256), a2(0x10000), a3(0x10000), a4(0x10000);

  // Update global context: pos, bpos, c0, c4, buf, nibble
  c0+=c0+y;
  if (c0>=256) {
    buf[pos++]=c0;
    c4=(c4<<8)+c0-256;
    c0=1;
  }
  bpos=(bpos+1)&7;

  // Filter the context model with APMs
  pr=contextModel2();
  pr=a1.p(pr, c0)*3+pr>>2;
  int pr2=a2.p(pr, c0+256*buf(1));
  int pr3=a3.p(pr, c0^hash(buf(1), buf(2))&0xffff);
  int pr4=a4.p(pr, c0^hash(buf(1), buf(2), buf(3))&0xffff);
  pr=pr2+pr3*2+pr4+2>>2;
}

//////////////////////////// Encoder ////////////////////////////

// An Encoder does arithmetic encoding.  Methods:
// Encoder(COMPRESS, f) creates encoder for compression to archive f, which
//   must be open past any header for writing in binary mode.
// Encoder(DECOMPRESS, f) creates encoder for decompression from archive f,
//   which must be open past any header for reading in binary mode.
// code(i) in COMPRESS mode compresses bit i (0 or 1) to file f.
// code() in DECOMPRESS mode returns the next decompressed bit from file f.
//   Global y is set to the last bit coded or decoded by code().
// compress(c) in COMPRESS mode compresses one byte.
// decompress() in DECOMPRESS mode decompresses and returns one byte.
// flush() should be called exactly once after compression is done and
//   before closing f.  It does nothing in DECOMPRESS mode.

typedef enum {COMPRESS, DECOMPRESS} Mode;
class Encoder {
private:
  Predictor predictor;
  const Mode mode;       // Compress or decompress?
  FILE* archive;         // Compressed data file
  U32 x1, x2;            // Range, initially [0, 1), scaled by 2^32
  U32 x;                 // Decompress mode: last 4 input bytes of archive
public:
  Encoder(Mode m, FILE* f);
  void flush();
  int code(int i=0) {    // Compress bit y or return decompressed bit
    int p=predictor.p();
    p += p < 2048;
    U32 xmid = x1 + ((x2-x1) >> 6) * (p >> 6) + ((x2-x1) >> 12) * (p & 63);
    assert(xmid>=x1 && xmid<x2);
    if (mode==DECOMPRESS) y=x<=xmid; else y=i;
    y ? (x2=xmid) : (x1=xmid+1);
    predictor.update();
    while (((x1^x2)&0xff000000)==0) {  // pass equal leading bytes of range
      if (mode==COMPRESS) putc(x2>>24, archive);
      x1<<=8;
      x2=(x2<<8)+255;
      if (mode==DECOMPRESS) x=(x<<8)+(getc(archive)&255);  // EOF is OK
    }
    return y;
  }

  // Compress one byte
  void compress(int c) {
    assert(mode==COMPRESS);
    for (int i=7; i>=0; --i)
      code((c>>i)&1);
  }

  // Decompress and return one byte
  int decompress() {
    assert(mode==DECOMPRESS);
    int c=0;
    for (int i=0; i<8; ++i)
      c+=c+code();
    return c;
  }
};

Encoder::Encoder(Mode m, FILE* f): 
    mode(m), archive(f), x1(0), x2(0xffffffff), x(0) {
  if (mode==DECOMPRESS) {  // x = first 4 bytes of archive
    for (int i=0; i<4; ++i)
      x=(x<<8)+(getc(archive)&255);
  }
}

void Encoder::flush() {
  if (mode==COMPRESS)
    putc(x1>>24, archive);  // Flush first unequal byte of range
}


//////////////////////////// main program ////////////////////////////

// Read one line, return NULL at EOF or ^Z.  f may be opened ascii or binary.
// Trailing \r\n is dropped.  Lines longer than MAXLINE-1=511 are truncated.

/*char *getline(FILE *f=stdin) {
  const int MAXLINE=512;
  static char s[MAXLINE];
  int len=0, c;
  while ((c=getc(f))!=EOF && c!=26 && c!='\n' && len<MAXLINE-1) {
    if (c!='\r' && len<MAXLINE-1)
      s[len++]=c;
  }
  s[len]=0;
  if (c==EOF || c==26)
    return 0;
  else
    return s;
}*/

// Print i bytes compressed so far, leaving curser in place.
/*inline void print_status(long i) {
  if ((i&0x3fff)==0 && i>0)
     printf("%10ld \b\b\b\b\b\b\b\b\b\b\b", i);
}*/

// Compress/decompress files.  Usage: paq7 archive files...
// If archive does not exist, it is created and the named files are
// compressed.  If there are no file name arguments after the archive,
// then file names are read from input up to a blank line or EOF.
// If archive already exists, then the files in the archive are either
// extracted, or compared if the file already exists.  The files
// are extracted to the names listed on the command line in the
// order they were stored, defaulting to the stored names.

/*int main(int argc, char** argv) {
  clock_t start_time=clock();  // start timer
  long total_size=0;  // uncompressed size of all files

  // Check for args
  if (argc<2) {
    printf(PROGNAME " compressor/archiver (C) 2005, Matt Mahoney.\n"
      "Free under GPL, http://www.gnu.org/licenses/gpl.txt\n\n"
      "To compress: " PROGNAME " [-option] archive files...  (archive will be created)\n"
      "Or (Windows): dir/b | " PROGNAME " archive  (file names read from input)\n"
      "To decompress/compare: " PROGNAME " archive [files...] (defaults to stored names)\n"
      "To view contents: more < archive\n\n"
      "Options are -1 to -5 (use 62, 96, 163, 296, 525 MB memory), default -3\n"
      "You can't add/extract single files.  Max total file size is 2GB\n");
    return 0;
  }

    // Get option
  int option='3';
  if (argv[1][0]=='-')
    option=argv[1][1], ++argv, --argc;
  if (option<32) option=32;
  if (option<'1'||option>'5')
    fprintf(stderr, "Bad option -%c (use -1 to -5)\n", option), exit(1);

  // Test for archive.  If none, create one and write a header.
  // The first line is PROGNAME.  This is followed by a list of
  // file sizes (as decimal numbers) and names, separated by a tab
  // and ending with \r\n.  The last entry is followed by ^Z
  Mode mode=DECOMPRESS;
  FILE *f=fopen(argv[1], "rb");
  if (!f) {
    mode=COMPRESS;

    f=fopen(argv[1], "wb");
    if (!f)
      perror(argv[1]), exit(1);
    fprintf(f, "%s -%c\r\n", PROGNAME, option);

    // Get filenames
    if (argc==2)
      printf("Enter names of files to compress, followed by blank line\n");
    int i=2;
    while (true) {
      char *filename;
      if (argc==2) {
        filename=getline();
        if (!filename || !filename[0])
          break;
      }
      else {
        if (i==argc)
          break;
        filename=argv[i++];
      }

      // Test if files exist and get their sizes, store in archive header
      FILE *fi=fopen(filename, "rb");
      if (!fi)
        perror(filename);
      else {
        fseek(fi, 0, SEEK_END);  // get size
        long size=ftell(fi);
        total_size+=size;
        if (size<-1 || total_size<0) {
          fprintf(stderr, "File sizes must total less than 2 gigabytes\n");
          fprintf(f, "-1\tError: over 2 GB\r\n");
          exit(1);
        }
        if (size==-1)
          perror(filename);
        else
          fprintf(f, "%ld\t%s\r\n", size, filename);
        fclose(fi);
      }
    }
    fputc(26, f);  // EOF
    fclose(f);
  }

  // Read existing archive. Two pointers (header and body) track the
  // current filename and current position in the compressed data.
  if (mode==COMPRESS)
    f=fopen(argv[1], "r+b");
  else
    f=fopen(argv[1], "rb");
  if (!f)
    perror(argv[1]), exit(1);
  long header, body;  // file positions in header, body
  char *filename=getline(f);  // check header
  if (!filename || strncmp(filename, PROGNAME " -", strlen(PROGNAME)+2))
    fprintf(stderr, "%s: not a " PROGNAME " file\n", argv[1]), exit(1);
  option=filename[strlen(filename)-1];
  MEM=0x40000<<option-'1';
  header=ftell(f);

  // Initialize encoder at end of header
  if (mode==COMPRESS)
    fseek(f, 0, SEEK_END);
  else {  // body starts after ^Z in file
    int c;
    while ((c=getc(f))!=EOF && c!=26)
      ;
    if (c!=26)
      fprintf(stderr, "Archive %s is incomplete\n", argv[1]), exit(1);
  }
  Encoder en(mode, f);
  body=ftell(f);

  // Compress/decompress files listed on command line, or header if absent.
  int filenum=1;  // command line index
  total_size=0;
  fseek(f, header, SEEK_SET);
  while ((filename=getline(f))!=0) {
    long size=atol(filename);  // parse size and filename, separated by tab
    total_size+=size;
    while (*filename && *filename!='\t')
      ++filename;
    if (*filename=='\t')
      ++filename;
    printf("%ld\t%s", size, filename);
    if (mode==DECOMPRESS && ++filenum<argc
        && strcmp(argv[filenum], filename)) {
      printf(" -> %s", argv[filenum]);
      filename=argv[filenum];
    }
    printf(": ");
    if (size<0 || total_size<0) break;
    header=ftell(f);
    fseek(f, body, SEEK_SET);

    // If file exists in COMPRESS mode, compare, else compress/decompress
    FILE *fi=fopen(filename, "rb");
    if (mode==COMPRESS) {
      if (!fi)
        perror(filename), exit(1);
      for (long i=0; i<size; ++i) {
        print_status(i);
        en.compress(getc(fi));
      }
      printf(" -> %4ld  \n", ftell(f)-body);
    }
    else {  // DECOMPRESS
      if (fi) {  // compare
        int c1, c2;
        bool diff=false;
        for (long i=0; i<size; ++i) {
          print_status(i);
          c1=getc(fi);
          c2=en.decompress();
          if (c1!=c2 && !diff) {
            printf("differ at %ld: file=%d archive=%d\n", i, c1, c2);
            diff=true;
          }
        }
        if (!diff) {
          if (getc(fi)!=EOF)
            printf("file is longer\n");
          else
            printf("identical \n");
        }
      }
      else {  // extract
        fi=fopen(filename, "wb");
        if (fi) {
          for (long i=0; i<size; ++i) {
            print_status(i);
            putc(en.decompress(), fi);
          }
          fclose(fi);
          fi=0;
          printf("extracted \n");
        }
        else {  // skip
          perror(filename);
          for (long i=0; i<size; ++i)
            en.decompress();
        }
      }
    }
    if (fi)
      fclose(fi);
    body=ftell(f);
    fseek(f, header, SEEK_SET);
  }
  fseek(f, body, SEEK_SET);
  en.flush();

  // Print stats
  if (f) {
    if (mode==DECOMPRESS && filenum<argc-1)
      printf("No more files to extract\n");
    fseek(f, 0, SEEK_END);
    long compressed_size=ftell(f);
    if (mode==COMPRESS)
      printf("%ld -> %ld", total_size, compressed_size);
    else
      printf("%ld -> %ld", compressed_size, total_size);
    start_time=clock()-start_time;
    if (compressed_size>0 && total_size>0 && start_time>0) {
      printf(" (%1.4f bpc) in %1.2f sec (%1.3f KB/sec)",
        8.0*compressed_size/total_size,
        (double)start_time/CLOCKS_PER_SEC,
        0.001*total_size*CLOCKS_PER_SEC/start_time);
    }
    printf("\n");
    fclose(f);
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
		e->compress(data[i]);
	}*/

	char *last = &data[size];
	for(;data!=last;data++){
		e->compress(*data);
	}
}

void setMode(char mode){
	if(mode >= 0 && mode <= 9){
		//MEM=0x40000<<mode;
		MEM = 0x4000<<mode;
		//printf("mem: %d\n", MEM);
	}
}

void kgbdecompress(FILE *archive, char *data, unsigned __int64 size){
	if(e == NULL)
		e = new Encoder(DECOMPRESS, archive);

	/*for(unsigned __int64 i=0;i<size;i++){
		data[i] = e->decompress();
	}*/
	char *last = &data[size];
	for(;data!=last;data++){
		*data = e->decompress();
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

