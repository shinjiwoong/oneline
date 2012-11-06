#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include"color.h"



typedef struct key
{
	int dir;
	int Hkey;
	int Lkey;
	int xPosition;
	int bar;
	int cnt;
	int kx;
	int ky;
	unsigned char mem[100];
	
	
} KEY;

typedef struct char_tag 
{
	char character; // �Է¹��� �ϳ��� ����
	int char_number; // �� �࿡�� ������ �Ϸù�ȣ
	struct char_tag *prev; // ���� char_tag ����ü(���� ����)�� ����Ű�� ������
	struct char_tag *next; // ���� char_tag����ü(���� ����)�� ����Ű�� ������
} DL;





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

/* Ű���ް� �۵�  */
int key();		//Ű����  Ű�� �ޱ�
void run();		//���� �۵��κ�
void menu();	//�޴� ������ �κ�


/*���Ḯ��Ʈ */
DL *Insert(DL *);	// ���Ḯ��Ʈ ���� �� ����
DL *Delete(DL *);	// ���Ḯ��Ʈ ����
DL *Free(DL *);		// ���Ḯ��Ʈ ����

KEY  k;
DL		*d;
DL		*q;
int main()
{ 
	clrscr();
	map();
	gettext(0,0,80,7,&k.mem);	// ���� ȭ�� ���� 
	k.xPosition = start;
	move_cursor(0,k.xPosition,yPosition);
	
	k.kx = 8;
	k.ky = 13;
	run();
	
	return 0;
}


/* ���� */

void run() //���� �۵�
{
	k.cnt = 0;
	while(1)
	{
		k.Lkey= getch();
		
		if(0 == k.Lkey)
		{
			k.Hkey= getch();
		}
		
		if((k.Hkey == F10)&&(0 == k.Lkey))	//�޴� 
		{
			menu();
		}
		
		if((77== k.Hkey)||(75 ==k.Hkey)||(0 != k.Lkey)) //����Ű ����
		{
		
			if((0 != k.Lkey)&&(k.xPosition != end+1)) //Ű �޴� �κ� 
			{
				draw(k.xPosition, yPosition, k.Lkey, 15);
				//q = Insert(d);
				//printf("word: %c, num : %d\n", q->character, q->char_number);
				++k.xPosition;
				move_cursor(0,k.xPosition,yPosition);
			
			}
			
			if((77== k.Hkey)&&(k.xPosition != end+1)&&(0 == k.Lkey))
			{	
				
				
				++k.xPosition;
				move_cursor(0,k.xPosition,yPosition);
		
			}
			if((75== k.Hkey)&&(k.xPosition != start)&&(0 == k.Lkey))
			{	
				--k.xPosition;
				move_cursor(0,k.xPosition,yPosition);
			
			}
		
		}
	

	
		
		if(k.Lkey == esc) //����
		{
			return;
		}
	}
	return;
}



void menu() //�޴� �۵�
{
		k.bar = sp;
		VGA_inverse_bar(k.bar, bar_po, bar_len);
		while(1)
		{
			k.dir = key();
			
			if(( right == k.dir) && (k.bar != 63))
			{
				puttext(0,0,80,7,&k.mem);	
				k.bar = k.bar + move;
				VGA_inverse_bar(k.bar, bar_po, bar_len);		
				
				
			}
			if(( left == k.dir) && (k.bar != 7))
			{
				puttext(0,0,80,7,&k.mem);
				k.bar = k.bar - move;
				VGA_inverse_bar(k.bar, bar_po, bar_len);
				
			}
			
			if( esc == k.dir)
			{
				puttext(0,0,80,7,&k.mem);	
				return;
			}
			
		}
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


/* ����� Ŀ�� ���� */
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



/* ȭ�鿡 �׸��� */
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


 ////////////////////////////////////////////////////
DL *Insert(DL *list)	/********************** 10�� 22�� �߰� �Լ�(23�� ����) *****************************/
{
	DL *current = NULL;
	DL *follow = NULL;
	DL *newnode = NULL;
	DL *final = NULL;
	int iNum = 1;


	final = current = follow = list;
	
	newnode = (DL *)malloc(sizeof(DL)) ;
	if(newnode== NULL)
	{
		printf("No memory allocated..\n");
		return list;
	}
	newnode->character = k.Lkey;	// ���Ե� �ܾ ����ü�� �ִ´�.

	while((current != NULL)&&(current->char_number < k.xPosition))	// Ŀ����ġ���� ���������� ã�´�.
	{
		follow = current;	// follow�� current�� ���� ���Ḯ��Ʈ�� ����Ų��.
		current = current->next;	// ���� ���Ḯ��Ʈ�� �̵�
	}
	newnode->next = current;

	if(current == list)	// ó������ ���� ��,
	{
		list = newnode;	
		newnode->prev = list;	// ����� �ּҸ� �ش�.
	}
	else	// ����Ǿ� ������,
	{
		follow->next = newnode;	// �߰��� ��
		newnode->prev = follow; // �� ���� ���Ḯ��Ʈ�� �ּҸ� ����Ų��.
	}

	if(current != NULL)	// �ڿ� ������ ���Ḯ��Ʈ�� ������,
	{
		current->prev = newnode;	// ���Ḯ��Ʈ ����
	}

	while(final != NULL)	// ó������ ���Ḯ��Ʈ�� �Ⱦ� �Ϸù�ȣ�� �ٽ� ������� �ο��Ѵ�.
	{
		final->char_number = iNum;	// �Ϸù�ȣ �־��ش�.
		final = final->next;	// ���� ���Ḯ��Ʈ �̵�
		iNum ++;		// �Ϸù�ȣ ++
	}
	iNum = 1;	// �Ϸù�ȣ �ʱ�ȭ

	return list;
}

DL *Delete(DL *list)		/********************** 10�� 24�� �߰� �Լ� *****************************/
{
	DL *current;
	DL *follow;
	DL *newnode;
	DL *final;
	int iNum = 1;

	final = current = follow = list;
	newnode = current->next;	// ���� ���Ḯ��Ʈ�� ������ ����Ų��.

	if(0 == (k.xPosition-1)) // Ŀ����ġ�� ���� �տ� ���� �ÿ�
	{
		return list;
	}

	while((current != NULL)&&(current->char_number != (k.xPosition-1)))
	{
		follow = current;
		current = current->next;
		newnode = current->next;
	}

	if(current == NULL)		// ���� ���Ḯ��Ʈ�� ������
	{
		return list;
	}

	if(list == current)	// ù��° ����� ����
	{
		list = current->next;
		newnode->next = list;
	}
	else if(current->next == NULL)	// ������ ����� ����
	{
		follow->next = NULL;
	}
	else		// ��� ����� ����
	{
		follow->next = current->next;
		newnode->prev = follow;
	}

	while(final != NULL)	// ó������ ���Ḯ��Ʈ�� �Ⱦ� �Ϸù�ȣ�� �ٽ� ������� �ο��Ѵ�.
	{
		final->char_number = iNum;	// �Ϸù�ȣ �־��ش�.
		final = final->next;	// ���� ���Ḯ��Ʈ �̵�
		iNum ++;		// �Ϸù�ȣ ++
	}
	iNum = 1;	// �Ϸù�ȣ �ʱ�ȭ

	free(current);
	return list;
}

DL *Free(DL *list)
{
	DL *temp;
	DL *current;

	for(temp = list; temp != NULL; temp = current)
	{
		current = temp->next;
		free(temp);
	}
	return 0;
}

  


