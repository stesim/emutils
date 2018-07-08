#include <emscripten/emscripten.h>

#include <stdint.h>

#define FOO_EXPORT EMSCRIPTEN_KEEPALIVE

#ifdef __cplusplus
extern "C" {
#endif

typedef void* foo_Foo;
typedef void* foo_Bar;

typedef enum {
  FOO_ERROR_NONE,
  FOO_ERROR_SOME
} foo_Error;

typedef enum {
  FOO_FOO_NAME,
  FOO_FOO_TYPE
} foo_FooInfo;

typedef enum {
  FOO_FOO_TYPE_PRIMARY,
  FOO_FOO_TYPE_SECONDARY
} foo_FooType;

FOO_EXPORT const char* foo_GetVersion();

FOO_EXPORT foo_Error foo_GetFoos(uint32_t num_entries, foo_Foo* foos, uint32_t* num_foos);

FOO_EXPORT foo_Error foo_GetFooInfoString(foo_Foo foo, foo_FooInfo property,
                                          uint32_t property_value_size, void* property_value,
                                          uint32_t* property_value_size_out);

FOO_EXPORT foo_Bar foo_CreateBar(foo_Foo foo, foo_Error* error);

FOO_EXPORT foo_Error foo_ReleaseBar(foo_Bar bar);

FOO_EXPORT const char* foo_GetBarName(foo_Bar bar, foo_Error* error);

FOO_EXPORT foo_Error foo_SetBarName(foo_Bar bar, const char* name);

#ifdef __cplusplus
}
#endif
