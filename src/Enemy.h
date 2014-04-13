#include "CubeAsset.h"
#include "Player.h"
#include "IInterpolator.h"

#ifndef Enemy_H_
#define Enemy_H_

class Enemy : public GameAsset {
 public:
  Enemy();
  Enemy(float x, float y, float z);
    ~Enemy();;

  bool collidesWith(Player & a);

  //virtual void spawn();
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

#endif // Enemy_H_
