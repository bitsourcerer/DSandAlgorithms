/* Foreach loop in GNU C, released in the public domain by Joe Davis.
 * Credit would be nice ;)
 */

#ifndef FOREACH_H_
#define FOREACH_H_

#include <cstdbool>
#include <cstring>

/* Utility functions */

#define CONCAT(a,b) CONCAT_(a,b)
#define CONCAT_(a,b) a ## b

/* Hygiene in macros */
#define GENSYM(name) \
  CONCAT(CONCAT(CONCAT(_anon_variable_, name),__LINE__),__COUNTER__)

/* Helper functions for foreach */

/* FOREACH_COMP
   Checks if the value of INDEX, is greater than the length of ARRAY.
 */

#define FOREACH_COMP(INDEX, ARRAY, ARRAY_TYPE, SIZE) \
  FOREACH_COMP_ (INDEX, ARRAY, ARRAY_TYPE, SIZE, GENSYM (ret))
#define FOREACH_COMP_(INDEX, ARRAY, ARRAY_TYPE, SIZE, ret) \
  __extension__ \
  ({ \
    bool ret = 0; \
    if (__builtin_types_compatible_p (const char*, ARRAY_TYPE)) \
      ret = INDEX < strlen ((const char*)ARRAY); \
    else \
      ret = INDEX < SIZE; \
    ret; \
  })

/* FOREACH_ELEM
   Return a pointer to the element at INDEX in ARRAY.
 */

#define FOREACH_ELEM(INDEX, ARRAY, TYPE) \
  FOREACH_ELEM_ (INDEX, ARRAY, TYPE, GENSYM (tmp_array))

#define FOREACH_ELEM_(INDEX, ARRAY, TYPE, tmp_array) \
  __extension__ \
  ({ \
    TYPE *tmp_array_ = ARRAY; \
    &tmp_array_[INDEX]; \
  })

/* Foreach loop itself */

/* FOREACH
   A for-each loop implemented in GNU C.
   The variable b at the end of FOREACH is there to make sure that the final
   for loop eventually terminates.
 */

#define FOREACH(VAR, ARRAY) \
  FOREACH_ (VAR, ARRAY, GENSYM (array), GENSYM (i), GENSYM (b))

#define FOREACH_(VAR, ARRAY, array, i, b) \
for (void *array = (void*)(ARRAY); array; array = 0) \
for (size_t i = 0; array && FOREACH_COMP (i, array, \
                              __typeof__ (ARRAY), \
                              sizeof (ARRAY) / sizeof ((ARRAY)[0])); \
                              i++) \
for (bool b = 1; b; (b) ? array = 0 : 0, b = 0) \
for (VAR = FOREACH_ELEM (i, array, __typeof__ ((ARRAY)[0])); b; b = 0)

#endif /* FOREACH_H_ */