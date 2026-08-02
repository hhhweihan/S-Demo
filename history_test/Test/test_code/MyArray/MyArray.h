#pragma once

class MyArray{
public:
    MyArray();  //无参的构造
    MyArray(int capacity);  //有参构造
    MyArray(const MyArray& arr);    //拷贝构造
    ~MyArray(); //析构函数

    void pushBack(int data);    // 添加数据
    int getData(int position);  // 获取指定位置处的数据
    void setData(int position, int data);   //修改指定位置处的数据
    
    int getCapacity(){
        return m_Capacity;
    }

    int getSize(){
        return m_Size;
    }
private:
    int m_Capacity; //容量
    int m_Size; //大小
    int *pAddress;  //底层维护的堆区的数组
};
