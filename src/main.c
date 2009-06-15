#include "graphics/graphics.h"
#include "physics/physics.h"

int main(char argc, char* argv[])
{
  Physics_Init();
  Graphics_Init(640, 480, 0);
  return 0;
}
