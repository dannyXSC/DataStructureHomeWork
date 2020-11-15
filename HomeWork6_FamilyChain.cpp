/*------------------------------------------------------------------------
家谱管理系统
emm应该比较简单
如果有多个重名成员  以最年长的？（用队列实现了）
删除一个人 它父亲节点的数组要对应处理
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

//定义中间数组最大值
const int max_temp = 256;

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
    while ((c = getchar()) == '\n' || c == ' ')
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

    bool if_exist_order;   //标志 是否有提示
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

#if 1
/*------------------------------------------------------------------------
函数名  :wait_for_end
功能    :等待输入一个字符串以结束(不区分大小写)
参数    :prompt 提示语
        key 结束标准字符串
------------------------------------------------------------------------*/
#ifndef _WIN32
#define _strcmpi strcasecmp
#endif
void wait_for_end(const char* prompt, const char* key)
{
    char input_str[1024];

    while (1) {
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
/*------------------------------------------------------------------------
函数名  :wait_for_enter
功能    :等待输入回车以结束
参数    :
------------------------------------------------------------------------*/
void wait_for_enter()
{
    cout << "Please type enter to continue..." << endl;
    getchar();
    while (getchar() != '\n')
        ;
}
/*------------------------------------------------------------------------
函数名  :safe_cin_positive_integer
功能    :安全输入一个正整数
参数    :x 需要输入的变量
        tips 问题提示语
------------------------------------------------------------------------*/
void safe_cin_positive_integer(int& x, const char* tips, int toplimit = 0x7fffffff)    //默认上限为int最大值
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
        else if (x > toplimit)
        {
            cout << "Input above upper limit! (Limit: " << toplimit << " ) please retype!" << endl;
        }
        else
        {
            break;
        }
        cout << tips;
    }
}
/*------------------------------------------------------------------------
函数名  :safe_cin_string
功能    :安全输入一个string
参数    :x 需要输入的变量
        tips 问题提示语
------------------------------------------------------------------------*/
void safe_cin_string(string& x, const char* tips)
{
    cout << tips;
    cin >> x;
    return;
}

#endif

//节点
class FamilyNode {
public:
    string name;    //成员姓名
    int num_kids;   //存储有几个孩子
    FamilyNode** kids;  //储存子女节点的地址    用数组存储 暂不使用链表存储 因为变化子女次数比较少同时一个人的子女也不会很多
    FamilyNode() :name(""), num_kids(0), kids(NULL) {}  //构造函数
};

//树
class FamilyTree {
protected:
    FamilyNode* root;   //祖先节点 要求进入构造函数的时候就赋初值
    FamilyNode* find_member(string name, FamilyNode* current);   //以姓名寻找某个家庭成员    后面一个参数是当前的root
                                            //如果有多个重名成员  以最年长的？（用队列实现了）
                                            //如果没找到 返回NULL
    Status delete_tree(FamilyNode* nodepointer);    //通过节点指针删除
    Status delete_maintain_tree(string name, FamilyNode* current);
    void show_kids(FamilyNode* nodepointer);    //标准输出子孙
public:
    FamilyTree();   //构造函数
    ~FamilyTree() { this->delete_tree(this->root); }   //析构函数
    Status set_kids();  //完成任务a
    Status add_kids();  //完成任务b
    Status delete_FamilyTree();     //通过姓名删除家庭树 完成c
                                            //删除一个人 它父亲节点的数组要对应处理
    Status update_name();   //完成d
};

//树的实现
FamilyNode* FamilyTree::find_member(string name, FamilyNode* current)
{
    if (current == NULL)
    {
        //节点为空 结束查找
        return NULL;
    }
    //如果名字相同 可以 返回自身
    if (current->name == name)return current;
    //如果不相同 寻找儿子节点
    for (int i = 0; i < current->num_kids; i++)
    {
        //寻找以第i个节点为根的节点中有没有符合要求的
        FamilyNode* temp = find_member(name, current->kids[i]);
        if (temp != NULL)
        {
            //找到了
            return temp;
        }
        //没找到继续循环
    }
    //儿子节点也没有 返回NULL
    return NULL;
}

Status FamilyTree::delete_tree(FamilyNode* nodepointer)
{
    //空节点 结束
    if (nodepointer == NULL)
    {
        return OK;
    }
    for (int i = 0; i < nodepointer->num_kids; i++)
    {
        this->delete_tree(nodepointer->kids[i]);
    }
    //清除用来保存子女节点的数组
    delete[]nodepointer->kids;
    //清理本身
    delete nodepointer;
    return OK;
}

void FamilyTree::show_kids(FamilyNode* nodepointer)
{
    if(nodepointer->num_kids==0)
    {
        //说明此人无子孙
        cout << nodepointer->name << " do not have descendant!" << endl;
        return;
    }
    cout << nodepointer->name << "'s first descendant is: ";
    for (int i = 0; i < nodepointer->num_kids; i++)
    {
        cout << nodepointer->kids[i]->name << " ";
    }
    cout << endl;
}

//return ERROR错误 return OK 正确
Status  FamilyTree::set_kids()
{
    string name;
    int num_kids;
    char temp[max_temp];    //中间数组
    safe_cin_string(name, "Please enter the name of the person who started the family: "); //输入姓名
    //找到name
    FamilyNode* pointer = find_member(name, root); //找到需要加子女的节点
    if (pointer == NULL)   //如果没找到
    {
        cout << "Input error, none " << name << " in this tree!" << endl;
        wait_for_enter();
        return ERROR;
    }
    //找到了 输入人数
    sprintf(temp, "Please enter the number of children of %s: ", name.c_str()); //配置问句
    safe_cin_positive_integer(num_kids, temp);

    //开始维护节点关系
    //更改子女数
    pointer->num_kids = num_kids;
    //更改与子女的关系
    pointer->kids = new FamilyNode * [num_kids];    //为parent的子女节点申请空间
    if (pointer->kids == NULL)
    {
        //没申请到空间
        cout << "OVERFLOW!" << endl;
        return LOVERFLOW;
    }
    cout << "Please enter " << name << "'s children's name: ";
    for (int i = 0; i < num_kids; i++)
    {
        //为每一个子女节点申请空间
        pointer->kids[i] = new FamilyNode;    //这里应该会自动调用构造函数 所以这里就不写初始化了
        //输入名字
        cin >> pointer->kids[i]->name;
    }
    //输出子孙
    this->show_kids(pointer);
    wait_for_enter();
    return OK;
}

Status FamilyTree::add_kids()
{
    string name;    //父亲姓名
    FamilyNode* pointer;
    safe_cin_string(name, "Please enter the name of the person to whom you want to add a son (or daughter) : ");
    pointer = find_member(name, this->root);
    if (pointer == NULL)
    {
        cout << "Input error, none " << name <<" in the tree!"<< endl;
        wait_for_enter();
        return ERROR;
    }

    //找到了 开始配置子女
    //更改子女数
    pointer->num_kids++;
    //更改空间
    //申请新空间
    FamilyNode** temp_kids = new FamilyNode * [pointer->num_kids];
    for (int i = 0; i < pointer->num_kids - 1; i++)
    {
        //转移
        temp_kids[i] = pointer->kids[i];
    }
    //为新加的节点申请空间
    temp_kids[pointer->num_kids - 1] = new FamilyNode;
    cout << "Please enter " << name << "'s newly added son (or daughter)'s name : ";
    cin >> temp_kids[pointer->num_kids - 1]->name;
    //删除原来的节点
    delete[]pointer->kids;
    //连上新的节点
    pointer->kids = temp_kids;

    //输出子孙
    this->show_kids(pointer);
    wait_for_enter();
    return OK;
}

Status FamilyTree::delete_maintain_tree(string name, FamilyNode* current)
{
    if (current->kids == NULL || current->num_kids == 0)
    {
        //节点已经被查过 并且没儿子了
        return ERROR;
    }
    //不查自身 因为一般是root进来 root已经被查过
    //寻找儿子节点
    for (int i = 0; i < current->num_kids; i++)
    {
        if (current->kids[i]->name == name)
        {
            //找到了
            //删除
            this->delete_tree(current->kids[i]);
            //维护数量
            current->num_kids--;
            //维护kids数组
            //数组变短 不维护大小
            for (int j = i; j < current->num_kids; j++)
            {
                current->kids[j] = current->kids[j + 1];
            }
            return OK;
        }
        //这个子节点不是 去查看他的儿子
        Status temp = delete_maintain_tree(name, current->kids[i]);
        if (temp != ERROR)
        {
            //找到了
            return OK;
        }
        //没找到继续循环
    }
    //儿子节点也没有 返回NULL
    return ERROR;
}

Status FamilyTree::delete_FamilyTree()
{
    string name;
    FamilyNode* pointer;
    safe_cin_string(name, "Please enter the name of the person who wants to dissolve the family: ");
    pointer = find_member(name, this->root);
    if (pointer == NULL)
    {
        cout << "Input error, none " << name << " in this tree" << endl;
        wait_for_enter();
        return ERROR;
    }
    //输出被删除的人
    cout << "The person who wants to break up the family is: " << pointer->name << endl;
    //输出子孙
    this->show_kids(pointer);
    wait_for_enter();

    if (this->root->name == name)
    {
        //如果是根节点 就不用维护父亲节点的数组
        this->delete_tree(this->root);
        return OK;
    }
    return delete_maintain_tree(name, this->root);
}

Status FamilyTree::update_name()
{
    string name;
    string name_new;
    FamilyNode* pointer;
    safe_cin_string(name, "Please enter the current name of the person who wants to change the name: ");
    pointer = find_member(name, this->root);
    if (pointer == NULL)
    {
        cout << "Input error, none " << name <<" in this tree!"<< endl;
        wait_for_enter();
        return ERROR;
    }
    safe_cin_string(name_new, "Please enter your changed name: ");
    pointer->name = name_new;
    cout << name << " has been changed to " << name_new << endl;
    wait_for_enter();
    return OK;
}

const char *FT_MENU[] = {
    "A.To complete family tree",
    "B.Add family members",
    "C.Dissolution of a local family",
    "D.Change family member name",
    "E.Show the ancestors of this family tree",
    "F.Outputs a generation of descendants of designated members",
    "G.exit",
    NULL};

const char key[] = "abcdefgABCDEFG";
const char title[] = "Genealogical management System";
const char order[] = "a-g(Case insensitive)";

FamilyTree::FamilyTree()
{
    SIMPLE_MENU menu(FT_MENU, 0, 1, 1, key, title, order,"=",65);
    string anscestor;
    cout << "Start by building a family tree!" << endl;
    safe_cin_string(anscestor, "Please enter the name of your ancestor: ");
    cout << "The ancestors of this family tree are: " << anscestor << endl;
    wait_for_enter();

    //设置祖先
    this->root = new FamilyNode;
    this->root->name = anscestor;
    //其他选项默认空

    int loop = 1;
    while (loop)
    {
        char sel;
        sel = menu.display_menu();           //调用menu
        switch (sel)
        {
            case('A'):
            {
                this->set_kids();
                break;
            }
            case('B'):
            {
                this->add_kids();
                break;
            }
            case('C'):
            {
                this->delete_FamilyTree();
                break;
            }
            case('D'):
            {
                this->update_name();
                break;
            }
            case('E'):
            {
                cout << "The ancestors of this family tree are: " << this->root->name << endl;
                wait_for_enter();
                break;
            }
            case('F'):
            {
                string name;
                FamilyNode* pointer;
                safe_cin_string(name, "Please enter the name of the person who needs to export offspring: ");
                pointer=find_member(name,this->root);
                if(pointer==NULL)
                {
                    cout << "Input error, none " << name <<" in this tree!"<< endl;
                    wait_for_enter();
                    break;
                }
                this->show_kids(pointer);
                wait_for_enter();
                break;
            }
            default:
            {
                wait_for_end("Please enter 'end' to conclude! \n", "end");
                loop = 0;
                break;
            }
        }
    }
}


int main()
{
    FamilyTree FT;
    return 0;
}
