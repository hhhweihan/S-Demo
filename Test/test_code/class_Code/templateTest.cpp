#include<iostream>
using namespace std;

template<class NAMETYPE, class AGETYPE>
class Person{
public:
    
    Person(NAMETYPE name, AGETYPE age){
        this->m_Name = name;
        this->m_Age = age;
    }

    void showPerson(){
        cout << "name: " << this->m_Name << "age: " << this->m_Age << endl;
    }

    NAMETYPE m_Name;
    AGETYPE m_Age;
}

int main(){

    Person<string, int> p1("Tom", 20);
    p1.showPerson();

    return 0;
}
