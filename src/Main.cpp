#define GLEW_STATIC // Easier debugging
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <list>
#include <time.h>
#include <unistd.h>

#include "GameAsset.h"
#include "Md2Asset.h"
#include "TriangularPyramidAsset.h"
#include "Player.h"
#include "Enemy.h"
#include "BallisticInterpolator.h"
#include "MovementInterpolator.h"
#include "Camera.h"

using namespace std;

#define RUN_GRAPHICS_DISPLAY 0x00;

//Adding the player to the game
shared_ptr<Player> player ;

string filename = "data/ogre.md2";

//Adding the assets and enemy assets into the game
vector<shared_ptr<GameAsset>> assets;
vector<shared_ptr<Enemy>> enemies;

//Crude Enemy counter to help regulate enemy spawn rates.
int EnemyCount = 30;

clock_t t;

bool horrible_global_go = false;

SDL_Window * window = nullptr;

/*
 * SDL timers run in separate threads.  In the timer thread
 * push an event onto the event queue.  This event signifies
 * to call display() from the thread in which the OpenGL 
 * context was created.
 */
Uint32 display(Uint32 interval, void *param) {
	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user.code = RUN_GRAPHICS_DISPLAY;
	event.user.data1 = 0;
	event.user.data2 = 0;
	SDL_PushEvent(&event);
	return interval;
}

void display() {
  glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  //Increase EnemyCount by 1
  EnemyCount++;

  //Calculating the time to deal with difficulty 
  t = clock();

  //If the player is alive spawn enemies every time the count increments by 20.
  if(player->isAlive)
  {
    if(fmod(EnemyCount, 40) == 0 )
    {
      //get the position of the player to spawn the enemies infront of him but 25 blocks 		back.
      int pos  = int(player->bbox->getCentre()->getX());
      for( int n_2 = 0; n_2 <= 8; n_2++ )
      {
	int rnd = rand() % 60 - 30;
	enemies.push_back(shared_ptr<Enemy> (new Enemy(pos + rnd, 0, 25)));
      }
    }
  }
  
  //Setting different difficulty values over time to make the game harder the longer you 	play.
  //for(double d = 0.1; d <= 4; d += 0.001){
	//for(auto it : enemies){ it->setDiff(d); }}}


  if(((float)t/CLOCKS_PER_SEC) < 1){
	for(auto it : enemies){ it->setDiff(0.15);}}
  if(((float)t/CLOCKS_PER_SEC) > 1 && ((float)t/CLOCKS_PER_SEC) < 2){ 
	for(auto it : enemies){ it->setDiff(0.175);}}
  if(((float)t/CLOCKS_PER_SEC) > 2 && ((float)t/CLOCKS_PER_SEC) < 4){ 
	for(auto it : enemies){ it->setDiff(0.2);}}
  if(((float)t/CLOCKS_PER_SEC) > 4){  
	for(auto it : enemies){ it->setDiff(0.225);}} 

  // This O(n + n^2 + n) sequence of loops is written for clarity,
  // not efficiency
  for(auto it : assets)  { it->update(); } 
  for(auto it : enemies) { if(it->isItAlive()){ it->update(); }}
  player->update();

  //Collision detection between the player and enemies.
  for(auto i : enemies) { 
      if( i->collidesWith(*player)){
	player->dead();
    }
  }

  for(auto it : assets)  { it->draw(); } 
  for(auto it : enemies) { if(it->isItAlive()){ it->draw(); }}
  player->draw();

  //delete dead enemies
  for(auto it : enemies)
  {
    if(!it->isItAlive())
    {
      it.reset();
    }	
  }
  
  // Don't forget to swap the buffers
  SDL_GL_SwapWindow(window);
}

int main(int argc, char ** argv) {
	SDL_Surface * surf;
	Uint32 width = 640;
	Uint32 height = 480;
	Uint32 colour_depth = 16; // in bits
	Uint32 delay = 1000/60; // in milliseconds

	// Initialise SDL - when using C/C++ it's common to have to
	// initialise libraries by calling a function within them.
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)<0) {
			cout << "Failed to initialise SDL: " << SDL_GetError() << endl;
			SDL_Quit();
	}

	// When we close a window quit the SDL application
	atexit(SDL_Quit);

	// Create a new window with an OpenGL surface
	window = SDL_CreateWindow("CI224 - Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (nullptr == window) {
		cout << "Failed to create SDL window: " << SDL_GetError() << endl;
		SDL_Quit();
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (nullptr == glContext) {
		cout << "Failed to create OpenGL context: " << SDL_GetError() << endl;
		SDL_Quit();
	}

	// Initialise GLEW - an easy way to ensure OpenGl 2.0+
	// The *must* be done after we have set the video mode - otherwise we have no OpenGL context.
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0")) {
	  cerr<< "OpenGL 2.0 not available" << endl;
	  return 1;
	}

	/*shared_ptr<TriangularPyramidAsset> p = shared_ptr<TriangularPyramidAsset> (new TriangularPyramidAsset(0, 0, 0));	
	shared_ptr<IInterpolator> i = shared_ptr<IInterpolator>(new BallisticInterpolator(Vector3(0.0, 10.0, 0), 60));
	p->setInterpolator(i);
	assets.push_back(p); */

	//Adding the players coordinates into the game
	player = shared_ptr<Player> (new Player(0, 0, 0));
	
	//Using lists to create multiple enemies.
	//for( int n = 0; n < 5; n++)
	//{
	//	list<shared_ptr<Enemy>> Enemies ( n, shared_ptr<Enemy> (new Enemy(n,0,10)));
	//	assets.push_back(Enemies);
	//}
	
	//Using a for loop to create a bunch of randomly generated enemies.
	/*if(player->isAlive)
	{
	for( int n = 10; n <= 100; n+= 5 )
	{
	    int pos = int(player->bbox->getCentre()->getZ());
	    for( int n_2 = 0; n_2 <= 5; n_2++ )
	    {
		int rnd = rand() % 40 - 20;
		enemies.push_back(shared_ptr<Enemy> (new Enemy(pos + rnd, 0, n)));
	    }
	} 
	} */

	//assets.push_back(shared_ptr<Md2Asset> (new Md2Asset(filename)));

	// Set the camera to be looking down at the player and give them a good field of view
	//Camera::getInstance().lookAt(Point3(0.0, 0.0, 10.0), Point3(0.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0));
	Camera::getInstance().setCamera(Camera::getInstance().getCameraM() * Matrix4::translation(Vector3(0.0, -2.0, 5.0)));
	display();
	//Camera::getInstance().setCamera(Matrix4::identity());

	// Call the function "display" every delay milliseconds
	SDL_AddTimer(delay, display, NULL);

	// Add the main event loop
	SDL_Event event;
	while (SDL_WaitEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
			  SDL_Quit();
			  break;
			case SDL_USEREVENT:
			  display();
			  break;
			case SDL_KEYUP:
			  //			  Camera::getInstance().setCamera(Matrix4::identity());
			  break;
			case SDL_KEYDOWN:
			  Matrix4 camera = Camera::getInstance().getCameraM();
			  switch(event.key.keysym.sym){
			case SDLK_UP:
			    Camera::getInstance().setCamera(camera * Matrix4::translation(Vector3(0.0, 0.0, -5.0)));
			    break; 
			  case SDLK_DOWN:
			    Camera::getInstance().setCamera(camera * Matrix4::translation(Vector3(0.0, 0.0, 5.0))); 
			    break;
			  case SDLK_LEFT:
			{
				//Introducing similtanious player and camera movement.
				Camera::getInstance().setCamera(camera * Matrix4::translation(Vector3(0.5, 0.0, 0.0)) ); 
				shared_ptr<Point3> mLeft = player->bbox->getCentre();
				*mLeft = Point3(mLeft->getX() - 0.5, 0.0, 0.0);
			}		 
			    break;
			  case SDLK_RIGHT:
			{
				//Introducing similtanious player and camera movement.
			    	Camera::getInstance().setCamera(camera * Matrix4::translation(Vector3(-0.5, 0.0, 0.0)) );
				shared_ptr<Point3> mRight = player->bbox->getCentre();
				*mRight = Point3(mRight->getX() + 0.5, 0.0, 0.0);
			}	
			    break;
			  case SDLK_g:

			  default:
			    break;
			  }
			  break;
			}
	}
}
