#include"main.H"
#include"MarioBM.C"
unsigned int last_record;
unsigned int setting,i;
unsigned char Music_tone,Music_PM;  // ���ֲ����������ڴգ�ÿ���ӽ�������
unsigned char high;
unsigned char begin_sign,end_sign,restart_sign,count_sign,write_sign;
unsigned char new_record,score;
unsigned char record0,record1,record2,record3;

code char barrier[]={0,1,0,0,2,0,0,1,0,2,0,0,1,0,0,2,0,0,
			               1,0,0,0,1,0,0,0,2,2,0,0,2,0,0,1,0,1,
			               0,0,0,2,0,0,1,0,0,1,0,0,2,0,2,0,0,1,
                     0,0,2,0,0,1,0,2,0,0,1,0,0,2,0,1,0,2,
                     0,0,0,1,0,1,0,2,0,1,0,0,2,0,0,1,0,0};
//#include"fun.c"
#include"func_test.C"


void my100mS_callback()			                //������100mS�¼��ص�����
{ 
	i++;
	if(i>setting) i=0;
	dealwithDisp();	
}


void mykey_callback()		                            // ������Key1��Key2���¼��ص�����
{	dealwithmykey();	  
}


void main(){
	
	
  Key_Init(); 
  DisplayerInit();
  BeepInit();
  MusicPlayerInit();
	
	
	  SetEventCallBack(enumEventKey, mykey_callback);            //����
		//SetEventCallBack(enumEventSys1mS, my1mS_callback);         //����
		//SetEventCallBack(enumEventSys10mS, my10mS_callback);       //����
		SetEventCallBack(enumEventSys100mS, my100mS_callback); 
	
	
	  SetDisplayerArea(0,7);


	  Music_PM=90;//90
		Music_tone=0xFC;
	  high=1;
		begin_sign=0;
		end_sign=0;
		restart_sign=1;
	  count_sign=0;
		write_sign=0;
		setting=2000;
		score=0;
		new_record=0;
		
		record0 = M24C02_Read(0x77);
	  record1 = M24C02_Read(0x78);
		record2 = M24C02_Read(0x79);
		record3 = M24C02_Read(0x80);
		last_record = (record3<<24) | (record2<<16) | (record1<<8)	| record0 ;
	  MySTC_Init();
	while(1)
	{ MySTC_OS();
		
		mykey_callback();
		
		
		if(begin_sign==1)
		{
		my100mS_callback();
    } 
  }
  }