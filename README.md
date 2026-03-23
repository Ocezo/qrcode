# QR Code Generator

Small C++ project that generates a QR code PNG from a simple input such as `1`.
At the moment, it acts as a lightweight wrapper around [qrencode](https://github.com/fukuchi/libqrencode).

## Status

The project currently includes:

- a CMake build
- a modular C++ codebase
- a `qrcode` executable
- a YAML configuration file used to build the final URL

The program reads an input value, builds a target URL from `config/params.yaml`, generates a QR code with `qrencode`, and writes the final PNG into `img/`.

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
https://ocezo.fr/earthbag.php?bag=bag1
```

## Build

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Requirements

`qrencode` must be available in `PATH` at runtime.

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

Command-line input:

```bash
./qrcode 1
```

Standard input:

```bash
echo "1" | ./qrcode
```

Help:

```bash
./qrcode --help
```

## Configuration

The target URL is built from [config/params.yaml](https://github.com/Ocezo/qrcode/blob/main/config/params.yaml):

```yaml
url_params:
  website: "https://ocezo.fr"
  page:    "earthbag.php"
  key:     "bag"
```

Final URL format:

```txt
<website>/<page>?<key>=<key><input>
```

With the current configuration and the input `1`, the encoded URL is:

```txt
https://ocezo.fr/earthbag.php?bag=bag1
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
Encoded URL: https://ocezo.fr/earthbag.php?bag=bag1
Image written to: /home/your/Workspace/qrcode/img/qrcode_bag1.png
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

- add tests
- add configurable output options
- document runtime options once the feature set is stable
