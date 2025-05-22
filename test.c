#include "STC15F2K60S2.H"

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
