#include <TFT_eSPI.h>         // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();    // Invoke custom library


void setup(){

   pinMode(WIO_KEY_A, INPUT_PULLUP);
   pinMode(WIO_KEY_B, INPUT_PULLUP);
   pinMode(WIO_KEY_C, INPUT_PULLUP);
   pinMode(WIO_5S_UP, INPUT_PULLUP);
   pinMode(WIO_5S_DOWN, INPUT_PULLUP);
   pinMode(WIO_5S_LEFT, INPUT_PULLUP);
   pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
   pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  
    // tft 画面初期設定
    tft.init();
    tft.setRotation(3); // 3 がスティックのある面を下にした状態
    tft.fillScreen(TFT_BLACK);
}

void loop(){
  int i=1;
  int c=48;
  char str[16];

  for(int y=0;y<240;y=y+16){
    for(int x = 0;x<320;x=x+16){
        if(i<255){
          tft.drawChar( x,y, i,TFT_DARKGREY, TFT_BLACK, 2);
          i=i+1;
        }
    }
  }

  while(1){
    if (digitalRead(WIO_5S_UP) == LOW) {
      if(c>1) c=c-1;
    }
    if (digitalRead(WIO_5S_DOWN) == LOW) {
      if(c<255) c=c+1;
    }
    sprintf(str,"%d:",c);
    tft.drawString(str, 250, 230);
    tft.drawChar( 280,230, c,TFT_WHITE, TFT_BLACK, 1);
    delay(200);
  }
  
}
