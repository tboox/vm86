# The x86 Script Instruction Virtual Machine

[![Join the chat at https://gitter.im/waruqi/tboox](https://badges.gitter.im/waruqi/tboox.svg)](https://gitter.im/waruqi/tboox?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge) [![donate](http://tboox.org/static/img/donate.svg)](http://tboox.org/donation/)

## Introduction

This is a very simple and lightweight x86 virtual machine which can load and run the assembly code from ida pro directly.
 
## Features

* Supports cross-platform and it's able to run the x86 assembly code on linux, windows, maxosx, android and ios ...
* Supports the frequently-used x86 assembly instruction (.e.g logical operations, goto, loop, call, stack operations ...)
* Supports call the third-party library interfaces. (.e.g libc ..)
* We can pass arguments and get the return results after running.
* Supports thread-safe.
* Does not support arm64 and only for 32-bits architecture 

## Example

We get one assemble code from ida pro first and this code will call the libc api: `printf`

```asm
sub_hello	proc near 
arg_0		= dword	ptr  8 
.data 
        format db \"hello: %x\", 0ah, 0dh, 0 
 
off_5A74B0	dd offset loc_6B2B50	; DATA XREF: sub_589100+1832r 
		dd offset loc_58A945	; jump table for switch	statement 
 
.code 
        ; hi
        push	ebp ;hello 
		mov	ebp, esp 
 
    loc_6B2B50:				; CODE XREF: sub_6B2B40+8j
        push    eax 
		mov	eax, [ebp+arg_0] 
        push eax 
        mov eax, offset format 
        push eax 
        call printf 
        add esp, 4 
        pop eax 
        
        mov ecx, 1
        jmp ds:off_5A74B0[ecx*4]
 
loc_58A945:
        push    eax 
		mov	eax, [ebp+arg_0] 
        push eax 
        mov eax, offset format 
        push eax 
        call printf 
        add esp, 4 
        pop eax 
        
  end:
        mov	esp, ebp 
		pop	ebp 
        retn 
sub_hello    endp 
```

And we call it in c language first.

```c
sub_hello(31415926);
```

The output results:

```
hello: 31415926
hello: 31415926
```

Nextly, we attempt to load this asm code using our x86 virtual machine.

```c
static tb_void_t vm86_demo_proc_exec_hello(tb_uint32_t value)
{
    // the code
    static tb_char_t const s_code_sub_hello[] = 
    {
"sub_hello	proc near \n\
arg_0		= dword	ptr  8 \n\
.data \n\
        format db \"hello: %x\", 0ah, 0dh, 0 \n\
 \n\
off_5A74B0	dd offset loc_6B2B50	; DATA XREF: sub_589100+1832r \n\
		dd offset loc_58A945	; jump table for switch	statement \n\
 \n\
.code \n\
        ; hi\n\
        push	ebp ;hello \n\
		mov	ebp, esp \n\
 \n\
    loc_6B2B50:				; CODE XREF: sub_6B2B40+8j\n\
        push    eax \n\
		mov	eax, [ebp+arg_0] \n\
        push eax \n\
        mov eax, offset format \n\
        push eax \n\
        call printf \n\
        add esp, 4 \n\
        pop eax \n\
        \n\
        mov ecx, 1\n\
        jmp ds:off_5A74B0[ecx*4]\n\
 \n\
loc_58A945:\n\
        push    eax \n\
		mov	eax, [ebp+arg_0] \n\
        push eax \n\
        mov eax, offset format \n\
        push eax \n\
        call printf \n\
        add esp, 4 \n\
        pop eax \n\
        \n\
  end:\n\
        mov	esp, ebp \n\
		pop	ebp \n\
        retn \n\
sub_hello    endp \n\
    "
    };

    // the machine
    vm86_machine_ref_t machine = vm86_machine();
    if (machine)
    {
        // the lock
        tb_spinlock_ref_t lock = vm86_machine_lock(machine);

        // enter
        tb_spinlock_enter(lock);

        // the stack
        vm86_stack_ref_t stack = vm86_machine_stack(machine);

        // compile proc
        vm86_proc_ref_t proc = vm86_text_compile(vm86_machine_text(machine), s_code_sub_hello, sizeof(s_code_sub_hello));
        if (proc)
        {
            // add function
            vm86_machine_function_set(machine, "printf", vm86_demo_proc_func_printf);

            // init arguments
            vm86_stack_push(stack, value);

            // done proc
            vm86_proc_done(proc);

            // restore stack
            vm86_stack_pop(stack, tb_null);

            // trace
            tb_trace_i("sub_hello(%x)", value);
        }

        // leave
        tb_spinlock_leave(lock);
    } 
}

int main(int argc, char** argv)
{
    // call this function: sub_hello(0x31415926)
    vm86_demo_proc_exec_hello(0x31415926);    
}
```

The output results:

```
hello: 31415926
hello: 31415926
```

## Compilation

Please install [xmake](http://www.xmake.io) first!

### Compile project on macosx 

```bash
$ sudo brew install xmake
$ xmake f -a i386
$ xmake
```

### Compile project on linux 

```bash
$ git clone https://github.com/waruqi/xmake.git
$ cd xmake
$ sudo ./install
$
$ cd vm86
$ xmake f -a i386
$ xmake
```

### Compile project on windows 

Downloads https://github.com/waruqi/xmake/archive/master.zip first.

Extracts it and run install.bat

Lastly, we start compiling vm86 project.

```bash
$ xmake
```

### Compile project for android 

```bash
$ cd vm86
$ xmake f -p android --ndk=/xxx/ndk
$ xmake
```

## Running

```bash
$ xmake r demo
```

## Ida scripts

The script files: `export_function.idc` and `export_data.idc` in the project directory (idc) 
can help us to export the given assembly function and data from the ida pro.

#### Contacts

- Email:        
    - waruqi@gmail.com
- Website: 	    
	- [tboox.org](http://www.tboox.org)
	- [tboox.net](http://www.tboox.net)


# x86汇编指令脚本虚拟机

## 简介

这是一个可以直接解释执行从ida pro里面提取出来的x86汇编代码的虚拟机。

非常精简，整体架构上不能跟那些成熟的虚拟机相比，主要目标是够用、能用、轻量就行，如果觉得代码架构设计的不是很好的话，也不用过于吐槽哈。。

虽然我还有写过两个比较成熟的虚拟机项目（jvm和avm），虽然架构上比这个更完善，更容易扩展，功能也更强大

但是毕竟是给公司写的，没法拿出来分享。。

## 背景

先说说，为什么要写这个东西。。

之前有段时间，我在用ida逆向分析某些程序的算法，并且要把它提取出来将其跨平台运行，这个时候我首先考虑到是ida的F5插件

毕竟这个可以直接反成c/c++代码，还是很强大的，基本上98%的x86汇编代码，我在通过f5还原成c/c++代码后，都能正常运行。

原本我以为可以万事大吉了，不过就在当我沾沾自喜的时候，发现其中某个汇编函数的c代码，死活就是运行不正常，输出结果不对。

而且那个函数偏偏代码量出奇的大，光c代码就有上万行，而且里面还对数据结构和明文都做了变换和加密，要是慢慢调试的话，得痛苦死。。哎。。


没办法，只好另想出路，既然ida还原c有时候不一定完全准确，但是其汇编代码的准确度还是可以保证的，并且从ida中提取的汇编代码
基本上，不用怎么改，就能编译通过，因此，我先验证了下直接编译汇编代码，运行看看结果对不对。。

结果跟我想的一样，是ok的。。那么问题来了。。

既然汇编运行结果正常，那怎么把它整成跨平台运行呢，直接从编译后x86的指令集进行模拟？工作量有点大，得不偿失。。

有没有取巧些办法呢？当然有，那就是直接解析和运行源码级的x86汇编代码，相当于写个轻量级的精简版x86的脚本虚拟机，来把它运行起来。。

听上去，貌似更麻烦了，其实由于这里只要能够跑通部分需要的汇编指令就行了，因此写个精简版的还是很方便，不需要多少工作量

我前前后后，也就花了一个礼拜就搞定了，非常精简，当然也不完善（也没必要哈，不能跟那些大部头相比）

我的目标就是够用就行，因此我写的差不多厚，就尝试去加载之前有问题的汇编代码，如果发现有指令没实现，那就去实现它，直到跑通为主。。

最后测试结果：

可以正常跑通那个十几万行的汇编代码，并且在arm下运行的性能还算ok，至少满足我的个人需求了。。: )

## 特性

* 跨平台运行支持，可以在windows、linux、macosx以及android, ios上运行x86的汇编代码。。
* 支持常用x86汇编指令（例如，逻辑操作，跳转，循环，调用，压栈等指令）
* 支持函数间跳转，以及第三方api调用
* 支持参数传入，以及运行结束后，返回值的获取
* 虚拟机的运行粒度为单个函数，函数间的跳转可以通过多个虚拟机实例来完成（轻量的，性能影响不大）
* 支持线程安全
* 暂时不支持arm64，只能在32位下运行（有兴趣的同学可以自行修改）

## 例子

我们先从ida中提取一段汇编代码，这段汇编主要是`printf`库函数打印外部传入的数值

```asm
sub_hello	proc near 
arg_0		= dword	ptr  8 
.data 
        format db \"hello: %x\", 0ah, 0dh, 0 
 
off_5A74B0	dd offset loc_6B2B50	; DATA XREF: sub_589100+1832r 
		dd offset loc_58A945	; jump table for switch	statement 
 
.code 
        ; hi
        push	ebp ;hello 
		mov	ebp, esp 
 
    loc_6B2B50:				; CODE XREF: sub_6B2B40+8j
        push    eax 
		mov	eax, [ebp+arg_0] 
        push eax 
        mov eax, offset format 
        push eax 
        call printf 
        add esp, 4 
        pop eax 
        
        mov ecx, 1
        jmp ds:off_5A74B0[ecx*4]
 
loc_58A945:
        push    eax 
		mov	eax, [ebp+arg_0] 
        push eax 
        mov eax, offset format 
        push eax 
        call printf 
        add esp, 4 
        pop eax 
        
  end:
        mov	esp, ebp 
		pop	ebp 
        retn 
sub_hello    endp 
```

如果用c来调用的话，就是

```c
sub_hello(31415926);
```

输出结果：

```
hello: 31415926
hello: 31415926
```

接下来我们把这段汇编直接放到我们的虚拟机里面执行：

```c
static tb_void_t vm86_demo_proc_exec_hello(tb_uint32_t value)
{
    // 上述汇编代码的字符串表示
    static tb_char_t const s_code_sub_hello[] = 
    {
"sub_hello	proc near \n\
arg_0		= dword	ptr  8 \n\
.data \n\
        format db \"hello: %x\", 0ah, 0dh, 0 \n\
 \n\
off_5A74B0	dd offset loc_6B2B50	; DATA XREF: sub_589100+1832r \n\
		dd offset loc_58A945	; jump table for switch	statement \n\
 \n\
.code \n\
        ; hi\n\
        push	ebp ;hello \n\
		mov	ebp, esp \n\
 \n\
    loc_6B2B50:				; CODE XREF: sub_6B2B40+8j\n\
        push    eax \n\
		mov	eax, [ebp+arg_0] \n\
        push eax \n\
        mov eax, offset format \n\
        push eax \n\
        call printf \n\
        add esp, 4 \n\
        pop eax \n\
        \n\
        mov ecx, 1\n\
        jmp ds:off_5A74B0[ecx*4]\n\
 \n\
loc_58A945:\n\
        push    eax \n\
		mov	eax, [ebp+arg_0] \n\
        push eax \n\
        mov eax, offset format \n\
        push eax \n\
        call printf \n\
        add esp, 4 \n\
        pop eax \n\
        \n\
  end:\n\
        mov	esp, ebp \n\
		pop	ebp \n\
        retn \n\
sub_hello    endp \n\
    "
    };

    // 定义一个虚拟机
    vm86_machine_ref_t machine = vm86_machine();
    if (machine)
    {
        // 锁定虚拟机，保证线程安全（这个根据需要，可选）
        tb_spinlock_ref_t lock = vm86_machine_lock(machine);
        tb_spinlock_enter(lock);

        // 获取虚拟机的堆栈
        vm86_stack_ref_t stack = vm86_machine_stack(machine);

        // 编译上面的汇编代码，并生成一个过程对象的引用
        vm86_proc_ref_t proc = vm86_text_compile(vm86_machine_text(machine), s_code_sub_hello, sizeof(s_code_sub_hello));
        if (proc)
        {
            // 添加汇编里面需要调用到的外部库函数
            vm86_machine_function_set(machine, "printf", vm86_demo_proc_func_printf);

            // 初始化调用参数
            vm86_stack_push(stack, value);

            // 执行这个汇编代码
            vm86_proc_done(proc);

            // 恢复堆栈，获取返回值（这里是void的，传null就行了）
            vm86_stack_pop(stack, tb_null);
        }

        // 解锁虚拟机
        tb_spinlock_leave(lock);
    } 
}

int main(int argc, char** argv)
{
    // 执行这个汇编函数：sub_hello(0x31415926)
    vm86_demo_proc_exec_hello(0x31415926);    
}
```


如果ok，那么输出结果当然也是：

```
hello: 31415926
hello: 31415926
```

## 编译

需要先安装[xmake](http://www.xmake.io/cn)

### 在 macosx 上编译

```bash
$ sudo brew install xmake
$ xmake f -a i386
$ xmake
```

### 在 linux 上编译

```bash
$ git clone https://github.com/waruqi/xmake.git
$ cd xmake
$ sudo ./install
$
$ cd vm86
$ xmake f -a i386
$ xmake
```

### 在 windows 上编译

下载 https://github.com/waruqi/xmake/archive/master.zip

解压运行里面的 install.bat 安装xmake后进行编译：


```bash
$ xmake
```

### 编译android版本

```bash
$ cd vm86
$ xmake f -p android --ndk=/xxx/ndk
$ xmake
```

## 运行

运行测试程序：

```bash
$ xmake r demo
```

## 后话

最后，在项目的idc目录下，有两个脚本工具：`export_function.idc` 和 `export_data.idc` 可以用来辅助我们从ida中导出指定的汇编函数和数据

## 联系方式

- Email:        
    - waruqi@gmail.com
    - waruqi@126.com
- Website: 	    
	- [tboox.org](http://www.tboox.org/cn)
	- [tboox.net](http://www.tboox.net/cn)
- QQ(group):    
    - 343118190
