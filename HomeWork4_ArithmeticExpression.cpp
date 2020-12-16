/*------------------------------------------------------------------------
利用栈求解算数表达式
1、先全部读进来
    不知道用string读还是cin慢慢读
        string可以全读进来在判断
        cin可以读正负号
2、判断是符号还是数字
------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>  //用一点点string
#include <iomanip> //输出格式控制头文件
using namespace std;

#if 1
/*------------------------------------------------------------------------
基本类型的通用链式栈
------------------------------------------------------------------------*/

//定义返回值
typedef int Status;
#define YES 1
#define NO 0
#define OK 1
#define ERROR 0
#define LOVERFLOW -1
#define INFEASIBLE -2
#define TRUE 1
#define FALSE 0

//节点
template <class T>
struct Node
{
    T element;                                      //元素 只要重定义了=的类型就可以使用
    Node<T> *Next;                                  //指针 指向下一个Node
    Node(Node<T> *ptr = NULL) { this->Next = ptr; } //空初始化和带一参初始化
    Node(const T &item, Node<T> *ptr = NULL)
    {
        this->element = item;
        this->Next = ptr;
    } //一参初始化和两参初始化
};

//栈
template <class T>
class LinkedStack
{
protected:
    Node<T> *top; //栈只需要一个表头即可
public:
    LinkedStack();
    ~LinkedStack() { MakeEmpty(); }
    void MakeEmpty();          //清空栈
    void Push(const T &x);     //入栈
    Status Pop(T &x);          //退栈
    Status GetTop(T &x) const; //取栈头
    Status IsEmpty() const;    //判断是否是空栈
    int GetSize() const;       //取战元素个数
    void DisplayStack();
};

/*------------------------------------------------------------------------
基本类型的通用链式栈的实现
------------------------------------------------------------------------*/
template <class T>
LinkedStack<T>::LinkedStack()
{
    //申请头结点
    this->top = new Node<T>;
    if (this->top == NULL)
        exit(LOVERFLOW);
    //初始化在Node中写过了
}
template <class T>
void LinkedStack<T>::MakeEmpty()
{
    Node<T> *p = this->top, *q;
    while (p)
    {
        q = p;
        p = p->Next;
        delete q;
    }
}
template <class T>
void LinkedStack<T>::Push(const T &x)
{
    Node<T> *s = new Node<T>(x);
    if (s == NULL)
    {
        exit(LOVERFLOW);
    }
    //连上
    s->Next = this->top->Next;
    this->top->Next = s;
}
template <class T>
Status LinkedStack<T>::Pop(T &x)
{
    //如果是空表 返回错误
    if (this->top == NULL || this->top->Next == NULL)
        return ERROR;
    //赋值
    x = this->top->Next->element;
    //删除节点
    Node<T> *p = this->top->Next;
    this->top->Next = p->Next;
    delete p;
    return OK;
}
template <class T>
Status LinkedStack<T>::GetTop(T &x) const
{
    if (this->top == NULL || this->top->Next == NULL)
        return ERROR;
    //赋值
    x = this->top->Next->element;
    return OK;
}
template <class T>
Status LinkedStack<T>::IsEmpty() const
{
    if (this->top == NULL || this->top->Next == NULL)
        return YES;
    else
        return NO;
}
template <class T>
int LinkedStack<T>::GetSize() const
{
    Node<T> *p = this->top->Next;
    int cnt = 0;
    while (p != NULL)
    {
        cnt++;
        p = p->Next;
    }
    return cnt;
}
template <class T>
void LinkedStack<T>::DisplayStack()
{
    Node<T> *p = this->top->Next;
    while (p)
    {
        if (p->Next == NULL)
        {
            printf("<%d>\n", p->element);
        }
        else
        {
            printf("<%d>-->", p->element);
        }
        p = p->Next;
    }
}

#endif

/*------------------------------------------------------------------------
实现
只对整数能进行操作  日后可以维护一下
主要考虑的问题是 1/-+-+2    +-+-2^4
乘方的负数次幂
规定到等号结束
------------------------------------------------------------------------*/

const char Order_OutStack[] = "=)+-*/%^("; //栈外次序
const char Order_InStack[] = "=(+-*/%^)";  //栈内次序

class CalculateInix
{
protected:
    LinkedStack<int> num;        //数栈
    LinkedStack<char> cha;       //符号栈
    string expression;           //储存表达式
    int cur_point;               //指向当前计算到的位置
    Status if_push(char coming); //判断符号c是否需要入栈
    Status calculate();          //把栈顶符号使用了
public:
    CalculateInix(string exp);
    Status solve();
};

//实现
CalculateInix::CalculateInix(string exp)
{
    cur_point = 0;
    //这里对前缀进行排重
    //打算通过加括号的方式实现把前缀表达式变成中缀表达式
    int i;
    int len = exp.length();
    if (exp[exp.length() - 1] != '=')
    {
        cout << "Must end with '='!" << endl;
        exit(ERROR);
    }
    for (i = 0; i < len; i++)
    {
        //先对两个相邻相同的符号进行操作
        if (i > 0 && !(exp[i] >= '0' || exp[i] <= '9') && (exp[i] == exp[i - 1] || exp[i] == exp[i - 1]))
        {
            cout << "Two duplicated " << exp[i] << " are not allowed!" << endl;
            exit(ERROR);
        }
    }
    //上面一步保证了 我遇见重复出现的符号的时候一定和前一个不同
    for (i = 0; exp[i] != '='; i++) //由于这里的len会实时变化 所以只能算
    {
        if ((!(exp[i] >= '0' && exp[i] <= '9')) && (i == 0 || (!(exp[i - 1] >= '0' && exp[i - 1] <= '9'))) && (exp[i] == '-' || exp[i] == '+') && exp[i - 1] != ')')
        {
            //遇到了重复的+-
            //看成前缀+-
            //没有重复的
            int cnt = 0;     //有多少个连续的+-+-
            int cnt_pas = 0; //有多少个-
            int k = i;       //下一个数值的位置
            while (k < len && !(exp[k] >= '0' && exp[k] <= '9'))
            {
                if (exp[k] == '-')
                {
                    cnt++;
                    cnt_pas++;
                }
                else if (exp[k] == '+')
                {
                    cnt++;
                }
                else
                {
                    cout << "Incorrect expression input (other symbols appear after +- sign)" << endl;
                    exit(ERROR);
                }
                k++;
            }
            //在后面寻找加')'的位置
            for (k = k + 1; k < len; k++) //k为数值之后的一个
            {
                if (!(exp[k] >= '0' && exp[k] <= '9') && exp[k] != '^')
                {
                    //如果是符号并且不为^
                    //退出
                    break;
                }
                //否则继续
            }
            if (k >= len)
            {
                exp.insert(len, ")"); //在最后插入')'
            }
            else
            {
                exp.insert(k, ")"); //在k指向的位置之前插入')'
            }

            //此时我要把这部分删除并加入括号
            //前文是在string的后面插入不会影响前面的位置 所以这之前len不维护也没事
            string temp = "(0";
            temp += (cnt_pas % 2 == 1 ? '-' : '+');
            exp.erase(i, cnt); //在i(第一次找到的位置)删除cnt个
            exp.insert(i, temp);

            //由于len变化了 维护一下
            len = exp.length();
        }
    }
    this->expression = exp;
    //cout<<exp<<endl;
}
Status CalculateInix::calculate()
{
    //由于都是双目运算符 所以除了括号都是需要两个数字的
    //括号不用本函数处理
    //前缀+-也做特殊处理
    //这里只对于+-*/%^做处理

    //先把数栈栈顶的两个元素取出来
    int x1, x2;
    int ans;
    char instack_char;
    if (this->num.Pop(x1) == ERROR)
    {
        //说明数栈栈顶没数 错误
        return ERROR;
    }
    if (this->num.Pop(x2) == ERROR)
    {
        //说明数栈栈顶没数 错误
        return ERROR;
    }
    if (this->cha.Pop(instack_char) == ERROR)
    {
        //说明符号栈栈顶没数 错误
        return ERROR;
    }

    //数、符号都取出来了
    switch (instack_char)
    {
    case ('+'):
    {
        ans = x1 + x2;
        break;
    }
    case ('-'):
    {
        //注意顺序
        ans = x2 - x1; //后入栈的是减数
        break;
    }
    case ('%'):
    {
        ans = x2 % x1;
        break;
    }
    case ('*'):
    {
        ans = x2 * x1;
        break;
    }
    case ('/'):
    {
        ans = x2 / x1;
        break;
    }
    case ('^'):
    {
        ans = 1;
        for (int i = 0; x1 >= 0 && i < x1; i++)
        {
            ans *= x2;
        }
        if (x1 < 0)
        {
            cout << "This procedure can only handle the power of natural Numbers!" << endl;
            return ERROR;
        }
        break;
    }
    case ('('):
    case (')'):
    case ('='):
    {
        cout << "Arithmetic expression error (parentheses or = have been evaluated)" << endl;
        return ERROR;
        //不需要break
    }
    default:
    {
        cout << "There is an incorrect sign in the stack!" << endl;
        return ERROR;
    }
    }
    //入栈
    this->num.Push(ans);
    return OK;
}
Status CalculateInix::if_push(char coming)
{
    //c的
    char instack;
    if (cha.GetTop(instack) == ERROR)
    {
        //如果符号栈是空的 那么入栈
        return YES;
    }
    //对+-  */进行特判
    if ((coming == '*' && instack == '/') || (coming == '/' && instack == '*') ||
        (coming == '+' && instack == '-') || (coming == '-' && instack == '+'))
    {
        return NO;
    }
    if (coming == ')' && instack == '(')
    {
        //如果是括号匹配了 也让进栈
        return YES;
    }
    int order_coming = strchr(Order_OutStack, coming) - Order_OutStack;
    int order_instack = strchr(Order_InStack, instack) - Order_InStack;
    if (order_coming > order_instack)
    {
        //如果coming优先级大于栈顶元素 那么就入栈
        return YES;
    }
    else
    {
        //如果coming小于等于栈顶元素 那么先算栈中的元素
        return NO;
    }
}
Status CalculateInix::solve()
{
    //肯定是一个个读取,是数字继续读直到遇见符号放入数栈。是符号判断1、是前缀还是中缀2、与栈顶元素比较
    int len = this->expression.length(); //把总长度记录一下
    while (this->cur_point < len)        //cur_point比表达式的常数小进入循环
    {

        if (expression[cur_point] >= '0' && expression[cur_point] <= '9') //如果是数字，把数字全部读完
        {
            //暂时不考虑前缀情况
            string temp_num;
            while (cur_point < len && expression[cur_point] >= '0' && expression[cur_point] <= '9')
            {
                temp_num += expression[cur_point];
                cur_point++;
            }
            num.Push(atoi(temp_num.c_str())); //数值入栈
        }
        else
        {
            //如果是符号
            switch (expression[cur_point])
            {
            case ('+'):
            {
                //如果flag=0即刚刚进入符号栈 那么我就认为这个+是前缀
                //但是还是要考虑优先级的  比如 -+-+-2^2应该不是4而是-4
                while (if_push('+') == NO)
                {
                    if (this->calculate() == ERROR)
                    {
                        cout << "Arithmetic expression error!" << endl;
                        return ERROR;
                    }
                }
                //到这里不会有错误 并且可以加入
                this->cha.Push('+'); //把+入栈
                break;
            }
            case ('-'):
            {
                while (if_push('-') == NO)
                {
                    if (this->calculate() == ERROR)
                    {
                        cout << "Arithmetic expression error!" << endl;
                        return ERROR;
                    }
                }
                //到这里不会有错误 并且可以加入
                this->cha.Push('-'); //把+入栈
                break;
            }
            case ('%'):
            {
                while (if_push('%') == NO)
                {
                    if (this->calculate() == ERROR)
                    {
                        cout << "Arithmetic expression error!" << endl;
                        return ERROR;
                    }
                }
                //到这里不会有错误 并且可以加入
                this->cha.Push('%'); //把+入栈
                break;
            }
            case ('*'):
            {
                while (if_push('*') == NO)
                {
                    if (this->calculate() == ERROR)
                    {
                        cout << "Arithmetic expression error!" << endl;
                        return ERROR;
                    }
                }
                //到这里不会有错误 并且可以加入
                this->cha.Push('*'); //把+入栈
                break;
            }
            case ('/'):
            {
                while (if_push('/') == NO)
                {
                    if (this->calculate() == ERROR)
                    {
                        cout << "Arithmetic expression error!" << endl;
                        return ERROR;
                    }
                }
                //到这里不会有错误 并且可以加入
                this->cha.Push('/'); //把+入栈
                break;
            }
            case ('^'):
            {
                while (if_push('^') == NO)
                {
                    if (this->calculate() == ERROR)
                    {
                        cout << "Arithmetic expression error!" << endl;
                        return ERROR;
                    }
                }
                //到这里不会有错误 并且可以加入
                this->cha.Push('^'); //把+入栈
                break;
            }
            case ('('):
            {
                while (if_push('(') == NO)
                {
                    if (this->calculate() == ERROR)
                    {
                        cout << "Arithmetic expression error!" << endl;
                        return ERROR;
                    }
                }
                //到这里不会有错误 并且可以加入
                this->cha.Push('('); //把+入栈
                break;
            }
            case (')'):
            {
                while (if_push(')') == NO)
                {

                    if (this->calculate() == ERROR)
                    {
                        cout << "Arithmetic expression error!" << endl;
                        return ERROR;
                    }
                }
                //此时应该是在栈中碰到  左括号 空栈  =
                char temp;
                this->cha.GetTop(temp);
                if (this->cha.IsEmpty() == YES || temp == '=')
                {
                    cout << "Incorrect expression input (no open parenthesis or equal sign not acceptable)" << endl;
                    return ERROR;
                }
                //左括号在头
                if (temp == '(')
                {
                    this->cha.Pop(temp); //左括号出栈 结束
                }
                else
                {
                    cout << "Strange error!" << endl;
                    return ERROR;
                }

                break;
            }
            case ('='):
            {
                //结束条件
                while (this->cha.IsEmpty() == NO)
                {
                    if (this->calculate() == ERROR)
                    {
                        cout << "Arithmetic expression error!" << endl;
                        return ERROR;
                    }
                }
                //符号运算结束
                if (this->num.GetSize() != 1)
                {
                    //如果栈中的数不是1个 有错误
                    cout << "Arithmetic expression error!" << endl;
                    return ERROR;
                }
                //运算成功
                int temp_num = 0;
                this->num.GetTop(temp_num);
                cout << temp_num << endl;
                return OK;
            }
            default:
            {
                cout << "Incorrect input sign!" << endl;
                return ERROR;
            }
            }
            cur_point++;
        }
    }
    return OK;
}

/*
-2*(3+5)+2^3/4=
2^4/8-(+2+8)%3=

*/

//测试
int main()
{
    string exp;
    string op;
    int loop = 1;
    while (loop)
    {
        cout << "Please enter an expression:" << endl;
        cin >> exp;
        CalculateInix ci(exp);
        ci.solve();
        while (getchar() != '\n')
            ;
        while (1)
        {
            cout << "Do you want to retype(y/n): ";
            cin >> op;
            if (op == "n" || op == "N")
            {
                cout << "quit successfully!" << endl;
                loop = 0;
                break;
            }
            else if (op == "y" || op == "Y")
            {
                while (getchar() != '\n')
                    ;
                break;
            }
            else
            {
                cout << "enter error!Please retype." << endl;
                while (getchar() != '\n')
                    ;
            }
        }
    }
    return 0;
}
