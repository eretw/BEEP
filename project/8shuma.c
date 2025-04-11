#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define N 3
//#define N 4
//记录层数的全局变量
int layer = 0;
 
//保存初始表盘的初态和末态
static int Pstart[N][N] = { 2,3,1,5,0,8,4,6,7 };
static int Pend[N][N] = { 1,2,3,8,0,4,7,6,5 };
//static int Pstart[N][N] = { 1,3,0,4,5,2,7,8,9,6,11,12,13,10,14,15 };
//static int Pend[N][N] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };
 
//定义每个结点数据类型
typedef struct PNode {
	int status;       //与末态表盘之间的错位个数
	int Ppoint[N][N];  //记录每个小格的数据
}PNode;
 
//定义open表和close表的结构(采用队列形式)
//队列的定义
typedef struct ListNode {  //定义链表结点
	PNode* pNode;      //数据项
	struct ListNode* List_next;  //指向下一结点的指针域
}ListNode, *pListNode;
typedef struct Queue {    //定义队列相当于两个指针分别控制链表的头尾
	pListNode qHead;	  //指向队列的头	
	pListNode qTail;      //指向队列的尾
}Queue;
Queue* Open_List;      //open表
Queue* Close_List;     //close表
//ListNode* Lhead;
//队列的操作
//创建表盘结点
PNode* CreatePNode() {
	PNode* p = (PNode*)malloc(sizeof(PNode));
	p->status = 0;
	return p;
}
//创建链表结点
ListNode* CreatNode(PNode* p) { //PNode* p p是指向数据结构体的指针
	ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
	if (!newNode) {
		printf("分配内存空间失败");
		system("pause");
		exit(1);
	}
	newNode->List_next = NULL;
	newNode->pNode = p;
	return newNode;
}
//队列初始化,相当于创建首元结点
void QueueInit(Queue* q) {      //Queue* q是指向队列的指针
	assert(q);
	q->qHead=CreatNode(NULL);
	q->qTail = q->qHead;
}
//入队
void QueuePush(Queue* q,PNode* p) {
	assert(q);
	q->qTail->List_next=CreatNode(p);
	q->qTail = q->qTail->List_next;
}
//出队,将首元结点后的一个结点指针出队并返回
ListNode* QueuePop(Queue* q) {
	pListNode dNode = q->qHead->List_next;        //首元结点后的第一个结点
	if (dNode) {            //存在一个结点
		q->qHead->List_next = dNode->List_next;
		if (q->qHead->List_next == NULL) {        //队列只有一个结点
			q->qTail = q->qHead;
		}
	}
	return dNode;
}
//判断队列是否为空
int QueueEmpty(Queue* q) {
	return NULL == q->qHead->List_next;
}
//计算表的结点个数
int ListLength(ListNode* l) {
	int count=0;   //记录个数
	//首元结点不算一个结点
	l = l->List_next;
	while (l!=NULL) {
		count++;
		l = l->List_next;
	}
	return count;
}
 
//定义评价函数(错位棋牌个数)
void judgefun1(PNode* p) {
	p->status = 0;
	for (int m = 0;m < N;m++) {
		for (int n = 0;n < N;n++) {
			if (p->Ppoint[m][n] != Pend[m][n]) {
				if (p->Ppoint[m][n] != 0) {
					p->status++;
				}
			}
		}
	}
	p->status = p->status + layer;
}
//定义评价函数(错位棋牌数到达目标位置最少步数)     *****************************????????????
void judgefun2(PNode* p) {
	p->status = 0;
	int i = 0;
	int j = 0;
		do {
			if (p->Ppoint[i][j] == 0) {
				if (j < N - 1) {
					j++;
				}
				else {
					j = 0;
					i++;
				}
				continue;
			}
			for (int m = 0;m < N;m++) {
				for (int n = 0;n < N;n++) {
					if (p->Ppoint[i][j] == Pend[m][n]) {
						p->status = p->status + abs(m - i) + abs(n - j);
					}
				}
			}
			if (j < N - 1) {
				j++;
			}
			else {
				j = 0;
				i++;
			}
		} while (i < N);
	p->status = p->status + layer;
}
//判断是否生成目标结点
bool judgeTarget(PNode* p) {
	for (int m = 0;m < N;m++) {
		for (int n = 0;n < N;n++) {
			if(p->Ppoint[m][n]!=Pend[m][n]){
				return 0;
			}
		}
	}
	return 1;
}
 
//定义空白格子移动顺序同时判断是否生成目标结点
//左下右上
int move(Queue* q,PNode* p) {        //q为要入队的队列(OPEN表),p为待扩展的结点
	//找到0的位置
	int m;
	int n;
	int t;                //交换中间变量
	PNode* ptmp[4];
	layer++;
	//定位0的位置
	for (m = 0;m < N;m++) {
		for (n = 0;n < N;n++) {
			if (p->Ppoint[m][n] == 0) {
				break;
			}
		}
		if (n < N) {
			break;
		}
	}
	//左移
	if (n != 0) {      //只要不是第一列
		//交换点
		ptmp[0] = CreatePNode();
		for (int j = 0;j < N;j++) {
			for (int k = 0;k < N;k++) {
				ptmp[0]->Ppoint[j][k] = p->Ppoint[j][k];
			}
		}
		t = ptmp[0]->Ppoint[m][n];
		ptmp[0]->Ppoint[m][n] = ptmp[0]->Ppoint[m][n - 1];
		ptmp[0]->Ppoint[m][n-1] = t;
		//judgefun1(ptmp[0]);
		judgefun2(ptmp[0]);
		QueuePush(Open_List, ptmp[0]);     //扩展结点入队
		//判断扩展结点是否是目标结点
		if (judgeTarget(ptmp[0])) {
			//将此结点入CLOSE表
			QueuePush(Close_List, ptmp[0]);
			return 0;
		}
	}
	//下
	if (m != N-1) {      //只要不在第N行
		ptmp[1] = CreatePNode();
		for (int j = 0;j < N;j++) {
			for (int k = 0;k < N;k++) {
				ptmp[1]->Ppoint[j][k] = p->Ppoint[j][k];
			}
		}
		t = ptmp[1]->Ppoint[m][n];
		ptmp[1]->Ppoint[m][n] = ptmp[1]->Ppoint[m + 1][n];
		ptmp[1]->Ppoint[m + 1][n] = t;
		//judgefun1(ptmp[1]);
		judgefun2(ptmp[1]);
		QueuePush(Open_List, ptmp[1]);
		if (judgeTarget(ptmp[1])) {
			//将此结点入CLOSE表
			QueuePush(Close_List, ptmp[1]);
			return 0;
		}
	}
	//右
	if (n != N - 1) {        //只要不在第N列
		ptmp[2] = CreatePNode();
		for (int j = 0;j < N;j++) {
			for (int k = 0;k < N;k++) {
				ptmp[2]->Ppoint[j][k] = p->Ppoint[j][k];
			}
		}
		t = ptmp[2]->Ppoint[m][n];
		ptmp[2]->Ppoint[m][n] = ptmp[2]->Ppoint[m][n + 1];
		ptmp[2]->Ppoint[m][n + 1] = t;
		//计算估计函数的值
		//judgefun1(ptmp[2]);
		judgefun2(ptmp[2]);
		QueuePush(Open_List, ptmp[2]);
		if (judgeTarget(ptmp[2])) {
			//将此结点入CLOSE表
			QueuePush(Close_List, ptmp[2]);
			return 0;
		}
	}
	//上
	if (m != 0) {
		ptmp[3] = CreatePNode();
		for (int j = 0;j < N;j++) {
			for (int k = 0;k < N;k++) {
				ptmp[3]->Ppoint[j][k] = p->Ppoint[j][k];
			}
		}
		t = ptmp[3]->Ppoint[m][n];
		ptmp[3]->Ppoint[m][n] = ptmp[3]->Ppoint[m - 1][n];
		ptmp[3]->Ppoint[m - 1][n] = t;
		//judgefun1(ptmp[3]);
		judgefun2(ptmp[3]);
		QueuePush(Open_List, ptmp[3]);
		if (judgeTarget(ptmp[3])) {
			//将此结点入CLOSE表
			QueuePush(Close_List, ptmp[3]);
			return 0;
		}
	}
	return 1;
}
 
//排序表：分治法实现
ListNode* list_spilt(ListNode* head) {  //Queue* q                     //传入head->next分裂链表并且返回第二个子链表的首指针
	if (NULL == head) {                  //没有元素可以分裂
		return head;
	}
	ListNode* tmp;                      //用于记录前一个表的表尾
	ListNode* slow = head;
	ListNode* fast = head;              //一个快指针一个慢指针用于定位中间结点
 
	while (fast) {
		fast = fast->List_next;
		if (fast) {
			fast = fast->List_next;
		}                                    //相当于fast移动两次slow移动一次
		if (NULL == fast) {
			break;
		}
		slow = slow->List_next;
	}
	tmp = slow;
	slow = slow->List_next;            //指向第二个子链表的首
	tmp->List_next = NULL;             //断链
	return slow;
}
 
ListNode* merge(ListNode* head1, ListNode* head2) {               //合并两个链表，并且返回排序完成之后的指针
	if (NULL == head1)
		return head1;
	if (NULL == head2)
		return head2;
 
	ListNode head;                   //记录排序链表的首元节点
	ListNode* tail = &head;           
 
	//尾插法实现
	while (head1 && head2) {
		if (head1->pNode->status < head2->pNode->status) {
			tail->List_next = head1;
			head1 = head1->List_next;
		}
		else {
			tail->List_next = head2;
			head2 = head2->List_next;
		}
		tail = tail->List_next;
	}
 
	if (head1) {
		tail->List_next = head1;
	}
	if (head2) {
		tail->List_next = head2;
	}
	return head.List_next;
}
 
ListNode* merge_sort(ListNode* head) {                  //传入head->next
	if (head==NULL || head->List_next==NULL) {      //分裂到只包含一个结点
		return head;
	}
 
	ListNode* head1 = head;
	ListNode* head2 = list_spilt(head);
 
	//对链表分别进行分裂
	head1 = merge_sort(head1);
	head2 = merge_sort(head2);
	//对链表合并
	return merge(head1, head2);
}
 
 
 
int main() {
	int steps = 1;
	int target=-1;
	//初始化
	/*
	   建立只包含初始状态结点s的搜索图G
	   OPEN={s}
	   CLOSE={}
	*/
	//先将两表（队列）初始化
	Open_List = (Queue *)malloc(sizeof(Queue));
	Close_List = (Queue*)malloc(sizeof(Queue));
	QueueInit(Open_List);
	QueueInit(Close_List);
	//将s插入open表
	PNode* s = (PNode*)malloc(sizeof(PNode));
	for (int j = 0;j < N;j++) {
		for (int k = 0;k < N;k++) {
			s->Ppoint[j][k] = Pstart[j][k];
		}
	}
	//s->Ppoint = Pstart;   //存入数组
	QueuePush(Open_List, s);   //入队
 
	//搜索循环（循环条件是扩展结点包含目标结点）
	while (target != 0) {      //当target=1时找到目标结点
		//取出OPEN表首结点作为扩展结点
		ListNode* extendNode = QueuePop(Open_List);
		//将其移入CLOSE表中
		QueuePush(Close_List, extendNode->pNode);
		//扩展结点插入OPEN表中
		target = move(Open_List, extendNode->pNode);
		//对每个子节点算其估计函数(在move函数中实现)
		//排序OPEN
		//ListNode* Lhead = Open_List->qHead->List_next;
		Open_List->qHead->List_next = merge_sort(Open_List->qHead->List_next);
	}
	printf("%d数码问题:空格移动位置步骤为:\n",8);
	//找到目标结点之后输出CLOSE队列找到相应的路径
	Close_List->qHead = Close_List->qHead->List_next;
	while (Close_List->qHead != NULL) {
		printf("-----steps%d-----\n", steps++);
		for (int i = 0;i < N;i++) {
			for (int j = 0;j < N;j++) {
				printf("%d  ", Close_List->qHead->pNode->Ppoint[i][j]);
			}
			printf("\n");
		}
		Close_List->qHead = Close_List->qHead->List_next;
		printf("---------------");
		printf("\n");
 
	}
	system("pause");
	return 0;
}
 
 
 
 
 