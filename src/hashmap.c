// robust, reliable hashmapping written
// in C.
// collision detection method:
// Chain Hashing
// or in essence, chaining multiple
// instances of the same key together,
// with different/same values

#include <hashmap.h>

#include <stdarg.h>
#include <string.h>

void
__panic (const char *fmt, ...)
{
  expect (fmt);

  va_list vp;
  va_start (vp, fmt);

  fprintf (stderr, "\x1B[31mpanic\x1B[90m: %s(%d): \x1B[0m\x1b[1m", __FILE__,
           __LINE__);
  vfprintf (stderr, fmt, vp);
  fprintf (stderr, "\x1B[0m");
  fprintf (stderr, "\n");
  abort ();
}

unsigned int
hash (int m, const char *k)
{
  expect (k);
  unsigned int r = SEED; // the initial value for the hash

  while (*k != 0)
    {
      r = *k++ * 33
          + r; // kill two birds with one stone, but not the program lol
    }

  return r % m;
}

char *
strdup_p (const char *ref)
{
  expect (ref);

  int length = strlen (ref) + 1;

  char *newstr = calloc (length, sizeof (char));
  expect (newstr);

  memcpy (newstr, ref, length);
  expect (newstr);

  return newstr;
}

Bucket *
buck_new ()
{
  Bucket *bucket = malloc (sizeof (Bucket));
  expect (bucket);

  bucket->head = NULL;

  return bucket;
}

HashMap *
hm_new ()
{
  HashMap *hsmp = malloc (sizeof (HashMap));
  expect (hsmp);

  hsmp->buckets = calloc (INITIAL_SIZE, sizeof (Bucket *));
  expect (hsmp->buckets);

  hsmp->size = 0;
  hsmp->capacity = INITIAL_SIZE;

  return hsmp;
}

HashLink *
hl_new ()
{
  HashLink *link = malloc (sizeof (HashLink));
  expect (link);

  link->next = NULL;
  link->key = NULL;

  return link;
}

void
hm_addbucket (HashMap *map, char *key, T value)
{
  // TODO: collisions
  expect (map);
  expect (key);
  expect (value); // NOTE: remove this if he gets in the way of things, keep
                  // track of this
                  // little guy

  if (map->size + 1 >= HARD_LIMIT || map->capacity >= HARD_LIMIT)
    __panic ("hard limit reached");

  if (map->size + 1 >= map->capacity) // reaching the end
    {
      // reallocate buckets to fit new buckets
      map->buckets
          = realloc (map->buckets, (map->capacity * 2) * sizeof (Bucket *));
      // double the cap
      map->capacity *= 2;
    }

  // hash index
  int hashi = hash (map->capacity, key);

  if (!map->buckets[hashi])
    {
      map->buckets[hashi] = buck_new ();

      HashLink *tmp = hl_new ();
      tmp->key = strdup_p (key);
      tmp->value = value; // TODO: manage certain cases

      map->buckets[hashi]->head = tmp;
    }
  else
    {
      if (map->buckets[hashi]->head == NULL)
        return;
      HashLink *tmp = map->buckets[hashi]->head;

      expect (tmp);

      while (tmp->next)
        {
          tmp = tmp->next;
        }

      tmp->next = hl_new ();
      tmp = tmp->next;
      tmp->key = strdup_p (key);
      tmp->value = value;
    }
}

Bucket *
hm_lookup (HashMap *map, const char *key)
{
  expect (map);
  expect (key);

  int hashindex = hash (map->capacity, key);

  Bucket *bucket = hm_findbuckhash (map, hashindex);

  if (!bucket)
    __panic ("key `%s' does not exist.", key);

  return bucket;
}

Bucket *
hm_findbuckhash (HashMap *map, int hashindx)
{
  expect (map);

  if (map->capacity <= hashindx)
    __panic ("invalid index `%d'\n", hashindx);

  if (!map->buckets[hashindx])
    return NULL;

  return map->buckets[hashindx];
}

int
hm_collisions (HashMap *map, const char *key)
{
  expect (map);
  expect (key);

  int iter = 0;

  Bucket *bucket = hm_lookup (map, key);
  expect (bucket);

  HashLink *tmp = bucket->head;

  while (tmp->next)
    {
      tmp = tmp->next;
      iter++;
    }

  return iter;
}

HashLink *
hashfirst (HashMap *map, const char *key)
{
  expect (map);
  expect (key);

  Bucket *ptr = hm_lookup (map, key);

  if (!ptr)
    __panic ("key not found: `%s' (%s)", key, __func__);

  return ptr->head;
}

void
buck_free (Bucket *bucket)
{
  expect (bucket);
  expect (bucket->head);

  HashLink *tmp = bucket->head;

  if (tmp)
    {
      while (tmp)
        {
          HashLink *next = tmp->next;
          free (tmp->key);
          free (tmp);
          tmp = next;
        }
    }

  free (bucket);
}

void
hm_free (HashMap *map)
{
  expect (map);
  expect (map->buckets);

  for (int i = 0; i < map->capacity; i++)
    {
      if (map->buckets[i])
        {
          buck_free (map->buckets[i]);
        }
    }

  map->capacity = 0;
  map->size = 0;

  free (map->buckets);
  free (map);

  map = NULL;
}

int
test_simple_collisions (void)
{
  HashMap *M = hm_new ();
  expect (M);

  hm_addbucket (M, "hello", 8);
  hm_addbucket (M, "hello", 9); // collides, but will not override anything

  testing_expect (hm_collisions (M, "hello") == 1);

  testing_expect (hashfirst (M, "hello")->value == 8);
  testing_expect (hashfirst (M, "hello")->next->value == 9);

  testing_expect (strncmp (hashfirst (M, "hello")->key, "hello", 5) == 0);
  testing_expect (strncmp (hashfirst (M, "hello")->next->key, "hello", 5)
                  == 0);

  hm_free (M);

  testing_passed ();

  return 0;
}

int
test_wrong_inputs (void)
{
  HashMap *M = hm_new ();
  expect (M);

  // testing bad inputs to see if they
  // error out.

  // hm_addbucket (NULL, NULL, -1); --- WORKS!
  // hm_collisions(NULL, NULL);     --- WORKS!
  // hm_findbuckhash(M, 512903);    --- WORKS!
  // hm_free(NULL);                 --- WORKS!
  // hm_lookup (M, NULL);           --- WORKS!

  hm_free (M);

  testing_passed ();

  return 0;
}

int
main (void)
{
  int n = 0;
  n += test_simple_collisions ();
  n += test_wrong_inputs ();

  return n;
}
