#include "include.h"
#include "LPTMR.h"

void LPTMR_delay_us(uint16 us)
{
    if(us == 0)
        return;
    OSC0_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                        //ʹ��LPTģ��ʱ��
    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT���Զ����������ֵ
    LPTMR0_CMR = (us * OSC_FREQ_MHZ + 16) / 32;                 //���ñȽ�ֵ������ʱʱ��
    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );
    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );
    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)); //�ȴ��Ƚ�ֵ�����ֵ��ȣ���ʱ�䵽��
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;          //����Ƚϱ�־λ
}

void LPTMR_time_close()
{
    LPTMR0_CSR = 0x00;
}

void LPTMR_time_start(void)
{
  OSC0_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK
  SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;                        //ʹ��LPTģ��ʱ��
  LPTMR0_CSR = 0x00;                                          //�ȹ���LPT���Զ����������ֵ
  LPTMR0_CMR = ~0;                                            //���ñȽ�ֵΪ���ֵ
  //ѡ��ʱ��Դ
  LPTMR0_PSR  =   (0
                   | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                   //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                   | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                  );
  //ʹ�� LPT
  LPTMR0_CSR  =  (0
                  //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                  //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                  //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                  | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                  //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                  //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                 );
}

uint32 LPTMR_time_get_us(void)
{
    uint32 data;
    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK) //�Ѿ����
        data = ~0;                      //����
    else
        data = (LPTMR0_CNR * 32) / OSC_FREQ_MHZ; //����Ϊus
    return data;
}

uint32 LPTMR_time_get_ms(void)
{
    uint32 data;
    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)     //�Ѿ������
        data = ~0;                          //���� 0xffffffff ��ʾ����
    else
        data = (LPTMR0_CNR * 32) / OSC_FREQ_MHZ / 1000;
    return data;
}