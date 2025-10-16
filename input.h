#ifndef INPUT_H
#define INPUT_H

typedef struct {
  int move_forward;
  int move_backward;
  int move_left;
  int move_right;
  int turn_left;
  int turn_right;
  int n_wall;
  int e_wall;
  int s_wall;
  int w_wall;
} Input;

void getinput(Input *in, int *running);

#endif
