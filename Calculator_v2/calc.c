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

		//���� �����ڸ� ���
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
	stack operand = newStack(); // �ǿ����� ����

	stack operator = newStack(); // ������ ����

	double num = 0;

	double point = 0; //�Ҽ��� üũ

	int preNumCheck = 0; // ������ ���ڰ� �Դ��� üũ

	int minusCheck = 0; // -�����ڰ� �������� ������ üũ

	int plusCheck = 0; // +�����ڰ� �������� ������ üũ

	int closeCheck = 0; // ')'�� �������� üũ

	int errorCode = 0; // 0: ���� ����, 1: �߸��� ���� ����, 2: / 0 or % 0 ����

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

				num = pop(&operand); // �������� ���ڸ� ���ÿ� �־��ִ� ���¸� ����� �ֱ� ����
				
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
	// �������� ���ڷ� ������ �ʾ��� �� üũ, ')'�� ������ ���ڷ� ������ ������ ���
	if (preNumCheck == 0) {
		errorCode = 1;
	}

	//���� ���� ���� üũ
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
	queue exp = newQueue(); // ������ �� ť
	
	stack open = newStack(); // ��ȣ�� �˻�� ����

	char cur = getc(stdin);

	if (cur == '\n') return 0; 

	while (cur != '\n') {
		if (cur == ' ') {
			cur = getc(stdin);
			continue;
		}
		//��ȣ, ����, ������ �� �Է� �˻�
		if (('0' <= cur && cur <= '9') || cur == '(' || cur == ')' || cur == '+' || cur == '-' || cur == '*' || cur == '/' || cur == '%' || cur == '^' || cur == '!' || cur == '~' || cur == '.' || cur == 'l' || cur == 'o' || cur == 'g') {

			//��ȣ�� �˻�
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