/**********************************key V2.0 说明 ************************************************************************
Key模块用于获取“STC-B学习板”上三个按键的状态。提供按键模块加载和一个应用函数,一个“按键事件：enumEventKey：
  (1)  Key_Init()：按键模块加载函数；
  (2)  char GetKeyAct(char Key)：获取按键状态。
        函数参数：Key，指定要获取状态的按键。Key取值：
						enumKey1
						enumKey2
						enumKey3    
（当参数取值超出有效范围，函数将返回fail）
		函数返回值：
						enumKeyNull（无按键动作）
						enumKeyPress（按下）
						enumKeyRelease（抬起）
						enumKeyFail（失败） 
	  返回值是经过多次检测按键实时状态和统计检测结果后（软件消抖）的有效事件。
    每个按键查询一次后,事件值变成enumKeyNull。事件值仅查询一次有效。
  (3)  按键事件：enumEventKey
     当三个按键（enumKey1,enumKey2,enumKey3）中任意一个按键有”按下“或”抬起“动作时，将产生一个”按键事件“，响应按键事件的用户处理函数由用户编写,并有sys中提供的SetEventCallBack函数设置.	 

补充说明：如果启用了ADC模块，按键3（Key3）任何操作在本模块不可检测到和有任何信息反应，这时按键3（Key3）任何操作将在ADC模块中检测和反应。使用方法相同，具体见ADC模块说明。	
*/

#ifndef _key_H_
#define _key_H_

extern void Key_Init();
extern unsigned char GetKeyAct(char Key) ;                              //获取按键enumKey1,enumKey2,enumKey3事件
								 	                                                      //返回值：enumKeyNull——无，enumKeyPress——下降沿，enumKeyRelease——上升沿，enumKeyFail——错误
enum KeyName     {enumKey1,enumKey2,enumKey3};                          //按键名
enum KeyActName  {enumKeyNull,enumKeyPress,enumKeyRelease,enumKeyFail}; //按键动作名

#endif