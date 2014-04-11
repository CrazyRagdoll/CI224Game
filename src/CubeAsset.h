#include "GameAsset.h"
#include "IInterpolator.h"

#ifndef CUBEASSET_H_
#define CUBEASSET_H_

class CubeAsset : public GameAsset {
 public:
  CubeAsset();
  CubeAsset(float x, float y, float z);
    ~CubeAsset();

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

#endif // CUBEASSET_H_
