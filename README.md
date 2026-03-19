# QR Code Generator

Small C++ project intended to generate a QR code from a string input such as `bag_1`.

## Status

This repository now includes a minimal CMake-based C++ skeleton:

- a `CMakeLists.txt`
- a `src/main.cpp`
- a `qrcode` executable target

The current program parses input from either the command line or standard input.
Actual QR code generation is still not implemented yet.

## Goal

The intended goal of the project is to:

- accept a text value as input
- generate the corresponding QR code
- output the result in a usable form

Example input:

```txt
bag_1
```

## Build

Standard CMake build flow:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Usage

Run with a command-line argument:

```bash
./qrcode bag_1
```

Or pass the input through standard input:

```bash
echo "bag_1" | ./qrcode
```

Show the help message:

```bash
./qrcode --help
```

## Current Behavior

The program currently:

- reads a text value
- validates that an input was provided
- prints the received value
- reports that QR code generation is not implemented yet

Example output:

```txt
Input received: bag_1
QR code generation is not implemented yet.
```

## Project Layout

```txt
.
├── CMakeLists.txt
├── README.md
└── src/
    └── main.cpp
```

## Next Steps

- implement the QR code generation logic
- define the output format
- add tests
- document runtime options once the feature set is stable
