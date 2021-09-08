# Abu Debug

[![CI](https://github.com/abu-lib/debug/actions/workflows/ci.yml/badge.svg)](https://github.com/abu-lib/debug/actions/workflows/ci.yml)

This is part of the [Abu](http://github.com/abu-lib/abu) meta-project.

This contains utilities to ensure program correctness.

## Enabling Checks

If the `ABU_ENABLE_CHECKS` macro is set to `1`, then aggressive verification
will be enabled, at the cost of performance. If unspecified, it will be set if
`NDEBUG` is not set.

## Quick reference

### `abu::dbg::unreachable` 

Flags a portion of code as being unreachable. Most compilers will optimize 
accordingly.

Example:
```
void foo(int val) {
    switch(val) {
    case 0: break;
    case 5: break;
    // ...
    default:
        abu::bdg::unreachable();
    }
}
```

### `abu_assume`

In debug builds, the expression will be asserted to be true. In release builds, 
informs the compiler of the expectation.

The code in the assumption is still always executed regardless of the build type.

Example:
```
void to_float_from_string(std::variant<int, float, std::string>& val) {
    abu_assume(val.index() == 0);
    val = 12.3f;
}
```

### `abu_precondition`

Effectively identical to `abu_assume()`, but indicates that the the onus is on 
the caller, and should be documented.

Example:
```
// Calling foo() with a null pointer is undefined behavior
void foo(int * some_ptr) {
    abu_precondition(some_ptr);
}
```

### `abu_postcondition`

Effectively identical to `abu_assume()`, but indicates that callers are allowed 
to assume it to be true, and thus should be documented.

Example:
```
// Will always return values above 100.
int foo(int val) {
    int result = ...;


    abu_precondition(result > 100);
    return result;
}
```
