#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

typedef struct {
  int move_forward;
  int move_backward;
  int move_left;
  int move_right;
  int turn_left;
  int turn_right;
} Input;

void getinput(Input *in, int *running);

#endif
