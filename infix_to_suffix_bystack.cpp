/*
利用栈来把中缀转换为后缀
*/

#include <iostream>
#include <stack>
using namespace std;

/*------------------------------------------------------------
规定target 以#开头 以#结尾
暂时只考虑字母+符号组合
暂时只考虑中缀，不考虑前缀后缀还有取负号
------------------------------------------------------------*/
void postfix(string target) 
{
    stack<char> num_stack,cha_stack;  //数(这里先用负号表示)栈、符号栈
    
}