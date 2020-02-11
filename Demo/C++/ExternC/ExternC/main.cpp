//
//  main.cpp
//  ExternC
//
//  Created by 裴明明 on 2020/2/6.
//  Copyright © 2020 裴明明. All rights reserved.
//

#include <iostream>
using namespace std;

/*
 虽然说C++是C语言的超集，并且基本上在C++可以直接调用C语言的函数，但是C++与C语言还是在用不同的编译系统，语法上还是有很多不同的特性
 如果你希望编写的纯C的代码在C++上被调用，可以使用 extern "C" 去修饰C语言的代码块
 
 extern "C" 修饰的代码会按照C语言的方式去编译
 
 注意点：
 如果函数既有声明又有定义，函数的声明必须被 extern "C" 修饰，定义可以不必
 */

// 可以有以下4种不同的用法

// 用法一：
extern "C" void test1() {
    cout << "test1()" << endl;
}
// 用法二：
extern "C" {
    void test2() {
        cout << "test2()" << endl;
    }
}
// 用法三：
extern "C" {
    // 这个会引用系统库math.h
    #include <math.h>
}
// 用法四(更通常的做法，在math.h内部已经做了extern "C"的处理)：
#include "math.h"

int main(int argc, const char * argv[]) {
    
    test1();
    test2();
    
    int result = sum(1, 2);
    cout << result << endl;
  
    return 0;
}
