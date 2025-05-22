/********************************** music V2.0 说明 ************************************************************************
Music模块在Beep和Displayer模块基础上再次封装，用于控制“STC-B学习板”上播放音乐。加载该模块，将同时加载Beep模块、displayer模块。music模块共提供1个music驱动函数、4个应用函数：
 （1）MusicPlayerInit()：驱动music模块；
 （2）char PlayTone(unsigned char tone, unsigned char beatsPM ,unsigned char scale, unsigned char beats)：播放音乐音阶，实现用指定音调、指定节拍率、发指定音阶、发音节拍数。
函数参数：
      tone：指定音调，有效值：0xFA、0xFB、0xFC、0xFD、0xFE、0xFF、0xF9分别对应音乐A、B、C、D、E、F、G调
			beatsPM：节拍率，即每分钟节拍数，值范围10~255拍/分钟
			scale：音乐简谱音高，1字节。
0x00——休止符
高4位：1——低8度音 2——中8度音  3——高8度音
低3位：1-7对应简谱音。其它值无效。
如：0x13表示低音3（mi）
	  beats：音长(节拍数),单位1/16拍。
如：16（0x10）对应1拍，32（0x20）对应2拍，8（0x08）对应半拍...... 。
函数返回值：enumBeepOK：调用成功
				enumBeepBusy：忙（上一音未按设定发完，或因蜂鸣器正在发音）	
				enumBeepFail：调用失败（音调参数tone不对，或音高编码scale不对） 
							（见Beep.h中定义BeepActName）
 （3）SetMusic(unsigned char beatsPM, unsigned char tone, unsigned char *pt, unsigned int datasize, unsigned char display)：设定或改变要播放音乐和播放参数。
函数参数：
      beatsPM：节拍率，即每分钟节拍数，值范围10~255拍/分钟，如果参数值为enumModeInvalid将不改变原beatsPM；
			tone：指定音调， 有效值：0xFA、0xFB、0xFC、0xFD、0xFE、0xFF、0xF9分别对应音乐A、B、C、D、E、F、G调，参数值为enumModeInvalid或其它值将不改变原tone
			*pt ：指向要播放的音乐编码的首地址            。*pt 和 datasize 只有有一个参数 = enumModeInvalid，将不改变 *pt 和 datasize
			datasize：要播放的音乐编码的长度（字节数）    。*pt 和 datasize 只有有一个参数 = enumModeInvalid，将不改变 *pt 和 datasize
			display：Seg7和Led是否用来显示播放音乐信息，取值： 
enumMscNull          —— 不用
			  enumMscDrvSeg7      —— 用7段数码管Seg7（显示信息）
			  enumMscDrvLed       —— 用Led指示灯（打拍）							  enumMscDrvSeg7andLed —— 用Seg7 和 Led
			 参数值为enumModeInvalid或其它值将不改变原display
音乐编码规则：
1，常规音乐简谱发音编码（成对出现，不可分开，中间不能插入其它编码和控制字）
	   基本格式：音高（1字节），节拍数（1字节），音高，节拍，......
         其中“音高”部分：
0x11 — 0x17 ：对应低音 do、re、mi、fa、so、la、si、						    0x21 — 0x27 ：对应中音 do、re、mi、fa、so、la、si
          0x31 — 0x37 ：对应高音 do、re、mi、fa、so、la、si
		其中“节拍数”部分：
0x01-0xFF：单位1/16拍。也即十六进制中，高4位表示整拍数，低4位表示分拍数（1/16）
	    如：发音2拍：  0x20
		    发音半拍：  0x08
			    发音1拍半：0x18
	2，音乐编码中可以插入以下控制字，用于设定音乐播放参数等（前6个也可以通过函数，用程序设定和实现）:
       enumMscNull              ：  不用
       enumMscDrvSeg7          ：  用7段数码管Seg7（显示信息）
			 enumMscDrvLed           ：  用Led指示灯（打拍）
			 enumMscDrvSeg7andLed    ：  用Seg7 和 Led
			 enumMscSetBeatsPM       ：  设置节拍率， 后面再跟 节拍率（1字节）
			 enumMscSetTone          ：   设置音调， 后面再跟 音调（1字节）
0xFA 或 0xFB 或 0xFC 或 0xFD 或 0xFE 或 0xFF 或 0xF9
分别对应音乐：A调 或B调………
	   enumMscRepeatBegin       ：  设置音乐播放重复开始处。重复一次（暂不支持多次），暂不能嵌套（嵌套无效或可能导致不可预期结果）
     enumMscRepeatEnd         ：  设置音乐播放重复结束处
																		
 （4）SetPlayerMode(unsigned char play_ctrl)	：音乐播放控制函数。
    函数参数：
          play_ctrl：.   ： ”播放“
					enumModePause  ： ”暂停“
					enumModeStop    ：”停止/结束“
         （其它参数无效）
所有操作在当前”音“播放完成后生效；				                                  
 （5）char GetPlayerMode(void)： 获取当前播放状态
		函数返回值：（play_ctrl值）
				enumModePlay      ：  播放状态
				enumModePause     ：  暂停状态
				enumModeStop      ：  停止/结束
（其它功能应用型函数可根据需要设置和增加） 
*/

#ifndef _music_H_			
#define _music_H_		
							       
extern void MusicPlayerInit();
extern char PlayTone(unsigned char tone, unsigned char beatsPM ,unsigned char scale, unsigned char beats);
																//  tone： 音调。F9,FA,FB,FC,FD,FE,FF分别对应G\A\B\C\D\E\F调,其它值无效
																//  beatsPM：节凑，单位：每分钟节拍数		
																//  scale: 音高。0x00——休止符，高4位：1——低8度音 2——中8度音  3——高8度音，低3位：1-7简谱音。其它无效
																//  beats: 音长(节拍数),单位1/16拍。16（0x10）对应1拍，32（0x20）对应2拍，8（0x08）对应半拍......  
extern void SetMusic(unsigned char beatsPM, unsigned char tone, unsigned char *pt, unsigned int datasize, unsigned char display);
                                //可设置或改变：全部、或部分播放参数（节拍率、音调、显示方式、音乐原数据）
																//其中节拍率、音调、显示方式 也可以在音乐编码中设置
extern void SetPlayerMode(unsigned char play_ctrl);   
                                //操作播放：enumModePlay 或 enumModePause 或 enumModeStop，分别设置音乐”播放“、”暂停“、”停止/结束“，所有操作在当前”音“播放完成后生效；
extern char GetPlayerMode(void);   //获取当前播放状态

enum PlayerMode   {enumModeInvalid=0,             //播放模式非法
                   enumModePlay,                  //播放          
                   enumModePause,                 //暂停（可恢复续放）
                   enumModeStop};                 //停止（结束）
enum MusicKeyword {enumMscNull=0xF0,              //播放音乐时，不用7段数码管、LED指示灯（显示音乐播放相关信息）
                   enumMscDrvSeg7,                //播放音乐时，  用7段数码管
                   enumMscDrvLed,                 //播放音乐时，  用           LED指示灯
                   enumMscDrvSeg7andLed,          //播放音乐时，  用7段数码管、LED指示灯
                   enumMscSetBeatsPM,             //音乐编码中关键字： 设置 音乐节拍
                   enumMscSetTone,                //音乐编码中关键字： 设置   音调
                   enumMscRepeatBegin,            //音乐编码中关键字： 设置 重复开始 
                   enumMscRepeatEnd};             //音乐编码中关键字： 设置 重复结束

#endif