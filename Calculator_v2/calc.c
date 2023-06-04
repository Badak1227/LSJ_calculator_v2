#include <stdio.h>
#include <math.h>
#include "queue.h"
#include "stack.h"

int calculate(stack* operand, stack* operator, int n) {
	int error = 0;

	while (!isEmptyStack(operator)) {

		char sign = pop(operator);
		if (sign == '(') break;

		double num2 = pop(operand), num1 = pop(operand);
		if (sign == '+') {
			push(num1 + num2, operand);
		}
		else if (sign == '-') {
			push(num1 - num2, operand);
		}
		else if (sign == '*') {
			push(num1 * num2, operand);
		}
		else if (sign == '/') {
			if (num2 == 0) {
				error = 2;
				printf("ERROR: Div By 0\n");
				break;
			}
			push(num1 / num2, operand);
		}
		else if (sign == '%') {
			if (num2 == 0) {
				printf("ERROR: Mod By 0\n");
				error = 2;
				break;
			}
			push(fmod(num1, num2), operand);
		}
		else if (sign == '^') {
			if (num1 == 1) {
				push(1, operand);
			}
			else {
				push(pow(num1, num2), operand);
			}
		}

		//이전 연산자만 계산
		if (n) {
			break;
		}
	}

	if (!isEmptyStack(operator)) {
		if (top(operator) == '1') {
			pop(operator);

			int num = pop(operand);

			if (num < 1) {
				printf("ERROR: log10(n), n Less Than 1");
				error = 2;
			}
			push(log10(num), operand);
		}
		else if (top(operator) == '2') {
			pop(operator);

			int num = pop(operand);
			if (num < 1) {
				printf("ERROR: log2(n), n Less Than 1");
				error = 2;
			}
			push(log2(num), operand);
		}
	}

	return error;
}

void calc(queue* exp) {
	stack operand = newStack(); // 피연산자 스택

	stack operator = newStack(); // 연산자 스택

	double num = 0;

	double point = 0; //소수점 체크

	int preNumCheck = 0; // 이전에 숫자가 왔는지 체크

	int minusCheck = 0; // -연산자가 연속으로 오는지 체크

	int plusCheck = 0; // +연산자가 연속으로 오는지 체크

	int closeCheck = 0; // ')'로 끝났는지 체크

	int errorCode = 0; // 0: 에러 없음, 1: 잘못된 수식 에러, 2: / 0 or % 0 에러

	while (!isEmptyQueue(exp) && errorCode == 0) {
		char cur = deQueue(exp);

		if (cur == '.') {
			if (preNumCheck == 0 || point > 0) {
				errorCode = 1;
				break;
			}

			point = 1;
		}
		else if (cur == '+') {
			if (preNumCheck == 0) {
				if (plusCheck == 1) {
					errorCode = 1;
					break;
				}

				push(1, &operand);
				push('*', &operator);
			}
			else {
				push(num, &operand);
				num = 0;

				if (!isEmptyStack(&operator)) {
					char prev = top(&operator);
					if (prev == '-' || prev == '+' || prev == '*' || prev == '/' || prev == '%' || prev == '^') {
						errorCode = calculate(&operand, &operator, 1);
					}
				}


				push(cur, &operator);
				preNumCheck = 0;
				closeCheck = 0;
			}

			point = 0;
			plusCheck = 1;
			minusCheck = 0;
		}
		else if (cur == '-') {
			if (preNumCheck == 0) {
				if (minusCheck == 1) {
					errorCode = 1;
					break;
				}

				push(-1, &operand);
				push('*', &operator);
			}
			else {
				push(num, &operand);
				num = 0;

				if (!isEmptyStack(&operator)) {
					char prev = top(&operator);
					if (prev == '-' || prev == '+' || prev == '*' || prev == '/' || prev == '%' || prev == '^') {
						errorCode = calculate(&operand, &operator, 1);
					}
				}

				push(cur, &operator);
				preNumCheck = 0;
				
				closeCheck = 0;
			}

			point = 0;
			plusCheck = 0;
			minusCheck = 1;
		}
		else if ('0' <= cur && cur <= '9') {
			if (closeCheck == 1) {
				errorCode = 1;
				break;
			}
			
			if (point > 0) {
				point *= 10;
				num = num + (cur - '0')/ point;
			}
			if (point == 0) {
				num = num * 10 + (cur - '0');
			}

			preNumCheck = 1;
			minusCheck = 0;
			plusCheck = 0;
			closeCheck = 0;
		}
		else {
			if (cur == '*' || cur == '/' || cur == '%') {
				if (preNumCheck == 0) {
					errorCode = 1;
					break;
				}
				
				push(num, &operand);
				num = 0;
				if (!isEmptyStack(&operator)) {
					char prev = top(&operator);
					if (prev == '^' || prev == '*' || prev == '/' || prev == '%') {
						errorCode = calculate(&operand, &operator, 1);
					}
				}
				
				push(cur, &operator);
				
				preNumCheck = 0;
				closeCheck = 0;
			}
			else if (cur == '^') {
				if (preNumCheck == 0) {
					errorCode = 1;
					break;
				}

				push(num, &operand);
				num = 0;
				push(cur, &operator);

				preNumCheck = 0;
				closeCheck = 0;
			}
			else if (cur == '(') {
				if (preNumCheck == 1) {
					errorCode = 1;
					break;
				}
				
				push('(', &operator);
			}
			else if (cur == ')') {
				if (preNumCheck == 0) {
					errorCode = 1;
					break;
				}

				push(num, &operand);

				errorCode = calculate(&operand, &operator, 0);

				num = pop(&operand); // 마지막에 숫자를 스택에 넣어주는 형태를 만들어 주기 위함
				
				closeCheck = 1;
			}
			else if (cur == 'l') {
				errorCode = 1;

				if (preNumCheck == 1) break;

				if (isEmptyQueue(exp)) break;
				cur = deQueue(exp);

				if (cur == 'o') {
					if (isEmptyQueue(exp)) break;
					cur = deQueue(exp);

					if (cur == 'g') {
						if (isEmptyQueue(exp)) break;
						cur = deQueue(exp);

						if (cur == '1') {
							if (isEmptyQueue(exp)) break;
							cur = deQueue(exp);

							if (cur == '0') {
								if (isEmptyQueue(exp)) break;
								cur = deQueue(exp);

								if (cur == '(') {
									
									push('1', &operator);
									push('(', &operator);

									errorCode = 0;
								}
							}
						}
						else if (cur == '2') {
							if (isEmptyQueue(exp)) break;
							cur = deQueue(exp);

							if (cur == '(') {
								
								push('2', &operator);
								push('(', &operator);

								errorCode = 0;
							}
						}
					}
				}
			}
			else {
				errorCode = 1;
				break;
			}

			point = 0;
			plusCheck = 0;
			minusCheck = 0;
		}
	}
	// 마지막이 숫자로 끝나지 않았을 시 체크, ')'로 끝나도 숫자로 끝나는 것으로 취급
	if (preNumCheck == 0) {
		errorCode = 1;
	}

	//수식 형태 오류 체크
	if (errorCode == 1) {
		printf("ERROR: Invalid Experiment\n");
	}
	else if (errorCode == 2) {

	}
	else {
		push(num, &operand);

		errorCode = calculate(&operand, &operator, 0);

		if (errorCode == 0) {
			double res = top(&operand);
			printf("result: %.2lf\n", round(res*100)/100);
		}
	}

	freeStack(&operand);
	freeStack(&operator);
}

int input() {
	queue exp = newQueue(); // 수식이 들어갈 큐
	
	stack open = newStack(); // 괄호쌍 검사용 스택

	char cur = getc(stdin);

	if (cur == '\n') return 0; 

	while (cur != '\n') {
		if (cur == ' ') {
			cur = getc(stdin);
			continue;
		}
		//괄호, 숫자, 연산자 외 입력 검사
		if (('0' <= cur && cur <= '9') || cur == '(' || cur == ')' || cur == '+' || cur == '-' || cur == '*' || cur == '/' || cur == '%' || cur == '^' || cur == '!' || cur == '~' || cur == '.' || cur == 'l' || cur == 'o' || cur == 'g') {

			//괄호쌍 검사
			if (cur == '(') {
				push('(', &open);
			}
			else if (cur == ')') {
				if (isEmptyStack(&open)) {
					printf("ERROR: Invalid Pair Of Parentheses\n");

					while (getc(stdin) != '\n');

					freeQueue(&exp);
					return 1;
				}
				else {
					pop(&open);
				}
			}

			enQueue(cur, &exp);
			cur = getc(stdin);
		}
		else {
			printf("ERROR: Not An Operand or Operator\n");

			while (getc(stdin) != '\n');

			freeQueue(&exp);
			return 1;
		}
	}
	
	if (!isEmptyStack(&open)) {
		printf("ERROR: Invalid Pair Of Parentheses\n");
		
		freeStack(&open);
		freeQueue(&exp);
		return 1;
	}

	calc(&exp);
	freeQueue(&exp);
	return 1;
}