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

int MyX; // Start Position
int MyY;

int NowX;
int NowY;

uint16_t Score;
uint16_t HiScore;


void setup() {
  HiScore=0;
  Serial.begin(115200); // いる。Serialからもコントロール可に。
  
   pinMode(WIO_KEY_A, INPUT_PULLUP);
   pinMode(WIO_KEY_B, INPUT_PULLUP);
   pinMode(WIO_KEY_C, INPUT_PULLUP);
   pinMode(WIO_5S_UP, INPUT_PULLUP);
   pinMode(WIO_5S_DOWN, INPUT_PULLUP);
   pinMode(WIO_5S_LEFT, INPUT_PULLUP);
   pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
   pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  
   pinMode(BUZZER_PIN, OUTPUT);

    randomSeed(analogRead(A0)); // ランダムシード
    tft.init();
    tft.setRotation(3); // 3 がスティックのある面を下にした状態

    gameStart();

}

void gameStart(){

    MyX = X_START;
    MyY = Y_START;
    NowX = 0;
    NowY = 0;
    
    Score=0;

   stageInit();

   Serial.print("Start ");

   while( !buttonChk() ){
    // start loop
   }
    Serial.println(": ");
   
   NowY=0;
   NowX=0;
  
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


    //tft.setTextColor(color);
    // tft.setTextBackgroundColor(TFT_BLACK);          // これがない・・・
    //tft.setTextSize(textSize);                   //sets the size of text　（Charの2相当）Stringにのみにかかるのでここではか

    //↑これの変わりを tft.drawCharを使っておこなう
    
    // Length (with one extra character for the null terminator)
    int str_len = str.length() + 1; 
    
    // Prepare the character array (the buffer) 
    char char_array[str_len];
    
    // Copy it over 
    str.toCharArray(char_array, str_len);
    
    //String loop
    //int ts = textSize * TEXT_WIDTH ; // 幅のみでみてる。
    //int ts = textSize * 8 ;
    for(int i=0;i<str_len;i++){
      tft.drawChar( poX + (i * tWidth), poY, char_array[i],t_color, bg, textSize);
    }

    
  
}

void stageInit(){
    tft.fillScreen(ILI9341_GREEN);

    tft.fillRect(TEXT_WIDTH-8, TEXT_HEIGHT, PLAY_FIELD_WIDTH*TEXT_WIDTH-8, PLAY_FIELD_HEIGHT*TEXT_HEIGHT-16 , ILI9341_BLACK);

 
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
                  n = random(1, 10);
                }
              }
            }
          }
          STAGE[x][y]=n;  // 画面キャラ情報を配列に入れておく
          //tft.drawNumber(STAGE[x][y], x*TEXT_WIDTH, y*TEXT_HEIGHT, 2); // Draw the Number
          tft.drawChar( x*TEXT_WIDTH, y*TEXT_HEIGHT,STAGE[x][y]+48,ILI9341_GREEN-STAGE[x][y]*4, ILI9341_BLACK, 2);
      }
    }
    //初期キャラ位置
    //tft.drawChar('X', MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2);
    tft.drawChar( MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT,'X',TFT_WHITE, TFT_BLACK,2); // 自キャラ表示
    STAGE[MyX][MyY]=0;  // 空白は 0 とする。    

    //tft.drawString(" < NUMBER TRON > ", 100, 0, 2); // Draw the Number character
    putString(" NUMBER TRON",0,0,TFT_WHITE,TFT_BLACK,2,11);
    
    scorePut();
  
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(tone);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(tone);
  }
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
  }else{
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

  if(HiScore<Score){
    //tft.drawString(" You Got Hi-Score! ", 50, 110,2); 
    //putString(" You Got Hi-Score! ",50,100,TFT_YELLOW,TFT_BLACK,2,10);

    putRoundRect("You Got Hi-Score!",70, 100,  TFT_RED,TFT_BLACK,2,10,  8,  TFT_YELLOW);

    HiScore=Score;
  }

  scorePut();

  putRoundRect("Press buttton to restart!",110, 210,  TFT_YELLOW,TFT_BLACK,1,8, 4,  TFT_YELLOW);

  while(1){
     if (digitalRead(WIO_5S_PRESS) == LOW) {
        break;
     }
  }
  gameStart();
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
      //Serial.print("C Loop Out   ");
      Score=Score+STAGE[MyX+ox][MyY+oy];  // 移動 （無移動もあり得る）
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
//         tft.drawString(s, 110, 0, 2); // Draw the Number
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
      
          //tft.drawChar(' ', MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2);
          //tft.drawString("  ", MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2); //Char(一文字）だとゴミが残る
          tft.drawChar( (MyX)*TEXT_WIDTH, (MyY)*TEXT_HEIGHT,' ',ILI9341_GREEN, ILI9341_BLACK, 2);
        
      }
      if(GOF){
        GameOver();
      }

      scorePut();
    
    }else{
      //tft.drawString("  ", MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2); //Char(一文字）だとゴミが残る
      tft.drawChar( (MyX)*TEXT_WIDTH, (MyY)*TEXT_HEIGHT,' ',ILI9341_GREEN, ILI9341_BLACK, 2);
      playTone(1014, 100); 
      //tft.drawChar('X', MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2); // 自キャラ表示
      tft.drawChar( MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT,'X',TFT_WHITE, TFT_BLACK,2); // 自キャラ表示
      delay(200);
    }

  }
 
}

void scorePut(){
    char scr[16];
    sprintf(scr,"Score:%d",Score);
    //tft.drawString(scr, 12, 0, 2); // Draw Score
    //putString(scr,4,224,TFT_WHITE,TFT_BLACK,2,10);
    putRoundRect(scr,8,230,TFT_WHITE,TFT_BLACK,1,8,  4,  TFT_YELLOW);


    //char scr[10];
    sprintf(scr,"HiScore: %d",HiScore);
    //tft.drawString(scr, 240, 0, 2); // Draw Score
    //putString(scr,200,0,TFT_WHITE,TFT_BLACK,2,10);
    int w=0;
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
