#ifndef TETRIS_DOOR_H
#define TETRIS_DOOR_H

#define PIN_PIXEL D0
#define PIN_MODE  D1
#define PIN_UP    D2
#define PIN_DOWN  D3
#define PIN_LEFT  D4
#define PIN_RIGHT D5

#define PIXEL_TYPE      WS2811
#define MATRIX_X             5
#define MATRIX_Y            10
#define DEBOUNCE_VALUE     100

uint8_t matrix[MATRIX_X][MATRIX_Y] = {0};

uint8_t debounce = 0;

typedef enum {OFF,
              SPOCK,
              TETRIS,
              GAME_OF_LIFE,
              FIRE,
              ////////
              MAX_MODE
             } MODES_e;
        
MODES_e mode = GAME_OF_LIFE;

#endif
