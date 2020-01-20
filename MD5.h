
/*
 * This is an implementation of the RSA Data Security,
 * Inc. MD5 Message-Digest Algorithm (RFC 1321).
 *
 * Written by Solar Designer <solar at openwall.com> in 2001, and placed
 * in the public domain.  There's absolutely no warranty.
 *
 * This differs from Colin Plumb's older public domain implementation in
 * that no 32-bit integer data type is required, there's no compile-time
 * endianness configuration, and the function prototypes match OpenSSL's.
 * The primary goals are portability and ease of use.
 *
 * This implementation is meant to be fast, but not as fast as possible.
 * Some known optimizations are not included to reduce source code size
 * and avoid compile-time configuration.
 */

/*
 * Updated by Scott MacVicar for arduino
 * <scott@macvicar.net>
 */

/* Updated by Rickie S Kerndt
 * <kerndtr@kerndt.com>
 * Refactored into a more complete object oriented structure, replaced macros
 * with static const variables and inline functions. No longer OpenSSL
 * compatible.
 */

#ifndef MD5_H
#define MD5_H

//#include "Arduino.h"
#include <cstring>
#include <string>
#include <stdio.h>
#include <ctype.h>

using namespace std;

typedef unsigned int MD5_u32;

class MD5 {

public:

  // constants
  static const int HASH_LEN = 16;
  static const int DIGEST_LEN = HASH_LEN << 1;
  static const int BUFFER_LEN = 64;
  static const int SOURCE_SIZE_INDEX = 56; // buffer location for writing source size
  static const char HEX_BITS[];            // hex chars for generating human readable output

private:

  // padding
  static const unsigned char PADDING[];

  // chaining variables
  static const MD5_u32 _A = 0x67452301;
  static const MD5_u32 _B = 0xefcdab89;
  static const MD5_u32 _C = 0x98badcfe;
  static const MD5_u32 _D = 0x10325476;

  // MD5 context variables, buffers
  MD5_u32 _a, _b, _c, _d;
  char _buffer[BUFFER_LEN];  // working buffer for partial blocks and streams
  size_t _input_len;         // length of input in bytes
  size_t _blocks;            // number of 64 byte blocks to process in transform()
  size_t _bits;              // number of bits processed by transform()

public:

  MD5(void);        // does not initialize source context variables _input_len, _blocks.
  MD5(size_t len);  // Initializes source context variables from len (number bytes in source)

  ~MD5(void);       // zeros context state variables to clear sensitive data from memory

  /* MD5 hash functions
   * These are the entry functions to generate MD5 hashes. */
  static void make_hash(const char *data, size_t len, unsigned char *hash);
  static void make_hash(const void *data, size_t len, unsigned char *hash);
  static void make_hash(const string &data, unsigned char *hash);
  static void make_hash(FILE *f, unsigned char *hash);

  /* Utility function to generate a human readable c_string from MD5 hash.
   * hash   - pointer to null terminated char array holding MD5 hash.
   *          Should be a 17 element array.
   * digest - pointer to the unsigned char array to store generated hash.  Should be at least
   *          17 char wide.
   */
  static void make_digest(const unsigned char *hash, char *digest);

  /* Utility function to compare two hashes for equality. Takes pointer
   * to a 17 element unsigned char array. */
  bool comp_hash(const unsigned char *hash_1, const unsigned char *hash_2);

  /* Initializes MD5 context variables and buffer. */
  void init(void);

  /* Processes 64 byte blocks for the MD5 transforms. */
  const char *transform(const char *data);

  /* Transform remaining bits less than a full block with required padding and cummulates
   * the context variables. */
  void finalize(const char *data);

  /* Encodes the context variables into an unsigned char array
   * hash - unsigned char pointer to a 17 element array */
  void encode(unsigned char *hash);

private:

  /* The basic MD5 functions.
   * F and G are optimized compared to their RFC 1321 definitions for
   * architectures that lack an AND-NOT instruction, just like in Colin Plumb's
   * implementation.
   * These functions replace the macro definitions used in the original source
   * Be sure to enable -finline-functions.
   */
  MD5_u32 _F(MD5_u32& cx2, MD5_u32& cx3, MD5_u32& cx4) { return (cx4 ^ (cx2 & (cx3 ^ cx4))); }
  MD5_u32 _G(MD5_u32& cx2, MD5_u32& cx3, MD5_u32& cx4) { return (cx3 ^ (cx4 & (cx2 ^ cx3))); }
  MD5_u32 _H(MD5_u32& cx2, MD5_u32& cx3, MD5_u32& cx4) { return ((cx2 ^ cx3) ^ cx4); }
  MD5_u32 _I(MD5_u32& cx2, MD5_u32& cx3, MD5_u32& cx4) { return cx3 ^ (cx2 | (~cx4)); }

  /* The MD5 transformation for all four rounds.
   * Variables:
   *   n  - result of a basic MD5 function (cx2, cx3, cx4) , use the above function calls
   *   in the argument list.
   *   cx1 - reference to the cummulative MD5 context variable.
   *   cx2 - reference to the cummulative MD5 context variable.
   *   x   - the data element being processed.
   *   sf  - sine function constant
   */
  void step(MD5_u32 n, MD5_u32& cx1, MD5_u32& cx2, MD5_u32 x, MD5_u32 sf, MD5_u32 s)
  {
    cx1 += n + x + sf;
    cx1 = (cx1 << s) | (cx1 >> (32 - s));
    cx1 += cx2;
  }

  /* Convert four char elements to a 32 bit int. */
  MD5_u32 decode(const char *data, int index)
  {
    return ( *(MD5_u32*) (data + (index << 2)));
  }

};
#endif
