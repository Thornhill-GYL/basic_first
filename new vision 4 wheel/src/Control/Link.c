//#include "include.h"
//#include "Link.h"
//LINK_TYPE Link;
//int recevie_flag=0;
//void Nrf_connect(void)
//{
//  while(nrf_init()==0)
//  {
//    LED_R = 0;LED_G = 1;LED_B = 1;
//    BEEP_Open_once();
//    delayms(50);
//  }
//  delayms(50);
//  NVIC_EnableIRQ(PORTB_IRQn);
//  delayms(50);
//  EnableInterrupts;
// 
//  while(strcmp((char *)Link.Rx.buf,"Are You OK?") != 0)
//  delayms(10);
//  sprintf((char *)Link.Tx.buf,"I am fine.Do you like miband?");
//  Nrf_Senddata();
//  while(strcmp((char *)Link.Rx.buf,"Yeah,thank you!") != 0);
//  
//   for(int i = 0;i<DATA_PACKET;i++)
//    Link.Tx.buf[i]=Link.Rx.buf[i] = 0;
//   LED_R = 1;LED_G = 0;LED_B = 1;
//   Link.Allready = 1;
//}
//  void Nrf_Receivedata(void)
//{
//  if(!Link.NeedConnect)
//    return;
//  if(nrf_rx(Link.Rx.buf,DATA_PACKET)==0)
//  {
//    led_flag=0;
//    return;
//  }
//  else
//  {
//     recevie_flag=1;
//     led_flag=1;
//  }
//  if(Link.Allready)
//  {
//    meeting_Receive();
//    Nrf_Senddata();
//  }
//
//}
//void Nrf_Senddata(void)
//{
//  if(!Link.NeedConnect)//if(!x)是x等于零的意思
//    return;
//  if(Link.Allready)
//  {
//   
//    meeting_send();
////     for(int i=0;i<DATA_PACKET;i++)
////   {
////     if(i!=31)
////      Link.Tx.buf[i]='c';
////     else
////       Link.Tx.buf[i]='\0';
////   }
//    
//  }
// 
//  //  LED_NO;
//   LED_R = nrf_tx(Link.Tx.buf,DATA_PACKET);
//   
//}
