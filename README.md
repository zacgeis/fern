# Fern

Fern is a simple toy compiler I work on in my spare time.

## Notes

### TODO

- Add arena based allocators to simplify the memory management of the compiler.

### General

- Add functions for pretty printing all steps - tokens, ast, output, etc.
- Tracer bullet style. Start with the most minimal int only pass from input to
assembly.
  - Only ints, single operator, no variables, etc.
- Only support a single file to start.
- Start with only supporting ints and go from there.
- output assembly file.
- simple control flow. if and loop.
- always have a main entry point
  - `func main`
  - start without passing in any args.
- Insipired by tcc and quickjs
- Toy language / compiler.
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

### Thoughts

- Always think of the smallest, simplest example, and build from there. Start
Recursive descent parser with just plus. Then plus and minus. Etc.
- Understand the core interesting part about an algorithm. Recursive descent,
using the stack to enforce order of operations.
- Avoid using NULL. Instead use empty holder types like NodeEmpty and TokenEOF.
Helps prevent segfault issues / dereferencing NULL.
- Understand what pieces you can minimally build out while contiuing the core
functionality. Example: built out parser by only supporting the number literal
'1'. It's easy to add better number parsing later, but it's not something that's
critical for implementing most of the parser.
- Don't get caught up on specific algorithms like recursive descent parsing.
Writing a toy compiler is just like any other program. Translate one form to
another.
  - Look at a specific small example like "1 + 2", get that implemented simply.
  - Move to "1 + 2 + 3", get that implemented correctly, without making anything
  too generic.
  - Then move to "1 + 2 + 3 + 4", etc.
  - Once that's working, move to "1 + 2 * 3".
  - Take things in small chunks.
  - This approach worked really well in writing the parser from scratch.

### Naming

- Subclass should be Class[BaseClass]
  - Example: EmptyNode (not NodeEmpty).
- Functions on classes should follow the class name Class[Function]
  - Example: EmptyNodePrint (not PrintEmptyNode).
- Functions not associated with a strict "class" don't need to follow the
  previous convention.
  - Example: PrintError (doesn't need to be ErrorPrint).
- Enum constants should have the full enum as a prefix
  - Example: enum kTokenType - constant kTokenTypeOpenParen (not kOpenParen).

### Basic Example

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

### Generics

```
func add<T>(a: T, b: T): T {
  return a + b;
}
```

### Types

```
int
float
char
string?
arrays?
pointer types?
```
