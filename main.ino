#include <SPI.h>
#include <epd2in13.h>
#include <epdpaint.h>

#include "dino.c"
#include "background.c"
#include "cactus.c"

#define START 240
#define JUMP_BUTTON A0

Epd epd;
unsigned char shit[] = {0xFF, 0xFF};
float i;
bool GAME_OVER, EndScreen;

void setup() {
  Serial.begin(9600);
  GAME_OVER = false;
  EndScreen = false;
  i = 4;
  
  pinMode(JUMP_BUTTON, INPUT);
  digitalWrite(JUMP_BUTTON, HIGH);
  
  epd.Init(lut_full_update);
  
  epd.ClearFrameMemory(0xFF);
  epd.SetFrameMemory(BACKGROUND);
  epd.SetFrameMemory(TEST, 20, 5, 32, 32);
  epd.DisplayFrame();

  epd.Init(lut_partial_update);
}

void loop() {
  if (GAME_OVER) {
    if (!EndScreen) {
      delay(1000);
      epd.SetFrameMemory(ENDSCENE);
      epd.DisplayFrame();
      
      EndScreen = true;
    }
    
    return;
  }
  int state = digitalRead(JUMP_BUTTON);
  if (state == LOW) {
    if (i > M_PI) i = 0;
  }
  epd.ClearFrameMemory(0xFF);
  jump();
  updateCactus();

  epd.DisplayFrame();
}

int y;
void jump() {
  if (i < M_PI) {
    y = (sin(i) * 40) + 20;
    epd.SetFrameMemory(BACKGROUND);
    epd.SetFrameMemory(TEST, y, 5, 32, 32);
    i += 0.6;
  } else if (i >= M_PI) {
    epd.SetFrameMemory(BACKGROUND);
    epd.SetFrameMemory(TEST, 20, 5, 32, 32);
  }
}

int j = START;
void updateCactus() {
  epd.SetFrameMemory(CACTUS_SMALL, 23, j -= 10, 24, 16);
  if (j < 16) j = START;
  if (j < 40) {
    if (y - 32 < 24) GAME_OVER = true;
  }
}
