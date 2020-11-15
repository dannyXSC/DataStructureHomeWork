/*------------------------------------------------------------------------
最短路径
利用并查集
------------------------------------------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>   //用一点点string
#include <iomanip>  //输出格式控制头文件
using namespace std;

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

/*------------------------------------------------------------------------
函数名  :safe_cin_positive_integer
功能    :安全输入一个正整数
参数    :x 需要输入的变量
        tips 问题提示语
------------------------------------------------------------------------*/
void safe_cin_positive_integer(int &x, const char *tips, int toplimit = 0x7fffffff) //默认上限为int最大值
{
    cout << tips;
    while (1) //考虑一部分输入错误问题
    {
        if (scanf("%d", &x) != 1)
        {
            cout << "Input error please retype!" << endl;
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (x <= 0)
        {
            cout << "Input error please retype!" << endl;
        }
        else if (x > toplimit)
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

struct Node{
    string name;    //保存自己的名字
    int root;       //根节点
    int cost;       //总花费(根节点才有效)
    Node():name(""),root(-1),cost(0){}
    Node(string n,int r):name("n"),root(r),cost(0){}
};
struct Edge{
    string name1;
    string name2;
    int value;
    Edge():name1(""),name2{""},value(0){}
    Edge(const Edge& e){this->name1=e.name1;this->name2=e.name2;this->value=e.value;}
    void operator=(const Edge& e){this->name1=e.name1;this->name2=e.name2;this->value=e.value;}
};

class CostSystem{
protected:
    Node* sections; //存顶点的数组
    Edge* edges;
    int n;
    int n_edges;
    int find(string name);  //返回name对应的位置
    void Sort_Edges();
    int find_parent(int no);
public:
    CostSystem();
    
};

int CostSystem::find_parent(int no)
{
    return sections[no].root==no?no:(sections[no].root=find_parent(sections[no].root));
}

int CostSystem::find(string name)
{
    for(int i=0;i<this->n;i++)
    {
        if(this->sections[i].name==name)
            return i;
    }
    return -1;
}

void CostSystem::Sort_Edges()
{
    for(int i=0;i<n_edges-1;i++)
    {
        for(int j=n_edges-1;j>i;j--)
        {
            if(this->edges[j].value<this->edges[j-1].value)
            {
                //交换
                Edge temp=edges[j];
                edges[j]=edges[j-1];
                edges[j-1]=temp;
            }
        }
    }
    return;
}

CostSystem::CostSystem()
{
    safe_cin_positive_integer(this->n, "Please enter number of vertices: ");
    cout << "Please enter the name of each vertex: " << endl;
    this->sections=new Node[this->n];
    this->edges=new Edge[n*n];
    for(int i=0;i<this->n;i++)
    {
        cin>>this->sections[i].name;    //输入姓名
        this->sections[i].root=i;       //初始化根节点
    }
    string e1,e2;
    int v;
    int i=0;
    while(1)
    {
        //输入edges
        cin>>e1>>e2>>v;
        if(e1=="?"&&e2=="?"&&v==0)
        {
            //结束条件
            break;
        }
        edges[i].name1=e1;
        edges[i].name2=e2;    
        edges[i].value=v;
        i++;
    }
    this->n_edges=i;
    //edges按value排序
    this->Sort_Edges();

    i=0;        //记录有效边数量
    int cnt=0;  //循环变量
    while(i<this->n-1)
    {
        int no1=find(edges[cnt].name1);
        int no2=find(edges[cnt].name2);
        int temp_root=find_parent(no1);
        int temp_children=find_parent(no2);
        if(temp_root==temp_children)
        {
            //成环
            cnt++;
            continue;
        }
        //如果不成环 链接
        sections[no2].root=temp_root;
        sections[temp_root].cost+=sections[temp_children].cost;
        //把值加上
        sections[temp_root].cost+=edges[cnt].value;
        cout<<sections[temp_root].cost<<"*"<<endl;
        cnt++;
        i++;
    }
    cout<<sections[find_parent(0)].cost<<endl;
}


/*
4
a b c d
a b 8
b c 7
c d 5
d a 11
a c 18
b d 12
? ? 0

*/

int main()
{
    CostSystem cs;
    return 0;
}











