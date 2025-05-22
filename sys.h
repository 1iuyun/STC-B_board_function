/******************************* sys Ver3.3 说明 ************************************************
  (1) sys.c构成STC程序基本架构。提供：
	      a, 系统初始化MySTC_Init(),系统调度函数MySTC_OS(),加载用户回调函数SetEventCallBack()
				b, 若干可设置和触发回调函数的事件；
					          enumEventSys1mS       : 1mS定时到
										enumEventSys10mS      : 10mS定时到
										enumEventSys100mS     : 100mS定时到
										enumEventSys1S        : 1S定时到
										enumEventKey          : 按键事件
										enumEventNav          : 导航按键事件“						
										enumEventHall         : hall传感器事件
										enumEventVib          : 振动传感器事件
										enumEventXADC         : 扩展接口上新的AD值事件
										enumEventUart1Rxd     : 串口1上收到一个符合格式定义的事件。串口1：USB上与PC机通信
										enumEventUart2Rxd     : 串口2上收到一个符合格式定义的事件。串口2：485、或EXT上（由初始化确定）
										enumEventIrRxd        : 红外收到一个数据包事件
        c, 获取系统运行性能评价参数
				
	(2) MySTC_Init()：sys初始化函数，必须执行一次。
  (3) MySTC_OS()：  sys调度函数，应在while（1）循环中。	
  (4) SetEventCallBack(char event, void *(user_callback)):加载"事件"用户回调函数.
	(5) SysPerF GetSysPerformance(void)  :获取系统运行性能评估参数
	     函数参数：无
			 函数返回值：结构 struct SysPerF。定义如下：
					typedef struct                   //系统性能评估参数,每秒更新一次
						{ unsigned long MainLoops;            //SysPerformance.MainLoops            每秒主循环次数（应大于1000以上）
							unsigned char PollingMisses;        //SysPerformance.PollingMisses        每秒轮询丢失次数（理想值为0。如果有非0情况，说明用户程序或算法存在问题）
						} SysPerF;	
			
	(6) 补充说明：
	      sys.c 基本调度时间为1mS，非抢占式，要求用户程序片段，其单次循环执行时间累加起来应小于1mS。
*/

#ifndef	_sys_h_
#define _sys_h_

typedef struct                     //系统性能评估参数,每秒更新一次
  { unsigned long MainLoops;            //SysPerformance.MainLoops            每秒主循环次数（应大于1000以上）
		unsigned char PollingMisses;        //SysPerformance.PollingMisses        每秒轮询丢失次数（理想值为0。如果有非0情况，说明用户程序或算法存在问题）
	} struct_SysPerF;
	
extern void MySTC_Init();             //sys初始化函数
extern void MySTC_OS();               //sys调度函数，应在while（1）循环中                                     
extern void SetEventCallBack(char event, void *(user_callback));  //加载"事件"用户回调函数.
extern struct_SysPerF GetSysPerformance(void); 
		
enum event{enumEventSys1mS,                      //系统1mS事件
	         enumEventSys10mS,                     //系统10mS事件
           enumEventSys100mS,                    //系统100mS事件
           enumEventSys1S,                       //系统1S事件
           enumEventKey,                         //按键事件
           enumEventHall,                        //hall传感器事件
           enumEventVib,                         //振动传感器事件
           enumEventNav,                         //导航按键事件“
           enumEventXADC,                        //扩展接口上新的AD值事件
           enumEventUart1Rxd,                    //串口1上收到一个符合格式定义的事件
	         enumEventUart2Rxd,                    //串口2上收到一个符合格式定义的事件
	         enumEventIrRxd                        //红外Ir上收到一个数据包
          };                         
	
#endif