#include<stdio.h>
#include<dos.h>
#include<conio.h>

#define	s	179 // ���� 
#define g	196  //����
#define L	218 //������  
#define R	191 //������ �� 
#define l	192 // ���� �Ʒ� 
#define r	217 //������ �Ʒ�
#define A	195 // ��
#define U	180 //��

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


