/*
 * MD5Hash.h
 *
 * Copyright 2020 Rickie Kerndt <kerndtr@kerndt.com>
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

using namespace std;

/* Container to store and operate on a MD5 hash. Uses class MD5 to
 * compute hash values and provides operator overload functions for
 * comparision and to generate human readable form */
class MD5Hash {

  unsigned char hash[MD5::HASH_LEN + 1];
  char *c_string;

public:

  MD5Hash(void);
  MD5Hash(const MD5Hash &hash);
  MD5Hash(const unsigned char *hash);
  ~MD5Hash(void);

  // operator overloads
  MD5Hash& operator=(const MD5Hash &hash);
  bool operator==(const MD5Hash &hash);
  bool operator!=(const MD5Hash &hash);

  /* Generates human readable hash */
  string to_string(void);
  char *c_str(void);

  /* MD5 hash functions generating MD5Hash objects */
  static MD5Hash make_MD5Hash(const char *data, size_t len);
  static MD5Hash make_MD5Hash(const void *data, size_t len);
  static MD5Hash make_MD5Hash(const string &data);
  static MD5Hash make_MD5Hash(FILE *f);

};
