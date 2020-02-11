//
//  main.cpp
//  DefaultParameters
//
//  Created by 裴明明 on 2020/2/6.
//  Copyright © 2020 裴明明. All rights reserved.
//

#include <iostream>
using namespace std;

// 函数默认参数

// MARK: - 函数默认参数的规则

// 规则：
// 默认参数可以是 常量，全局符号（变量，函数名）且默认参数必须从最右边开始

// 注意点：
// * 默认参数只能放在函数声明中（如果声明和定义同时存在）
// * 默认参数与函数重载可能会产生冲突，二义性

int age = 10;

void test1() {
    cout << "test()" << endl;
}
void test2() {
    cout << "test2()" << endl;
}

void printParameters(int v1, int v2 = 10, int v3 = age, void (*p)() = test1) {
    cout << "v1 is: " << v1 << endl;
    cout << "v2 is: " << v2 << endl;
    cout << "v3 is: " << v3 << endl;
    p();
}

void test3() {
    printParameters(10);
    printParameters(10, 20, 30);
    printParameters(10, 30, 40, test2);
}

// MARK: - 函数默认参数的本质

int sum(int v1, int v2 = 2) {
    return v1 + v2;
}

void test4() {
    sum(1);
    
    /*
     对应的汇编代码：
     DefaultParameters`test4:
         0x100001290 <+0>:  pushq  %rbp
         0x100001291 <+1>:  movq   %rsp, %rbp
         0x100001294 <+4>:  subq   $0x10, %rsp
         0x100001298 <+8>:  movl   $0x1, %edi
         0x10000129d <+13>: movl   $0x2, %esi
         0x1000012a2 <+18>: callq  0x100001270               ; sum at main.cpp:47
         0x1000012a7 <+23>: movl   %eax, -0x4(%rbp)
     ->  0x1000012aa <+26>: addq   $0x10, %rsp
         0x1000012ae <+30>: popq   %rbp
         0x1000012af <+31>: retq
     
     汇编代码：movl $0x2, %esi 这一句表示编译器最终也会将默认参数当作一般的参数存储到寄存器中进行运算
     也就是说：默认参数和一般的参数在本质上没有区别
     */
}

int main(int argc, const char * argv[]) {
    
    test3();
    test4();
    
    return 0;
}
