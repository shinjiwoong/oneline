#include<stdio.h>
#include<dos.h>
#include<conio.h>

#define	s	179 // 세로 
#define g	196  //가로
#define L	218 //왼쪽위  
#define R	191 //오른쪽 위 
#define l	192 // 왼쪽 아래 
#define r	217 //오른쪽 아래
#define A	195 // ㅏ
#define U	180 //ㅓ

unsigned char far *p = (unsigned char far *)0XB8000000;

void paint(int x, int y, int ch ,int attr);
void word(int x,int y,int ch,int attr);

int main()
{ 
	clrscr();
	word(30,15,65,15);

	
	paint(0,0,10,150);
	getch();
	return 0;
}

void paint(int x, int y, int garo ,int sero)
{

	for(x=0; sero>x; x++)
	{
		for(y=0; garo>y; y++)
		{	
			word(x,y,g,15);
		
		}
	}
}
void word(int x,int y,int ch,int attr)
{
		p =  (unsigned char far *)0XB8000000+ (y * 160) + (x *2);
		*p = ch;
		++p;
		*p = attr;
		++p;
		return;
}


