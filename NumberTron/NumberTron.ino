#include <TFT_eSPI.h>         // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();    // Invoke custom library

#define TEXT_HEIGHT     16     // Height of text to be printed and scrolled
#define TEXT_WIDTH     16     
#define PLAY_FIELD_WIDTH  19
#define PLAY_FIELD_HEIGHT    14


#define BUZZER_PIN WIO_BUZZER

int STAGE[PLAY_FIELD_WIDTH][PLAY_FIELD_HEIGHT];

#define X_START 10
#define Y_START 8

int PlayerMode = 1;  // 1 = OnePlayer / 2= TwoPlayers

int NowPlayer = 1;  // 現在のプレイヤー

int MyX; // Start Position
int MyY;

int NowX; // 現在のポジション
int NowY;

uint16_t ScoreP1;
uint16_t ScoreP2;
uint16_t HiScore;


void setup() {
  HiScore=0;
  ScoreP1 =0;
  ScoreP2 =0;

  Serial.begin(115200); // デバッグ用のほか、Serialからもコントロール可にしておく。
  
   pinMode(WIO_KEY_A, INPUT_PULLUP);
   pinMode(WIO_KEY_B, INPUT_PULLUP);
   pinMode(WIO_KEY_C, INPUT_PULLUP);
   pinMode(WIO_5S_UP, INPUT_PULLUP);
   pinMode(WIO_5S_DOWN, INPUT_PULLUP);
   pinMode(WIO_5S_LEFT, INPUT_PULLUP);
   pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
   pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  
   pinMode(BUZZER_PIN, OUTPUT);

    randomSeed(analogRead(A0)); // ランダムシードはガンダムシードに似ている（違

    // tft 画面初期設定
    tft.init();
    tft.setRotation(3); // 3 がスティックのある面を下にした状態

    firstScreen();
    gameStart();

}

void firstScreen(){
  tft.fillScreen(TFT_BLUE);

  putString("NUMBER TRON mini",10, 60,TFT_WHITE, TFT_BLUE, 2,12);

  putString("ORIGINAL GAME DESIGN",320-(20*10), 160,TFT_WHITE, TFT_BLUE, 1,10);

  putString("JACHIM FROHOLT"              ,320-(14*10), 175,TFT_WHITE, TFT_BLUE, 1,10);

  putString("WIO VERSION PROGRAM",320-(19*10), 190,TFT_WHITE, TFT_BLUE, 1,10);
  putString("RASEN KAGURAZAKA"       ,320-(16*10), 205,TFT_WHITE, TFT_BLUE, 1,10);

  tft.drawLine(24, 240, 24, 118, TFT_WHITE);
  putString("01234567890123456789012345",30, 120,TFT_WHITE, TFT_BLUE, 2,10);
  tft.drawLine(294,0, 294, 136, TFT_WHITE);
  tft.drawLine(24, 118, 294, 118,TFT_WHITE);
  tft.drawLine(24, 136, 294, 136,TFT_WHITE);

  putRoundRect("1P/2P",5, 6,  TFT_WHITE,TFT_BLACK,1,8, 4,  TFT_BLACK);
  putRoundRect("Level",80, 6,  TFT_WHITE,TFT_BLACK,1,8, 4,  TFT_BLACK);
  putRoundRect("Map",160, 6,  TFT_WHITE,TFT_BLACK,1,8, 4,  TFT_BLACK);

  putRoundRect("You can control it with a 5-Way Switch",6, 230,  TFT_WHITE,TFT_BLACK,1,8, 4,  TFT_BLACK);


}

void gameStart(){

    MyX = X_START;
    MyY = Y_START;
    NowX = 0;
    NowY = 0;
    
    ScoreP1=0;
    ScoreP2=0;

    NowPlayer = 1;

   //stageInit();
    
   Serial.print("Start ");

   while( !buttonChk() ){
    // start loop
    playerModeSelect();
   }
    Serial.println(": ");

    stageInit();
    stageProt();   // あった方が良いがスタート時に入力が一つ増える

   NowY=0;
   NowX=0;
  
}


void stageInit(){

    // マップデータセット
    for (int y = 1; y < PLAY_FIELD_HEIGHT; y += 1) {        
      for (int x = 1; x < PLAY_FIELD_WIDTH; x += 1) {
          int n = random(1, 10);
          if(n>5){
            n = random(1, 10);
            if(n>6){
              n = random(1, 10);
              if(n>7){
                n = random(1, 10);
                if(n>8){
                  //n = random(1, 10);  // 9がレアすぎてしまうのでここはオフ
                }
              }
            }
          }
          STAGE[x][y]=n;  // 画面キャラ情報を配列に入れておく
          //tft.drawChar( x*TEXT_WIDTH, y*TEXT_HEIGHT,STAGE[x][y]+48,ILI9341_GREEN-STAGE[x][y]*4, ILI9341_BLACK, 2);
      }
    }
    
    //stageProt();
  
}

void stageProt(){
    tft.fillScreen(ILI9341_GREEN);
    tft.fillRect(TEXT_WIDTH-8, TEXT_HEIGHT, PLAY_FIELD_WIDTH*TEXT_WIDTH-8, PLAY_FIELD_HEIGHT*TEXT_HEIGHT-16 , ILI9341_BLACK);

    // マップデータ描画
    for (int y = 1; y < PLAY_FIELD_HEIGHT; y += 1) {        
      for (int x = 1; x < PLAY_FIELD_WIDTH; x += 1) {
          if(STAGE[x][y]>0){
            tft.drawChar( x*TEXT_WIDTH, y*TEXT_HEIGHT,STAGE[x][y]+48,ILI9341_GREEN-STAGE[x][y]*4, ILI9341_BLACK, 2);
          }
      }
    }

    //初期キャラ位置
    tft.drawChar( MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT,'X',TFT_WHITE, TFT_BLACK,2); // 自キャラ表示
    STAGE[MyX][MyY]=0;  // 空白は 0 とする。    

    putString(" NUMBER TRON",0,0,TFT_WHITE,TFT_BLACK,2,11);
    
    scorePut();

}


int buttonChk(){  // 入力方向はNowXとNowYに入り、入力があったら1が戻り、なければ０を返す
  int r=1;
  if (digitalRead(WIO_5S_UP) == LOW) {
    NowY=-1;
    NowX=0;
    if (digitalRead(WIO_5S_LEFT) == LOW) {
      NowX=-1;
    } else if (digitalRead(WIO_5S_RIGHT) == LOW) {
      NowX=+1;
    }
  }else if(digitalRead(WIO_5S_DOWN) == LOW) {
    NowY=+1;
    NowX=0;
    if (digitalRead(WIO_5S_LEFT) == LOW) {
      NowX=-1;
    } else if (digitalRead(WIO_5S_RIGHT) == LOW) {
      NowX=+1;
    }
  }else if(digitalRead(WIO_5S_LEFT) == LOW) {
      NowY=0;
      NowX=-1;
  }else if (digitalRead(WIO_5S_RIGHT) == LOW) {
      NowY=0;
      NowX=+1;
  }else if (digitalRead(WIO_5S_PRESS) == LOW){
   r=1;
   NowY=0;
   NowX=0;
  }else {
   r=0;
   NowY=0;
   NowX=0;
  }

  //Serialからコントロール
  char key;     // 受信データを格納するchar型の変数

  // 受信データがあった時だけ、処理を行う
  if ( Serial.available() ) {       // 受信データがあるか？
    key = Serial.read();            // 1文字だけ読み込む
    Serial.write( key );            // 1文字送信。受信データをそのまま送り返す。
    r=0;
    switch(key){
      case '1':
        NowX=-1;
        NowY=+1;  
        r=1;
        break;
      case '2':
        NowX=0;
        NowY=+1;  
        r=1;
        break;
      case '3':
        NowX=+1;
        NowY=+1;  
        r=1;
        break;
      case '4':
        NowX=-1;
        NowY=0;  
        r=1;
        break;
      case '5':
        NowX=0;
        NowY=0;  
        //r=1;
        break;
      case '6':
        NowX=+1;
        NowY=0;  
        r=1;
        break;
      case '7':
        NowX=-1;
        NowY=-1;  
        r=1;
        break;
      case '8':
        NowX=0;
        NowY=-1;  
        r=1;
        break;
      case '9':
        NowX=+1;
        NowY=-1;  
        r=1;
        break;
    }

  }

  return(r);
}

void GameOver(){
  //tft.drawChar('#', MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2); // 自キャラ表示
  tft.drawChar( MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT,'#',TFT_RED, TFT_BLACK,2); // 自キャラ表示
  playTone(1614, 1000); 


  char txt[26];

  int higher;
  int score;


  if(PlayerMode==2){
      if(NowPlayer==1){
        putRoundRect("OPS! Player1 is DEAD !!",50,32,TFT_WHITE,TFT_BLACK,2,10,  8,  TFT_YELLOW);
        if(ScoreP1>ScoreP2){
          // P1 END で P1自分が勝ってる
          higher =1;
          putRoundRect("But ! You are WIN !!",70,60,TFT_WHITE,TFT_BLACK,2,10,  8,  TFT_YELLOW);
        } else {
          higher =2;
          putRoundRect("And You are LOSE !!",70,60,TFT_WHITE,TFT_BLACK,2,10,  8,  TFT_YELLOW);
        }
      } else {
        putRoundRect("OPS! Player2 is DEAD !!",50,32,TFT_WHITE,TFT_BLACK,2,10,  8,  TFT_YELLOW);
        if(ScoreP2>ScoreP1){
          putRoundRect("But ! You are WIN !!",70,60,TFT_WHITE,TFT_BLACK,2,10,  8,  TFT_YELLOW);
          higher =2;
        } else {
          higher =1;
          putRoundRect("And You are LOSE !!",70,60,TFT_WHITE,TFT_BLACK,2,10,  8,  TFT_YELLOW);
        }
      }

      if(higher==1){
        score = ScoreP1;
      } else {
        score = ScoreP2;
      }


  } else {
      putRoundRect("OPS!!!!",50,32,TFT_WHITE,TFT_BLACK,2,10,  8,  TFT_YELLOW);
      putRoundRect("G A M E O V E R !",80,62,TFT_RED,TFT_BLACK,2,10,  8,  TFT_YELLOW);
      score = ScoreP1;
  }


  if(HiScore<score){
      sprintf(txt,"P:%d Got Hi-Score!!",higher);

    putRoundRect(txt,70, 100,  TFT_RED,TFT_BLACK,2,10,  8,  TFT_YELLOW);

    HiScore=score;
  }

  scorePut();

  putRoundRect("Press buttton to restart!",110, 210,  TFT_YELLOW,TFT_BLACK,1,8, 4,  TFT_YELLOW);

  while(1){
    playerModeSelect();
     if (digitalRead(WIO_5S_PRESS) == LOW) {
        break;
     }
  }
  gameStart();
}


void playerModeSelect(){
  char txt[40];
       if (digitalRead(WIO_KEY_C) == LOW) { // PlayerMode Change
        if(PlayerMode==1){
          PlayerMode=2;
        }else{
          PlayerMode=1;
        }
        sprintf(txt,"Player mode %d",PlayerMode);

        putRoundRect(txt,10, 10,  TFT_YELLOW,TFT_BLACK,1,8, 4,  TFT_YELLOW);
        while(1){
          if (digitalRead(WIO_KEY_C) != LOW) { // PlayerMode Change
              //stageProt();
              sprintf(txt,"Player mode %d !",PlayerMode);
              putRoundRect(txt,50, 100,  TFT_YELLOW,TFT_BLACK,2,10, 8,  TFT_YELLOW);
              putRoundRect("Press Button to Start",50, 140,  TFT_YELLOW,TFT_BLACK,2,10, 8,  TFT_YELLOW);
              break;
          }
        }
     }
    
}


void loop() {
//  String s;
  char s[100];
 int b;
    //Serial.printf("NowX:%d , NowY:%d \n",NowX,NowY);

  if(NowX==0 && NowY==0){ // 移動していなかったらボタンチェック
    b = buttonChk();
    Serial.printf("b:%d ",b);

    if(b){
      //Serial.println("in... ");
      //点滅開始
      int c=1;
      int ox=0;
      int oy=0;

      while(c){
        ox=NowX;
        oy=NowY;
        tft.drawChar( (MyX+ox)*TEXT_WIDTH, (MyY+oy)*TEXT_HEIGHT,'.',ILI9341_GREEN, ILI9341_BLACK, 2); // キー入力方向いったん消し
        delay(200);
        if(STAGE[MyX+ox][MyY+oy]>0){ // 正の数なら（数字が入っていたら）
          tft.drawChar( (MyX+ox)*TEXT_WIDTH, (MyY+oy)*TEXT_HEIGHT,STAGE[MyX+ox][MyY+oy]+48,ILI9341_GREEN, ILI9341_BLACK, 2);  // 数字再プロット
        } else {
          tft.drawChar( (MyX+ox)*TEXT_WIDTH, (MyY+oy)*TEXT_HEIGHT,' ',ILI9341_GREEN, ILI9341_BLACK, 2); // 数字でなければ（０）なら空白
        }
        playTone(1014, 100); 
        c = buttonChk();
      }
      tft.drawChar( (MyX)*TEXT_WIDTH, (MyY)*TEXT_HEIGHT,' ',ILI9341_GREEN, ILI9341_BLACK, 2); // 自キャラ位置消し
      

      if(NowPlayer==1){  // スコアアップ
          ScoreP1=ScoreP1+STAGE[MyX+ox][MyY+oy];  
      }else{
          ScoreP2=ScoreP2+STAGE[MyX+ox][MyY+oy];  
      }

      Serial.printf("\n >> ox:%d oy:%d Get:%d\n",ox,oy,STAGE[MyX+ox][MyY+oy]);
      int GOF=0;  // GameOver FLAG
       if(STAGE[MyX+ox][MyY+oy]==0 ||MyX<1||MyY<1 || MyX>PLAY_FIELD_WIDTH-1 || MyY>PLAY_FIELD_HEIGHT-1  ){  // 一歩目自殺。これでforループをくくったほうがシンプル？
           MyX=MyX+ox;
           MyY=MyY+oy;
           GOF=1;  
       }      
      for(int i=STAGE[MyX+ox][MyY+oy];i>0;i=i-1){
         MyX=MyX+ox;
         MyY=MyY+oy;
         Serial.printf(" MyX:%d MyY:%d ox:%d oy:%d i:%d \n",MyX,MyY,ox,oy,i);
         if(STAGE[MyX][MyY]==0 ||MyX<1||MyY<1 || MyX>PLAY_FIELD_WIDTH-1 || MyY>PLAY_FIELD_HEIGHT-1  ){
            i=0;  // これが必要だったみたい
            //GameOver();
            GOF=1;  // ↑でOKだったが、念のためちゃんとループ脱出するようにしておく。
            break;
         }      
         playTone(1000-STAGE[MyX][MyY]*100, 100);
         playTone(1500, 30); 
         //tft.drawChar('X', MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2); // 自キャラ表示
         tft.drawChar( MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT,'X',TFT_RED, TFT_BLACK,2); // 自キャラ表示
          STAGE[MyX][MyY]=0; // キャラ位置データも消去
          delay(200);
      
          tft.drawChar( (MyX)*TEXT_WIDTH, (MyY)*TEXT_HEIGHT,' ',ILI9341_GREEN, ILI9341_BLACK, 2);
        
      }
      if(GOF){
        GameOver();
      } else {
        // ゲームオーバーではなかったらプレイヤーチェンジ
        // Player Change
        if(PlayerMode==2){
            Serial.printf("Player Change :%d -> ",NowPlayer);
            if(NowPlayer==1){
                NowPlayer=2;
            }else{
                NowPlayer=1;
            }
            Serial.printf("%d \n",NowPlayer);
        }
        scorePut();
      }

    
    }else{
      tft.drawChar( (MyX)*TEXT_WIDTH, (MyY)*TEXT_HEIGHT,' ',ILI9341_GREEN, ILI9341_BLACK, 2);
      playTone(1014, 100); 
      tft.drawChar( MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT,'X',TFT_WHITE, TFT_BLACK,2); // 自キャラ表示
      delay(200);
    }

  }
 
}

void playerInfo(){
      //Player Info
      char pls[16]; 
      sprintf(pls,"P%ds turn!",NowPlayer);
      putRoundRect(pls,130,230,TFT_WHITE,TFT_BLACK,1,8,  4,  TFT_YELLOW);
}

void scorePut(){

    //playerInfo();
  
    char scr[16];
    sprintf(scr,"P1-Score:%d",ScoreP1);
    if(NowPlayer==1){
      putRoundRect(scr,8,230,TFT_BLACK,TFT_RED,1,8,  4,  TFT_RED);
    }else{
      putRoundRect(scr,8,230,TFT_WHITE,TFT_BLACK,1,8,  4,  TFT_YELLOW);
    }

    int w=0;
    if(PlayerMode==2){
        sprintf(scr,"P2-Score:%d",ScoreP2);
        //int w=0;
        if(ScoreP2>9){
          w=w+1;
          if(ScoreP2>99){
            w=w+1;
            if(ScoreP2>999){
              w=w+1;
            }
          }
        }
        if(NowPlayer==2){
          putRoundRect(scr,220-(w*8),230,TFT_BLACK,TFT_RED,1,8,  4,  TFT_RED);
        }else{
          putRoundRect(scr,220-(w*8),230,TFT_WHITE,TFT_BLACK,1,8,  4,  TFT_YELLOW);
        }
    }

    sprintf(scr,"HiScore: %d",HiScore);
    w=0;
    if(HiScore>9){
      w=w+1;
      if(HiScore>99){
        w=w+1;
        if(HiScore>999){
          w=w+1;
        }
      }
    }
    putRoundRect(scr,228-(w*8),4,TFT_WHITE,TFT_BLACK,1,8,  4,  TFT_YELLOW);


}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(tone);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(tone);
  }
}

// カド丸長方形つきテキスト表示
void putRoundRect(String str,int32_t x, int32_t y,  uint32_t t_color, uint32_t bg, uint8_t textSize,int tWidth,   int32_t r, uint32_t rColor){

    int32_t w = tWidth * (str.length() + 1) + (r * 2);
    int32_t h = (r * 2) + tWidth;

    tft.fillRoundRect(x - tWidth, y - tWidth + (r/2), w,  h, r,  bg);

    tft.drawRoundRect( x - tWidth, y - tWidth + (r/2),  w, h, r, rColor);

    putString( str, x, y ,t_color,  bg, textSize, tWidth);
}

// 背景色（塗りつぶし）付きのテキスト表示ルーチン
void putString(String str, int32_t poX, int32_t poY,uint32_t t_color, uint32_t bg, uint8_t textSize,int tWidth){
    
    // Length (with one extra character for the null terminator)
    int str_len = str.length() + 1; 
    
    // Prepare the character array (the buffer) 
    char char_array[str_len];
    
    // Copy it over 
    str.toCharArray(char_array, str_len);
    
    for(int i=0;i<str_len;i++){
      tft.drawChar( poX + (i * tWidth), poY, char_array[i],t_color, bg, textSize);
    }
  
}
