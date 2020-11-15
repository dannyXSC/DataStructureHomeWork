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
const int three_class[] = {0, 0, 0, 1, 0, 1, 2, 1, 2, 3};
const int two_class[] = {0, 0, 1, 0, 2, 1, 0, 2, 1, 0};
const int first_class[] = {0, 0, 2, 3, 2, 2, 3};
const int second_class[] = {0, 0, 0, 0, 2, 3, 3};

typedef class CourseSchedulingSystem
{
protected:
    //format
    //information of course
    CI ci[max_courseNumber]; //i didt want to apply for space
    CSI csi[max_courseNumber];
    //number of course
    int totalNumber; //total number of course
    int maxSemester;
    char (*classSchedule)[max_day_week][max_class_day][max_numberBit]; //schedule
    int (*class_two_day)[max_day_week];
    int (*class_three_day)[max_day_week];

public:
    CourseSchedulingSystem(const char *filename, int max_semester = 8); //initialize
    ~CourseSchedulingSystem();
    Status ReadInformation(const char *filename);
    Status SortCSI();
    Status Judge(int pos, int cur);  //to judge if csi[cur] can be insert in pos
    int GetPos(int origin, int cur); //get correct pos
    void ScheduleClass();
    int FindCorrectDay(int course);
} CSS;

CSS::CourseSchedulingSystem(const char *filename, int max_semester)
{
    int i, j, k;
    this->ReadInformation(filename);
    this->maxSemester = max_semester;                                                             //max semester
    this->classSchedule = new char[max_semester + 1][max_day_week][max_class_day][max_numberBit]; //classSchedule[i] means semester i's class schedule
    for (i = 0; i < max_semester + 1; i++)
    {
        for (j = 0; j < max_day_week; j++)
        {
            for (k = 0; k < max_class_day; k++)
            {
                classSchedule[i][j][k][0] = 0;
            }
        }
    }
    this->class_two_day = new int[max_semester + 1][max_day_week];
    this->class_three_day = new int[max_semester + 1][max_day_week];
    for (i = 0; i < max_semester + 1; i++)
    {
        for (j = 0; j < max_day_week; j++)
        {
            class_two_day[i][j] = 0;
            class_three_day[i][j] = 0;
        }
    }
    //statistical inDrgee and outDrgee
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
        }
    } //end of for
    this->SortCSI();
    this->ScheduleClass();

    
}
CSS::~CourseSchedulingSystem()
{
    delete[] classSchedule;
    delete[] class_three_day;
    delete[] class_two_day;
}

void CSS::ScheduleClass()
{
    int i, j, k;
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
        for (j = 0; j < max_day_week; j++)
        {
            for (k = 0; k < max_class_day; k++)
            {
                classSchedule[i][j][k][0] = 0;
            }
        }
    }
    for (i = 1; i <= this->maxSemester; i++)
    {
        for (j = 0; j < this->totalNumber; j++)
        {

            if (csi[j].semester == i)
            {

                int semester = csi[j].semester;
                int times = ci[j].time;
                //input schedule
                //find the correct day for j
                int day = FindCorrectDay(j); //day is the day if you need subscript you should -1
                //cout << "*" << ci[j].number << " " << csi[j].semester << " " << day << endl;
                int day2 = day + 2 - 5 >= 1 ? day + 2 - 5 : day + 2;
                //update two three class
                if(first_class[times]==2)
                {
                    class_two_day[semester][day-1] += 1;
                }
                else if (first_class[times] == 3)
                {
                    class_three_day[semester][day-1] += 1;
                }
                if (second_class[times] == 2)
                {
                    class_two_day[semester][day2-1] += 1;
                }
                else if (second_class[times] == 3)
                {
                    class_three_day[semester][day2-1] += 1;
                }
                //select the right class for j
                int t1;
                for (t1 = 0; first_class[times] == 2 && t1 < max_class_day; t1 += 2)
                {
                    if (classSchedule[semester][day - 1][t1][0] != 0)
                        continue;
                    else
                    {
                        strcpy(classSchedule[semester][day - 1][t1], ci[j].number);
                        //cout << semester << " " << day << " " << ci[j].number << endl;
                        break;
                    }
                }
                for (t1 = 1; first_class[times] == 3 && t1 < max_class_day; t1 += 2)
                {
                    if (classSchedule[semester][day - 1][t1][0] != 0)
                        continue;
                    else
                    {
                        strcpy(classSchedule[semester][day - 1][t1], ci[j].number);
                        //cout << semester << " " << day << " " << ci[j].number << endl;
                        break;
                    }
                }
                for (t1 = 0; second_class[times] == 2 && t1 < max_class_day; t1 += 2)
                {
                    if (classSchedule[semester][day2 - 1][t1][0] != 0)
                        continue;
                    else
                    {
                        strcpy(classSchedule[semester][day2 - 1][t1], ci[j].number);
                        //cout << semester << " " << day2 << " " << ci[j].number << endl;
                        break;
                    }
                }
                for (t1 = 1; second_class[times] == 3 && t1 < max_class_day; t1 += 2)
                {
                    if (classSchedule[semester][day2 - 1][t1][0] != 0)
                        continue;
                    else
                    {
                        strcpy(classSchedule[semester][day2 - 1][t1], ci[j].number);
                        //cout << semester << " " << day2 << " " << ci[j].number << endl;
                        break;
                    }
                }
            }
        }
    }
}

int CSS::FindCorrectDay(int course)
{
    int i, j;
    int semester = csi[course].semester; //semester
    int times = ci[course].time;         //course times of every semester
    double value = 1024;            //inf
    int pickedDay = -1;
    //calculate average
    double average2=0, average3=0, sum2=0, sum3=0;
    for (i = 0; i < max_day_week; i++)
    {
        sum2 += class_two_day[semester][i];
        sum3 += class_three_day[semester][i];
    }
    average2 = sum2 / max_day_week;
    average3 = sum3 / max_day_week;
    //cout << ci[course].number << " " << average2 << " " << average3 << endl;
    for (i = 0; i < max_day_week; i++)
    {
        //loop the first picked day
        sum2 = 0;
        sum3 = 0;

        if ((first_class[times] == 2 && class_two_day[semester][i] >= 2 )
        || (first_class[times] == 3 && class_three_day[semester][i] >= 2) 
        || (second_class[times] == 2 && class_two_day[semester][i + 2 - 5 >= 0 ? i + 2 - 5 : i + 2] >= 2) 
        || (second_class[times] == 3 && class_three_day[semester][i + 2 - 5 >= 0 ? i + 2 - 5 : i + 2] >= 2))
        {
            continue;
        }
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

        if (sum2 + sum3 < value)
        {
            pickedDay = i;
            value = sum2 + sum3;
        }

        //cout << ci[course].number << " " << i << " " << class_three_day[semester][i] << " " << value << " " << sum2 << " " << sum3 << endl;
    }
    //cout << ci[course].number << " " << pickedDay<< " " << value << endl;
    return pickedDay + 1; //pickedDay is day-1
}

Status CSS::SortCSI()
{
    int i, j;
    while (1)
    {
        //end state: doesnt exist one whose inDegree is more than 0
        for (i = 0; i < this->totalNumber; i++)
        {
            if (csi[i].inDegree > 0)
                break;
        }
        if (i == this->totalNumber)
            break;

        LinkedStack<int> sta[2]; //to distingush
        int pos = 1;
        //find: inDgree is 0 (but outDegree is more than 0 or initial inDegree is more than 0)
        //and it has not been picked
        //push them
        for (i = 0; i < this->totalNumber; i++)
        {
            if (csi[i].semester == 0 && csi[i].inDegree <= 0 && (csi[i].outDegree > 0 || ci[i].num_preCourse > 0))
            {
                sta[1].Push(i); //push
                //cout << "i1=" << i <<" "<< csi[i].inDegree << endl;
                csi[i].semester = GetPos(pos, i);
                this->ScheduleClass();
            }
        }

        //process
        int switcher = 1;
        int p;
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
                            csi[i].inDegree--; //adjust inDegree
                            if (csi[i].inDegree == 0)
                            {
                                sta[(switcher + 1) % 2].Push(i); //push
                                //cout << "i2=" << i << " " << csi[i].inDegree << endl;

                                //find pre is p and set pos and finally push them in queue
                                //first, determine pos
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
                                    //change i's pos7
                                    csi[i].semester = GetPos(pos, i);
                                    this->ScheduleClass();
                                }
                            }
                        }
                    }
                } //end of push
            }     //end of one generation

            //switch
            switcher = (switcher + 1) % 2;
        } //end of while //end of one group
    }     //end of while

    //sort independent course
    for (i = 0; i < this->totalNumber; i++)
    {
        if (csi[i].semester == 0)
        {
            csi[i].semester = GetPos(1, i);
            this->ScheduleClass();
        }
    }

    return OK;
}

Status CSS::Judge(int pos, int cur)
{
    int num_two = 0, num_three = 0;
    int i;
    for (i = 0; i < this->totalNumber; i++)
    {
        if (csi[i].semester == pos)
        {
            num_three += three_class[ci[i].time];
            num_two += two_class[ci[i].time];
        }
    }
    if (num_three + three_class[ci[cur].time] > 10 || num_two + two_class[ci[cur].time] > 10)
    {
        return NO;
    }
    else if (FindCorrectDay(cur)==0)
    {
        return NO;
    }
    else
    {
        return YES;
    }
}

int CSS::GetPos(int orign, int cur)
{
    int i = orign;
    while (i <= this->maxSemester && this->Judge(i, cur) == NO)
    {
        i++;
    }
    if (i > this->maxSemester)
    {
        return -1;
    }
    else
    {
        return i;
    }
}

Status CSS::ReadInformation(const char *filename)
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
    //record number of every course
    int i = 0;
    //read the first line
    in.getline(line, max_line);
    while (1)
    {
        in.getline(line, max_line);
        if (in.good() == 0)
            break;
        //input all by sscanf
        sscanf(line, "%s %s %d %d %[^\n]", (char*)(&ci[i].number),
               (char*)(&ci[i].name), &ci[i].time, &ci[i].limitTerm,
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

/*
2 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 3   10个2 10个3


*/

int main()
{
    const char *file = "D:\\vscode\\C++\\.vscode\\test.txt";
    CSS css(file);
}
