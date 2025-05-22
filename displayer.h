/**********************************display V2.0 说明 ************************************************************************
displayer用于控制“STC-B学习板”上8个7段数码管（Seg7）和8个指示灯（Led）工作.提供显示模块加载和三个应用函数：

  (1) displayerInit()：显示模块加载函数；
	
  (2) SetDisplayerArea(char Begin_of_scan,char Ending_of_Scan)：设置LED启用区域。8个数码管从左至右编号分别为0——7，函数参数
      Begin_of_scan设定启用数码管起始编号，Ending_of_Scan为结束编号。设定范围内的数码管才工作和显示。
			注：正常情况下，两个参数取值范围为0——7，且Ending_of_Scan>Begin_of_scan。但利用动态扫描和人眼视觉效果，可设置超出
			    该范围的参数，以实现特殊显示效果：如软件调整显示亮度，或非灯亮度显示，等；
					
  (3) Seg7Print(char d0,char d1,char d2,char d3,char d4,char d5,char d6,char d7)：将8个参数值分别译码显示到对应的数码管上。
      显示译码表(code char decode_table[])在main.c中,用户可以修改和增减.
			
  (4) LedPrint(char led_val)：控制8个指示灯开关。参数light_val的8个bit位对应8个指示灯的开关，“1”——指示灯“亮”		
*/

#ifndef _displayer_H_
#define _displayer_H_
extern void DisplayerInit();										                                               //加载显示模块工作
extern void SetDisplayerArea(char Begin_of_scan,char Ending_of_Scan);						               //设置有效显示区域
extern void Seg7Print(char d0,char d1,char d2,char d3,char d4,char d5,char d6,char d7);        //在数码管上输出	
extern void LedPrint(char led_val);
#endif  

