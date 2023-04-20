/* postfix.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;
	printf("[----- [Yongho Bae] [2020039008] -----]");

	do{// 무조건 한번은 실행
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I': //i 또는 I를 입력할 경우 Infix 입력
			getInfix();
			break;
		case 'p': case 'P': //p 또는 P를 입력할 경우 infix를 postfix로 변환
			toPostfix();
			break;
		case 'e': case 'E': //e 또는 E를 입력할 경우 postfix 계산
			evaluation();
			break;
		case 'd': case 'D': //d 또는 D를 입력할 경우 현황 출력
			debug();
			break;
		case 'r': case 'R': //r 또는 R을 입력할 경우 계산에 쓰는 변수들 초기화
			reset();
			break;
		case 'q': case 'Q': //q 또는 Q를 입력할 경우 switch문 종료
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); // q 또는 Q를 입력할 경우에 반복문 종료

	return 1;
}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
} //postfixStack에 push

char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];
    }
    return x;
} //postfixStack에서 pop

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
} //evalStack에 push

int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
} //evalStack에서 pop

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
} // 받은 문자의 종류 판단 후 리턴

precedence getPriority(char x)
{
	return getToken(x); 
} // getToken으로 받은 우선순위 리턴
/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}
/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */
/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		if(getPriority(*exp) == operand) //exp가 가리키는 문자가 operand이 경우 
		{
			x = *exp; // x에 *exp 대입
 			charCat(&x); // 피연사자이기 떄문에 바로 postfixExp에 저장
		}
 		else if(getPriority(*exp) == lparen) { //exp가 가리키는 문자가 lparen이 경우 
 			postfixPush(*exp); // '('를 postfixStack에 push
		}
 		else if(getPriority(*exp) == rparen) //exp가 가리키는 문자가 rparen이 경우 
		{
 			while((x = postfixPop()) != '(') {
 				charCat(&x);
			} // '('가 나올떄까지 postfixStack에서 pop하고 postfixExp에 저장
 		}
 		else // 그 외 operators의 경우
 		{
 			while(getPriority(postfixStack[postfixStackTop]) >=getPriority(*exp)) // postfixStack의 top에 있는 문자보다 exp의 문자의 우선순위가 낮을 경우 실행반복
 			{
 				x = postfixPop(); // postfixStack에서 pop후 x에 대입
 				charCat(&x); // postfixExp에 x 저장
 			}
 			postfixPush(*exp); // exp가 가리키는 문자 posstfixStack에 push
 		}
 		exp++; // 다음 문자 검사 
	}
 	while(postfixStackTop != -1)
	{
		 x = postfixPop();
 		charCat(&x);
 	} // postfixStack에 남아있는 문자들 postfixexp에 저장
}

void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

} //변환과정 ,계산결과 현황 출력

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
} //계산에 쓰는 스택,배열, 변수들 초기화

void evaluation()
{
	/* postfixExp, evalStack을 이용한 계산 */
	int opr1, opr2, i; // 변수 opr1, opr2, i 선언
	int length = strlen(postfixExp); // 변수 length 선언, postfixExp의 길이 대입
	char symbol; // 변수 symbol 선언
	evalStackTop = -1; // evalStack의 top을 -1로 초기화
	for(i = 0; i < length; i++)// postfixExp 길이만큼 반복
	{
		symbol = postfixExp[i]; //symbol에 postfixExp의 i번째 문자 대입
		if(getToken(symbol) == operand) {
			evalPush(symbol - '0');
		} //symbol이 피연산자면 evalStack에 문자에서 실제값으로 바꾼 피연산자를 push
		else { //연산자의 경우
			opr2 = evalPop(); // evalStack에 있는 피연산자 pop 후 opr2에 대입
			opr1 = evalPop(); // 이어서 evalStack에 있는 피연산자 pop 후 opr1에 대입
			switch(getToken(symbol)) {
				case plus: evalPush(opr1 + opr2); break;
				case minus: evalPush(opr1 - opr2); break;
				case times: evalPush(opr1 * opr2); break;
				case divide: evalPush(opr1 / opr2); break;
				default: break; //연산자종류에 따라서 계산 실행 후 evalStack에 push
			}
		}
	}
	evalResult = evalPop(); //evalStack에 있는 결과값 pop해서 evalResult에 대입
}