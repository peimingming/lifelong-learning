//
//  main.cpp
//  SmartPointer
//
//  Created by 裴明明 on 2020/2/2.
//  Copyright © 2020 裴明明. All rights reserved.
//

#include <iostream>
using namespace std;

// MARK: - 传统指针和智能指针差异对比

// 传统指针存在的问题：
// 1.需要手动管理内存
// 2.容易发生内存泄露(忘记释放、出现异常等)
// 3.释放之后产生野指针

// 智能指针就是为了解决传统指针存在的问题
// 特点：
// 1.自动管理内存
// 2.只能使用匿名对象或者另外一个智能指针进行初始化
// 有以下几种：
// auto_ptr (已经被弃掉，不建议使用，并且并不适用于数组类型)
// shared_ptr
// weak_ptr
// unique_ptr

class Person {
public:
    Person() {
        cout << "Person:Person()" << endl;
    }
    ~Person() {
        cout << "Person:~Person()" << endl;
    }
    void run() {
        cout << "Person::run()" << endl;
    }
};

// MARK: - auto_ptr(deprecated, 而且并不适用于数组)

void test1() {
    // 打开下边这段代码会出现一个编译警告：'auto_ptr<Person>' is deprecated
//    auto_ptr<Person> p1(new Person());
//    p1->run();
    /*
     该函数调用完成后会自动调用p1指针所指向的对象的析构函数
     Person:Person()
     Person::run()
     Person:~Person()
     */
    
    // 打开下边这段代码会出现一个编译报错：No matching constructor for initialization of 'auto_ptr<Person>''
    // 也说明：auto_ptr不支持数组类型
//    auto_ptr<Person> people2(new Person[3]{}, default_delete<Person[]>());
    
    // 打开下边代码，运行，程序会直接崩溃，原因是：对象xm会在该函数调用完成后进行析构，而auto_ptr内部会进行再次delete对象指针
    // 所以：智能指针的初始化只能使用上边的匿名对象
//    Person xm;
//    auto_ptr<Person> p1(&xm);
}

// MARK: - 智能指针的简单自实现

template <typename T>
class SmartPointer {
    T *pointer;
public:
    SmartPointer(T *pointer) :pointer(pointer) { }
    ~SmartPointer() {
        if (pointer == nullptr) return;
        delete pointer;
    }
    
    // 重写运算符: ->
    T *operator->() {
        return pointer;
    }
};

void test2() {
    SmartPointer<Person> p(new Person());
    p->run();
}

// MARK: - shared_ptr

// 特点：
// 多个shared_ptr可以指向同一个对象，当最后一个shared_ptr在作用域范围内结束时，对象才会被自动释放

void test3() {
    shared_ptr<Person> p1(new Person());
    shared_ptr<Person> p2 = p1;
    shared_ptr<Person> p3(p2);
    cout << p1.use_count() << endl; // 3
    
    // 针对数组的用法
    shared_ptr<Person> people1(new Person[2]{}, [](Person *p) { delete[] p; });
    shared_ptr<Person> people2(new Person[3]{}, default_delete<Person[]>());
    
    /*
     输出(6次构造函数调用，6次析构函数调用):
     Person:Person()
     3
     Person:Person()
     Person:Person()
     Person:Person()
     Person:Person()
     Person:Person()
     Person:~Person()
     Person:~Person()
     Person:~Person()
     Person:~Person()
     Person:~Person()
     Person:~Person()
     */
}

// MARK: - weak_ptr

// 特点：会对一个对象产生弱引用，不会计算引用计数
// 作用：解决使用强指针可能会产生强引用的问题

class Pet;

// 以下两个类有可能导致循环引用问题
class Master {
public:
    shared_ptr<Pet> pet = nullptr;
    Master() {
        cout << "Master:Master()" << endl;
    }
    ~Master() {
        cout << "Master:~Master()" << endl;
    }
};
class Pet {
public:
    shared_ptr<Master> master = nullptr;
    Pet() {
        cout << "Pet:Pet()" << endl;
    }
    ~Pet() {
        cout << "Pet:~Pet()" << endl;
    }
};

void test4() {
    // 下边的代码会产生 循环引用
    // 原因：
    // 对象master和pet是该函数的局部指针变量，存储在栈空间中，并分别指向堆空间中的类型为Master和Pet的对象
    // 然而，堆空间中的类型为Master和Pet的对象又分别持有了对方
    // 当该函数调用完成的时候，两个对象的引用计数都不为0
    // 所以，因为循环引用，两个对象都不会被自动释放，造成了内存泄露
    // 解决方案：
    // 将其中一个类中的强指针换成弱指针(weak_ptr)
    shared_ptr<Master> master(new Master());
    shared_ptr<Pet> pet(new Pet());
    master->pet = pet;
    pet->master = master;
}

// 以下两个类因为其中一个类中的成员指针变量使用了弱指针，不会产生循环引用
class Student;
class Teacher {
public:
    weak_ptr<Student> student;
    Teacher() {
        cout << "Teacher:Teacher()" << endl;
    }
    ~Teacher() {
        cout << "Teacher:~Teacher()" << endl;
    }
};
class Student {
public:
    shared_ptr<Teacher> teacher = nullptr;
    Student() {
        cout << "Student:Student()" << endl;
    }
    ~Student() {
        cout << "Student:~Student()" << endl;
    }
};

void test5() {
    shared_ptr<Teacher> teacher(new Teacher());
    shared_ptr<Student> student(new Student());
    teacher->student = student;
    student->teacher = teacher;
    
    /*
     输出(2次构造函数调用，2次析构函数调用):
     Teacher:Teacher()
     Student:Student()
     Student:~Student()
     Teacher:~Teacher()
     */
}

// MARK: - unique_ptr

// 特点：
// 强指针，不过它可以确保同一时间只有1个指针指向对象

void test6() {
    unique_ptr<Person> p1(new Person());
    // 可以使用std::move函数转移unique_ptr的所有权
    unique_ptr<Person> p2 = move(p1);
    cout << p1 << endl;
    cout << p2 << endl;
}

// MARK: - 测试代码

void test() {
    // 放开下边的测试代码进行测试
    test1();
    //test2();
    //test3();
    //test4();
    //test5();
    //test6();
}

int main(int argc, const char * argv[]) {
    
    test();
    
    return 0;
}
