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

  virtual void update();
  virtual void draw();
  virtual void clean();
  virtual void incDiff(double diff);

  void setInterpolator(shared_ptr<IInterpolator> li);
  
  double difficulty = 0.2;

  enum vertices {
	F0, F1, F2, F3, B4, B5, B6, B7,
  };
 
  private:
  shared_ptr<IInterpolator> li;

};

#endif // Enemy_H_
