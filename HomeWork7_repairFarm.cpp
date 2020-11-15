#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>   //用一点点string
#include <iomanip>  //输出格式控制头文件
using namespace std;

//堆的最大容积
const int maxn=10005;
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

/*小根堆*/
template<class T>
class MinHeap{
protected:
    T *heap;
    int volumn;
    int maxsize;
    void filterUp(int start);
    void siftDown(int start,int end);
public:
    MinHeap(int V=maxn);
    MinHeap(T arr[],int n);
    ~MinHeap();
    Status push(const T &x);    //插入一个
    Status pop(T &x);           //堆顶出堆
    Status top(T &x);           //取堆顶
    Status is_empty();
};

/*实现*/
template<class T>
MinHeap<T>::MinHeap(int V)
{
    heap=new T[V];
    if(heap==NULL)exit(-1);
    this->maxsize=V;
    this->volumn=0;
}
template<class T>
MinHeap<T>::MinHeap(T arr[],int n)
{
    this->maxsize=maxn<n?n:maxn;
    this->volumn=n;
    this->heap=new T[maxsize];
    if(this->heap==NULL)exit(-1);    //退出
    for(int i=0;i<n;i++)heap[i]=arr[i]; //赋值
    //进行排序
    int curpos=(n-2)/2; //找到第一个最下面的父亲节点
    while(curpos>=0)
    {
        siftDown(curpos,n-1);   //下落
        curpos--;       //向前一个节点
    }
    
}
template<class T>
MinHeap<T>::~MinHeap()
{
    //析构
    delete []this->heap;
}
template<class T>
void MinHeap<T>::siftDown(int start,int end)
{
    int i=start,j=2*i+1;
    T temp=heap[i];
    while(j<=end)
    {
        if(j<end&&heap[j]>heap[j+1])j++;
        if(temp<=heap[j])break; //结束
        else
        {
            //交换
            heap[i]=heap[j];
            i=j;
            j=i*2+1;
        }
    }
    heap[i]=temp;   //放回
}
template<class T>
void MinHeap<T>::filterUp(int start)
{
    //从current开始到0结束
    //从末尾插入，直到父亲节点小于它或者到根节点
    int j=start,i=(j-1)/2;
    T temp=heap[j];
    while(j>0)
    {
        if(heap[i]<=temp)break;
        else
        {
            heap[j]=heap[i];
            j=i;
            i=(j-1)/2;
        }
    }
    heap[j]=temp;
}
template<class T>
Status MinHeap<T>::push(const T &x)
{
    if(this->volumn==maxsize)return LOVERFLOW;
    heap[volumn]=x;
    this->filterUp(volumn);
    volumn++;
    return OK;
}
template<class T>
Status MinHeap<T>::pop(T &x)
{
    if(volumn==0)return ERROR;
    x=heap[0];heap[0]=heap[volumn-1];
    volumn--;
    siftDown(0,volumn-1);
    return OK;
}
template<class T>
Status MinHeap<T>::top(T &x)
{
    if(volumn==0)return ERROR;
    x=heap[0];
    return OK;
}
template<class T>
Status MinHeap<T>::is_empty()
{
    if(this->volumn==0)return YES;
    else return NO;
}


/*------------------------------------------------------------------------
函数名  :safe_cin_positive_integer
功能    :安全输入一个正整数
参数    :x 需要输入的变量
        tips 问题提示语
------------------------------------------------------------------------*/
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
            cout << "Input above upper limit (Limit " << toplimit << " ) please retype!" << endl;
        }
        else
        {
            break;
        }
        cout << tips;
    }
}


int len[maxn];
//8 4 5 1 2 1 3 1 1 
int main()
{
    MinHeap<int> hp;
    int n;
    int sum=0;
    safe_cin_positive_integer(n, "Please enter total N(<=10000)", 10000);
    for(int i=0;i<n;i++)
    {
        cin>>len[i];
        hp.push(len[i]);
    }
    for(int i=0;i<n-1;i++)
    {
        int first,second;
        hp.pop(first);
        if(hp.is_empty()==YES)break;
        hp.pop(second);
        first=first+second;
        hp.push(first);
        sum+=first;
    }
    if(n==1)
    {
        cout<<len[0]<<endl;
    }
    else
    {
        cout<<sum<<endl;
    }
    
    return 0;
}












