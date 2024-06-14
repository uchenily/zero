## Zero

一个极小的编程语言, 从零开始

### 构建步骤

1. 安装构建工具 meson, ninja

```shell
pip install -U meson ninja
```

2. 编译项目

```shell
meson setup build
meson compile -C build
```

3. 执行测试

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
