# Task 5

# Command Shell Parser in C

A command-line shell parser implementation in C that processes user input according to a defined grammar and constructs a parse tree representation of commands, pipelines, and redirections.

## Features

- **Lexical Analysis**: Tokenizes shell commands with support for:
  - Quoted strings (`"..."`)
  - Input/output redirection (`<`, `>`, `>>`)
  - Pipes (`|`)
  - Logical operators (`&&`, `||`)
  - Background execution (`&`)
  - Command separators (`;`)

- **Parse Tree Construction**: Builds a hierarchical tree structure where:
  - Each node represents a command with its arguments
  - `pipe` pointers connect piped commands
  - `next` pointers connect sequentially or logically linked commands
  - `type` indicates logical connection type (`NXT`, `AND`, `OR`)

- **Environment Variable Expansion**: Supports expansion of common shell variables:
  - `$HOME` – User's home directory
  - `$SHELL` – Current shell path
  - `$USER` – Current username
  - `$EUID` – Effective user ID

## Grammar Definition

The parser follows this EBNF grammar:

```
<command_shell> ::= <conveyor> { [one from &, &&, ||, ;] <conveyor> } [&, ;]
<conveyor>      ::= <command> { | <command> }
<command>       ::= <name> { <arg> } [ < <name> ] [[one from >, >>] <name> ]
```

## Project Structure

| File | Description |
|------|-------------|
| `main.c` | Program entry point and main loop |
| `input.c` / `input.h` | Input tokenization and lexical analysis |
| `tree.c` / `tree.h` | Parse tree construction and manipulation |
| `list.c` / `list.h` | Dynamic list and buffer utilities |
| `README.md` | This documentation |

## Data Structures

### Command Node (`cmd_inf`)
```c
struct cmd_inf {
    char **argv;           // Command name and arguments array
    char *infile;          // Input redirection file
    char *outfile;         // Output redirection file
    int append;            // Append mode flag (>>)
    int backgrnd;          // Background execution flag
    struct cmd_inf* pipe;  // Next command in pipeline
    struct cmd_inf* next;  // Next command in sequence
    enum type_of_next type;// Connection type (NXT, AND, OR)
};
```

### Dynamic List & Buffer
Custom implementations for efficient string storage and token management.

## Building and Running

### Compilation
```bash
make
```

### Execution
```bash
./shell
```

The program will display a prompt and process commands entered interactively.

## Example Usage

**Input:**
```
echo "Hello World" | wc -l && ls -la > output.txt &
```

**Output:**
- Parse tree showing the pipeline structure
- Tokenized command list after variable expansion
- Tree visualization with indentation showing hierarchy

## Error Handling

The parser uses `setjmp`/`longjmp` for error recovery, providing robust handling of malformed input and memory allocation failures.

## Limitations

- This is a parser only – no actual command execution is performed
- Supports a subset of shell syntax (not a full POSIX shell)
- Designed primarily for educational purposes in parsing techniques

## Notes

The implementation demonstrates several important concepts:
- Recursive descent parsing techniques
- Abstract syntax tree construction
- Memory management in C
- Lexical analysis with state machines
- Environment variable expansion
