#include "Enemy.h"

Enemy::Enemy() 
  : GameAsset(
	      string("shaders/hello-gl.v.glsl")
	      , string("shaders/hello-gl.f.glsl")
	      )
{
  Enemy(0, 0, 0);
}

Enemy::Enemy(float x, float y, float z) {
  this->li = nullptr;

  // A default "unit" cube
  num_vertices = 8;
  num_triangles = 12;
  g_vertex_buffer_data = new GLfloat[num_vertices * 3]{

  //     x      y     z
	-0.5, -0.5,  0.5, //F - 0
	 0.5, -0.5,  0.5, //F - 1
	-0.5,  0.5,  0.5, //F - 2
	 0.5,  0.5,  0.5, //F - 3
	-0.5, -0.5, -0.5, //B - 4
	 0.5, -0.5, -0.5, //B - 5
	-0.5,  0.5, -0.5, //B - 6
	 0.5,  0.5, -0.5  //B - 7
}; // three points per vertex

  g_element_buffer_data = new GLushort[num_triangles * 3]{

	F0, F1, F2,  //front
	F1, F3, F2,

	F1, B5, F3,  //right
	B5, B7, F3,

	B5, B4, B7,  //back
	B4, B6, B7,

	B4, F0, B6,  //left
	F0, F2, B6,

	F2, F3, B6,  //top
	F3, B7, B6,

	B4, B5, F0,  //bottom
	B5, F1, F0
	
}; // three vertices per triangle

  bbox.reset();
  bbox = shared_ptr<BoundingBox>(new BoundingBox(Point3(x, y, z), 1.0, 1.0, 1.0));

  make_resources();
}

Enemy::~Enemy() {
  // TODO: do something nice and fun here.
}

//Setting the difficulty of the game.
void Enemy::setDiff(double diff){
	difficulty = diff;
}

//Implementing a method to increase the difficulty of the game over time
/*void Enemy::incDiff(double diff){
	difficulty = difficulty + diff;
} */

//Implementing automatically moving enemy objects.
void Enemy::update() {

  if(isAlive) {

    shared_ptr<Point3> p = shared_ptr<Point3>(new Point3(this->bbox->getCentre()->getX(), this->bbox->getCentre()->getY(), this->bbox->getCentre()->getZ()-difficulty));

    this->bbox.reset();
    this->bbox = shared_ptr<BoundingBox>(new BoundingBox(*p, 1.0, 1.0, 1.0));
    if( this->bbox->getCentre()->getZ() < -10) { this->dead(); }
    //cout << difficulty << endl;

  }
  //else{ };
}

bool Enemy::collidesWith(Player & a) {
  return bbox->collidesWith((*a.bbox));
}

void Enemy::setInterpolator(shared_ptr<IInterpolator> li) {
  this->li.swap(li);
}

void Enemy::draw() {
//  std::cout << "x: " << bbox->getCentre()->getX() << "\ty: " << bbox->getCentre()->getY() << "\tz:" << bbox->getCentre()->getZ() << std::endl;
  GameAsset::draw();
}

void Enemy::clean() {}
