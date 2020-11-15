/*------------------------------------------------------------------------
sort
we can optimization:
	expand the number of digits
------------------------------------------------------------------------*/
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <iomanip>
#include <time.h>
#include <stdlib.h>
using namespace std;

//define a global variable
int num_of_swap;	//record the swap times of every sort 

#if 1
#ifndef _WIN32
#define _strcmpi strcasecmp
#endif
/*------------------------------------------------------------------------
函数名  :safe_cin_string
功能    :安全输入一个string
参数    :x 需要输入的变量
        tips 问题提示语
------------------------------------------------------------------------*/
void safe_cin_string(string &x, const char* tips)
{
    cout<<tips;
    cin>>x;
    return;
}
/*------------------------------------------------------------------------
函数名  :wait_for_end
功能    :等待输入一个字符串以结束(不区分大小写)
参数    :prompt 提示语
        key 结束标准字符串
------------------------------------------------------------------------*/
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
    cout<<"please press enter to continue..."<<endl;
    while(getchar()!='\n')
    ;
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
            cout << "input error, please re-enter" << endl;
            getchar();
            while (getchar() != '\n');
        }
        else if (x <= 0)
        {
            cout << "input error, please re-enter" << endl;
        }
        else if(x>toplimit)
        {
            cout<<"input exceeds upper limit("<<toplimit<<"), please re-enter"<<endl;
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
	int width;		   //每个菜单项的宽度

	bool if_sensitive; //标志 可选项中是否敏感大小写
	const char *key;   //可选择项

	bool if_exist_title; //标志 是否有标题
	const char *title;	 //存储标题内容

	bool if_exist_order;   //标志 是否有提示
	char order[MAX_ORDER]; //储存 提示(order)

	int item_nums; //menu中有多少个条目

	char border[MAX_BORDER]; //边框的符号

	/*暂时不用*/
	int arrangement; //0表示从上到下1表示从左到右
	int col;		 //表示多列显示

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
	const char display_menu();				  //输出表
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

void BubbleSort(int* arr,const int max_arr)
{
	int i,j;
	for(i=0;i<max_arr-1;i++)
	{
		for(j=max_arr-1;j>i;j--)
		{
			if(arr[j]<arr[j-1])
			{
				//swap j j-1
				int temp=arr[j];
				arr[j]=arr[j-1];
				arr[j-1]=temp;

				num_of_swap++;
			}
		}
	}
	return;
}

void SelectionSort(int* arr,const int max_arr)
{
	int i,j;
	for(i=0;i<max_arr-1;i++)
	{
		for(j=i+1;j<max_arr;j++)
		{
			if(arr[j]<arr[i])
			{
				//swap i j
				int temp=arr[j];
				arr[j]=arr[i];
				arr[i]=temp;

				num_of_swap++;
			}
		}
	}
	return;
}

void InsertSort(int* arr,const int max_arr)
{
	int i,j;
	for(i=1;i<max_arr;i++)
	{
		int temp=arr[i];
		for(j=i-1;j>=0;j--)
		{
			if(arr[j]>temp)
			{
				//move j
				arr[j+1]=arr[j];

				num_of_swap++;
			}
			else
			{
				//if find the right pos for temp 
				//then break;
				break;
			}
		}
		arr[j+1]=temp;
	}
	return;
}

void ShellSort(int* arr,const int max_arr)
{
	//shell sort is a optimization of insert sort;
	int gap=max_arr/2;
	int i,j,k;
	while(gap>0)
	{
		//stop condition: gap==0
		for(i=0;i<gap;i++)
		{
			//shell in gap
			for(j=i+gap;j<max_arr;j+=gap)
			{
				//ergodic from i+gap
				int temp=arr[j];	//record
				for(k=j-gap;k>=0;k-=gap)
				{
					if(arr[k]>temp)
					{
						//move
						arr[k+gap]=arr[k];

						num_of_swap++;
					}
					else
					{
						//find the right pos for temp
						break;
					}
				}
				//the right pos is k+gap
				//k is the first wrong pos 
				arr[k+gap]=temp;	//return the value;
			}
		}
		//narrow the gap;
		gap/=2;
	}
}

void FastSort(int* arr,int left,int right)
{
	if(left>=right)
	{
		//end condition left==right
		return;
	}
	//i point to the first 
	//j point to the last
	int i=left,j=right;
	//record value of left
	int temp=arr[i];
	while(i<j)
	{
		while(i<j&&arr[j]>=temp)
		{
			//if j>temp then j is in order
			j--;
		}
		//i=j or arr[j]<=temp
		if(i<j)
		{
			//swap i j
			arr[i]=arr[j];
			//pos i is in order 
			//so we move i
			//pos j will be filled
			i++;
			num_of_swap++;
		}
		while(i<j&&arr[i]<temp)
		{
			//if i<temp then i is in order;
			i++;
		}
		//i=j or arr[i]>=temp and arr[j]<=temp
		if(i<j)
		{
			//swap i j
			arr[j]=arr[i];
			//pos j is in order
			j--;
			num_of_swap++;
		}
	}//end of while
	//pos i/j is the right pos for temp
	//else is in order
	arr[i]=temp;
	//sort left to i-1 
	FastSort(arr,left,i-1);
	//sort i+1 to right
	FastSort(arr,i+1,right);
	return;
}

/*-------------------------------------------------------------------------------
 * accomplish a heap
-------------------------------------------------------------------------------*/
//to shiftup a node
static void HeapShiftUp(int* arr,int cur)
{
	if(cur==0)
	{
		//end condition cur is the root of the tree
		return;
	}
	//root of cur is cur/2
	int root=(cur-1)/2;
	if(arr[cur]<arr[root])
	{
		//swap root and cur
		swap(arr[cur],arr[root]);
		num_of_swap++;
	}
	//shiftup root
	HeapShiftUp(arr,root);
	return;
}
//to dropDown a node
static void HeapDropDown(int* arr,int cur,const int max_arr)
{
	int i=cur*2+1;	//represent left child
	int j=cur*2+2;	//represent right child
	if(i>=max_arr)	//the reason of adding '=' is that max_arr is the size of arr
	{
		//left children is invalid
		//cant do anything
		return;
	}
	else if (j>=max_arr)
	{
		//j==max_arr
		//right child is invalid
		//left child is valid
		if(arr[cur]>arr[i])
		{
			swap(arr[cur],arr[i]);
			num_of_swap++;
			//do dropdown again
			HeapDropDown(arr,i,max_arr);
		}
	}
	else
	{
		//both of children is valid
		int point=i;
		if(arr[i]>arr[j])point++;	//if right child is smaller then point represent right child
		if(arr[cur]>arr[point])
		{
			swap(arr[cur],arr[point]);
			num_of_swap++;
			HeapDropDown(arr, point, max_arr);
		}
	}
	return;
}
//to push a element to arr
static void push(int*arr ,const int now_size,int x)
{
	//push to the end
	arr[now_size]=x;
	//shiftUp
	HeapShiftUp(arr,now_size);
	return;
}
//to pop a element
static int pop(int*arr,const int now_size)
{
	int temp=arr[0];
	//use the last pos as root
	arr[0]=arr[now_size-1];
	//dropDown
	HeapDropDown(arr,0,now_size-1);	//size is now_size-1
	return temp;
}
//accomplish a sort
void HeapSort(int* arr,const int max_arr)
{
	int* heapArr=new int[max_arr];
	int i;
	//initialization the heap
	for(i=0;i<max_arr;i++)
	{
		push(heapArr,i,arr[i]);
	}
	//already finish sort
	for(i=0;i<max_arr;i++)
	{
		//adjust arr
		arr[i]=heapArr[i];
	}
	return;
}
//out put a heap
void OutPutHeap(int* arr,const int max_arr)
{
	int i;
	for(i=0;i<max_arr;i++)
	{
		cout<<pop(arr,max_arr-i)<<endl;
	}
	return;
}

/*------------------------------------------------------------------
 * accomplish merge sort 
 * l:refer to the first pos
 * r:refer to the last pos
------------------------------------------------------------------*/
void MergeSort(int* arr,int l,int r)
{
	if (l >= r)
	{
		//end condition
		return;
	}
	//create an array of size r-l+1
	int* temp=new int[r-1+1];
	//mid l-m m+1-r
	int m=(l+r)/2;
	//sort 
	if(l<m)
		MergeSort(arr, l, m);
	if(m+1<r)
		MergeSort(arr, m + 1, r);
	//l-m m+1-r is in order
	//begin to merge
	int i=l,j=m+1;
	int cnt=0;	//count of temp
	while(i<=m&&j<=r)
	{
		num_of_swap++;
		if (arr[i] < arr[j])
		{
			temp[cnt++]=arr[i];
			i++;
		}
		else
		{
			temp[cnt++]=arr[j];
			j++;
		}
	}//end of while
	//i>m||j>r
	while(i<=m)
	{
		temp[cnt++]=arr[i++];
	}//end of while
	while(j<=r)
	{
		temp[cnt++]=arr[j++];
	}//end of while
	//update array
	for(i=0;i<r-l+1;i++)
	{
		arr[l+i]=temp[i];
	}
	delete[] temp;
	return;
}

/*----------------------------------------------------------------
 * RadixSort 
----------------------------------------------------------------*/
static int get_maxBit(int* arr,const int max_arr,string* s)
{
	//we ensure every num in array greater than 0
	int i,j;
	int max_bit=0;	//record the max digits of the judgements
	for(i=0;i<max_arr;i++)	//i refer to arr[i]
	{
		int temp=1;	//divisor
		int bit=0;	//digits 
		while(arr[i]/temp)
		{
			temp*=10;
			bit++;
		}
		if(bit>max_bit)max_bit=bit;
	}//end of for
	for(i=0;i<max_arr;i++)
	{
		int temp=1;	//divisor
		int bit=0;	//digits 
		while(arr[i]/temp)
		{
			s[i]+=char((arr[i]%(temp*10))/temp+'0');
			temp*=10;
			bit++;
		}
		for(j=0;j<max_bit-bit;j++)
		{
			s[i]+="0";
		}

	}//end of for
	return max_bit;
}

static void BucketSort(string* s,const int max_arr,int bit)
{
	//product bucketsort on every bit
	int num[10];	//to record num of every digit
	int begin[10];	//to record begin pos of every digit
	memset(num,0,sizeof(num));
	memset(begin,0,sizeof(begin));
	string* temp=new string[max_arr];	//intermediate array
	int i;
	for(i=0;i<max_arr;i++)
	{
		num[s[i][bit]-'0']++;
	}
	for(i=1;i<10;i++)begin[i]=begin[i-1]+num[i-1];	//begin pos equal to (begin of i-1) + (num of i-1)
	for(i=0;i<max_arr;i++)
	{
		temp[begin[s[i][bit]-'0']++]=s[i];
	}
	for(i=0;i<max_arr;i++)
	{
		s[i]=temp[i];
	}
	delete []temp;
	return;
}

void RadixSort(int* arr,const int max_arr)
{
	//find the minimal num
	int i,j;
	int min=0x7fffffff;
	for(i=0;i<max_arr;i++)
	{
		if(arr[i]<min)min=arr[i];
	}
	//add -min+1 to every num
	for(i=0;i<max_arr;i++)
	{
		arr[i]+=1-min;
	}
	string* s=new string[max_arr];
	int max_bit;
	max_bit=get_maxBit(arr,max_arr,s);
	for(i=0;i<max_bit;i++)
	{
		BucketSort(s,max_arr,i);
	}
	//s is in order
	for(i=0;i<max_arr;i++)
	{
		arr[i]=0;
		int mul=1;
		//update array
		for(j=0;j<max_bit;j++)
		{
			arr[i]+=(s[i][j]-'0')*mul;
			mul*=10;
		}
	}
	//recovery array
	for(i=0;i<max_arr;i++)
	{
		arr[i]+=min-1;
	}
	delete []s;
	return;
}

//give simpleMenu
const char* menu[] = {
    "a.BubbleSort",
    "b.SelectionSrot",
    "c.InsertSort",
    "d.ShellSort",
    "e.FastSort",
    "f.HeapSort",
    "g.MergeSort",
    "h.RaixSort",
	"i.Compare",
	"j.Change the number of random",
	"k.Change the random array",
	"q.exit",
    NULL
};
const char* key = "qQabcdefghiABCDEFGHIjJKk";
const char* order = "a-k(q/Q:quit)";
const char* title = "Please select the operation you want to perform";

class Show_Sort{
protected:
	int random_num;
	int* random_ques;
	void display(int *arr, const int max_arr, void (*func)(int *, const int), const char *sortName);
	void display(int* arr,int l,int r,void (*func)(int *, int, int), const char *sortName);
	void give_ques(int *arr);
	void ask_for_showArr(int* ques);

public:
	Show_Sort();	//init
};

void Show_Sort::display(int* arr,const int max_arr,void (*func)(int *, const int), const char *sortName)
{
	time_t pre_time = 0;
	time_t lat_time = 0;
	num_of_swap = 0;
	pre_time = time(NULL);
	func(arr,max_arr);
	lat_time = time(NULL);
	//output
	cout << "Time of "<<sortName<<": " << lat_time - pre_time << endl;
	cout << "Times of swap: " << num_of_swap << endl;
}
void Show_Sort::display(int* arr,int l,int r,void (*func)(int *, int, int), const char *sortName)
{
	time_t pre_time = 0;
	time_t lat_time = 0;
	num_of_swap = 0;
	pre_time = time(NULL);
	func(arr,l,r);
	lat_time = time(NULL);
	//output
	cout << "Time of "<<sortName<<": " << lat_time - pre_time << endl;
	cout << "Times of swap: " << num_of_swap << endl;
	return;
}
void Show_Sort::give_ques(int* ques)
{
	int i;
	for (i = 0; i < this->random_num; i++)
	{
		ques[i] = random_ques[i];	
	}
}
void Show_Sort::ask_for_showArr(int* ques)
{
	const char str[] = "do you want to show array?\n[Y/N] : ";
	string choose;
	cout << endl;
	while (1)
	{
		safe_cin_string(choose, str);
		if(choose=="Y"||choose=="y")
		{
			int i;
			cout << "press enter to show old array.";
			getchar();
			while (getchar() != '\n')
				;
			for (i = 0; i < this->random_num;i++)
			{
				cout << this->random_ques[i] << endl;
			}

			cout << "press enter to show new array.";
			while (getchar() != '\n')
				;
			for (i = 0; i < this->random_num;i++)
			{
				cout << ques[i] << endl;
			}
			break;
		}
		else if(choose=="N"||choose=="n")
		{
			break;
		}
		else
		{
			cout << "input error! please input again!" << endl;
		}
	}
}

Show_Sort::Show_Sort()
{
	//initial time seed
	srand(time(NULL));
	int i;
	SIMPLE_MENU simpleMenu(menu, 0, 1, 1, key, title, order);

	//input random_num
	const char str[] = "Please enter the number of random numbers to generate[50000]: ";
	const int max_random_num = 50000;
	safe_cin_positive_integer(random_num, str, max_random_num);

	//initial random_ques
	this->random_ques = new int[random_num];
	for (i = 0; i < random_num; i++)
	{
		this->random_ques[i] = rand();	//generate 0-random_max
	}

	//ques is a temp array
	int *ques = new int[random_num];

	//loop variable
	int loop = 1;
	while (loop)
	{
		char sel;
		cout << endl;
		sel = simpleMenu.display_menu();	//call menu
		switch(sel)
		{
			case('Q'):
			{
				loop = 0;
				wait_for_enter();
				break;
			}
			case('A'):
			{
				this->give_ques(ques);
				this->display(ques, random_num, BubbleSort, "BubbleSort");
				this->ask_for_showArr(ques);
				wait_for_enter();
				break;
			}
			case('B'):
			{
				this->give_ques(ques);
				this->display(ques, random_num, SelectionSort, "SelectionSort");
				this->ask_for_showArr(ques);
				wait_for_enter();
				break;
			}
			case('C'):
			{
				//InsertSort
				this->give_ques(ques);
				this->display(ques, random_num, InsertSort, "InsertSort");
				this->ask_for_showArr(ques);
				wait_for_enter();
				break;
			}
			case('D'):
			{
				//ShellSort
				this->give_ques(ques);
				this->display(ques, random_num, ShellSort, "ShellSort");
				this->ask_for_showArr(ques);
				wait_for_enter();
				break;
			}
			case('E'):
			{
				//FastSort
				this->give_ques(ques);
				this->display(ques, 0,random_num-1, FastSort, "FastSort");
				this->ask_for_showArr(ques);
				wait_for_enter();
				break;
			}
			case('F'):
			{
				//HeapSort
				this->give_ques(ques);
				this->display(ques, random_num, HeapSort, "HeapSort");
				const char str[] = "do you want to show array?\n[Y/N] : ";
				string choose;
				cout << endl;
				while (1)
				{
					safe_cin_string(choose, str);
					if (choose == "Y" || choose == "y")
					{
						int i;
						cout << "press enter to show old array.";
						getchar();
						while (getchar() != '\n')
							;
						for (i = 0; i < this->random_num; i++)
						{
							cout << this->random_ques[i] << endl;
						}

						cout << "press enter to show new array.";
						while (getchar() != '\n')
							;
						OutPutHeap(ques, this->random_num);
						break;
					}
					else if (choose == "N" || choose == "n")
					{
						break;
					}
					else
					{
						cout << "input error! please input again!" << endl;
					}
				}
				wait_for_enter();
				break;
			}
			case('G'):
			{
				//MergeSort
				this->give_ques(ques);
				this->display(ques, 0,random_num-1, MergeSort, "MergeSort");
				this->ask_for_showArr(ques);
				wait_for_enter();
				break;
			}
			case('H'):
			{
				//RadixSort
				this->give_ques(ques);
				this->display(ques, random_num, RadixSort, "RadixSort");
				this->ask_for_showArr(ques);
				wait_for_enter();
				break;
			}
			case('I'):
			{
				this->give_ques(ques);
				this->display(ques, random_num, BubbleSort, "BubbleSort");
				cout << endl;

				this->give_ques(ques);
				this->display(ques, random_num, SelectionSort, "SelectionSort");
				cout << endl;

				this->give_ques(ques);
				this->display(ques, random_num, InsertSort, "InsertSort");
				cout << endl;

				this->give_ques(ques);
				this->display(ques, random_num, ShellSort, "ShellSort");
				cout << endl;

				this->give_ques(ques);
				this->display(ques, 0,random_num-1, FastSort, "FastSort");
				cout << endl;

				this->give_ques(ques);
				this->display(ques, random_num, HeapSort, "HeapSort");
				cout << endl;

				this->give_ques(ques);
				this->display(ques, 0,random_num-1, MergeSort, "MergeSort");
				cout << endl;

				this->give_ques(ques);
				this->display(ques, random_num, RadixSort, "RadixSort");
				cout << endl;

				wait_for_enter();
				break;
			}
			case('J'):
			{
				//change the number of random
				safe_cin_positive_integer(this->random_num, str, max_random_num);
				delete[] this->random_ques;
				delete[] ques;
				this->random_ques = new int[random_num];
				ques = new int[random_num];

				for (i = 0; i < random_num; i++)
				{
					this->random_ques[i] = rand(); 
				}
				break;
			}
			case('K'):
			{
				for (i = 0; i < random_num; i++)
				{
					this->random_ques[i] = rand(); 
				}

				wait_for_enter();
				break;
			}
		}
	}
	delete[] ques;
	delete[] this->random_ques;
}

int main()
{
	Show_Sort ss;
	return 0;
}




