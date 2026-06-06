# passgen

**English** | [中文](./README.zh-CN.md)

A simple and secure CLI random password generator

## Features

- Generate random passwords with high entropy
- Passwords contain A-Z, a-z, 1-9, !@#$%^&*_-=+?, but 0, I, l, 1 are excluded to avoid confusion
- Customizable length and count
- Statically linked, no runtime depandencies

## Build

```bash
git clone https://github.com/Limakx/passgen.git
cd passgen
mkdir build
cd build
cmake ..
make
```

The executable will be generated at build/bin

## Usage

```bash
./passgen [OPTIONS] [LENGTH] [COUNT] [OUTPUT_FILE]
```

Type "./passgen -h" for help

Run without arguments to enter interactive mode