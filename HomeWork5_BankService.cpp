/*------------------------------------------------------------------------
银行业务
emm应该比较简单
------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>   //用一点点string
#include <iomanip>  //输出格式控制头文件
using namespace std;

/*------------------------------------------------------------------------
队列的实现
------------------------------------------------------------------------*/
#if 1
//定义数组链表的最大存储空间
const int MaxSize=50;
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
template<class T>
struct Node{        
    T element;  //元素 只要重定义了=的类型就可以使用
    Node<T>* Next;  //指针 指向下一个Node
    Node(Node<T>* ptr=NULL){this->Next=ptr;}    //空初始化和带一参初始化
    Node(const T& item,Node<T>* ptr=NULL){this->element=item;this->Next=ptr;}   //一参初始化和两参初始化
};

//队列
template<class T>
class LinkedQueue{
protected:
    Node<T>* head;
    Node<T>* end;
public:
    LinkedQueue();
    ~LinkedQueue(){makeEmpty();}
    void makeEmpty();           //清空队列
    Status EnQueue(const T& x); //元素x进入队列
    Status DeQueue(T& x);     //队头元素出队列
    Status GetFront(T& x);    //读取队头元素的值
    Status IsEmpty()const;    //判断队列是否为空
    int GetSize()const;        //求队列元素个数
};

//实现
template<class T>
LinkedQueue<T>::LinkedQueue()
{
    Node<T> *s=new Node<T>;  //申请头结点 next为null
    if(s==NULL)exit(LOVERFLOW); //如果申请不成功，退出
    this->head=s;   //头指针指向s
    this->end=s;    //尾指针指向s
}
template<class T>
void LinkedQueue<T>::makeEmpty()
{
    Node<T>* p=this->head,*q;
    while(p!=NULL)
    {
        q=p;
        p=p->Next;
        delete q;
    }
}
template<class T>
Status LinkedQueue<T>::EnQueue(const T& x)
{
    //加入到尾结点之后
    Node<T>* s=new Node<T>(x); //申请一个新节点,值为x，next为null
    if(s==NULL)exit(LOVERFLOW); //如果申请不成功，退出
    this->end->Next=s;
    this->end=s;
    return OK;
}
template<class T>
Status LinkedQueue<T>::DeQueue(T& x)
{
    if(this->IsEmpty()==YES)return ERROR;
    //如果非空 删除第一个节点
    Node<T> *p=this->head,*q=p->Next;
    x=q->element;
    p->Next=q->Next;
    delete q;
    return OK;
}
template<class T>
Status LinkedQueue<T>::GetFront(T& x)
{
    if(this->IsEmpty()==YES)return ERROR;
    //非空 返回第一个节点的值
    x=this->head->Next->element;
    return OK;
}
template<class T>
Status LinkedQueue<T>::IsEmpty()const
{
    if(this->head->Next==NULL)return YES;
    else {return NO;}
}
template<class T>
int LinkedQueue<T>::GetSize()const
{
    Node<T> *p=this->head->Next;
    int cnt=0;
    while(p!=NULL)
    {
        cnt++;
        p=p->Next;
    }
    return cnt;
}

#endif

/*------------------------------------------------------------------------
公共函数
------------------------------------------------------------------------*/
//正确输入一个非负整数
void safe_cin_positive_integer(int& x, const char* tips,int toplimit=0x7fffffff)    //默认上限为int最大值
{
    cout << tips;
    while (1)    //考虑一部分输入错误问题
    {
        if (scanf("%d", &x) != 1)
        {
            cout << "Input error please retype!" << endl;
            getchar();
            while (getchar() != '\n');
        }
        else if (x <= 0)
        {
            cout << "Input error please retype!" << endl;
        }
        else if(x>toplimit)
        {
            cout << "Enter more than the upper! (Limit " << toplimit << " ) please reenter!" << endl;
        }
        else
        {
            break;
        }
        cout << tips;
    }
}

/*------------------------------------------------------------------------
银行业务系统的实现
------------------------------------------------------------------------*/
//定义出栈比例
const int output_A=2;
const int output_B=1;
const int priority_AB=1;   //1代表A 2代表B

//定义输入的最长
const int input_length_max=1024;

//声明
class BankService{
protected:
    LinkedQueue<int> A;
    LinkedQueue<int> B;
    int n;
    void solve();
public:
    BankService();
    ~BankService(){}
};

//实现
BankService::BankService()
{
    safe_cin_positive_integer(this->n, "Please enter total number of customers (<=1000):", 1000);
    for(int i=1;i<=n;i++)
    {
        char question[input_length_max];
        int no_clint;
        sprintf(question, "Please enter the number of %d customer ", i);
        safe_cin_positive_integer(no_clint,question,1000);
        if(no_clint%2==1)
        {
            //编号为奇数 入队列A
            A.EnQueue(no_clint);
        }
        else
        {
            //编号为偶数 入队列B
            B.EnQueue(no_clint);
        }
    }
    //输入结束 开始处理
    this->solve();
}
void BankService::solve()
{
    int top;
    cout << "The output customer number is: " << endl;
    while(A.IsEmpty()==NO&&B.IsEmpty()==NO)
    {
        //两个队列都不为空的时候开始执行操作
        if(priority_AB==1)
        {
            //先输出A
            for (int i = 0; A.IsEmpty()==NO&&i < output_A; i++)
            {
                A.DeQueue(top);
                cout<<top<<" ";
            }
            for (int i = 0; B.IsEmpty()==NO&&i < output_B; i++)
            {
                B.DeQueue(top);
                cout<<top<<" ";
            }
        }
        else
        {
            //先输出B
            for (int i = 0; B.IsEmpty()==NO&&i < output_B; i++)
            {
                B.DeQueue(top);
                cout<<top<<" ";
            }
            for (int i = 0; A.IsEmpty()==NO&&i < output_A; i++)
            {
                A.DeQueue(top);
                cout<<top<<" ";
            }
        }
        
        
    }
    while(A.IsEmpty()==NO)
    {
        //A不为空 出队列
        A.DeQueue(top);
        cout<<top<<" ";
    }
    while(B.IsEmpty()==NO)
    {
        //B不为空 出队列
        B.DeQueue(top);
        cout<<top<<" ";
    }
    cout<<endl;
    return;
}


int main()
{
    BankService BS;
    return 0;
}

