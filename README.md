# Advent of Code 2025

## Overview

Repository for my solutions of [Advent of Code](https://adventofcode.com/2025) (AOC) puzzles.

My goal is to implement all solutions in C++ using only the Standard Library.
Besides solving the puzzle, the code also runs unit tests that I used during development, and it reproduces the examples given by AOC.

## Usage

The root directory of the repository (the one that contains this `README.md`) contains subdirectories corresponding to each AOC day.
In the following, the variable `DD` can be any of these directories (`DD = 01, 02, ... 12`).

### Prerequisites

* C++ compiler that supports standards >=20
* [CMake](https://cmake.org/) >= 3.10

In addition to the software prerequisites, the code may require input files provided by AOC.
There is typically some example input, and the actual puzzle input.
This project uses the convention of placing the example input in a file called `input_test.txt`, and the puzzle input in `input.txt`.
The input files for a given day need to be placed in the corresponding `DD` directory.

### Build

Execute the following commands on the command line in a directory `DD`:

```
cmake .
cmake --build .
```

This will create an executable called `aocDD`.

### Run

After building the executable, run it by executing the following command on the command line in a directory `DD`:

```
./aocDD
```