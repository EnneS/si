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
  _points = std::vector<Point>(10);

  for(unsigned int i = 0; i < _points.size(); i++){
    _points[i].pos = randv2(-1, 1);
    _points[i].dir = randv2(-0.01,0.01);
    _points[i].col = randv3(0, 1);
    _points[i].size = randf(10,20);
  }
}

void Viewer::updatePointPositions() {
  for(unsigned int i = 0; i < _points.size(); i++) {
    _points[i].pos += _points[i].dir;

    int r =_points[i].isDescreasing ? -1 : 1;

    _points[i].size +=  _points[i].size * r * 0.04;

    if(_points[i].size >= 100 || _points[i].size <= 10){
      _points[i].isDescreasing = !_points[i].isDescreasing;
    }

    if(_points[i].size < 10) {
      _points[i].size = 15;
      _points[i].isDescreasing = false;
    }
    
    if(_points[i].pos.x > 1 || _points[i].pos.x < -1 ) {
      _points[i].dir.x =  -_points[i].dir.x;
      _points[i].rebond = true;
    } else if( _points[i].rebond) {
      _points[i].rebond = false;
      // _points[i].size *= 1.5;
    }

    if(_points[i].pos.y > 1 || _points[i].pos.y < -1 ) {
      _points[i].dir.y = -_points[i].dir.y;
      _points[i].rebond = true;
    } else if( _points[i].rebond) {
      _points[i].rebond = false;
      // _points[i].size *= 0.5;
    }
  }
}

void Viewer::paintGL() {
  // clear color and depth buffers 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO
  for(unsigned int i = 0; i < _points.size(); i++){
    glPointSize(_points[i].size);
    
    glBegin(GL_POINTS);
    glColor3f(_points[i].col.x, _points[i].col.y, _points[i].col.z);
    glVertex3f(_points[i].pos.x, _points[i].pos.y, 0);
    glEnd();
  }

  updatePointPositions();
}

void Viewer::resizeGL(int width,int height) {
  // TODO 
  glViewport(0,0, width, height);

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
  glClearColor(0.0,0.0,0.0,1.0);
  glDisable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  // initialize points 
  generateRandomPoints();
  
  glEnable(GL_POINT_SMOOTH);
  
  // starts the timer 
  _timer.start();
}

