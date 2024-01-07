// $Id: hashmap.h

#ifndef HASHMAPS_H
#define HASHMAPS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void __panic (const char *fmt, ...);

int TEST_COUNT = 0;

#define panic(n)                                                              \
  __panic ("`%s' is invalid - a potential OOM (%s())", #n, __func__)

#define ugly_print(cond)                                                      \
  __panic ("test `%s' \x1B[31mFAILED\x1B[0m\x1B[1m | condition: `%s'\n",      \
           __func__, #cond)
#define nice_print()                                                          \
  printf (                                                                    \
      "\x1B[1mtest `%s` \x1B[32mPASSED\x1B[0m -> testing conditions: %d\n",   \
      __func__, TEST_COUNT);                                                  \
  TEST_COUNT = 0

// primarily used on pointers, integers
// have no differences and don't check
// any boxes unless they're 0
#define expect(n) (((n == NULL) ? panic (n) : (void)0))
#define testing_expect(cond)                                                  \
  if (cond)                                                                   \
    {                                                                         \
      TEST_COUNT++;                                                           \
    }                                                                         \
  else                                                                        \
    {                                                                         \
      ugly_print (cond);                                                      \
    }
#define testing_passed() nice_print ()
// char* = int
#define T int
// #define FREE_VALUES // uncomment this
// if the type you are using is an
// allocated string, etc, or just
// something you need gone, and also for
// probabilistic hashmapping, to replace
// it's value.

// the seed for hashes - note that if
// you change this all of the hashes
// will change, but the logic stays the same.
#define SEED 5039

#define INITIAL_SIZE 5  // the initial size, this will double
#define HARD_LIMIT 1000 // this limit for how large the size can be

typedef struct HashPair
{
  char *key;
  T value;
} HashPair;

typedef struct HashLink
{
  char *key;
  T value;

  struct HashLink *next;
} HashLink;

// TODO: make this a linked list structure
typedef struct Bucket
{
  HashLink *head;
} Bucket;

typedef struct HashMap
{
  Bucket **buckets;
  int size;
  int capacity;
} HashMap;

// the size of the table, and the key
unsigned int hash (int m, const char *k);

// simple helper function that works
// like `strdup`, returns a new
// allocated string that the caller now
// owns.
char *strdup_p (const char *ref);

// Creates a new Bucket, albeit empty,
// still checks and allocates proper
// aligned memory for one.
Bucket *buck_new ();

// Allocates a new hashmap, verifying
// that the memory allocation process
// did not fail using `expect()`.
HashMap *hm_new ();

// Allocates a new hashlink
HashLink *hl_new ();

// adds a bucket to the HashMap's bucket
// list, verifies that the addition of
// the bucket does not overflow the
// list, going over the hard limit (if
// not -1) will stop all memory
// operations and panic out.
//
// YES, this function does handle
// hashing the key.
void hm_addbucket (HashMap *map, char *key, T value);

// return bucket (unhashed), this
// function is a direct wrapper around
// hm_findbuckhash()
Bucket *hm_lookup (HashMap *map, const char *key);

// return bucket (hashed), primarily for
// internal usage -- use hm_lookup for a
// general lookup function.
Bucket *hm_findbuckhash (HashMap *map, int hashindx);

// returns the amount of collisions a
// hash contains.
int hm_collisions (HashMap *map, const char *key);

// returns a pointer to the first link
// in a HashLink list.
HashLink *hashfirst (HashMap *map, const char *key);

// frees a bucket, this function is
// primarily good in case buckets change
// overtime.
void buck_free (Bucket *bucket);

// frees a hashmap, primarily calling
// bucket's free function, but can
// change overtime.
void hm_free (HashMap *map);

#endif /* HASHMAPS_H */
