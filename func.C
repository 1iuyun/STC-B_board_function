
unsigned char d0;

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
			     SetMusic(Music_PM, Music_tone, &MarioBM , 140 , enumMscNull);
			     SetPlayerMode(enumModePlay);
				 }
     }
		 
		 if (GetKeyAct(enumKey1)== enumKeyPress)
     {
			 if(high==1) 
				 high=2;
			 else if(high==2)
				 high=1;
			 if(high==d0-9 )
			 {
				   end_sign=1;
				   SetPlayerMode(enumModeStop);
       }
     }
		 if (GetKeyAct(enumKey3)== enumKeyPress)
		 {
			 restart_sign=1;
			 setting=1300;
     }
     
}

void dealwithDisp()
{
     
	   /*unsigned char barrier[65]={0,1,0,0,2,0,0,1,0,2,0,0,1,0,0,2,0,0,
			                          1,0,0,0,1,0,0,0,2,2,0,0,2,0,0,1,0,1,
			                          0,0,0,2,0,0,1,0,0,1,0,0,2,0,2,0,0,1,
                                0,0,2,0,0,1,0,2,0,0,0};
	   */
	   unsigned char /*d0,*/d1,d2,d3,d4,d5,d6,d7,dx;

		 
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
               d3=12; 
               d4=12; 
               d5=12; 
               d6=12; 
               d7=12;
					     
               SetMusic(Music_PM, Music_tone, &MarioEN , sizeof(MarioEN) , enumMscNull);
		    	     SetPlayerMode(enumModePlay);
               restart_sign=0;								 
               end_sign=0;
					     begin_sign=0;
					     count_sign=0;
         }
				 Seg7Print(d0,d1,d2,d3,d4,d5,d6,d7);
     }
}  
		 
