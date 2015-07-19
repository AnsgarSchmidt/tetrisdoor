#ifndef TETRIS_H
#define TETRIS_H

#define FALL_SPEED 100


int8_t bricks[7][4][2]             = { {{ 0,-1}, {-1, 0}, {0, 0}, {1, 0}},  // 0
                                       {{-1,-1}, {-1, 0}, {0, 0}, {1, 0}},  // 1
                                       {{ 1,-1}, {-1, 0}, {0, 0}, {1, 0}},  // 2
                                       {{-1, 0}, { 0, 0}, {0,-1}, {1,-1}},  // 3
                                       {{-1,-1}, { 0,-1}, {0, 0}, {1, 0}},  // 4
                                       {{ 1, 0}, { 1,-1}, {0,-1}, {0, 0}},  // 5
                                       {{-1, 0}, { 0, 0}, {1, 0}, {2, 0}}}; // 6
uint8_t colors[7][3]               = { {255, 0, 0}, {100, 255, 0}, {0, 0, 255}, {255, 255, 0}, {0, 255, 255}, {255, 100, 0}, {255, 255, 255}};
uint8_t pos[2]                     = { MATRIX_X - 3, MATRIX_Y};
int8_t current[4][2]               = {  {-1,-1}, {-1, 0}, {0, 0}, {1, 0} };
int8_t currentcach[4][2]           = { 0};
uint8_t currentnum                 = 1;
int8_t truepos[2]                  = { 0,0};
uint8_t fallspeed                  = FALL_SPEED;
uint8_t fallcount                  = 0;
bool moveable                      = 0;
bool fullrow                       = 0;
bool rowtodelete                   = 0;
bool gameover                      = 0;
uint8_t currentColor[3]            = {0};



#endif
