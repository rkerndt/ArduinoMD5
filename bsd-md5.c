/*
 * bsd-md5.c
 *
 * Copyright 2020 Rickie S. Kerndt <kerndtr@captain>
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

#include <stdio.h>
#include <bsd/md5.h>
#include <string.h>
#include <time.h>

// function declarations
void MD5String(const char *);
void MDTimeTrial(void);
void MDTestSuite(void);

// constants
static const int TEST_BLOCK_COUNT = 1000;
static const int TEST_BLOCK_LEN = 1000;

static const char HELP[] = "\
Arguments (may be any combination):\n\
\t-sstring - digests string\n\
\t-t        - runs time trial\n\
\t-x        - runs test script\n\
\t-h        - print this message\n\
\tfilename  - digests file\n\
";

/* Uses the bsd md5 library to generate md5 digests */
int main(int argc, char **argv)
{
  if (argc > 1)
  {
    for(int i = 1; i < argc; i++)
    {
      if ((argv[i][0] == '-') && (argv[i][1] == 's'))
      {
        MD5String(argv[i] + 2);
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
        printf("%s", HELP);
      }
      else
      {
        char digest[MD5_DIGEST_STRING_LENGTH];
        memset(digest, '\0', MD5_DIGEST_STRING_LENGTH);
        MD5File(argv[i], digest);
        printf("MD5 (%s) = %s\n", argv[i], digest);
      }
    }
  }
  else
  {
    printf("%s", HELP);
  }
  return 0;
}

/* digests a c_tring */
void MD5String(const char *str)
{
  MD5_CTX context;
  char digest[MD5_DIGEST_STRING_LENGTH];
  const unsigned char *p = (const unsigned char *) str;

  // initialize
  MD5Init(&context);
  memset(digest, '\0', MD5_DIGEST_STRING_LENGTH);

  MD5Data(p, strlen(str), digest);
  printf("MD5 (%s) = %s\n", str, digest);
}

/* Digests a reference suite of strings and prints the results */
void MDTestSuite(void)
{
  printf("MD5 test suite:\n");
  MD5String("");
  MD5String("a");
  MD5String("abc");
  MD5String("message digest");
  MD5String("abcdefghijklmnopqrstuvwxyz");
  MD5String("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  MD5String("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
}

/* Measures the time to digest 1000 1000-byte blocks */
void MDTimeTrial(void)
{
  MD5_CTX context;
  char digest[MD5_DIGEST_STRING_LENGTH];
  struct timespec end_time;
  struct timespec start_time;
  unsigned char block[TEST_BLOCK_LEN];
  long delta = 0;

  // initialize
  MD5Init(&context);
  memset(digest, '\0', MD5_DIGEST_STRING_LENGTH);
  memset(&end_time, '\0', sizeof(struct timespec));
  memset(&start_time, '\0', sizeof(struct timespec));
  memset(block, '\0', sizeof(block));
  for (int i=0; i < TEST_BLOCK_LEN; i++)
  {
    block[i] = (char) (i & 0xff);
  }

  printf("MD5 time trial, Digesting %d %d-byte blocks ...", TEST_BLOCK_COUNT, TEST_BLOCK_LEN);

  clock_gettime(CLOCK_REALTIME, &start_time);

  // digest blocks
  for(int i = 0; i < TEST_BLOCK_COUNT; i++)
  {
    MD5Data(block, TEST_BLOCK_LEN, digest);
  }

  // stop time
  clock_gettime(CLOCK_REALTIME, &end_time);

  // calculate microseconds between times
  if (start_time.tv_sec != end_time.tv_sec)
  {
    delta = ((end_time.tv_sec - start_time.tv_sec) * 1000000) - start_time.tv_nsec + end_time.tv_nsec;
  }
  else
  {
    delta += end_time.tv_nsec - start_time.tv_nsec;
  }
  delta = delta / 1000; // express time in microseconds

  printf("done\n");
  printf("Digest = %s\n", digest);
  printf("Time = %ld usecs\n", delta);

  if (delta > 0)
  {
    printf("Speed = %ld bytes/usec\n", (TEST_BLOCK_COUNT * TEST_BLOCK_LEN) / delta);
  }
}


