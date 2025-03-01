#include "../gbdk/include/gb/gb.h"

// Character structure: id, position, dimensions
typedef struct gamecharacter {
  UBYTE spriteid;
  UINT8 x;
  UINT8 y;
  UINT8 width;
  UINT8 height;
} gamecharacter;
