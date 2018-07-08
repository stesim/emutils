#include "foo.h"

#include <cstring>
#include <iostream>

namespace {

class Foo {
 public:
  Foo(const char* name, foo_FooType type) : name_{name}, type_{type} {}

  const char* GetName() const {
    return name_;
  }

  foo_FooType GetType() const {
    return type_;
  }

 private:
  const char* name_;
  foo_FooType type_;
};

class Bar {
 public:
  Bar(Foo& foo) : name_{nullptr}, foo_{&foo} {}

  ~Bar() {
    if(name_ != nullptr) {
      delete[] name_;
    }
  }

  const char* GetName() const {
    return name_;
  }

  void SetName(const char* name) {
    if(name_ != nullptr) {
      delete[] name_;
      name_ = nullptr;
    }

    size_t length = strlen(name);
    name_ = new char[length + 1];
    name_[length] = '\0';

    memcpy(name_, name, length);
  }

  Foo& GetFoo() const {
    return *foo_;
  }

 private:
  Foo* foo_;
  char* name_;
};

}

const char* foo_GetVersion() {
  return "0.0.1";
}

foo_Error foo_GetFoos(uint32_t num_entries, foo_Foo* foos, uint32_t* num_foos) {
  static Foo kFoo{"MyFoo", FOO_FOO_TYPE_PRIMARY};

  foo_Error error{FOO_ERROR_NONE};
  if ((foos == nullptr && num_foos == nullptr) ||
      (foos != nullptr && num_entries == 0)) {
    error = FOO_ERROR_SOME;
  } else {
    if (foos != nullptr) {
      *foos = &kFoo;
    }

    if (num_foos != nullptr) {
      *num_foos = 1;
    }
  }

  return error;
}

foo_Error foo_GetFooInfoString(foo_Foo foo, foo_FooInfo property, uint32_t property_value_size,
                               void* property_value, uint32_t* property_value_size_out) {
  foo_Error error{FOO_ERROR_NONE};

  if (property_value == nullptr && property_value_size_out) {
    error = FOO_ERROR_SOME;
  } else {
    if (property == FOO_FOO_NAME) {
      if (property_value != nullptr) {
        if (property_value_size >= sizeof(const char*)) {
          *reinterpret_cast<const char**>(property_value) = static_cast<Foo*>(foo)->GetName();
        } else {
          error = FOO_ERROR_SOME;
        }
      }

      if (property_value_size_out != nullptr) {
        *property_value_size_out = sizeof(const char*);
      }
    } else {
      error = FOO_ERROR_SOME;
    }
  }

  return error;
}

foo_Bar foo_CreateBar(foo_Foo foo, foo_Error* error_out) {
  foo_Error error{FOO_ERROR_NONE};
  foo_Bar result{nullptr};

  if (foo == nullptr) {
    error = FOO_ERROR_SOME;
  } else {
    result = new Bar(*static_cast<Foo*>(foo));
  }

  if (error_out != nullptr) {
    *error_out = error;
  }

  return result;
}

foo_Error foo_ReleaseBar(foo_Bar bar) {
  foo_Error error{FOO_ERROR_NONE};

  if (bar == nullptr) {
    error = FOO_ERROR_SOME;
  } else {
    delete static_cast<Bar*>(bar);
  }

  return error;
}

const char* foo_GetBarName(foo_Bar bar, foo_Error* error_out) {
  foo_Error error{FOO_ERROR_NONE};
  const char* result{nullptr};

  if (bar == nullptr) {
    error = FOO_ERROR_SOME;
  } else {
    result = static_cast<Bar*>(bar)->GetName();
  }

  if(error_out != nullptr) {
    *error_out = error;
  }

  return result;
}

foo_Error foo_SetBarName(foo_Bar bar, const char* name) {
  foo_Error error{FOO_ERROR_NONE};

  if (bar == nullptr) {
    error = FOO_ERROR_SOME;
  } else {
    static_cast<Bar*>(bar)->SetName(name);
  }

  return error;
}
