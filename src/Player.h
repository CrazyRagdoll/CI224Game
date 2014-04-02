#include "GameAsset.h"

#ifndef PLAYER_H_
#define PLAYER_H_

class Player : public Player {
 public:
  Player();
  Player(float x, float y, float z);
    ~Player();

  virtual void update();
  virtual void draw();
  void setInterpolator(shared_ptr<IInterpolator> li);
 
  private:
  shared_ptr<IInterpolator> li;

};

#endif // PLAYER_H_
