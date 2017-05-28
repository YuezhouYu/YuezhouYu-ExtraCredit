#include <stdlib.h>
#include<iostream>
#define M 3         //ready queue number

using namespace std;

typedef struct pcb{
	int id;         
	int intime;     
	int priority;
	int starttime;
	int length;
	int endtime;
	char state;
	int ptime;
	struct pcb *next;
}*PCB, pcb;

void InitQueue(PCB *Q)
{
	*Q = (PCB)malloc(sizeof(pcb));
	(*Q)->next = NULL;
	(*Q)->endtime = 0;
	(*Q)->id = 0;
	(*Q)->intime = 0;
	(*Q)->length = 0;
	(*Q)->priority = 0;
	(*Q)->ptime = 0;
	(*Q)->starttime = 0;
	(*Q)->state = 0;
}

PCB ReadyQueue(PCB R, int i)
{
	InitQueue(&R);
	R->id = i + 1;
	R->priority = 5 - i;              //init priority
	R->ptime = i + 1;                 //init ptime
	R->state = 'R';
	printf("Q%d：\t\t%d\t\t%d\n", R->id, R->priority, R->ptime);
	return R;
}

void IdleQueue(PCB I, int n)
{
	int i;
	pcb *r, *s;
	r = I;

	cout << "Please input the informations of process(time priority length):" << endl;
	for (i = 0; i < n; i++)
	{
		//s = (PCB)malloc(sizeof(pcb));
		InitQueue(&s);
		s->next = NULL;
		s->id = i + 1;
		printf("process%d: ", s->id);
		scanf("%d%d%d", &s->intime, &s->priority, &s->length);
		r->next = s;
		r = s;
	}
}

void Print1(PCB Q)
{
	pcb *p;
	p = Q->next;

	cout << "process\ttime\tpriority\tlength" << endl;
	while (p != NULL)
	{
		printf("P%d\t", p->id);
		printf("%d\t%d\t\t%d\n", p->intime, p->priority, p->length);
		p = p->next;
	}
}

void Print2(PCB Q)
{
	pcb *p;
	p = Q->next;
	if (p == NULL)
		printf("\tNULL");
	else
	{
		while (p != NULL)
		{
			printf("\tP%d", p->id);
			p = p->next;
		}
	}
	printf("\n");
}

void Print3(PCB Q)
{
	pcb *p;
	p = Q->next;
	printf("----------------------------------------------\n");
	cout << "process\tstate\tintime\tstarttime\tlength\tpriority" << endl;
	while (p != NULL)
	{
		printf("P%d\t", p->id);
		printf("%c\t%d\t%d\t\t%d\t%d\n", p->state, p->intime, p->starttime, p->length, p->priority);
		p = p->next;
	}
	printf("----------------------------------------------\n");
}

void Insert(PCB R, pcb *cp)
{
	pcb *tail;
	tail = R;

	//add cp in the tail of queue
	while (tail->next)
	{
		tail = tail->next;
	}
	tail->next = cp;
	cp->next = NULL;
}

int FindQuery(PCB R[M])
{
	int i = 0, j = 0;
	for (i = 0; i < M; i++)
	{
		if (R[i]->next)
		{
			j = i;
			break;
		}
	}
	return j;
}

int Dispatch(PCB R, pcb *cp, PCB I, int time, pcb *pp)
{
	int r;
	pcb *p, *phead;
	phead = I;
	p = I->next;
	cp->starttime = time;                   //starttime of process is present time
	cp->length -= R->ptime;                 //the length is length of ptime
	cp->endtime = cp->starttime + R->ptime; 
	if (cp->length <= 0)
	{                 
		r = 1;                              //finish
		cp->endtime += cp->length;         
	}
	else
	{
		if (p)
		{
			//is there any processes can be preemptive
			while (p)
			{
				if ((p->intime < cp->endtime) && (p->priority > cp->priority))
				{    
					//preemptive process sucessful,backup p in pp
					pp->id = p->id;
					pp->intime = p->intime;
					pp->length = p->length;
					pp->priority = p->priority;
					pp->state = p->state;

					phead->next = p->next;                      //delete process from sqare queue
					cp->length += cp->endtime - pp->intime;     
					cp->endtime -= cp->endtime - pp->intime;    
					//join run queue
					r = -1;
					break;
				}
				else
				if (p->intime >= cp->endtime)
				{
					r = 0;                                  
					break;
				}
				p = p->next;
				phead = phead->next;
			}
		}
		else r = 0;                                            
	}
	return r;
}

void Convert(PCB Q, pcb *p)
{
	pcb *s, *t;
	s = Q;
	//t = (pcb*)malloc(sizeof(pcb));
	InitQueue(&t);
	if (p != NULL)
	{
		t->id = p->id;
		t->intime = p->intime;
		t->starttime = p->starttime;
		t->state = p->state;
		t->priority = p->priority;

		if (t->state == 'R')
			t->state = 'Y';

		s->next = t;
		t->next = NULL;
	}
	else
		s->next = NULL;
}

void PManagement(PCB I, PCB R[M], int n)
{
	PCB Y;      //run queue
	PCB B;      //obstruct queue
	PCB F;      //finish queue

	InitQueue(&Y);
	Y->state = 'Y';
	
	InitQueue(&F);
	F->state = 'F';
	
	InitQueue(&B);
	B->state = 'B';

	pcb *cp;                            //present process
	pcb *pp;                            //preemptive process 
	//pp = (PCB)malloc(sizeof(pcb));
	InitQueue(&pp);
	pp->priority = 0;                   
	pp->next = NULL;

	int time = 0;                       //present time
	int Finish = 0;                     
	int i, flag, j, r;

	while (Finish < n)
	{
		
		cp = I->next;
		if (cp && cp->intime <= time)
		{   
			flag = 1;
		}
		else
			flag = 0;

		if (flag)
		{
			i = 0;
			I->next = cp->next;                 
			Insert(R[0], cp);                   
			cp->priority = R[0]->priority;      
			cp->state = R[0]->state;            
			cp->starttime = time;
			cp = R[0]->next;
			printf("time:%d: P%d run。（P%d join Q%d）\n", time, cp->id, cp->id, R[0]->id);

			Convert(Y, cp);
			printf("run queue Y:");
			Print2(Y);
			Print3(Y);

			printf("ready queue R[0]:");
			Print2(R[0]);
			printf("finish queue F:");
			Print2(F);
			printf("obstruct queue B:");
			Print2(B);
		}
		else
		{
			i = FindQuery(R);                  
			cp = R[i]->next;
			printf("time:%d: P%d run.（Q%d run P%d）\n", time, cp->id, R[i]->id, cp->id);

			Convert(Y, cp);
			printf("run queue Y:");
			Print2(Y);
			Print3(Y);

			printf("ready queue R[%d]:", i);
			Print2(R[i]);

			printf("finish queue F:");
			Print2(F);

			if (cp->next)
				Convert(B, cp->next);
			else
				Convert(B, NULL);
			printf("obstruct queue B:");
			Print2(B);
		}

		r = Dispatch(R[i], cp, I, time, pp);    //cpu run process

		if (r == 1)
		{
			R[i]->next = cp->next;                
			Insert(F, cp);                        
			Finish++;
			printf("==============================================\n");
			printf("（time:%d: P%d finish。）\n", cp->endtime, cp->id);
		}

		else if (r == 0)
		{
			if (i < 2)
				j = i + 1;
			else
				j = i;

			R[i]->next = cp->next;              
			Insert(R[j], cp);                   
			cp->priority = R[j]->priority;
			printf("==============================================\n");
			printf("（time:%d: P%d ptime finish，join Q%d）\n", cp->endtime, cp->id, R[j]->id);

		}
		else if (r == -1)
		{
			R[i]->next = cp->next;
			Insert(R[i], cp);

			pp->next = R[0]->next;
			R[0]->next = pp;
			pp->priority = R[0]->priority;
			pp->state = R[0]->state;
			printf("time:%d: P%d is preemptived by P%d，join ready queue %d\n", cp->endtime, cp->id, pp->id, R[i]->id);

			printf("ready queue R[%d]\n", i);
			Print2(R[i]);
			printf("finish queue F:");
			Print2(F);
		}
		time = cp->endtime;
	}
	printf("finish queue F:");
	Print2(F);
}

void Print(PCB F)
{
	pcb *p;
	p = F->next;
	while (p)
	{
		printf("finish time of P%d\t：%d\n", p->id, p->endtime);
		p = p->next;
	}
}

int main(){
	PCB I;      //sqare queue
	PCB R[M];   //ready queue

	int i, n;

	printf("==============================================\n");
	printf("ready queue\tpriority\tptime（5 is the highest priority）\n");
	for (i = 0; i < M; i++){
		R[i] = ReadyQueue(R[i], i);
	}
	printf("==============================================\n");

	printf("Please input the number of process：");
	scanf("%d", &n);
	InitQueue(&I);          
	I->state = 'I';
	IdleQueue(I, n);       
	printf("----------------------------------------------\n");
	Print1(I);              
	printf("==============================================\n");

	PManagement(I, R, n);
	return 0;
}

