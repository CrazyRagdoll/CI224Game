#include "CubeAsset.h"
#include <SDL2/SDL.h>

#ifndef PLAYER_H_
#define PLAYER_H_

class Player : public GameAsset {
 public:
  Player();
  Player(float x, float y, float z);
    ~Player();;

  //Score
  virtual void incScore(double points);

  //Movement
  virtual void moveLeft();
  virtual void moveRight();

  virtual void update();
  virtual void draw();
  virtual void clean();

  enum vertices {
	F0, F1, F2, F3, 
  };
 
  private:
  double score = 0;

};

#endif // PLAYER_H_
