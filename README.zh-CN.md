# passgen

 **中文** | [English](./README.md)

轻量、安全的命令行随机密码生成器

## 特性

- 能够生成高熵随机密码
- 密码包含 A-Z, a-z, 1-9, !@#$%^&*_-=+?, 除 0, I, l, 1 不包含在内以避免混淆
- 自由确定生成密码的长度和数量
- 静态编译，不需要额外的运行库

## 构建

```bash
git clone https://github.com/Limakx/passgen.git
cd passgen
mkdir build
cd build
cmake ..
make
```

可执行文件将会生成在 build/bin

## 用法

```bash
./passgen [OPTIONS] [LENGTH] [COUNT] [OUTPUT_FILE]
```

输入 "./passgen -h" 获取帮助

可以不附加参数直接运行