#include<iostream>
#include"Student.h"

using namespace std;

// 编写一个学生类，创建学生对象，使用学生对象
int main(){
    
    // 创建学生对象
    Student s1;
    
    // 使用学生对象
    s1.m_Name = "zs";
    s1.m_Age = 20;
    s1.m_Id = 1001;

    cout << s1.m_Name << "\t" << s1.m_Age << "\t" << s1.m_Id << endl;

    // 调用对象的方法
    s1.sleep();
    s1.eat();
    s1.study();

    // 动态创建
    Student * p_stu = new Student;
    

    return 0;
}
