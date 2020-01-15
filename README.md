A C++ implementation of the MD5 hash (rfc 1321). This was built off work by
Solar Designer and Scott MacVicar. Macros are replaced by static const variables
and use of in-line functions with performance equivalent to the c style
code used in the Solar Designer implementation as well as the bsd md5.

Two reference implementations are included and were used for testing and
time trials.

  bsd-md5 uses the md5 functions from the linux bsd compatibility
  library.

  mddriver is an implementation of mddriver.c found in rfc1321 and uses
  the md5 core functions provided by openwell (openwell-md5.{c,h}).

## Compiling
The speed of the MD5 library depends on inlining the core functons:
_F, _G, _H, _I, step, and decode. I recommend using the compiling
optimization "-O -inline-functions" to compile MD5.cpp. Do not use the
size restriction optimization "-Os" as this will not inline the step
function. Tested with gcc version 9.2.0.

## Usage

The MD5 class has three public static functions for generating the MD5
hash and digest:

  * MD5::make_hash(const char *data, size_t len, unsigned char *hash)
  * MD5::make_hash(FILE *f, unsigned char *hash)
  * MD5::make_digest(const unsigned char *hash, char *digest)

Both make_hash functions store the computed hash into the unsigned char
array. Generate a human readable c_string using the make_digest()
function.

The core MD5 routines are private to the class with context variables
zeroized at completion to clean memory of sensitive data. The main.cxx
(md5 executable) can be used as examples on how to use the MD5 class.
Please overload the make_hash function to handle different source types
rather than modifying the core functions directly.


