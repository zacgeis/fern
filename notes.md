# Notes

## General

- Add functions for pretty printing all steps - tokens, ast, output, etc.
- Tracer bullet style. Start with the most minimal int only pass from input to
assembly.
  - Only ints, single operator, no variables, etc.
- Only support a single file to start.
- Keep source file to a single file too.
- Start with only supporting ints and go from there.
- output assembly file.
- simple control flow. if and loop.
- always have a main entry point
  - `func main`
  - start without passing in any args.
- Single file c and only support single file Fern. Toy language. No
optimization. Simple syntax.
- Insipired by tcc and quickjs
- Similar syntax to typescript.
- Generics come later.
- All values have default init
- No const
- Very simple
- Direct compile to assembly
- No dependencies
- Slices for viewing memory
- Region allocators for handling memory
- Only templates, no metaprogramming
- C style *& for pointers.

## Basic Example

```
func add(a: int, b: int): int {
  return a + b;
}
```

```
func max(a: int, b: int) {
  if (a > b) {
    return a;
  }
  return b;
}
```

## generics

```
func add<T>(a: T, b: T): T {
  return a + b;
}
```

## types

```
int
float
char
arrays?
pointer types?
```
