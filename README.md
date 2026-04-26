# 42life

Conway's Game of Life in C, started from a 42 exam-style implementation and expanded into a personal project.

This repository currently contains two tracks:

- `exam5.1/exam5_1.c`: the 42 exam-oriented version.
- `life/life.c`: the version under active development, focused on becoming a safer and more robust implementation.

## What It Does

The program lets you:

- Draw an initial pattern on a grid using keyboard input.
- Run a chosen number of Game of Life iterations.
- Print the final grid to standard output.

Alive cells are rendered as `O`, dead cells as spaces.

## Build

Compile either file with `cc`:

```bash
cc -Wall -Wextra -Werror exam5.1/exam5_1.c -o exam5_1
cc -Wall -Wextra -Werror life/life.c -o life
```

## Usage

```bash
./life <width> <height> <iterations>
```

Example:

```bash
./life 20 10 15
```

Arguments:

- `width`: number of columns
- `height`: number of rows
- `iterations`: number of simulation steps to run after input ends

## Input Controls

Before the simulation starts, the program reads characters from stdin:

- `w`: move up
- `a`: move left
- `s`: move down
- `d`: move right
- `x`: toggle drawing mode on/off

When drawing mode is on, moving marks visited cells as alive.
Simulation begins when input reaches EOF.

## Quick Example (Piped Input)

```bash
printf "xdddssaa" | ./life 10 10 8
```

This sends movement commands, then EOF, then runs 8 iterations and prints the resulting board.

## Project Direction

The ongoing work is centered on `life/life.c`, with emphasis on:

- Safer input handling
- Better validation and error reporting
- Improved code structure and maintainability

## License

No license file is currently provided.
