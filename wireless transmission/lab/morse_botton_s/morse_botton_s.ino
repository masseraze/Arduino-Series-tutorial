#include <SPI.h>
#include "RF24.h"

RF24 rf24(7, 8); // CE腳, CSN腳

const byte addr[] = "1Node";
const byte pipe = 1;  // 指定通道編號

char alphabet[29] = {'e', 't', 'i', 'a', 'n', 'm', 's', 'u', 'r', 'w', 'd', 'k', 'g', 'o', 'h', 'v', 'f', ';', 'l', ';', 'p', 'j', 'b', 'x', 'c', 'y', 'z', 'q', '\n'};
char morse[31][5] = {{'\n', '\n', '\n', '\n', '\n'},
  {'\n', '\n', '\n', '\n', '\n'},
  {'0', ' ', ' ', ' ', ' '},//e 2
  {'1', ' ', ' ', ' ', ' '},//t 3
  {'0', '0' , ' ', ' ', ' '}, //i 4
  {'0', '1' , ' ', ' ', ' '}, //a 5
  {'1', '0' , ' ', ' ', ' '}, //n 6
  {'1', '1' , ' ', ' ', ' '}, //m 7
  {'0', '0' , '0' , ' ', ' '}, //s 8
  {'0', '0' , '1' , ' ', ' '}, //u 9
  {'0', '1' , '0' , ' ', ' '}, //r 10
  {'0', '1' , '1' , ' ', ' '}, //w 11
  {'1', '0' , '0' , ' ', ' '}, //d 12
  {'1', '0' , '1' , ' ', ' '}, //k 13
  {'1', '1' , '0' , ' ', ' '}, //g 14
  {'1', '1' , '1' , ' ', ' '}, //o 15
  {'0', '0' , '0' , '0' , ' '}, //h 16
  {'0', '0' , '0' , '1' , ' '}, //v 17
  {'0', '0' , '1' , '0' , ' '}, //f 18
  {'0', '0' , '1' , '1' , ' '}, //none 19
  {'0', '1' , '0' , '0' , ' '}, //l 20
  {'0', '1' , '0' , '1' , ' '}, //none 21
  {'0', '1' , '1' , '0' , ' '}, //p 22
  {'0', '1' , '1' , '1' , ' '}, //j 23
  {'1', '0' , '0' , '0' , ' '}, //b 24
  {'1', '0' , '0' , '1' , ' '}, //x 25
  {'1', '0' , '1' , '0' , ' '}, //c 26
  {'1', '0' , '1' , '1' , ' '}, //y 27
  {'1', '1' , '0' , '0' , ' '}, //z 28
  {'1', '1' , '0' , '1' , ' '}, //q 29
  {'\n', '\n', '\n', '\n', '\n'}};

void reverse_transformation(char buf[32]) {
  int n = 1;
  for (int v = 0; v < 32; v++) {
    if (buf[v] == '1') {
      n = 2 * n + 1;
    }
    else if (buf[v] == '0') {
      n = 2 * n;
    }
    else if (buf[v] != '\0') {
      n = n - 2;
      Serial.print(alphabet[n]);
      n = 1; //restart
    }
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  rf24.begin();
  rf24.setChannel(60);  // 設定頻道編號
  rf24.setDataRate(RF24_250KBPS);
  rf24.openReadingPipe(pipe, addr);  // 開啟通道和位址
  rf24.startListening();  // 開始監聽無線廣播
  Serial.println("nRF24L01 ready!");
}

void loop() {
  if (rf24.available(&pipe)) {
    char msg[32] = {'\0'};
    rf24.read(&msg, sizeof(msg));
    reverse_transformation(msg);
    Serial.print(msg); // 顯示訊息內容 
  }
}
