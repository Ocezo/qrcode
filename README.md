# C++ QR Code Generator

Small C++ program that generates a QR code from an input string.

## Example

Input:

```txt
bag_1
```

Output:

A QR code generated from the value `bag_1`.

## Build

Typical CMake build steps:

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Usage

If the program accepts the input as a command-line argument:

```bash
./qrcode bag_1
```

If it reads from standard input instead:

```bash
echo "bag_1" | ./qrcode
```

## Notes

- Replace `qrcode` with the actual binary name if needed.
- Depending on your project setup, the executable may be located inside the `build/` directory.
