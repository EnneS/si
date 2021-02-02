#include "viewer.h"

#include <math.h>
#include <iostream>

using namespace std;

Viewer::Viewer(const QGLFormat &format,const QString &)
  : QGLWidget(format),
    _timer(this) {

  _timer.setInterval(10);
  connect(&_timer,SIGNAL(timeout()),this,SLOT(updateGL()));
}

void Viewer::generateRandomPoints() {
  // TODO
}

void Viewer::updatePointPositions() {
  // TODO 
}

void Viewer::paintGL() {
  // clear color and depth buffers 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // TODO
}

void Viewer::resizeGL(int width,int height) {
  // TODO 

}

void Viewer::keyPressEvent(QKeyEvent *ke) {
  
  // key a: play/stop animation
  if(ke->key()==Qt::Key_A) {
    if(_timer.isActive()) 
      _timer.stop();
    else 
      _timer.start();
  }
  

  updateGL();
}

void Viewer::initializeGL() {
  // make this window the current one
  makeCurrent();

  // init and chack glew
  if(glewInit()!=GLEW_OK) {
    cerr << "Warning: glewInit failed!" << endl;
  }

  if(!GLEW_ARB_vertex_program   ||
     !GLEW_ARB_fragment_program ||
     !GLEW_ARB_texture_float    ||
     !GLEW_ARB_draw_buffers     ||
     !GLEW_ARB_framebuffer_object) {
    cerr << "Warning: Shaders not supported!" << endl;
  }

  // init OpenGL settings
  glClearColor(0.0,0.0,1.0,1.0);
  glDisable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  // initialize points 
  generateRandomPoints();
  
  // starts the timer 
  _timer.start();
}

