#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include"color.h"


/*�׸��� �Լ� */
void square(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form);//�簢�� �׸���
void axis(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form);// ��׸���
void draw(unsigned char spX, unsigned char spY, unsigned char type, unsigned char form);//ȭ�鿡 �׸���
void map();// Ʋ ����



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
	
		draw(spx,spy,L,form);						   // ���� ��� �𼭸�
		for(spx++; hX+ hLen -1> spx; spx++) // ��� ���� 
		{	
			draw(spx,spy,g,form);
		}
		draw(spx,spy,R,form);					     // ������ ��� �𼭸�
		for(spy++; vY+vLen -1> spy; spy++) // ������ ������
		{	
			draw(spx,spy,s,form);
		}
		draw(spx,spy,r,form);						// ������ �ϴ� �𼭸�
		for(spx--; hX < spx; spx--) 			// �ϴ� ����
		{	
			draw(spx,spy,g,form);
		}
		draw(spx,spy,l,form);							// ���� �ϴ� �𼭸�
		for(spy--; vY< spy; spy--) 			// ���� ������
		{	
			draw(spx,spy,s,form);
		}
		return;
}
void axis(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form) 
{
		unsigned char spx = hX;
		unsigned char spy = vY;
		
		if(hX == 0)	 //���� ��
		{
			draw(spx,spy,A,form);						   // ���� ��� �𼭸�
			for(spx++; hX+ hLen -1> spx; spx++)    // ��� ���� 
			{	
				draw(spx,spy,g,form);
			}
			draw(spx,spy,U,form);
		}
		if(vY == 0) // ���μ�
		{
			draw(spx,spy,N,form);
			for(spy++; vY+vLen -1> spy; spy++) // ������ ������
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






