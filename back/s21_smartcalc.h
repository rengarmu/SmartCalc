#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define EPS 1E-7

#define OK 0
#define InvalidDIGIT -1     // больше одной точки
#define InvalidOPERATOR -2  // неправильный оператор
#define InvalidSYNTAX -3  // неправильная последовательность лексем

enum priority { FNULL, FDIGIT, FVAL, FOPN, FADD, FMULL, FPOW, FUNC, FCLS, ALL };

typedef struct {
  char *leks_ptr;  // указатель на начало лексемы
  int priority;    // приоритет лексемы
} leksem;

typedef struct {
  leksem mass_leks[255];  // массив из структур
  int leks_count;         // количество лексем
} leks_list;

int smartcalc(char *, double *);  // основная функция
int pars_leks(char *, leks_list *);  // разбирает строку на лексемы
int sqnc_valid(leks_list *);  // проверяет правильность последовательности
void polish_sort(
    leks_list *natation,
    leks_list *polish_nat);  // сортировка лексем по польской нотации
void find_result(leks_list *, double *);  // финальный рассчёт

void push_leks(leksem, leks_list *);  // пушим лексемы в сруктуру
leksem pop_leks(leks_list *);  // извлекаем лексемы из сруктуры
