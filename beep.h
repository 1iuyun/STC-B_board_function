/**********************************Beep V2.0 说明 ************************************************************************
Beep用于控制“STC-B学习板”上无源蜂鸣器的发声。Beep模块共提供1个驱动函数、2个应用函数：
  (1)  BeepInit()：蜂鸣器模块驱动函数
  (2)  Set_Beep(unsigned int Beep_freq, unsigned char Beep_time)：控制蜂鸣器发声，函数参数
        Beep_freq：指定发声频率，单位Hz。小于10将无输出  
			  Beep_time：指定发声时长。发声时长＝10*Beep_time	  (mS)  ，最长 655350mS
				函数返回值：enumSetBeepOK：调用成功，enumSetBeepFail：调用失败（或因蜂鸣器正在发音）	 
	(3)  GetBeepStatus(void):	获取Beep当前状态,enmuBeepFree:空闲, enumBeepBusy ,正在发音
	(4)  Beep模块使用了STC内部CCP模块1通道
*/

#ifndef _beep_H_			
#define _beep_H_		

extern void BeepInit();										                             //  蜂鸣器初始化

extern char SetBeep(unsigned int Beep_freq, unsigned int Beep_time);   //  发指定频率声音， 发声时长＝10×Beep_time	  (mS)  ，最长 655350mS
                                                                       //  Beep_freq < 10 Hz, 不发音
                                                                       //  函数返回 enumSetBeepOK：调用成功，   enumSetBeepFail：调用失败（调用时正在发音）

extern unsigned char GetBeepStatus(void);                              //  获取状态,enumBeepFree:自由, enumBeepBusy,正在发声

enum BeepActName {enumBeepFree=0,enumBeepBusy,enumSetBeepOK,enumSetBeepFail}; 

#endif