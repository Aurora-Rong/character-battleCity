#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include<windows.h>
#include<conio.h>
#include<ctype.h>
#include<ctime>
#include<cstdio>
#include<list>
#define ZERO SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0})
#define POSAT(a,b) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {(a),(b)})
using namespace std;

struct Point{
	int x,y;
};
class Prt{
private:
	int length = 0;
	int Len = 60;		//�߿��
	int padding = 2;	//�����ڱ߾�
	int margin = 2;		//��߾�
	int win = 1;		//�����win10���͸�Ϊ1�������win7���͸�Ϊ2 
public:
	void setPadding(int padding_){padding = padding_;}
	void setMargin(int margin_){margin = margin_;}
	int getPadding(){return padding;}
	int getMargin(){return margin;}
	void mar();						//���������߾� 
	void pLine(char str[],int len_);			//������� 
	void print(char str[]);			//���һ�� 
	void borP(char str[]);			//���Զ��ױ߿�ĸ�ʽ���,�������Ҫ�������,����&������,���Ҫ����ָ��ߣ�������~����� 
	void borPLogo(char str[]);		//��logo���ױ߿��ʽ��� 
};
struct Map{			//��ͼ 
	int x;		//��ͼ����������ǽ 
	int y;		//��ͼ�� 
	char name[30];
	char  map_[200][200];		//��ͼ���±��1��ʼ 
	int lifeTotal_e;			//�з�̹������ 
};
class Tank{			//̹���� 
private:
	int x,y;			//���� 
	int camp;			//������Ӫ ,1Ϊ�Ѿ���2Ϊ�о� 
	int dir;			//��������,1 2 3 4 ��Ӧ �������� 
	bool life = true;	//�Ƿ��� 
	char shape[3];
	clock_t moveLastTime = 0;
	clock_t fireLastTime = 0;
public:
	static int fire_cd_e;
	static int fire_cd_f;		//������ȴʱ�� ����λ������ 
	static int move_cd_e;
	static int move_cd_f;
	static int reborn_cd;
	static int e;			//���ϵз�̹������
	static int max_e;		//����ϳ��з�̹������ 
	Tank();
	Tank(int x_,int y_,int camp_,int dir_);
	Tank(const Tank& t);
	~Tank();
	void move();	//�ƶ�  
	void fire();	//���� 
	bool isLife(){return life;}	
	void kill();						//ɱ����̹�� ������̹������ʱ 
	void turnDir(int dir);	
	void turnDir();						//�޲�һ��Ϊ�з�̹���Զ����� 
	static void createTank_e();			//���ɵз�̹�� 
	static void createTank_f();			//�������̹�� 
	void print();
	Point getPoint();
};
class Shell{		//�ڵ��� 
private:
	int x,y;
	int camp;
	int dir;		//�ƶ����� 
	int life = true;
	char shape[3];	
	clock_t _time = 0;	
public:
	static int move_cd;		//�ƶ���ȴʱ�� ����λ������ 
	Shell();
	Shell(int x_,int y_,int camp_,int dir_);
	void move();	//�ƶ� 
	bool isLife(){return life;}
	void kill(){life = false;}
	void print();
	Point getPoint();
};
class Prop{						//������
private: 
	int id;						//ָ�����ĸ�����
	int x,y;					//����
	char shape[3];				//�ڵ�ͼ�ϵ���ʾ
	clock_t propTime = 0;		//����Ч����ǰ����ʱ�� 
	clock_t max_propTime;		//����Ч������ʱ�� 
	clock_t maxTime = 8000;		//���ߴ��ʱ�� 
	clock_t lifeTime;			//���ߵ�ǰ���ʱ�� 
	bool pickup = false;
	int tmp;					//��ŵ��߸ı�����Ե�ԭ��ֵ��ʱ��һ���ٱ���� 
public:
	friend void delProp();
	Prop(int id_,int x_,int y_);
	bool ispickup(){return pickup;}
	void pup(){pickup = true;}
	void show();				//���߷���Ч��,ʰȡʱ���� 
	bool isLife(){return clock()-lifeTime<maxTime;}
	void print();				//��ӡ����ͼ�� 
	Point getPoint();
	bool regain();				//��ʱ���ѵ��ĵ���Ч��ʧЧ ,�����Ƿ����ɾ�� 
};
struct Game{			//��Ϸ���� 
	vector<Map> maps;			//��׼��ͼ��
	Map map;					//�þ���Ϸ��ͼ 
	Map tmpMap;					//ÿһ��ˢ��ʱ����ĵ�ͼ 
	list<Tank> tank;			//̹�� 
	list<Shell> shell;			//�ڵ� 
	Tank t_player1;				//���1��̹�� 
	Tank t_player2;				//���2��̹�� 
	int refreshMapTime = 50;			//ˢ��ʱ���� 
	int lifeTotal_f;			//���ʣ��̹������ 
	int lifeTotal_e;			//�о��þ�̹��ʣ������	
	Prop *prop;
	bool gameOver;
	bool isDouble;
	bool gaming = false;
	bool win = false;			//�ؿ�ʤ�� 
	Point core;					//����λ�� 
	int num;					//�ؿ�	
	double OSClock;				//��������ʱ�� 
	double gameClock;			//�þ���Ϸʱ��	
	char printMap[200][200][3];	//��������Ϸ���ģ�� 
	void refreshGame();			//ˢ����Ϸ���ݣ��л��ؿ�ʱ���� 
	void printGame();			//�����Ϸ��� ,ˢ����Ϸ���� 
	int ran;					//һ���ȽϿ͹۹��������������Ϊû���ܵ�������Լ 
};
//���������f�k�c�h
//�������� 
//ȫ�ֱ����� 
int row,col;
Prt p;
int mTime = 1200;		//��ʾ����ʱ�� 
Game g;
//��ʼ����̬��Ա
int Tank::e = 0;
int Tank::max_e = 5; 
int Tank::fire_cd_e = 800;		
int Tank::fire_cd_f = 800;		//������ȴʱ�� ����λ������ 
int Tank::move_cd_e = 1000;		//�з�̹���ƶ���ȴʱ�� 
int Tank::move_cd_f = 100;		//���̹���ƶ���ȴʱ�� 
int Tank::reborn_cd = 1500; 	//̹��������ȴʱ�� 
int Shell::move_cd = 100;		//�ڵ��ƶ���ȴʱ�� 



//���������� 
inline void init();		//��ʼ�� 
void startGame();		//��ʼ��Ϸģ��
void control();			//����
void help();			//����
void ex();				//�˳� 
//����ģ�飺�з�̹�˿���ǰ�Զ����ã������������������û�����̹�� 
bool havePlayer(Point p,int checkDir);			
int getNum(int min,int max,int thisNum);		//���û�����һ��ֵ��ֻ�������ֵ��min~max֮��ʱ���Ž����������ֵ 
void setBorder();
void setColor();
void delProp();				//ɾ������ 
void dosClear(int x,int y,int n,int choose);				//������x y��Ӧ��ʼλ�ã�n�����м���ѡ�� 

//����ʵ���� 
int main(){
	init();	
	int choose = 1;
	char key;
	short x = 45+p.getMargin()*2,y = 24+p.getMargin()+p.getPadding();		//��Ӧ��������к��� 
	bool back = false;		//�������������� 
	p.setMargin(11);
	p.borP("&��ӭ�������̹�˴�ս0.1beta���԰�&~&&����ѡ��: W S�����&ȷ��ѡ��Enter    &�����ϲ�: ESC       &̹���ƶ���          \
&P1��W S A D &P2: ��������&̹�˿���          &P1: J  P2: P&&~&����ʱ�ڰ���ѡ���в鿴������ʽ\
&�밴Enter������");
	p.setMargin(2);
	while(1){		//���س����ɼ��� 
		key = getch();
		if(key==13)break;
	}		
	system("cls");
	p.borPLogo("&~&&  ��ʼ��Ϸ  &    ����    &    ����    &    �˳�    ");
	while(1){
		dosClear(x,y,4,choose);
		switch(choose){
			case 1:POSAT(x,y);break;
			case 2:POSAT(x,y+1);break;
			case 3:POSAT(x,y+2);break;
			case 4:POSAT(x,y+3);break;
		}
		cout << "->";
		if(kbhit()){		//����м��̼��� 
			key = getch();
			switch(key){
				case 'w':choose = choose==1?4:choose-1;break;
				case 's':choose = choose==4?1:choose+1;break;
				case 13:		//Enter
					switch(choose){
						case 1:startGame();break;
						case 2:control();break;
						case 3:help();break;
						case 4:ex();break;
					}	
					system("cls");
					x = 45+p.getMargin()*2,y = 24+p.getMargin()+p.getPadding();
					p.borPLogo("&~&&  ��ʼ��Ϸ  &    ����    &    ����    &    �˳�    ");
					init();
					break;
			}
			if(key<0||key>128){
				key = getch();
				switch(key){
					case 72:choose = choose==1?4:choose-1;break;
					case 80:choose = choose==4?1:choose+1;break;
				}
			}
			
		}
		ZERO;		//�����Ϊ0,0 
	}
	
	return 0;
}

inline void init(){
	//��ʼ�����������
	srand(unsigned(time(0))); 
    //���ع��
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info); 
	SetConsoleTitle("�ռ�����̹�˴�ս"); 			//����̨���� 
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rc = {0,0, 100+2+p.getMargin()*2*2, 34+p.getMargin()*2+p.getPadding()*2};  // ���ô���λ�úʹ�С
	SetConsoleWindowInfo(hOut,true ,&rc); 
	//��ȡ��ͼ 
	Map *map;
	FILE *fp;
	char tmp;
	if((fp=fopen("data/maps.txt","r"))!=NULL){
		while(!feof(fp)){	//ֻҪ�ļ���������û���꣬�ͼ�����ȡ 
			map = new Map;
			fscanf(fp,"%s%d %d %d\n",map->name,&map->y,&map->x,&map->lifeTotal_e);
			for(int i = 1;i <= map->y;i++){
				for(int j = 1;j <= map->x;j++){
					if(feof(fp))break;
					fscanf(fp,"%c",&map->map_[i][j]);
				}
				if(feof(fp))break;	
				fscanf(fp,"%c",&tmp);
			}
			g.maps.push_back(*map);			//������Ϸ����ı�׼��ͼ�� 
		}
	}
	else{
		cerr << "error!maps.txt��ʧ��!" << endl;
	}
	fclose(fp);
	//��ʼ����Ϸ����
	g.gaming = false;
	g.tank.clear();
	g.shell.clear();
	g.lifeTotal_f = 3;
	g.num = 0;
	g.lifeTotal_e = g.map.lifeTotal_e;
 	g.gameOver = false;
 	g.OSClock = 0;
}
//��Ϸ���� 
void startGame(){
	system("cls");
	int key;
	short x = 46+p.getMargin()*2,y = 25+p.getMargin()+p.getPadding();
	//ѡ��˫��
	if(!g.gaming){					//���û�����ڽ��е���Ϸ
		int choose = 1;
		bool flag = true;
		p.borPLogo("&~&&&  ������Ϸ&  ˫����Ϸ");
		while(flag){
			dosClear(x,y,2,choose);
			switch(choose){
				case 1:POSAT(x,y);break;
				case 2:POSAT(x,y+1);break;
			}
			cout << "->";
			key = getch();
			switch(key){
				case 'w':choose = choose==1?2:1;break;
				case 's':choose = choose==2?1:2;break;
				case 13:flag = false;break;
				case 27:return;
			}
			if(key<0||key>128){
				key = getch();
				switch(key){
					case 72:choose = choose==1?2:1;break;
					case 80:choose = choose==2?1:2;break;
				}
			}
			ZERO;
		}
		system("cls");
		g.isDouble = choose==2?true:false;
		g.refreshGame();
	} 
	//�����ڴ��ŵ�ͼ��һ�ݴ�Ÿùؿ���ͼԴ�ļ���һ�ݴ�������Ϸ�е�ͼ��ģ����һ�ݴ��Ҫ��ӡ�ĵ�ͼģ��
	while(!g.gameOver){
		g.ran = rand();
		//���ɵз�̹��
		Tank::createTank_e();
		//ˢ�½��� 
		g.printGame();
		//�з�̹������
		for(Tank& tmp:g.tank){
			tmp.move();
		}
		//�з�̹�˿��� 
		for(Tank& tmp:g.tank)
			tmp.fire();
		//�ڵ��ƶ�
		for(list<Shell>::iterator it = g.shell.begin();it!=g.shell.end();){
			it->move();
			if(it->isLife()==false){
				g.shell.erase(it++);
				continue; 
			}
			it++;
		} 
		if(g.prop!=NULL&&g.prop->regain())delProp();										//��ʰ���Ҳ���ʱЧ���߻�ʱЧʱ���ѹ� 
		if(g.prop!=NULL&&g.prop->isLife()==false&&g.prop->ispickup()==false)delProp();		//�����Ѿ�����û�б�ʰ��  	
				
		if(kbhit()){						//����м��̱����� 
			key = getch();
			switch(key){
				case 'a':g.t_player1.turnDir(1);g.t_player1.move();break;
				case 'w':g.t_player1.turnDir(2);g.t_player1.move();break;
				case 'd':g.t_player1.turnDir(3);g.t_player1.move();break;
				case 's':g.t_player1.turnDir(4);g.t_player1.move();break;
				case 'j':g.t_player1.fire();break;
				case 27:return;
			}
			if(key<0||key>128){
				key = getch();
				switch(key){
					case 75:g.t_player2.turnDir(1);g.t_player2.move();break;
					case 72:g.t_player2.turnDir(2);g.t_player2.move();break;
					case 77:g.t_player2.turnDir(3);g.t_player2.move();break;
					case 80:g.t_player2.turnDir(4);g.t_player2.move();break;
				}
			}
			else if(g.isDouble&&key=='p')g.t_player2.fire();
		}
		//������ϵз�̹�˶�û�˲���û�д��������������һ�� 
		if(g.win){
			g.refreshGame(); 
			g.lifeTotal_f+=3;		//�������� 
		}
	}
	Sleep(400);
	system("cls"); 
	p.borP("&&GAME  OVER&&");
	Sleep(mTime); 
	init();
}
void control(){
	system("cls");
	int choose = 1;
	int key;
	short x = 44+p.getMargin()*2,y = 24+p.getMargin()+p.getPadding();
	p.borPLogo("  ����&~&&    �߿�����  &    ��ɫ����  &  ̹���ƶ��ٶ�&  ̹�˿����ٶ�&  �ڵ������ٶ�&  �ϳ�̹������&  ���̹������");
	while(1){
		dosClear(x,y,7,choose);
		switch(choose){
			case 1:POSAT(x,y);break;
			case 2:POSAT(x,y+1);break;
			case 3:POSAT(x,y+2);break;
			case 4:POSAT(x,y+3);break;
			case 5:POSAT(x,y+4);break;
			case 6:POSAT(x,y+5);break;
			case 7:POSAT(x,y+6);break;
		} 
		cout << "->";
		if(kbhit()){		//����м��̼��� 
			key = getch();
			switch(key){
				case 'w':choose = choose==1?7:choose-1;break;
				case 's':choose = choose==7?1:choose+1;break;
				case 27:return;
				case 13:		//Enter
					system("cls");
					switch(choose){
						case 1:setBorder();break;
						case 2:setColor();break;
						case 3:p.pLine("�޸�̹���ƶ��ٶ�",60);Tank::move_cd_e = getNum(100,2000,Tank::move_cd_e);break;
						case 4:p.pLine("�޸�̹�˿����ٶ�",60);Tank::fire_cd_f = getNum(100,1800,Tank::move_cd_e);break;
						case 5:p.pLine("�޸��ڵ������ٶ�",60);Shell::move_cd = getNum(20,1200,Shell::move_cd);break;
						case 6:p.pLine("�޸��ϳ�̹���ٶ�",60);Tank::max_e = getNum(1,10,Tank::max_e);break;
						case 7:p.pLine("�޸����̹������",60);g.lifeTotal_f = getNum(1,10,g.lifeTotal_f);break;
					}	
					system("cls");
					x = 44+p.getMargin()*2,y = 24+p.getMargin()+p.getPadding();
					p.borPLogo("  ����&~&&    �߿�����  &    ��ɫ����  &  ̹���ƶ��ٶ�&  ̹�˿����ٶ�&  �ڵ������ٶ�&  �ϳ�̹������&  ���̹������");
					break;
			}
			if(key<0||key>128){
				key = getch();
				switch(key){
					case 72:choose = choose==1?7:choose-1;break;
					case 80:choose = choose==7?1:choose+1;break;
				}
			}
		}
		ZERO;
	}
	
}
void setBorder(){
	system("cls");
	int choose = 1;
	int key;
	int num;
	while(1){
		switch(choose){
			case 1:p.borP("  ����&~&&->��߾�&  �ڱ߾�");break;
			case 2:p.borP("  ����&~&&  ��߾�&->�ڱ߾�");break;
		}
		if(kbhit()){		//����м��̼��� 
			key = getch();
			switch(key){
				case 'w':choose = choose==1?2:choose-1;break;
				case 's':choose = choose==2?1:choose+1;break;
				case 27:return;
				case 13:		//Enter
					
					switch(choose){
						case 1:p.pLine("�޸���߾�",60); num = getNum(0,4,p.getMargin());p.setMargin(num);break;
						case 2:p.pLine("�޸��ڱ߾�",60); num = getNum(0,4,p.getPadding());p.setPadding(num);break;
					}	
					HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
					SMALL_RECT rc = {0,0, 100+2+p.getMargin()*2*2, 34+p.getMargin()*2+p.getPadding()*2};  // ���ô���λ�úʹ�С
					SetConsoleWindowInfo(hOut,true ,&rc); 
					system("cls");
					p.borP(" �޸ĳɹ�!");
					Sleep(mTime);
					system("cls");
					break;
			}
			if(key<0||key>128){
				key = getch();
				switch(key){
					case 72:choose = choose==1?2:choose-1;break;
					case 80:choose = choose==2?1:choose+1;break;
				}
			}
		}
		ZERO;
	}
}
void setColor(){
	system("cls");
	int choose = 1;
	int key;
	short x = 26+p.getMargin()*2,y = 3+p.getMargin()+p.getPadding();
	p.borP("ѡ����ɫ&~&  �ڵװ���&  �Ƶ׺���&  �ҵ׺���&  �ҵ�����&  �ҵ�����&   �׵׺���");
	while(1){
		dosClear(x,y,5,choose);
		switch(choose){
			case 1:POSAT(x,y);break;
			case 2:POSAT(x,y+1);break;
			case 3:POSAT(x,y+2);break;
			case 4:POSAT(x,y+3);break;
			case 5:POSAT(x,y+4);break;
			case 6:POSAT(x,y+5);break;
		}
		cout << "->";
		if(kbhit()){		//����м��̼��� 
			key = getch();
			switch(key){
				case 'w':choose = choose==1?6:choose-1;break;
				case 's':choose = choose==6?1:choose+1;break;
				case 27:return;
				case 13:		//Enter
					switch(choose){
						case 1:system("color 07");break;
						case 2:system("color e0");break;
						case 3:system("color 7c");break;
						case 4:system("color 79");break;
						case 5:system("color a0");break;
						case 6:system("color f0");break;
					}
			}
			if(key<0||key>128){
				key = getch();
				switch(key){
					case 72:choose = choose==1?5:choose-1;break;
					case 80:choose = choose==5?1:choose+1;break;
				}
			}
		}
	}
}
int getNum(int min,int max,int thisNum){
	int num;
	printf("������%d-%d֮�������(��ǰΪ:%d):\n",min,max,thisNum);
	scanf("%d",&num); 
	while(num<min||num>max){
		printf("���벻�Ϸ�,����������:\n");
		scanf("%d",&num);
	}
	system("cls");
	return num;
}
void help(){
	int key;
	system("cls");
	p.setMargin(11);
	p.borP("&����ָ��&~&&����ѡ��: W S�����&ȷ��ѡ��Enter    &�����ϲ�: ESC       &̹���ƶ���          \
&P1��W S A D &P2: ��������&̹�˿���          &P1: J  P2: P");
	p.setMargin(2);
	getch();
}
void ex(){
	system("cls");
	p.borP("�����˳������Ժ�...");
	Sleep(1200);
	system("cls");
	exit(0); 
}

void Prt::mar(){
	int i = 0;
	for(;i < margin;i++)printf("  ");
}
Tank::Tank(){
	if(camp==2)e++;
}
Tank::Tank(int x_,int y_,int camp_,int dir_) : x{x_},y{y_},camp{camp_},dir{dir_}{
	turnDir(dir_);
	if(camp==2)e++;	
}
Tank::Tank(const Tank& t){
	x = t.x;y = t.y;camp = t.camp;dir = t.dir;
	turnDir(dir);
	if(camp==2)e++;
}
Tank::~Tank(){
	if(camp==2)e--;
}
void Tank::move(){
	if(clock()-moveLastTime>(camp==1?move_cd_f:move_cd_e)){
		if(camp==2)turnDir();		//����ǵз�̹�ˣ�������Ҫ�Զ�ת�� 
		Point change;
		bool isNull = true;		//��������ǰ��û���ϰ��� 
		switch(dir){
			case 1:change.x = x-1;change.y = y;break;
			case 2:change.x = x;change.y = y-1;break;
			case 3:change.x = x+1;change.y = y;break;
			case 4:change.x = x;change.y = y+1;break;
		}
		if(g.map.map_[change.y][change.x]=='0'){			//��ͼ��������ǿյ� 
			for(Tank tmp:g.tank)
				if(tmp.x==change.x&&tmp.y==change.y)isNull = false;
			if(g.t_player1.x==change.x&&g.t_player1.y==change.y)isNull = false;
			if(g.isDouble&&g.t_player2.x==change.x&&g.t_player2.y==change.y)isNull = false;
			//�����һ��ȷʵΪ����û̹�ˣ����ƶ�������� 
			if(isNull){
				int random = rand()%100;
				if(random<50&&g.prop==NULL&&camp==2){				//�з�̹��ÿ���ƶ���20%�ĸ������ƶ�ǰ��λ������һ������ 
					//ֻ��̹�˵���Χ��������ǰһ�����ɵ��� 
					//int px = x+random%3-1,py = y+random%3-1;
					//if(px>0&&px<=g.map.x&&py>0&&py<=g.map.y&&px!=change.x&&py!=change.y&&g.map.map_[py][px]=='0')
						g.prop = new Prop({g.ran%3+1,x,y});
				}
				strcpy(g.printMap[y][x],"  ");
				x = change.x;
				y = change.y;
				//�ж�����Ƿ�ʰȡ���� 
				if(g.prop&&camp==1&&x==g.prop->getPoint().x&&y==g.prop->getPoint().y){		
					g.prop->show();
					g.prop->pup();
					strcpy(g.printMap[g.prop->getPoint().y][g.prop->getPoint().x],"  ");
				} 
			} 
		}
		moveLastTime = clock();	
	}
}  
void Tank::fire(){
	if(clock()-fireLastTime>(camp==1?fire_cd_f:fire_cd_e)){
		if(camp==1){		//��������̹�ˣ���ֱ��ִ�п��� 
			g.shell.push_back({x,y,camp,dir});
		}
		else{
			int random = (rand()+1)%100;			//1~100���ֵ
			if(random<50){				//ֻҪ���Թ����𣬲�����û�гɹ������ٴν��뿪��cd 
				g.shell.push_back({x,y,camp,dir});
			}
			else{
				for(int tmpDir = 1;tmpDir <= 4;tmpDir++){
				//�������з�̹����tmpDir������������굽�����ֱ���������̹�ˣ��͸ı䷽��Ȼ�󿪻� 
					if(havePlayer(getPoint(),tmpDir)){			
						turnDir(tmpDir);
						g.shell.push_back({x,y,camp,dir});
					}
				} 
			}
				 
		}
		fireLastTime = clock();
	}
	
} 
void Tank::turnDir(int dir_){		//�������������޸�̹����ʽ 
	//�f�k�c�h�ȩة���
	dir = dir_;
	switch(dir){
		case 1:strcpy(shape,camp==1?"��":"��");break;
		case 2:strcpy(shape,camp==1?"��":"��");break;
		case 3:strcpy(shape,camp==1?"��":"��");break;
		case 4:strcpy(shape,camp==1?"��":"��");break;
	}
} 
void Tank::turnDir(){
	bool forward = true;		//�Ƿ����ǰ�� 
	Point p = this->getPoint();		//��������ĵ�һ�����꣬�����ж��Ƿ����ǰ�� 
	int changeDir = 1;
	int backDir = (this->dir+2)%4;		//�󱳷��� �������� ,ֻ���򲻵��Ѳŷ����� 
	switch(this->dir){
	 	case 1:p.x-=1;break;
		case 2:p.y-=1;break;
		case 3:p.x+=1;break;
		case 4:p.y+=1;break; 
	}
	if(g.map.map_[p.y][p.x]=='2')		//���̹����ǰ�����飬����ǰ��
	 	 forward = false;
	for(Tank& tmp:g.tank)				//���̹����ǰ��̹�ˣ�Ҳ����ǰ�� 
		if(tmp.getPoint().y==p.y&&tmp.getPoint().x==p.x){forward = false;break;}
	//��ʼ�ж���һ�����ı�
	int random = (rand()+1)%100;
	if(forward&&random<=55){			//�������ǰ������ô60%�ĸ��ʼ���ֱ��
		changeDir = dir;
	}
	else{
		vector<int> okDir;			//������ߵķ��� 
		if(g.map.map_[y][x-1]!='2'&&backDir!=1&&dir!=1)okDir.push_back(1);
		if(g.map.map_[y-1][x]!='2'&&backDir!=2&&dir!=2)okDir.push_back(2);
		if(g.map.map_[y][x+1]!='2'&&backDir!=3&&dir!=3)okDir.push_back(3);
		if(g.map.map_[y+1][x]!='2'&&backDir!=4&&dir!=4)okDir.push_back(4);
		if(okDir.size()>0){
			int base = 100.0f/okDir.size();		//��ȡÿ�����򱻳��еĸ���
			int idx = 1;
			for(int dir:okDir){
				if(random>(idx-1)*base&&random<=idx*base)		//�������,��ô�͸�Ϊ������� 
					changeDir = dir; 
				idx++; 
			} 
		}
		else
			changeDir = backDir;
	}
	turnDir(changeDir);
} 
void Tank::createTank_e(){
	if(Tank::e==0&&g.lifeTotal_e==0){g.win = true;return;}
	static clock_t rebornLastTime = 0;
	static const int reborn_cd = 1500;
	if(clock()-rebornLastTime>reborn_cd){
		int random;
		bool create_left,create_right,create_f;
		//������ϵĵз�̹����������5����������һ�� ,����������ɵ㶼�������ɣ����ʱ��̶Ⱦ��Ȳ����� 
		if(Tank::e < Tank::max_e){		 
			random = rand()%2;		//�������߻��ұ�����̹�� 
			create_left = create_right = true;
			for(Tank tmp:g.tank)
				if(tmp.x == 1&&tmp.y == 1)create_left = false;
				else if(tmp.x == g.map.x&&tmp.y == 1)create_right = false;
			if(g.lifeTotal_e>0)
				if(random == 0 && create_left){
					g.tank.push_back({1,1,2,4});
					g.lifeTotal_e--;
				}	
				else if(random == 1 && create_right){
					g.tank.push_back({g.map.x,1,2,4});
					g.lifeTotal_e--;
				}	
		} 
		rebornLastTime = clock();
	}
	
	 
}
void Tank::createTank_f(){
	//�������̹��
	if(g.t_player1.isLife()==false&&g.lifeTotal_f>0){
			g.t_player1 = {g.core.x-2,g.core.y,1,2};
			g.lifeTotal_f--;
	}
	if(g.isDouble&&g.t_player2.isLife()==false&&g.lifeTotal_f>0){
			g.t_player2 = {g.core.x+2,g.core.y,1,2};
			g.lifeTotal_f--;
	}
	if(g.lifeTotal_f==0){			//�����Ӫû�д������� 
		if(g.t_player1.isLife()==false)
			if(!g.isDouble)g.gameOver = true;			//������1�������Ҳ���˫�ˣ�����Ϸ����
			else if(g.isDouble&&g.t_player2.isLife()==false)g.gameOver = true;			//�����˫�������2��̹��Ҳ������������Ϸ���� 
	}
} 
void Tank::print(){
	strcpy(g.printMap[y][x],shape);
}
void Tank::kill(){
	strcpy(g.printMap[y][x],"  ");
	life = false;
	x = 0;y = 0;
}
Point Tank::getPoint(){
	Point ret;
	ret.x = x;ret.y = y;
	return ret;
};

void Shell::move(){				//�ڵ����ƶ� 
	if(clock()-_time>move_cd){
		strcpy(g.printMap[y][x],"  ");
		Point changeP = this->getPoint();
		switch(dir){			//�ڵ���ǰ��һ�����������������ж� 
			case 1:changeP.x-=1;break;
			case 2:changeP.y-=1;break;
			case 3:changeP.x+=1;break;
			case 4:changeP.y+=1;break;
		}
		switch(g.map.map_[changeP.y][changeP.x]){
			case '1':g.map.map_[changeP.y][changeP.x] = '0';this->kill();break;		//�򵽵���ש�飬��ôש����ʧ���ڵ����� 
			case '2':this->kill();break;						//�򵽵������飬�ڵ����� 
			case '9':this->kill();g.gameOver = true;			//����򵽵��Ǻ��ģ���ô��Ϸ���� 
		}
		//�����û�������з�̹�� 
		for(list<Tank>::iterator tmp = g.tank.begin();tmp!=g.tank.end();){
			if(tmp->getPoint().x==changeP.x&&tmp->getPoint().y==changeP.y){
				this->kill();
				if(camp==1)	{	//�������ҵ��ڵ�����̹������
					tmp->kill();
					g.tank.erase(tmp++);
					continue; 
				}
			} 
			tmp++;
		}
		//�����û�����������ڵ� 
		for(list<Shell>::iterator tmp = g.shell.begin();tmp!=g.shell.end();){
			if(tmp->getPoint().y==changeP.y&&tmp->getPoint().x==changeP.x){
				this->kill();
				strcpy(g.printMap[tmp->getPoint().y][tmp->getPoint().x],"  ");
				g.shell.erase(tmp++);
				continue;
			}
			tmp++;
		}	
		if(g.t_player1.getPoint().x==changeP.x&&g.t_player1.getPoint().y==changeP.y){
			this->kill();
			if(camp==2)g.t_player1.kill();				//����ǵз��ڵ��������1̹������ 
		}
		if(g.isDouble&&g.t_player2.getPoint().x==changeP.x&&g.t_player2.getPoint().y==changeP.y){
			this->kill();
			if(camp==2)g.t_player2.kill();
		}
		if(this->isLife()){x = changeP.x;y = changeP.y;}
		_time = clock();
	}
	Tank::createTank_f();
}
void Shell::print(){
	strcpy(g.printMap[y][x],shape);
}
Shell::Shell(){
	strcpy(shape,"+ ");
}
Shell::Shell(int x_,int y_,int camp_,int dir_) : x{x_},y{y_},camp{camp_},dir{dir_}{
	strcpy(shape,"+ ");
}
Point Shell::getPoint(){
	Point p;
	p.x = x;p.y = y;
	return p;
}
void Game::refreshGame(){			//��һ����Ϸ���л��ؿ�ʱ���ô˺��� 
	g.map = g.maps[++g.num-1];		//���ݹؿ��л���Ϸ��ǰ��ͼ
	g.tmpMap = g.map;
	g.gaming= true;
	g.win = false;
	if(g.prop!=NULL)delProp();
	system("cls");
	p.borP(g.map.name);				//��ӡ�ؿ��� 
	Sleep(mTime);
	system("cls");
	//��ȡ����λ��,�Ҹ���ΧһȦ����Χǽ 
	for(int i = 0;i <= g.map.y+1;i++)
		for(int j = 0;j <= g.map.x+1;j++)
			if(i==0||i==g.map.y+1||j==0||j==g.map.x+1)		//��Χǽ 
				g.map.map_[i][j] = '2';
			else if(g.map.map_[i][j]=='9'){
				 g.core.x = j;
				 g.core.y = i;
			}
	//��ʼ���з�̹������
	g.lifeTotal_e = g.map.lifeTotal_e; 
	//�������̹��
	g.t_player1 = {g.core.x-2,g.core.y,1,2};g.lifeTotal_f--;
	if(isDouble) {
		g.t_player2 = {g.core.x+2,g.core.y,1,2};g.lifeTotal_f--;
	}
	//���øþ���Ϸʱ�� 
	g.gameClock = clock();
	//��ʼ����Ϸ���ģ��
	for(int i = 0;i <= g.map.y+1;i++)
		for(int j = 0;j <= g.map.x+1;j++)
			switch(g.map.map_[i][j]){
				case '0':strcpy(g.printMap[i][j],"  ");break;
				case '1':strcpy(g.printMap[i][j],"��");break;
				case '2':strcpy(g.printMap[i][j],"��");break;
				case '9':strcpy(g.printMap[i][j],"��");break;
			}
}
void Game::printGame(){
	static int lastF = 0;
	static int lastE = 0;
	static clock_t _time = 0;
	if(clock()-_time>g.refreshMapTime){
		Point tmpPoint;
		ZERO;							//���ù��λ��Ϊ0,0 
		//�ȸ��µ�ͼ 
		for(int i = 1;i <= g.map.y;i++)
			for(int j = 1;j <= g.map.x;j++)
				if(g.map.map_[i][j]!=g.tmpMap.map_[i][j])		//����ϴ�ˢ��ʱ��ͼ����������β�ͬ�����޸�
					strcpy(g.printMap[i][j],"  ");			//��Ŀǰ�Ĺ��򣬷������ֻ�п�����ש���Ϊ��
		g.tmpMap = g.map;									//����󣬸�����ʱ��ͼ 
		//Ȼ����ڵ��Ž���ͼ
		for(Shell tmp:g.shell)
			tmp.print();	
		//�ٰ�̹�˷Ž���ͼ
		for(Tank tmp:g.tank) 
			tmp.print();
		if(g.t_player1.isLife())g.t_player1.print();
		if(isDouble&&g.t_player2.isLife())g.t_player2.print();
		//�ѵ��߷����ͼ
		if(g.prop!=NULL&&g.prop->ispickup()==false){g.prop->print();}
		//��ӡ��Ϸ���
		for(int i = 0;i <= g.map.y+1;i++){
			for(int j = 0;j <= g.map.x+1;j++)
				cout << g.printMap[i][j];
			cout << endl;
		} 
		//����ӡ��Ϸ����
		p.pLine(g.map.name,g.map.x*2+4);
		printf("��Ϸʱ��:%.1fs\n",(clock()-g.gameClock)/CLOCKS_PER_SEC);
		printf("���ʣ��̹��:");
		for(int i = 0;i < g.lifeTotal_f;i++)
			cout << "��";
		for(int i = g.lifeTotal_f;i < lastF;i++)
			cout << "  ";
		cout << endl;
		printf("�з�ʣ��̹��:");
		for(int i = 0;i < g.lifeTotal_e;i++)
			cout << "��";
		for(int i = g.lifeTotal_e;i < lastE;i++)
			cout << "  ";
		cout << endl;
		lastF = g.lifeTotal_f;
		lastE = g.lifeTotal_e;
		_time = clock();
	}
}
bool havePlayer(Point p,int checkDir){
	int changeX = 0,changeY = 0;
	switch(checkDir){
		case 1:changeX = -1;break;
		case 2:changeY = -1;break;
		case 3:changeX = 1;break;
		case 4:changeY = 1;break;
	}
	bool havePlayer = false;
	while(g.map.map_[p.y][p.x]!='2'){			//ֻҪû�������鷽�飬�ͼ������� 
		if(g.t_player1.getPoint().y==p.y&&g.t_player1.getPoint().x==p.x){
			havePlayer = true;break;
		}
		else if(g.isDouble&&g.t_player2.getPoint().y==p.y&&g.t_player2.getPoint().x==p.x){
			havePlayer = true;break;
		}
		p.x+=changeX;p.y+=changeY;
	}
	return havePlayer;
}
//������� 
void Prt::pLine(char str[],int len_){
	int i;
	int s_len = 0;
	for(i = 0;str[i];i++)s_len++;
	for(i = 0;i < (len_-s_len)/2;i++)printf("*");
	printf("%s",str);
	for(i = 0;i < (len_-s_len)/2;i++)printf("*");
	if((len_-s_len)%2)printf("*");		//����м����ݲ���˫������ĩβ�Ͳ�һ��*�� 
	printf("\n");
} 
//���һ�� 
void Prt::print(char str[]){
	int i,s_len = 0;
	for(i = 0;str[i];i++)s_len++;
	mar();		//ÿ���������߾� 
	printf("��");
	for(i = 0;i < (Len-s_len)/2;i++)printf(" ");
	printf("%s",str);
	s_len = s_len%2?s_len-1:s_len;
	for(i = 0;i < (Len-s_len)/2;i++)printf(" ");
	printf("��\n");
}

//���Զ��ױ߿�ĸ�ʽ���,�������Ҫ�������,����&������ 
void Prt::borP(char str[]){
	int i,s_len = 0;
	int begin = 0;		//�Ӵ���str���ĸ�Ԫ���±꿪ʼ���� 
	bool haveLine = false;
	char substr[300];
	substr[0] = '\0';		//��ʼ��
	for(i = 0;i < margin;i++)printf("\n");
	mar();printf("��");for(i = 0;i < Len/win;i++)printf("��");printf("��\n");
	for(i = 0;i < padding;i++)print("");
	for(i = 0;1;i++){
		if(str[i]=='&'){substr[(haveLine?Len*2/win:i-begin)] = '\0';begin = i+1;print(substr);substr[0] = '\0';haveLine = false;}
		else if(str[i]=='~'){
			for(int j = 0;j < Len;j++){
				strcat(substr,"��");
			}
			haveLine = true;
		}
		else if(str[i] == '\0'){substr[i-begin] = '\0';print(substr);break;}
		else 
			substr[i-begin] = str[i]; 
		
	}
	for(i = 0;i < padding;i++)print("");
	mar();printf("��");for(i = 0;i < Len/win;i++)printf("��");printf("��\n");
	for(i = 0;i < margin;i++)printf("\n");
	this->Len = 60;
}
void Prt::borPLogo(char str[]){
	this->Len = 100;
	char logoStr[2000] = {"\
������������        ������        ������������  ������������  ����          ������������&\
����      ����    ����  ����          ����          ����      ����          ����        &\
����      ����  ����      ����        ����          ����      ����          ����        &\
������������    ����      ����        ����          ����      ����          ����������  &\
����      ����  ��������������        ����          ����      ����          ����        &\
����      ����  ����      ����        ����          ����      ����          ����        &\
������������    ����      ����        ����          ����      ������������  ������������&\
&&\
    ��������      ������������    ������������    ����    ����&\
  ����    ����        ����            ����        ����    ����&\
����                  ����            ����        ����    ����&\
����                  ����            ����          ��������  &\
����                  ����            ����            ����    &\
  ����    ����        ����            ����            ����    &\
    ��������      ������������        ����            ����    &\
&&����̹�˴�ս0.1beta���԰�&&"};
	strcat(logoStr,str);
	borP(logoStr);
}
Prop::Prop(int id_,int x_,int y_){
	id = id_;x = x_;y = y_;
	lifeTime = clock();
	tmp = 0;
	max_propTime = 0; 
	switch(id){		
		case 1:strcpy(shape,"��");break;
		case 2:strcpy(shape,"��");break;
		case 3:strcpy(shape,"��"); break;
	}
} 
void Prop::show(){
	propTime = clock();
	switch(id){
		case 1:
			for(list<Tank>::iterator it = g.tank.begin();it!=g.tank.end();){			//������г��ϵз�̹��
				strcpy(g.printMap[it->getPoint().y][it->getPoint().x] ,"  ");
				g.tank.erase(it++);
			}
			break;
		case 2:
			tmp = Tank::fire_cd_f;											//�������,�������� 
			max_propTime = 5000;Tank::fire_cd_f = 100;break;
		case 3:
			g.lifeTotal_f++;break;											//�����Ӫ��һ���� 
	}
} 
void Prop::print(){
	strcpy(g.printMap[y][x],shape);
} 
Point Prop::getPoint(){
	Point p;
	p.x = x;p.y = y;
	return p;
};
bool Prop::regain(){				//�Ƿ����ɾ���� 
	bool ret = false;
	if(ispickup()&&max_propTime>0){			//�����ʰȡ������ʱЧ���� 
		if(clock()-propTime>max_propTime){			//����ʱЧ�ѵ� 
			ret = true;
		}
		else ret = false;
	}
	else if(ispickup()&&max_propTime==0)ret = true;
	return ret;
}
void delProp(){
	if(g.prop->tmp!=0)
		switch(g.prop->id){
			case 2:Tank::fire_cd_f = g.prop->tmp;break;
		}
	strcpy(g.printMap[g.prop->getPoint().y][g.prop->getPoint().x],"  ");
	delete g.prop;g.prop = NULL;
}
void dosClear(int x,int y,int n,int choose){
 	for(int i = 0;i < n;i++){
 		if(i+1==choose)continue;
 		POSAT(x,y+i);
 		cout << "  ";
 	}	
 } 
