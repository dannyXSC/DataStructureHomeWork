/*1951565 ��02 л˼��*/

/*------------------------------------------------------------------------
���Ա���ϵͳ
���ܣ�
    1�����뿼����Ϣ
    2�����������Ϣ
    3����ѯ������Ϣ
    4�����ӿ�����Ϣ
    5���޸Ŀ�����Ϣ
    6��ɾ��������Ϣ
��Ӧ������
    1��Status input(int no,string name,string sex,int age,string choice);
    2��Status output(int no);
    3��Status inquire(int no);
    4��Status add(int pos,int no,string name,string sex,int age,string choice);
    5��Status update_no(int no_past,int no_cur);
       Status update_name(int no,string name);
       Status update_sex(int no,string sex);
       Status update_age(int no,int age);
       Status update_choice(int no,string choice);
    6��Status delete(int no);
���Ӻ�����
˼�룺
�ο����ݿ��֪ʶ��
    ���Բ��ܰ����е���Ϣ����һ�������ԭ���ԣ�
    ���ǣ�
        ��1�����˻������Ա�����
            ��ſ��š��������Ա����䣨��ϣ������������ͳһ�仯��
        ��2��������Ϣ������
            ��ſ��š���������
        ��1����2��������ʽ�洢��ע�⣬�ڸı俼�ŵ�ʱ�����ű���Ҫ�ı�
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
���е�ͷ�ļ�
------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>  //��һ���string
#include <iomanip> //�����ʽ����ͷ�ļ�
using namespace std;

/*------------------------------------------------------------------------
��������
------------------------------------------------------------------------*/
#if 1

#ifndef _WIN32
#define _strcmpi strcasecmp
#endif
//������ʾ(ֻ֧���ַ����룬�ݲ�֧�ֻس�)
void wait_for_end(const char *prompt, const char *key)
{
    char input_str[1024];

    while (1)
    {
        cout << prompt;
        cin >> input_str;

        /* ��ջ����� */
        cin.clear();
        cin.ignore(1024, '\n');
        //		fgets(input_str, sizeof(input_str), stdin);

        /* �����ں�����Χ�����˳�����ѭ�� */
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
    while (1) //����һ���������������
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
������ѧ��������Ϣ��
------------------------------------------------------------------------*/
#if 1
//����
class STUDENT
{
private:
    int no;      //ѧ��
    string name; //����
    string sex;  //�Ա�
    int age;     //����
    string pick; //��������
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

//ʵ��
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
�򵥵�����
------------------------------------------------------------------------*/
#if 1
/*����������*/
typedef int Status;
/*�궨��*/
#define OK 1
#define ERROR 0
#define LOVERFLOW -1
#define INFEASIBLE -2
#define TRUE 1
#define FALSE 0

//�����ڵ�
typedef struct StudentNodeList
{
    STUDENT ele;
    StudentNodeList *next; //ָ��ͷ����ָ��
} NodeList, *ListPointer;

//��������
class StudentList
{
private:
    ListPointer head; //ָ��������ͷ���
    int num;          //��¼�����ڵ���
public:
    StudentList();                                 //���캯��
    ~StudentList();                                //��������
    Status ClearList();                            //��ձ� �����ͷ�������
    Status ListEmpty();                            //�ж��ǲ��ǿձ� �Ƿ���true ���Ƿ���false
    int ListLength();                              //�󳤶�
    Status GetElem(int pos, STUDENT *e);           //ȡ����Ԫ��
    Status GetElem_by_no(int no, STUDENT *e);      //����ѧ��ȡ����Ԫ��
    int LocateElem(int no);                        //��no��λ�ã�û�ҵ�����ERROR�����򷵻�pos
    Status ListInsert(int pos, STUDENT e);         //��ָ��λ�ò���һ��Ԫ�� ���Բ����λ����[1,n+1]
    Status ListInsert_by_no(STUDENT e);            //��ѧ��˳�����
    Status ListDelete_by_pos(int pos, STUDENT *e); //ɾ��ָ��λ�õ�Ԫ�أ�������ɾ��Ԫ�ط���e�� ����ɾ���ķ�Χ��[1,n]
    Status ListDelete(int no, STUDENT *e);         //ɾ��ָ��ѧ�ŵ�Ԫ�أ�������ɾ����Ԫ�ط���e��
    Status Update(int no, STUDENT *e);             //����
    Status Sort();
};
//ʵ��
Status StudentList::Sort()
{
    if (this->head->next == NULL)
        return OK;
    ListPointer p = this->head->next, q = p->next, r;
    p->next = NULL; //�ر�
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
    //�ҵ���С��e�����һ��λ��
    while (p->next && e.get_no() > p->next->ele.get_no())
    {
        p = p->next;
        pos++;
    }

    //��p��λ�ô�����һ��Ԫ��
    s = new NodeList;
    if (s == NULL)
        return LOVERFLOW;
    s->next = p->next;
    p->next = s;

    //��ֵ
    s->ele = e;
    return OK;
}
StudentList::StudentList()
{
    this->head = new NodeList;
    if (this->head == NULL)
    {
        //һ����˵��ø�ȫ�ֱ�����ֵ���˳�
        exit(LOVERFLOW);
    }
    this->head->next = NULL;
}
StudentList::~StudentList()
{
    ListPointer p = this->head, q;
    //˳������
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
    ListPointer p = this->head->next; //ָ��ͷ���
    int i;
    for (i = 1; i < pos && p; i++)
    {
        p = p->next;
    }
    if (p == NULL || i > pos) //p=NULL���Ҳ����� i>pos����ʼ״̬������
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
    //�ж��Ƿ�Խ��1
    int len=this->ListLength();
    if(!(pos>=1&&pos<=len+1))
        return ERROR;
    */
    ListPointer p = head, s; //s���������µĽڵ�
    int i = 0;               //��ʼλ����ͷ���

    //Ѱ�ҵ�pos-1���ڵ�
    while (p && i < pos - 1) //����ֻдp��ԭ������ֻ��Ҫȷ��p�Ǵ��ڵģ��Ϳ�����p������������
    {
        i++;
        p = p->next;
    }

    //�жϴ���
    if (p == NULL || i > pos - 1)
    {
        return ERROR;
    }

    //������Ϊֹpָ��i-1���ڵ�
    //�����½ڵ�
    s = new NodeList;
    if (s == NULL)
    {
        return LOVERFLOW;
    }
    s->ele = e; //��ֵ
    //ά������
    s->next = p->next;
    p->next = s;
    return OK;
}
Status StudentList::ListDelete_by_pos(int pos, STUDENT *e)
{
    ListPointer p = this->head, q;
    int i = 0;

    //Ѱ�ҵ�pos-1���ڵ�
    while (p->next && i < pos - 1) //����дp->next��ԭ����Ҫ��֤p��next�����ɾ��
    {
        i++;
        p = p->next;
    }

    //�����ų�2  ���ӿ���
    if (p->next == NULL || i > pos - 1)
    {
        return ERROR;
    }

    //ά������
    //��ʱpָ��i-1
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
    //�ҵ���i-1��λ��
    while (p->next && no != int(p->next->ele))
    {
        p = p->next;
        pos++;
    }
    if (p->next == NULL)
    {
        return ERROR;
    }

    //ά��
    q = p->next;
    p->next = q->next;

    //��ֵ��ɾ��
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
#elif __Linux__
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
/*�궨��ͳ�����*/
static const int Unit_width = 30; //�˵��Ļ�������Ϊ30
const int MAX_ORDER = 256;
const int MAX_BORDER = 5;

//����
class SIMPLE_MENU
{
private:
    const char **menu; //menu�����ڹ��캯���г�ʼ��menuӦ����ָ�� ָ������ָ�� ��ָ��
    int width;         //ÿ���˵���Ŀ���

    bool if_sensitive; //��־ ��ѡ�����Ƿ����д�Сд
    const char *key;   //��ѡ����

    bool if_exist_title; //��־ �Ƿ��б���
    const char *title;   //�洢��������

    bool if_exist_order; //��־ �Ƿ�����ʾ
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

/*------------------------------------------------------------------------
���Ա������Ľ���     ERS
------------------------------------------------------------------------*/
#if 1
/*�궨��ͳ�����*/
//�궨���˳��ͼ���
#define ERS_QUIT 0
#define ERS_CONTINUE 1
//�������Ŀ���
const int ERS_TABLE_WIDTH_no = 10;
const int ERS_TABLE_WIDTH_name = 12;
const int ERS_TABLE_WIDTH_sex = 8;
const int ERS_TABLE_WIDTH_age = 8;
const int ERS_TABLE_WIDTH_pick = 20;
//������
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

//����
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

int operation(char sel, ERS &ers) //�޸�menu��Ҫ�޸ĵĺ���    ���岻��Ҫ�޸�
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
        ers.SL.Sort(); //��Ҫ������
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

//ʵ��
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
        sel = simple_menu.display_menu(); //����menu
        if (operation(sel, *this) == ERS_QUIT)
            break; //operationΪ����ִ�г���
    }
}

#endif

int main()
{
    ERS ers;
    return 0;
}