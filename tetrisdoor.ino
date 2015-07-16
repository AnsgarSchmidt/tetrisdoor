#include "application.h"
#include "neopixel/neopixel.h"
#include "MQTT/MQTT.h"

#include "application.h"

#define PIXEL_PIN      D0
#define PIXEL_TYPE     WS2811

#define matrixx        10
#define matrixy         5
#define fallspeedorg  800

int pos[2]                   = { 5, 1};
int bricks[7][4][2]          = { {{0, 1}, {-1, 0}, {0, 0}, {1, 0}}, {{-1, 1}, {-1, 0}, {0, 0}, {1, 0}}, {{1, 1}, {-1, 0}, {0, 0}, {1, 0}}, {{-1, 0}, {0, 0}, {0, 1}, {1, 1}}, {{-1, 1}, {0, 1}, {0, 0}, {1, 0}}, {{1, 0}, {1, 1}, {0, 1}, {0, 0}}, {{-1, 0}, {0, 0}, {1, 0}, {2, 0}}};
int colors[7][3]             = { {255, 0, 0}, {100, 255, 0}, {0, 0, 255}, {255, 255, 0}, {0, 255, 255}, {255, 100, 0}, {255, 255, 255}};
int matrix[matrixx][matrixy] = { 0};
int leds[matrixx * matrixy]  = { 0};
int current[4][2]            = { {1, 1}, {-1, 0}, {0, 0}, {1, 0} };
int currentcach[4][2]        = { 0};
int currentnum               = 1;
int truepos[2]               = { 0,0};
int fallspeed                = fallspeedorg;
int fallcount                = 0;
int moveable                 = 0;
int fullrow                  = 0;
int rowtodelete              = 0;
int gameover                 = 0;
int mode                     = 0;
int currentColor[3]          = {0};

Adafruit_NeoPixel strip = Adafruit_NeoPixel((matrixx * matrixy), PIXEL_PIN, PIXEL_TYPE);

void callback(char* topic, byte* payload, unsigned int length);

MQTT client("ansinas", 1883, callback);

void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);

    if (message.equals("LEFT")){
        left();
    }

    if (message.equals("RIGHT")){
        right();
    }

    if (message.equals("DOWN")){
        down();
    }

    if (message.equals("ROTATE")){
        rotate();
    }

}

void setup() {
    
    strip.begin();

    for(int i = 0; i < strip.numPixels(); i++){
        strip.setPixelColor(i, strip.Color(1, 0, 0));
    }

    strip.show();
    
    client.connect("tetrisdoor");

    if (client.isConnected()) {
        client.publish("tetrisdoor","start");
        client.subscribe("tetrisdoor");
    }

}

void checkend(){
  
  gameover = 0;
  
  for(int x = 0; x < matrixx; x++){

    if(matrix[x][1] != 0){
      gameover = 1;
    }

  }
  
  if(gameover == 1){
  
    pos[0] = 5;
    pos[1] = 1;
  
    for(int x = 0; x < matrixx; x++){

      for(int y = 0; y < matrixy; y++){
        matrix[x][y] = 0;
      }

    }
    
    add(random(7));
    
    // ????????????????????????????????
    for(int x = 0; x < matrixx; x++){

      for(int y = 0; y< matrixy; y++){
        matrix[x][y] = 0;
      }

    }
    // ?????????????????????????????????
    
  }
  
}

void checkrows(){
  
  for(int y = 0; y < matrixy; y++){
    fullrow = 1;
    
    for(int x = 0; x < matrixx; x++){

      if(matrix[x][y] == 0){
        fullrow = 0;
      }

    }
    
    if(fullrow == 1){

      for(int row = y; row > 0; row--){

        for(int x = 0; x < matrixx; x++){
          matrix[x][row] = matrix[x][row-1];
        }

      }

    }
    
  }
  
}

void down(){
  moveable = 1;
  for(int e = 0; e < 4; e++){

    if(matrix[current[e][0]+pos[0]][current[e][1]+pos[1]+1] != 0){
      moveable = 0;
    }

    if(current[e][1]+pos[1]+1 == 35){
      moveable = 0;
    }

  }
  
  if(moveable == 1){
    pos[1]++;
  }else{
    add(random(7));
  }
}

void right(){
  moveable = 1;
  
  for(int e = 0; e < 4; e++){

    if(matrix[current[e][0]+pos[0]+1][current[e][1]+pos[1]] != 0){
      moveable = 0;
    }

    if(current[e][0]+pos[0]+1 == matrixx){
      moveable = 0;
    }

  }
  
  if(moveable == 1){
    pos[0]++;
  }
}

void left(){
  moveable = 1;
  
  for(int e = 0; e < 4; e++){

    if(matrix[current[e][0]+pos[0]-1][current[e][1]+pos[1]] != 0){
      moveable = 0;
    }

    if(current[e][0]+pos[0]-1 == -1){
      moveable = 0;
    }

  }
  
  if(moveable == 1){
    pos[0]--;
  }

}

void rotate(){
  
  for(int e = 0; e < 4; e++){
    currentcach[e][0] = current[e][0];
    currentcach[e][1] = current[e][1];
  }
  
  for(int e = 0; e < 4; e++){
    current[e][1] =  currentcach[e][0];
    current[e][0] = -currentcach[e][1];
  }
  
  moveable = 1;
  
  for(int e = 0; e < 4; e++){

    if(matrix[current[e][0]+pos[0]][current[e][1]+pos[1]] != 0){
      moveable = 0;
    }

    if(current[e][0]+pos[0] < 0){
      moveable = 0;
    }

    if(current[e][0]+pos[0] > 9){
      moveable = 0;
    }

  }
  
  if(moveable == 0){

    for(int e = 0; e < 4; e++){
      current[e][0] = currentcach[e][0];
      current[e][1] = currentcach[e][1];
    }

  }
  
}

void add(int b){

  for(int e = 0; e < 4; e++){
    matrix[current[e][0]+pos[0]][current[e][1]+pos[1]] = currentnum + 1;
    current[e][0] = bricks[b][e][0];
    current[e][1] = bricks[b][e][1];
  }

  currentnum = b;
  pos[0] = 5;
  pos[1] = 1;

}

void show(){

  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }

  for(int x = 0; x < matrixx; x++){

    for(int y = 0; y < matrixy; y ++){

      if(matrix[x][y] != 0){

        if(x%2 == 1){
          strip.setPixelColor((x*matrixy)+y,             strip.Color( colors[matrix[x][y]-1][0], colors[matrix[x][y]-1][1], colors[matrix[x][y]-1][2]) );
        }else{
          strip.setPixelColor((x*matrixy)+(matrixy-y-1), strip.Color( colors[matrix[x][y]-1][0], colors[matrix[x][y]-1][1], colors[matrix[x][y]-1][2]) );
        }
    
      }
    
    }
  
  }
  
  for(int e = 0; e < 4; e++){
    truepos[0] = current[e][0]+pos[0];
    truepos[1] = current[e][1]+pos[1];
  
    if(truepos[0]%2 == 1){
      strip.setPixelColor((truepos[0]*matrixy)+truepos[1],             strip.Color(colors[currentnum][0], colors[currentnum][1], colors[currentnum][2]) );
    }else{
      strip.setPixelColor((truepos[0]*matrixy)+(matrixy-truepos[1]-1), strip.Color(colors[currentnum][0], colors[currentnum][1], colors[currentnum][2]) );
    }
    
  }
  
  strip.show();
}

void loop() {
    
    if (client.isConnected()){
        client.loop();
    }
    
    if(mode == 0){
        
          for(int i = 0; i < strip.numPixels(); i++){
            strip.setPixelColor(i, strip.Color(currentColor[0], currentColor[1], currentColor[2]));
          }
          
    }
    
    if(mode == 1){
        if(fallcount == fallspeed){
            fallcount = 0;
            down();
        }
        
        fallcount++;
        checkrows();
        checkend();
        show();
        delay(100);
    }
}
