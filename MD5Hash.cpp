/*
 * MD5Hash.cpp
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

#include "MD5Hash.h"

MD5Hash::MD5Hash(void)
{
  c_string = NULL;
  memset(this->hash, '\0', MD5::HASH_LEN + 1);
}

MD5Hash::MD5Hash(const MD5Hash &obj)
{
  for(int i = 0; i < MD5::HASH_LEN; i++)
  {
    this->hash[i] = obj.hash[i];
  }
  if (obj.c_string != NULL)
  {
    this->c_str();
  }
}

MD5Hash::MD5Hash(const unsigned char *hash)
{
  if (hash != NULL)
  {
    for(int i = 0; i < MD5::HASH_LEN; i++)
    {
      this->hash[i] = hash[i];
    }
  }
}

MD5Hash::~MD5Hash(void)
{
  memset(this->hash, '\0', MD5::HASH_LEN + 1);
  if (this->c_string != NULL)
  {
    memset(this->c_string, '\0', MD5::DIGEST_LEN + 1);
    free(this->c_string);
  }
}

MD5Hash& MD5Hash::operator=(const MD5Hash &rhs)
{
  if (this != &rhs)
  {
    for(int i = 0; i < MD5::HASH_LEN; i++)
    {
      this->hash[i] = rhs.hash[i];
    }
    if (rhs.c_string != NULL)
    {
      this->c_str();
    }
  }
  return *this;
}

bool MD5Hash::operator==(const MD5Hash &rhs)
{
  bool result = true;
  if (this != &rhs)
  {
    for(int i = 0; i < MD5::HASH_LEN; i++)
    {
      if (rhs.hash[i] != this->hash[i])
      {
        result = false;
        break;
      }
    }
  }
  return result;
}

bool MD5Hash::operator!=(const MD5Hash &rhs)
{
    return !operator==(rhs);
}

char *MD5Hash::c_str(void)
{
  this->c_string = (char *) calloc(MD5::DIGEST_LEN + 1, sizeof(char));
  MD5::make_digest(this->hash, this->c_string);
  return this->c_string;
}

string MD5Hash::to_string(void)
{
  if (this->c_string == NULL)
  {
    c_str();
  }
  return string(this->c_string);
}

MD5Hash MD5Hash::make_MD5Hash(const char *data, size_t len)
{
  MD5Hash obj;
  MD5::make_hash(data, len, obj.hash);
  return obj;
}

MD5Hash MD5Hash::make_MD5Hash(const void *data, size_t len)
{
  MD5Hash obj;
  MD5::make_hash(data, len, obj.hash);
  return obj;
}

MD5Hash MD5Hash::make_MD5Hash(FILE *f)
{
  MD5Hash obj;
  MD5::make_hash(f, obj.hash);
  return obj;
}

MD5Hash MD5Hash::make_MD5Hash(const string &data)
{
  MD5Hash obj;
  MD5::make_hash(data, obj.hash);
  return obj;
}
