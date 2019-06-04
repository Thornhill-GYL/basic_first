#include "include.h"

//高端霸气的横断路障程序
int Length_S0 = 0;
int block_flag=0;
Block_Data Block = {.K = 6.3682f,
                    .Speed_target_ave = 65,};
void roadblock_process(void)
{
  roadblock_test();//用于检测路障
  navigate_pre();
  
  
}

//用于检测路障
void roadblock_test(void)
{
//   if(sure_block==1&&TOF_Distance<=1000)
//   {
//     Block.State = Block_S1;
//   }
//   else
//     Block.State = NoBlock;
     
}

//用于导航前直道摆正
void navigate_pre(void)
{
  
}


void navigation(void)
{
  if(Block.State < Block_S1)
  {
    return ;
  }
  switch(Block.State)
  {
  case Block_S1:
    Block.R = 0.3;
    break;
  case Block_S2:
    Block.R = -0.3;
    break;
  case Block_S3:
    Block.R = 0.5;
    break;
  default:
    break;
  }

//  Block.Speed_target_L = (int)(Block.Speed_target_ave*(1 - DIR.Width/2/Block.R));
//  Block.Speed_target_R = (int)(Block.Speed_target_ave*(1 + DIR.Width/2/Block.R));
  
}