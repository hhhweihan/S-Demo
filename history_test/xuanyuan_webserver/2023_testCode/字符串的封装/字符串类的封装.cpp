#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "MyString.h"
using namespace std;

//测试 MyString
void test01() {
	MyString str = "abc";

	cout << str << endl;

	/*
	* cout << "请输入str新的内容： " << endl;

	cin >> str;

	cout << "新内容为：" << str << endl;
	*/
	MyString str2(str);

	MyString str3 = "";

	str3 = str2;
	str3 = "aaaa";

	cout << "str3= " << str3 << endl;

	str3[0] = 'w';

	cout << "str3 第一个位置为 = " << str3[0] << endl;

	MyString str4 = "";
	str4 = str2 + str3;	//字符串拼接

	cout << "str4为 " << str4 << endl;

	if (str3 == str4) {
		cout << "str3与str4相等" << endl;
	}
	else {
		cout << "str3与str4不相等" << endl;
	}
	//int a = 10;
	//cin >> a;
	//cout << "a= " << a << endl;
}

int main(int argc, char const* argv[]) {

	test01();

	system("pause");
	return EXIT_SUCCESS;
}