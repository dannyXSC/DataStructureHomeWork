/*------------------------------------------------------------------------
���׹���ϵͳ
emmӦ�ñȽϼ�
����ж��������Ա  �����곤�ģ����ö���ʵ���ˣ�
ɾ��һ���� �����׽ڵ������Ҫ��Ӧ����
------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>   //��һ���string
#include <iomanip>  //�����ʽ����ͷ�ļ�
using namespace std;

//���巵��ֵ
typedef int Status;
#define YES 1
#define NO 0
#define OK 1
#define ERROR 0
#define LOVERFLOW -1 
#define INFEASIBLE -2
#define TRUE 1
#define FALSE 0

//�����м��������ֵ
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
/*�궨��ͳ�����*/
static const int Unit_width = 30; //�˵��Ļ������Ϊ30
const int MAX_ORDER = 256;
const int MAX_BORDER = 5;

//����
class SIMPLE_MENU
{
private:
    const char **menu; //menu�����ڹ��캯���г�ʼ��menuӦ����ָ�� ָ������ָ�� ��ָ��
    int width;         //ÿ���˵���Ŀ��

    bool if_sensitive; //��־ ��ѡ�����Ƿ����д�Сд
    const char *key;   //��ѡ����

    bool if_exist_title; //��־ �Ƿ��б���
    const char *title;   //�洢��������

    bool if_exist_order;   //��־ �Ƿ�����ʾ
    char order[MAX_ORDER]; //���� ��ʾ(order)

    int item_nums; //menu���ж��ٸ���Ŀ

    char border[MAX_BORDER]; //�߿�ķ���

    /*��ʱ����*/
    int arrangement; //0��ʾ���ϵ���1��ʾ������
    int col;         //��ʾ������ʾ

public:
    SIMPLE_MENU(const char **menu, bool if_sensitive = false, bool if_exist_title = false,
                bool if_exist_order = false, const char *key = "", const char *title = "",
                const char *order = "", const char *border = "=", int width = Unit_width) : menu(menu), width(width), if_sensitive(if_sensitive), key(key),
                                                                                            if_exist_title(if_exist_title), title(title), if_exist_order(if_exist_order)
    {
        //�ڹ��캯���м���item����Ŀ��ʡʱ��
        int cnt = 0;
        while (menu[cnt])
            cnt++;
        this->item_nums = cnt;

        //��ʱ���õ�˽�б���
        this->arrangement = 0;
        this->col = 1;

        //LINUX: should deal with order and border
        strcpy(this->order, order);
        strcpy(this->border, border);
    };
    ~SIMPLE_MENU() {}
    friend void wait_for_end(const char *prompt, const char *key);
    const char display_menu();                //�����
    const char *get_menuitem(const char sel); //�鿴menu���Ƿ���sel,������
};

const char *SIMPLE_MENU::get_menuitem(const char sel)
{
    int i;
    for (i = 0; menu[i]; i++)
        if (this->menu[i][0] == sel) //ÿ���˵��ַ����ĵ�[0]���ַ��Ƿ�ѡ����
            return menu[i];

    return NULL;
};
const char SIMPLE_MENU::display_menu()
{
    char sel;
    int i, j;
    int loop = 1;
    cout << setiosflags(ios::left); //�����
    while (loop)
    {
        if (this->if_exist_title) //�������
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
        for (i = 0; !arrangement && i < (this->item_nums + col - 1) / col; i++) //�����е�����
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
        /* ѡ������������ʾ���ٴδ�ӡ�˵� */
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
������  :wait_for_end
����    :�ȴ�����һ���ַ����Խ���(�����ִ�Сд)
����    :prompt ��ʾ��
        key ������׼�ַ���
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

        /* ��ջ����� */
        cin.clear();
        cin.ignore(1024, '\n');
        //		fgets(input_str, sizeof(input_str), stdin);

                /* �����ں���Χ�����˳�����ѭ�� */
        if (_strcmpi(input_str, key) == 0)
            break;
    }

    return;
}
/*------------------------------------------------------------------------
������  :wait_for_enter
����    :�ȴ�����س��Խ���
����    :
------------------------------------------------------------------------*/
void wait_for_enter()
{
    cout << "Please type enter to continue..." << endl;
    getchar();
    while (getchar() != '\n')
        ;
}
/*------------------------------------------------------------------------
������  :safe_cin_positive_integer
����    :��ȫ����һ��������
����    :x ��Ҫ����ı���
        tips ������ʾ��
------------------------------------------------------------------------*/
void safe_cin_positive_integer(int& x, const char* tips, int toplimit = 0x7fffffff)    //Ĭ������Ϊint���ֵ
{
    cout << tips;
    while (1)    //����һ���������������
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
������  :safe_cin_string
����    :��ȫ����һ��string
����    :x ��Ҫ����ı���
        tips ������ʾ��
------------------------------------------------------------------------*/
void safe_cin_string(string& x, const char* tips)
{
    cout << tips;
    cin >> x;
    return;
}

#endif

//�ڵ�
class FamilyNode {
public:
    string name;    //��Ա����
    int num_kids;   //�洢�м�������
    FamilyNode** kids;  //������Ů�ڵ�ĵ�ַ    ������洢 �ݲ�ʹ������洢 ��Ϊ�仯��Ů�����Ƚ���ͬʱһ���˵���ŮҲ����ܶ�
    FamilyNode() :name(""), num_kids(0), kids(NULL) {}  //���캯��
};

//��
class FamilyTree {
protected:
    FamilyNode* root;   //���Ƚڵ� Ҫ����빹�캯����ʱ��͸���ֵ
    FamilyNode* find_member(string name, FamilyNode* current);   //������Ѱ��ĳ����ͥ��Ա    ����һ�������ǵ�ǰ��root
                                            //����ж��������Ա  �����곤�ģ����ö���ʵ���ˣ�
                                            //���û�ҵ� ����NULL
    Status delete_tree(FamilyNode* nodepointer);    //ͨ���ڵ�ָ��ɾ��
    Status delete_maintain_tree(string name, FamilyNode* current);
    void show_kids(FamilyNode* nodepointer);    //��׼�������
public:
    FamilyTree();   //���캯��
    ~FamilyTree() { this->delete_tree(this->root); }   //��������
    Status set_kids();  //�������a
    Status add_kids();  //�������b
    Status delete_FamilyTree();     //ͨ������ɾ����ͥ�� ���c
                                            //ɾ��һ���� �����׽ڵ������Ҫ��Ӧ����
    Status update_name();   //���d
};

//����ʵ��
FamilyNode* FamilyTree::find_member(string name, FamilyNode* current)
{
    if (current == NULL)
    {
        //�ڵ�Ϊ�� ��������
        return NULL;
    }
    //���������ͬ ���� ��������
    if (current->name == name)return current;
    //�������ͬ Ѱ�Ҷ��ӽڵ�
    for (int i = 0; i < current->num_kids; i++)
    {
        //Ѱ���Ե�i���ڵ�Ϊ���Ľڵ�����û�з���Ҫ���
        FamilyNode* temp = find_member(name, current->kids[i]);
        if (temp != NULL)
        {
            //�ҵ���
            return temp;
        }
        //û�ҵ�����ѭ��
    }
    //���ӽڵ�Ҳû�� ����NULL
    return NULL;
}

Status FamilyTree::delete_tree(FamilyNode* nodepointer)
{
    //�սڵ� ����
    if (nodepointer == NULL)
    {
        return OK;
    }
    for (int i = 0; i < nodepointer->num_kids; i++)
    {
        this->delete_tree(nodepointer->kids[i]);
    }
    //�������������Ů�ڵ������
    delete[]nodepointer->kids;
    //������
    delete nodepointer;
    return OK;
}

void FamilyTree::show_kids(FamilyNode* nodepointer)
{
    if(nodepointer->num_kids==0)
    {
        //˵������������
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

//return ERROR���� return OK ��ȷ
Status  FamilyTree::set_kids()
{
    string name;
    int num_kids;
    char temp[max_temp];    //�м�����
    safe_cin_string(name, "Please enter the name of the person who started the family: "); //��������
    //�ҵ�name
    FamilyNode* pointer = find_member(name, root); //�ҵ���Ҫ����Ů�Ľڵ�
    if (pointer == NULL)   //���û�ҵ�
    {
        cout << "Input error, none " << name << " in this tree!" << endl;
        wait_for_enter();
        return ERROR;
    }
    //�ҵ��� ��������
    sprintf(temp, "Please enter the number of children of %s: ", name.c_str()); //�����ʾ�
    safe_cin_positive_integer(num_kids, temp);

    //��ʼά���ڵ��ϵ
    //������Ů��
    pointer->num_kids = num_kids;
    //��������Ů�Ĺ�ϵ
    pointer->kids = new FamilyNode * [num_kids];    //Ϊparent����Ů�ڵ�����ռ�
    if (pointer->kids == NULL)
    {
        //û���뵽�ռ�
        cout << "OVERFLOW!" << endl;
        return LOVERFLOW;
    }
    cout << "Please enter " << name << "'s children's name: ";
    for (int i = 0; i < num_kids; i++)
    {
        //Ϊÿһ����Ů�ڵ�����ռ�
        pointer->kids[i] = new FamilyNode;    //����Ӧ�û��Զ����ù��캯�� ��������Ͳ�д��ʼ����
        //��������
        cin >> pointer->kids[i]->name;
    }
    //�������
    this->show_kids(pointer);
    wait_for_enter();
    return OK;
}

Status FamilyTree::add_kids()
{
    string name;    //��������
    FamilyNode* pointer;
    safe_cin_string(name, "Please enter the name of the person to whom you want to add a son (or daughter) : ");
    pointer = find_member(name, this->root);
    if (pointer == NULL)
    {
        cout << "Input error, none " << name <<" in the tree!"<< endl;
        wait_for_enter();
        return ERROR;
    }

    //�ҵ��� ��ʼ������Ů
    //������Ů��
    pointer->num_kids++;
    //���Ŀռ�
    //�����¿ռ�
    FamilyNode** temp_kids = new FamilyNode * [pointer->num_kids];
    for (int i = 0; i < pointer->num_kids - 1; i++)
    {
        //ת��
        temp_kids[i] = pointer->kids[i];
    }
    //Ϊ�¼ӵĽڵ�����ռ�
    temp_kids[pointer->num_kids - 1] = new FamilyNode;
    cout << "Please enter " << name << "'s newly added son (or daughter)'s name : ";
    cin >> temp_kids[pointer->num_kids - 1]->name;
    //ɾ��ԭ���Ľڵ�
    delete[]pointer->kids;
    //�����µĽڵ�
    pointer->kids = temp_kids;

    //�������
    this->show_kids(pointer);
    wait_for_enter();
    return OK;
}

Status FamilyTree::delete_maintain_tree(string name, FamilyNode* current)
{
    if (current->kids == NULL || current->num_kids == 0)
    {
        //�ڵ��Ѿ������ ����û������
        return ERROR;
    }
    //�������� ��Ϊһ����root���� root�Ѿ������
    //Ѱ�Ҷ��ӽڵ�
    for (int i = 0; i < current->num_kids; i++)
    {
        if (current->kids[i]->name == name)
        {
            //�ҵ���
            //ɾ��
            this->delete_tree(current->kids[i]);
            //ά������
            current->num_kids--;
            //ά��kids����
            //������ ��ά����С
            for (int j = i; j < current->num_kids; j++)
            {
                current->kids[j] = current->kids[j + 1];
            }
            return OK;
        }
        //����ӽڵ㲻�� ȥ�鿴���Ķ���
        Status temp = delete_maintain_tree(name, current->kids[i]);
        if (temp != ERROR)
        {
            //�ҵ���
            return OK;
        }
        //û�ҵ�����ѭ��
    }
    //���ӽڵ�Ҳû�� ����NULL
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
    //�����ɾ������
    cout << "The person who wants to break up the family is: " << pointer->name << endl;
    //�������
    this->show_kids(pointer);
    wait_for_enter();

    if (this->root->name == name)
    {
        //����Ǹ��ڵ� �Ͳ���ά�����׽ڵ������
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

    //��������
    this->root = new FamilyNode;
    this->root->name = anscestor;
    //����ѡ��Ĭ�Ͽ�

    int loop = 1;
    while (loop)
    {
        char sel;
        sel = menu.display_menu();           //����menu
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
