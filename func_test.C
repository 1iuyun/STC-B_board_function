
unsigned char d0;

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

void dealwithmywrite()
{
	   unsigned char record_0;
	   if(write_sign==1)
     {
								
					record_0 = score & 0xff;
					M24C02_Write(0x77,record_0);
					Delay10ms();				
					record_0 = (score >> 8 ) & 0xff;
					M24C02_Write(0x78,record_0);
					Delay10ms();
					record_0 = (score >> 16 ) & 0xff;
					M24C02_Write(0x79,record_0);
					Delay10ms();
					record_0 = (score >> 24 ) & 0xff;
					M24C02_Write(0x80,record_0);
					write_sign=0;				
     }
}


void dealwithmykey()			              
{
	   if (GetKeyAct(enumKey2)== enumKeyPress)
     {
			   if(restart_sign==1)
			   {
					 restart_sign=0;
					 begin_sign=1;
					 score=0;
			     end_sign=0;
			     //SetMusic(Music_PM, Music_tone, &MarioBM , sizeof(MarioBM) , enumMscNull);
			     //SetPlayerMode(enumModePlay);
				 }
     }
		 
		 if (GetKeyAct(enumKey1)== enumKeyPress)
     {
			 if(high==1) 
				 high=2;
			 else if(high==2)
				 high=1;
			 if(high==d0-9)
			 {
				   end_sign=1;
				   //SetPlayerMode(enumModeStop);
				   //SetMusic(enumModeInvalid, enumModeInvalid, enumModeInvalid, enumModeInvalid, enumMscDrvSeg7);
       }
     }
		 if (GetKeyAct(enumKey3)== enumKeyPress)
		 {
			 restart_sign=1;
			 setting=2000;
			 new_record=0;
     }
}

void dealwithDisp()
	{
     
	   unsigned char d1,d2,d3,d4,d5,d6,d7,dx;
	   
		 if(begin_sign==0&&restart_sign==1)		 
		 {
       d0=5; 			 
			 d1=6;
		   d2=7;
	     d3=8;
			 d4=9;
			 d5=12;
			 d6=12;
			 d7=12;
			 Seg7Print(d0,d1,d2,d3,d4,d5,d6,d7);
     }
		 else if(begin_sign==1)
		 {
			     if(high==1&&dx==2)
					        d0=11;
					 else if(high==2&&dx==1)
						      d0=10;
			     else
                  d0=high+2;
					 
			     if(count_sign==0)
			     {
               d1=0;
               d2=0;			 
		           d3=0;
	    	       d4=0;
		           d5=0;
		           d6=0;
		           d7=0;
				       count_sign++;
           }
			     else
			     {
				      if(i==0)
				     { 
							 score++;
			         dx= d1;
					     d1= d2;
			         d2= d3;
			         d3= d4;
			         d4= d5;
			         d5= d6;
			         d6= d7;
			         if( count_sign + 1 > 64) 
               {
                  count_sign=1;
								  if(setting>1000) setting-=200;
               }
							 else
								 count_sign++;
			         d7= barrier[count_sign];
			        
							 
			         if(high==dx )
			         {
				           end_sign=1;
				    		   SetPlayerMode(enumModeStop);
               }
				   
		         }
			 	
            }
		 
	    	 if(end_sign==1)
	    	 {
		    	     d0=7; 
		    	     d1=7;
               d2=12; 
					     //if(0 < score)//重置记录条件
					     if(last_record < score)
							 {
								   new_record=1;
								   write_sign=1;
								   setting=109;
								   dealwithmywrite();
								   /*重置记录
								   M24C02_Write(0x77,0x00);
								   Delay10ms();
								   M24C02_Write(0x78,0x00);
								   Delay10ms();
								   M24C02_Write(0x79,0x00);
								   Delay10ms();
								   M24C02_Write(0x80,0x00);
								   */
               }
               else							 
							 {
								   SetMusic(Music_PM, Music_tone, &MarioEN , sizeof(MarioEN) , enumMscNull);
								   restart_sign=0;								 
                   end_sign=0;
					         begin_sign=0;
					         count_sign=0;
		    	     }
							 
							 SetPlayerMode(enumModePlay);
               
							 d3=11; 
               d4=23; 
               
               d5=score/100+13;
							 d6=(score/10)%10+13; 
							 d7=score%10+13;
							 
							 /*
							 d5=last_record /100+13;
							 d6=(last_record /10)%10+13; 
							 d7=last_record %10+13;
							 */
         }
				 Seg7Print(d0,d1,d2,d3,d4,d5,d6,d7);
     }
		 if(new_record==1)
		{
					    unsigned char signx[]={0, 25,27, 0 , 9 , 6, 24, 11,23 ,0, 0, 0,  0, 0 , 0,   9,  6, 24 ,0 ,25, 6 ,26}; 
  		        //                     0   1  2   3   4   5   6  7  8  9 10  11 12  13  14   15  16 17  18 19 20  21 
					    if(i==0)
							{
								d0=9;
								d1=6;
								d2=24;
								d3=0;
								d4=25;
								d5=6;
								d6=26;
								d7=13;
              }		    
					    else if(i%5==0)
							{
								d0=d1;
								d1=d2;
								d2=d3;
								d3=d4;
								d4=d5;
								d5=d6;
								d6=d7;
								if(i<45)
								d7=signx[i/5];
								else if(i==45)
								d7=score/100+13;
								else if(i==50)
								d7=(score/10)%10+13; 
								else if(i==55)
								d7=score%10+13;
								else 
							  d7=signx[i/5];
              }
					    restart_sign=0;								 
              end_sign=0;
					    begin_sign=0;
					    count_sign=0;
					    Seg7Print(d0,d1,d2,d3,d4,d5,d6,d7);
     }
}  
		 
