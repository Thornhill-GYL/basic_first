#include "include.h"

void DMA_PORTx2BUFF_Init(u8 CHn, void *SADDR, void *DADDR,DMA_BYTEn byten,u32 count,DMA_REQUEST_SOURCE Source ,DMA_TYPE type,DMA_DLASTCFG keepon)
{
    u8 BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 16 ) ) ); //���㴫���ֽ���
    ASSERT((CHn>=0)&&(CHn<=31),"DMA CHn Error");
    ASSERT(count<=32767,"DMA count Error");
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
    //����TCD
    DMA_SADDR(CHn) =    (u32)SADDR;
    DMA_DADDR(CHn) =    (u32)DADDR; 
    switch(type)
    {
    case DMA_PERIPHERAL_TO_MEMORY:
      DMA_SOFF(CHn)  =    0x00u;
      DMA_DOFF(CHn)  =    BYTEs;
      break;
    case DMA_MEMORY_TO_MEMORY:
      DMA_SOFF(CHn)  =    BYTEs;
      DMA_DOFF(CHn)  =    BYTEs;
      break; 
    case DMA_MEMORY_TO_PERIPHERAL:
      DMA_SOFF(CHn)  =    BYTEs;                              //Դ��ַƫ��
      DMA_DOFF(CHn)  =    0x00u;                              //Ŀ�ĵ�ַƫ��
      break; 
    default:
      ASSERT(0,"DMA Type Error");
    }

    DMA_ATTR(CHn)  =    (0
                         | DMA_ATTR_SMOD(0x0)
                         | DMA_ATTR_SSIZE(byten)             //Դ���ݿ��
                         | DMA_ATTR_DMOD(0x0)
                         | DMA_ATTR_DSIZE(byten)             //Ŀ�����ݿ��
                        );

    DMA_CITER_ELINKNO(CHn)  = DMA_CITER_ELINKNO_CITER(count); //��ǰ��ѭ������
    DMA_BITER_ELINKNO(CHn)  = DMA_BITER_ELINKYES_BITER(count);//��ʼ��ѭ������

    DMA_CR &= ~DMA_CR_EMLM_MASK;                            // CR[EMLM] = 0
    DMA_NBYTES_MLNO(CHn) =   DMA_NBYTES_MLNO_NBYTES(BYTEs); // ͨ��ÿ�δ����ֽ�������������ΪBYTEs���ֽڡ�0��ʾ4GB 
    DMA_SLAST(CHn)      =   0;                              //��ѭ��������ָ�Դ��ַ
    DMA_DLAST_SGA(CHn)  =   (u32)((keepon==DMA_REVERT)? (-count)  : 0 ); //Ŀ�ĵ�ַ������
    DMA_CSR(CHn)        =   (0
                             | DMA_CSR_DREQ_MASK            //��ѭ��������ֹͣӲ������
                             | DMA_CSR_INTMAJOR_MASK        //��ѭ������������ж�
                            );

    //���� DMA ����Դ 
    DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR, CHn) = (0
            | DMAMUX_CHCFG_ENBL_MASK
            //| DMAMUX_CHCFG_TRIG_MASK          //PIT���ڴ�������ģʽ
            | DMAMUX_CHCFG_SOURCE(Source)       //ͨ����������Դ  
                                             );
}