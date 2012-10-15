#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include"color.h"


/*그리기 함수 */
void square(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form);//사각형 그리기
void axis(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form);// 축그리기
void draw(unsigned char spX, unsigned char spY, unsigned char type, unsigned char form);//화면에 그리기
void map();// 틀 구성



int main()
{ 
	clrscr();
	map();
	
	getch();
	return 0;
}
void map() 
{
	square(0,0,80,22,green);
	axis(0,3,80,0,green);
	square(5,6,70,9,red);
	square(6,7,68,7,green);
	square(5,17,70,4,green);
}

void square(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form) 
{
	unsigned char spx = hX;
	unsigned char spy = vY;
	
		draw(spx,spy,L,form);						   // 왼쪽 상단 모서리
		for(spx++; hX+ hLen -1> spx; spx++) // 상단 수평선 
		{	
			draw(spx,spy,g,form);
		}
		draw(spx,spy,R,form);					     // 오른쪽 상단 모서리
		for(spy++; vY+vLen -1> spy; spy++) // 오른쪽 수직선
		{	
			draw(spx,spy,s,form);
		}
		draw(spx,spy,r,form);						// 오른쪽 하단 모서리
		for(spx--; hX < spx; spx--) 			// 하단 수평선
		{	
			draw(spx,spy,g,form);
		}
		draw(spx,spy,l,form);							// 왼쪽 하단 모서리
		for(spy--; vY< spy; spy--) 			// 왼쪽 수직선
		{	
			draw(spx,spy,s,form);
		}
		return;
}
void axis(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form) 
{
		unsigned char spx = hX;
		unsigned char spy = vY;
		
		if(hX == 0)	 //가로 선
		{
			draw(spx,spy,A,form);						   // 왼쪽 상단 모서리
			for(spx++; hX+ hLen -1> spx; spx++)    // 상단 수평선 
			{	
				draw(spx,spy,g,form);
			}
			draw(spx,spy,U,form);
		}
		if(vY == 0) // 세로선
		{
			draw(spx,spy,N,form);
			for(spy++; vY+vLen -1> spy; spy++) // 오른쪽 수직선
			{	
			draw(spx,spy,s,form);
			}
			draw(spx,spy,H,form);
		}
		return;
}
void draw(unsigned char spX, unsigned char spY, unsigned char type, unsigned char form)
{
		unsigned char far *p;
		p =  (unsigned char far *)0XB8000000+ (spY * 160) + (spX *2);
		*p++ = type;
		*p = form;
		return;
}






