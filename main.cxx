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

/* An implementation of mddriver.c from rfc1321 providing test
 * routines and time trial for MD5 transform.
 *
 * Copyright (C) 1990-2, RSA Data Security, Inc. Created 1990. All
 * rights reserved.
 *
 * RSA Data Security, Inc. makes no representations concerning either
 * the merchantability of this software or the suitability of this
 * software for any particular purpose. It is provided "as is"
 * without express or implied warranty of any kind.
 *
 * These notices must be retained in any copies of any part of this
 * documentation and/or software.
 */

#include <iostream>
#include <time.h>
#include <string.h>
#include "MD5.h"

// Function declarations
void MDString(const char *);
void MDTimeTrial(void);
void MDTestSuite(void);
void MDFile(const char *);
void MDFilter(FILE *);
void MDPrint(const char *);


static const char HELP[] = "\
Arguments (may be any combination):\n\
\t-sstring - digests string\n\
\t-t        - runs time trial\n\
\t-x        - runs test script\n\
\t-h        - print this message\n\
\tfilename  - digests file\n\
\t(none)    - digests standard input\n\
";

// Constants
static const int TEST_BLOCK_COUNT = 1000;
static const int TEST_BLOCK_LEN = 1000;
static const int OUTPUT_LEN = 1024; // limit memory usage for output buffers

// char buffer for formatting output
char *output = NULL;

int main(int argc, char **argv)
{
  output = (char*) calloc(OUTPUT_LEN, sizeof(char));

  if (output == NULL)
  {
    MDPrint("Failed to allocate memory for output buffer\n");
    return -1;
  }

  if (argc > 1)
  {
    for(int i = 1; i < argc; i++)
    {
      if ((argv[i][0] == '-') && (argv[i][1] == 's'))
      {
        MDString(argv[i] + 2);
      }
      else if (strcmp(argv[i], "-t") == 0 )
      {
        MDTimeTrial();
      }
      else if (strcmp(argv[i], "-x") == 0)
      {
        MDTestSuite();
      }
      else if (strcmp(argv[i], "-h") == 0)
      {
        MDPrint(HELP);
      }
      else
      {
        MDFile(argv[i]);
      }
    }
  }
  else
  {
    MDFilter(stdin);
  }
  if (output != NULL)
  {
    free(output);
  }
  return 0;
}

/* Digests a c_string and prints the result */
void MDString(const char *c_string)
{
  unsigned char hash[MD5::HASH_LEN + 1];
  memset(hash, '\0', sizeof(hash));
  char digest[MD5::DIGEST_LEN + 1];
  memset(digest, '\0', sizeof(digest));
  MD5::make_hash(c_string, strlen(c_string), hash);
  MD5::make_digest(hash, digest);
  snprintf(output, OUTPUT_LEN, "MD5 (\"%s\") = %s\n", c_string, digest);
  MDPrint(output);
}

/* Measures the time to digest 1000 1000-byte blocks */
void MDTimeTrial(void)
{
  static const int TEST_BLOCK_COUNT = 1000;
  static const int TEST_BLOCK_LEN = 1000;
  struct timespec end_time;
  memset(&end_time, '\0', sizeof(struct timespec));
  struct timespec start_time;
  memset(&start_time, '\0', sizeof(struct timespec));
  char block[TEST_BLOCK_LEN];
  memset(block, '\0', sizeof(block));
  unsigned char hash[MD5::HASH_LEN + 1];
  memset(hash, '\0', sizeof(hash));
  char digest[MD5::DIGEST_LEN + 1];
  memset(digest, '\0', sizeof(digest));

  snprintf(output, OUTPUT_LEN, "MD5 time trial, Digesting %d %d-byte blocks ...",
    TEST_BLOCK_COUNT, TEST_BLOCK_LEN);
  MDPrint(output);

  // initialize block
  for (int i=0; i < TEST_BLOCK_LEN; i++)
  {
    block[i] = (char) (i & 0xff);
  }

  clock_gettime(CLOCK_REALTIME, &start_time);

  // digest blocks
  for(int i = 0; i < TEST_BLOCK_COUNT; i++)
  {
    MD5::make_hash(block, TEST_BLOCK_LEN, hash);
  }

  // stop time
  clock_gettime(CLOCK_REALTIME, &end_time);

  // calculate microseconds between times
  long delta = 0;
  if (start_time.tv_sec != end_time.tv_sec)
  {
    delta = ((end_time.tv_sec - start_time.tv_sec) * 1000000) - start_time.tv_nsec + end_time.tv_nsec;
  }
  else
  {
    delta += end_time.tv_nsec - start_time.tv_nsec;
  }
  delta = delta / 1000; // express time in microseconds

  MDPrint("done\n");
  MD5::make_digest(hash, digest);
  snprintf(output, OUTPUT_LEN, "Digest = %s\n", digest);
  MDPrint(output);
  snprintf(output, OUTPUT_LEN, "Time = %ld usecs\n", delta);
  MDPrint(output);
  if (delta > 0)
  {
    snprintf(output, OUTPUT_LEN, "Speed = %ld bytes/usec\n", (TEST_BLOCK_COUNT * TEST_BLOCK_LEN) / delta);
    MDPrint(output);
  }
}

/* Digests a reference suite of strings and prints the results */
void MDTestSuite(void)
{
  MDPrint("MD5 test suite:\n");
  MDString("");
  MDString("a");
  MDString("acb");
  MDString("message digest");
  MDString("abcdefghijklmnopqrstuvwxyz");
  MDString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  MDString("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
}

/* Digests a file and prints the result */
void MDFile(const char *filename)
{
  FILE *f;

  if ((f = fopen(filename, "rb")) == NULL)
  {
    snprintf(output, OUTPUT_LEN, "Unable to open file %s\n", filename);
    MDPrint(output);
  }
  else
  {
    snprintf(output, OUTPUT_LEN, "MD5 (%s) = ", filename);
    MDPrint(output);
    MDFilter(f);
  }
}

/* Digests a FILE stream and prints the result */
void MDFilter(FILE *f)
{
  unsigned char hash[MD5::HASH_LEN + 1];
  memset(hash, '\0', sizeof(hash));
  char digest[MD5::DIGEST_LEN + 1];
  memset(digest, '\0', sizeof(digest));
  MD5::make_hash(f, hash);
  MD5::make_digest(hash, digest);
  snprintf(output, OUTPUT_LEN, "%s\n", digest);
  MDPrint(output);
}

/* Prints to standard output */
void MDPrint(const char *c_string)
{
  printf(c_string);
}

bool is_big_endian(void)
{
  union {
    uint32_t i;
    char c[4];
  } bint = {0x01020304};
  return bint.c[0] == '\001';
}
