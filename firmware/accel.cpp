#include "accel.h"

typedef unsigned char bit8;
int add(int a, int b)
{
  bit8 out;
  out = a+b; 
  return out;
}

int math_op::sub(int a, int b)
{
  return a-b;
}

int math_op::add(int a, int b)
{
  return a+b;
}

math_op::math_op(int start_in, int end_in)
{
  this->start = start_in;
  this->end = end_in;
}
