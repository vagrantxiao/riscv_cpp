// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// A simple Sieve of Eratosthenes

#include "firmware.h"

static void stats_print_dec(unsigned int val, int digits, bool zero_pad)
{
	char buffer[32];
	char *p = buffer;
	while (val || digits > 0) {
		if (val)
			*(p++) = '0' + val % 10;
		else
			*(p++) = zero_pad ? '0' : ' ';
		val = val / 10;
		digits--;
	}
	while (p != buffer) {
		if (p[-1] == ' ' && p[-2] == ' ') p[-1] = '.';
		print_chr(*(--p));
	}
        print_chr('\n');
}


void stream(void) 
{
  int din=0;
  int i;
  //float x=0;
  
  //x = 1.43456;
  unsigned int num_cycles, num_instr;
  __asm__ volatile ("rdcycle %0; rdinstret %1;" : "=r"(num_cycles), "=r"(num_instr));
  print_str("before write_stream");
  stats_print_dec(num_cycles, 8, false);

  for(i=0; i<100; i++)
  {
    //print_dec(i);
    //print_str("\n");
    write_word(i);
  }

   __asm__ volatile ("rdcycle %0; rdinstret %1;" : "=r"(num_cycles), "=r"(num_instr));
  print_str("after write_stream");
  stats_print_dec(num_cycles, 8, false);

 
  for(i=0; i<100; i++)
  {
    
    //print_str("we got ");
    din = din+read_word();
    //x = x + 1;
    //print_str("float: ");
    //print_float(x);
    //print_str("\n");
    //print_dec(din);
   // print_str("\n");
  }
  __asm__ volatile ("rdcycle %0; rdinstret %1;" : "=r"(num_cycles), "=r"(num_instr));
  print_str("after read_stream");
  stats_print_dec(num_cycles, 8, false);


  
}

// find the max from 3 integers
static unsigned char find_max_new( unsigned char in0, unsigned char in1, unsigned char in2 )
{

  if (in0 > in1)
  {
    if (in0 > in2)
    	return in0;

    else
    	return in2;
  }
  else
  {
    if (in1 > in2)
    	return in1;
    else
    	return in2;
  }
}

// find the min from 3 integers
static unsigned char find_min_new( unsigned char in0, unsigned char in1, unsigned char in2 )
{
  if (in0 < in1)
  {
    if (in0 < in2)
      return in0;
    else
      return in2;
  }
  else
  {
    if (in1 < in2)
      return in1;
    else
      return in2;
  }
}




static void loop_atom(void)
{
  unsigned int input_lo;
  unsigned int input_mi;
  unsigned int input_hi;

  unsigned char triangle_2d_x0;
  unsigned char triangle_2d_y0;
  unsigned char triangle_2d_z0;
  unsigned char triangle_2d_x1;
  unsigned char triangle_2d_y1;
  unsigned char triangle_2d_z1;
  unsigned char triangle_2d_x2;
  unsigned char triangle_2d_y2;
  unsigned char triangle_2d_z2;
  unsigned char triangle_2d_z;
  unsigned int tmp;

  int cw;
  static unsigned char max_min[5]={0, 0, 0, 0, 0};
  static int max_index[1]={0};

  //input_lo = Input_1.read();
  //input_mi = Input_1.read();
  //input_hi = Input_1.read();
  input_lo = read_word();
  input_mi = read_word();
  input_hi = read_word();

  triangle_2d_x0 = (unsigned char)((input_lo      ) & 0xff);
  triangle_2d_y0 = (unsigned char)((input_lo >> 8 ) & 0xff);
  triangle_2d_z0 = (unsigned char)((input_lo >> 16) & 0xff);
  triangle_2d_x1 = (unsigned char)((input_lo >> 24) & 0xff);
  triangle_2d_y1 = (unsigned char)((input_mi      ) & 0xff);
  triangle_2d_z1 = (unsigned char)((input_mi >> 8 ) & 0xff);
  triangle_2d_x2 = (unsigned char)((input_mi >> 16) & 0xff);
  triangle_2d_y2 = (unsigned char)((input_mi >> 24) & 0xff);
  triangle_2d_z2 = (unsigned char)((input_hi      ) & 0xff);



  triangle_2d_z = triangle_2d_z0 / 3 + triangle_2d_z1 / 3 + triangle_2d_z2 / 3;


  cw = (triangle_2d_x2 - triangle_2d_x0) * (triangle_2d_y1 - triangle_2d_y0)
       - (triangle_2d_y2 - triangle_2d_y0) * (triangle_2d_x1 - triangle_2d_x0);


  if(cw == 0 ){
    tmp = 1;
    tmp = tmp + (((unsigned int)triangle_2d_x0) << 8);
    tmp = tmp + (((unsigned int)triangle_2d_y0) << 16);
    tmp = tmp + (((unsigned int)triangle_2d_x1) << 24);
    write_word(tmp);
    //Output_1.write(tmp);

    tmp = (unsigned int) triangle_2d_y1;
    tmp = tmp + (((unsigned int)triangle_2d_x2) << 8);
    tmp = tmp + (((unsigned int)triangle_2d_y2) << 16);
    tmp = tmp + (((unsigned int)triangle_2d_z)  << 24);
    write_word(tmp);
    //Output_1.write(tmp);

    tmp = (unsigned int) max_index[0];
    tmp = tmp + (((unsigned int)max_min[0]) << 16);
    tmp = tmp + (((unsigned int)max_min[1]) << 24);
    write_word(tmp);
    //Output_1.write(tmp);

    tmp =  (unsigned int)max_min[2];
    tmp = tmp + (((unsigned int)max_min[3]) << 8);
    tmp = tmp + (((unsigned int)max_min[4]) << 16);
    //tmp(31,24) = 0;
    write_word(tmp);
    //Output_1.write(tmp);

    return;
  }


  if ( cw < 0 )
  {
    unsigned char tmp_x, tmp_y;

    tmp_x = triangle_2d_x0;
    tmp_y = triangle_2d_y0;

    triangle_2d_x0 = triangle_2d_x1;
    triangle_2d_y0 = triangle_2d_y1;

    triangle_2d_x1 = tmp_x;
    triangle_2d_y1 = tmp_y;

  }


  // find the rectangle bounds of 2D triangles
  max_min[0] = find_min_new( triangle_2d_x0, triangle_2d_x1, triangle_2d_x2 );
  max_min[1] = find_max_new( triangle_2d_x0, triangle_2d_x1, triangle_2d_x2 );
  max_min[2] = find_min_new( triangle_2d_y0, triangle_2d_y1, triangle_2d_y2 );
  max_min[3] = find_max_new( triangle_2d_y0, triangle_2d_y1, triangle_2d_y2 );
  max_min[4] = max_min[1] - max_min[0];





  // calculate index for searching pixels
  max_index[0] = (max_min[1] - max_min[0]) * (max_min[3] - max_min[2]);

  tmp = 0;
  tmp = tmp + (((unsigned int)triangle_2d_x0) << 8);
  tmp = tmp + (((unsigned int)triangle_2d_y0) << 16);
  tmp = tmp + (((unsigned int)triangle_2d_x1) << 24);
  write_word(tmp);
  //Output_1.write(tmp);

  tmp = (unsigned int) triangle_2d_y1;
  tmp = tmp + (((unsigned int)triangle_2d_x2) << 8);
  tmp = tmp + (((unsigned int)triangle_2d_y2) << 16);
  tmp = tmp + (((unsigned int)triangle_2d_z) << 24);
  write_word(tmp);
  //Output_1.write(tmp);



  tmp = (unsigned int)max_index[0];
  tmp = tmp + (((unsigned int)max_min[0]) << 16);
  tmp = tmp + (((unsigned int)max_min[1]) << 24);
  write_word(tmp);
  //Output_1.write(tmp);

  tmp = (unsigned int) max_min[2];
  tmp = tmp + (((unsigned int)max_min[3]) << 8);
  tmp = tmp + (((unsigned int)max_min[4]) << 16);
  //tmp= 0;
  write_word(tmp);
  //Output_1.write(tmp);

  return;
}


void data_redir(void)
{
  for(int i=0; i<3192; i++)
  {
    loop_atom();
  }

}
 
