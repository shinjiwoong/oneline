#include<stdio.h>
#include<dos.h>
#include<conio.h>

/zzz/
#define	s	179 // ���� 
#define g	196  //����
#define L	218 //������  
#define R	191 //������ �� 
#define l	192 // ���� �Ʒ� 
#define r	217 //������ �Ʒ�
#define A	195 // ��
#define U	180 //��

unsigned char far *p = (unsigned char far *)0XB8000000;

void make(int ch, int attr);
void map();
void menu();
void VAG_inverse_attrib(unsigned char far *attrib);
void VGA_inverse_bar(int x, int y, int length);
void cursor_on();
void cursor_off();
void move_cursor(int page, int x, int y);

void keym();
int keyv();



int main()
{ 
	int a=0;

	clrscr();
	map();
	menu();	
	keym();

	//puttext(8,2,80,4,&mem);	
	//VGA_inverse_bar(8, 2, 7);
	return 0;
}

void keym()
{
	
	//int key=0;
	//int x=0;
	//unsigned char mem[100];
	//gettext(8,2,80,4,&mem);
	//key = keyv();
	if(key == 68)					//F10�� �޾������
	{
		VGA_inverse_bar(8, 2, 7);
	}
}

int keyv() //Ű �� �ޱ�(����,����)
{
	int low = 0;
	int high = 0;
	
	low = getch();
	if(0 == low)
	{
		high = getch();
		return high;
	}
	return low;
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
void menu() //�޴� �����
{
	unsigned char far *loc;
	char word[]="New          Load          Save          Save as        Exit ";
	char menu[]="F10 - Menu          Alt + x - Exit          F1 - Help";
	int x;
	int cnt;
	for(cnt=0,x=9; 71>x; x++)
	{
		loc = (unsigned char far *)0XB8000000+(2 * 160) + (x *2);
		*loc = word[cnt];
		++loc;
		*loc= 15;
		++loc;
		cnt++;
	}
	for(cnt=0,x=13; 67>x; x++)
	{
		loc = (unsigned char far *)0XB8000000+(21* 160) + (x *2);
		*loc = menu[cnt];
		++loc;
		*loc= 15;
		++loc;
		cnt++;
	}
}
void make(int ch, int attr) // video memory set
{
	*p=ch;
	++p;
	*p=attr;
	++p;
	return;
}
void map() // ���� �����
{
	int a;
	int b;
	int ch;
	int attr=15;

	unsigned char map[25][81]=
	{	
		"LggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggR",
		"s                                                                              s",
		"s                                                                              s",
		"s                                                                              s",
		"AggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggU",
		"s                                                                              s",
		"s                                                                              s",
		"s  LggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggR  s",
		"s  s LggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggR s  s",
		"s  s s                                                                    s s  s",
		"s  s s                                                                    s s  s",
		"s  s s                                                                    s s  s",
		"s  s s                                                                    s s  s",
		"s  s s                                                                    s s  s",
		"s  s s                                                                    s s  s",
		"s  s s                                                                    s s  s",
		"s  s lggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggr s  s",
		"s  lggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggr  s",
		"s                                                                              s",
		"s  LggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggR  s",
		"s  s                                                                        s  s",
		"s  s                                                                        s  s",
		"s  s                                                                        s  s",
		"s  lggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggr  s",
		"lggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggr",
	 
	 };

	 for(a=0; 25>a; ++a)
	 {
		for(b=0; 80>b; ++b)
		{
			if(map[a][b]=='L')
			{
				ch = L;
			}
			else if(map[a][b]=='g')
			{
				ch = g;
			}
			else if(map[a][b]=='R')
			{
				ch = R;
			}
			else if(map[a][b]=='s')
			{
				ch = s;
			}
			else if(map[a][b]=='l')
			{
				ch = l;
			}
			else if(map[a][b]=='r')
			{
				ch = r;
			}
			else if(map[a][b]=='g')
			{
				ch = g;
			}
			else if(map[a][b]=='A')
			{
				ch = A;
			}
			else if(map[a][b]=='U')
			{
				ch = U;
			}
			else if(map[a][b]==' ')
			{
				ch = 0;
			}
			make(ch,attr);
		}
	 }
	 return;
}

