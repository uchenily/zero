## Zero

一个极小的编程语言, 从零开始

### 构建步骤

1. 安装构建工具 meson, ninja

```shell
pip install -U meson ninja
```

2. (可选项)需要安装依赖 `libfmt>=9.1.0`, 如果系统安装版本不满足要求, 可以选择通过meson安装

```shell
mkdir subprojects
meson wrap install fmt
```

3. 编译项目

```shell
meson setup build
meson compile -C build
```

4. 执行测试

执行REPL (Ctrl-C/Ctrl-D退出)

```shell
$ ./zero
> print("hello world");
hello world
```

执行脚本文件


```shell
$ cat examples/fibonacci.zero
fn fibonacci(n) {
    if (n < 2) {
        return n;
    }
    return fibonacci(n - 2) + fibonacci(n - 1);
}

print(fibonacci(20));
$ ./zero examples/fibonacci.zero
6765
```

```shell
$ cat examples/native_function.zero
print(clock);

let retval = clock();
print(retval);

let retval = read_file("/foo/bar");
print("读取到文件内容:");
print(retval);

$ ./zero examples/native_function.zero
<native fn>
1700075574.000000
reading /foo/bar ...
读取到文件内容:
example text
```
