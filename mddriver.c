/* MDDRIVER.C - test driver for MD5 */

/* Copyright (C) 1990-2, RSA Data Security, Inc. Created 1990. All
rights reserved.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
*/

/* Implementation by Rickie Kerndt <kerndtr@kerndt.com.
 * Updated to c99 with mostly just formatting changes. Uses clock_gettime
 * for nanosecond resolution in time trial.
 */

#define _POSIX_C_SOURCE  200112L // gets us struct timespec & clock_gettime()

#
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "openwell-md5.h"

/* typedefs from global.h */
typedef unsigned char *POINTER;
typedef unsigned short int UINT2;
typedef unsigned long int UINT4;

/* Length of test block, number of test blocks.*/
#define TEST_BLOCK_LEN 1000
#define TEST_BLOCK_COUNT 1000

static void MDString (char *);
static void MDTimeTrial(void);
static void MDTestSuite(void);
static void MDFile(char *);
static void MDFilter(void);
static void MDPrint(unsigned char [16]);

/* Main driver. */
static const char HELP[] = "\
Arguments (may be any combination):\n\
\t-sstring - digests string\n\
\t-t       - runs time trial\n\
\t-x       - runs test script\n\
\tfilename - digests file\n\
\t(none)   - digests standard input\n";

int main (int argc, char **argv)
{
  if (argc > 1)
  {
    for (int i = 1; i < argc; i++)
    {
      if (argv[i][0] == '-' && argv[i][1] == 's')
      {
        MDString (argv[i] + 2);
      }
      else if (strcmp (argv[i], "-t") == 0)
      {
        MDTimeTrial ();
      }
      else if (strcmp (argv[i], "-x") == 0)
      {
        MDTestSuite ();
      }
      else if (strcmp (argv[i], "-h") == 0)
      {
        printf("%s\n", HELP);
      }
      else
      {
        MDFile (argv[i]);
      }
    }
  }
  else
  {
    MDFilter ();
  }
  return (0);
}

/* Digests a string and prints the result. */
static void MDString (string)
char *string;
{
  MD5_CTX context;
  unsigned char digest[16];
  unsigned int len = strlen (string);

  MD5_Init (&context);
  MD5_Update (&context, string, len);
  MD5_Final (digest, &context);

  printf ("MD5 (\"%s\") = ", string);
  MDPrint (digest);
  printf ("\n");
}

/* Measures the time to digest TEST_BLOCK_COUNT TEST_BLOCK_LEN-byte
  blocks. */
static void MDTimeTrial ()
{
  MD5_CTX context;
  struct timespec endTime, startTime;
  unsigned char block[TEST_BLOCK_LEN], digest[16];
  unsigned int i;

  printf("MD5 time trial. Digesting %d %d-byte blocks ...", \
         TEST_BLOCK_LEN, TEST_BLOCK_COUNT);

  /* Initialize block */
  for (i = 0; i < TEST_BLOCK_LEN; i++)
  {
    block[i] = (unsigned char)(i & 0xff);
  }
  memset(&endTime, '\0', sizeof(struct timespec));
  memset(&startTime, '\0', sizeof(struct timespec));

  /* Start timer */
  clock_gettime(CLOCK_REALTIME, &startTime);

  /* Digest blocks */
  for (i = 0; i < TEST_BLOCK_COUNT; i++)
  {
    MD5_Init(&context);
    MD5_Update (&context, block, TEST_BLOCK_LEN);
    MD5_Final (digest, &context);
  }

  /* Stop timer */
  clock_gettime(CLOCK_REALTIME, &endTime);

  // calculate microseconds between times
  long delta = 0;
  if (startTime.tv_sec != endTime.tv_sec)
  {
    delta = ((endTime.tv_sec - startTime.tv_sec) * 1000000) - startTime.tv_nsec + endTime.tv_nsec;
  }
  else
  {
    delta += endTime.tv_nsec - startTime.tv_nsec;
  }
  delta = delta / 1000; // express time in microseconds


  printf (" done\n");
  printf ("Digest = ");
  MDPrint (digest);
  printf ("\nTime = %ld usecs\n", delta);
  if (delta > 0)
  {
    printf("Speed = %ld bytes/usec\n", (long)TEST_BLOCK_LEN * (long)TEST_BLOCK_COUNT/delta);
  }
}

/* Digests a reference suite of strings and prints the results. */
static void MDTestSuite ()
{
  printf ("MD5 test suite:\n");

  MDString ("");
  MDString ("a");
  MDString ("abc");
  MDString ("message digest");
  MDString ("abcdefghijklmnopqrstuvwxyz");
  MDString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  MDString("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
}

/* Digests a file and prints the result. */
static void MDFile (char *filename)
{
  FILE *file;
  MD5_CTX context;
  int len;
  unsigned char buffer[1024], digest[16];

  if ((file = fopen (filename, "rb")) == NULL)
  {
    printf ("%s can't be opened\n", filename);
  }
  else
  {
    MD5_Init (&context);
    while ((len = fread (buffer, 1, 1024, file)))
    {
      MD5_Update (&context, buffer, len);
    }
    MD5_Final (digest, &context);
    fclose (file);
    printf ("MD5 (%s) = ", filename);
    MDPrint (digest);
    printf ("\n");
  }
}

/* Digests the standard input and prints the result.
 */
static void MDFilter ()
{
  MD5_CTX context;
  int len;
  unsigned char buffer[16], digest[16];

  MD5_Init (&context);
  while ((len = fread (buffer, 1, 16, stdin)))
  {
    MD5_Update (&context, buffer, len);
  }
  MD5_Final (digest, &context);
  MDPrint (digest);
  printf ("\n");
}

/* Prints a message digest in hexadecimal.
 */
static void MDPrint (unsigned char digest[16])
{
  unsigned int i;

  for (i = 0; i < 16; i++)
  {
    printf ("%02x", digest[i]);
  }
}

