//
//  main.cpp
//  FunctionOverload
//
//  Created by 裴明明 on 2020/2/4.
//  Copyright © 2020 裴明明. All rights reserved.
//

#include <iostream>
using namespace std;

// MARK: - 函数重载的规则

// 规则：
// 函数名相同，参数个数不一样，类型不一样，或者顺序不一样

// 注意点：
// 返回值类型的不同与函数重载无关
// 调用函数时，实参的隐式类型转换，默认参数的设置 等情况都有可能会产生二义性

// MARK: - 参数个数不一样

int sum1(int v1, int v2) {
    return v1 + v2;
}
int sum1(int v1, int v2, int v3) {
    return v1 + v2 + v3;
}

// MARK: - 参数类型不一样

int sum2(int v1, int v2) {
    return v1 + v2;
}
double sum2(double v1, double v2) {
    return v1 + v2;
}

// MARK: - 参数顺序不一样

int sum3(int v1, double v2) {
    return v1 + int(v2);
}
int sum3(double v1, int v2) {
    return int(v1) + v2;
}

// MARK: - 只有返回值类型不同(不构成重载)

int sum4(int v1, int v2) {
    return v1 + v2;
}

// 编译报错: Functions that differ only in their return type cannot be overloaded
//double sum4(int v1, int v2) {
//    return v1 + v2;
//}

// MARK: - 实参的隐式类型转换(调用函数的时候可能会产生二义性)

void printValue(long v1) {
    cout << "test(long v1)" << endl;
}

void printValue(double v1) {
    cout << "test(double v1)" << endl;
}

void test1() {
    // 编译报错: Call to 'printValue' is ambiguous
    // 原因：10可以被隐式转换成long或者double，编译器懵了，不知道你到底想转哪一个
    // 正确的调用：printValue(10l) or printValue(10.0)
//    printValue(10);
    
    printValue(10l);
    printValue(10.0);
}

// MARK: - 默认参数情况(调用函数的时候可能会产生二义性)

int sum5(int v1, int v2 = 10) {
    return v1 + v2;
}
int sum5(int v1) {
    return v1 + 10;
}

void test2() {
    // 编译报错: Call to 'sum5' is ambiguous
//    sum5(20);
}

// MARK: - 函数重载的本质

// 本质：
// 其实是C++中采用了name mangling or name decoration技术，即C++编译器会对重载的函数名按照一定的规则进行改编，底层原理还是函数名不同的函数

int sum(int v1, int v2) {
    return v1 + v2;
}

int sum(int v1, int v2, int v3) {
    return v1 + v2 + v3;
}

void test() {
    sum(1, 2);
    sum(3, 4, 5);
    
    /*
     对应的汇编代码如下（从汇编代码可以看出来上边的2个sum函数调用分别对应2个不同地址的sum函数，所以我们可以认为，虽然函数的名称看起来相同，但是本质上还是2个独立的函数）：
     `0x100001230` & `0x100001250` 分别对应上边调用的2个sum函数的地址
     FunctionOverload`test:
         0x100001270 <+0>:  pushq  %rbp
         0x100001271 <+1>:  movq   %rsp, %rbp
         0x100001274 <+4>:  subq   $0x10, %rsp
         0x100001278 <+8>:  movl   $0x1, %edi
         0x10000127d <+13>: movl   $0x2, %esi
         0x100001282 <+18>: callq  0x100001230               ; sum at main.cpp:98
         0x100001287 <+23>: movl   $0x3, %edi
         0x10000128c <+28>: movl   $0x4, %esi
         0x100001291 <+33>: movl   $0x5, %edx
         0x100001296 <+38>: movl   %eax, -0x4(%rbp)
         0x100001299 <+41>: callq  0x100001250               ; sum at main.cpp:102
         0x10000129e <+46>: movl   %eax, -0x8(%rbp)
         0x1000012a1 <+49>: addq   $0x10, %rsp
         0x1000012a5 <+53>: popq   %rbp
     ->  0x1000012a6 <+54>: retq
     */
}

int main(int argc, const char * argv[]) {
    
    test1();
    test2();
    
    test();
    
    return 0;
}
