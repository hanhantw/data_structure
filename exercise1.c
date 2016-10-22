#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define STACK_SIZE 20	//把stack_size定義成「20」，之後修改這邊就可以了

typedef enum{lparen=0,rparen=1,plus=2,minus=3,times=4,divide=5,eos=6,operand=7} precedence;

char map[6]={'(', ')', '+', '-', '*', '/'};

int isp[6]={0, 19, 12, 12, 13, 13};
int icp[6]={20, 19, 12, 12, 13, 13};

typedef struct {
	bool is_char;
	int data;
} D;

precedence get_token(int *num, int *u, char *exp)	//num存放數字的地方,u次序ㄝ,exp運算式
{
	char s=exp[*u];	//exp是一個陣列，存運算式
	(*u)++;
	*num=(int)s-48;	//字符轉型成數字，'0'的編碼是48
	if (*num>-1 && *num<10) { //判斷是不是數字0-9
		while (exp[*u]>47 && exp[*u]<58)
			*num=*num*10+exp[(*u)++]-48;	//判斷數字用，123＝(1*10+2)*10+3
		return operand;	//判斷到非數字時就回傳前面判斷出的數字，不用num是因爲要分辨是真的數字還是符號(左右括號、加減乘除)的數字
	}
	switch(s){
		case '(':
			return lparen;
		case ')':
			return rparen;
		case '+':
			return plus;
		case '-':
			return minus;
		case '/':
			return divide;
		case '*':
			return times;
		case '\0':
			return eos;
	}
}

//把postfix的結果存進D這個structure
void print_D(bool is_char, int data, D *d, int *offset) {
	d[*offset].is_char=is_char; 	//offset：第幾個
	d[*offset].data=data;
	(*offset)++;
	if (is_char)
		printf("%c", map[data]);	//符號轉數字
	else
		printf("%d ", data);
	return;
}

void postfix(char *exp, D *d, int *offset)
{
	int op;
	precedence token;
	precedence stack[STACK_SIZE];
	int n = 0;
	int top = 0;
	
	for (token = get_token(&op, &n, exp); token != eos; token = get_token(&op, &n, exp)){
		if (token == operand)
			print_D(false, op, d, offset);	//d是一個array
		else if (token == rparen){
			while(top>0 && stack[top-1] != lparen)
				print_D(true, stack[--top], d, offset);	//--top:先減再存取top
			top--;	//top--:先存取top再減
		}
		else{
			while(top>0 && isp[stack[top-1]]>=icp[token])
				print_D(true, stack[--top], d, offset);
			stack[top++]=token;;
		}
	}
	while (true) {	//等同於while(top>0)
		if (top<=0)
			break;
		token=stack[--top];
		print_D(true, token, d, offset);
	}
	printf("\n");	//跳行
}
int eval(D *d, int size){
	int stack[STACK_SIZE];
	int op1,op2, offset;
	int n = 0;
	int top = 0;
	for (offset=0;offset<size;++offset) {
		D token=d[offset];
		if(!token.is_char)
			stack[top++]=token.data;
		else {
			op2 = stack[--top];
			op1 = stack[--top];
			switch((precedence)token.data){
				case plus:stack[top++]=op1+op2;
						  break;
				case minus:stack[top++]=op1-op2;
						  break;
				case times:stack[top++]=op1*op2;
						 	break;
				case divide:stack[top++]=op1/op2;
							break;
				}
		}
	}
	return stack[--top];
}
int main(void){
	char input[100];
	D d[STACK_SIZE];
	int offset=0;
	scanf("%s",input);
	postfix(input, d, &offset);
	int result=eval(d, offset);
	printf("%d\n", result);
	return 0;
}	
