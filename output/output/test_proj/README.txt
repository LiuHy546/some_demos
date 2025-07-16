# test_proj

本项目用于测试 output 目录下 mylib.a 的递归依赖和链接效果。

## 编译

```sh
mkdir build
cd build
cmake ..
make
```

## 运行

```sh
./bin/test_main
```

## 说明
- CMakeLists.txt 会自动链接 ../mylib.a
- main.c 会调用 output 项目下的多个函数，验证静态库递归依赖是否完整。 