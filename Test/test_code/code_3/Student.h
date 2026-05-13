#pragma once

#include<string>
#include<iostream>

using namespace std;


// 声明一个类使用class 关键字，后面跟上类名
class Student{

public:
    string m_Name;    // 姓名
    int m_Age;    // 年龄
    int m_Id;   // 学号

    // 成员方法
    void sleep();   // 睡觉的方法
    void eat(); // 吃饭的方法
    void study();   //学习的方法
};
