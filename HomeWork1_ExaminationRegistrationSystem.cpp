/*1951565 软02 谢思程*/

/*------------------------------------------------------------------------
考试报名系统
功能：
    1、输入考生信息
    2、输出考生信息
    3、查询考生信息
    4、添加考生信息
    5、修改考试信息
    6、删除考生信息
对应函数：
    1、Status input(int no,string name,string sex,int age,string choice);
    2、Status output(int no);
    3、Status inquire(int no);
    4、Status add(int pos,int no,string name,string sex,int age,string choice);
    5、Status update_no(int no_past,int no_cur);
       Status update_name(int no,string name);
       Status update_sex(int no,string sex);
       Status update_age(int no,int age);
       Status update_choice(int no,string choice);
    6、Status delete(int no);
附加函数：
思想：
参考数据库的知识：
    绝对不能把所有的信息塞在一个数组里（原子性）
    考虑：
        表1（个人基本属性表）：
            存放考号、姓名、性别、年龄（我希望这个年龄可以统一变化）
        表2（个人信息表）：
            存放考号、报考类型
        表1、表2都采用链式存储，注意，在改变考号的时候，两张表都要改变
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
所有的头文件
------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>  //用一点点string
#include <iomanip> //输出格式控制头文件
using namespace std;

/*------------------------------------------------------------------------
公共函数
------------------------------------------------------------------------*/
#if 1

#ifndef _WIN32
#define _strcmpi strcasecmp
#endif
//结束提示(只支持字符输入，暂不支持回车)
void wait_for_end(const char *prompt, const char *key)
{
    char input_str[1024];

    while (1)
    {
        cout << prompt;
        cin >> input_str;

        /* 清空缓冲区 */
        cin.clear();
        cin.ignore(1024, '\n');
        //		fgets(input_str, sizeof(input_str), stdin);

        /* 输入在合理范围内则退出输入循环 */
        if (_strcmpi(input_str, key) == 0)
            break;
    }

    return;
}
void wait_for_enter()
{
    cout << "please type enter to continue ..." << endl;
    getchar();
    while (getchar() != '\n')
        ;
}
void safe_cin_positive_integer(int &x, const char *tips)
{
    cout << tips;
    while (1) //考虑一部分输入错误问题
    {
        if (scanf("%d", &x) != 1)
        {
            cout << "error, please input again!" << endl;
            getchar();
            while (getchar() != '\n')
                ;
        }
        else if (x <= 0)
        {
            cout << "error, please input again!" << endl;
        }
        else
        {
            break;
        }
        cout << tips;
    }
}

#endif

/*------------------------------------------------------------------------
首先是学生基本信息类
------------------------------------------------------------------------*/
#if 1
//声明
class STUDENT
{
private:
    int no;      //学号
    string name; //姓名
    string sex;  //性别
    int age;     //年龄
    string pick; //报考种类
public:
    STUDENT();
    STUDENT(int n, string na, string se, int ag, string pic) : no(n), name(na), sex(se), age(ag), pick(pic){};
    ~STUDENT() {}
    STUDENT &operator=(const STUDENT &stu);
    int operator==(const STUDENT &stu);
    int operator>(const STUDENT &stu);
    int operator<(const STUDENT &stu);
    friend istream &operator>>(istream &in, STUDENT &stu);
    explicit operator int();
    int get_no() const;
    string get_name() const;
    string get_sex() const;
    int get_age() const;
    string get_pick() const;
};

//实现
int STUDENT::get_no() const
{
    return this->no;
}
string STUDENT::get_name() const
{
    return this->name;
}
string STUDENT::get_sex() const
{
    return this->sex;
}
int STUDENT::get_age() const
{
    return this->age;
}
string STUDENT::get_pick() const
{
    return this->pick;
}
istream &operator>>(istream &in, STUDENT &stu)
{
    cin >> stu.no >> stu.name >> stu.sex >> stu.age >> stu.pick;
    return in;
}
STUDENT::STUDENT()
{
    this->no = 0;
    this->name = "";
    this->sex = "";
    this->age = 0;
    this->pick = "";
}
STUDENT &STUDENT::operator=(const STUDENT &stu)
{
    this->no = stu.no;
    this->name = stu.name;
    this->sex = stu.sex;
    this->age = stu.age;
    this->pick = stu.pick;
    return *this;
}
int STUDENT::operator==(const STUDENT &stu)
{
    if (this->no == stu.no)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int STUDENT::operator>(const STUDENT &stu)
{
    if (this->no > stu.no)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int STUDENT::operator<(const STUDENT &stu)
{
    if (this->no < stu.no)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
STUDENT::operator int()
{
    return this->no;
}

#endif

/*------------------------------------------------------------------------
简单的链表
------------------------------------------------------------------------*/
#if 1
/*定义新类型*/
typedef int Status;
/*宏定义*/
#define OK 1
#define ERROR 0
#define LOVERFLOW -1
#define INFEASIBLE -2
#define TRUE 1
#define FALSE 0

//链表节点
typedef struct StudentNodeList
{
    STUDENT ele;
    StudentNodeList *next; //指向头结点的指针
} NodeList, *ListPointer;

//链表声明
class StudentList
{
private:
    ListPointer head; //指向链表的头结点
    int num;          //记录链表节点数
public:
    StudentList();                                 //构造函数
    ~StudentList();                                //析构函数
    Status ClearList();                            //清空表 不会把头结点消除
    Status ListEmpty();                            //判断是不是空表 是返回true 不是返回false
    int ListLength();                              //求长度
    Status GetElem(int pos, STUDENT *e);           //取表中元素
    Status GetElem_by_no(int no, STUDENT *e);      //根据学号取表中元素
    int LocateElem(int no);                        //求no的位置，没找到返回ERROR，否则返回pos
    Status ListInsert(int pos, STUDENT e);         //在指定位置插入一个元素 可以插入的位置是[1,n+1]
    Status ListInsert_by_no(STUDENT e);            //按学号顺序插入
    Status ListDelete_by_pos(int pos, STUDENT *e); //删除指定位置的元素，并将被删除元素返回e中 可以删除的范围是[1,n]
    Status ListDelete(int no, STUDENT *e);         //删除指定学号的元素，并将被删除的元素返回e中
    Status Update(int no, STUDENT *e);             //更改
    Status Sort();
};
//实现
Status StudentList::Sort()
{
    if (this->head->next == NULL)
        return OK;
    ListPointer p = this->head->next, q = p->next, r;
    p->next = NULL; //关闭
    while (q)
    {
        r = q;
        q = q->next;
        this->ListInsert_by_no(r->ele);
        delete r;
    }
    return OK;
}
Status StudentList::Update(int no, STUDENT *e)
{
    ListPointer p = this->head->next;
    int pos = 1;
    while (p && no != p->ele.get_no())
    {
        p = p->next;
        pos++;
    }
    if (p == NULL)
    {
        return ERROR;
    }
    p->ele = *e;
    return OK;
}
Status StudentList::GetElem_by_no(int no, STUDENT *e)
{
    ListPointer p = this->head->next;
    int pos = 1;
    while (p && no != p->ele.get_no())
    {
        p = p->next;
        pos++;
    }
    if (p == NULL)
    {
        return ERROR;
    }
    *e = p->ele;
    return OK;
}
Status StudentList::ListInsert_by_no(STUDENT e)
{
    ListPointer p = this->head, s;
    int pos = 0;
    //找到第小于e的最后一个位置
    while (p->next && e.get_no() > p->next->ele.get_no())
    {
        p = p->next;
        pos++;
    }

    //在p的位置处插入一个元素
    s = new NodeList;
    if (s == NULL)
        return LOVERFLOW;
    s->next = p->next;
    p->next = s;

    //赋值
    s->ele = e;
    return OK;
}
StudentList::StudentList()
{
    this->head = new NodeList;
    if (this->head == NULL)
    {
        //一般来说最好给全局变量赋值并退出
        exit(LOVERFLOW);
    }
    this->head->next = NULL;
}
StudentList::~StudentList()
{
    ListPointer p = this->head, q;
    //顺序消除
    do
    {
        q = p;
        p = p->next;
        delete q;
    } while (p);
}
Status StudentList::ClearList()
{
    ListPointer p = this->head->next, q;
    while (p)
    {
        q = p;
        p = p->next;
        delete q;
    }
    this->head->next = NULL;
    return OK;
}
Status StudentList::ListEmpty()
{
    if (head->next == NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
int StudentList::ListLength()
{
    ListPointer p = this->head->next;
    int len = 0;
    while (p)
    {
        p = p->next;
        len++;
    }
    return len;
}
Status StudentList::GetElem(int pos, STUDENT *e)
{
    ListPointer p = this->head->next; //指向头结点
    int i;
    for (i = 1; i < pos && p; i++)
    {
        p = p->next;
    }
    if (p == NULL || i > pos) //p=NULL即找不到了 i>pos即初始状态不合理
    {
        return ERROR;
    }
    *e = p->ele;
    return OK;
}
int StudentList::LocateElem(int no)
{
    ListPointer p = this->head->next;
    int pos = 1;
    while (p && no != int(p->ele))
    {
        p = p->next;
        pos++;
    }
    return p == NULL ? ERROR : pos;
}
Status StudentList::ListInsert(int pos, STUDENT e)
{
    /*
    //判断是否越界1
    int len=this->ListLength();
    if(!(pos>=1&&pos<=len+1))
        return ERROR;
    */
    ListPointer p = head, s; //s用来申请新的节点
    int i = 0;               //初始位置是头结点

    //寻找第pos-1个节点
    while (p && i < pos - 1) //这里只写p的原因是我只需要确定p是存在的，就可以在p后面添加内容
    {
        i++;
        p = p->next;
    }

    //判断错误
    if (p == NULL || i > pos - 1)
    {
        return ERROR;
    }

    //到这里为止p指向i-1个节点
    //申请新节点
    s = new NodeList;
    if (s == NULL)
    {
        return LOVERFLOW;
    }
    s->ele = e; //赋值
    //维护链表
    s->next = p->next;
    p->next = s;
    return OK;
}
Status StudentList::ListDelete_by_pos(int pos, STUDENT *e)
{
    ListPointer p = this->head, q;
    int i = 0;

    //寻找第pos-1个节点
    while (p->next && i < pos - 1) //这里写p->next的原因是要保证p的next必须可删除
    {
        i++;
        p = p->next;
    }

    //错误排除2  更加快速
    if (p->next == NULL || i > pos - 1)
    {
        return ERROR;
    }

    //维护链表
    //此时p指向i-1
    q = p->next;
    p->next = q->next;

    *e = q->ele;
    delete q;
    return OK;
}
Status StudentList::ListDelete(int no, STUDENT *e)
{
    ListPointer p = this->head, q;
    int pos = 0;
    //找到第i-1个位置
    while (p->next && no != int(p->next->ele))
    {
        p = p->next;
        pos++;
    }
    if (p->next == NULL)
    {
        return ERROR;
    }

    //维护
    q = p->next;
    p->next = q->next;

    //赋值并删除
    *e = q->ele;
    delete q;
    return OK;
}

#endif

/*------------------------------------------------------------------------
menu
------------------------------------------------------------------------*/
#if 1
#ifdef _WIN32
#include <conio.h> //_getche
#else
#include <termio.h>
#include <unistd.h>
char _getch()
{
    char c;
    int STA = 0;
    STA++;
    STA = system("stty -echo");
    STA = system("stty -icanon");
    c = getchar();
    STA = system("stty icanon");
    STA = system("stty echo");
    return c;
}
char _getche()
{
    char c;
    int STA = 0;
    STA++;
    STA = system("stty -icanon");
    while((c = getchar())=='\n'||c==' ')
        ;
    STA = system("stty icanon");
    return c;
}
#endif
/*宏定义和常变量*/
static const int Unit_width = 30; //菜单的基本宽度为30
const int MAX_ORDER = 256;
const int MAX_BORDER = 5;

//声明
class SIMPLE_MENU
{
private:
    const char **menu; //menu必须在构造函数中初始化menu应该是指向 指向常量的指针 的指针
    int width;         //每个菜单项的宽度

    bool if_sensitive; //标志 可选项中是否敏感大小写
    const char *key;   //可选择项

    bool if_exist_title; //标志 是否有标题
    const char *title;   //存储标题内容

    bool if_exist_order; //标志 是否有提示
    char order[MAX_ORDER]; //储存 提示(order)

    int item_nums; //menu中有多少个条目

    char border[MAX_BORDER]; //边框的符号

    /*暂时不用*/
    int arrangement; //0表示从上到下1表示从左到右
    int col;         //表示多列显示

public:
    SIMPLE_MENU(const char **menu, bool if_sensitive = false, bool if_exist_title = false,
                bool if_exist_order = false, const char *key = "", const char *title = "",
                const char *order = "", const char *border = "=", int width = Unit_width) : menu(menu), width(width), if_sensitive(if_sensitive), key(key),
                                                                                            if_exist_title(if_exist_title), title(title), if_exist_order(if_exist_order)
    {
        //在构造函数中计算item的项目节省时间
        int cnt = 0;
        while (menu[cnt])
            cnt++;
        this->item_nums = cnt;

        //暂时不用的私有变量
        this->arrangement = 0;
        this->col = 1;

        //LINUX: should deal with order and border
        strcpy(this->order, order);
        strcpy(this->border, border);
    };
    ~SIMPLE_MENU() {}
    friend void wait_for_end(const char *prompt, const char *key);
    const char display_menu();                //输出表
    const char *get_menuitem(const char sel); //查看menu中是否有sel,并返回
};

const char *SIMPLE_MENU::get_menuitem(const char sel)
{
    int i;
    for (i = 0; menu[i]; i++)
        if (this->menu[i][0] == sel) //每个菜单字符串的第[0]个字符是否选择项
            return menu[i];

    return NULL;
};
const char SIMPLE_MENU::display_menu()
{
    char sel;
    int i, j;
    int loop = 1;
    cout << setiosflags(ios::left); //左对齐
    while (loop)
    {
        if (this->if_exist_title) //输出标题
        {
            cout << title << endl;
        }
        for (i = 1; i <= col * this->width; i++)
        {
            cout << this->border;
        }
        cout << endl;
        for (i = 0; arrangement && menu[i];)
        {
            for (j = 1; j <= col && menu[i]; j++, i++)
                cout << setw(this->width) << menu[i];
            cout << endl;
        }
        for (i = 0; !arrangement && i < (this->item_nums + col - 1) / col; i++) //可能有点问题
        {
            for (j = 0; j < col && menu[i + j * (this->item_nums / col) + (j > this->item_nums % col ? this->item_nums % col : j)]; j++)
                cout << setw(this->width) << menu[i + j * (this->item_nums / col) + (j > this->item_nums % col ? this->item_nums % col : j)];
            cout << endl;
        }
        for (i = 1; i <= col * this->width; i++)
        {
            cout << this->border;
        }
        cout << endl;

        if (this->if_exist_order)
        {
            cout << "choose[" << this->order << "] : ";
        }
        else
        {
            cout << "choose:";
        }
        sel = _getche();
        cout << endl;
        for (i = 0; key[i]; i++)
            if (sel == key[i])
            {
                loop = 0;
                break;
            }
        /* 选择错误则给出提示并再次打印菜单 */
        if (loop)
            cout << endl
                 << endl
                 << "error, please choose again!" << endl
                 << endl;
    }
    if (sel >= 'a' && sel <= 'z' && !this->if_sensitive)
        sel = sel - 'a' + 'A';
    return sel;
}

#endif

/*------------------------------------------------------------------------
考试报名表的建立     ERS
------------------------------------------------------------------------*/
#if 1
/*宏定义和常变量*/
//宏定义退出和继续
#define ERS_QUIT 0
#define ERS_CONTINUE 1
//输出表格的宽度
const int ERS_TABLE_WIDTH_no = 10;
const int ERS_TABLE_WIDTH_name = 12;
const int ERS_TABLE_WIDTH_sex = 8;
const int ERS_TABLE_WIDTH_age = 8;
const int ERS_TABLE_WIDTH_pick = 20;
//输出语句
const char *ERS_menu[] = {
    "1.Insert (by location)",
    "2.Insert (by number)",
    "3.Delete (by location)",
    "4.Delete (by number)",
    "5.Find",
    "6.Modification",
    "7.Sort by number",
    "0.exit",
    NULL};
const char *ERS_key = "012345670";
const char *ERS_order = "0-7";
const char *ERS_title = "please choose the option";

//声明
typedef class ExaminationRegistrationSystem
{
private:
    friend int operation(char sel, ExaminationRegistrationSystem &ERS);
    StudentList SL;
    int student_num;
    SIMPLE_MENU simple_menu;

public:
    ExaminationRegistrationSystem(const char **menu = ERS_menu, const char *key = ERS_key, const char *title = ERS_title, const char *order = ERS_order, const char *border = "=");
    ~ExaminationRegistrationSystem(){};
    void display_table();
    void output_student(const STUDENT &stu);
} ERS;

int operation(char sel, ERS &ers) //修改menu需要修改的函数    主体不需要修改
{
    switch (sel)
    {
    case ('1'):
    {
        int pos;
        STUDENT temp_stu;
        safe_cin_positive_integer(pos, "Please enter the location of the candidate you want to insert: ");
        if (ers.SL.ListLength() + 1 < pos)
        {
            cout << "location " << pos << "is not allowed to insert!" << endl;
            wait_for_enter();
        }
        else
        {
            cout << "Please enter the test number, name, gender, age and type of candidate to be inserted: " << endl;
            cin >> temp_stu;
            ers.SL.ListInsert(pos, temp_stu);
            ers.student_num++;
        }
        break;
    }
    case ('2'):
    {
        STUDENT temp_stu;
        ers.SL.Sort(); //需要先排序
        cout << "Please enter the test number, name, gender, age and type of candidate to be inserted: " << endl;
        cin >> temp_stu;
        ers.SL.ListInsert_by_no(temp_stu);
        ers.student_num++;
        break;
    }
    case ('3'):
    {
        int pos;
        STUDENT temp_stu;
        safe_cin_positive_integer(pos, "Please enter the location of the candidate you want to delete: ");
        if (ers.SL.ListLength() < pos)
        {
            cout << "location " << pos << " is not allowed to delete!" << endl;
            wait_for_enter();
        }
        else
        {
            ers.SL.ListDelete_by_pos(pos, &temp_stu);
            cout << "The candidate information you deleted is: " << endl;
            ers.output_student(temp_stu);
            ers.student_num--;
            wait_for_enter();
        }
        break;
    }
    case ('4'):
    {
        int no;
        int flag;
        STUDENT temp_stu;
        safe_cin_positive_integer(no, "Please enter the test number of the candidate you want to delete: ");
        flag = ers.SL.ListDelete(no, &temp_stu);
        if (flag == ERROR)
        {
            cout << "student " << no << "is not existing!" << endl;
            wait_for_enter();
        }
        else
        {
            cout << "The candidate information you deleted is: " << endl;
            ers.output_student(temp_stu);
            ers.student_num--;
            wait_for_enter();
        }
        break;
    }
    case ('5'):
    {
        int no;
        int flag;
        STUDENT temp_stu;
        safe_cin_positive_integer(no, "Please enter the test number of the candidate you are looking for: ");
        flag = ers.SL.GetElem_by_no(no, &temp_stu);
        if (flag == ERROR)
        {
            cout << "student " << no << "is not existing!" << endl;
            wait_for_enter();
        }
        else
        {
            cout << "The candidate information you inquired is: "  << endl;
            ers.output_student(temp_stu);
            wait_for_enter();
        }
        break;
    }
    case ('6'):
    {
        int no;
        int flag;
        STUDENT temp_stu;
        safe_cin_positive_integer(no, "Please enter the test number of the candidate to be modified: ");
        flag = ers.SL.GetElem_by_no(no, &temp_stu);
        if (flag == ERROR)
        {
            cout << "student " << no << "is not existing!" << endl;
            wait_for_enter();
        }
        else
        {
            cout << "Please enter all the information of the candidate (test number, name, gender, age and type of candidate): " << endl;
            cin >> temp_stu;
            flag = ers.SL.Update(no, &temp_stu);
        }
        break;
    }
    case ('7'):
    {
        ers.SL.Sort();
        break;
    }
    case ('0'):
    {
        return ERS_QUIT;
    }
    }
    return ERS_CONTINUE;
}

//实现
void ERS::output_student(const STUDENT &stu)
{

    cout << setw(ERS_TABLE_WIDTH_no) << "number";
    cout << setw(ERS_TABLE_WIDTH_name) << "name";
    cout << setw(ERS_TABLE_WIDTH_sex) << "gender";
    cout << setw(ERS_TABLE_WIDTH_age) << "age";
    cout << setw(ERS_TABLE_WIDTH_pick) << "type of candidate";
    cout << endl;
    cout << setw(ERS_TABLE_WIDTH_no) << stu.get_no();
    cout << setw(ERS_TABLE_WIDTH_name) << stu.get_name();
    cout << setw(ERS_TABLE_WIDTH_sex) << stu.get_sex();
    cout << setw(ERS_TABLE_WIDTH_age) << stu.get_age();
    cout << setw(ERS_TABLE_WIDTH_pick) << stu.get_pick();
    cout << endl;
}
void ERS::display_table()
{
    cout << setiosflags(ios::left);
    cout << setw(ERS_TABLE_WIDTH_no) << "number";
    cout << setw(ERS_TABLE_WIDTH_name) << "name";
    cout << setw(ERS_TABLE_WIDTH_sex) << "gender";
    cout << setw(ERS_TABLE_WIDTH_age) << "age";
    cout << setw(ERS_TABLE_WIDTH_pick) << "type of candidate";
    cout << endl;
    for (int i = 1; i <= this->student_num; i++)
    {
        STUDENT temp_stu;
        this->SL.GetElem(i, &temp_stu);
        cout << setw(ERS_TABLE_WIDTH_no) << temp_stu.get_no();
        cout << setw(ERS_TABLE_WIDTH_name) << temp_stu.get_name();
        cout << setw(ERS_TABLE_WIDTH_sex) << temp_stu.get_sex();
        cout << setw(ERS_TABLE_WIDTH_age) << temp_stu.get_age();
        cout << setw(ERS_TABLE_WIDTH_pick) << temp_stu.get_pick();
        cout << endl;
    }
}
ERS::ExaminationRegistrationSystem(const char **menu, const char *key, const char *title, const char *order, const char *border)
    : simple_menu(menu, 0, 1, 1, key, title, order, border)
{
    cout << "First, please establish the candidate information system!" << endl;
    safe_cin_positive_integer(this->student_num, "Please enter the number of candidates (>0): ");
    for (int i = 0; i < this->student_num; i++)
    {
        STUDENT temp_stu;
        cout << "Please enter student " << i + 1 << "'s number, name, gender, age and type of candidate!" << endl;
        cin >> temp_stu;
        this->SL.ListInsert(i + 1, temp_stu);
    }
    while (1)
    {
        char sel;
        cout << endl;
        this->display_table();
        sel = simple_menu.display_menu(); //调用menu
        if (operation(sel, *this) == ERS_QUIT)
            break; //operation为具体执行程序
    }
}

#endif

int main()
{
    ERS ers;
    return 0;
}