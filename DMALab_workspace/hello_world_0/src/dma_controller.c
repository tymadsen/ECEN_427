/*****************************************************************************
* Filename:          C:\Madsen\SpaceInvaderTestHW2/drivers/dma_controller_v1_00_a/src/dma_controller.c
* Version:           1.00.a
* Description:       dma_controller Driver Source File
* Date:              Mon Dec 07 16:26:13 2015 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "dma_controller.h"

/************************** Function Definitions ***************************/


/**
 *
 * User logic master module to send/receive word to/from remote system memory.
 * While sending, one word is read from user logic local FIFO and written to remote system memory.
 * While receiving, one word is read from remote system memory and written to user logic local FIFO.
 *
 * @param   BaseAddress is the base address of the DMA_CONTROLLER device.
 * @param   Src/DstAddress is the destination system memory address from/to which the data will be fetched/stored.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void DMA_CONTROLLER_MasterSendWord(Xuint32 BaseAddress, Xuint32 DstAddress)
{
  /*
   * Set user logic master control register for write transfer.
   */
  Xil_Out8(BaseAddress+DMA_CONTROLLER_MST_CNTL_REG_OFFSET, MST_SGWR);

  /*
   * Set user logic master address register to drive IP2Bus_Mst_Addr signal.
   */
  Xil_Out32(BaseAddress+DMA_CONTROLLER_MST_ADDR_REG_OFFSET, DstAddress);

  /*
   * Set user logic master byte enable register to drive IP2Bus_Mst_BE signal.
   */
  Xil_Out16(BaseAddress+DMA_CONTROLLER_MST_BE_REG_OFFSET, 0xFFFF);

  /*
   * Start user logic master write transfer by writing special pattern to its go port.
   */
  Xil_Out8(BaseAddress+DMA_CONTROLLER_MST_GO_PORT_OFFSET, MST_START);
}

void DMA_CONTROLLER_MasterRecvWord(Xuint32 BaseAddress, Xuint32 SrcAddress, Xuint32 dest)
{

  Xil_Out32(BaseAddress+DMA_CONTROLLER_SLV_REG0_OFFSET, SrcAddress);

  Xil_Out32(BaseAddress+DMA_CONTROLLER_SLV_REG1_OFFSET, dest);
//  Xil_Out32(BaseAddress+DMA_CONTROLLER_SLV_REG2_OFFSET, (Xuint32) 4);
  /*
   * Set user logic master control register for read transfer.
   */
  Xil_Out8(BaseAddress+DMA_CONTROLLER_MST_CNTL_REG_OFFSET, MST_SGRD);

  /*
   * Set user logic master address register to drive IP2Bus_Mst_Addr signal.
   */
  Xil_Out32(BaseAddress+DMA_CONTROLLER_MST_ADDR_REG_OFFSET, SrcAddress);

  /*
   * Set user logic master byte enable register to drive IP2Bus_Mst_BE signal.
   */
  Xil_Out16(BaseAddress+DMA_CONTROLLER_MST_BE_REG_OFFSET, 0xFFFF);

  /*
   * Start user logic master read transfer by writing special pattern to its go port.
   */
  Xil_Out8(BaseAddress+DMA_CONTROLLER_MST_GO_PORT_OFFSET, MST_START);
}

/*
 * Initialize the DMA controller
 * Sets the operation type to a burst read
 * Sets source, destination and length to slave registers in the DMA controller
 * Sets the byte lane to all
 */
void DMA_CONTROLLER_TranseferInitialize(Xuint32 BaseAddress, Xuint32 srcAddress, Xuint32 destAddress, Xuint32 length){

	  // Set control register to burst write operation
	  Xil_Out8(BaseAddress+DMA_CONTROLLER_MST_CNTL_REG_OFFSET, MST_BRRD);
	  xil_printf("control reg: %x (should be: %x)\r\n", Xil_In8(BaseAddress+DMA_CONTROLLER_MST_CNTL_REG_OFFSET), MST_BRRD);

	  // Set slv_reg0 to src address
	  Xil_Out32(BaseAddress+DMA_CONTROLLER_SLV_REG0_OFFSET, srcAddress);
	  xil_printf("slave reg 0 (source): %x (should be: %x)\r\n", Xil_In32(BaseAddress+DMA_CONTROLLER_SLV_REG0_OFFSET), srcAddress);
	  // Set slv_reg1 to dest address
	  Xil_Out32(BaseAddress+DMA_CONTROLLER_SLV_REG1_OFFSET, destAddress);
	  xil_printf("slave reg 1 (dest): %x (should be: %x)\r\n", Xil_In32(BaseAddress+DMA_CONTROLLER_SLV_REG1_OFFSET), destAddress);
	  // Set data transfer length
	  Xil_Out32(BaseAddress+DMA_CONTROLLER_SLV_REG2_OFFSET, length);
	  xil_printf("length reg: %x (should be: %x)\r\n", Xil_In32(BaseAddress+DMA_CONTROLLER_SLV_REG2_OFFSET), length);
	  // Set byte lane value
	  Xil_Out16(BaseAddress+DMA_CONTROLLER_MST_BE_REG_OFFSET, 0xFFFF);

}
/*
 * Kicks off the read/write transaction by setting the go value (0x0a) to the master go register
 */
void DMA_CONTROLLER_TransferGoGoGOOOOO(Xuint32 BaseAddress){
	 // Start user logic master write transfer by writting special pattern to its go port.
	  Xil_Out8(BaseAddress+DMA_CONTROLLER_MST_GO_PORT_OFFSET, MST_START);
}

void DMA_CONTROLLER_TransferStopSTOPSTOOOOOOOOP(Xuint32 BaseAddress){
	 // Start user logic master write transfer by writting special pattern to its go port.
	  Xil_Out8(BaseAddress+DMA_CONTROLLER_MST_GO_PORT_OFFSET, MST_START+1);
}

