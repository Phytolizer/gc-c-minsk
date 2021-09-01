#pragma once

#define DECLARE_NULLABLE(Name, T) \
  enum Name##Discriminator{ \
      Name##_SOMETHING, \
      Name##_NOTHING, \
  }; \
  struct Name \
  { \
    enum Name##Discriminator discriminator; \
    T value; \
  }

#define SOMETHING(Name, Value) \
  ((struct Name){ \
      .discriminator = Name##_SOMETHING, \
      .value = (Value), \
  })

#define NOTHING(Name) \
  ((struct Name){ \
      .discriminator = Name##_NOTHING, \
  })
