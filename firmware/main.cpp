// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// A simple Sieve of Eratosthenes

#include "firmware.h"
#include "accel.h"

int main(void)
{
  char const *s = "Hello world!\n";
  int c;
  int a = 5;
  int b = 1; 
  math_op my_inst(a, b);

  c = my_inst.add(a, b);
  print_chr(c+48);
  print_chr('\n');
  c = my_inst.sub(a, b);
  print_chr(c+48);
  print_chr('\n');
  print_str(s);
  //stream();
  data_redir();
  return 0;

}  
