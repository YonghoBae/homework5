/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // 최대 큐사이즈 4로 설정

typedef char element;
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType; // 구조체 정의(큐,프론트,리어)


QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
	QueueType *cQ = createQueue(); //구조체 CQ 포인터 선언 후 createQueue에서 리턴된 큐의 주소값으로 초기화
	element data; // 스택 원소용 변수 data 선언
	char command; // 기능 입력용 변수 command 선언

	printf("[----- [Yongho Bae] [2020039008] -----]");

	do{// 한번은 무조건 실행
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //실행할 기능 입력

		switch(command) {
		case 'i': case 'I': //I또는 i를 입력할 경우
			data = getElement(); // getElement에서 리턴된 데이터를 data에 대입
			enQueue(cQ, data); // cQ에 data를 enqueue
			break;
		case 'd': case 'D': // d 또는 D를 입력할 경우
			deQueue(cQ, &data); // cQ에서 dequeue
			break; 
		case 'p': case 'P':
			printQ(cQ); // cQ출력
			break;
		case 'b': case 'B':
			debugQ(cQ); // cQ 디버그
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ); // cQ로 할당된 메모리 해제
			break;
		default: //다른키 입력할 경우
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); // q 또는 Q입력시 반복문 종료


	return 1;
}

QueueType *createQueue()
{
	QueueType *cQ; //구조체 cQ 포인터 변수 선언
	cQ = (QueueType *)malloc(sizeof(QueueType)); //cQ에 구조체 QueueType의 크기만큼 메모리 할당 후 메모리주소 대입
	cQ->front = 0; // front 0초기화
	cQ->rear = 0; // rear 0초기화
	return cQ; //cQ리턴
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1; //cQ가 비어있을 경우 종료
    free(cQ); //cQ에 할당된 메모리 해제
    return 1;
}

element getElement()
{
	element item; //변수 item 선언
	printf("Input element = ");
	scanf(" %c", &item); //item값 입력
	return item; //item값 리턴
}
//문자입력

/* complete the function */
int isEmpty(QueueType *cQ)
{
	if(cQ->front == cQ->rear){
		printf("error");
		return 1;
	} //비어있을 경우 에러 출력 후 1리턴
	return 0; //0리턴
}

/* complete the function */
int isFull(QueueType *cQ)
{
	if(cQ->rear == cQ->front){
		printf("error");
		return 1;
	} //가득찼을 경우 에러 출력 후 1리턴
	return 0;
}


/* complete the function */
void enQueue(QueueType *cQ, element item)
{
	cQ->rear = (cQ->rear + 1)%MAX_QUEUE_SIZE; // rear값이 최대사이즈를 넘어가는 경우 다시 0으로 돌아가게함. 그외에는 cQ->rear++과 동일
	if(isFull(cQ)==1){
		cQ->rear--;
	} //큐가 가득찼을 경우 위에서 증가했던 rear값을 다시 감소시킴
	else{
		cQ->queue[cQ->rear] = item;
	} //큐가 가득차지않았을 경우 item enqueue 
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)
{
	if(isEmpty(cQ)==0){ //비어있지 않을 경우
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; // front값이 최대사이즈를 넘어가는 경우 다시 0으로 돌아가게함. 그외에는 (cQ->front)++과 동일
	}
}


void printQ(QueueType *cQ)
{
	int i, first, last; //변수 i,first, last 선언

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //first에 front앞의 위치 대입
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;  //last에 rear앞의 위치 대입

	printf("Circular Queue : [");

	i = first; //i에 first 대입
	while(i != last){ //i가 last가 될떄까지 반복
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;
	} //큐에 있는 원소 모두 출력
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;
		} //front 위치 출력
		printf("  [%d] = %c\n", i, cQ->queue[i]); //CQ에 있는 원소값 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //현재 front, rear 값 출력
}