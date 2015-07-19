// Based on code from Tuertris hacked by mclien
// https://hackaday.io/project/3708-trtris
// http://mclien.de/files/tuertris/tuertris_sw.zip
// Joel Lienhard  (joel@mclien.de  joel@sisam42.de)


#include "application.h"
#include "neopixel__spark_internet_button/neopixel__spark_internet_button.h"

#define PIXEL_PIN      D0
#define MODE_PIN       D1
#define LEFT           D2
#define RIGHT          D3
#define DOWN           D4
#define ROTATE         D5

#define MAX_MODE        2

#define PIXEL_TYPE     WS2811
#define matrixx         5
#define matrixy        10
#define fallspeedorg  100
#define debouncevalue 100

int pos[2]                   = { matrixx-3, matrixy};
int bricks[7][4][2]          = { {{ 0,-1}, {-1, 0}, {0, 0}, {1, 0}},  // 0
                                 {{-1,-1}, {-1, 0}, {0, 0}, {1, 0}},  // 1
                                 {{ 1,-1}, {-1, 0}, {0, 0}, {1, 0}},  // 2
                                 {{-1, 0}, { 0, 0}, {0,-1}, {1,-1}},  // 3
                                 {{-1,-1}, { 0,-1}, {0, 0}, {1, 0}},  // 4
                                 {{ 1, 0}, { 1,-1}, {0,-1}, {0, 0}},  // 5
                                 {{-1, 0}, { 0, 0}, {1, 0}, {2, 0}}}; // 6
int colors[7][3]             = { {255, 0, 0}, {100, 255, 0}, {0, 0, 255}, {255, 255, 0}, {0, 255, 255}, {255, 100, 0}, {255, 255, 255}};
int matrix[matrixx][matrixy] = { 0};
int current[4][2]            = {  {-1,-1}, {-1, 0}, {0, 0}, {1, 0} };
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
int debounce                 = 0;
int currentColor[3]          = {0};

Adafruit_NeoPixel strip = Adafruit_NeoPixel( (matrixx * matrixy) , PIXEL_PIN, PIXEL_TYPE);

void setup() 
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(ROTATE,       INPUT);
  pinMode(DOWN,         INPUT);
  pinMode(LEFT,         INPUT);
  pinMode(RIGHT,        INPUT);
  pinMode(MODE_PIN,     INPUT);
  digitalWrite(ROTATE,  HIGH);
  digitalWrite(DOWN,    HIGH);
  digitalWrite(LEFT,    HIGH);
  digitalWrite(RIGHT,   HIGH);
  digitalWrite(MODE_PIN, HIGH);
}

void checkend(){
  
  gameover = 0;
  
  for(int x = 0; x < matrixx; x++){

    if(matrix[x][matrixy-1] != 0){
      gameover = 1;
    }

  }
  
  if(gameover == 1){
  
    pos[0] = matrixx-3;
    pos[1] = matrixy;
  
    for(int x = 0; x < matrixx; x++){

      for(int y = 0; y < matrixy; y++){
        matrix[x][y] = 0;
      }

    }
    
    add(random(7));
    
    for(int x = 0; x < matrixx; x++){

      for(int y = 0; y< matrixy; y++){
        matrix[x][y] = 0;
      }

    }

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

      for(int row = y; row < matrixy-1; row++){

        for(int x = 0; x < matrixx; x++){
          matrix[x][row] = matrix[x][row+1];
        }

      }

      for(int x = 0; x < matrixx; x++){
        matrix[x][matrixy-1] = 0;
      }

    }
    
  }
  
}

void down(){
  moveable = 1;

  for(int e = 0; e < 4; e++){

    if(current[e][0] + pos[0] < 0 ){
        moveable = 0;
    }

    if(current[e][0] + pos[0] >= matrixx ){
        moveable = 0;
    }

    if((current[e][1] + pos[1] - 1) < 0 ){
        moveable = 0;
    }

    if((current[e][1] + pos[1] - 1) >= matrixy ){
        moveable = 0;
   } 

    if(moveable && (matrix[current[e][0] + pos[0]][current[e][1] + pos[1] - 1] != 0)) {
      moveable = 0;
    }

  }
  
  if(moveable){
    pos[1]--;
  }else{
    add(random(7));
  }

}

void right(){
    
  moveable = 1;
  
  for(int e = 0; e < 4; e++){

    if((current[e][0] + pos[0] + 1) < 0 ){
        moveable = 0;
    }

    if((current[e][0] + pos[0] + 1) >= matrixx ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] < 0 ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] >= matrixy ){
        moveable = 0;
   } 

    if(moveable && matrix[current[e][0]+pos[0] + 1][current[e][1]+pos[1]] != 0){
      moveable = 0;
    }

  }
  
  if(moveable){
    pos[0]++;
  }
}

void left(){
  moveable = 1;
  
  for(int e = 0; e < 4; e++){

    if((current[e][0] + pos[0] - 1) < 0 ){
        moveable = 0;
    }

    if((current[e][0] + pos[0] - 1) >= matrixx ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] < 0 ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] >= matrixy ){
        moveable = 0;
   } 


    if(moveable && matrix[current[e][0]+pos[0] - 1][current[e][1]+pos[1]] != 0){
      moveable = 0;
    }

  }
  
  if(moveable){
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

    if(current[e][0] + pos[0] < 0 ){
        moveable = 0;
    }

    if(current[e][0] + pos[0] >= matrixx ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] < 0 ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] >= matrixy ){
        moveable = 0;
   } 

    if(moveable && matrix[current[e][0]+pos[0]][current[e][1]+pos[1]] != 0){
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
  
  pos[0] = matrixx-3;
  pos[1] = matrixy;

}

void show(bool tetris){

  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }

  for(int y = 0; y < matrixy; y++){

    for(int x = 0; x < matrixx; x ++){

      if(matrix[x][y] != 0){

        if(y%2 == 0){
          strip.setPixelColor((y*matrixx)+x,             strip.Color( colors[matrix[x][y]-1][0], colors[matrix[x][y]-1][1], colors[matrix[x][y]-1][2]) );
        }else{
          strip.setPixelColor((y*matrixx)+(matrixx-x-1), strip.Color( colors[matrix[x][y]-1][0], colors[matrix[x][y]-1][1], colors[matrix[x][y]-1][2]) );
        }
    
      }
    
    }
  
  }

  if (tetris) {
  
      for(int e = 0; e < 4; e++){
        truepos[0] = current[e][0]+pos[0];
        truepos[1] = current[e][1]+pos[1];
      
        if(truepos[1]%2 == 0){
          strip.setPixelColor((truepos[1]*matrixx)+truepos[0],             strip.Color(colors[currentnum][0], colors[currentnum][1], colors[currentnum][2]) );
        }else{
          strip.setPixelColor((truepos[1]*matrixx)+(matrixx-truepos[0]-1), strip.Color(colors[currentnum][0], colors[currentnum][1], colors[currentnum][2]) );
        }
        
      }
  
  }
  
  strip.show();
}

void loop() {
    
    if(digitalRead(MODE_PIN) == LOW and debounce == 0){
      debounce = debouncevalue;
      mode++;
      if (mode > MAX_MODE){
          mode = 0;
      }
    }

    // RANDOM SPOCK Pattern
    if(mode == 0){
      for(int x = 0; x < matrixx; x++){
        for(int y = 0; y < matrixy; y ++){
            matrix[x][y] = 0;
        }  
      }
      
      matrix[4][2] = 1;
      show(false);
    }
    
    // Tetris
    if(mode == 1){
        if(fallcount == fallspeed){
            fallcount = 0;
            down();
        }
        
        if(digitalRead(DOWN) == LOW and debounce == 0){
          debounce = debouncevalue;
          down();
        }

        if(digitalRead(RIGHT) == LOW and debounce == 0){
          debounce = debouncevalue;
          right();
        }
        
        if(digitalRead(LEFT) == LOW and debounce == 0){
          debounce = debouncevalue;
          left();
        }
        
        if(digitalRead(ROTATE) == LOW and debounce == 0){
          debounce = debouncevalue; 
          rotate();
        }
       
        fallcount++;
        checkrows();
        checkend();
        show(true);
        delay(1);
    }
    
    // Game of life
    if(mode == 2){
        
    }
    
    if(debounce != 0){
      debounce--;
    }

}