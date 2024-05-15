#include "finwo/assert.h"

#include "src/dtype.h"

void test_init() {
  struct dtype_value *subject;
  char *str = "Hello World";
  void *dat = strdup(str);
  size_t datl = strlen(str)+1;

  subject = dtype_init_null();
  ASSERT("init_null returns a valid pointer", subject != NULL                  );
  ASSERT("init_null returns a null value"   , dtype_type(subject) == DTYPE_NULL);
  dtype_free(subject);

  subject = dtype_init_string(str);
  ASSERT("init_string returns a valid pointer"  , subject != NULL                    );
  ASSERT("init_string returns the correct type" , dtype_type(subject) == DTYPE_STRING);
  ASSERT("init_string returns a cloned value"   , subject->val_string != dat         );
  ASSERT("init_string returns the correct value", !strcmp(str, subject->val_string)  );
  dtype_free(subject);

  subject = dtype_init_integer(12);
  ASSERT("init_integer returns a valid pointer"  , subject != NULL                     );
  ASSERT("init_integer returns the correct type" , dtype_type(subject) == DTYPE_INTEGER);
  ASSERT("init_integer returns the correct value", subject->val_integer == 12          );
  dtype_free(subject);

  subject = dtype_init_double(12);
  ASSERT("init_double returns a valid pointer"  , subject != NULL                    );
  ASSERT("init_double returns the correct type" , dtype_type(subject) == DTYPE_DOUBLE);
  ASSERT("init_double returns the correct value", subject->val_double == 12          );
  dtype_free(subject);

  subject = dtype_init_buffer((&((struct buf){ .cap = datl, .len = datl, .data = dat })) );
  ASSERT("init_buffer returns a valid pointer"  , subject != NULL                        );
  ASSERT("init_buffer returns the correct type" , dtype_type(subject) == DTYPE_BUFFER    );
  ASSERT("init_buffer returns a cloned value"   , subject->val_buffer->data != dat       );
  ASSERT("init_buffer returns the correct value", !strcmp(str, subject->val_buffer->data));
  dtype_free(subject);

  subject = dtype_init_unknown(dat);
  ASSERT("init_unknown returns a valid pointer"  , subject != NULL                     );
  ASSERT("init_unknown returns the correct type" , dtype_type(subject) == DTYPE_UNKNOWN);
  ASSERT("init_unknown returns a copied value"   , subject->val_unknown == dat         );
  dtype_free(subject);

  ASSERT("unknown data is untouched after free", !strcmp(dat, str));
}

int main() {
  RUN(test_init);
  return TEST_REPORT();
}
