//
//  main.cpp
//  ExceptionHandling
//
//  Created by 裴明明 on 2020/2/2.
//  Copyright © 2020 裴明明. All rights reserved.
//

#include <iostream>
using namespace std;

// MARK: - 异常的定义和表现

void test1() {
    // 异常的定义：
    // 异常是一种在程序运行过程中的发生的不好预测的错误(比如内存不够)
    // 异常没有被处理的后果：
    // 如果以下的异常没有被捕获，那么程序就会崩溃
    try {
        int *p = new int[99999999999999999]();
        cout << p << endl;
    } catch (...) {
        cout << "出现异常了" << endl;
    }
    
    // 因为以上代码捕获了异常，所以这一句代码依然可以被执行
    cout << "哈哈，打印我，打印我！！！" << endl;
}

// MARK: - throw的用法

int divide(int v1, int v2) {
    // 除数不能为0，如果为0，则抛出一个异常
    // 可以使用关键字throw来抛出一个异常
    if (v2 == 0) throw "不能除以0";
    return v1 / v2;
}

void test2() {
    // throw异常后，会在当前函数中查找匹配的catch，找不到就终止当前函数代码，去上一层函数中查找。如果最终都找不到匹配的catch，整个程序就会终止
    // 所以函数divide抛出的异常，会在他的上一层函数，即在该函数中被捕获到
    try {
        int a = 10;
        int b = 0;
        int c = divide(a, b);
        cout << c << endl; // 因为异常捕获，这一句代码不会被执行
    } catch (const char *exception) {
        cout << exception << endl;
    }
    
    cout << "哈哈，打印我，打印我！！！" << endl;
}

// MARK: - 异常的抛出声明

/// 可能抛出任意可能的异常
void test3_1() {
    throw "随便啥异常";
}

/// 不抛出任何异常
void test3_2() throw() {
}

/// 只抛出int、double类型的异常
void test3_3() throw(int, double) {
}

void test3() {
    // 异常的抛出声明
    // 如果函数内部可能会抛出异常，可以声明一下异常类型，调用者可以以此确定要捕获的异常类型
    try {
        test3_1();
        test3_2();
        test3_3();
    } catch (const int &exception) {
        cout << exception << endl;
    } catch (const double &exception) {
        cout << exception << endl;
    } catch (...) {
        cout << "其他异常" << endl;
    }
}

// MARK: - 自定义异常类型

class Exception {
public:
    virtual string what() const = 0;
};

class DivideException: public Exception {
public:
    string what() const override {
        return "不能除以0";
    }
};

int divide2(int v1, int v2) throw (Exception) {
    if (v2 == 0) throw DivideException();
    return v1 / v2;
}

void test4() {
    try {
        int a = 10;
        int b = 0;
        int c = divide2(a, b);
        cout << c << endl;
    } catch (const Exception &exception) {
        cout << exception.what() << endl;
    }
}

// MARK: - 拦截所有类型的异常

void test5() {
    try {
        int a = 10;
        int b = 0;
        int c = divide(a, b);
        cout << c << endl;
    } catch (...) {
        // 使用 ... 可以拦截所有类型的异常
        cout << "出现异常了" << endl;
    }
}

// MARK: - 标准库异常(std)

// 可以参考网上的一篇文章：
// https://www.csdn.net/gather_29/NtjakgzsMTQtYmxvZwO0O0OO0O0O.html

void test6() {
    try {
        int *p = new int[99999999999999999]();
        cout << p << endl;
    } catch (const exception &e) {
        cout << e.what() << endl; // std::bad_alloc
    }
    
    try {
        int *p = new int[99999999999999999]();
        cout << p << endl;
    } catch (const std::bad_alloc &e) {
        cout << e.what() << endl; // std::bad_alloc
    }
    
    try {
        int *p = new int[99999999999999999]();
        cout << p << endl;
    } catch (const bad_alloc &e) {
        cout << e.what() << endl; // std::bad_alloc
    }
}

// MARK: - 测试函数

void test() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
}

int main(int argc, const char * argv[]) {
    
    test();
    
    return 0;
}
