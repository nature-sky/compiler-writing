#include <stdio.h>

#define IF   "A"
#define THEN "B"
#define ELSE "C"
#define CHAR(x) x[0]

int
f(char c) { /* input precedence function */
  if (c == '=')             return 24;
  if (c == CHAR(IF))        return 23;
  if (c == CHAR(THEN))      return 4;
  if (c == CHAR(ELSE))      return 6;
  if (c == '|')             return 7;
  if (c == '&')             return 9;
  if (c == '!')             return 22;
  if (c == '<' || c == '>') return 12;
  if (c == '+' || c == '-') return 14;
  if (c == '*' || c == '/') return 16;
  if (c == '^')             return 21;
  if (c == '%')             return 20; /* unary minus */
  if (c == '(')             return 27;
  if (c == ')' || c == 0)   return 0;
  return 25;
}

int
g(char c) { /* stack precedence function */
  if (c == '=')             return 1;
  if (c == CHAR(IF))        return 2;
  if (c == CHAR(THEN))      return 3;
  if (c == CHAR(ELSE))      return 5;
  if (c == '|')             return 8;
  if (c == '&')             return 10;
  if (c == '!')             return 11;
  if (c == '<' || c == '>') return 13;
  if (c == '+' || c == '-') return 15;
  if (c == '*' || c == '/') return 17;
  if (c == '^')             return 18;
  if (c == '%')             return 19;
  if (c == '(')             return 0;
  return 26;
}

int
h(char f, char s) { /* character-pair matrix h */
  return ((f == '=' ||
           f == CHAR(IF) || f == CHAR(THEN) || f == CHAR(ELSE) ||
           f == '|' || f == '&' || f == '!' ||
           f == '<' || f == '>' ||
           f == '+' || f == '-' || f == '*' || f == '/' || f == '^' ||
           f == '%' || f == '(') ^
          (s == '=' ||
           s == CHAR(THEN) || s == CHAR(ELSE) ||
           s == '|' || s == '&' ||
           s == '<' || s == '>' ||
           s == '+' || s == '-' || s == '*' || s == '/' || s == '^' ||
           s == ')' || s == 0));
}

int
op(char c) {
  return c == '=' || c == CHAR(IF) || c == CHAR(THEN) || c == CHAR(ELSE) ||
      c == '|' || c == '&' || c == '!' ||
      c == '<' || c == '>' || c == '+' || c == '-' || c == '*' || c == '/' ||
      c == '^' || c == '%' || c == '(' || c == ')' || c == 0;
}

int
unary(char c) {
  return c == '!' || c == CHAR(THEN) || c == '%';
}

typedef struct {
  char op;
  int l, r;
} nd_t;

int
main(void) {
  /* operator and operand stack, input and output string */
  char r[100] = {'('}, in[] = IF "a<b" THEN "c=d=%" IF "e" THEN "f" ELSE "g",
       out[100] = {0}, p = '(', loop = 0;
  nd_t t[100] = {0}, d;
  int i = 0, len = sizeof(in) / sizeof(in[0]), ri = 1, ti = 1, n[100], ni = 0;
  for (; i < len && ri && h(p, in[i]); loop || (p = in[i++]), loop = 0)
    if (!op(in[i])) t[ti] = (nd_t) {in[i]}, n[ni++] = ti++;
    else if (g(r[ri-1]) < f(in[i])) r[ri++] = in[i];
    else if (g(r[ri-1]) > f(in[i]) && (loop = 1))
      if (unary(r[ri-1])) t[ti] = (nd_t) {r[--ri], n[ni-1]}, n[ni-1] = ti++;
      else       t[ti] = (nd_t) {r[--ri], n[ni-2], n[--ni]}, n[ni-1] = ti++;
    else ri--;
  if (i < len && !h(p, in[i]))     puts("INVALID SYMBOL PAIR");
  else if ((i < len) == (ri == 0)) puts("INVALID EXPRESSION");
  else while (ni-- && (d = t[n[ni]], 1) || !putchar('\n'))
    putchar(d.op), d.r && (n[ni++] = d.r), d.l && (n[ni++] = d.l);
  return 0;
}
