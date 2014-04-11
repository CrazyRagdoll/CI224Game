#include "CubeAsset.h"
#include "IInterpolator.h"

#ifndef PLAYER_H_
#define PLAYER_H_

class Player : public GameAsset {
 public:
  Player();
  Player(float x, float y, float z);
    ~Player();;

  virtual void update();
  virtual void draw();
  virtual void clean();
  void setInterpolator(shared_ptr<IInterpolator> li);

  enum vertices {
	F0, F1, F2, F3, B4, B5, B6, B7,
  };
 
  private:
  shared_ptr<IInterpolator> li;

};

#endif // PLAYER_H_
