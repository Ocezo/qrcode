# QR Code Generator

Small C++ project that generates a QR code PNG from a simple input such as `1`.
For now, this project is a small wrapper around the well-known [qrencode](https://github.com/fukuchi/libqrencode) library written by [Kentaro Fukuchi](https://fukuchi.org).

## Status

This repository now includes:

- a CMake build
- a small modular C++ codebase
- a `qrcode` executable target
- a YAML configuration file used to build the final target URL

The program reads an input value, converts it into a URL using `config/params.yaml`, generates a QR code with `qrencode`, and writes the final PNG image into `img/`.

## Goal

The project is intended to:

- accept a text value as input
- build a target URL from configuration parameters
- generate the corresponding QR code
- save the result as a PNG image

Example input:

```txt
1
```

With the current configuration, this becomes:

```txt
https://ocezo.fr/earthbag.html/#bag1
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

- [libqrencode](https://github.com/fukuchi/libqrencode)

## Usage

Run with a command-line argument:

```bash
./qrcode 1
```

Or pass the input through standard input:

```bash
echo "1" | ./qrcode
```

Show the help message:

```bash
./qrcode --help
```

## Configuration

The target URL is built from [config/params.yaml](https://github.com/Ocezo/qrcode/blob/main/config/params.yaml):

```yaml
url_params:
  website: "https://ocezo.fr"
  page:    "earthbag.html"
  key:     "bag"
```

The final URL format is:

```txt
<website>/<page>/#<key><input>
```

With the current configuration and the input `1`, the program encodes:

```txt
https://ocezo.fr/earthbag.html/#bag1
```

## Current Behavior

The program currently:

- reads a text value from the command line or standard input
- validates that an input was provided
- loads `website`, `page`, and `key` from `config/params.yaml`
- builds the target URL from those parameters
- generates a real QR code image from that URL
- writes the output image into `img/`

Example output:

```txt
Input received: 1
Encoded URL: https://ocezo.fr/earthbag.html/#bag1
Image written to: /home/you/Workspace/qrcode/img/qrcode_bag1.png
```

## Project Layout

```txt
.
├── CMakeLists.txt
├── README.md
├── config/
│   └── params.yaml
├── img/
│   └── .dummy
├── include/
│   ├── config_loader.hpp
│   ├── qr_generator.hpp
│   ├── url_builder.hpp
│   └── url_params.hpp
└── src/
    ├── config_loader.cpp
    ├── main.cpp
    ├── qr_generator.cpp
    └── url_builder.cpp
```

## Next Steps

- add configurable output options
- add tests
- document runtime options once the feature set is stable
