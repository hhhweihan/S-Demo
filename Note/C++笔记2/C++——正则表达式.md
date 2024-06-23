# C++——正则表达式（仅了解）

## 正则表达式的定义与组成

### 定义

- 正则表达式是一种描述字符序列的方法，是一种极其强大的计算工具

### 表格

| regex           | 表示有一个正则表达式的类                                     |
| --------------- | ------------------------------------------------------------ |
| regex_match     | 讲一个字符序列与一个正则表达式匹配                           |
| regex_search    | 寻找第一个与正则表达式匹配的子序列                           |
| regex_replace   | 使用给定格式替换一个正则表达式                               |
| sregex_iterator | 迭代器适配器，调用regex_search来遍历一个string中所有匹配的子串 |
| smatch          | 容器类，保存在string中搜索的结果                             |
| ssub_match      | string中匹配的子表达式的结果                                 |

## 正则表达式组件的使用

```c++
string pattern("[^c]ei");
pattern="[[:alpha:]]*"+pattern+"[[:alpha":]]*"
//这里pattern的意思是，我们利用[[:alpha:]]去匹配任意字符
regex r(pattern);//构造一个用于查找模式的regex
smatch results;。。定义一个对象保存搜索结果
string text_str="receipt friend theif receive"；//用r在test——str中查找与pattern匹配的子串
if(regex_search(test_str,results,r))cout<<results.str()<<endl;//如果有匹配子串，打印匹配的单词
```

