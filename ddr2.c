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
	char character; // 입력받은 하나의 문자
	int char_number; // 각 행에서 문자의 일련번호
	struct char_tag *prev; // 앞의 char_tag 구조체(앞의 문자)를 가리키는 포인터
	struct char_tag *next; // 다음 char_tag구조체(다음 문자)를 가리키는 포인터
} DL;





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

/* 키값받고 작동  */
int key();		//키보드  키값 받기
void run();		//메인 작동부분
void menu();	//메뉴 움직임 부분


/*연결리스트 */
DL *Insert(DL *);	// 연결리스트 연결 및 삽입
DL *Delete(DL *);	// 연결리스트 삭제
DL *Free(DL *);		// 연결리스트 해제

KEY  k;
DL		*d;
DL		*q;
int main()
{ 
	clrscr();
	map();
	gettext(0,0,80,7,&k.mem);	// 현재 화면 저장 
	k.xPosition = start;
	move_cursor(0,k.xPosition,yPosition);
	
	k.kx = 8;
	k.ky = 13;
	run();
	
	return 0;
}


/* 구동 */

void run() //메인 작동
{
	k.cnt = 0;
	while(1)
	{
		k.Lkey= getch();
		
		if(0 == k.Lkey)
		{
			k.Hkey= getch();
		}
		
		if((k.Hkey == F10)&&(0 == k.Lkey))	//메뉴 
		{
			menu();
		}
		
		if((77== k.Hkey)||(75 ==k.Hkey)||(0 != k.Lkey)) //방향키 제어
		{
		
			if((0 != k.Lkey)&&(k.xPosition != end+1)) //키 받는 부분 
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
	

	
		
		if(k.Lkey == esc) //종료
		{
			return;
		}
	}
	return;
}



void menu() //메뉴 작동
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


/* 역상바 커서 생성 */
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



/* 화면에 그리기 */
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


 ////////////////////////////////////////////////////
DL *Insert(DL *list)	/********************** 10월 22일 추가 함수(23일 보완) *****************************/
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
	newnode->character = k.Lkey;	// 삽입될 단어를 구조체에 넣는다.

	while((current != NULL)&&(current->char_number < k.xPosition))	// 커서위치값과 같을때까지 찾는다.
	{
		follow = current;	// follow는 current의 앞의 연결리스트를 가리킨다.
		current = current->next;	// 다음 연결리스트로 이동
	}
	newnode->next = current;

	if(current == list)	// 처음으로 만들 때,
	{
		list = newnode;	
		newnode->prev = list;	// 헤더의 주소를 준다.
	}
	else	// 연결되어 있으면,
	{
		follow->next = newnode;	// 중간과 끝
		newnode->prev = follow; // 그 앞의 연결리스트의 주소를 가리킨다.
	}

	if(current != NULL)	// 뒤에 남겨진 연결리스트가 있으면,
	{
		current->prev = newnode;	// 연결리스트 연결
	}

	while(final != NULL)	// 처음부터 연결리스트를 훑어 일련번호를 다시 순번대로 부여한다.
	{
		final->char_number = iNum;	// 일련번호 넣어준다.
		final = final->next;	// 다음 연결리스트 이동
		iNum ++;		// 일련번호 ++
	}
	iNum = 1;	// 일련번호 초기화

	return list;
}

DL *Delete(DL *list)		/********************** 10월 24일 추가 함수 *****************************/
{
	DL *current;
	DL *follow;
	DL *newnode;
	DL *final;
	int iNum = 1;

	final = current = follow = list;
	newnode = current->next;	// 현재 연결리스트의 뒤쪽을 가리킨다.

	if(0 == (k.xPosition-1)) // 커서위치가 제일 앞에 있을 시에
	{
		return list;
	}

	while((current != NULL)&&(current->char_number != (k.xPosition-1)))
	{
		follow = current;
		current = current->next;
		newnode = current->next;
	}

	if(current == NULL)		// 지울 연결리스트가 없으면
	{
		return list;
	}

	if(list == current)	// 첫번째 노드의 삭제
	{
		list = current->next;
		newnode->next = list;
	}
	else if(current->next == NULL)	// 마지막 노드의 삭제
	{
		follow->next = NULL;
	}
	else		// 가운데 노드의 삭제
	{
		follow->next = current->next;
		newnode->prev = follow;
	}

	while(final != NULL)	// 처음부터 연결리스트를 훑어 일련번호를 다시 순번대로 부여한다.
	{
		final->char_number = iNum;	// 일련번호 넣어준다.
		final = final->next;	// 다음 연결리스트 이동
		iNum ++;		// 일련번호 ++
	}
	iNum = 1;	// 일련번호 초기화

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

  


