# 脚本语言设计说明
该脚本语言以函数为单位进行执行，函数以提供的语句行为基础，可以自由组合,并支持函数调用
***
##  语句
语句为函数的组成部分，不同的语句组合组成了函数，函数之间相互调用使整个脚本按预期运行
### let valuable_name valuable
定义变量，支持string以及double类型    
### add/sub/div/mul $valuable_name valuable
对指定变量进行数字运算，并赋值
### get
接受用户输入
### reply
向用户输出
可以使用$valuable_name或""字符串，每个单位之间使用+连接
### if_eqaul valuable1 valuable2 (expression)
如果变量1和变量2相等，执行expression
***
## 函数定义
函数为本脚本语言的基本执行单位，每个函数可以通过fun "函数名"来进行调用
函数定义语法为：
```
func 函数名
    ...
endf
其中...表示若干原语或其他函数调用
```
调用格式为：
```c++
call 函数名
```
函数嵌套以栈的形式实现，具体使用运行environment表示栈