/*------------------------------------------------------------------------
course scheduling system
------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

/*define return value*/
typedef int Status;
#define YES 1
#define NO 0
#define OK 1
#define ERROR 0
#define LOVERFLOW -1
#define INFEASIBLE -2
#define TRUE 1
#define FALSE 0

#if 1
/*----------------------------------------------------------------------------
*stack
----------------------------------------------------------------------------*/
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
        exit(LOVERFLOW);
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

#if 1

//队列
template <class T>
class LinkedQueue
{
protected:
    Node<T> *head;
    Node<T> *end;

public:
    LinkedQueue();
    ~LinkedQueue() { makeEmpty(); }
    void makeEmpty();           //清空队列
    Status EnQueue(const T &x); //元素x进入队列
    Status DeQueue(T &x);       //队头元素出队列
    Status GetFront(T &x);      //读取队头元素的值
    Status IsEmpty() const;     //判断队列是否为空
    int GetSize() const;        //求队列元素个数
};

/*------------------------------------------------------------------------
基本类型的通用链表的实现
------------------------------------------------------------------------*/
template <class T>
LinkedQueue<T>::LinkedQueue()
{
    Node<T> *s = new Node<T>; //申请头结点 next为null
    if (s == NULL)
        exit(LOVERFLOW); //如果申请不成功，退出
    this->head = s;      //头指针指向s
    this->end = s;       //尾指针指向s
}
template <class T>
void LinkedQueue<T>::makeEmpty()
{
    Node<T> *p = this->head, *q;
    while (p != NULL)
    {
        q = p;
        p = p->Next;
        delete q;
    }
}
template <class T>
Status LinkedQueue<T>::EnQueue(const T &x)
{
    //加入到尾结点之后
    Node<T> *s = new Node<T>(x); //申请一个新节点,值为x，next为null
    if (s == NULL)
        exit(LOVERFLOW); //如果申请不成功，退出
    this->end->Next = s;
    this->end = s;
    return OK;
}
template <class T>
Status LinkedQueue<T>::DeQueue(T &x)
{
    if (this->IsEmpty() == YES)
        return ERROR;
    //如果非空 删除第一个节点
    Node<T> *p = this->head, *q = p->Next;
    x = q->element;
    p->Next = q->Next;
    delete q;
}
template <class T>
Status LinkedQueue<T>::GetFront(T &x)
{
    if (this->IsEmpty() == YES)
        return ERROR;
    //非空 返回第一个节点的值
    x = this->head->Next->element;
    return OK;
}
template <class T>
Status LinkedQueue<T>::IsEmpty() const
{
    if (this->head->Next == NULL)
        return YES;
    else
    {
        return NO;
    }
}
template <class T>
int LinkedQueue<T>::GetSize() const
{
    Node<T> *p = this->head->Next;
    int cnt = 0;
    while (p != NULL)
    {
        cnt++;
        p = p->Next;
    }
    return cnt;
}
#endif

//define of CI
const int max_preCourse = 16;
const int max_numberBit = 16;
const int max_nameBit = 256;

//record information of course
typedef class CourseInformation
{
public:
    char number[max_numberBit]; //courseNumber
    char name[max_nameBit];     //courseName
    int time;                   //times
    int limitTerm;              //given courseTerm
    int num_preCourse;
    char preCourse[max_preCourse][max_numberBit]; //preCourse use Course's number
    CourseInformation() : number(""), name(""), time(0),
                          limitTerm(0), num_preCourse(0), preCourse() {}
} CI;

typedef class CourseSimpleInfo
{
public:
    int inDegree;
    int outDegree;
    int semester;
    //subscript is corresponding to CI's subscript
    CourseSimpleInfo() : inDegree(0), outDegree(0), semester(0) {}
    CourseSimpleInfo(CourseSimpleInfo &csi)
    {
        inDegree = csi.inDegree;
        outDegree = csi.outDegree;
        semester = csi.semester;
    }
    CourseSimpleInfo &operator=(CourseSimpleInfo &csi)
    {
        inDegree = csi.inDegree;
        outDegree = csi.outDegree;
        semester = csi.semester;
        return *this;
    }
} CSI;

/*define of CSS*/
const int max_courseNumber = 64;
const int max_line = 256;
const int max_day_week = 5;
const int max_class_day = 4;
//const int three_class[] = {0, 0, 0, 1, 0, 1, 2, 1, 2, 3};
//const int two_class[] = {0, 0, 1, 0, 2, 1, 0, 2, 1, 0};
const int first_class[] = {0, 0, 2, 3, 2, 2, 3};
const int second_class[] = {0, 0, 0, 0, 2, 3, 3};

char (*classSchedule)[max_day_week][max_class_day][max_numberBit]; //schedule
int (*class_two_day)[max_day_week];
int (*class_three_day)[max_day_week];

typedef class CourseSchedulingSystem
{
protected:
    //assigned classNumber_Semester
    int *assigned_classNumber_Semester;
    int *current_classNumber_Semster;
    //format
    //information of course
    CI ci[max_courseNumber]; //i didt want to apply for space
    CSI csi[max_courseNumber];
    //number of course
    int totalNumber; //total number of course
    int maxSemester;

public:
    CourseSchedulingSystem(const char *filename, int max_semester = 8); //initialize
    ~CourseSchedulingSystem();
    void InitializeVariable();
    void InitializeInOutDegree();
    Status ReadInformation(const char *filename, int max_semester);
    void DisplaySchedule();
    Status CountInDegreeNum();
    Status Schedule();
    Status If_Visit(int course);
    Status FillClassIn(int course, int start = 1);
    Status FillInCorrectSemester(int semester, int course);
    Status FillInCorrectDay(int semester, int day, int course);
    double CalVariance(int semester, int curday, int course);
    Status If_DayAvaliable(int semester, int curday, int course);
} CSS;

CSS::~CourseSchedulingSystem()
{
    delete[] classSchedule;
    delete[] class_three_day;
    delete[] class_two_day;
}

CSS::CourseSchedulingSystem(const char *filename, int max_semester)
{
    //initialize
    this->maxSemester = max_semester;
    classSchedule = new char[max_semester + 1][max_day_week][max_class_day][max_numberBit]; //classSchedule[i] means semester i's class schedule
    class_two_day = new int[max_semester + 1][max_day_week];
    class_three_day = new int[max_semester + 1][max_day_week];
    assigned_classNumber_Semester = new int[max_semester + 1];
    current_classNumber_Semster = new int[max_semester + 1];
    //read
    this->ReadInformation(filename, max_semester);
    this->InitializeVariable(); //initialize shedule two three
    this->InitializeInOutDegree();
    //schedule
    if (this->Schedule() == ERROR)
    {
        cout << "error!" << endl;
        exit(-1);
    }
    //display
    this->DisplaySchedule();
}

Status CSS::Schedule()
{
    int i, j;
    while (1)
    {
        //end state: doesnt exist one whose inDegree is more than 0
        if (CountInDegreeNum() == NO)
            break;

        LinkedStack<int> sta[2]; //to distingush
        //find: inDgree is 0 (but outDegree is more than 0 or initial inDegree is more than 0)
        //and it has not been picked
        //push them
        for (i = 0; i < this->totalNumber; i++)
        {
            if (If_Visit(i) == YES)
            {
                sta[1].Push(i); //push
                //fill the class in schedule
                if (FillClassIn(i) == ERROR)
                {
                    return ERROR;
                }
            }
        }

        //process
        int switcher = 1;
        int p;
        int pos;
        while (sta[switcher].IsEmpty() == NO)
        {
            //the switcher will change so this is not one loop
            while (sta[switcher].IsEmpty() == NO)
            {
                sta[switcher].Pop(p); //p is the father
                pos = csi[p].semester + 1;
                //to speed up
                if (csi[p].outDegree == 0)
                    continue;

                //push p's child to switcher+1
                for (i = 0; i < this->totalNumber; i++) //i refer to course i
                {
                    for (j = 0; j < ci[i].num_preCourse; j++) //j refer to i's father
                    {
                        if (strcmp(ci[i].preCourse[j], ci[p].number) == 0) //if j's father == p
                        {
                            csi[i].inDegree--;

                            if (csi[i].inDegree == 0)
                            {
                                sta[(switcher + 1) % 2].Push(i); //push
                                //determine pos
                                if (csi[i].semester != 0)
                                {
                                    //it has been initialize
                                    //p's semeseter mast bigger than pos for pos is the min positon of it
                                    if (csi[i].semester + 1 <= this->maxSemester || csi[i].outDegree == 0)
                                    {
                                        //do not change
                                    }
                                    else
                                    {
                                        //error
                                        cout << this->maxSemester << " "
                                             << "error1" << endl;
                                        return ERROR;
                                    }
                                }
                                else
                                {
                                    //change
                                    if (FillClassIn(i, pos) == ERROR)
                                    {
                                        return ERROR;
                                    }
                                }
                            }

                        } //end of if
                    }
                } //end of for
                //DisplaySchedule();
                //switch
            }
            switcher = (switcher + 1) % 2;
        } //end of while
    }     //end of while

    //process isolate course
    for (i = 0; i < totalNumber; i++)
    {
        if (csi[i].semester == 0 && csi[i].outDegree == 0 && ci[i].num_preCourse == 0) //isolate and have not visited
        {
            //visit
            FillClassIn(i);
        }
    }
    return OK;
}

Status CSS::FillInCorrectDay(int semester, int day, int course)
{
    //record
    int day_after = day + 2 - 5 >= 0 ? day + 2 - 5 : day + 2;
    int times = ci[course].time;
    int no = -1, no_after = -1;
    int no_start = max_class_day, no_after_start = max_class_day;
    int i;
    if (first_class[times] == 2)
    {
        no_start = 0;
    }
    else if (first_class[times] == 3)
    {
        no_start = 1;
    }
    if (second_class[times] == 2)
    {
        no_after_start = 0;
    }
    else if (second_class[times] == 3)
    {
        no_after_start = 1;
    }

    for (i = no_start; i < max_class_day; i += 2)
    {
        if (classSchedule[semester][day][i][0] != 0)
            continue;
        no = i;
        break;
    }
    for (i = no_after_start; i < max_class_day; i += 2)
    {
        if (classSchedule[semester][day_after][i][0] != 0)
            continue;
        no_after = i;
        break;
    }
    if (no == -1) //not find
        return ERROR;
    strcpy(classSchedule[semester][day][no], ci[course].number);
    if (no_after != -1)
        strcpy(classSchedule[semester][day_after][no_after], ci[course].number);
    return OK;
}

Status CSS::FillInCorrectSemester(int semester, int course)
{
    //record
    int times = ci[course].time;
    //variable
    double min = 1024; //inf
    int res_day = -1;  //none
    int res_day_after;
    //loop variable
    int i;

    //deal with the puzzle

    //calculate the average of 2 3 first
    for (i = 0; i < max_day_week; i++)
    {
        //loop the first day
        //if avaliable
        if (If_DayAvaliable(semester, i, course) == NO)
            continue;

        //calculate the weight
        double variance = CalVariance(semester, i, course);

        //compare
        if (variance < min)
        {
            min = variance;
            res_day = i;
        }
    }

    //error condition
    if (res_day == -1)
    {
        return ERROR;
    }

    //fill the day
    if (FillInCorrectDay(semester, res_day, course) == ERROR)
        return ERROR;
    //already find the correct semester
    csi[course].semester = semester;
    //adjust res_day_after
    res_day_after = res_day + 2 - 5 >= 0 ? res_day + 2 - 5 : res_day + 2;
    //adjust the variable
    class_two_day[semester][res_day] += (first_class[times] == 2 ? 1 : 0);
    class_three_day[semester][res_day] += (first_class[times] == 3 ? 1 : 0);
    class_two_day[semester][res_day_after] += (second_class[times] == 2 ? 1 : 0);
    class_three_day[semester][res_day_after] += (second_class[times] == 3 ? 1 : 0);
    return OK;
}

Status CSS::FillClassIn(int course, int start)
{
    int i;
    //traverse from semester 1 to max_semester
    for (i = start; i <= this->maxSemester; i++)
    {
        if (current_classNumber_Semster[i - 1] >= assigned_classNumber_Semester[i - 1])
        {
            continue;
        }
        if (FillInCorrectSemester(i, course) == OK)
        {
            this->current_classNumber_Semster[i - 1]++;
            return OK;
        }
    }
    return ERROR;
}

void CSS::DisplaySchedule()
{
    int i, j, k, t1;
    // for (i = 0; i < totalNumber;i++)
    // {
    //     cout << ci[i].number << " " << csi[i].semester<<" "<<csi[i].inDegree << endl;
    // }
    for (i = 0; i < this->maxSemester; i++)
    {
        if (current_classNumber_Semster[i] != assigned_classNumber_Semester[i])
        {
            cout << "can't schedule it" << endl;
            return;
        }
    }
    for (i = 1; i <= this->maxSemester; i++)
    {
        cout << "semester" << i << ":" << endl;
        for (t1 = 0; t1 < totalNumber; t1++)
        {
            if (csi[t1].semester == i)
                cout << ci[t1].number << " time:" << ci[t1].time
                     << " semester:" << csi[t1].semester << endl;
        }
        cout << endl;
        for (k = 0; k < max_class_day; k++)
        {
            for (j = 0; j < max_day_week; j++)
            {
                if (classSchedule[i][j][k][0] != 0)
                    cout << classSchedule[i][j][k] << "  ";
                else
                {
                    cout << "     ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }
}

void CSS::InitializeInOutDegree()
{
    int i, j, k;
    for (i = 0; i < this->totalNumber; i++)
    {
        //course i
        csi[i].inDegree = ci[i].num_preCourse; //initialize inDegree of csi
        for (j = 0; j < ci[i].num_preCourse; j++)
        {
            //precourse j
            //initialize inDegree of csi
            for (k = 0; k < this->totalNumber; k++)
            {
                if (k == i)
                    continue;
                //find its pre
                if (strcmp(ci[k].number, ci[i].preCourse[j]) == 0)
                {
                    csi[k].outDegree++; //its pre's outDegree++
                    break;
                }
            }
        }
    } //end of for
    for (i = 0; i < this->totalNumber; i++)
    {
        if (ci[i].limitTerm != 0)
        {
            csi[i].semester = ci[i].limitTerm;
            FillClassIn(i, csi[i].semester);
        }
    } //end of for
}

void CSS::InitializeVariable()
{
    int i, j, k;
    for (i = 0; i < this->maxSemester + 1; i++)
    {
        for (j = 0; j < max_day_week; j++)
        {
            for (k = 0; k < max_class_day; k++)
            {
                classSchedule[i][j][k][0] = 0;
            }
        }
    }
    for (i = 0; i < this->maxSemester + 1; i++)
    {
        for (j = 0; j < max_day_week; j++)
        {
            class_two_day[i][j] = 0;
            class_three_day[i][j] = 0;
        }
    }
    for (i = 0; i < this->maxSemester + 1; i++)
    {
        current_classNumber_Semster[i] = 0;
    }
}

Status CSS::ReadInformation(const char *filename, int max_semester)
{
    ifstream in(filename, ios::in | ios::binary);
    if (in.is_open() == 0)
    {
        //if the file dont exist
        return ERROR;
    }
    //record inf of every line
    char line[max_line];
    //to read preCourse
    char temp[max_line];
    //read the first line
    int i;
    for (i = 0; i < max_semester; i++)
    {
        in >> assigned_classNumber_Semester[i];
    }
    //record number of every course
    i = 0;
    in.getline(line, max_line);
    while (1)
    {
        in.getline(line, max_line);
        if (in.good() == 0)
            break;
        //input all by sscanf
        sscanf(line, "%s %s %d %d %[^\n]", (char *)(&ci[i].number),
               (char *)(&ci[i].name), &ci[i].time, &ci[i].limitTerm,
               temp);
        //input preCourse by stringstream
        stringstream ss;
        ss << temp;
        while (1)
        {
            ss >> ci[i].preCourse[ci[i].num_preCourse]; //numCourse is the number of course
            if (ss.fail())                              //if buffer has nothing then break
                break;
            ci[i].num_preCourse++;
        }
        //set 0
        memset(temp, 0, sizeof(temp));
        i++;
    }
    this->totalNumber = i;
    in.close();
    return OK;
}

Status CSS::CountInDegreeNum()
{
    int i;
    for (i = 0; i < this->totalNumber; i++)
    {
        if (csi[i].inDegree > 0)
            break;
    }
    if (i == this->totalNumber)
        return NO;
    return YES;
}

Status CSS::If_Visit(int course)
{
    if (csi[course].semester == 0                                       //not visit yet
        && csi[course].inDegree <= 0                                    //no inDegree
        && (csi[course].outDegree > 0 || ci[course].num_preCourse > 0)) //not isolate
        return YES;
    return NO;
}

double CSS::CalVariance(int semester, int curday, int course)
{
    int i, j;
    double average2 = 0, average3 = 0, sum2 = 0, sum3 = 0;
    int times = ci[course].time;
    for (i = 0; i < max_day_week; i++)
    {
        sum2 += class_two_day[semester][i];
        sum3 += class_three_day[semester][i];
    }
    average2 = sum2 / max_day_week;
    average3 = sum3 / max_day_week;
    for (j = 0; j < max_day_week; j++)
    {
        //calculate variance
        if (j == i && first_class[times] == 2)
        {
            sum2 += (class_two_day[semester][j] + 1 - average2) * (class_two_day[semester][j] + 1 - average2);
            sum3 += (class_three_day[semester][j] - average3) * (class_three_day[semester][j] - average3);
        }
        else if (j == i && first_class[times] == 3)
        {
            sum2 += (class_two_day[semester][j] - average2) * (class_two_day[semester][j] - average2);
            sum3 += (class_three_day[semester][j] + 1 - average3) * (class_three_day[semester][j] + 1 - average3);
        }
        else if (j == (i + 2 - 5 >= 0 ? i + 2 - 5 : i + 2) && second_class[times] == 2)
        {
            sum2 += (class_two_day[semester][j] + 1 - average2) * (class_two_day[semester][j] + 1 - average2);
            sum3 += (class_three_day[semester][j] - average3) * (class_three_day[semester][j] - average3);
        }
        else if (j == (i + 2 - 5 >= 0 ? i + 2 - 5 : i + 2) && second_class[times] == 3)
        {
            sum2 += (class_two_day[semester][j] - average2) * (class_two_day[semester][j] - average2);
            sum3 += (class_three_day[semester][j] + 1 - average3) * (class_three_day[semester][j] + 1 - average3);
        }
        else
        {
            sum2 += (class_two_day[semester][j] - average2) * (class_two_day[semester][j] - average2);
            sum3 += (class_three_day[semester][j] - average3) * (class_three_day[semester][j] - average3);
        }
    }
    return sum2 + sum3;
}

Status CSS::If_DayAvaliable(int semester, int curday, int course)
{
    int times = ci[course].time;
    if ((first_class[times] == 2 && class_two_day[semester][curday] >= 2) || (first_class[times] == 3 && class_three_day[semester][curday] >= 2) || (second_class[times] == 2 && class_two_day[semester][curday + 2 - 5 >= 0 ? curday + 2 - 5 : curday + 2] >= 2) || (second_class[times] == 3 && class_three_day[semester][curday + 2 - 5 >= 0 ? curday + 2 - 5 : curday + 2] >= 2))
        return NO;
    return YES;
}

int main()
{
    const char *file = "D:\\vscode\\C++\\.vscode\\test2.txt";
    CSS css(file);
}

// int main(int argc,char* argv[])
// {
//     if(argc>1)
//     {
//         const char *file = argv[1];
//         CSS css(file);
//     }
// }
