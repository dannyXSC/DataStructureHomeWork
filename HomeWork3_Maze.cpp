/*------------------------------------------------------------------------
有深度优先和广度优先两种方法
采用广度优先求得最短路径
------------------------------------------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>   //用一点点string
#include <iomanip>  //输出格式控制头文件
using namespace std;

#if 1
/*------------------------------------------------------------------------
队列的抽象数据类型的实现
------------------------------------------------------------------------*/

//定义数组链表的最大存储空间
const int MaxSize = 50;
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
基本类型的通用链式队列
------------------------------------------------------------------------*/

//节点
template<class T>
struct Node {
    T element;  //元素 只要重定义了=的类型就可以使用
    Node<T>* Next;  //指针 指向下一个Node
    Node(Node<T>* ptr = NULL) { this->Next = ptr; }    //空初始化和带一参初始化
    Node(const T& item, Node<T>* ptr = NULL) { this->element = item; this->Next = ptr; }   //一参初始化和两参初始化
};

//队列
template<class T>
class LinkedQueue {
protected:
    Node<T>* head;
    Node<T>* end;
public:
    LinkedQueue();
    ~LinkedQueue() { makeEmpty(); }
    void makeEmpty();           //清空队列
    Status EnQueue(const T& x); //元素x进入队列
    Status DeQueue(T& x);     //队头元素出队列
    Status GetFront(T& x);    //读取队头元素的值
    Status IsEmpty()const;    //判断队列是否为空
    int GetSize()const;        //求队列元素个数
};
/*------------------------------------------------------------------------
基本类型的通用链表的实现
------------------------------------------------------------------------*/
template<class T>
LinkedQueue<T>::LinkedQueue()
{
    Node<T>* s = new Node<T>;  //申请头结点 next为null
    if (s == NULL)exit(LOVERFLOW); //如果申请不成功，退出
    this->head = s;   //头指针指向s
    this->end = s;    //尾指针指向s
}
template<class T>
void LinkedQueue<T>::makeEmpty()
{
    Node<T>* p = this->head, *q;
    while (p != NULL)
    {
        q = p;
        p = p->Next;
        delete q;
    }
    return;
}
template<class T>
Status LinkedQueue<T>::EnQueue(const T& x)
{
    //加入到尾结点之后
    Node<T>* s = new Node<T>(x); //申请一个新节点,值为x，next为null
    if (s == NULL)exit(LOVERFLOW); //如果申请不成功，退出
    this->end->Next = s;
    this->end = s;
    return OK;
}
template<class T>
Status LinkedQueue<T>::DeQueue(T& x)
{
    if (this->IsEmpty() == YES)return ERROR;
     //维护尾结点
    if(this->end==this->head->Next)
    {
        this->end=this->head;
    }
    //如果非空 删除第一个节点
    Node<T>* p = this->head, *q = p->Next;
    x = q->element;
    p->Next = q->Next;
    delete q;
    return OK;
}
template<class T>
Status LinkedQueue<T>::GetFront(T& x)
{
    if (this->IsEmpty() == YES)return ERROR;
    //非空 返回第一个节点的值
    x = this->head->Next->element;
    return OK;
}
template<class T>
Status LinkedQueue<T>::IsEmpty()const
{
    if (this->head->Next == NULL)return YES;
    else { return NO; }
}
template<class T>
int LinkedQueue<T>::GetSize()const
{
    Node<T>* p = this->head->Next;
    int cnt = 0;
    while (p != NULL)
    {
        cnt++;
        p = p->Next;
    }
    return cnt;
}

#endif

/*------------------------------------------------------------------------
函数名  :wait_for_enter
功能    :等待输入回车以结束
参数    :
------------------------------------------------------------------------*/
void wait_for_enter()
{
    cout << "Please type enter to continue..." << endl;
    if(getchar()=='\n')return;
    while (getchar() != '\n')
        ;
}

/*------------------------------------------------------------------------
实现迷宫界面和算法
------------------------------------------------------------------------*/
/*定义储存迷宫墙和非墙的类型*/
typedef int ELEMENT;
//这里用1,0表示有墙和无墙
#define map_have_barrier 1
#define map_no_barrier 0
#define map_path 2

/*定义符号*/
const char* Mark_Barrier = "#";
const char* Mark_NO_Barrier = "0";
const char* Mark_Path = "1";

/*定义走路对r，c的影响*/    //0-3分别为向上、向右、向下、向左走
const int walk_r[] = { -1,0,1,0 };
const int walk_c[] = { 0,1,0,-1 };

/*定义输出宽度*/
const int MAP_OUT_WIDTH = 5;

/*点坐标结构体*/
struct point {
    int r;
    int c;
    point(int r = 0, int c = 0) :r(r), c(c) {}
    ~point() {}
    point& operator=(const point& p) { this->r = p.r; this->c = p.c; return *this; }
    bool operator==(const point& p) { if (this->r == p.r && this->c == p.c)return true; else return false; }
};

class Solve_Maze {
private:
    int row_total;
    int col_total;
    const point start;          //都是从1开始的r，c
    const point destination;
    ELEMENT** map;  //迷宫地图
    bool** is_visited;    //判断一次执行中来过这个点没
    point** pre;    //存储通往本节点的前一个节点   不用担心被多次赋值，因为visit过了就不会再visit了
    LinkedQueue<point> queue;

    //输出符号
    const char* M_Barrier;
    const char* M_NoBarrier;
    const char* M_Path;
public:
    Solve_Maze(const int r, const int c, const int rs, const int cs, const int rd, const int cd, ELEMENT** map
        , const char* MB = Mark_Barrier, const char* MNB = Mark_NO_Barrier, const char* MP = Mark_Path);
    ~Solve_Maze();
    int solve();
    void show_map();
    void show_path(int r, int c);
};

//成员函数的实现
Solve_Maze::Solve_Maze(const int r, const int c, const int rs, const int cs, const int rd, const int cd, ELEMENT** map_source,
    const char* MB, const char* MNB, const char* MP) :start(point(rs, cs)), destination(point(rd, cd))
{
    this->row_total = r;
    this->col_total = c;
    this->M_Barrier=MB;
    this->M_NoBarrier=MNB;
    this->M_Path=MP;
    //对map和is_visited进行初始化
    //申请r+2行和c+2列以方便操作
    this->map = new ELEMENT * [r + 2];
    this->is_visited = new bool* [r + 2];
    this->pre = new point * [r + 2];
    for (int i = 0; i < r + 2; i++)
    {
        this->map[i] = new ELEMENT[c + 2];
        this->is_visited[i] = new bool[c + 2];
        this->pre[i] = new point[c + 2];
    }
    for (int i = 0; i < r + 2; i++)
    {
        for (int j = 0; j < c + 2; j++)
        {
            if (j == 0 || i == 0 || j == c + 1 || i == r + 1)
            {
                this->map[i][j] = map_have_barrier;
            }
            else
            {
                this->map[i][j] = map_source[i - 1][j - 1];
            }
            this->is_visited[i][j] = 0;   //对is_visited进行初始化
        }
    }
}
Solve_Maze::~Solve_Maze()
{
    for (int i = 0; i < this->row_total + 2; i++)
    {
        delete[]this->map[i];
        delete[]this->is_visited[i];
        delete[]this->pre[i];
    }
    delete[]this->map;
    delete[]this->is_visited;
    delete[]this->pre;
}
void Solve_Maze::show_map()
{
    cout << setiosflags(ios::left);
    cout << "Maze map is:" << endl;
    cout << setw(MAP_OUT_WIDTH) << " ";
    //首先打印列数
    for (int i = 1; i <= this->col_total; i++)
    {
        cout << i << setw(MAP_OUT_WIDTH - (i < 10 ? 1 : 2)) << "column"; //这里可以把i的位数也考虑上去
    }
    cout << endl;
    for (int i = 1; i <= this->row_total; i++)
    {
        //打印行数
        cout << i << setw(MAP_OUT_WIDTH) << "row";
        for (int j = 1; j <= this->col_total; j++)
        {
            if (map[i][j] == map_have_barrier)
            {
                cout << setw(MAP_OUT_WIDTH) << this->M_Barrier;
            }
            else if (map[i][j] == map_no_barrier)
            {
                cout << setw(MAP_OUT_WIDTH) << this->M_NoBarrier;
            }
            else if (map[i][j] == map_path)
            {
                cout << setw(MAP_OUT_WIDTH) << this->M_Path;
            }
        }
        cout << endl;
    }
    cout << endl;
}
void Solve_Maze::show_path(int r, int c)
{
    //递归求解
    //结束条件
    if (r == this->start.r && c == this->start.c)
    {
        printf("(%d,%d)", r, c);
        return;
    }
    //执行操作
    point u = pre[r][c];
    show_path(u.r, u.c);
    printf("--->(%d,%d)", r, c);
    return;
}
int Solve_Maze::solve()
{
    //把start节点先进入
    pre[start.r][start.c] = point(-1, -1); //维护开始节点的前节点
    is_visited[start.r][start.c]=true;
    queue.EnQueue(start);
    while (this->queue.IsEmpty() == NO)
    {
        point u;
        queue.DeQueue(u);   //把队列的头结点存在u中并删除
        //结束条件
        if (u.r == destination.r && u.c == destination.c)
        {
            //找到结束
            //对map进行操作 把path存入map和path中
            //首先先对map进行更改
            point temp = u;
            while (1)
            {
                map[temp.r][temp.c] = map_path;
                temp = pre[temp.r][temp.c];
                if (pre[temp.r][temp.c] == point(-1, -1))
                {
                    //如果这个是起点了 做最后一次就停止
                    map[temp.r][temp.c] = map_path;
                    break;
                }
            }
            //输出map
            this->show_map();
            cout << "Maze path" << endl;
            this->show_path(u.r, u.c);
            cout<<endl;
            //结束
            wait_for_enter();
            return OK;
        }
        for (int i = 0; i < 4; i++)
        {
            point v = point(u.r + walk_r[i], u.c + walk_c[i]);
            if (is_visited[v.r][v.c] == 0 && map[v.r][v.c] != map_have_barrier)//可以走的条件是没有墙、并且没访问过（因为已经访问过了走这里一定不是最短）
            {
                //走这里
                //维护visit
                this->is_visited[v.r][v.c] = 1;
                //维护pre
                this->pre[v.r][v.c] = u;
                //入队列
                this->queue.EnQueue(v);
            }
        }
    }
    //如果所有队列都取完了 说明走不到
    //输出样式
    this->show_map();
    cout << "Path do not exist!" << endl;
    return 0;
}


/*老师要求的自定义输入*/
const int r_given = 7, c_given = 7;
const int r_start = 2, c_start = 2;
const int r_des = 7, c_des = 5;
const ELEMENT map_given[r_given][c_given] = {
    1,1,1,1,1,1,1,
    0,0,0,1,1,1,1,
    0,1,0,1,1,1,1,
    0,1,0,1,0,0,0,
    0,0,0,1,0,1,1,
    1,1,0,0,0,1,1,
    0,0,0,1,0,1,1
};

int main()
{
    ELEMENT** map;
    map = new ELEMENT * [r_given];
    for (int i = 0; i < r_given; i++)
    {
        map[i] = new ELEMENT[c_given];
    }
    for (int i = 0; i < r_given; i++)
    {
        for (int j = 0; j < c_given; j++)
        {
            map[i][j] = map_given[i][j];
        }
    }
    Solve_Maze sm(r_given, c_given, r_start, c_start, r_des, c_des, map);
    //sm.show_map();
    sm.solve();
    return 0;
}
