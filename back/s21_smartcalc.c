#include "s21_smartcalc.h"

#include <stdio.h>

int smartcalc(char *str, double *result) {
  int err_code = OK;
  leks_list spolnat = {0};
  spolnat.leks_count = -1;
  err_code = pars_leks(str, &spolnat);
  if (err_code == OK) {
    err_code = sqnc_valid(&spolnat);
    if (err_code == OK) {
      leks_list polish_nat = {0};
      polish_nat.leks_count = -1;
      polish_sort(&spolnat, &polish_nat);
      find_result(&polish_nat, result);
    }
  }
  return err_code;
}

int pars_leks(char *str, leks_list *spolnat) {
  int err_code = OK;
  while (*str) {
    if (isdigit(*str) || *str == '.') {
      int point = 0;
      leksem tmp = {0};
      tmp.leks_ptr = str;
      tmp.priority = FDIGIT;
      push_leks(tmp, spolnat);
      while (isdigit(*str) || *str == '.') {
        ++str;
        if (*str == '.') ++point;
      }
      if (point > 1) {
        err_code = InvalidDIGIT;
        break;
      }
    } else if (*str == '+' || *str == '-' || *str == '*' || *str == '/' ||
               *str == '(' || *str == ')' || *str == '^') {
      leksem opt = {0};
      opt.leks_ptr = str;
      if (*str == '+' || *str == '-')
        opt.priority = FADD;
      else if (*str == '*' || *str == '/')
        opt.priority = FMULL;
      else if (*str == '^')
        opt.priority = FPOW;
      else if (*str == '(')
        opt.priority = FOPN;
      else if (*str == ')')
        opt.priority = FCLS;
      push_leks(opt, spolnat);
      ++str;
    } else if (*str == 'x') {
      leksem tmp = {0};
      tmp.leks_ptr = str;
      tmp.priority = FVAL;
      push_leks(tmp, spolnat);
      ++str;
    } else if (*str == 'm' || *str == 's' || *str == 'c' || *str == 'a' ||
               *str == 't' || *str == 'l') {
      char *names[10] = {"mod",  "cos",  "sin",  "tan", "acos",
                         "asin", "atan", "sqrt", "ln",  "log"};
      int pointer[10] = {0, 0, 0, 0, 2, 2, 0, 1, 1, 0};
      char ident[5] = {0};
      int k = 0;
      leksem tmp = {0};
      tmp.leks_ptr = str;
      while (*str >= 'a' && *str <= 'z') {
        ident[k++] = *str++;
      }
      int func = 0;
      for (int i = 0; i < 10; i++) {
        if (strcmp(ident, names[i]) == 0) {
          if (i == 0)
            tmp.priority = FMULL;
          else
            tmp.priority = FUNC;
          for (int j = pointer[i]; j > 0; j--) {
            ++tmp.leks_ptr;
          }
          push_leks(tmp, spolnat);
          ++func;
          break;
        }
      }
      if (func == 0) {
        err_code = InvalidOPERATOR;
        break;
      }
    } else if (*str == ' ') {
      ++str;
    } else {
      err_code = InvalidOPERATOR;
      break;
    }
  }
  return err_code;
}

int sqnc_valid(leks_list *spolnat) {
  int val_matrix[][ALL] = {
      {0, 1, 1, 0, 0, 0, 0, 0, 1},  // end
      {1, 0, 0, 1, 1, 1, 1, 0, 0},  // digit
      {1, 0, 0, 1, 1, 1, 1, 0, 0},  // val
      {1, 0, 0, 1, 1, 1, 1, 1, 0},  // opn
      {1, 1, 1, 1, 0, 0, 0, 0, 1},  // add
      {0, 1, 1, 0, 0, 0, 0, 0, 1},  // mull
      {0, 1, 1, 0, 0, 0, 0, 0, 1},  // pow
      {1, 0, 0, 1, 1, 1, 1, 0, 0},  // func
      {0, 1, 1, 0, 0, 0, 0, 0, 1}   // cls
  };
  int err_code = OK;
  int brackets_check = 0;
  for (int i = 0; i <= spolnat->leks_count; ++i) {
    if (spolnat->mass_leks[i].priority == FOPN) ++brackets_check;
    if (spolnat->mass_leks[i].priority == FCLS) --brackets_check;
    if (brackets_check < 0) {
      err_code = InvalidSYNTAX;
    }
  }
  if (brackets_check != 0) err_code = InvalidSYNTAX;
  if (err_code == OK) {
    if (val_matrix[spolnat->mass_leks[0].priority][0] == 0 ||
        val_matrix[0][spolnat->mass_leks[spolnat->leks_count].priority] == 0)
      err_code = InvalidSYNTAX;
    for (int i = 1; i < spolnat->leks_count; ++i) {
      if (val_matrix[spolnat->mass_leks[i].priority]
                    [spolnat->mass_leks[i - 1].priority] == 0) {
        err_code = InvalidSYNTAX;
        break;
      }
    }
  }

  return err_code;
}

void push_leks(leksem tmp, leks_list *spolnat) {
  spolnat->leks_count++;
  spolnat->mass_leks[spolnat->leks_count] = tmp;
}

leksem pop_leks(leks_list *buff) {
  leksem tmp = {0};
  tmp = buff->mass_leks[buff->leks_count];
  buff->leks_count--;
  return tmp;
}

void polish_sort(leks_list *spolnat, leks_list *polish_nat) {
  leks_list buff = {0};
  buff.leks_count = -1;
  int i = 0;
  while (spolnat->mass_leks[i].priority) {
    if (spolnat->mass_leks[i].priority == FDIGIT ||
        spolnat->mass_leks[i].priority == FVAL) {
      push_leks(spolnat->mass_leks[i], polish_nat);
    } else if (spolnat->mass_leks[i].priority == FOPN) {
      push_leks(spolnat->mass_leks[i], &buff);
    } else if (spolnat->mass_leks[i].priority > FOPN &&
               spolnat->mass_leks[i].priority < FCLS) {
      if ((spolnat->mass_leks[i].priority == FADD && i == 0) ||
          (spolnat->mass_leks[i].priority == FADD && i != 0 &&
           spolnat->mass_leks[i - 1].priority == FOPN)) {
        leksem tmp = {"0", FDIGIT};
        push_leks(tmp, polish_nat);
      }
      while ((buff.mass_leks[buff.leks_count].priority >=
              spolnat->mass_leks[i].priority) &&
             buff.leks_count != -1 &&
             !(buff.mass_leks[buff.leks_count].priority == FPOW &&
               spolnat->mass_leks[i].priority == FPOW)) {
        push_leks(pop_leks(&buff), polish_nat);
      }
      push_leks(spolnat->mass_leks[i], &buff);
    } else if (spolnat->mass_leks[i].priority == FCLS) {
      while (buff.mass_leks[buff.leks_count].priority != FOPN) {
        push_leks(pop_leks(&buff), polish_nat);
      }
      pop_leks(&buff);
    }
    ++i;
  }
  while (buff.leks_count != -1) {
    push_leks(pop_leks(&buff), polish_nat);
  }
}

void find_result(leks_list *polish_nat, double *result) {
  double d_digit[136] = {0};
  int k = -1;
  for (int i = 0; i <= polish_nat->leks_count; ++i) {
    if (polish_nat->mass_leks[i].priority == FDIGIT) {
      double digit = strtod(polish_nat->mass_leks[i].leks_ptr, NULL);
      d_digit[++k] = digit;
    } else if (polish_nat->mass_leks[i].priority == FVAL) {
      d_digit[++k] = *result;
    } else if (*polish_nat->mass_leks[i].leks_ptr == '+') {
      d_digit[k - 1] = d_digit[k - 1] + d_digit[k];
      --k;
    } else if (*polish_nat->mass_leks[i].leks_ptr == '-') {
      d_digit[k - 1] = d_digit[k - 1] - d_digit[k];
      --k;
    } else if (*polish_nat->mass_leks[i].leks_ptr == '*') {
      d_digit[k - 1] = d_digit[k - 1] * d_digit[k];
      --k;
    } else if (*polish_nat->mass_leks[i].leks_ptr == '/') {
      d_digit[k - 1] = d_digit[k - 1] / d_digit[k];
      --k;
    } else if (*polish_nat->mass_leks[i].leks_ptr == 'm') {
      d_digit[k - 1] = fmod(d_digit[k - 1], d_digit[k]);
      --k;
    } else if (*polish_nat->mass_leks[i].leks_ptr == '^') {
      d_digit[k - 1] = pow(d_digit[k - 1], d_digit[k]);
      --k;
    } else if (*polish_nat->mass_leks[i].leks_ptr == 's') {
      d_digit[k] = sin(d_digit[k]);
    } else if (*polish_nat->mass_leks[i].leks_ptr == 'c') {
      d_digit[k] = cos(d_digit[k]);
    } else if (*polish_nat->mass_leks[i].leks_ptr == 't') {
      d_digit[k] = tan(d_digit[k]);
    } else if (*polish_nat->mass_leks[i].leks_ptr == 'i') {
      d_digit[k] = asin(d_digit[k]);
    } else if (*polish_nat->mass_leks[i].leks_ptr == 'o') {
      d_digit[k] = acos(d_digit[k]);
    } else if (*polish_nat->mass_leks[i].leks_ptr == 'a') {
      d_digit[k] = atan(d_digit[k]);
    } else if (*polish_nat->mass_leks[i].leks_ptr == 'q') {
      d_digit[k] = sqrt(d_digit[k]);
    } else if (*polish_nat->mass_leks[i].leks_ptr == 'n') {
      d_digit[k] = log(d_digit[k]);
    } else if (*polish_nat->mass_leks[i].leks_ptr == 'l') {
      d_digit[k] = log10(d_digit[k]);
    }
  }
  *result = d_digit[0];
}
