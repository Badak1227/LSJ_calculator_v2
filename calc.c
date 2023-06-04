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
	return error;
}

void calc(queue* exp) {
	stack operand = newStack(); // �ǿ����� ����

	stack operator = newStack(); // ������ ����

	double num = 0;

	int preNumCheck = 0; // ������ ���ڰ� �Դ��� üũ

	int minusCheck = 0; // -�����ڰ� �������� ������ üũ

	int plusCheck = 0; // +�����ڰ� �������� ������ üũ

	int closeCheck = 0; // ')'�� �������� üũ

	int errorCode = 0; // 0: ���� ����, 1: �߸��� ���� ����, 2: / 0 or % 0 ����

	while (!isEmptyQueue(exp) && errorCode == 0) {
		char cur = deQueue(exp);

		if ('0' <= cur && cur <= '9') {
			if (closeCheck == 1) {
				errorCode = 1;
				break;
			}

			num = num * 10 + (cur - '0');

			preNumCheck = 1;
			minusCheck = 0;
			plusCheck = 0;
			closeCheck = 0;
		}
		else {
			if (cur == '-') {
				if (preNumCheck == 0) {
					if (minusCheck == 1) {
						errorCode = 1;
						break;
					}

					push(-1, &operand);
					push('*', &operator);
					minusCheck = 1;
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
					minusCheck = 1;
					closeCheck = 0;
				}

				plusCheck = 0;
			}
			else if (cur == '+') {
				if (preNumCheck == 0) {
					if (plusCheck == 1) {
						errorCode = 1;
						break;
					}

					push(1, &operand);
					push('*', &operator);
					plusCheck = 1;
				}
				else {
					push(num, &operand);
					num = 0;

					if (!isEmptyStack(&operator)){
						char prev = top(&operator);
						if (prev == '-' || prev == '+' || prev == '*' || prev == '/' || prev == '%' || prev == '^') {
							errorCode = calculate(&operand, &operator, 1);
						}
					}

					
					push(cur, &operator);
					preNumCheck = 0;
					plusCheck = 1;
					closeCheck = 0;
				}

				minusCheck = 0;
			}
			else if (cur == '*' || cur == '/' || cur == '%') {
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
				plusCheck = 0;
				minusCheck = 0;
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
				plusCheck = 0;
				minusCheck = 0;
				closeCheck = 0;
			}
			else if (cur == '(') {
				if (preNumCheck == 1) {
					errorCode = 1;
					break;
				}
				
				push('(', &operator);

				plusCheck = 0;
				minusCheck = 0;
			}
			else if (cur == ')') {
				if (preNumCheck == 0) {
					errorCode = 1;
					break;
				}

				push(num, &operand);

				errorCode = calculate(&operand, &operator, 0);

				num = pop(&operand); // �������� ���ڸ� ���ÿ� �־��ִ� ���¸� ����� �ֱ� ����

				plusCheck = 0;
				minusCheck = 0;
				closeCheck = 1;
			}
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
	else {
		push(num, &operand);

		errorCode = calculate(&operand, &operator, 0);

		//��� ���� ���� üũ
		if (errorCode == 0) {
			double res = top(&operand);

			//�Ҽ��� ù° �ڸ����� ǥ��, ù°�ڸ��� 0�� ��� �Ҽ��� ǥ�� ����
			if (fmod(res * 10, 10) == 0) {
				printf("result: %.0lf\n", res);
			}
			else {
				printf("result: %.1lf\n", res);
			}
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
		if (('0' <= cur && cur <= '9') || cur == '(' || cur == ')' || cur == '+' || cur == '-' || cur == '*' || cur == '/' || cur == '%' || cur == '^') {

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