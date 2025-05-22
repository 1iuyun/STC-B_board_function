#include"STC15F2K60S2.H" //头文件
#include"intrins.H" //头文件
#define uchar unsigned char 	   //宏定义
#define uint unsigned int
#define ADC_CHS1_7 0X07
/***********时分秒写寄存器**************/
#define DS1302_SECOND_WRITE 0x80			
#define DS1302_MINUTE_WRITE 0x82
#define DS1302_HOUR_WRITE   0x84 
#define DS1302_WEEK_WRITE   0x8A
#define DS1302_DAY_WRITE    0x86
#define DS1302_MONTH_WRITE  0x88
#define DS1302_YEAR_WRITE   0x8C
#define ADC_POWER 0X80
#define ADC_FLAG 0X10  /*当A/D转换完成后，ADC_FLAG要软件清零*/
#define ADC_START 0X08
#define ADC_SPEED_90 0X60
/***********时分秒读寄存器**************/
#define DS1302_SECOND_READ  0x81
#define DS1302_MINUTE_READ  0x83
#define DS1302_HOUR_READ    0x85 
#define DS1302_WEEK_READ    0x8B
#define DS1302_DAY_READ     0x87
#define DS1302_MONTH_READ   0x89
#define DS1302_YEAR_READ    0x8D
#define	P1_7_ADC 0x80
sbit SEL0=P2^0;			//定义引脚
sbit SEL1=P2^1;
sbit SEL2=P2^2;
sbit SEL3=P2^3;
sbit beep=P3^4;		//蜂鸣器引脚
sbit Key1=P3^2;	   //Key1   切换标志位 

#define uint unsigned int
#define ulint unsigned long
#define uchar unsigned char	
#define NMAX_KEY 100  //100次读取按键值 

uint time=0;				//延时

uint l=0;						//执行光的次数
ulint suml=0;				//光AD值得总和

uint light=0;				//光
uint light_down=20; //警报下限
uint light_up=100;	//警报上限
uchar flag=0;				//切换模式，0代表下限，1代表上限

//设置用于显示光的三个变量
uint light_bai=0;		//百位
uint light_shi=0;		//十位
uint light_ge =0;		//个位
//警报值
uint down_bai=0;		//百位
uint down_shi=0;		//十位
uint down_ge =0;		//个位
//警报值
uint up_bai=0;		//百位
uint up_shi=0;		//十位
uint up_ge =0;		//个位
/**********************

变量定义
***********************/
uchar G_count;
uchar display;
uint Key1_count;
uint Key2_count;
uint Key3_count;
uint Key_count;

bit Key1_C;				//key1当前的状态
bit Key1_P;				//key1前一个状态

//数码管上显示0-F
char segtable[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
								 0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void Delay(int n);
void weixuan(char i);
void SEG_Display();
void check();
void InitADC_light();
void date_processlight();
void date_processlight_down();
void date_processlight_up();

void Delay(int n)			//延时函数
{
	int x;
	while(n--)
	{
		x=60;
		while(x--);
	}
}

void weixuan(char i)	//数码管位的选择
{
	SEL2=i/4;
	SEL1=i%4/2;
	SEL0=i%2;
}
void SEG_Display()
{
	//用于设置光的位数
	P0=0;
	weixuan(5);
	P0=segtable[light_bai];
	Delay(10);
	
	P0=0;
	weixuan(6);
	P0=segtable[light_shi];
	Delay(10);
	
	P0=0;
	weixuan(7);
	P0=segtable[light_ge];
	Delay(10);
	
	if(flag==0)    //flag为0 显示下限值 020
	{
		P0=0;
		weixuan(0);
		P0=segtable[down_bai];
		Delay(10);
	
		P0=0;
		weixuan(1);
		P0=segtable[down_shi];
		Delay(10);
	
		P0=0;
		weixuan(2);
		P0=segtable[down_ge];
		Delay(10);
	} 
	if(flag==1)   //flag为1 显示上限值 100
	{
		P0=0;
		weixuan(0);
		P0=segtable[up_bai];
		Delay(10);
	
		P0=0;
		weixuan(1);
		P0=segtable[up_shi];
		Delay(10);
	
		P0=0;
		weixuan(2);
		P0=segtable[up_ge];
		Delay(10);
	}
}

void main()
{
	P1M1=0x00;            //将P1^7设置为推挽模式，其余为准双向口模式
	P1M0=0x80;         
	P0M1=0x00;						//设置P0为推挽模式，点亮数码管
	P0M0=0xff;
	P2M1=0x00;
	P2M0=0x08;						//将P2^3设置为推挽模式，其余为准双向口模式
	P3M1=0x00;
	P3M0=0x10;					  //设置P3^4为推挽模式
	SEL3=0;								//熄灭LED灯
	
	/*初始化所有按键的当前状态、前一个状态*/
	Key1_C=1;							//key1当前的状态
	Key1_P=1;							//key1前一个状态
	Key1_count=0x80+NMAX_KEY/3*2;
	Key_count=NMAX_KEY;
	
	IE=0xa8;							//EA=1打开总中断，EADC=1允许A/D转化中断，ET1=1允许T1中断	
	TMOD=0x10;						//使用定时器1，16位不可重装载模式，TH1、TL1全用
	TH1=(65535-100)/256;	//高8位赋初值，定时400周期
	TL1=(65535-100)%256;	//低8位赋初值
	TR1=1;								//启动定时器1
	
	InitADC_light();
	while(1)
	{
		SEG_Display();
	}
}

void InitADC_light()		//初始化光ADC
{
	P1ASF=0xff;           //将P1口作为模拟功能A/D使用
	ADC_RES=0;            //转换结果清零
	ADC_RESL=0;
	ADC_CONTR=0x8c;				//ADC_POWER=1打开A/D转换器电源;ADC_START=1启动模拟转换器ADC;CHS=100选择P1^4作为A/D输入使用
	CLK_DIV=0x20;
}

//分别取出温度和光照的百位、十位、个位
void date_processlight_down()
{
	down_bai=light_down%1000/100;
	down_shi=light_down%100/10;
	down_ge =light_down%10;
}
void date_processlight_up()
{
	up_bai=light_up%1000/100;
	up_shi=light_up%100/10;
	up_ge =light_up%10;
}
void date_processlight()
{
	light_bai=light%1000/100;
	light_shi=light%100/10;
	light_ge =light%10;
}

void check()
{
			if(Key1==0)
			Key1_count--;
	
			Key_count--;					//总的次数减1

			if(Key_count==0)			//100次完了
			{
	
				if(Key1_count<0x80)
				{
					Key1_C=0;
					if(Key1_P==1)			//下降沿（按键做动作）
					{
						Key1_P=0;
						flag=!flag;
					}
				}
				if(Key1_count>=0x80)
				{
					Key1_C=1;
					if(Key1_P==0)
					   Key1_P=1;			//上升沿（假设不做动作那就继续）
				}
	
				//新一轮的判断
				Key1_count=0x80+NMAX_KEY/3*2;
				Key_count=NMAX_KEY;
			}
}

void Timer1_Routine() interrupt 3	//3为定时器1中断编号
{
	IE=0x00;												//关闭总中断
	TR1=0;													//定时器1停止
	TH1=(65535-100)/256;						//重新赋值
	TL1=(65535-100)%256;

	check();
	if(flag==0)
	{
		if(light<light_down)
				beep=~beep;				  		 //产生方波使得蜂鸣器发声
		else beep=0;								 //不使用蜂鸣器时，讲引脚置0——处于低电平
	}
	if(flag==1)
	{
		if(light>light_up)
				beep=~beep;				 		  //产生方波使得蜂鸣器发声
		else beep=0;
	}

	IE=0xa8;											//打开总中断
	TR1=1;												//启动定时器1
}

//AD中断
void adc_isr() interrupt 5 using 1
{
  time++;
	IE=0x00;											//关闭中断
	
	if(time>2000)                 //取多次值求平均值减小误差
	{

			light=(suml+l/2)/l;				//四舍五入
			suml=0;
			l=0;
			time=0;
	   	date_processlight();
			date_processlight_down();
			date_processlight_up();
	}

		//处理光部分的数据
		l++;
		suml+=ADC_RES*256+ADC_RESL;//求l次AD值的和

	ADC_CONTR&=~0X10;  					 //转换完成后，ADC_FLAG清零
	ADC_CONTR|=0X08;	 					 //转换完成后，ADC_START赋1
	IE=0xa8;					 					 //打开中断
}


sbit SEL0=P2^0;
sbit SEL1=P2^1;
sbit SEL2=P2^2;
sbit SEL3=P2^3;
sbit Echo=P1^0;						//发送
sbit Trig=P1^1;						//接收
sbit beep=P3^4;						//蜂鸣器引脚

/**************************
定义变量如下：
**************************/
#define uint unsigned int
float TIME;								//一个周期的时间
float receive;
uint receiveint; 					//距离
char qianwei=0;
char baiwei=0;
char shiwei=0;
char gewei=0;
uint flag=0;
char global=0;
char segtable[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x71};

void SEG_Display();
void date_process();
void Delay(int n);

void main_test()
{
	TIME=12/11.0592;						//寄存器AUXR中T1x12=0,12分频

	P0M1=0x00;									//推挽
    P0M0=0xff;
	P2M1=0x00;
	P2M0=0x08;	
	SEL3=0;

	TMOD=0x11;									//同时使用定时器T0和T1

    EA=1;                                       //打开总的中断
	ET1=1;                                      //开启定时器1中断	
 	TH1=(65535-60000)/256;			            //60ms
	TL1=(65535-60000)%256;		
	TR1=1;                                      //启动定时器

	T2L=(65535-1)/256;				             //定时器2，16位自动重装
	T2H=(65535-1)%256;				             //赋初值
	AUXR |= 0x10;								//允许定时器2运行
	IE2  |= 0x04;								//允许定时器2产生中断
	
	while(1)
	{
	 SEG_Display();
	}
}

//定义定时器1中断，用于每隔至少60ms发一次信号
void Timer1_Routine() interrupt 3
{
    global++;
	if(global>10)
	{
		ET1=0;										 //关闭T1中断
		TR1=0;										 //关闭定时器1
		TH1=(65535-60000)/256;
		TL1=(65535-60000)%256;		                //重新赋值
		

		Trig=1;				
		Delay(2);									//生成20us的脉冲宽度的触发信号
		Trig=0;

		while(!Echo);							    //等待回响信号变高电平
		TR0=1;										//启动定时器0
		
		while(Echo);							     //等待回响信号变低电平
		TR0=0;										//关闭定时器0
		
		receive=(340*TIME*((float)TH0*256+(float)TL0))/2000;
		receiveint=(int)receive;
		if(receiveint<30||receiveint>4000)
		{	
			receiveint=0;
		}
		date_process();
	
		TH0=0;										//定时器0赋值0
		TL0=0;

		ET1=1;										//打开定时器T1
		TR1=1;
		global=0;
	    }
}

void Timer2_Routine() interrupt 12
{
	if(receiveint>800)
		beep=0;
	else if(receiveint>500)
	{
		flag++;
		if(flag>8000)
		{
			beep=~beep;
			if(flag==8500)
				flag=0;
		}
	}
	else if(receiveint>300)
	{
		flag++;
		if(flag>5500)
			flag=0;
		if(flag>5000)
		{
				beep=~beep;
			if(flag==5500)
				flag=0;
		}
	}
	else if(receiveint>200)
	{
		flag++;
		if(flag>3500)
			flag=0;
		if(flag>3000)
		{
				beep=~beep;
			if(flag==3500)
				flag=0;
		}
	}
	else if(receiveint>100)
	{
		flag++;
		if(flag>1500)
			flag=0;
		if(flag>1000)
		{
				beep=~beep;
			if(flag==1500)
				flag=0;
		}
	}
	else if(receiveint>70)
	{
		flag++;
		if(flag>1000)
			flag=0;
		if(flag>500)
		{
				beep=~beep;
			if(flag==1000)
				flag=0;
		}
	}
	else if(receiveint>40)
	{
		flag++;
		if(flag>600)
			flag=0;
		if(flag>100)
		{
				beep=~beep;
			if(flag==600)
				flag=0;
		}
	}
	else if(receiveint<40)
	{
				beep=~beep;
	}
}

void date_process()
{
	qianwei=receiveint/1000;
	baiwei=receiveint%1000/100;
	shiwei=receiveint%100/10;
	gewei=receiveint%10;	
}
void SEG_Display()									 //动态显示数码管
{
	P2=0x04;
	P0=segtable[qianwei];
	Delay(3);
	P2=0x05;
	P0=segtable[baiwei];
	Delay(3);
	P2=0x06;
	P0=segtable[shiwei];
	Delay(3);
	P2=0x07;
	P0=segtable[gewei];
	Delay(3);	
}

void Delay(int n)
{
	int x;
	while(n--)
	{
		x=50;
		while(x--);
	}
}



/*时、分、秒标志*/
bit set_H_flag;
bit set_Ms_flag;
bit set_S_flag;

/*时、分、秒值*/
uint set_H_val;
uint set_Ms_val;
uint set_S_val;

bit set_HMS_done;  //时分秒设置完
bit show_set_HMS;  //显示时分秒
bit show_HMS;  //显示时分秒
bit show_key_val;
unsigned char key_val;

/**********************
引脚别名定义
***********************/
/********DS1302*******/
sbit Rtc_sclk = P1^5;			//时钟线引脚,控制数据的输入与输出
sbit Rtc_rst  = P1^6;			//CE线引脚,读、写数据时必须置为高电平
sbit Rtc_io   = P5^4;			//实时时钟的数据线引脚
/********数码管显示******/
sbit Led_sel = P2^3;		   //流水灯和数码管选通引脚
sbit Sel0    = P2^0;		   //Sel0、Sel1、Sel2三位二进制进行数码管位选0-7  	
sbit Sel1    = P2^1;
sbit Sel2    = P2^2;
sbit KEY1=P3^2;	 			  //Key1
/**********************
变量定义
***********************/
uchar duanxuan[]={ 					  //数码管显示所要用的段码
 
                0x3F,  //"0"
                0x06,  //"1"
                0x5B,  //"2"
                0x4F,  //"3"
                0x66,  //"4"
                0x6D,  //"5"
                0x7D,  //"6"
                0x07,  //"7"
                0x7F,  //"8"
                0x6F,  //"9"
                0x77,  //"A"
                0x7C,  //"B"
                0x39,  //"C"
                0x5E,  //"D"
                0x79,  //"E"
                0x71,  //"F"
                0x76,  //"H"
                0x38,  //"L"
                0x37,  //"n"
                0x3E,  //"u"
                0x73,  //"P"
                0x5C,  //"o"
                0x40,  //"-"
                0x00,  //熄灭
                0x00  //自定义
 
                };

uchar weixuan[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};	   //数码管位选数组
uchar flag;					 //所选择点亮的数码管0-7标志位
uchar temp;					 //要写入到DS1302的数据
/*******************************
 * 函数名：Delayms
 * 描述  ：毫秒延时程序
 * 输入  ：延时i毫秒
 * 输出  ：无
 *******************************/
void Delayms(char i) 			
{
	while(i--)
	{	
		int n=500;
		while (n--)
	    {
	        _nop_();
	        _nop_();
	        _nop_();
	        _nop_();
	        _nop_();
			_nop_();
	    }
	}
}
/**********************
定义时间结构体
***********************/
typedef struct __SYSTEMTIME__
{
	uchar Second;
	uchar Minute;
	uchar Hour;
	uchar Week;
	uchar Day;
	uchar Month;
	uchar Year;
}SYSTEMTIME; 			//定义的时间类型
SYSTEMTIME t;			
/**********************
函数名称：Ds1302_write
功能描述：Ds1302写入一字节
入口参数：uchar 要写入的数据 
***********************/
void Ds1302_write(uchar temp) 		 //temp:要写入的数据
{
	uchar i;
	for(i=0;i<8;i++)				 //循环8次写入一个字节
	{
		Rtc_sclk=0;					//时钟脉冲拉低
		Rtc_io=(bit)(temp&0x01);	 //取最低位数据
		temp>>=1;					 //右移一位
		Rtc_sclk=1;					  //上升沿输入 	
	}
}

/**********************
函数名称：Ds1302_read
功能描述：Ds1302读取一字节
入口参数：无
出口参数：返回uchar 读出的数据
***********************/
uchar Ds1302_read()	 
{
	 uchar i, dat;
	 for(i=0;i<8;i++)			
	 {
	 	Rtc_sclk=0;				//时钟脉冲拉低	
	 	dat>>=1;				//要返回的数据右移一位	 
		if(Rtc_io==1)			//数据线为高，证明该位数据为1
		dat|=0x80;				//要传输数据的当前值置为1，若不是，则为0
		Rtc_sclk=1;				//拉高时钟线 
	 }
	 return dat;				//返回读取出的数据
}


/**********************
函数名称：WriteDS1302
功能描述：向Addr对应的寄存器中写入数据Data
入口参数：uchar Addr 寄存器地址, uchar Data 要写入寄存器的数据
***********************/
void WriteDS1302(uchar Addr, uchar Data) 	   //Addr: DS1302地址,Data: 要写的数据
{
    Rtc_rst = 0;						 //初始CE线置0
    Rtc_sclk = 0;						 //初始时钟线置0
    Rtc_rst = 1;						 //初始CE线置为1，传输开始
	Ds1302_write(Addr);					 // 地址，传输命令字
	Ds1302_write(Data);			   		 // 写1Byte数据 
	Rtc_rst = 0;   						 //读取结束，CE置0，结束数据的传输
    Rtc_sclk = 1;						 //时钟线拉高
}

/**********************
函数名称：ReadDS1302
功能描述：读出Addr对应的寄存器中的数据
入口参数：uchar cmd 寄存器地址
***********************/
uchar ReadDS1302(uchar cmd)
{
    uchar Data;
    Rtc_rst = 0;			  //初始CE线置为0
    Rtc_sclk = 0;			  //初始时钟线置为0
    Rtc_rst = 1;			  //初始CE置为1，传输开始
    Ds1302_write(cmd);        // 地址，传输命令字 
    Data =Ds1302_read();      // 读1Byte数据	  
	Rtc_rst = 0;			  //读取结束，CE置为0，结束数据的传输
    Rtc_sclk = 1; 			  //时钟线拉高
    return Data;			  //返回得到的数据
}

/**********************
函数名称：DS1302_GetTime_BCD
功能描述：读出DS1302中时、分、秒、年、月、日寄存器中对应的数据
***********************/

SYSTEMTIME DS1302_GetTime()
{
	SYSTEMTIME Time;
 	uchar ReadValue;
 	ReadValue = ReadDS1302(DS1302_SECOND_READ);
 	//将BCD码转换成十进制了
 	Time.Second=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //读取秒寄存器中的数据
	ReadValue=ReadDS1302(DS1302_MINUTE_READ);
 	Time.Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //读取分寄存器中的数据
 	ReadValue = ReadDS1302(DS1302_HOUR_READ);
 	Time.Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //读取时寄存器中的数据
	ReadValue = ReadDS1302(DS1302_DAY_READ);
 	Time.Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); 		  //读取日寄存器中的数据
 	ReadValue = ReadDS1302(DS1302_WEEK_READ);
 	Time.Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //读取周寄存器中的数据
 	ReadValue = ReadDS1302(DS1302_MONTH_READ);
 	Time.Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);		  //读取月寄存器中的数据
 	ReadValue = ReadDS1302(DS1302_YEAR_READ);
 	Time.Year=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); 			  //读取年寄存器中的数据

	return Time;
}

/**********************
函数名称：Init
功能描述：完成各部分功能模块的初始化
入口参数：无 
出口参数：无
备注：
***********************/
void Init()
{
 	P2M0=0xff;				 //设置推挽模式
   	P2M1=0x00;
    P0M0=0xff;
    P0M1=0x00;
	Led_sel=0;				 //选通数码管

 	TMOD=0x01;				//定时器0，方式1
	EA=1;					//打开总的中断
	ET0=1;					//开启定时器中断	
 	TH0=(65535-1000)/256;	//设置定时初值
	TL0=(65535-1000)%256;
	TR0=1;					//启动定时器
}
void Init_key()
{
	/*时、分、秒值*/
	set_H_val=7;
	set_Ms_val=8;
	set_S_val=9;
	set_HMS_done=0;  //时分秒设置完
	/*时、分、秒标志*/
	set_H_flag=0;
	set_Ms_flag=0;
	set_S_flag=0;
	/*默认显示时、分、秒标志*/
	show_HMS=1;
	show_set_HMS=0;
	show_key_val=0;
	key_val=0x00;
	P3M0=0x10;			   //设置推挽模式
	P3M1=0x00;
	P1M0=0x00;
	P1M1=0x00;
}
/**************************************
 * 函数名：GetADC
 * 描述  ：获得AD转换的值,没有设置A/D转换中断（具体看IE、IP）
 * 输入  ：无
 * 输出  ：AD转换的结果
 **************************************/
unsigned char GetADC()
{
	unsigned char result;
	ADC_CONTR = ADC_POWER | ADC_START | ADC_SPEED_90 | ADC_CHS1_7;//没有将ADC_FLAG置1，用于判断A/D是否结束
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!(ADC_CONTR&ADC_FLAG));//等待直到A/D转换结束
	ADC_CONTR &= ~ADC_FLAG; //ADC_FLAGE软件清0
	result = ADC_RES; //获取AD的值
	return result;	  //返回ADC值
}

/********************************
 * 函数名：Fun_Keycheck()	   
 * 描述  ：检测功能键的上5、下2、左4、右1、确认键3、开关按键3（0），没按下返回0x07，返回相应的值  (包含消抖过程)
 * 输入  ：无
 * 输出  ：键对应的值
********************************/
unsigned char Fun_Keycheck()
{
	unsigned char key;
	key=GetADC();		  //获得ADC值赋值给key
	if(key!=255)
	{
		Delayms(1);
		key=GetADC();
		if(key!=255)	  //按键消抖
		{
	     	key=key&0xE0;//获取高3位，其他位清零
        	key=_cror_(key,5);//循环右移5位
			return key;
		}
	}
	return 0x07;
}

/**********************
函数名称：Fun_Key_task_HMS
功能描述：监听功能键，完成时分秒相关值的设置，左对应时、有对应秒、确认对应分；
		  上对应值加1、下对应值减1
入口参数：无 
出口参数：无
***********************/
void Fun_Key_task_HMS()
{
	key_val=Fun_Keycheck();
	switch(key_val)	   
	{
		case 0x05:
		if(set_H_flag)//小时加1
		set_H_val=(set_H_val+1)%24;
		if(set_Ms_flag)//分钟加1
		set_Ms_val=(set_Ms_val+1)%60;
		if(set_S_flag)//秒钟加1
	   	set_S_val=(set_S_val+1)%60;
		break; 	//上  加
	    case 0x02:
		if(set_H_flag)//小时减1
		set_H_val=(set_H_val>0)?set_H_val-1:23;
		if(set_Ms_flag)//分钟减1
		set_Ms_val=(set_Ms_val>0)?set_Ms_val-1:59;
		if(set_S_flag)//秒钟减1
	   	set_S_val=(set_S_val>0)?set_S_val-1:59;
		break;  //下  减
		case 0x04:
		{
			set_H_flag=1;
			set_S_flag=0;
			set_Ms_flag=0;
		}
		break; 	//左   时
	    case 0x01:
		{
			set_S_flag=1;
			set_H_flag=0;
			set_Ms_flag=0;
		}
		break;  //右	  秒
		case 0x03:
		{
			set_Ms_flag=1;
			set_H_flag=0;
			set_S_flag=0;
		}
		break;	//确认 分
		default:break;	  
	}
}
/**********************
函数名称：Fun_key1
功能描述：监听按键1，完成时分秒设置
入口参数：无 
出口参数：无
***********************/
void Fun_key1()
{
	if(KEY1==0)// 按键2控制时分秒的设置
	{		 	
 		Delayms(5);
		while(!KEY1); //消抖
		
		set_HMS_done=0;
		show_set_HMS=1;  //显示十分秒

		show_HMS=0;

		/*默认设置分*/
		set_H_flag=0;				   //设置时间时默认设置分钟
		set_Ms_flag=1;
		set_S_flag=0;

		set_H_val=t.Hour;				//把此时的时间作为设置时间的默认值
		set_Ms_val=t.Minute;
		set_S_val=t.Second;	

		while(1)
		{

			if(KEY1==0)
			{
				Delayms(5);
				while(!KEY1);					//按键消抖
				set_H_flag=0;
				set_Ms_flag=0;
				set_S_flag=0;
				set_HMS_done=1;				   //时间设置完成
				break;						   //退出时间设置
			}
			else
			{
				Fun_Key_task_HMS();			   //进入设置选项
				while(key_val!=0x07)
				{
					key_val=Fun_Keycheck();
				}
			}
		}
	}
}
/**********************
函数名称：Key_OFFON
功能描述：设置时分秒，进行时间的校准。
入口参数：无 
出口参数：无
***********************/
void Key_OFFON()
{
	uchar temp,dtime;
	uchar table_D_BCD[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
 	t=DS1302_GetTime();

    key_val=Fun_Keycheck();
	if(key_val==0x03)
	{
		while(key_val==0x03)
		{
			key_val=Fun_Keycheck();
		}
		set_HMS_done=0;
	
	}
	if(set_HMS_done) //完成设置 对应校时功能
	{				
		show_set_HMS=0;			
		show_HMS=1;//显示时分秒 
		WriteDS1302(0x8E,0x00);	 //禁止写保护位
		temp=ReadDS1302(DS1302_SECOND_READ)|0x80;
		WriteDS1302(0x80,temp);//晶振停止工作
		/*写入时、分、秒值*/	 
		dtime=(table_D_BCD[set_S_val/10]<<4)|table_D_BCD[set_S_val%10];	
		WriteDS1302(DS1302_SECOND_WRITE,dtime);
		dtime=(table_D_BCD[set_Ms_val/10]<<4)|table_D_BCD[set_Ms_val%10];
		WriteDS1302(DS1302_MINUTE_WRITE,dtime);
		dtime=(table_D_BCD[set_H_val/10]<<4)|table_D_BCD[set_H_val%10];	
		WriteDS1302(DS1302_HOUR_WRITE,dtime);
		WriteDS1302(0x8E,0x80); //写保护位置1
		set_HMS_done = 0;
	}
}

void Initial_DS1302(void)	
{
    WriteDS1302(0x8E,0x00);			 //禁止写保护位	
	temp=ReadDS1302(DS1302_SECOND_READ)&0x7f ;
	WriteDS1302(0x80,temp);		   	//晶振开始工作
	WriteDS1302(0x8E,0x80);			 //写保护位置1							 
}	

/********************************定时器中断处理程序************************************************/
void timer0() interrupt 1	  //把数码管的显示提到中断里面来了
 {
 	TH0=(65535-1000)/256;	  //重新填充初值
	TL0=(65535-1000)%256;

	/********显示程序*******/
	flag++;						  
	if(flag==8)					  
	flag=0;
	P0=0x00;							   
	P2=weixuan[flag];
	if(show_HMS)
	{  //显示格式14-23-54（14点23分54秒）
		switch(flag){
		case 0:P0=duanxuan[t.Hour/10];break; 
	    case 1:P0=duanxuan[t.Hour%10];break; 
		case 3:P0=duanxuan[t.Minute/10];break; 
		case 4:P0=duanxuan[t.Minute%10];break; 
		case 6:P0=duanxuan[t.Second/10];break; 
		case 7:P0=duanxuan[t.Second%10];break; 
		default:P0=duanxuan[22];break;
		}
	}
	if(show_set_HMS)
	{  //显示格式14-23-54（14点23分54秒）
		switch(flag){
		case 0:P0=duanxuan[set_H_val/10];break; 
		case 1:P0=(set_H_flag==1)?duanxuan[set_H_val%10]|0x80:duanxuan[set_H_val%10];break; 
		case 3:P0=duanxuan[set_Ms_val/10];break; 
		case 4:P0=(set_Ms_flag==1)?duanxuan[set_Ms_val%10]|0x80:duanxuan[set_Ms_val%10];break; 
		case 6:P0=duanxuan[set_S_val/10];break; 
		case 7:P0=(set_S_flag==1)?duanxuan[set_S_val%10]|0x80:duanxuan[set_S_val%10];break;  
		default:P0=duanxuan[22];break;
		}
	}
	if(show_key_val)
	{
		switch(flag){
		case 0:P0=duanxuan[key_val/10];break; 
		case 1:P0=duanxuan[key_val%10];break; 
		default:P0=duanxuan[22];break;
		}
	}
}
/**********************************
 * 函数名：Init_ADC
 * 描述  ：初始化P1.7口为ADC
 * 输入  ：无
 * 输出  ：无
 **********************************/
void Init_ADC()
{
	P1ASF=P1_7_ADC;//将P1ASF寄存器对应位置1
	ADC_RES = 0;//结果寄存器清零
//	ADC_RESL=0;
	ADC_CONTR = ADC_POWER | ADC_FLAG | ADC_START | ADC_SPEED_90 | ADC_CHS1_7;		//对应位赋值
	Delayms(2);
}

void main2()
{
	Init();												 //系统初始化
	Init_ADC();											 //ADC初始化
	Init_key();											 //按键初始化
	if (ReadDS1302(DS1302_SECOND_READ)&0x80)			  //判断掉电之后时钟是否还在运行
	Initial_DS1302();									  //若正在运行，则进行DS1302初始化
	while(1)
	{
		Delayms(200);								  //延时取值，减少取值次数
		t=DS1302_GetTime();							  //从DS1302取值送给结构体t
		Fun_key1();										  
		Key_OFFON();			
		if (ReadDS1302(DS1302_SECOND_READ)&0x80)	
		Initial_DS1302();							 //取时间各数据放到结构体t中
	}	

}
