#include <iostream>
#include <ctime> //for time();
#include <cstdlib> // for rand(); and srand();
#include <fstream> // write data on save.txt and read from it
#include <time.h>
using namespace std;
typedef long long ll;
struct pos{
  bool t =false; //  state
  bool bomb = false;
  char nb = 0;   //number of neighbour_bombs
  char state = '*';

};
struct user{
  char name[101];
  ll score = 0;
  int h=0,m=0,s=0;
  time_t total;
};
void bomb_pos(pos **,int,int,int); //set bombs in the area 
void neighbour_bomb(pos **,int,int); // count neighbour bombs
void revealed(pos **,user &,int,int,bool);//show the entire area
void show_zeros(pos **,user &,int,int,int,int); //it  shows neighbour zeros
void print_area(pos **,user &,int,int);  //print the game area while player is still palying
void my_timer(time_t,time_t,user &);   // timer
void spaces(int);    // put n spaces
int nlen(int);    // length of a number
int main(){
  fstream save;
  int n,m,b,r,c;
  bool explosion = false,check = true;
  user u;
  time_t start,finish;

  char cm,yn;
  cout<<"Welcome"<<endl;
  cout<<"What is your name soldier?: ";
  cin.getline(u.name,100);
  cout<<"Try to find Bombs and save poor people of this land"<<endl;
  while(true){  //real start
  u.score =0;
  explosion = false;
  cout<<"enter the size of operation area: "<<endl;
  cout<<"number of rows: ";
  cin>>n;cout<<endl;
  cout<<"number of columns: ";
  cin>>m;cout<<endl;
  cout<<"how many bomb is enough for you?: ";
  cin>>b;cout<<endl;
  pos **area = new pos*[n];
  for(int i=0;i<n;i++){
    area[i] = new pos[m];
  }
  bomb_pos(area,n,m,b);
  neighbour_bomb(area,n,m);
	  start = time(0);
  while(u.score<m*n-b&& !explosion){ //playing the game
    check = true;
    print_area(area,u,n,m);
    finish = time(0);
    my_timer(start,finish,u);
    cout<<"time h: "<<u.h<<" min: "<<u.m<<" sec: "<<u.s<<endl;
    while(check){
      cout<<"enter o to dig a howl,enter ^ to put a flag,enter x to remove a flag: "<<endl;
      cin  >> cm;
      if(cm != 'o' && cm != '^' && cm != 'x')continue;
     cout<<"enter the row and the columns of the place you want to do the operation on it!: "<<endl;
     cout<<"enter the row: ";
     cin >> r;cout<<endl;
     if((r<0||r>=n)){
       cout<<"you should enter a number between 0 and "<<n-1<<endl;
       continue;
     }
     cout<<"enter the column: ";
     cin>>c;cout<<endl;
    if(c<0||c>=m){
      cout<<"you  should enter a number between 0 and "<<m-1<<endl;
      continue;
    }
    if(area[r][c].t){
      cout<<"you have dug this place already!"<<endl;
      continue;
    }
    check = false;
}
switch (cm) {
  case 'o':
      if(area[r][c].state=='^'){

        cout<<"you have put a flag on this place before,do you still want to dig here? enter y for yes,any thing else for no: ";
        cin>>yn;cout<<endl;
        if(yn!='y')break;
        }
      if(area[r][c].bomb){
        explosion = true;
        break;
      }
      if(area[r][c].nb!=0){
        area[r][c].t=true;
        area[r][c].state = area[r][c].nb;
        u.score++;
        break;
      }
        show_zeros(area,u,n,m,r,c);
        break;

  case '^':
        if(area[r][c].state=='^'){
          cout<<"you have put a flag on this place befor."<<endl;
          getchar();
          getchar();
          break;
        }
        area[r][c].state = '^';
        break;
  case 'x':
        if(area[r][c].state!='^'){
          cout<<"there is no flag here!"<<endl;
          getchar();
          getchar();
          break;
        }
        area[r][c].state = '*';
        break;
    }

}
revealed(area,u,n,m,explosion);
finish = time(0);
my_timer(start,finish,u);
cout<<"time h: "<<u.h<<" min: "<<u.m<<" sec: "<<u.s<<endl;
cout<<"do you like to try to save another area? enter y for yes,anything else for no: ";
save.open("./save.txt",ios::app);
save<<"name: "<<u.name<<' '<<"score: "<<u.score<<' '<<"time(second):"<< u.total<<'\n';
save.close();
cin>>yn;cout<<endl;
if(yn!='y'){
  cout<<"goodbye soldier "<<u.name;
  break;

}
for(int i=0;i<n;i++){
  delete[] area[i];
}
delete[] area;
}
system("cls");

return 0;
}
void bomb_pos(pos **area,int n,int m,int b){
  int bpx,bpy;
  srand (time(NULL));
  for(int i=0;i<b;i++){
    bpx = rand() % n;
    bpy = rand() % m;
    if(area[bpx][bpy].bomb){
      i--;
      continue;
    }
    area[bpx][bpy].bomb = true;
    }
}
void neighbour_bomb(pos **area,int n,int m){
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      if(i-1>=0 && j-1>=0){
        area[i][j].nb = area[i-1][j-1].bomb ? area[i][j].nb+1:area[i][j].nb;
      }
      if(i-1>=0){
        area[i][j].nb = area[i-1][j].bomb ? area[i][j].nb+1:area[i][j].nb;

      }
      if(i-1>=0 && j+1<m){
        area[i][j].nb = area[i-1][j+1].bomb ? area[i][j].nb+1:area[i][j].nb;

      }
      if(j-1>=0){
        area[i][j].nb = area[i][j-1].bomb ? area[i][j].nb+1:area[i][j].nb;

      }
      if(j+1<m){
        area[i][j].nb = area[i][j+1].bomb ? area[i][j].nb+1:area[i][j].nb;

      }
      if(i+1<n && j-1>=0){
        area[i][j].nb = area[i+1][j-1].bomb ? area[i][j].nb+1:area[i][j].nb;

      }
      if(i+1<n){
        area[i][j].nb = area[i+1][j].bomb ? area[i][j].nb+1:area[i][j].nb;

      }
      if(i+1<n && j+1<m){
        area[i][j].nb = area[i+1][j+1].bomb ? area[i][j].nb+1:area[i][j].nb;

      }
    }
  }
}
void spaces(int n){
  char s =' ';
  for(int i=0;i<n;i++){
    cout<<s;
  }
}
int nlen(int n){
  int l=0;
  if(n==0)return 1;
  while(n>0){
    n/=10;
    l++;
  }
  return l;
}
void revealed(pos **area,user &u,int n,int m,bool explosion){
    system("cls");
    int ln = nlen(n);
    int lm = nlen(m);
    spaces(ln+1);
    for(int i=0;i<m;i++){
          spaces(lm-nlen(i)+1);
          cout<<i;
          spaces(lm);
  }cout<<endl;
    for(int i=0;i<n;i++){
      cout<<i;
      spaces(ln-nlen(i)+1);
      for(int j=0;j<m;j++){
        if(area[i][j].bomb){
          spaces(lm);
          cout<<'o';
          spaces(lm);
        }
        else{
          spaces(lm);
          cout<<(int)area[i][j].nb;
          spaces(lm);
        }
      }
      cout<<'\n';
    }
    if(explosion){
    cout<<"Game Over"<<endl;
  }else{
    cout<<"Congratulation! you have saved people of this area!"<<endl;
  }
    cout<<u.name<<" final score is "<<u.score<<endl;
}
void show_zeros(pos **area,user &u,int n,int m,int i,int j){
  area[i][j].t = true;
  area[i][j].state = area[i][j].nb;
  u.score++;
  if(i-1>=0 && j-1>=0){
    if(!area[i-1][j-1].t&&!area[i-1][j-1].bomb){
    if(!area[i-1][j-1].nb)show_zeros(area,u,n,m,i-1,j-1);
    else {
      area[i-1][j-1].t = true;
      area[i-1][j-1].state = area[i-1][j-1].nb;
      u.score++;
    }
  }
  }
  if(i-1>=0){
    if(!area[i-1][j].t&&!area[i-1][j].bomb){
    if(!area[i-1][j].nb)show_zeros(area,u,n,m,i-1,j);
    else {
      area[i-1][j].t = true;
      area[i-1][j].state = area[i-1][j].nb;
      u.score++;
  }
  }
}
  if(i-1>=0 && j+1<m){
    if(!area[i-1][j+1].t&&!area[i-1][j+1].bomb){
    if(!area[i-1][j+1].nb)show_zeros(area,u,n,m,i-1,j+1);
    else {
      area[i-1][j+1].t = true;
      area[i-1][j+1].state = area[i-1][j+1].nb;
      u.score++;
    }
  }
  }
  if(j-1>=0){
    if(!area[i][j-1].t&&!area[i][j-1].bomb){
    if(!area[i][j-1].nb)show_zeros(area,u,n,m,i,j-1);
    else {
      area[i][j-1].t = true;
      area[i][j-1].state = area[i][j-1].nb;
      u.score++;
    }
  }
  }
  if(j+1<m){
    if(!area[i][j+1].t&&!area[i][j+1].bomb){
    if(!area[i][j+1].nb)show_zeros(area,u,n,m,i,j+1);
    else {
      area[i][j+1].t = true;
      area[i][j+1].state = area[i][j+1].nb;
      u.score++;
  }
}

  }
  if(i+1<n && j-1>=0){
    if(!area[i+1][j-1].t&&!area[i+1][j-1].bomb){
    if(!area[i+1][j-1].nb)show_zeros(area,u,n,m,i+1,j-1);
    else {
    area[i+1][j-1].t = true;
    area[i+1][j-1].state = area[i+1][j-1].nb;
    u.score++;
  }
  }
  }
  if(i+1<n){
    if(!area[i+1][j].t&&!area[i+1][j].bomb){
    if(!area[i+1][j].nb)show_zeros(area,u,n,m,i+1,j);
    else{
      area[i+1][j].t = true;
      area[i+1][j].state = area[i+1][j].nb;
      u.score++;
   }
  }
  }
  if(i+1<n && j+1<m){
    if(!area[i+1][j+1].t&&!area[i+1][j+1].bomb){
    if(!area[i+1][j+1].nb)show_zeros(area,u,n,m,i+1,j+1);
    else{
       area[i+1][j+1].t = true;
       area[i+1][j+1].state = area[i+1][j+1].nb;
       u.score++;

  }
}

}
}
void print_area(pos **area,user &u,int n,int m){
  system("cls");
  int ln=nlen(n),lm=nlen(m);
  spaces(ln+1);
  for(int i=0;i<m;i++){
        spaces(lm-nlen(i)+1);
        cout<<i;
        spaces(lm);
}cout<<endl;
  for(int i=0;i<n;i++){
    cout<<i;
    spaces(ln-nlen(i)+1);
    for(int j=0;j<m;j++){
      if(area[i][j].t){
        spaces(lm);
        cout<<(int)area[i][j].state;
        spaces(lm);
      }

      else{
        spaces(lm);
        cout<<area[i][j].state;
        spaces(lm);
      }

    }
    cout<<'\n';
  }
  cout<<"name: "<<u.name<<" score: "<<u.score<<endl;
}
void my_timer(time_t start,time_t finish,user &u){
  u.total = finish-start;
  time_t t=finish-start;
  u.h = t /3600;
  t %= 3600;
  u.m = t / 60;
  t %= 60;
  u.s = t;

}
