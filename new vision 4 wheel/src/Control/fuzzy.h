#ifndef _FUZZY_H_
#define _FUZZY_H_

#include "mymath.h"
#define FMAX        (1000)    //ÓïÑÔÖµµÄÂú·ùÖµ

typedef struct
{
  unsigned int (*rule)[7];
  int *N1;
  int *N2;
  int *F1;
  int *F2;
  float *out_range;
  unsigned char filter_flag;
  Filter_1st_Str Fil;
}Fuzzy_Str;

void System_Fuzzy_init(void);
void lishudu(float e,float ec,float s,float sd,float sdi);
float Fuzzy(Fuzzy_Str *Fuz);
void subordinating(float input,float *input_range,int *output,int *Nn);

#endif