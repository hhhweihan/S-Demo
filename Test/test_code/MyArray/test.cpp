#include<iostream>
#include"MyArray.h"

using namespace std;

int main(){

    MyArray arr1;
    cout << "数组的容量为：" << arr1.getCapacity() << endl;
    cout << "数组的大小为：" << arr1.getSize() << endl;

    return 0;
}
