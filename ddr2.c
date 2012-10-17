#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include"color.h"



typedef struct key
{
	int dir;
	int loc;
	unsigned char mem[100];
	
} KEY;



/*그리기 함수 */
void square(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form);//사각형 그리기
void axis(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form);// 축그리기
void draw(unsigned char spX, unsigned char spY, unsigned char type, unsigned char form);//화면에 그리기
void word(unsigned char strX, unsigned char strY, unsigned char attr,unsigned char *string );//화면에 문자 그리기
void map();// 틀 구성


/* 커서,역상바 생성 */
void move_cursor(int page, int x, int y); //커서 생성
void cursor_off();//커서 깜빢임 끄기
void cursor_on();// 커서 깜빡임 켜기
void VAG_inverse_attrib(unsigned char far *attrib); //역상
void VGA_inverse_bar(int x, int y, int length); //역상바 생성

/* 키값받고 메인 메뉴 이동 */
int key();		//키보드  키값 받기
void run();		//메인 작동부분
void menu();	//메뉴부분


KEY  k;
int main()
{ 
	
	clrscr();
	map();
	gettext(8,2,80,4,&k.mem);	
	run();
	
	
	
	
	//getch();
	return 0;
}



int key() //키 값 받기(상위,하위)
{
	
	k.dir= getch();
	if(0 == k.dir)
	{
		k.dir= getch();
		return k.dir  ;
	}
	return k.dir;
}

void run() //메인 작동
{
	while(1)
	{
		switch(key())
		{
			case F10:
			menu();
			break;
		
			case esc:
			clrscr();
			return;
		
			default:
			return;
		}
	}
	return;
}

void menu() //메뉴 작동
{
		k.loc = sp;
		VGA_inverse_bar(k.loc, bar_po, bar_len);
		while(1)
		{
			k.dir = key();
			if(( right == k.dir) && (k.loc != 63))
			{
				puttext(8,2,80,4,&k.mem);	
				k.loc = k.loc + move;
				VGA_inverse_bar(k.loc, bar_po, bar_len);		
			}
			if(( left == k.dir) && (k.loc != 7))
			{
				puttext(8,2,80,4,&k.mem);	
				k.loc = k.loc - move;
				VGA_inverse_bar(k.loc, bar_po, bar_len);
			}
			if( esc == k.dir)
			{
				puttext(8,2,80,4,&k.mem);	
				return;
			}
		}
}
void move_cursor(int page, int x, int y) //커서 생성
{
	union REGS regs;
	regs.h.ah = 2;
	regs.h.dh = y;
	regs.h.dl = x;
	regs.h.bh = page;
	int86(0X10, &regs, &regs);
}
void cursor_off() //커서 깜빢임 끄기
{
	union REGS regs;
	regs.h.ah = 1;
	regs.h.ch = 0X20;
	regs.h.cl = 0;
	int86(0X10, &regs, &regs);
}
void cursor_on() // 커서 깜빡임 켜기
{
	union REGS regs;
	regs.h.ah = 1;
	regs.h.ch = 0X0B;
	regs.h.cl = 0X0C;
	int86(0X10, &regs, &regs);
}
void VAG_inverse_attrib(unsigned char far *attrib) //역상
{
	unsigned char origin_attrib;
	
	origin_attrib = *attrib;
	*attrib >>=4;
	*attrib = *attrib & 0X0F;
	origin_attrib <<=4;
	*attrib = *attrib | origin_attrib;
}
void VGA_inverse_bar(int x, int y, int length) //역상바 생성
{
	int i = 0;
	unsigned char far *attr_memory = (unsigned char far *) 0XB8000001L;
	attr_memory = attr_memory + y * 160 + x * 2;
	
	for(i = 0; i < length; i++)
	{
		VAG_inverse_attrib(attr_memory);
		attr_memory += 2;
	}
}




void map() // 틀 구성
{
	word(9, 2, white, "New          Load          Save         Save as        Exit ");
	word(13, 22, white, "F10 - Menu          Alt + x - Exit          F1 - Help");
	square(0,0,80,25,green); //제일큰 사각형
	square(5,8,70,9,red);		//빨간 사각형
	square(6,9,68,7,green);	//빨간 사각형 안에 사각형
	square(5,21,70,3,green);// 맨밑 사각형
	axis(0,4,80,0,green); 		//상단 평행선
}

void square(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form) //사각형 그리기
{
	unsigned char spx = hX;
	unsigned char spy = vY;

		draw(spx,spy,L,form);			    // 왼쪽 상단 모서리
		for(spx++; hX+ hLen -1> spx; spx++) // 상단 수평선 
		{	
			draw(spx,spy,g,form);
		}
		draw(spx,spy,R,form);			   // 오른쪽 상단 모서리
		for(spy++; vY+vLen -1> spy; spy++) // 오른쪽 수직선
		{	
			draw(spx,spy,s,form);
		}
		draw(spx,spy,r,form);				// 오른쪽 하단 모서리
		for(spx--; hX < spx; spx--) 		// 하단 수평선
		{	
			draw(spx,spy,g,form);
		}
		draw(spx,spy,l,form);				// 왼쪽 하단 모서리
		for(spy--; vY< spy; spy--) 			// 왼쪽 수직선
		{	
			draw(spx,spy,s,form);
		}
		return;
}
void axis(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form) // 축그리기
{
		unsigned char spx = hX;
		unsigned char spy = vY;
		
		if(vLen == 0)	//가로 선
		{
			draw(spx,spy,A,form);				// 'ㅏ' 모양
			for(spx++; hX+ hLen -1> spx; spx++) // 수평선
			{	
				draw(spx,spy,g,form);
			}
			draw(spx,spy,U,form);               // 'ㅓ' 모양
		}
		if(hLen == 0) // 세로선
		{
			draw(spx,spy,N,form);			   // 'ㅜ' 모양
			for(spy++; vY+vLen -1> spy; spy++) // 수직선
			{	
			draw(spx,spy,s,form);
			}
			draw(spx,spy,H,form);			   // 'ㅗ' 모양
		}
		return;
}
void draw(unsigned char spX, unsigned char spY, unsigned char type, unsigned char form)//화면에 그리기
{
		unsigned char far *p;
		p =  (unsigned char far *)0XB8000000+ (spY * 160) + (spX *2);
		*p++ = type;
		*p = form;
		return;
}
void word(unsigned char strX, unsigned char strY, unsigned char attr, unsigned char *string)//화면에 문자 그리기
{
	for(;'\0' != *string; strX++,*string++)
	{
		draw(strX,strY,*string,attr);
	}
	return;
}


  
  


