#include "STC15F2K60S2.H"       
#include "sys.H"
#include "displayer.H"
#include "key.h"
#include "beep.H"
#include "music.h"
#include "M24C02.h"
code unsigned long SysClock=11059200;
#ifdef _displayer_H_                          //显示模块选用时必须。（数码管显示译码表，用戶可修改、增加等） 
code char decode_table[]={  0x00  ,   0x5C  ,  0x63  ,   0x10  ,        0x20       , 0x7f , 0x79 , 0x3d , 0x30 , 0x37 , 0x7c , 0x73 , 0x00 ,
                // 显示：    路       下框     上框     一格人     跳起来的一格人     B       E      G      I      N      b      p      无
	              /* 序号:      0        1         2	       3              4	          5       6	     7      8	     9	    10	   11		  12   13    14     15     */
                
                             0x3f,    0x06,     0x5b,     0x4f,         0x66,        0x6d,   0x7d,  0x07,  0x7f, 0x6f,  0xf8 ,0xcf , 0xf7 ,   0x39 , 0x5e };
                /* 序号:      13       14        15	       16            17	          18      19	   20     21    22	   23	    24		 25      26     27*/
                /* 显示:       0        1         2         3            4             5       6      7      8     9     t.     3.      R       C      d   */
             //0x3f|0x80,0x06|0x80,0x5b|0x80,0x4f|0x80,0x66|0x80,0x6d|0x80,0x7d|0x80,0x07|0x80,0x7f|0x80,0x6f|0x80 };  
             /* 显示:   0   1    2    3    4     5    6    7   8    9  (无)   下-  中-  上-  上中-  中下-   */  
             /* 带小数点     0         1         2         3         4         5         6         7         8         9        */

#endif