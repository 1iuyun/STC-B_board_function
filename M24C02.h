/**********************************M24C02 V1.0 说明 ************************************************************************
M24C02模块用于控制“STC-B学习板”上IIC接口的非易失存储器(NVM)M24C02芯片操作。
M24C02提供2K bits（256 Bytes）非易失存储器(NVM)功能(非易失存储器单元地址为：00～0xff）。
M24C0402模块共提供2个应用函数(本模块不需要初始化）								
	
	(1)  unsigned char M24C02_Read(unsigned char NVM_addr):	读取M24C02一个指定地址内容
	      函数参数：  
				      NVM_addr：指定非易失存储单元地址，有效值00～0xff（共256个单元）
	      函数返回值：返回M24C02中对应单元的存储数值（1Byte）
				
	(2)  void M24C02_Write(unsigned char NVM_addr, unsigned char NVM_data)：向M24C02一个指定地址写入新值
	      函数参数：  
				      NVM_addr：指定非易失存储单元地址，有效值：00～0xff（共256个单元）
	            NVM_data：待写入M24C02单元的新值（1Byte）

补充说明：
      M24C02为非易失性存储器，其主要特点是：存储的内容在断电后能继续保存，一般用于保存断电需保留的工作系统参数；
      但读、写M24C02内部每一个字节均需要花费一定时间（每次读写操作大约数十uS，写周期为5～10mS），且有”写“寿命限制（每一单元大约”写“寿命为10万次量级寿命）；
			与DS1302内部NVM区别：容量大（M24C02提供256字节，M24CXX系列最大可提供64K字节），但有“写”寿命限制（一般为数十万次“写”寿命“，且写周期长（5～10mS）
      因此，两次写操作之间需间隔5～10mS以上；
			      仅在需要时使用以上读或写函数读写需要的特定字节内容，应避免对其进行无效、大量、重复操作！
*/

#ifndef _M24C02_H_
#define _M24C02_H_

extern unsigned char M24C02_Read(unsigned char NVM_addr);                   //读NVM（读M24C02中的非易失存储单元内容）
extern void M24C02_Write(unsigned char NVM_addr, unsigned char NVM_data);   //写NVM（写M24C02中的非易失存储单元）

#endif