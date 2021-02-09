#include "viewer.h"

#include <math.h>
#include <iostream>

using namespace std;

Viewer::Viewer(const QGLFormat &format,const QString &)
  : QGLWidget(format),
    _timer(new QTimer(this)),
    _quadric(NULL),
    _drawMode(false),
    _spin(0.0f),
    _speed(0.05f) {

  _timer->setInterval(10);
  connect(_timer,SIGNAL(timeout()),this,SLOT(updateGL()));
}

Viewer::~Viewer() {
  delete _timer;
  gluDeleteQuadric(_quadric);
}

void Viewer::drawSphere(const glm::vec3 &color) {
  glColor3fv(&color[0]);
  gluSphere(_quadric,1.0f,20,20);
}

void Viewer::setMatrices(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) {
  // tells OpenGL to use these model/view/projection matrices 
  glm::mat4 mdv = view*model;

  glMatrixMode(GL_PROJECTION);  // go to the projection mode 
  glLoadMatrixf(&projection[0][0]);  // load the corresponding matrix
  
  glMatrixMode(GL_MODELVIEW); // go to the modelview mode 
  glLoadMatrixf(&mdv[0][0]); // load the corresponding matrix

  // note that model and view matrices are combined here, because 
  // the view can actually be placed in the first level of the stack. 
  // this is the OpenGL way of manipulating matrices 
}

void Viewer::drawScene() {
  // copy the previous matrix
  _modelMatrices.push(_modelMatrices.top());
  

  // eventually, apply some transformationss to the current (top) model matrix
  // ... 
  _modelMatrices.top() = glm::rotate(_modelMatrices.top(), _spin/7, glm::vec3(0,1,0));

  // we will use these 3 matrices for the next drawings
  setMatrices(_modelMatrices.top(),_viewMatrix,_projMatrix);
  // THE SUN : draw something (a sphere centered at 0 with radius 1)
  drawSphere(glm::vec3(1.0f,1.0f,0.0f));
  _modelMatrices.pop();

  _modelMatrices.push(_modelMatrices.top()); // On se met sur le plan du soleil

  //size of the sphere
  float size = 0.6;
  // Draw a planet
  _modelMatrices.top() = glm::rotate(_modelMatrices.top(), -_spin/4, glm::vec3(0,1,0)); // Rotation autour du soleil 
  _modelMatrices.top() = glm::translate(_modelMatrices.top(), glm::vec3(4, 0, 0));
  _modelMatrices.top() = glm::scale(_modelMatrices.top(),glm::vec3(size, size, size));
  _modelMatrices.top() = glm::rotate(_modelMatrices.top(), _spin/4, glm::vec3(0,1,0)); // Rotation autour du soleil 
  setMatrices(_modelMatrices.top(),_viewMatrix,_projMatrix);
  drawSphere(glm::vec3(0.0f,1.0f,0.0f));
  _modelMatrices.push(_modelMatrices.top()); // On se positionne sur le plan de la planète

  // Satellite 1
  size = 0.4;
  _modelMatrices.top() = glm::rotate(_modelMatrices.top(), -_spin/4, glm::vec3(0,1,0)); // Rotation autour de la planète 
  _modelMatrices.top() = glm::translate(_modelMatrices.top(), glm::vec3(4, 0, 0));
  _modelMatrices.top() = glm::scale(_modelMatrices.top(),glm::vec3(size, size, size));
  setMatrices(_modelMatrices.top(),_viewMatrix,_projMatrix);
  drawSphere(glm::vec3(1.0f,0.0f,0.0f));
  _modelMatrices.pop(); // On  se remet sur le plan de la planète

  // Satellite 2 
  size = 0.4;
  _modelMatrices.top() = glm::rotate(_modelMatrices.top(), -_spin/3, glm::vec3(0,0,1)); // Rotation autour de la planète  
  _modelMatrices.top() = glm::translate(_modelMatrices.top(), glm::vec3(3, 2, 0));
  _modelMatrices.top() = glm::scale(_modelMatrices.top(),glm::vec3(size, size, size));
  setMatrices(_modelMatrices.top(),_viewMatrix,_projMatrix);
  drawSphere(glm::vec3(0.5f,0.5f,0.5f));



  // back to the previous model matrix state
  _modelMatrices.pop();
}

void Viewer::paintGL() {
  // clear the color and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set the viewport (covering the entire window)
  glViewport(0,0,width()/2,height()/2);
  glm::vec3 pos = glm::vec3(0, 6, 6); // Position de la caméra
  glm::vec3 center = glm::vec3(0,0,0); // Là où la caméra regarde : centre de la sphère
  glm::vec3 up = glm::vec3(0,1,0);

  // initialize view and projection matrices here
  _viewMatrix =  glm::lookAt(pos, center, up);
  _projMatrix = glm::perspective(45.0f, (float)width()/(float)height(), 0.1f, 20.0f);

  // draw the scene 
  drawScene();

  // set the viewport (covering the entire window)
  glViewport(width()/2,height()/2,width()/2,height()/2);
  pos = glm::vec3(6, 6, 6); // Position de la caméra
  center = glm::vec3(0,0,0); // Là où la caméra regarde : centre de la sphère
  up = glm::vec3(0,1,0);
  _viewMatrix =  glm::lookAt(pos, center, up);
  _projMatrix = glm::perspective(45.0f, (float)width()/(float)height(), 0.1f, 20.0f);
  drawScene();

  // set the viewport (covering the entire window)
  glViewport(0,height()/2,width()/2,height()/2);
  pos = glm::vec3(6, 0, 6); // Position de la caméra
  center = glm::vec3(0,0,0); // Là où la caméra regarde : centre de la sphère
  up = glm::vec3(0,1,0);
  _viewMatrix =  glm::lookAt(pos, center, up);
  _projMatrix = glm::perspective(80.0f, (float)width()/(float)height(), 0.1f, 20.0f);
  drawScene();

  // set the viewport (covering the entire window)
  glViewport(width()/2,0,width()/2,height()/2);
  pos = glm::vec3(6, 6, 0); // Position de la caméra
  center = glm::vec3(0,0,0); // Là où la caméra regarde : centre de la sphère
  up = glm::vec3(0,1,0);
  _viewMatrix =  glm::lookAt(pos, center, up);
  int div = 100;
  _projMatrix = glm::ortho((float)-width()/div,(float)width()/div,(float)-height()/div,(float)height()/div,0.1f, 20.0f);
  drawScene();

  

  // the _spin variable may be used to animate spheres
  _spin = fmod(_spin+_speed,360.0f);
}

void Viewer::keyPressEvent(QKeyEvent *ke) {
  
  // key w: wire/filled
  if(ke->key()==Qt::Key_W) {
    if(!_drawMode) 
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else 
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    _drawMode = !_drawMode;
  } 

  // key a: play/stop animation
  if(ke->key()==Qt::Key_A) {
    if(_timer->isActive()) 
      _timer->stop();
    else 
      _timer->start();
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
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  // init the glu quadric for drawing spheres
  _quadric = gluNewQuadric();

  // init model,view and projection with identity matrices
  _modelMatrices.push(glm::mat4(1.0f));
  _viewMatrix = glm::mat4(1.0f);
  _projMatrix = glm::mat4(1.0f);
  
  // starts the timer 
  _timer->start();
}

