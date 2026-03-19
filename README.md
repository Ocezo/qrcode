# QR Code Generator

Small C++ project intended to generate a QR code from a string input such as `bag_1`.
For the moment this project is just a wrapper of the famous [qrencode](https://github.com/fukuchi/libqrencode) library written by [Kentaro Fukuchi](https://fukuchi.org).

## Status

This repository now includes a minimal CMake-based C++ skeleton:

- a `CMakeLists.txt`
- a `src/main.cpp`
- a `qrcode` executable target

The current program parses input from either the command line or standard input.
It generates a real QR code PNG using `qrencode` and writes the final image with OpenCV.

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

## Requirements

This project requires `qrencode` to be available in `PATH` at runtime.

Linux (Debian/Ubuntu):

```bash
sudo apt update
sudo apt install qrencode
```

Windows (Chocolatey):

```powershell
choco install qrencode
```

Project link:

- `libqrencode`: https://github.com/fukuchi/libqrencode

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
- generates a real QR code image from the input text
- writes the output image to the current directory

Example output:

```txt
Input received: bag_1
Image written to: qrcode_bag_1.png
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

- add configurable output options
- add tests
- document runtime options once the feature set is stable
