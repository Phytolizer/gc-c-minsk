#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <IncludeMe.h>
#include <sds.h>

struct UnpackedList
{
  char** p_data;
  size_t sizeof_t;
  long* p_length;
  long* p_capacity;
};

#define DECLARE_NAMED_LIST(Name, T) \
  struct Name \
  { \
    T* data; \
    long length; \
    long capacity; \
  }

#define LIST(T) \
  struct \
  { \
    T* data; \
    long length; \
    long capacity; \
  }

#define LIST_UNPACK_(L) \
  ((struct UnpackedList){ \
      .p_data = (char**)&(L)->data, \
      .sizeof_t = sizeof(*(L)->data), \
      .p_length = &(L)->length, \
      .p_capacity = &(L)->capacity, \
  })

#define LIST_INIT(L) \
  do \
  { \
    (L)->data = NULL; \
    (L)->length = 0; \
    (L)->capacity = 0; \
  } \
  while (false)

#define LIST_DEINIT(L) \
  do \
  { \
    if ((L)->data) \
    { \
      mc_free((L)->data); \
    } \
  } \
  while (false)

#define LIST_INITIAL_CAPACITY 8

#define LIST_RESERVE(L, Capacity) \
  do \
  { \
    if ((Capacity) > (L)->capacity) \
    { \
      while ((Capacity) > (L)->capacity) \
      { \
        (L)->capacity = ((L)->capacity == 0) ? LIST_INITIAL_CAPACITY \
                                             : (L)->capacity * 2; \
      } \
      (L)->data = mc_realloc((L)->data, (L)->capacity * sizeof(*(L)->data)); \
    } \
  } \
  while (false)

#define LIST_RESIZE(L, Length) \
  do \
  { \
    LIST_RESERVE(L, Length); \
    (L)->length = (Length); \
  } \
  while (false)

#define LIST_PUSH(L, Element) \
  do \
  { \
    LIST_RESERVE(L, (L)->length + 1); \
    (L)->data[(L)->length++] = (Element); \
  } \
  while (false)

#define LIST_POP(L) ((L)->data[--(L)->length])

DECLARE_NAMED_LIST(StringList, sds);
