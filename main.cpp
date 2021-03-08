#include <iostream>
#include <cmath>

using namespace std;

int factorial(int n)
{
	int fact=1;
	for(int i=n; i>1; i--){
		fact *= i;
	}
	return fact;
}

int power(int x,
		  int n)
{
	if(n == 0) return 1;

	int y = 1;
	while(n > 1){
		if(n % 2 == 0){
			x *= x;
			n /= 2;
		}
		else{
			y *= x;
			x *= x;
			n--;
			n /=2;
		}
	}
	return x*y;
}

int str_to_int(char *num_char,
			   int len)
{
	int num = 0;
	for(int i=0; i<len; i++){
		num += power(10, i) * (*(num_char - i) - '0');
	}
	return num;
}

int priority(char op)
{
	switch(op)
	{
	case '!':
		return 0;
	case '^':
		return 1;
	case '*':
		return 2;
	case '/':
		return 2;
	case '+':
		return 3;
	case '-':
		return 3;
	case '\0':
		return 4;
	}
}

class double_stack
{
private:
	double array [128];
	int ptr = 0;
public:
	void push(double number)
	{
		array[ptr] = number;
		ptr++;
	}
	double pop()
	{
		ptr--;
		return array[ptr];
	}
	void calculate(char op);
};

void double_stack::calculate(char op)
{
	char temp;
	switch(op)
	{
	case '+':
		push(pop() + pop());
		break;
	case '-':
		temp = pop();
		push(pop() - temp);
		break;
	case '*':
		push(pop() * pop());
		break;
	case '/':
		temp = pop();
		push(pop() / temp);
		break;
	case '^':
		temp = pop();
		push(pow(pop(), temp));
		break;
	case '!':
		push(factorial(pop()));
		break;
	}
}

class char_stack
{
private:
	char array [128];
	int ptr = 0;
public:
	void push(char op)
	{
		array[ptr] = op;
		ptr++;
	}
	char pop()
	{
		if(ptr>0){
			ptr--;
			return array[ptr];
		}
		else return '\0';
	}
	char peek()
	{
		if(ptr>0){
			return array[ptr-1];
		}
		else return '\0';
	}
};

int main()
{
	char input [256];
	double_stack rpn;
	char_stack stack;

	cin>>input;

	int len = 0;
	int j = 0;
	for(int i=0; input[i-j] != '\0'; i++){
		switch(input[i])
		{
		case '\0':
			if(len != 0){
				rpn.push(str_to_int(&input[i-1], len));
				len = 0;
			}
			break;
		case '(':
			if(len != 0){
				rpn.push(str_to_int(&input[i-1], len));
				len = 0;
				for(char temp = stack.peek(); priority(temp) <= priority('*'); temp = stack.peek()){
					temp = stack.pop();
					rpn.calculate(temp);
				}
				stack.push('*');
			}
			else if(input[i-1] == ')'){
				for(char temp = stack.peek(); priority(temp) <= priority('*'); temp = stack.peek()){
					temp = stack.pop();
					rpn.calculate(temp);
				}
				stack.push('*');
			}
			stack.push('(');
			break;
		case ')':
			if(len != 0){
				rpn.push(str_to_int(&input[i-1], len));
				len = 0;
			}
			for(char temp = stack.pop(); temp != '('; temp = stack.pop()){
				rpn.calculate(temp);
			}
			break;
		default:
			if(input[i] >= '0' && input[i] <= '9'){
				len++;
			}
			else{
				if(len != 0){
					rpn.push(str_to_int(&input[i-1], len));
					len = 0;
				}
				for(char temp = stack.peek(); priority(temp) <= priority(input[i]); temp = stack.peek()){
					temp = stack.pop();
					rpn.calculate(temp);
				}
				stack.push(input[i]);
			}
			break;
		}
		j = 1;
	}

	for(char temp = stack.pop(); temp != '\0'; temp = stack.pop()){
		rpn.calculate(temp);
	}

	cout<<rpn.pop()<<endl;
	return main();
}
