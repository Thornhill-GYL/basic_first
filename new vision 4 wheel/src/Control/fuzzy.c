#include "fuzzy.h"

/*输出量U语言值特征点*/ 
float UFF_dP[7] ={0,25,50,75,100,125,120};
float UFF_sP[7] ={0,25,50,75,100,125,120};
float UFF_I[7] ={0.2,0.4,0.8,1.5,2.0,2.5};
float UFF_dD[7] ={0,150,300,450,600,750,900};
float UFF_dDD[7];
float UFF_Angel[7];
float UFF_sI[7];
float UFF_sI_limit[7];
float UFF_main_line[7] = {-7,-3,-1,0,1,3,7};

//e,speed 变化范围点 
//float   PFF[4]={0,800 ,1900 ,3500};	//方向误差(角速度)
//float   DFF[4]={0,400,1100,2000};		//方向误差导数
float   PFF[4]={0,20 ,40 ,68};	//方向误差
float   DFF[4]={0,7,17,30};		//方向误差导数
float   SFF[4]={0,0.1,0.2,0.3};	//速度比
float   SSFF[4]={0,10 ,35 ,75};	//速度导数
float   SDFF[4]={0,0.15,0.35,0.6}; //减速积分

int   PF[2],DF[2],SF[2],SSF[2],SDF[2];   //偏差,偏差微分隶属度
int   Pn,Dn,Sn,SSn,SDn;   //角标

Fuzzy_Str Fuz_Dir_P;
Fuzzy_Str Fuz_Dir_D;
Fuzzy_Str Fuz_Dir_DD;
Fuzzy_Str Fuz_Speed_P;
Fuzzy_Str Fuz_Angle_min;
Fuzzy_Str Fuz_Speed_I;
Fuzzy_Str Fuz_Speed_I_limit;
Fuzzy_Str Fuz_Main_line;

//p规则表
unsigned int rule_dir_p[7][7]={
//速度                  -3,-2,-1, 0, 1, 2, 3      // 误差     
			{3, 4, 4, 5, 5, 6, 6,},   //   -3   
			{2, 3, 3, 4, 4, 5, 5,},   //   -2 
			{1, 1, 1, 2, 2, 3, 3,},   //   -1 
			{0, 0, 0, 0, 0, 1, 2,},   //    0 
			{1, 1, 1, 2, 2, 3, 3,},   //    1 
			{2, 3, 3, 4, 4, 5, 5,},   //    2 
			{3, 4, 4, 5, 5, 6, 6}};   //    3 

//d规则表
unsigned int rule_dir_d[7][7]={
//减速积分              -3,-2,-1, 0, 1, 2, 3      // 误差 
			{6, 6, 5, 3, 2, 1, 0,},   //   -3 
			{5, 5, 4, 2, 1, 0, 0,},   //   -2 
			{3, 2, 1, 1, 0, 0, 0,},   //   -1 
			{2, 1, 0, 0, 0, 0, 0,},   //    0 
			{3, 2, 1, 1, 0, 0, 0,},   //    1 
			{5, 5, 4, 2, 1, 0, 0,},   //    2 
			{6, 6, 5, 3, 2, 1, 0}};   //    3 

unsigned int rule_speed_p[7][7]={
//速度变化             -3,-2,-1, 0, 1, 2, 3      // 误差      
			{6, 6, 6, 6, 6, 6, 6,},   //   -3   
			{6, 5, 3, 2, 1, 0, 4,},   //   -2 
			{6, 5, 2, 1, 0, 1, 5,},   //   -1 
			{6, 3, 1, 0, 1, 3, 6,},   //    0 
			{5, 1, 0, 1, 2, 5, 6,},   //    1 
			{4, 0, 1, 2, 3, 5, 6,},   //    2 
			{6, 6, 6, 6, 6, 6, 6}};   //    3 

unsigned int rule_angle_min[7][7]={
//速度变化              -3,-2,-1, 0, 1, 2, 3      // 误差  
			{6, 6, 4, 2, 1, 0, 0,},   //   -3   
			{5, 5, 3, 1, 0, 0, 0,},   //   -2 
			{3, 3, 2, 1, 0, 0, 0,},   //   -1 
			{0, 0, 0, 0, 0, 0, 0,},   //    0 
			{3, 3, 2, 1, 0, 0, 0,},   //    1 
			{5, 5, 3, 1, 0, 0, 0,},   //    2 
			{6, 6, 4, 2, 1, 0, 0}};   //    3 

unsigned int rule_speed_I[7][7]={
//速度                  -3,-2,-1, 0, 1, 2, 3       // 误差     
			{6, 4, 2, 0, 0, 0, 0,},   //   -3   
			{5, 3, 2, 0, 0, 0, 0,},   //   -2 
			{4, 2, 2, 1, 0, 0, 0,},   //   -1 
			{3, 3, 2, 2, 2, 3, 3,},   //    0 
			{4, 2, 2, 1, 0, 0, 0,},   //    1 
			{5, 3, 2, 0, 0, 0, 0,},   //    2 
			{6, 4, 2, 0, 0, 0, 0}};   //    3 

unsigned int rule_speed_I_pool_max[7][7]={
//速度                  -3,-2,-1, 0, 1, 2, 3       // 误差     
			{6, 6, 6, 6, 6, 5, 5,},   //   -3   
			{5, 5, 5, 5, 5, 4, 4,},   //   -2 
			{2, 2, 2, 2, 2, 2, 2,},   //   -1 
			{1, 1, 1, 1, 1, 1, 1,},   //    0 
			{2, 2, 2, 2, 2, 2, 2,},   //    1 
			{5, 5, 5, 5, 5, 4, 4,},   //    2 
			{6, 6, 6, 6, 6, 5, 5}};   //    3 

unsigned int rule_main_line[7][7]={
//误差                  -3,-2,-1, 0, 1, 2, 3      // 速度     
			{0, 0, 0, 0, 0, 0, 0,},   //   -3   
			{1, 1, 1, 1, 1, 1, 1,},   //   -2 
			{2, 2, 2, 2, 2, 2, 2,},   //   -1 
			{3, 3, 3, 3, 3, 3, 3,},   //    0 
			{4, 4, 4, 4, 4, 4, 4,},   //    1 
			{5, 5, 5, 5, 5, 5, 5,},   //    2 
			{6, 6, 6, 6, 6, 6, 6}};   //    3 

void System_Fuzzy_init(void)
{
  Fuz_Dir_P.rule = &rule_dir_p[0];
  Fuz_Dir_P.N1 = &Pn;
  Fuz_Dir_P.N2 = &Sn;
  Fuz_Dir_P.F1 = PF;
  Fuz_Dir_P.F2 = SF;
  Fuz_Dir_P.out_range = UFF_dP;
  
  Fuz_Dir_D.rule = &rule_dir_d[0];
  Fuz_Dir_D.N1 = &Pn;
  Fuz_Dir_D.N2 = &Sn;
  Fuz_Dir_D.F1 = PF;
  Fuz_Dir_D.F2 = SF;
  Fuz_Dir_D.out_range = UFF_dD;
  
  Fuz_Dir_DD.rule = &rule_dir_d[0];
  Fuz_Dir_DD.N1 = &Pn;
  Fuz_Dir_DD.N2 = &SDn;
  Fuz_Dir_DD.F1 = PF;
  Fuz_Dir_DD.F2 = SDF;
  Fuz_Dir_DD.out_range = UFF_dDD;
  
  Fuz_Angle_min.rule = &rule_angle_min[0];
  Fuz_Angle_min.N1 = &Pn;
  Fuz_Angle_min.N2 = &SSn;
  Fuz_Angle_min.F1 = PF;
  Fuz_Angle_min.F2 = SSF;
  Fuz_Angle_min.out_range = UFF_Angel;
  
  Fuz_Speed_I.rule = &rule_speed_I[0];
  Fuz_Speed_I.N1 = &Pn;
  Fuz_Speed_I.N2 = &Sn;
  Fuz_Speed_I.F1 = PF;
  Fuz_Speed_I.F2 = SF;
  Fuz_Speed_I.out_range = UFF_sI;
  
  Fuz_Speed_I_limit.rule = &rule_speed_I_pool_max[0];
  Fuz_Speed_I_limit.N1 = &Pn;
  Fuz_Speed_I_limit.N2 = &Sn;
  Fuz_Speed_I_limit.F1 = PF;
  Fuz_Speed_I_limit.F2 = SF;
  Fuz_Speed_I_limit.out_range = UFF_sI_limit;
  Fuz_Speed_I_limit.filter_flag = 1;
  Fuz_Speed_I_limit.Fil.m_filter = 0.1f;
  
  Fuz_Main_line.rule = &rule_main_line[0];
  Fuz_Main_line.N1 = &Sn;
  Fuz_Main_line.N2 = &Pn;
  Fuz_Main_line.F1 = SF;
  Fuz_Main_line.F2 = PF;
  Fuz_Main_line.out_range = UFF_main_line;
}

void lishudu(float e,float ec,float s,float sd,float sdi)
{
   /*根据E Ec的指定语言值获得有效隶属度*/ 
   //寻找e的隶属度 
    subordinating(e,  PFF ,PF ,&Pn );
    subordinating(ec, DFF ,DF ,&Dn );
    subordinating(s,  SFF ,SF ,&Sn );
    subordinating(sd ,SSFF,SSF,&SSn);
    subordinating(sdi,SDFF,SDF,&SDn);
}



float Fuzzy(Fuzzy_Str *Fuz)
{
  float output;
  int Un[4];//对应附近P值 
  float Un_out[4]; 
  unsigned int UF[4]; //隶属度
  float temp1,temp2; 
  
  /*使用误差范围优化后的规则表rule[7][7]*/ 
  /*一般都是四个规则有效*/   
  Un[0]=Fuz->rule[*(Fuz->N1)+2][*(Fuz->N2)+2]; 
  Un[1]=Fuz->rule[*(Fuz->N1)+3][*(Fuz->N2)+2]; 
  Un[2]=Fuz->rule[*(Fuz->N1)+2][*(Fuz->N2)+3];   
  Un[3]=Fuz->rule[*(Fuz->N1)+3][*(Fuz->N2)+3]; 
  
  if(Fuz->F1[0]<=Fuz->F2[0])
	UF[0]=Fuz->F1[0];  
  else 
	UF[0]=Fuz->F2[0]; 
	  
  if(Fuz->F1[1]<=Fuz->F2[0])
	  UF[1]=Fuz->F1[1];
  else 
	  UF[1]=Fuz->F2[0];
	   
  if(Fuz->F1[0]<=Fuz->F2[1])
	  UF[2]=Fuz->F1[0]; 
  else 
	  UF[2]=Fuz->F2[1]; 
	  
  if(Fuz->F1[1]<=Fuz->F2[1])
	  UF[3]=Fuz->F1[1]; 
  else 
	  UF[3]=Fuz->F2[1]; 
	  
	  
  /*同隶属函数输出语言值求大*/ 
  if(Un[0]==Un[1])
  {
	  if(UF[0]>UF[1])
		  UF[1]=0;
	  else 
		  UF[0]=0;
  } 
  
  if(Un[0]==Un[2])
  {
	  if(UF[0]>UF[2])
		UF[2]=0;
	  else 
		UF[0]=0;
  } 
  
  if(Un[0]==Un[3])
  {
	  if(UF[0]>UF[3])
		UF[3]=0;
	  else
		UF[0]=0;
  } 
  
  if(Un[1]==Un[2])
  {
	  if(UF[1]>UF[2])
	  	UF[2]=0;
	  else
	    UF[1]=0;
  } 
  
  if(Un[1]==Un[3])
  {
	  if(UF[1]>UF[3])
		UF[3]=0;
	  else 
		UF[1]=0;
  } 
  
  if(Un[2]==Un[3])
  {
	  if(UF[2]>UF[3])
	    UF[3]=0;
	  else 
		UF[2]=0;
  } 
  
  /*重心法反模糊*/ 
  /*Un[]原值为输出隶属函数标号，转换为隶属函数值*/ 

          Un_out[0]=Fuz->out_range[Un[0]];
          Un_out[1]=Fuz->out_range[Un[1]];
          Un_out[2]=Fuz->out_range[Un[2]];
          Un_out[3]=Fuz->out_range[Un[3]];
  
  temp1=UF[0]*Un_out[0]+UF[1]*Un_out[1]+UF[2]*Un_out[2]+UF[3]*Un_out[3]; 
  temp2=UF[0]+UF[1]+UF[2]+UF[3]; 
  output=(float)temp1/temp2; 

  if(Fuz->filter_flag)
    output = filter_1st(output,&(Fuz->Fil));
  
  return output; 
}



void subordinating(float input,float *input_range,int *output,int *Nn)
{
   if(input>-input_range[3] && input<input_range[3])//input的变化在幅值内 
   {
   		if(input<=-input_range[2])     
        {
			*Nn=-2;
			output[0]=(int)(FMAX*((float)(-input_range[2]-input)/(input_range[3]-input_range[2])));
      	} 
    	else if(input<=-input_range[1])   
     	{
		    *Nn=-1;
		 	output[0]=(int)(FMAX*((float)(-input_range[1]-input)/(input_range[2]-input_range[1])));
   	 	} 
   		else if(input<=input_range[0])   
		{
	        *Nn=0;
	    	output[0]=(int)(FMAX*((float)(-input_range[0]-input)/(input_range[1]-input_range[0])));
	 	} 
	 	else if(input<=input_range[1])   
    	{
            *Nn=1; 
            output[0]=(int)(FMAX*((float)(input_range[1]-input)/(input_range[1]-input_range[0])));
        } 
        else if(input<=input_range[2]) 
    	{
            *Nn=2; 
	    	output[0]=(int)(FMAX*((float)(input_range[2]-input)/(input_range[2]-input_range[1])));
    	} 
    	else if(input<=input_range[3])   
     	{
         	*Nn=3; 
	    	output[0]=(int)(FMAX*((float)(input_range[3]-input)/(input_range[3]-input_range[2])));
     	}   
   }
   else if(input<=-input_range[3])  //限幅 
   {
	    *Nn=-2;   
		output[0]=FMAX;
   } 
   else if(input>=input_range[3])   
   {
		*Nn=3;   
		output[0]=0;
   } 
   output[1]=FMAX-output[0];  
	
}
