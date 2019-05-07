#include "include.h"

void DMA_PORTx2BUFF_Init(u8 CHn, void *SADDR, void *DADDR,DMA_BYTEn byten,u32 count,DMA_REQUEST_SOURCE Source ,DMA_TYPE type,DMA_DLASTCFG keepon)
{
    u8 BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 16 ) ) ); //计算传输字节数
    ASSERT((CHn>=0)&&(CHn<=31),"DMA CHn Error");
    ASSERT(count<=32767,"DMA count Error");
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
    //配置TCD
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
      DMA_SOFF(CHn)  =    BYTEs;                              //源地址偏移
      DMA_DOFF(CHn)  =    0x00u;                              //目的地址偏移
      break; 
    default:
      ASSERT(0,"DMA Type Error");
    }

    DMA_ATTR(CHn)  =    (0
                         | DMA_ATTR_SMOD(0x0)
                         | DMA_ATTR_SSIZE(byten)             //源数据宽度
                         | DMA_ATTR_DMOD(0x0)
                         | DMA_ATTR_DSIZE(byten)             //目标数据宽度
                        );

    DMA_CITER_ELINKNO(CHn)  = DMA_CITER_ELINKNO_CITER(count); //当前主循环次数
    DMA_BITER_ELINKNO(CHn)  = DMA_BITER_ELINKYES_BITER(count);//起始主循环次数

    DMA_CR &= ~DMA_CR_EMLM_MASK;                            // CR[EMLM] = 0
    DMA_NBYTES_MLNO(CHn) =   DMA_NBYTES_MLNO_NBYTES(BYTEs); // 通道每次传输字节数，这里设置为BYTEs个字节。0表示4GB 
    DMA_SLAST(CHn)      =   0;                              //主循环结束后恢复源地址
    DMA_DLAST_SGA(CHn)  =   (u32)((keepon==DMA_REVERT)? (-count)  : 0 ); //目的地址调整量
    DMA_CSR(CHn)        =   (0
                             | DMA_CSR_DREQ_MASK            //主循环结束后停止硬件请求
                             | DMA_CSR_INTMAJOR_MASK        //主循环结束后产生中断
                            );

    //配置 DMA 触发源 
    DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR, CHn) = (0
            | DMAMUX_CHCFG_ENBL_MASK
            //| DMAMUX_CHCFG_TRIG_MASK          //PIT周期触发传输模式
            | DMAMUX_CHCFG_SOURCE(Source)       //通道触发传输源  
                                             );
}