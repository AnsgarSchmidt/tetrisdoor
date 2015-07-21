// Based on code from Tuertris hacked by mclien
// https://hackaday.io/project/3708-trtris
// http://mclien.de/files/tuertris/tuertris_sw.zip
// Joel Lienhard  (joel@mclien.de  joel@sisam42.de)

#include "application.h"
#include "neopixel__spark_internet_button.h"

#include "tetrisDoor.h"
#include "gameOfLife.h"
#include "spock.h"
#include "tetris.h"
#include "fire.h"

static Adafruit_NeoPixel strip = Adafruit_NeoPixel( (MATRIX_X * MATRIX_Y) , PIN_PIXEL, PIXEL_TYPE);

void setup() 
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(PIN_UP,         INPUT);
  pinMode(PIN_DOWN,       INPUT);
  pinMode(PIN_LEFT,       INPUT);
  pinMode(PIN_RIGHT,      INPUT);
  pinMode(PIN_MODE,       INPUT);
  
  digitalWrite(PIN_UP,    HIGH);
  digitalWrite(PIN_DOWN,  HIGH);
  digitalWrite(PIN_LEFT,  HIGH);
  digitalWrite(PIN_RIGHT, HIGH);
  digitalWrite(PIN_MODE,  HIGH);
}

void checkend(){
  
  gameover = 0;
  
  for(int x = 0; x < MATRIX_X; x++){

    if(matrix[x][MATRIX_Y - 1] != 0){
      gameover = 1;
    }

  }
  
  if(gameover == 1){
  
    pos[0] = MATRIX_X-3;
    pos[1] = MATRIX_Y;
  
    for(int x = 0; x < MATRIX_X; x++){

      for(int y = 0; y < MATRIX_Y; y++){
        matrix[x][y] = 0;
      }

    }
    
    add(random(7));
    
    for(int x = 0; x < MATRIX_X; x++){

      for(int y = 0; y< MATRIX_Y; y++){
        matrix[x][y] = 0;
      }

    }

  }
  
}

void checkrows(){
  
  for(int y = 0; y < MATRIX_Y; y++){
    fullrow = 1;
    
    for(int x = 0; x < MATRIX_X; x++){

      if(matrix[x][y] == 0){
        fullrow = 0;
      }

    }
    
    if(fullrow == 1){

      for(int row = y; row < MATRIX_Y-1; row++){

        for(int x = 0; x < MATRIX_X; x++){
          matrix[x][row] = matrix[x][row+1];
        }

      }

      for(int x = 0; x < MATRIX_X; x++){
        matrix[x][MATRIX_Y-1] = 0;
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

    if(current[e][0] + pos[0] >= MATRIX_X ){
        moveable = 0;
    }

    if((current[e][1] + pos[1] - 1) < 0 ){
        moveable = 0;
    }

    if((current[e][1] + pos[1] - 1) >= MATRIX_Y ){
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

    if((current[e][0] + pos[0] + 1) >= MATRIX_X ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] < 0 ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] >= MATRIX_Y ){
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

    if((current[e][0] + pos[0] - 1) >= MATRIX_X ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] < 0 ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] >= MATRIX_Y ){
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

    if(current[e][0] + pos[0] >= MATRIX_X ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] < 0 ){
        moveable = 0;
    }

    if(current[e][1] + pos[1] >= MATRIX_Y ){
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
  
  pos[0] = MATRIX_X-3;
  pos[1] = MATRIX_Y;

}

void loop() {
    
    if(digitalRead(PIN_MODE) == LOW and debounce == 0){
      debounce = DEBOUNCE_VALUE;
      for(int x = 0; x < MATRIX_X; x++){
        for(int y = 0; y < MATRIX_Y; y ++){
            matrix[x][y] = 0;
        }  
      }
      //mode++;
      if (mode > MAX_MODE){
          mode = OFF;
      }
    }

	// OFF
	if(mode == OFF){
      for(int x = 0; x < MATRIX_X; x++){
        for(int y = 0; y < MATRIX_Y; y ++){
            matrix[x][y] = 0;
        }  
      }
      show(false);		
	}

    // RANDOM SPOCK Pattern
    if(mode == SPOCK){
    
      if(spockLastUpdate + SPOCK_WAIT < millis()){
      
        spockLastUpdate = millis();

        for(int x = 0; x < MATRIX_X; x++){
          for(int y = 0; y < MATRIX_Y; y++){
            matrix[x][y] = 0;
          }  
        }
        
        for(int n = 0; n < SPOCK_NUMBER_OF_LEDS; n++){
        	matrix[random(MATRIX_X)][random(MATRIX_Y)] = random(7);
        }
      }
      
      show(false);      
          
    }
    
    // Tetris
    if(mode == TETRIS){
        if(fallcount == fallspeed){
            fallcount = 0;
            down();
        }
        
        if(digitalRead(PIN_DOWN) == LOW and debounce == 0){
          debounce = DEBOUNCE_VALUE;
          down();
        }

        if(digitalRead(PIN_RIGHT) == LOW and debounce == 0){
          debounce = DEBOUNCE_VALUE;
          right();
        }
        
        if(digitalRead(PIN_LEFT) == LOW and debounce == 0){
          debounce = DEBOUNCE_VALUE;
          left();
        }
        
        if(digitalRead(PIN_UP) == LOW and debounce == 0){
          debounce = DEBOUNCE_VALUE; 
          rotate();
        }
       
        fallcount++;
        checkrows();
        checkend();
        show(true);
        delay(1);
    }
    
    // Game of life
    if(mode == GAME_OF_LIFE){
        for(int x = 0; x < MATRIX_X; x++){
          for(int y = 0; y < MATRIX_Y; y++){
            matrix[x][y] = 0;
          }  
        }
        
        matrix[2][3] = 2;
        matrix[3][2] = 4;
        
        show(false);        
    }
    
    // fire
    if(mode == FIRE){
        
    }    
    
    if(debounce != 0){
      debounce--;
    }

}

void show(bool tetris){

  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }

  for(int y = 0; y < MATRIX_Y; y++){

    for(int x = 0; x < MATRIX_X; x ++){

      if(matrix[x][y] != 0){

        if(y%2 == 0){
          strip.setPixelColor((y*MATRIX_X)+x,              strip.Color( colors[matrix[x][y]-1][0], colors[matrix[x][y]-1][1], colors[matrix[x][y]-1][2]) );
        }else{
          strip.setPixelColor((y*MATRIX_X)+(MATRIX_X-x-1), strip.Color( colors[matrix[x][y]-1][0], colors[matrix[x][y]-1][1], colors[matrix[x][y]-1][2]) );
        }

      }

    }

  }

  if (tetris) {

      for(int e = 0; e < 4; e++){
        truepos[0] = current[e][0]+pos[0];
        truepos[1] = current[e][1]+pos[1];

        if(truepos[1]%2 == 0){
          strip.setPixelColor((truepos[1]*MATRIX_X)+truepos[0],              strip.Color(colors[currentnum][0], colors[currentnum][1], colors[currentnum][2]) );
        }else{
          strip.setPixelColor((truepos[1]*MATRIX_X)+(MATRIX_X-truepos[0]-1), strip.Color(colors[currentnum][0], colors[currentnum][1], colors[currentnum][2]) );
        }

      }

  }

  strip.show();
}
