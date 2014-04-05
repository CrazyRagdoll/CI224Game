#include "MovementInterpolator.h"

MovementInterpolator::MovementInterpolator(Vector3 move, int fps) {
  this->move = new Vector3(move);
  //this->G = new Vector3(0.0f, 0.0f, 0.0f);
  this->fps = fps;
  this->count = 1;
}

Vector3 MovementInterpolator::update() {
  count++;
  float time = ((float)count)/((float)fps);
  return (*move);
}
