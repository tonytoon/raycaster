#ifndef INPUT_H
#define INPUT_H

typedef struct {
  int move_forward;
  int move_backward;
  int move_left;
  int move_right;
  int turn_left;
  int turn_right;
  int change_sector;
} Input;

void getinput(Input *in, int *running);

#endif
