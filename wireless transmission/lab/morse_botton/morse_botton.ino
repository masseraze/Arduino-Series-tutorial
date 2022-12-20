#include<SPI.h>
#include"RF24.h"

RF24 rf24(7,8);

const byte addr[]="1Node";
char Y='0';
char msg[32]={'\0'};
int n=0;
int button=A0;
int button2=A1;
boolean STATE = false; //是否觸發button
unsigned long previous = 0; //觸發時button的時間
unsigned long current = 0; //當前的時間
void setup() {
  Serial.begin(9600);
  pinMode(button,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  Serial.println("button start");
  rf24.begin();
  rf24.setChannel(60);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setDataRate(RF24_250KBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式
}

void loop() {
  if(digitalRead(button)==LOW){ //按下button時進入
     delay(100);//處理button彈跳的問題
     if(STATE == false){ //當第一下觸發button
        previous = millis(); //設定按下button的時間點 
        STATE = true; //表示已經按button了
        Y='0';
       }
     else{
        current = millis(); //設定當前的時間點，也就是持續按壓button的時刻
        if(STATE == true && current-previous >= 500) //若連續按壓超過0.5s
           Y='1';
      }
    }
  else if(digitalRead(button2)==LOW){ //按下button時進入
     delay(100);//處理button彈跳的問題
     if(STATE == false){ //當第一下觸發button
        previous = millis(); //設定按下button的時間點 
        STATE = true; //表示已經按button了
        Y=' ';
       }
     else{
        current = millis(); //設定當前的時間點，也就是持續按壓button的時刻
        if(STATE == true && current-previous >= 500) //若連續按壓超過0.5s
           Y='\n';
      }
    }
  else{ 
    if(STATE==true){
      msg[n]=Y;
      Serial.println((String)+"n"+n+"y"+Y);
      n++;
      if((n==32)||(Y=='\n')){
        rf24.write(&msg,n);
        Serial.print(msg);
        for(int k=0;k<n;k++){
          msg[k]='\0';
        }
        n=0;
      }
    }
    STATE =false; //若放開button，則將狀態重設為false
  }
}
