#include <ctype.h> //isdigit() 함수를 사용하기 위한 헤더파일 선언
#include <stdio.h> //표준 입출력 함수를 사용하기 위한 헤더파일 선언
#include <stdlib.h> //동적 메모리 할당을 받기 위한 malloc()함수를 사용하기 위한 헤더파일 선언
#include <string.h> //strlen 함수를 선언하기 위한 헤더파일 선언

typedef struct { // 스택 정의
  int top;       // 스택 구성 요소
  double *data;  // 스택 구성 요소
  int capacity;  // 스택 구성 요소
} Stack; // 데이터 타입을 Stack로 정의

Stack *createStack(int capacity) { // 스택 초기화 함수
  Stack *stack = (Stack *)malloc(sizeof(Stack)); // 스택 동적 메모리 할당
  stack->capacity = capacity; // stack->capacity에 매개변수를 받아 저장
  stack->top = -1; // stack->top을 -1로 초기화
  stack->data = (double *)malloc(stack->capacity * sizeof(double)); // stack->data 동적 메모리 할당
  return stack; // stack 반환
}

int isEmpty(Stack *stack) { return stack->top == -1; } // 스택이 비어있는지 확인하는 함수

int isFull(Stack *stack) { return stack->top == stack->capacity - 1; } // 스택이 가득 찼는지 확인하는 함수

void push(Stack *stack, double item) {// 스택에 요소를 추가하는 함수
  if (isFull(stack)) // 스택이 비어있다면
    return; // 함수 종료
  stack->data[++stack->top] = item; // 스택이 비어있지 않다면 요소를 추가
}

double pop(Stack *stack) {// 스택에서 요소를 꺼내는 함수
  if (isEmpty(stack)) // 스택이 비어있다면
    return -1; // -1반환
  return stack->data[stack->top--]; // 스택이 비어있지 않다면 요소를 꺼내 반환하고 top을 감소
}

double peek(Stack *stack) {// 스택에서 가장 위의 요소를 확인하는 함수
  if (isEmpty(stack)) // 스택이 비어있다면
    return -1; // -1반환
  return stack->data[stack->top]; // 스택이 비어있지 않다면 요소를 꺼내 반환
}

int precedence(char op) { // 연산자 우선순위를 반환하는 함수
  if (op == '*' || op == '/') // 연산자가 곱셈 또는 나눗셈인 경우
    return 2; // 2 반환
  else if (op == '+' || op == '-') //연산자가 덧셈 또는 뺄셈인 경우
    return 1; // 1 반환
  else // 연산자가 아닌 경우
    return 0; // 0 반환
}

void calculate(Stack *operandStack, double op1, double op2, char op) { // 연산을 수행하여 operandStack에 결과를 추가
  switch (op) { // op의 값에 따라 다른 연산을 수행
  case '+': // op가 '+'인 경우
    push(operandStack, op1 + op2); // op1과 op2를 더한 값을 operandStack에 추가
    break; // switch문 종료
  case '-': // op가 '-'인 경우
    push(operandStack, op1 - op2); // op1과 op2를 뺀 값을 operandStack에 추가
    break; // switch문 종료
  case '*': // op가 '*'인 경우
    push(operandStack, op1 * op2); // op1과 op2를 곱한 값을 operandStack에 추가
    break; // switch문 종료
  case '/': // op가 '/'인 경우ㄴ
    push(operandStack, op1 / op2); // op1과 op2를 나눈 값을 operandStack에 추가
    break; // switch문 종료
  }
}
int minusCount(Stack *operatorStack, Stack *operandStack, char *input, int i, int n) { // '-'인 경우 연속된 '-'부호의 갯수를 세어 올바른 수식을 만들기 위한 함수
  int count = 0; // count를 0으로 초기화
  float op = 0; // op를 0으로 초기화
  while (input[i] == '-') { // '-'가 연속해서 나오는 경우 개수를 센다
    count++; // count를 1 증가
    i++; // i를 1 증가
  }
  if (n == 0) { //input[i] == '-'인 경우에 매개변수 n이 0인 경우
    if (count % 2 == 1) { // 마이너스 기호가 홀수개인 경우 음수로 처리
      push(operatorStack, '-'); // '-'를 스택에 추가하고 다음 문자로 이동
    }
    else { // 마이너스 기호가 짝수개인 경우
      push(operatorStack, '+'); // 음수가 아니므로 '+'를 추가
    }
  } else { // input[i]가 '*'나 '/'인 경우에 매개변수 n이 1이경우
    // 마이너스 기호가 홀수개인 경우 음수로 처리
    while (isdigit(input[i])) { //input[i]가 숫자인 경우
      op = op * 10 + (input[i] - '0'); // input[i] - '0'으로 숫자로 변환한 뒤 op에 자릿수를 맞추어 저장
      i++; // i를 1 증가시켜 다음 배열로 이동
    }
    if ((op > 32767) && (op < -32768)) { // op가 -32768~32767이 아닌 경우
      fprintf(stderr, "정수(-32768~32767)만을 사용하지 않았습니다.\n"); // 에러 메시지 출력
      exit(1); // 프로그램 종료
    }
    if(count==0); // count가 0인 경우 아무 연산 안함
    else if (count % 2 == 1) // '-'의 개수가 홀수인 경우
      push(operandStack, -op); // 음수로 변환한 op을 operandStack에 추가
    else // '-'의 개수가 짝수인 경우
      push(operandStack, op); // op을 operandStack에 추가
  }

  return i; // i 반환
}

double evaluate(char *input) { // 수식 평가 함수
  Stack *operandStack = createStack(strlen(input)); // 피연산자를 저장하는 스택
  Stack *operatorStack = createStack(strlen(input)); // 연산자를 저장하는 스택
  int i = 0; // 인덱스 변수 초기화

  while (input[i] != '\0') { // 문자열의 끝이 아닌 경우
    if (isdigit(input[i])) { // 현재 문자가 숫자인 경우 피연산자 스택에 추가
      double op = 0; // op를 0으로 초기화
      while (isdigit(input[i])) { // input[i]가 숫자인 경우
        op = op * 10 + (input[i] - '0'); // 연속된 숫자들을 하나의 숫자로 만든다
        i++; // i를 1 증가시켜 배열 이동
      }
      if ((op > 32767) && (op < -32768)) { // op가 -32768~32767이 아닌 경우
        fprintf(stderr, "정수(-32768~32767)만을 사용하지 않았습니다.\n"); // 에러 메시지 출력
        exit(1); // 프로그램 종료
      }
      push(operandStack, op); // 숫자를 operandStack에 푸시
    }

    else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') { // 현재 문자가 연산자인 경우
      while (!isEmpty(operatorStack) && precedence(peek(operatorStack)) >= precedence(input[i])) { // 연산자 스택이 비어있지 않고, 스택의 가장 위의 연산자의 우선순위가 높거나 같은 경우
        double op2 = pop(operandStack); // 스택에서 피연산자를 꺼내 op2를 초기화
        double op1 = pop(operandStack); // 스택에서 피연산자를 꺼내 op1을 초기화
        char ch = pop(operatorStack); // 스택에서 연산자를 꺼내 ch를 초기화
        calculate(operandStack, op1, op2, ch); // 연산을 수행하여 operandStack에 결과를 추가하는 함수 호출
      }
      if (input[i] == '-') // 마이너스 부호인 경우
        i = minusCount(operatorStack, operandStack, input, i, 0); // 마이너스 부호인 경우 연속된 '-'부호의 갯수를 센 뒤 i를 초기화
      else if (input[i] == '*' || input[i] == '/') { // input[i]가 '*'나 '/'인 경우
        push(operatorStack, input[i]); // '*'나 '/'를 연산자 스택에 푸시
        if (input[i + 1] == '-') { // '*'나 '/' 뒤에 입력된 연산자가 '-'인 경우
          i = minusCount(operatorStack, operandStack, input, i + 1, 1); // 마이너스 부호인 경우 연속된 '-'부호의 갯수를 센 뒤 i를 초기화
        } 
        else if(input[i + 1] == '+')// '*'나 '/' 뒤에 입력된 연산자가 '+'인 경우
            i+=2; // i를 2 증가시켜 숫자가 있는 배열로 이동
        // ex) 100*+2 = 200.000000
        else i++; // i를 1 증가
      }
      else { // input[i]가 '+'인 경우
        push(operatorStack, input[i]); // 현재 연산자를 스택에 추가
        i++; // i를 1 증가
      }
    }
  }

  while (!isEmpty(operatorStack)) { // 스택이 비어있지 않고 수식이 끝난 경우 남은 연산 실행
    double op2 = pop(operandStack); // 스택에서 피연산자를 꺼내 op2를 초기화
    double op1 = pop(operandStack); // 스택에서 피연산자를 꺼내 op1을 초기화
    char ch = pop(operatorStack); // 스택에서 연산자를 꺼내 ch를 초기화
    calculate(operandStack, op1, op2, ch); // 연산을 수행하여 operandStack에 결과를 추가하는 함수 호출
  }
  return pop(operandStack); // 최종 결과 반환
}

int main() { // main 함수 실행
  char input[100]; // 100글자 이내 입력받을 수식을 저장할 배열
  printf("수식을 입력하세요. 예)100+99-98*97/96+95-94/-93+92*91--90-89*88+87*86/87\n"); // 수식 입력 안내 메시지 출력
  scanf_s("%s", input); // 수식 입력 받기
  double result = evaluate(input); // 수식 평가 함수를 호출하여 결과를 result에 저장
  printf("결과: %.6lf\n", result); // 결과 출력

  return 0; // 프로그램 종료
}