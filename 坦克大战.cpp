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
	int Len = 60;		//边框宽
	int padding = 2;	//上下内边距
	int margin = 2;		//外边距
	int win = 1;		//如果是win10，就改为1，如果是win7，就改为2 
public:
	void setPadding(int padding_){padding = padding_;}
	void setMargin(int margin_){margin = margin_;}
	int getPadding(){return padding;}
	int getMargin(){return margin;}
	void mar();						//输出左右外边距 
	void pLine(char str[],int len_);			//输出标题 
	void print(char str[]);			//输出一行 
	void borP(char str[]);			//按自动套边框的格式输出,如果框内要输出多行,就用&符连接,如果要输出分割线，就输入~符替代 
	void borPLogo(char str[]);		//带logo的套边框格式输出 
};
struct Map{			//地图 
	int x;		//地图宽，不包括边墙 
	int y;		//地图长 
	char name[30];
	char  map_[200][200];		//地图，下标从1开始 
	int lifeTotal_e;			//敌方坦克数量 
};
class Tank{			//坦克类 
private:
	int x,y;			//坐标 
	int camp;			//所属阵营 ,1为友军，2为敌军 
	int dir;			//脸朝方向,1 2 3 4 对应 左上右下 
	bool life = true;	//是否存活 
	char shape[3];
	clock_t moveLastTime = 0;
	clock_t fireLastTime = 0;
public:
	static int fire_cd_e;
	static int fire_cd_f;		//开炮冷却时间 ，单位：毫秒 
	static int move_cd_e;
	static int move_cd_f;
	static int reborn_cd;
	static int e;			//场上敌方坦克总数
	static int max_e;		//最多上场敌方坦克数量 
	Tank();
	Tank(int x_,int y_,int camp_,int dir_);
	Tank(const Tank& t);
	~Tank();
	void move();	//移动  
	void fire();	//开炮 
	bool isLife(){return life;}	
	void kill();						//杀死该坦克 ，当该坦克死亡时 
	void turnDir(int dir);	
	void turnDir();						//无参一般为敌方坦克自动调用 
	static void createTank_e();			//生成敌方坦克 
	static void createTank_f();			//生成玩家坦克 
	void print();
	Point getPoint();
};
class Shell{		//炮弹类 
private:
	int x,y;
	int camp;
	int dir;		//移动方向 
	int life = true;
	char shape[3];	
	clock_t _time = 0;	
public:
	static int move_cd;		//移动冷却时间 ，单位：毫秒 
	Shell();
	Shell(int x_,int y_,int camp_,int dir_);
	void move();	//移动 
	bool isLife(){return life;}
	void kill(){life = false;}
	void print();
	Point getPoint();
};
class Prop{						//道具类
private: 
	int id;						//指明是哪个道具
	int x,y;					//坐标
	char shape[3];				//在地图上的显示
	clock_t propTime = 0;		//道具效果当前发动时间 
	clock_t max_propTime;		//道具效果持续时间 
	clock_t maxTime = 8000;		//道具存活时间 
	clock_t lifeTime;			//道具当前存活时间 
	bool pickup = false;
	int tmp;					//存放道具改变的属性的原来值，时间一过再变回来 
public:
	friend void delProp();
	Prop(int id_,int x_,int y_);
	bool ispickup(){return pickup;}
	void pup(){pickup = true;}
	void show();				//道具发动效果,拾取时调用 
	bool isLife(){return clock()-lifeTime<maxTime;}
	void print();				//打印到地图上 
	Point getPoint();
	bool regain();				//让时限已到的道具效果失效 ,返回是否可以删了 
};
struct Game{			//游戏对象 
	vector<Map> maps;			//标准地图库
	Map map;					//该局游戏地图 
	Map tmpMap;					//每一次刷新时输出的地图 
	list<Tank> tank;			//坦克 
	list<Shell> shell;			//炮弹 
	Tank t_player1;				//玩家1的坦克 
	Tank t_player2;				//玩家2的坦克 
	int refreshMapTime = 50;			//刷新时间间隔 
	int lifeTotal_f;			//玩家剩余坦克数量 
	int lifeTotal_e;			//敌军该局坦克剩余数量	
	Prop *prop;
	bool gameOver;
	bool isDouble;
	bool gaming = false;
	bool win = false;			//关卡胜利 
	Point core;					//核心位置 
	int num;					//关卡	
	double OSClock;				//程序运行时间 
	double gameClock;			//该局游戏时间	
	char printMap[200][200][3];	//存放输出游戏面板模样 
	void refreshGame();			//刷新游戏数据，切换关卡时适用 
	void printGame();			//输出游戏面板 ,刷新游戏调用 
	int ran;					//一个比较客观公正的随机数，因为没有受到条件制约 
};
//←↑→↓╢╧╟╤
//█▓■∞ 
//全局变量区 
int row,col;
Prt p;
int mTime = 1200;		//提示滞留时间 
Game g;
//初始化静态成员
int Tank::e = 0;
int Tank::max_e = 5; 
int Tank::fire_cd_e = 800;		
int Tank::fire_cd_f = 800;		//开炮冷却时间 ，单位：毫秒 
int Tank::move_cd_e = 1000;		//敌方坦克移动冷却时间 
int Tank::move_cd_f = 100;		//玩家坦克移动冷却时间 
int Tank::reborn_cd = 1500; 	//坦克重生冷却时间 
int Shell::move_cd = 100;		//炮弹移动冷却时间 



//函数申明区 
inline void init();		//初始化 
void startGame();		//开始游戏模块
void control();			//设置
void help();			//帮助
void ex();				//退出 
//功能模块：敌方坦克开火前自动调用，查找在这个方向上有没有玩家坦克 
bool havePlayer(Point p,int checkDir);			
int getNum(int min,int max,int thisNum);		//让用户输入一个值，只有输入的值在min~max之间时，才结束返回这个值 
void setBorder();
void setColor();
void delProp();				//删除道具 
void dosClear(int x,int y,int n,int choose);				//清屏，x y对应起始位置，n代表有几个选项 

//函数实现区 
int main(){
	init();	
	int choose = 1;
	char key;
	short x = 45+p.getMargin()*2,y = 24+p.getMargin()+p.getPadding();		//对应窗口里的列和行 
	bool back = false;		//从其他函数回来 
	p.setMargin(11);
	p.borP("&欢迎试玩抽象坦克大战0.1beta测试版&~&&上下选择: W S或↑↓&确认选择：Enter    &返回上层: ESC       &坦克移动：          \
&P1：W S A D &P2: ←↑→↓&坦克开火：          &P1: J  P2: P&&~&可随时在帮助选项中查看操作方式\
&请按Enter键继续");
	p.setMargin(2);
	while(1){		//按回车方可继续 
		key = getch();
		if(key==13)break;
	}		
	system("cls");
	p.borPLogo("&~&&  开始游戏  &    设置    &    帮助    &    退出    ");
	while(1){
		dosClear(x,y,4,choose);
		switch(choose){
			case 1:POSAT(x,y);break;
			case 2:POSAT(x,y+1);break;
			case 3:POSAT(x,y+2);break;
			case 4:POSAT(x,y+3);break;
		}
		cout << "->";
		if(kbhit()){		//如果有键盘键入 
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
					p.borPLogo("&~&&  开始游戏  &    设置    &    帮助    &    退出    ");
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
		ZERO;		//光标置为0,0 
	}
	
	return 0;
}

inline void init(){
	//初始化随机数种子
	srand(unsigned(time(0))); 
    //隐藏光标
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info); 
	SetConsoleTitle("终极抽象坦克大战"); 			//控制台标题 
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rc = {0,0, 100+2+p.getMargin()*2*2, 34+p.getMargin()*2+p.getPadding()*2};  // 重置窗口位置和大小
	SetConsoleWindowInfo(hOut,true ,&rc); 
	//读取地图 
	Map *map;
	FILE *fp;
	char tmp;
	if((fp=fopen("data/maps.txt","r"))!=NULL){
		while(!feof(fp)){	//只要文件还有内容没读完，就继续读取 
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
			g.maps.push_back(*map);			//导入游戏对象的标准地图库 
		}
	}
	else{
		cerr << "error!maps.txt打开失败!" << endl;
	}
	fclose(fp);
	//初始化游戏对象
	g.gaming = false;
	g.tank.clear();
	g.shell.clear();
	g.lifeTotal_f = 3;
	g.num = 0;
	g.lifeTotal_e = g.map.lifeTotal_e;
 	g.gameOver = false;
 	g.OSClock = 0;
}
//游戏流程 
void startGame(){
	system("cls");
	int key;
	short x = 46+p.getMargin()*2,y = 25+p.getMargin()+p.getPadding();
	//选择单双人
	if(!g.gaming){					//如果没有正在进行的游戏
		int choose = 1;
		bool flag = true;
		p.borPLogo("&~&&&  单人游戏&  双人游戏");
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
	//三份内存存放地图，一份存放该关卡地图源文件，一份存放这局游戏中地图的模样，一份存放要打印的地图模样
	while(!g.gameOver){
		g.ran = rand();
		//生成敌方坦克
		Tank::createTank_e();
		//刷新界面 
		g.printGame();
		//敌方坦克行走
		for(Tank& tmp:g.tank){
			tmp.move();
		}
		//敌方坦克开火 
		for(Tank& tmp:g.tank)
			tmp.fire();
		//炮弹移动
		for(list<Shell>::iterator it = g.shell.begin();it!=g.shell.end();){
			it->move();
			if(it->isLife()==false){
				g.shell.erase(it++);
				continue; 
			}
			it++;
		} 
		if(g.prop!=NULL&&g.prop->regain())delProp();										//被拾起且不是时效道具或时效时间已过 
		if(g.prop!=NULL&&g.prop->isLife()==false&&g.prop->ispickup()==false)delProp();		//寿命已尽并且没有被拾起  	
				
		if(kbhit()){						//如果有键盘被按下 
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
		//如果场上敌方坦克都没了并且没有储备命，则进入下一关 
		if(g.win){
			g.refreshGame(); 
			g.lifeTotal_f+=3;		//加三条命 
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
	p.borPLogo("  设置&~&&    边框属性  &    颜色属性  &  坦克移动速度&  坦克开火速度&  炮弹弹道速度&  上场坦克数量&  玩家坦克数量");
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
		if(kbhit()){		//如果有键盘键入 
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
						case 3:p.pLine("修改坦克移动速度",60);Tank::move_cd_e = getNum(100,2000,Tank::move_cd_e);break;
						case 4:p.pLine("修改坦克开火速度",60);Tank::fire_cd_f = getNum(100,1800,Tank::move_cd_e);break;
						case 5:p.pLine("修改炮弹弹道速度",60);Shell::move_cd = getNum(20,1200,Shell::move_cd);break;
						case 6:p.pLine("修改上场坦克速度",60);Tank::max_e = getNum(1,10,Tank::max_e);break;
						case 7:p.pLine("修改玩家坦克数量",60);g.lifeTotal_f = getNum(1,10,g.lifeTotal_f);break;
					}	
					system("cls");
					x = 44+p.getMargin()*2,y = 24+p.getMargin()+p.getPadding();
					p.borPLogo("  设置&~&&    边框属性  &    颜色属性  &  坦克移动速度&  坦克开火速度&  炮弹弹道速度&  上场坦克数量&  玩家坦克数量");
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
			case 1:p.borP("  设置&~&&->外边距&  内边距");break;
			case 2:p.borP("  设置&~&&  外边距&->内边距");break;
		}
		if(kbhit()){		//如果有键盘键入 
			key = getch();
			switch(key){
				case 'w':choose = choose==1?2:choose-1;break;
				case 's':choose = choose==2?1:choose+1;break;
				case 27:return;
				case 13:		//Enter
					
					switch(choose){
						case 1:p.pLine("修改外边距",60); num = getNum(0,4,p.getMargin());p.setMargin(num);break;
						case 2:p.pLine("修改内边距",60); num = getNum(0,4,p.getPadding());p.setPadding(num);break;
					}	
					HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
					SMALL_RECT rc = {0,0, 100+2+p.getMargin()*2*2, 34+p.getMargin()*2+p.getPadding()*2};  // 重置窗口位置和大小
					SetConsoleWindowInfo(hOut,true ,&rc); 
					system("cls");
					p.borP(" 修改成功!");
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
	p.borP("选择颜色&~&  黑底白字&  黄底黑字&  灰底红字&  灰底蓝字&  灰底绿字&   白底黑字");
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
		if(kbhit()){		//如果有键盘键入 
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
	printf("请输入%d-%d之间的整数(当前为:%d):\n",min,max,thisNum);
	scanf("%d",&num); 
	while(num<min||num>max){
		printf("输入不合法,请重新输入:\n");
		scanf("%d",&num);
	}
	system("cls");
	return num;
}
void help(){
	int key;
	system("cls");
	p.setMargin(11);
	p.borP("&操作指南&~&&上下选择: W S或↑↓&确认选择：Enter    &返回上层: ESC       &坦克移动：          \
&P1：W S A D &P2: ←↑→↓&坦克开火：          &P1: J  P2: P");
	p.setMargin(2);
	getch();
}
void ex(){
	system("cls");
	p.borP("正在退出，请稍后...");
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
		if(camp==2)turnDir();		//如果是敌方坦克，则先需要自动转向 
		Point change;
		bool isNull = true;		//脸朝方向前面没有障碍物 
		switch(dir){
			case 1:change.x = x-1;change.y = y;break;
			case 2:change.x = x;change.y = y-1;break;
			case 3:change.x = x+1;change.y = y;break;
			case 4:change.x = x;change.y = y+1;break;
		}
		if(g.map.map_[change.y][change.x]=='0'){			//地图上这个点是空地 
			for(Tank tmp:g.tank)
				if(tmp.x==change.x&&tmp.y==change.y)isNull = false;
			if(g.t_player1.x==change.x&&g.t_player1.y==change.y)isNull = false;
			if(g.isDouble&&g.t_player2.x==change.x&&g.t_player2.y==change.y)isNull = false;
			//如果这一格确实为空且没坦克，就移动到这个点 
			if(isNull){
				int random = rand()%100;
				if(random<50&&g.prop==NULL&&camp==2){				//敌方坦克每次移动，20%的概率在移动前的位置生成一个道具 
					//只在坦克的周围（除了脸前一格）生成道具 
					//int px = x+random%3-1,py = y+random%3-1;
					//if(px>0&&px<=g.map.x&&py>0&&py<=g.map.y&&px!=change.x&&py!=change.y&&g.map.map_[py][px]=='0')
						g.prop = new Prop({g.ran%3+1,x,y});
				}
				strcpy(g.printMap[y][x],"  ");
				x = change.x;
				y = change.y;
				//判断玩家是否拾取道具 
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
		if(camp==1){		//如果是玩家坦克，则直接执行开火 
			g.shell.push_back({x,y,camp,dir});
		}
		else{
			int random = (rand()+1)%100;			//1~100随机值
			if(random<50){				//只要尝试过开火，不管有没有成功，都再次进入开炮cd 
				g.shell.push_back({x,y,camp,dir});
			}
			else{
				for(int tmpDir = 1;tmpDir <= 4;tmpDir++){
				//如果这个敌方坦克在tmpDir方向从自身坐标到铁块的直线中有玩家坦克，就改变方向，然后开火 
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
void Tank::turnDir(int dir_){		//根据脸朝方向修改坦克样式 
	//╢╧╟╤┤┴├┬
	dir = dir_;
	switch(dir){
		case 1:strcpy(shape,camp==1?"←":"←");break;
		case 2:strcpy(shape,camp==1?"↑":"↑");break;
		case 3:strcpy(shape,camp==1?"→":"→");break;
		case 4:strcpy(shape,camp==1?"↓":"↓");break;
	}
} 
void Tank::turnDir(){
	bool forward = true;		//是否可以前进 
	Point p = this->getPoint();		//脸朝方向的第一格坐标，用于判断是否可以前进 
	int changeDir = 1;
	int backDir = (this->dir+2)%4;		//后背方向 ，反方向 ,只有万不得已才返回走 
	switch(this->dir){
	 	case 1:p.x-=1;break;
		case 2:p.y-=1;break;
		case 3:p.x+=1;break;
		case 4:p.y+=1;break; 
	}
	if(g.map.map_[p.y][p.x]=='2')		//如果坦克脸前是铁块，则不能前进
	 	 forward = false;
	for(Tank& tmp:g.tank)				//如果坦克面前有坦克，也不能前进 
		if(tmp.getPoint().y==p.y&&tmp.getPoint().x==p.x){forward = false;break;}
	//开始判断下一步走哪边
	int random = (rand()+1)%100;
	if(forward&&random<=55){			//如果可以前进，那么60%的概率继续直走
		changeDir = dir;
	}
	else{
		vector<int> okDir;			//存放能走的方向 
		if(g.map.map_[y][x-1]!='2'&&backDir!=1&&dir!=1)okDir.push_back(1);
		if(g.map.map_[y-1][x]!='2'&&backDir!=2&&dir!=2)okDir.push_back(2);
		if(g.map.map_[y][x+1]!='2'&&backDir!=3&&dir!=3)okDir.push_back(3);
		if(g.map.map_[y+1][x]!='2'&&backDir!=4&&dir!=4)okDir.push_back(4);
		if(okDir.size()>0){
			int base = 100.0f/okDir.size();		//获取每个方向被抽中的概率
			int idx = 1;
			for(int dir:okDir){
				if(random>(idx-1)*base&&random<=idx*base)		//如果抽中,那么就改为这个方向 
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
		//如果场上的敌方坦克数量少于5辆，就生成一辆 ,如果两个生成点都不能生成，这个时间刻度就先不生成 
		if(Tank::e < Tank::max_e){		 
			random = rand()%2;		//随机在左边或右边生成坦克 
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
	//生成玩家坦克
	if(g.t_player1.isLife()==false&&g.lifeTotal_f>0){
			g.t_player1 = {g.core.x-2,g.core.y,1,2};
			g.lifeTotal_f--;
	}
	if(g.isDouble&&g.t_player2.isLife()==false&&g.lifeTotal_f>0){
			g.t_player2 = {g.core.x+2,g.core.y,1,2};
			g.lifeTotal_f--;
	}
	if(g.lifeTotal_f==0){			//玩家阵营没有储备命了 
		if(g.t_player1.isLife()==false)
			if(!g.isDouble)g.gameOver = true;			//如果玩家1死亡并且不是双人，则游戏结束
			else if(g.isDouble&&g.t_player2.isLife()==false)g.gameOver = true;			//如果是双人且玩家2的坦克也已阵亡，则游戏结束 
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

void Shell::move(){				//炮弹的移动 
	if(clock()-_time>move_cd){
		strcpy(g.printMap[y][x],"  ");
		Point changeP = this->getPoint();
		switch(dir){			//炮弹先前进一格，至于死亡后面再判断 
			case 1:changeP.x-=1;break;
			case 2:changeP.y-=1;break;
			case 3:changeP.x+=1;break;
			case 4:changeP.y+=1;break;
		}
		switch(g.map.map_[changeP.y][changeP.x]){
			case '1':g.map.map_[changeP.y][changeP.x] = '0';this->kill();break;		//打到的是砖块，那么砖块消失，炮弹死亡 
			case '2':this->kill();break;						//打到的是铁块，炮弹死亡 
			case '9':this->kill();g.gameOver = true;			//如果打到的是核心，那么游戏结束 
		}
		//检测有没有碰到敌方坦克 
		for(list<Tank>::iterator tmp = g.tank.begin();tmp!=g.tank.end();){
			if(tmp->getPoint().x==changeP.x&&tmp->getPoint().y==changeP.y){
				this->kill();
				if(camp==1)	{	//如果是玩家的炮弹，则坦克阵亡
					tmp->kill();
					g.tank.erase(tmp++);
					continue; 
				}
			} 
			tmp++;
		}
		//检测有没有碰到其他炮弹 
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
			if(camp==2)g.t_player1.kill();				//如果是敌方炮弹，则玩家1坦克阵亡 
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
void Game::refreshGame(){			//第一次游戏或切换关卡时调用此函数 
	g.map = g.maps[++g.num-1];		//根据关卡切换游戏当前地图
	g.tmpMap = g.map;
	g.gaming= true;
	g.win = false;
	if(g.prop!=NULL)delProp();
	system("cls");
	p.borP(g.map.name);				//打印关卡数 
	Sleep(mTime);
	system("cls");
	//获取核心位置,且给周围一圈加上围墙 
	for(int i = 0;i <= g.map.y+1;i++)
		for(int j = 0;j <= g.map.x+1;j++)
			if(i==0||i==g.map.y+1||j==0||j==g.map.x+1)		//加围墙 
				g.map.map_[i][j] = '2';
			else if(g.map.map_[i][j]=='9'){
				 g.core.x = j;
				 g.core.y = i;
			}
	//初始化敌方坦克总数
	g.lifeTotal_e = g.map.lifeTotal_e; 
	//生成玩家坦克
	g.t_player1 = {g.core.x-2,g.core.y,1,2};g.lifeTotal_f--;
	if(isDouble) {
		g.t_player2 = {g.core.x+2,g.core.y,1,2};g.lifeTotal_f--;
	}
	//重置该局游戏时间 
	g.gameClock = clock();
	//初始化游戏面板模样
	for(int i = 0;i <= g.map.y+1;i++)
		for(int j = 0;j <= g.map.x+1;j++)
			switch(g.map.map_[i][j]){
				case '0':strcpy(g.printMap[i][j],"  ");break;
				case '1':strcpy(g.printMap[i][j],"▓");break;
				case '2':strcpy(g.printMap[i][j],"█");break;
				case '9':strcpy(g.printMap[i][j],"∞");break;
			}
}
void Game::printGame(){
	static int lastF = 0;
	static int lastE = 0;
	static clock_t _time = 0;
	if(clock()-_time>g.refreshMapTime){
		Point tmpPoint;
		ZERO;							//重置光标位置为0,0 
		//先更新地图 
		for(int i = 1;i <= g.map.y;i++)
			for(int j = 1;j <= g.map.x;j++)
				if(g.map.map_[i][j]!=g.tmpMap.map_[i][j])		//如果上次刷新时地图的这个点和这次不同，就修改
					strcpy(g.printMap[i][j],"  ");			//以目前的规则，方块更新只有可能是砖块变为空
		g.tmpMap = g.map;									//用完后，更新临时地图 
		//然后把炮弹放进地图
		for(Shell tmp:g.shell)
			tmp.print();	
		//再把坦克放进地图
		for(Tank tmp:g.tank) 
			tmp.print();
		if(g.t_player1.isLife())g.t_player1.print();
		if(isDouble&&g.t_player2.isLife())g.t_player2.print();
		//把道具放入地图
		if(g.prop!=NULL&&g.prop->ispickup()==false){g.prop->print();}
		//打印游戏面板
		for(int i = 0;i <= g.map.y+1;i++){
			for(int j = 0;j <= g.map.x+1;j++)
				cout << g.printMap[i][j];
			cout << endl;
		} 
		//最后打印游戏数据
		p.pLine(g.map.name,g.map.x*2+4);
		printf("游戏时间:%.1fs\n",(clock()-g.gameClock)/CLOCKS_PER_SEC);
		printf("玩家剩余坦克:");
		for(int i = 0;i < g.lifeTotal_f;i++)
			cout << "↑";
		for(int i = g.lifeTotal_f;i < lastF;i++)
			cout << "  ";
		cout << endl;
		printf("敌方剩余坦克:");
		for(int i = 0;i < g.lifeTotal_e;i++)
			cout << "↑";
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
	while(g.map.map_[p.y][p.x]!='2'){			//只要没碰见铁块方块，就继续查找 
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
//输出标题 
void Prt::pLine(char str[],int len_){
	int i;
	int s_len = 0;
	for(i = 0;str[i];i++)s_len++;
	for(i = 0;i < (len_-s_len)/2;i++)printf("*");
	printf("%s",str);
	for(i = 0;i < (len_-s_len)/2;i++)printf("*");
	if((len_-s_len)%2)printf("*");		//如果中间内容不是双数长，末尾就补一个*号 
	printf("\n");
} 
//输出一行 
void Prt::print(char str[]){
	int i,s_len = 0;
	for(i = 0;str[i];i++)s_len++;
	mar();		//每行先输出外边距 
	printf("│");
	for(i = 0;i < (Len-s_len)/2;i++)printf(" ");
	printf("%s",str);
	s_len = s_len%2?s_len-1:s_len;
	for(i = 0;i < (Len-s_len)/2;i++)printf(" ");
	printf("│\n");
}

//按自动套边框的格式输出,如果框内要输出多行,就用&符连接 
void Prt::borP(char str[]){
	int i,s_len = 0;
	int begin = 0;		//子串从str的哪个元素下标开始复制 
	bool haveLine = false;
	char substr[300];
	substr[0] = '\0';		//初始化
	for(i = 0;i < margin;i++)printf("\n");
	mar();printf("┌");for(i = 0;i < Len/win;i++)printf("─");printf("┐\n");
	for(i = 0;i < padding;i++)print("");
	for(i = 0;1;i++){
		if(str[i]=='&'){substr[(haveLine?Len*2/win:i-begin)] = '\0';begin = i+1;print(substr);substr[0] = '\0';haveLine = false;}
		else if(str[i]=='~'){
			for(int j = 0;j < Len;j++){
				strcat(substr,"─");
			}
			haveLine = true;
		}
		else if(str[i] == '\0'){substr[i-begin] = '\0';print(substr);break;}
		else 
			substr[i-begin] = str[i]; 
		
	}
	for(i = 0;i < padding;i++)print("");
	mar();printf("└");for(i = 0;i < Len/win;i++)printf("─");printf("┘\n");
	for(i = 0;i < margin;i++)printf("\n");
	this->Len = 60;
}
void Prt::borPLogo(char str[]){
	this->Len = 100;
	char logoStr[2000] = {"\
▓▓▓▓▓▓        ▓▓▓        ▓▓▓▓▓▓  ▓▓▓▓▓▓  ▓▓          ▓▓▓▓▓▓&\
▓▓      ▓▓    ▓▓  ▓▓          ▓▓          ▓▓      ▓▓          ▓▓        &\
▓▓      ▓▓  ▓▓      ▓▓        ▓▓          ▓▓      ▓▓          ▓▓        &\
▓▓▓▓▓▓    ▓▓      ▓▓        ▓▓          ▓▓      ▓▓          ▓▓▓▓▓  &\
▓▓      ▓▓  ▓▓▓▓▓▓▓        ▓▓          ▓▓      ▓▓          ▓▓        &\
▓▓      ▓▓  ▓▓      ▓▓        ▓▓          ▓▓      ▓▓          ▓▓        &\
▓▓▓▓▓▓    ▓▓      ▓▓        ▓▓          ▓▓      ▓▓▓▓▓▓  ▓▓▓▓▓▓&\
&&\
    ▓▓▓▓      ▓▓▓▓▓▓    ▓▓▓▓▓▓    ▓▓    ▓▓&\
  ▓▓    ▓▓        ▓▓            ▓▓        ▓▓    ▓▓&\
▓▓                  ▓▓            ▓▓        ▓▓    ▓▓&\
▓▓                  ▓▓            ▓▓          ▓▓▓▓  &\
▓▓                  ▓▓            ▓▓            ▓▓    &\
  ▓▓    ▓▓        ▓▓            ▓▓            ▓▓    &\
    ▓▓▓▓      ▓▓▓▓▓▓        ▓▓            ▓▓    &\
&&抽象坦克大战0.1beta测试版&&"};
	strcat(logoStr,str);
	borP(logoStr);
}
Prop::Prop(int id_,int x_,int y_){
	id = id_;x = x_;y = y_;
	lifeTime = clock();
	tmp = 0;
	max_propTime = 0; 
	switch(id){		
		case 1:strcpy(shape,"弹");break;
		case 2:strcpy(shape,"速");break;
		case 3:strcpy(shape,"命"); break;
	}
} 
void Prop::show(){
	propTime = clock();
	switch(id){
		case 1:
			for(list<Tank>::iterator it = g.tank.begin();it!=g.tank.end();){			//清除所有场上敌方坦克
				strcpy(g.printMap[it->getPoint().y][it->getPoint().x] ,"  ");
				g.tank.erase(it++);
			}
			break;
		case 2:
			tmp = Tank::fire_cd_f;											//提高射速,持续五秒 
			max_propTime = 5000;Tank::fire_cd_f = 100;break;
		case 3:
			g.lifeTotal_f++;break;											//玩家阵营加一条命 
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
bool Prop::regain(){				//是否可以删除了 
	bool ret = false;
	if(ispickup()&&max_propTime>0){			//如果已拾取并且是时效道具 
		if(clock()-propTime>max_propTime){			//道具时效已到 
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
