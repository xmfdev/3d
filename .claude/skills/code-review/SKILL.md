---
name: c-review
description: Reviews C code for memory safety issues and common pitfalls
---

# C Code Review Skill

You are reviewing C code. Be thorough and pragmatic. Focus on real bugs, not style.

## Memory Safety — Check Every Allocation

- Every `malloc`/`calloc`/`realloc` must have a corresponding `free`
- Trace returned pointers across function boundaries - leaks often happen at the call site, not the allocation site
- Check for `free` inside loops that may free the same pointer twice
- After `free`, the pointer is dangling - flag any use of it
- Check `realloc` return value - on failure it returns NULL but does NOT free the original pointer, causing a leak

## Null Pointer Dereference

- Every pointer returned from `malloc`, `realloc`, `fopen`, or any function that can fail must be checked before use
- Watch for null propagation - a null returned from one function passed into another
- Struct pointer members may be uninitialized - check before dereferencing

## Buffer Safety

- Flag any `strcpy`, `strcat`, `sprintf`, `gets` - prefer `strncpy`, `strncat`, `snprintf`
- Array indexing with user-controlled values must be bounds checked
- `sizeof` on a pointer is not the size of the array - flag this pattern

## Integer Issues

- Signed/unsigned comparison mismatches
- Narrowing conversions - where data is assigned to a smaller type
- Integer overflow before allocation: `malloc(a * b)` where a and b are user-controlled
- Off-by-one errors in loop bounds and buffer sizes

## Other Pitfalls

- Uninitialized variables, especially on stack-allocated structs
- Missing `break` in switch statements
- Modifying a string literal (undefined behavior)
- Returning a pointer to a stack-allocated variable

## How to Report

For each issue found:
1. State the exact line or function
2. Name the class of bug (e.g. "use-after-free", "null dereference")
3. Explain why it's a bug
4. Suggest a fix

If no issues are found, say so clearly. Do not invent issues.
