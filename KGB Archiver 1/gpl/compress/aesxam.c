/*
 -------------------------------------------------------------------------
 Copyright (c) 2001, Dr Brian Gladman <                 >, Worcester, UK.
 All rights reserved.

 LICENSE TERMS

 The free distribution and use of this software in both source and binary 
 form is allowed (with or without changes) provided that:

   1. distributions of this source code include the above copyright 
      notice, this list of conditions and the following disclaimer;

   2. distributions in binary form include the above copyright
      notice, this list of conditions and the following disclaimer
      in the documentation and/or other associated materials;

   3. the copyright holder's name is not used to endorse products 
      built using this software without specific written permission. 

 DISCLAIMER

 This software is provided 'as is' with no explcit or implied warranties
 in respect of any properties, including, but not limited to, correctness 
 and fitness for purpose.
 -------------------------------------------------------------------------
 Issue Date: 18/08/2002
*/

//  An example of the use of AES (Rijndael) for file encryption.  This code
//  implements AES in CBC mode with ciphertext stealing when the file length
//  is greater than one block (16 bytes).  This code is an example of how to
//  use AES and is not intended for real use since it does not provide any 
//  file integrity checking.
//
//  The Command line is:
//
//      aesxam input_file_name output_file_name [D|E] hexadecimalkey
//
//  where E gives encryption and D decryption of the input file into the 
//  output file using the given hexadecimal key string.  The later is a 
//  hexadecimal sequence of 32, 48 or 64 digits.  Examples to encrypt or 
//  decrypt aes.c into aes.enc are:
//
//      aesxam file.c file.enc E 0123456789abcdeffedcba9876543210
//
//      aesxam file.enc file2.c D 0123456789abcdeffedcba9876543210
//
//  which should return a file 'file2.c' identical to 'file.c'

//  Ciphertext stealing modifies the encryption of the last two CBC
//  blocks. It can be applied invariably to the last two plaintext
//  blocks or only applied when the last block is a partial one. For
//  a plaintext consisting of N blocks, with the last block possibly
//  a partial one, ciphertext stealing works as shown below (note the
//  reversal of the last two ciphertext blocks).  During decryption 
//  the part of the C:N-1 block that is not transmitted (x) can be 
//  recoverd from the decryption of the last but one ciphertext block
//  since this is xored with the zero padding appended to the last
//  plaintext block.
//
//    +---------+   +---------+   +---------+   +-------+-+
//    |  P:N-4  |   |  P:N-3  |   |  P:N-2  |   | P:N-1 |0|
//    +---------+   +---------+   +---------+   +-------+-+
//         |             |             |             |
//         v             v             v             v
//  +----->x      +----->x      +----->x      +----->x   x = xor
//  |      |      |      |      |      |      |      |
//  |      v      |      v      |      v      |      v
//  |    +---+    |    +---+    |    +---+    |    +---+
//  |    | E |    |    | E |    |    | E |    |    | E |
//  |    +---+    |    +---+    |    +---+    |    +---+
//  |      |      |      |      |      |      |      |
//  |      |      |      |      |      v      |      |
//  |      |      |      |      | +-------+-+ |      |
//  |      |      |      |      | | C:n-1 |x|-+      |
//  |      |      |      |      | +-------+-+        |
//  |      |      |      |      |     |              |
//  |      |      |      |      |     +------------+ |
//  |      |      |      |      |      +-----------|-+
//  |      |      |      |      |      |           |
//  |      v      |      v      |      v           v
//  | +---------+ | +---------+ | +---------+  +-------+
// -+ |  C:N-4  |-+ |  C:N-3  |-+ |  C:N-2  |  | C:N-1 |
//    +---------+   +---------+   +---------+  +-------+

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

#include "aes.h"

#define BLOCK_LEN   16
#define READ_ERROR  -7
#define WRITE_ERROR -8

#ifdef LINUX
#define file_len(x) (unsigned long)x.__pos
#else
#define file_len(x) (unsigned long)x
#endif

//  A Pseudo Random Number Generator (PRNG) used for the
//  Initialisation Vector. The PRNG is George Marsaglia's
//  Multiply-With-Carry (MWC) PRNG that concatenates two
//  16-bit MWC generators:
//      x(n)=36969 * x(n-1) + carry mod 2^16
//      y(n)=18000 * y(n-1) + carry mod 2^16
//  to produce a combined PRNG with a period of about 2^60.
//  The Pentium cycle counter is used to initialise it. This
//  is crude but the IV does not really need to be secret.

void cycles(volatile unsigned __int64 *rtn)    
{   
#if defined(_MSC_VER)
    __asm   // read the Pentium Time Stamp Counter
    {   cpuid
        rdtsc
        mov     ecx,rtn
        mov     [ecx],eax
        mov     [ecx+4],edx
        cpuid
    }
#else
#include <time.h>
  time_t tt;
  tt     = time(NULL);
  rtn[0] = tt;
  rtn[1] = tt & -36969l;
  return;
#endif
}

#define RAND(a,b) (((a = 36969 * (a & 65535) + (a >> 16)) << 16) + \
                    (b = 18000 * (b & 65535) + (b >> 16))  )

void fillrand(char *buf, const int len)
{   static unsigned long a[2], mt = 1, count = 4;
    static char          r[4];
    int                  i;

    if(mt) { mt = 0; cycles((unsigned __int64 *)a); }

    for(i = 0; i < len; ++i)
    {
        if(count == 4)
        {
            *(unsigned long*)r = RAND(a[0], a[1]);
            count = 0;
        }

        buf[i] = r[count++];
    }
}

int encfile(FILE *fin, FILE *fout, aes_ctx *ctx, const char* ifn, const char* ofn)
{   char            buf[BLOCK_LEN], dbuf[2 * BLOCK_LEN];
    fpos_t          flen;
    unsigned long   i, len, rlen;

    // set a random IV

    fillrand(dbuf, BLOCK_LEN);

    // find the file length

    fseek(fin, 0, SEEK_END);
    fgetpos(fin, &flen); 
    rlen = file_len(flen);
    // reset to start
    fseek(fin, 0, SEEK_SET);

    if(rlen <= BLOCK_LEN)               
    {   // if the file length is less than or equal to 16 bytes
        
        // read the bytes of the file into the buffer and verify length
        len = (unsigned long) fread(dbuf + BLOCK_LEN, 1, BLOCK_LEN, fin);
        rlen -= len;        
        if(rlen > 0) 
            return READ_ERROR;

        // pad the file bytes with zeroes
        for(i = len; i < BLOCK_LEN; ++i)
            dbuf[i + BLOCK_LEN] = 0;

        // xor the file bytes with the IV bytes
        for(i = 0; i < BLOCK_LEN; ++i)
            dbuf[i + BLOCK_LEN] ^= dbuf[i];

        // encrypt the top 16 bytes of the buffer
        aes_enc_blk(dbuf + BLOCK_LEN, dbuf + len, ctx);

        len += BLOCK_LEN;
        // write the IV and the encrypted file bytes
        if(fwrite(dbuf, 1, len, fout) != len)
            return WRITE_ERROR;
    }
    else
    {   // if the file length is more 16 bytes
        
        // write the IV
        if(fwrite(dbuf, 1, BLOCK_LEN, fout) != BLOCK_LEN)
            return WRITE_ERROR;

        // read the file a block at a time 
        while(rlen > 0 && !feof(fin))
        {  
            // read a block and reduce the remaining byte count
            len = (unsigned long)fread(buf, 1, BLOCK_LEN, fin);
            rlen -= len;

            // verify length of block 
            if(len != BLOCK_LEN) 
                return READ_ERROR;

            // do CBC chaining prior to encryption
            for(i = 0; i < BLOCK_LEN; ++i)
                buf[i] ^= dbuf[i];

            // encrypt the block
            aes_enc_blk(buf, dbuf, ctx);

            // if there is only one more block do ciphertext stealing
            if(rlen > 0 && rlen < BLOCK_LEN)
            {
                // move the previous ciphertext to top half of double buffer
                // since rlen bytes of this are output last
                for(i = 0; i < BLOCK_LEN; ++i)
                    dbuf[i + BLOCK_LEN] = dbuf[i];

                // read last part of plaintext into bottom half of buffer
                if(fread(dbuf, 1, rlen, fin) != rlen)
                    return READ_ERROR;

                // clear the remainder of the bottom half of buffer
                for(i = 0; i < BLOCK_LEN - rlen; ++i)
                    dbuf[rlen + i] = 0;

                // do CBC chaining from previous ciphertext
                for(i = 0; i < BLOCK_LEN; ++i)
                    dbuf[i] ^= dbuf[i + BLOCK_LEN];

                // encrypt the final block
                aes_enc_blk(dbuf, dbuf, ctx);

                // set the length of the final write
                len = rlen + BLOCK_LEN; rlen = 0;
            }

            // write the encrypted block
            if(fwrite(dbuf, 1, len, fout) != len)
                return WRITE_ERROR;
        }
    }

    return 0;
}

int decfile(FILE *fin, FILE *fout, aes_ctx *ctx, const char* ifn, const char* ofn)
{   char            buf1[BLOCK_LEN], buf2[BLOCK_LEN], dbuf[2 * BLOCK_LEN];
    char            *b1, *b2, *bt;
    fpos_t          flen;
    unsigned long   i, len, rlen;

    // find the file length

    fseek(fin, 0, SEEK_END);
    fgetpos(fin, &flen); 
    rlen = file_len(flen);
    // reset to start
    fseek(fin, 0, SEEK_SET);

    if(rlen <= 2 * BLOCK_LEN)
    {   // if the original file length is less than or equal to 16 bytes

        // read the bytes of the file and verify length
        len = (unsigned long)fread(dbuf, 1, 2 * BLOCK_LEN, fin);
        rlen -= len;
        if(rlen > 0)
            return READ_ERROR;

        // set the original file length
        len -= BLOCK_LEN;

        // decrypt from position len to position len + BLOCK_LEN
        aes_dec_blk(dbuf + len, dbuf + BLOCK_LEN, ctx);

        // undo CBC chaining
        for(i = 0; i < len; ++i)
            dbuf[i] ^= dbuf[i + BLOCK_LEN];

        // output decrypted bytes
        if(fwrite(dbuf, 1, len, fout) != len)
            return WRITE_ERROR; 
    }
    else
    {   // we need two input buffers because we have to keep the previous
        // ciphertext block - the pointers b1 and b2 are swapped once per
        // loop so that b2 points to new ciphertext block and b1 to the
        // last ciphertext block

        rlen -= BLOCK_LEN; b1 = buf1; b2 = buf2;

        // input the IV
        if(fread(b1, 1, BLOCK_LEN, fin) != BLOCK_LEN)
            return READ_ERROR;
        
        // read the encrypted file a block at a time
        while(rlen > 0 && !feof(fin))
        {
            // input a block and reduce the remaining byte count
            len = (unsigned long)fread(b2, 1, BLOCK_LEN, fin);
            rlen -= len;

            // verify the length of the read operation
            if(len != BLOCK_LEN)
                return READ_ERROR;

            // decrypt input buffer
            aes_dec_blk(b2, dbuf, ctx);

            // if there is only one more block do ciphertext stealing
            if(rlen > 0 && rlen < BLOCK_LEN)
            {
                // read last ciphertext block
                if(fread(b2, 1, rlen, fin) != rlen)
                    return READ_ERROR;

                // append high part of last decrypted block
                for(i = rlen; i < BLOCK_LEN; ++i)
                    b2[i] = dbuf[i];

                // decrypt last block of plaintext
                for(i = 0; i < rlen; ++i)
                    dbuf[i + BLOCK_LEN] = dbuf[i] ^ b2[i];

                // decrypt last but one block of plaintext
                aes_dec_blk(b2, dbuf, ctx);

                // adjust length of last output block
                len = rlen + BLOCK_LEN; rlen = 0;
            }

            // unchain CBC using the last ciphertext block
            for(i = 0; i < BLOCK_LEN; ++i)
                dbuf[i] ^= b1[i];

            // write decrypted block
            if(fwrite(dbuf, 1, len, fout) != len)
                return WRITE_ERROR;

            // swap the buffer pointers
            bt = b1, b1 = b2, b2 = bt;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{   FILE        *fin = 0, *fout = 0;
    char        *cp, ch, key[32];
    int         i, by, key_len, err = 0;
    aes_ctx     ctx[1];

    if(argc != 5 || toupper(*argv[3]) != 'D' && toupper(*argv[3]) != 'E')
    {
        printf("usage: aesxam in_filename out_filename [d/e] key_in_hex\n");
        err = -1; goto exit;
    }

    ctx->n_rnd = 0; // ensure all flags are initially set to zero
    ctx->n_blk = 0;
    cp = argv[4];   // this is a pointer to the hexadecimal key digits
    i = 0;          // this is a count for the input digits processed

    while(i < 64 && *cp)        // the maximum key length is 32 bytes and
    {                           // hence at most 64 hexadecimal digits
        ch = toupper(*cp++);    // process a hexadecimal digit
        if(ch >= '0' && ch <= '9')
            by = (by << 4) + ch - '0';
        else if(ch >= 'A' && ch <= 'F')
            by = (by << 4) + ch - 'A' + 10;
        else                    // error if not hexadecimal
        {
            printf("key must be in hexadecimal notation\n");
            err = -2; goto exit;
        }

        // store a key byte for each pair of hexadecimal digits
        if(i++ & 1)
            key[i / 2 - 1] = by & 0xff;
    }

    if(*cp)
    {
        printf("The key value is too long\n");
        err = -3; goto exit;
    }
    else if(i < 32 || (i & 15))
    {
        printf("The key length must be 32, 48 or 64 hexadecimal digits\n");
        err = -4; goto exit;
    }

    key_len = i / 2;

    if(!(fin = fopen(argv[1], "rb")))   // try to open the input file
    {
        printf("The input file: %s could not be opened\n", argv[1]);
        err = -5; goto exit;
    }

    if(!(fout = fopen(argv[2], "wb")))  // try to open the output file
    {
        printf("The input file: %s could not be opened\n", argv[1]);
        err = -6; goto exit;
    }

    if(toupper(*argv[3]) == 'E') // encryption in Cipher Block Chaining mode
    {
        aes_enc_key(key, key_len, ctx);

        err = encfile(fin, fout, ctx, argv[1], argv[2]);
    }
    else                         // decryption in Cipher Block Chaining mode
    {
        aes_dec_key(key, key_len, ctx);

        err = decfile(fin, fout, ctx, argv[1], argv[2]);
    }
exit:
    if(err == READ_ERROR)
        printf("Error reading from input file: %s\n", argv[1]);

    if(err == WRITE_ERROR)
        printf("Error writing to output file: %s\n", argv[2]);

    if(fout)
        fclose(fout);

    if(fin)
        fclose(fin);

    return err;
}
