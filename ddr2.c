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



/*�׸��� �Լ� */
void square(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form);//�簢�� �׸���
void axis(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form);// ��׸���
void draw(unsigned char spX, unsigned char spY, unsigned char type, unsigned char form);//ȭ�鿡 �׸���
void word(unsigned char strX, unsigned char strY, unsigned char attr,unsigned char *string );//ȭ�鿡 ���� �׸���
void map();// Ʋ ����


/* Ŀ��,����� ���� */
void move_cursor(int page, int x, int y); //Ŀ�� ����
void cursor_off();//Ŀ�� ������ ����
void cursor_on();// Ŀ�� ������ �ѱ�
void VAG_inverse_attrib(unsigned char far *attrib); //����
void VGA_inverse_bar(int x, int y, int length); //����� ����

/* Ű���ް� ���� �޴� �̵� */
int key();		//Ű����  Ű�� �ޱ�
void run();		//���� �۵��κ�
void menu();	//�޴��κ�


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



int key() //Ű �� �ޱ�(����,����)
{
	
	k.dir= getch();
	if(0 == k.dir)
	{
		k.dir= getch();
		return k.dir  ;
	}
	return k.dir;
}

void run() //���� �۵�
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

void menu() //�޴� �۵�
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
void move_cursor(int page, int x, int y) //Ŀ�� ����
{
	union REGS regs;
	regs.h.ah = 2;
	regs.h.dh = y;
	regs.h.dl = x;
	regs.h.bh = page;
	int86(0X10, &regs, &regs);
}
void cursor_off() //Ŀ�� ������ ����
{
	union REGS regs;
	regs.h.ah = 1;
	regs.h.ch = 0X20;
	regs.h.cl = 0;
	int86(0X10, &regs, &regs);
}
void cursor_on() // Ŀ�� ������ �ѱ�
{
	union REGS regs;
	regs.h.ah = 1;
	regs.h.ch = 0X0B;
	regs.h.cl = 0X0C;
	int86(0X10, &regs, &regs);
}
void VAG_inverse_attrib(unsigned char far *attrib) //����
{
	unsigned char origin_attrib;
	
	origin_attrib = *attrib;
	*attrib >>=4;
	*attrib = *attrib & 0X0F;
	origin_attrib <<=4;
	*attrib = *attrib | origin_attrib;
}
void VGA_inverse_bar(int x, int y, int length) //����� ����
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




void map() // Ʋ ����
{
	word(9, 2, white, "New          Load          Save         Save as        Exit ");
	word(13, 22, white, "F10 - Menu          Alt + x - Exit          F1 - Help");
	square(0,0,80,25,green); //����ū �簢��
	square(5,8,70,9,red);		//���� �簢��
	square(6,9,68,7,green);	//���� �簢�� �ȿ� �簢��
	square(5,21,70,3,green);// �ǹ� �簢��
	axis(0,4,80,0,green); 		//��� ���༱
}

void square(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form) //�簢�� �׸���
{
	unsigned char spx = hX;
	unsigned char spy = vY;

		draw(spx,spy,L,form);			    // ���� ��� �𼭸�
		for(spx++; hX+ hLen -1> spx; spx++) // ��� ���� 
		{	
			draw(spx,spy,g,form);
		}
		draw(spx,spy,R,form);			   // ������ ��� �𼭸�
		for(spy++; vY+vLen -1> spy; spy++) // ������ ������
		{	
			draw(spx,spy,s,form);
		}
		draw(spx,spy,r,form);				// ������ �ϴ� �𼭸�
		for(spx--; hX < spx; spx--) 		// �ϴ� ����
		{	
			draw(spx,spy,g,form);
		}
		draw(spx,spy,l,form);				// ���� �ϴ� �𼭸�
		for(spy--; vY< spy; spy--) 			// ���� ������
		{	
			draw(spx,spy,s,form);
		}
		return;
}
void axis(unsigned char hX, unsigned char vY, unsigned char hLen, unsigned char vLen, unsigned char form) // ��׸���
{
		unsigned char spx = hX;
		unsigned char spy = vY;
		
		if(vLen == 0)	//���� ��
		{
			draw(spx,spy,A,form);				// '��' ���
			for(spx++; hX+ hLen -1> spx; spx++) // ����
			{	
				draw(spx,spy,g,form);
			}
			draw(spx,spy,U,form);               // '��' ���
		}
		if(hLen == 0) // ���μ�
		{
			draw(spx,spy,N,form);			   // '��' ���
			for(spy++; vY+vLen -1> spy; spy++) // ������
			{	
			draw(spx,spy,s,form);
			}
			draw(spx,spy,H,form);			   // '��' ���
		}
		return;
}
void draw(unsigned char spX, unsigned char spY, unsigned char type, unsigned char form)//ȭ�鿡 �׸���
{
		unsigned char far *p;
		p =  (unsigned char far *)0XB8000000+ (spY * 160) + (spX *2);
		*p++ = type;
		*p = form;
		return;
}
void word(unsigned char strX, unsigned char strY, unsigned char attr, unsigned char *string)//ȭ�鿡 ���� �׸���
{
	for(;'\0' != *string; strX++,*string++)
	{
		draw(strX,strY,*string,attr);
	}
	return;
}


  
  


