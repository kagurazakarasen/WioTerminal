#include <TFT_eSPI.h>         // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();    // Invoke custom library

#define TEXT_HEIGHT     16     // Height of text to be printed and scrolled
#define TEXT_WIDTH     16     
#define PLAY_FIELD_WIDTH  19
#define PLAY_FIELD_HEIGHT    14


#define BUZZER_PIN WIO_BUZZER

int STAGE[PLAY_FIELD_WIDTH][PLAY_FIELD_HEIGHT];

int MyX = 10; // Start Position
int MyY = 8;

int NowX = 0;
int NowY = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // いらないよね
  
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

   stageInit();

   Serial.print("Start ");

   while( !buttonChk() ){
    // start loop
   }
    Serial.println(": ");
   
   NowY=0;
   NowX=0;


}

void stageInit(){
    tft.fillScreen(ILI9341_WHITE);
    for(int i=TEXT_HEIGHT;i<PLAY_FIELD_HEIGHT*TEXT_HEIGHT;i+=1){
      tft.drawLine(TEXT_WIDTH-8,i,PLAY_FIELD_WIDTH*TEXT_WIDTH,i,ILI9341_BLACK); //drawRectangle も、fillRectangle も使えない！？
    }

 
    for (int y = 1; y < PLAY_FIELD_HEIGHT; y += 1) {        
      for (int x = 1; x < PLAY_FIELD_WIDTH; x += 1) {
          int n = random(1, 9);
          STAGE[x][y]=n;  // 画面キャラ情報を配列に入れておく
          tft.drawNumber(STAGE[x][y], x*TEXT_WIDTH, y*TEXT_HEIGHT, 2); // Draw the Number
      }
    }
    //初期キャラ位置
    tft.drawChar('X', MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2);
    STAGE[MyX][MyY]=0;  // 空白は 0 とする。    

    tft.drawString(" < NUMBER TRON > ", 100, 0, 2); // Draw the Number character
  
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

  return(r);
}

void GameOver(){
  tft.drawChar('#', MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2); // 自キャラ表示
  while(1){
    //
  }
}

void loop() {
//  String s;
  char s[100];
 int b;
    Serial.printf("NowX:%d , NowY:%d \n",NowX,NowY);

  if(NowX==0 && NowY==0){ // 移動していなかったらボタンチェック
    b = buttonChk();
    Serial.print("B if ");

    if(b){
      Serial.println("in... ");
      //点滅開始
      int c=1;
      int ox,oy;
      while(c){
        ox=NowX;
        oy=NowY;
        tft.drawString("  ", (MyX+ox)*TEXT_WIDTH, (MyY+oy)*TEXT_HEIGHT, 2); //Char(一文字）だとゴミが残る
        delay(500);
        tft.drawNumber(STAGE[MyX+ox][MyY+oy], (MyX+ox)*TEXT_WIDTH, (MyY+oy)*TEXT_HEIGHT, 2); // Draw the Number
        playTone(1014, 100); 
        c = buttonChk();
        tft.drawString("  ", (MyX)*TEXT_WIDTH, (MyY)*TEXT_HEIGHT, 2); //Char(一文字）だとゴミが残る
      }
      Serial.print("C Loop Out   ");
      for(int i=STAGE[MyX+ox][MyY+oy];i>0;i=i-1){
         MyX=MyX+ox;
         MyY=MyY+oy;
         Serial.printf(s," MyX:%d MyY:%d Val:%d \n",MyX,MyY,STAGE[MyX][MyY]);
//         tft.drawString(s, 110, 0, 2); // Draw the Number
         if(STAGE[MyX][MyY]==0 ||MyX<1||MyY<1 || MyX>PLAY_FIELD_WIDTH-1 || MyY>PLAY_FIELD_HEIGHT-1  ){
            playTone(1614, 1000); 
            GameOver();
         }      
         playTone(1000-STAGE[MyX][MyY]*100, 100);
         playTone(1500, 30); 
         tft.drawChar('X', MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2); // 自キャラ表示
          STAGE[MyX][MyY]=0; // キャラ位置データも消去
          delay(500);
      
          //tft.drawChar(' ', MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2);
          tft.drawString("  ", MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2); //Char(一文字）だとゴミが残る
        
      }

    
    }else{
      tft.drawString("  ", MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2); //Char(一文字）だとゴミが残る
      playTone(1014, 100); 
      tft.drawChar('X', MyX*TEXT_WIDTH, MyY*TEXT_HEIGHT, 2); // 自キャラ表示
      delay(500);
    }

  }
  

  
}