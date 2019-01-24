/*kbhit.h等のファイルがないとコンパイルできません．http://www.segmentation-fault.xyz/entry/2017/02/08/190825 ここのサイトにあります*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define T 4//初期高さ
#define Y 6//初期横

char input();
void shaperotation(int a[][12], int ta, int yo, int ran, int rota);
void set(int a[][12]);
void shapebefor(int b[][3], int ranT);
int ta;
int yo;

int main(void)
{
   int a[27][12];
   KB_open();
   //ch=input();
   set(a);
   KB_close();
}
char input(){    
   if (kbhit()) {
      char key = linux_getch();
      return key;
    }
}
void set(int a[][12])
{
   int swap=0, i, j, f=0, cl=0, b[4][3]={0};//yoは横taは高さ
   char ch;
   for(i=1;i<26;i++){
      for(j=1;j<11;j++){
         a[i][j]=0;
      }
   }
//   for(i=0;i<27;i++){
//         a[i][0]=3;//3は左右の壁
//         a[i][11]=3;
//      }
//   for(i=0;i<12;i++){
//      a[0][i]=4;//4は上底
//      a[26][i]=4;
//   }
   //本体----------------------------------------------------------------------
   int ran, rota, ranT, hold;//ran=ブロック指定 ranT=次のブロック指定 rota=回転 hold=次のブロックと今のブロック入れ替え
   int overlap=0, stopline, flag=0, stopL, stopR;//stoplineは消えた一番下の高さをいれて、それより下のブロックが消えないようにする
   char pau;//終了のやつ
   srand((unsigned int) time(NULL));
   ranT=rand()%7;
   while(f!=2){
      f=0;
      stopR=0;
      stopL=0;
      rota=0;
      printf("%d",f);
      ta=T; yo=Y;//高さ横初期化
      cl=0;
      //横そろったら消す----------------------
      for(i=1;i<26;i++){
         swap=0;
         for(j=1;j<11;j++){
            if(a[i][j]==2) swap=1;
            else {
               swap=0;
               j=11;
               stopline=i;
            }
         }
         if(swap==1){
            for(j=1;j<11;j++) a[i][j]=0;
            for(i=stopline+1;i>1;i--){
               for(j=1;j<11;j++) a[i][j]=a[i-1][j];
            }
         }
      }
      //--------------------------------------------
      //新しいブロック出現-------------------------------
      ran=ranT;
      ranT=rand()%7;
      shapebefor(b,ranT);
      shaperotation(a, ta, yo, ran, rota);
      //--------------------------------------------------
      while(f==0){
         for(i=0;i<27;i++){
            a[i][0]=3;//3は左右の壁
            a[i][11]=3;
         }
         for(i=0;i<12;i++){
            a[0][i]=4;//4は上底
            a[26][i]=4;
         }

         /*for(i=0;i<27;i++){
            for(j=0;j<12;j++){
               if(a[i][j]!=2&&a[i][j]!=3&&a[i][j]!=4)
                  a[i][j]=0;//0は点
            }
         }*/
         //左右移動--------------------------------------
         stopL=0;
         stopR=0;
         ch=input();
         //左移動
         if(ch=='a'){
           for(i=1;i<11;i++){
             for(j=25;j>0;j--){
               if(a[j][i]==1&&(a[j][i-1]==0||a[j][i-1]==1)&&stopL==0) {
                  stopR=0;
               }
               else if(a[j][i]==1) stopL=1;
             }
           }
           for(i=1;i<11;i++){
              for(j=25;j>0;j--){
                 if(stopL==0&&a[j][i]==1){
                    a[j][i-1]=1;
                    a[j][i]=0;
                    }
              }
           }
           if(stopL==0) yo--;
         }
         //右移動
         else if(ch=='d'){
           //for(i=1;i<26;i++)
           //   if(a[i][10]==1) stopR=1;

           for(i=10;i>0;i--){
            for(j=25;j>0;j--){
               if(a[j][i]==1&&(a[j][i+1]==0||a[j][i+1]==1)&&stopR==0) {
                  stopL=0;
               }
               else if(a[j][i]==1) stopR=1;
            }
           }
           for(i=10;i>0;i--){
              for(j=25;j>0;j--){
                 if(a[j][i]==1&&stopR==0){
                    a[j][i+1]=1;
                    a[j][i]=0;
                 }
              }
           }
           if(stopR==0) yo++;
         }
//         for(i=1;i<26;i++){
//            if(a[i][0]==1)
//               for(i=1;i<26;i++)
//                  for(j=11;j>=0;j--)
//                     if(a[i][j]==1) a[i][j+1]=1;
//            
//            if(a[i][26]==1)
//               for(i=1;i<26;i++)
//                  for(j=1;j<=26;j++)
//                     if(a[i][j]==1) a[i][j-1]=1;
//         }
         //回転
         else if(ch=='r'){
            rota++;
            if(rota==4) rota=0;
            shaperotation(a, ta, yo, ran, rota);
         }
         //ブロック交換
         else if(ch=='g'){
            hold=ranT;
            ranT=ran;
            ran=hold;
            shaperotation(a, ta, yo, ran, rota);
            shapebefor(b,ranT);
         }
         //一時停止
         else if(ch=='q') {
            printf("PAUSE\n");
            scanf("%c",&pau);
         }
         //終了
         if(pau=='q') {
            f=2;
         }

         //----------------------------------------------
         //ブロック落る----------------------
            //早送り------------------------
         flag=0;
         if(ch==' '){
            for(i=1;i<26;i++)
               for(j=1;j<11;j++)
                  if(a[i][j]==1&&a[i+1][j]!=1&&a[i+1][j]!=0) flag=1;
            for(i=25;i>0;i--){
               for(j=1;j<11;j++){
                  if(a[i][j]==1&&a[i+1][j]==0&&flag==0){
                     a[i+1][j]=1;
                     a[i][j]=0;
                  }
               }
            }
            if(flag==0) ta++;
            for(i=25;i>0;i--){
               for(j=1;j<11;j++){
                  if(a[i][j]==1&&a[i+1][j]!=0&&a[i+1][j]!=1&&cl%1000==999){
                     for(i=25;i>0;i--){
                        for(j=1;j<11;j++){
                           if(a[i][j]==1) a[i][j]=2;
                           f=1;
                        }
                     }
                     break;
                  }
               }
            }
         }
              //-------------------------------------
         if(cl%1000==0){
            for(i=25;i>0;i--){
               for(j=1;j<11;j++){
                  if(a[i][j]==1){
                     a[i+1][j]=1;
                     a[i][j]=0;
                  }
               }
            }
            ta++;
         }
         //----------------------------------
         printf("%c[2J", 27);//画面クリーナー
      //----------------------------------------画面表示
         //落ち切ったブロックを白抜きにする
         if(cl%1000==999){
            for(i=25;i>0;i--){
               for(j=1;j<11;j++){
                  if(a[i][j]==1&&a[i+1][j]!=0&&a[i+1][j]!=1){
                     for(i=25;i>0;i--){
                        for(j=1;j<11;j++){
                           if(a[i][j]==1) a[i][j]=2;
                           f=1;
                        }
                     }
                  }
               }
            }
         }
         //操作方法の表示
         printf("a：左    ");printf("d：右\n");
         printf("q1回：一時停止 2回：終了\n");
         printf("r：回転");printf("g交換\n");
         //次のブロックを表示する
         for(i=0;i<4;i++){
            for(j=0;j<3;j++){
               if(b[i][j]==1) printf("■ ");
               else printf("  ");
            }
            printf("\n");
         }
         //ブロックとか表示
         for(i=0;i<27;i++){
            for(j=0;j<12;j++){
               if(i==T) printf("＝");
               else if(a[i][j]==3) printf("｜");
               else if(a[i][j]==4) printf("ー");
               else if(a[i][j]==0) printf("・");
               else if(a[i][j]==1) printf("■ ");
               if(a[i][j]==2) {
                  printf("□ ");
                  if(i<=T) {
                     f=2;
                  }
               }
            }
            printf("\n");
         }
         if(f==2&&pau!='q') printf("GAME OVER\n");
         if(pau=='q') printf("QUIT\n");
         //-----------------------------------------------------
         usleep(300);
         cl++;
      }
   }
}
void shapebefor(int b[][3], int ranT)
{
   int i, j;
   for(i=0; i<4; i++)
      for(j=0; j<3; j++)
         b[i][j]=0;
   switch(ranT){
      case 0:{//棒
         for(i=0; i<4; i++)
            b[i][1]=1;
            break;
      }
      case 1:{//L
         for(i=0; i<3; i++){
            b[3-i][0]=1;
            b[3][1]=1;
         }
            break;
      }
      case 2:{//逆L
         for(i=0; i<3; i++){
            b[3-i][1]=1;
            b[3][0]=1;
         }
            break;
      }
      case 3:{//Z
         b[1][1]=1; b[1][2]=1; b[0][1]=1; b[0][0]=1;
         break;
      }
      case 4:{//逆Z
         b[1][1]=1; b[1][0]=1; b[0][1]=1; b[0][2]=1;
         break;
      }
      case 5:{//四角
         b[0][0]=1; b[0][1]=1; b[1][0]=1; b[1][1]=1;
         break;
      }
      case 6:{//凸
         b[1][0]=1; b[1][1]=1; b[0][1]=1; b[1][2]=1;
         break;
      }
   }
}
int shaperotation(int a[][12], int ta, int yo, int ran, int rota)
{
   int i, j ,f;
   for(i=1;i<26;i++)
      for(j=1;j<11;j++)
         if(a[i][j]==1) a[i][j]=0;
   if(rota==0){
      switch(ran){
         case 0://棒
            for(i=0;i<4;i++)
               if(a[ta+i][yo>1])
						return 1;
            for(i=0;i<4;i++){
               a[ta-i][yo]=1;
            }
            break;
         case 1://L型
            if(a[ta][yo+1]>1)
					return 1;
            for(i=0;i<3;i++) {
               a[ta-i][yo]=1; a[ta][yo+1]=1;
            }
            break;
         case 2://逆L型
            if(a[ta][yo-1]>1)
					return 1;
            for(i=0;i<3;i++) {
               a[ta-i][yo]=1; a[ta][yo-1]=1;
               }
            break;
         case 3://Z型
            if(a[ta][yo+1]>1) 
					return 1;
            if(a[ta-1][yo-1]>1)
					return 1;
            a[ta][yo]=1; a[ta][yo+1]=1; a[ta-1][yo]=1; a[ta-1][yo-1]=1;
            break;
         case 4://逆Z
            {  if(a[ta][yo-1]>1) yo++;
               if(a[ta-1][yo+1]>1) yo--;
               a[ta][yo]=1; a[ta][yo-1]=1; a[ta-1][yo]=1; a[ta-1][yo+1]=1;}
            break;
         case 5://四角
            {a[ta][yo]=1; a[ta][yo-1]=1; a[ta-1][yo]=1; a[ta-1][yo-1]=1;}
            break;
         case 6://凸
            {  if(a[ta][yo+1]>1) yo--;
               if(a[ta][yo-1]>1) yo++;
               a[ta][yo]=1; a[ta-1][yo]=1; a[ta][yo-1]=1; a[ta][yo+1]=1;}
            break; 
      }
   }
   else if(rota==1){
      switch(ran){
         case 0://棒
            for(i=0;i<4;i++)
               if(a[ta][yo+i]>1) yo--;
            for(i=0;i<4;i++)
               a[ta][yo+i]=1;
            break;
         case 1://L型
            for(i=0;i<3;i++){
               if(a[ta][yo+i]>1) yo--;
            }
            if(a[ta+1][yo]>1) ta--;
            for(i=0;i<3;i++) {
               a[ta][yo+i]=1; a[ta+1][yo]=1;
            }
            break;
         case 2://逆L型
            for(i=0;i<3;i++){
               if(a[ta][yo+i]>1) yo--;
            }
            if(a[ta-1][yo]>1) ta--;
            for(i=0;i<3;i++) {a[ta][yo+i]=1; a[ta-1][yo]=1;}
            break;
         case 3://Z型
            {  if(a[ta][yo+1]>1) yo--;
               if(a[ta+1][yo]>1) ta--;
               a[ta][yo]=1; a[ta][yo+1]=1; a[ta-1][yo+1]=1; a[ta+1][yo]=1;}
            break;
         case 4://逆Z
            {  if(a[ta][yo-1]>1) yo++;
               if(a[ta+1][yo]>1) ta--;
               a[ta][yo]=1; a[ta][yo-1]=1; a[ta-1][yo-1]=1; a[ta+1][yo]=1;}
            break;
         case 5://四角
            {a[ta][yo]=1; a[ta][yo-1]=1; a[ta-1][yo]=1; a[ta-1][yo-1]=1;}
            break;
         case 6://凸
            {  if(a[ta][yo+1]>1) yo--;
               if(a[ta+1][yo]>1) ta--;
               if(a[ta-1][yo]>1) ta++;
               a[ta][yo]=1; a[ta-1][yo]=1; a[ta][yo+1]=1; a[ta+1][yo]=1;}
            break;
      }
   }
   else if(rota==2){
      switch(ran){
         case 0://棒
            for(i=0;i<4;i++)
               if(a[ta+i][yo]>1) ta--;
            for(i=0;i<4;i++)
               a[ta+i][yo]=1;
            break;
         case 1://L型
            for(i=0;i<3;i++){
               if(a[ta+i][yo]>1) ta--;
            }
            if(a[ta][yo-1]>1) yo++;
            for(i=0;i<3;i++) {
               a[ta+i][yo]=1; a[ta][yo-1]=1;
            }
            break;
         case 2://逆L型
            for(i=0;i<3;i++)
               if(a[ta+i][yo]>1) ta--;
            if(a[ta][yo+1]>1) yo--;
            for(i=0;i<3;i++) {a[ta+i][yo]=1; a[ta][yo+1]=1;}
            break;
         case 3://Z型
            {  if(a[ta][yo+1]>1) yo--;
               if(a[ta-1][yo-1]>1) yo++;
               a[ta][yo]=1; a[ta][yo+1]=1; a[ta-1][yo]=1; a[ta-1][yo-1]=1;}
            break;
         case 4://逆Z
            {  if(a[ta][yo-1]>1) yo++;
               if(a[ta-1][yo+1]>1) yo--;
               a[ta][yo]=1; a[ta][yo-1]=1; a[ta-1][yo]=1; a[ta-1][yo+1]=1;}
            break;
         case 5://四角
            {a[ta][yo]=1; a[ta][yo-1]=1; a[ta-1][yo]=1; a[ta-1][yo-1]=1;}
            break;
         case 6://凸
            {  if(a[ta+1][yo]>1) ta--;
               if(a[ta][yo-1]>1) yo++;
               if(a[ta][yo+1]>1) yo--;
               a[ta][yo]=1; a[ta+1][yo]=1; a[ta][yo-1]=1; a[ta][yo+1]=1;}
            break;
      }
   }
   else if(rota==3){
     switch(ran){
         case 0://棒
            for(i=0;i<4;i++)
               if(a[ta][yo-i]>1) yo++;
            for(i=0;i<4;i++)
               a[ta][yo-i]=1;
            break;
         case 1://L型
            for(i=0;i<3;i++)
               if(a[ta][yo-i]>1) yo++;
            if(a[ta-1][yo]>1) ta++;
            for(i=0;i<3;i++) a[ta][yo-i]=1;
            a[ta-1][yo]=1;
            break;
         case 2://逆L型
            for(i=0;i<3;i++)
               if(a[ta][yo-i]>1) yo++;
            if(a[ta+1][yo]>1) ta--;
            for(i=0;i<3;i++) a[ta][yo-i]=1;
            a[ta+1][yo]=1;
            break;
         case 3://Z型
            //{a[ta][yo]=1; a[ta][yo+1]=1; a[ta-1][yo]=1; a[ta-1][yo-1]=1;}
            {  if(a[ta][yo+1]>1) yo--;
               if(a[ta+1][yo]>1) ta--;
               a[ta][yo]=1; a[ta][yo+1]=1; a[ta-1][yo+1]=1; a[ta+1][yo]=1;}
            break;
         case 4://逆Z
            //{a[ta][yo]=1; a[ta][yo-1]=1; a[ta-1][yo]=1; a[ta-1][yo+1]=1;}
            {  if(a[ta][yo-1]>1) yo++;
               if(a[ta+1][yo]>1) ta--;
               a[ta][yo]=1; a[ta][yo-1]=1; a[ta-1][yo-1]=1; a[ta+1][yo]=1;}
            break;
         case 5://四角
            {a[ta][yo]=1; a[ta][yo-1]=1; a[ta-1][yo]=1; a[ta-1][yo-1]=1;}
            break;
         case 6://凸
            {  if(a[ta+1][yo]>1) ta--;
               if(a[ta-1][yo]>1) ta++;
               if(a[ta][yo-1]>1) yo++;
               a[ta][yo]=1; a[ta+1][yo]=1; a[ta-1][yo]=1; a[ta][yo-1]=1;}
            break;
      }
   }
}
