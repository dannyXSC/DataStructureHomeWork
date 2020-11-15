#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>   //用一点点string
#include <iomanip>  //输出格式控制头文件
using namespace std;

typedef struct Node{
    int ele;
    Node* next;
}*NodePointer;

class List{
private:
    NodePointer head;
    NodePointer end;
    int ele_num;
    void add_append(int x); //尾部添加
public:
    List();
    ~List();
    friend istream& operator>>(istream& in,List& l);        //需要排重 输入时保证不会有重复元素 不允许多次输入
    friend ostream& operator<<(ostream& out,const List& l);
    void intersection(const List& l);   //实现交集 把本表
};

List::List()
{
    //头
    this->head=new Node;
    if(this->head==NULL)
    {
        exit(-1);
    }
    this->head->next=NULL;
    //尾
    this->end=this->head;
    this->ele_num=0;
}
List::~List()
{
    NodePointer p=this->head,q;
    while(p!=NULL)
    {
        q=p;
        p=p->next;
        delete q;
    }
}
void List::add_append(int x)
{
    NodePointer s=new Node;
    if(s==NULL)exit(-1);
    s->ele=x;
    s->next=NULL;
    this->end->next=s;
    this->end=s;
    this->ele_num++;
}
istream& operator>>(istream& in,List& l)
{
    int t,old_t=-1;
    while(scanf("%d",&t)==1&&t!=-1)
    {
        if(t!=old_t)
        {
            l.add_append(t);
            old_t=t;
        }
    }
    return in;
}
ostream& operator<<(ostream& out,const List& l)
{
    NodePointer p=l.head->next;
    if(p==NULL)
    {
        out<<"NULL"<<endl;
        return out;
    }
    while(p!=NULL)
    {
        out<<p->ele<<" ";
        p=p->next;
    }
    out<<endl;
    return out;
}
void List::intersection(const List& l)
{
    //已知各个链表都是非递减、不重复
    NodePointer p=this->head,q=l.head->next,t;    //p指向主链表上比较节点的前一个节点 q指向比较链表上的本节点
    l.head->next=NULL;  //关闭这个链表
    while(!(p->next==NULL||q==NULL))
    {
        if(p->next->ele==q->ele)
        {
            //说明p的下一个节点是交集之后的节点
            p=p->next;  //保存下这个节点
            //删除q节点
            t=q;
            q=q->next;
            delete t;
        }
        else if(p->next->ele<q->ele)
        {
            //p->next中的节点无效
            //删除p->next
            t=p->next;
            p->next=t->next;
            delete t;
            //q节点不用动
        }
        else
        {
            //q中节点无效
            t=q;
            q=q->next;
            delete t;
        }
    }
    if(p->next!=NULL)
    {
        t=p;
        p=p->next;
        t->next=NULL;
        while(p)
        {
            t=p;
            p=p->next;
            delete t;
        }
    }
    else
    {
        while(q)
        {
            t=q;
            q=q->next;
            delete t;
        }
    }
    
}


int main()
{
    List l1,l2;
    cout << "Please enter the first set (non-decrement, repeatable): " << endl;
    cin>>l1;
    cout << "Please enter the second set (non-decrement, repeatable): " << endl;
    cin>>l2;
    l1.intersection(l2);
    cout << "The set that takes the intersection is: " << endl;
    cout<<l1;
    return 0;
}