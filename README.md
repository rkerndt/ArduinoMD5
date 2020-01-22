A C++ implementation of the MD5 hash (rfc 1321). This was built off work by
Solar Designer and Scott MacVicar. Macros are replaced by static const variables
and use of in-line functions with performance equivalent to the c style
code used in the Solar Designer implementation as well as the bsd md5.

#### Class MD5 : MD5.{h,c}

Class MD5 is the rfc1321 MD5 alrorithm. There are five public static
functions that server as the entry point for generating MD5 hashes.
These are:
  * MD5::make_hash(const char *data, size_t len, unsigned char *hash)
  * MD5::make_hash(const void *data, size_t len, unsigned char *hash)
  * MD5::make_hash(const string &data, unsigned char *hash)
  * MD5::make_hash(FILE *f, unsigned char *hash)
  * MD5::make_digest(const unsigned char *hash, char *digest)

These functions store the hash and digest (human readable) in char
arrays and are responsible for instantiating and clearing the MD5
class object to compute the hash. If you need to hash a different
source type, then either do the conversion in the calling function or
extend the class by overloading the make_hash() function to handle the
required source type.

#### Class MD5Hash : MD5Hash.{h,c}

Class MD5Hash provides a container for the hash with functions for
comparing hashes and generating the human readable form. MD5Hash provides
wrappers for the MD5 class make_hash functions that return a MD5Hash
object.

  * static MD5Hash make_MD5Hash(const char *data, size_t len);
  * static MD5Hash make_MD5Hash(const void *data, size_t len);
  * static MD5Hash make_MD5Hash(const string &data);
  * static MD5Hash make_MD5Hash(FILE *f);

#### Reference implementations:

  * bsd-md5 uses the md5 functions from the linux bsd compatibility
  library.

  * mddriver is an implementation of mddriver.c found in rfc1321 and uses
  the md5 core functions provided by openwell (openwell-md5.{c,h}).

## Examples
There is an executable based on mddriver.c written to test Class MD5
(main.cxx) and Class MD5Hash (MD5Hash-test.cxx) that provide usuage
examples.


## Compiling

The speed of the MD5 library depends on inlining the core functons:
_F, _G, _H, _I, step, and decode. I recommend using the compiling
optimization "-O -inline-functions" to compile MD5.cpp. Tested with gcc
 version 9.2.0.

