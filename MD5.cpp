/*
 * main.cxx
 *
 * Copyright 2019 Rickie S. Kerndt <kerndtr@kerndt.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include "MD5.h"


const unsigned char MD5::PADDING[] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

const char MD5::HEX_BITS[] = {'0', '1', '2', '3', '4', '5', '6', \
                              '7', '8', '9', 'a', 'b', 'c', 'd', \
                              'e', 'f'};

MD5::MD5(void)
{
  init();
}

MD5::MD5(size_t len)
{
  init();
  this->_input_len = len;
  this->_blocks = len >> 6;
}

MD5::~MD5(void)
{
  init();
}

void MD5::init(void)
{
  this->_a = MD5::_A;
  this->_b = MD5::_B;
  this->_c = MD5::_C;
  this->_d = MD5::_D;
  memset(this->_buffer, '\0', BUFFER_LEN);
  this->_input_len = 0;
  this->_bits = 0;
  this->_blocks = 0;
}

void MD5::make_digest(const unsigned char *hash, char *digest)
{
  char *p = digest;
  int j = 0;
  for(int i = 0; i < HASH_LEN; i++)
  {
    j = i << 1;
    digest[j] = HEX_BITS[(hash[i] >> 4) & 0xf];
    digest[j+1] = HEX_BITS[hash[i] & 0xf];
    p += 2;
  }
}

/* Processes 64 byte blocks for the MD5 transforms.
 * Set the MD5 class member _blocks to the number of full blocks */
const char * MD5::transform(const char *data)
{
  MD5_u32 a, b, c, d;
  MD5_u32 saved_a, saved_b, saved_c, saved_d;

  a = this->_a;
  b = this->_b;
  c = this->_c;
  d = this->_d;

  do {
    saved_a = a;
    saved_b = b;
    saved_c = c;
    saved_d = d;

/* Round 1 */
    step(_F(b, c, d), a, b, decode(data,0), 0xd76aa478, 7);   //1 [ABCD  0  7  1]
    step(_F(a, b, c), d, a, decode(data,1), 0xe8c7b756, 12);  //2 [DABC  1 12  2]
    step(_F(d, a, b), c, d, decode(data,2), 0x242070db, 17);  //3 [CDAB  2 17  3]
    step(_F(c, d, a), b, c, decode(data,3), 0xc1bdceee, 22);  //4 [BCDA  3 22  4]
    step(_F(b, c, d), a, b, decode(data,4), 0xf57c0faf, 7);   //5 [ABCD  4  7  5]
    step(_F(a, b, c), d, a, decode(data,5), 0x4787c62a, 12);  //6 [DABC  5 12  6]
    step(_F(d, a, b), c, d, decode(data,6), 0xa8304613, 17);  //7 [CDAB  6 17  7]
    step(_F(c, d, a), b, c, decode(data,7), 0xfd469501, 22);  //8 [BCDA  7 22  8]
    step(_F(b, c, d), a, b, decode(data,8), 0x698098d8, 7);   //9 [ABCD  8  7  9]
    step(_F(a, b, c), d, a, decode(data,9), 0x8b44f7af, 12);  //10 [DABC  9 12 10]
    step(_F(d, a, b), c, d, decode(data,10), 0xffff5bb1, 17); //11 [CDAB 10 17 11]
    step(_F(c, d, a), b, c, decode(data,11), 0x895cd7be, 22); //12 [BCDA 11 22 12]
    step(_F(b, c, d), a, b, decode(data,12), 0x6b901122, 7);  //13 [ABCD 12  7 13]
    step(_F(a, b, c), d, a, decode(data,13), 0xfd987193, 12); //14 [DABC 13 12 14]
    step(_F(d, a, b), c, d, decode(data,14), 0xa679438e, 17); //15 [CDAB 14 17 15]
    step(_F(c, d, a), b, c, decode(data,15), 0x49b40821, 22); //16 [BCDA 15 22 16]

/* Round 2 */
    step(_G(b, c, d), a, b, decode(data,1), 0xf61e2562, 5);   //17 [ABCD  1  5 17]
    step(_G(a, b, c), d, a, decode(data,6), 0xc040b340, 9);   //18 [DABC  6  9 18]
    step(_G(d, a, b), c, d, decode(data,11), 0x265e5a51, 14); //19 [CDAB 11 14 19]
    step(_G(c, d, a), b, c, decode(data,0), 0xe9b6c7aa, 20);  //20 [BCDA  0 20 20]
    step(_G(b, c, d), a, b, decode(data,5), 0xd62f105d, 5);   //21 [ABCD  5  5 21]
    step(_G(a, b, c), d, a, decode(data,10), 0x02441453, 9);  //22 [DABC 10  9 22]
    step(_G(d, a, b), c, d, decode(data,15), 0xd8a1e681, 14); //23 [CDAB 15 14 23]
    step(_G(c, d, a), b, c, decode(data,4), 0xe7d3fbc8, 20);  //24 [BCDA  4 20 24]
    step(_G(b, c, d), a, b, decode(data,9), 0x21e1cde6, 5);   //25 [ABCD  9  5 25]
    step(_G(a, b, c), d, a, decode(data,14), 0xc33707d6, 9);  //26 [DABC 14  9 26]
    step(_G(d, a, b), c, d, decode(data,3), 0xf4d50d87, 14);  //27 [CDAB  3 14 27]
    step(_G(c, d, a), b, c, decode(data,8), 0x455a14ed, 20);  //28 [BCDA  8 20 28]
    step(_G(b, c, d), a, b, decode(data,13), 0xa9e3e905, 5);  //29 [ABCD 13  5 29]
    step(_G(a, b, c), d, a, decode(data,2), 0xfcefa3f8, 9);   //30 [DABC  2  9 30]
    step(_G(d, a, b), c, d, decode(data,7), 0x676f02d9, 14);  //31 [CDAB  7 14 31]
    step(_G(c, d, a), b, c, decode(data,12), 0x8d2a4c8a, 20); //32 [BCDA 12 20 32]

/* Round 3 */
    step(_H(b, c, d), a, b, decode(data,5), 0xfffa3942, 4);   //33 [ABCD  5  4 33]
    step(_H(a, b, c), d, a, decode(data,8), 0x8771f681, 11);  //34 [DABC  8 11 34]
    step(_H(d, a, b), c, d, decode(data,11), 0x6d9d6122, 16); //35 [CDAB 11 16 35]
    step(_H(c, d, a), b, c, decode(data,14), 0xfde5380c, 23); //36 [BCDA 14 23 36]
    step(_H(b, c, d), a, b, decode(data,1), 0xa4beea44, 4);   //37 [ABCD  1  4 37]
    step(_H(a, b, c), d, a, decode(data,4), 0x4bdecfa9, 11);  //38 [DABC  4 11 38]
    step(_H(d, a, b), c, d, decode(data,7), 0xf6bb4b60, 16);  //39 [CDAB  7 16 39]
    step(_H(c, d, a), b, c, decode(data,10), 0xbebfbc70, 23); //40 [BCDA 10 23 40]
    step(_H(b, c, d), a, b, decode(data,13), 0x289b7ec6, 4);  //41 [ABCD 13  4 41]
    step(_H(a, b, c), d, a, decode(data,0), 0xeaa127fa, 11);  //42 [DABC  0 11 42]
    step(_H(d, a, b), c, d, decode(data,3), 0xd4ef3085, 16);  //43 [CDAB  3 16 43]
    step(_H(c, d, a), b, c, decode(data,6), 0x04881d05, 23);  //44 [BCDA  6 23 44]
    step(_H(b, c, d), a, b, decode(data,9), 0xd9d4d039, 4);   //45 [ABCD  9  4 45]
    step(_H(a, b, c), d, a, decode(data,12), 0xe6db99e5, 11); //46 [DABC 12 11 46]
    step(_H(d, a, b), c, d, decode(data,15), 0x1fa27cf8, 16); //47 [CDAB 15 16 47]
    step(_H(c, d, a), b, c, decode(data,2), 0xc4ac5665, 23);  //48 [BCDA  2 23 48]

/* Round 4 */
    step(_I(b, c, d), a, b, decode(data,0), 0xf4292244, 6);   //49 [ABCD  0  6 49]
    step(_I(a, b, c), d, a, decode(data,7), 0x432aff97, 10);  //50 [DABC  7 10 50]
    step(_I(d, a, b), c, d, decode(data,14), 0xab9423a7, 15); //51 [CDAB 14 15 51]
    step(_I(c, d, a), b, c, decode(data,5), 0xfc93a039, 21);  //52 [BCDA  5 21 52]
    step(_I(b, c, d), a, b, decode(data,12), 0x655b59c3, 6);  //53 [ABCD 12  6 53]
    step(_I(a, b, c), d, a, decode(data,3), 0x8f0ccc92, 10);  //54 [DABC  3 10 54]
    step(_I(d, a, b), c, d, decode(data,10), 0xffeff47d, 15); //55 [CDAB 10 15 55]
    step(_I(c, d, a), b, c, decode(data,1), 0x85845dd1, 21);  //56 [BCDA  1 21 56]
    step(_I(b, c, d), a, b, decode(data,8), 0x6fa87e4f, 6);   //57 [ABCD  8  6 57]
    step(_I(a, b, c), d, a, decode(data,15), 0xfe2ce6e0, 10); //58 [DABC 15 10 58]
    step(_I(d, a, b), c, d, decode(data,6), 0xa3014314, 15);  //59 [CDAB  6 15 59]
    step(_I(c, d, a), b, c, decode(data,13), 0x4e0811a1, 21); //60 [BCDA 13 21 60]
    step(_I(b, c, d), a, b, decode(data,4), 0xf7537e82, 6);   //61 [ABCD  4  6 61]
    step(_I(a, b, c), d, a, decode(data,11), 0xbd3af235, 10); //62 [DABC 11 10 62]
    step(_I(d, a, b), c, d, decode(data,2), 0x2ad7d2bb, 15);  //63 [CDAB  2 15 63]
    step(_I(c, d, a), b, c, decode(data,9), 0xeb86d391, 21);  //64 [BCDA  9 21 64]

    a += saved_a;
    b += saved_b;
    c += saved_c;
    d += saved_d;

    data += 64;
    this->_blocks -= 1;
    this->_bits += 512;
  } while (this->_blocks > 0);

  this->_a = a;
  this->_b = b;
  this->_c = c;
  this->_d = d;
  a = 0;
  b = 0;
  c = 0;
  d = 0;
  saved_a = 0;
  saved_b = 0;
  saved_c = 0;
  saved_d = 0;
  return data;
}

/* Transform remaining bits less than a full block with required padding and cummulates
  * the context variables. If the MD5 class member _blocks is set to a value greater than
  * 0, then transform() is called to process the full blocks. Set _blocks to zero when finishing
  * up a transform with a partial block */
void MD5::finalize(const char *data)
{
  // do we have one or more 64 byte blocks
  if (this->_blocks > 0)
  {
      data = transform(data);
  }

  // source bits to encode in transform
  size_t source_bits = this->_input_len << 3;

  // non-transformed bytes remaining
  size_t bytes = this->_input_len - (this->_bits >> 3);

  // Remaining bytes should be less than or equal to BUFFER_LEN if calling
  // function properly set MD5 context variables _blocks and _input_len.
  // If we are going to run into an overflow then null out context
  // variables and return leaving a null hash.
  if (bytes > BUFFER_LEN)
  {
    init();
    return;
  }

/* There are three remaining cases:
 * 1) transform ended on a block boundry (bytes == 0) -> append 512 bits
 * 2) transform ended between 448-512 bits -> append & transform then append to 512 bits.
 * 3) transform ended below or equal to 448 -> append to 448 bits
 */

  // Copy remaining bits to buffer, if necessary.
  if(data != this->_buffer)
  {
    memcpy(this->_buffer, data, bytes);
  }

  // case 2 add padding bits to 512, transform, and zero buffer
  if (bytes > SOURCE_SIZE_INDEX)
  {
    memcpy(this->_buffer + bytes, PADDING, BUFFER_LEN - bytes);
    this->_blocks = 1;
    transform(this->_buffer);
    memset(this->_buffer, '\0', BUFFER_LEN);
  }
  else
  {
  // add padding bits to 448
    memcpy(this->_buffer + bytes, PADDING, SOURCE_SIZE_INDEX - bytes);
  }

  // append length in bits (64 bit representation low order byte first) and transform
  this->_buffer[56] = source_bits & 0xff;
  this->_buffer[57] = (source_bits >> 8) & 0xff;
  this->_buffer[58] = (source_bits >> 16) & 0xff;
  this->_buffer[59] = (source_bits >> 24) & 0xff;
  this->_buffer[60] = (source_bits >> 32) & 0xff;
  this->_buffer[61] = (source_bits >> 40) & 0xff;
  this->_buffer[62] = (source_bits >> 48) & 0xff;
  this->_buffer[63] = (source_bits >> 56) & 0xff;
  this->_blocks = 1;
  transform(this->_buffer);
}

void MD5::encode(unsigned char *hash)
{
  hash[0] = this->_a & 0xff;
  hash[1] = (this->_a >> 8) & 0xff;
  hash[2] = (this->_a >> 16) & 0xff;
  hash[3] = (this->_a >> 24) & 0xff;
  hash[4] = this->_b & 0xff;
  hash[5] = (this->_b >> 8) & 0xff;
  hash[6] = (this->_b >> 16) & 0xff;
  hash[7] = (this->_b >> 24) & 0xff;
  hash[8] = this->_c & 0xff;
  hash[9] = (this->_c >> 8) & 0xff;
  hash[10] = (this->_c >> 16) & 0xff;
  hash[11] = (this->_c >> 24) & 0xff;
  hash[12] = this->_d  & 0xff;
  hash[13] = (this->_d >> 8) & 0xff;
  hash[14] = (this->_d >> 16) & 0xff;
  hash[15] = (this->_d >> 24) & 0xff;
  hash[16] = '\0';
}

void MD5::make_hash(const char *data, size_t len, unsigned char *hash)
{
  MD5 context(len);
  context.finalize(data);
  context.encode(hash);
}

void MD5::make_hash(const void *data, size_t len, unsigned char *hash)
{
  MD5 context(len);
  context.finalize( (const char *) data );
  context.encode(hash);
}

void MD5::make_hash(FILE *f, unsigned char *hash)
{
  MD5 context;
  size_t bytes_read = 0;
  char *ptr = NULL;

  // read from file until end of file is reached
  while ((f != NULL) && !feof(f))
  {
    memset(context._buffer, '\0', MD5::BUFFER_LEN);
    bytes_read = 0;
    ptr = context._buffer;

    // fill buffer & transform
    while ((bytes_read < MD5::BUFFER_LEN) && !feof(f))
    {
      bytes_read += fread(ptr, 1, MD5::BUFFER_LEN - bytes_read, f);
      if (ferror(f))
      {
        perror("Failed to read from file.\n");
        context.init();
        return;
      }
      ptr = context._buffer + bytes_read;
      context._input_len += bytes_read;
   }
   if (bytes_read == MD5::BUFFER_LEN)
   {
     context._blocks = 1;
     context.transform(context._buffer);
   }
  }
  context._blocks = 0;
  context.finalize(context._buffer);
  context.encode(hash);
}





