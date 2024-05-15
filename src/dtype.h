#ifndef __FINWO_DTYPE_H__
#define __FINWO_DTYPE_H__

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include "tidwall/buf.h"

enum dtype_typ {
  DTYPE_NULL,
  DTYPE_STRING,
  DTYPE_INTEGER,
  DTYPE_DOUBLE,
  DTYPE_BUFFER,
  DTYPE_UNKNOWN,
};

struct dtype_value {
  union {
    void       *val_null;
    char       *val_string;
    int         val_integer;
    double      val_double;
    struct buf *val_buffer;
    void       *val_unknown;
  };
  enum dtype_typ type;
};

#define dtype_type(x) ((x)->type)

struct dtype_value * dtype_copy(const struct dtype_value *subject) {
  struct dtype_value *output = (struct dtype_value *)calloc(1, sizeof(struct dtype_value));
  memcpy(output, subject, sizeof(struct dtype_value));
  if (dtype_type(output) == DTYPE_STRING) {
    output->val_string = strdup(output->val_string);
  } else if (dtype_type(subject) == DTYPE_BUFFER) {
    output->val_buffer = malloc(sizeof(struct buf));
    memcpy(output->val_buffer, subject->val_buffer, sizeof(struct buf));
    output->val_buffer->data = malloc(output->val_buffer->cap);
    memcpy(output->val_buffer->data, subject->val_buffer->data, subject->val_buffer->len);
  }
  return output;
}

#define dtype_init_null()     dtype_copy(&((const struct dtype_value){ .type = DTYPE_NULL                      }))
#define dtype_init_string(x)  dtype_copy(&((const struct dtype_value){ .type = DTYPE_STRING , .val_string  = x }))
#define dtype_init_integer(x) dtype_copy(&((const struct dtype_value){ .type = DTYPE_INTEGER, .val_integer = x }))
#define dtype_init_double(x)  dtype_copy(&((const struct dtype_value){ .type = DTYPE_DOUBLE , .val_double  = x }))
#define dtype_init_buffer(x)  dtype_copy(&((const struct dtype_value){ .type = DTYPE_BUFFER , .val_buffer  = x }))
#define dtype_init_unknown(x) dtype_copy(&((const struct dtype_value){ .type = DTYPE_UNKNOWN, .val_unknown = x }))

#define dtype_buf_append(x,data,len) buf_append(x->val_buffer, data, len)
#define dtype_buf_append_byte(x,c)   buf_append_byte(x->val_buffer, c)
#define dtype_buf_clear(x)           buf_clear(x->val_buffer)

#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
#define dtype_init(x) _Generic((x), \
    char *      : dtype_init_string, \
    int         : dtype_init_integer, \
    double      : dtype_init_double, \
    void *      : dtype_init_unknown, \
    struct buf *: dtype_init_buffer \
  )(x)
#endif

void dtype_free(struct dtype_value *subject) {
  if (dtype_type(subject) == DTYPE_STRING) {
    free(subject->val_string);
  } else if (dtype_type(subject) == DTYPE_BUFFER) {
    buf_clear((struct buf *)subject);
  }
  free(subject);
}

#endif // __FINWO_DTYPE_H__
