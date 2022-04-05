#include <stdio.h>
int main(){ for (int t=0;; t++) putchar (t * (((t >> 12) | (t >> 8)) & (63 & (t >> 4)))); }