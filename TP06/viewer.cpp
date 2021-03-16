#include "viewer.h"

#include <math.h>
#include <iostream>
#include "meshLoader.h"
#include <QTime>

using namespace std;


Viewer::Viewer(char *filename,const QGLFormat &format)
  : QGLWidget(format),
    _timer(new QTimer(this)),
    _currentshader(0),
    _light(glm::vec3(0,0,1)),
    _mode(false) {

  setlocale(LC_ALL,"C");

  _mesh = new Mesh(filename);
  _cam  = new Camera(_mesh->radius,glm::vec3(_mesh->center[0],_mesh->center[1],_mesh->center[2]));

  _timer->setInterval(10);
  connect(_timer,SIGNAL(timeout()),this,SLOT(updateGL()));
}

Viewer::~Viewer() {
  delete _timer;
  delete _mesh;
  delete _cam;

  for(unsigned int i=0;i<_shaders.size();++i) {
    delete _shaders[i];
  }

  deleteVAO();
  deleteTextures();
}

void Viewer::createTextures() {
  QImage image_color;
  
  // enable the use of 2D textures 
  glEnable(GL_TEXTURE_2D);

  // create one texture on the GPU
  glGenTextures(6,_texIds);

  // load an image (CPU side)
  image_color = QGLWidget::convertToGLFormat(QImage("textures/chesterfield-color.png"));

  // activate this texture (the current one)
  glBindTexture(GL_TEXTURE_2D,_texIds[0]);

  // set texture parameters 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  // transfer data from CPU to GPU memory
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,image_color.width(),image_color.height(),0,
  	       GL_RGBA,GL_UNSIGNED_BYTE,(const GLvoid *)image_color.bits());
  
  // =====================
  // generate mipmaps 
  glGenerateMipmap(GL_TEXTURE_2D);

  // ==================
  // Image Ambiant Occlusion
  QImage image_ao;

  // load an image (CPU side)
  image_ao = QGLWidget::convertToGLFormat(QImage("textures/chesterfield-ao.png"));

  // activate this texture (the current one)
  glBindTexture(GL_TEXTURE_2D,_texIds[1]);

  // set texture parameters 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  // transfer data from CPU to GPU memory
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,image_ao.width(),image_ao.height(),0,
  	       GL_RGBA,GL_UNSIGNED_BYTE,(const GLvoid *)image_ao.bits());

  // =====================
  // generate mipmaps 
  glGenerateMipmap(GL_TEXTURE_2D);

  // ==================
  // Image Specular
  QImage image_specular;

  // load an image (CPU side)
  image_specular = QGLWidget::convertToGLFormat(QImage("textures/chesterfield-specular.png"));

  // activate this texture (the current one)
  glBindTexture(GL_TEXTURE_2D,_texIds[2]);

  // set texture parameters 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  // transfer data from CPU to GPU memory
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,image_specular.width(),image_specular.height(),0,
  GL_RGBA,GL_UNSIGNED_BYTE,(const GLvoid *)image_specular.bits());

  // =====================
  // generate mipmaps 
  glGenerateMipmap(GL_TEXTURE_2D);

  // ==================
  // Image Specular
  QImage image_normal;

  // load an image (CPU side)
  image_normal = QGLWidget::convertToGLFormat(QImage("textures/chesterfield-normal.png"));

  // activate this texture (the current one)
  glBindTexture(GL_TEXTURE_2D,_texIds[3]);

  // set texture parameters 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  // transfer data from CPU to GPU memory
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,image_normal.width(),image_normal.height(),0,
  GL_RGBA,GL_UNSIGNED_BYTE,(const GLvoid *)image_normal.bits());

  // =====================
  // generate mipmaps 
  glGenerateMipmap(GL_TEXTURE_2D);

  // ==================
  // Image Environment Mapping
  QImage image_env;

  // load an image (CPU side)
  image_env = QGLWidget::convertToGLFormat(QImage("textures/env1.jpg"));

  // activate this texture (the current one)
  glBindTexture(GL_TEXTURE_2D,_texIds[4]);

  // set texture parameters 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  // transfer data from CPU to GPU memory
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,image_env.width(),image_env.height(),0,
  GL_RGBA,GL_UNSIGNED_BYTE,(const GLvoid *)image_env.bits());

  // =====================
  // generate mipmaps 
  glGenerateMipmap(GL_TEXTURE_2D);

  // ==================
  // Image Height
  QImage image_height;

  // load an image (CPU side)
  image_height = QGLWidget::convertToGLFormat(QImage("textures/chesterfield-height.png"));

  // activate this texture (the current one)
  glBindTexture(GL_TEXTURE_2D,_texIds[5]);

  // set texture parameters 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  // transfer data from CPU to GPU memory
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,image_height.width(),image_height.height(),0,
  GL_RGBA,GL_UNSIGNED_BYTE,(const GLvoid *)image_height.bits());

  // =====================
  // generate mipmaps 
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Viewer::deleteTextures() {
  glDeleteTextures(6,_texIds);
}

void Viewer::createVAO() {
  // create VAO
  glGenVertexArrays(1,&_vao);

  // create 5 associated VBOs (for positions, normals, tangents, coords and face indices)
  glGenBuffers(5,_buffers);

  // bind VAO 
  glBindVertexArray(_vao);
  
  // send and enable positions 
  glBindBuffer(GL_ARRAY_BUFFER,_buffers[0]);
  glBufferData(GL_ARRAY_BUFFER,_mesh->nb_vertices*3*sizeof(float),_mesh->vertices,GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void *)0);
  glEnableVertexAttribArray(0);

  // send and enable normals 
  glBindBuffer(GL_ARRAY_BUFFER,_buffers[1]);
  glBufferData(GL_ARRAY_BUFFER,_mesh->nb_vertices*3*sizeof(float),_mesh->normals,GL_STATIC_DRAW);
  glVertexAttribPointer(1,3,GL_FLOAT,GL_TRUE,0,(void *)0);
  glEnableVertexAttribArray(1);

  // send and enable tangents
  glBindBuffer(GL_ARRAY_BUFFER,_buffers[2]);
  glBufferData(GL_ARRAY_BUFFER,_mesh->nb_vertices*3*sizeof(float),_mesh->tangents,GL_STATIC_DRAW);
  glVertexAttribPointer(2,3,GL_FLOAT,GL_TRUE,0,(void *)0);
  glEnableVertexAttribArray(2);

  // send and enable coords 
  glBindBuffer(GL_ARRAY_BUFFER,_buffers[3]);
  glBufferData(GL_ARRAY_BUFFER,_mesh->nb_vertices*2*sizeof(float),_mesh->coords,GL_STATIC_DRAW);
  glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(void *)0);
  glEnableVertexAttribArray(3);

  // send faces 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_buffers[4]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,_mesh->nb_faces*3*sizeof(unsigned int),_mesh->faces,GL_STATIC_DRAW);

  // back to normal
  glBindVertexArray(0);
}

void Viewer::deleteVAO() {
  // delete your VAO here (function called in destructor)
  glDeleteBuffers(5,_buffers);
  glDeleteVertexArrays(1,&_vao);
}

void Viewer::drawVAO() {
  // activate the VAO, draw the associated triangles and desactivate the VAO
  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES,3*_mesh->nb_faces,GL_UNSIGNED_INT,(void *)0);
  glBindVertexArray(0);
}

void Viewer::createShaders() {
  // add your own shader files here 

  // *** texture mapping *** 
  _vertexFilenames.push_back("shaders/texture-mapping.vert");
  _fragmentFilenames.push_back("shaders/texture-mapping.frag");
  // ******************************
  _vertexFilenames.push_back("shaders/normal-mapping.vert");
  _fragmentFilenames.push_back("shaders/normal-mapping.frag");

  _vertexFilenames.push_back("shaders/environment-mapping.vert");
  _fragmentFilenames.push_back("shaders/environment-mapping.frag");

  _vertexFilenames.push_back("shaders/displacement-mapping.vert");
  _fragmentFilenames.push_back("shaders/displacement-mapping.frag");
}

void Viewer::enableShader(unsigned int shader) {
  // current shader ID 
  GLuint id = _shaders[shader]->id(); 

  // activate the current shader 
  glUseProgram(id);

  // send the model-view matrix 
  glUniformMatrix4fv(glGetUniformLocation(id,"mdvMat"),1,GL_FALSE,&(_cam->mdvMatrix()[0][0]));

  // send the projection matrix 
  glUniformMatrix4fv(glGetUniformLocation(id,"projMat"),1,GL_FALSE,&(_cam->projMatrix()[0][0]));

  // send the normal matrix (top-left 3x3 transpose(inverse(MDV))) 
  glUniformMatrix3fv(glGetUniformLocation(id,"normalMat"),1,GL_FALSE,&(_cam->normalMatrix()[0][0]));

  // send a light direction (defined in camera space)
  glUniform3fv(glGetUniformLocation(id,"light"),1,&(_light[0]));

  // send textures
  // Send Color
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,_texIds[0]);
  glUniform1i(glGetUniformLocation(id,"colormap"),0);

  // Send AO
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D,_texIds[1]);
  glUniform1i(glGetUniformLocation(id,"aomap"),1);

  // Send Specular
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D,_texIds[2]);
  glUniform1i(glGetUniformLocation(id,"specularmap"),2);

  // Send Normal
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D,_texIds[3]);
  glUniform1i(glGetUniformLocation(id,"normalmap"),3);

  // Send Env
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D,_texIds[4]);
  glUniform1i(glGetUniformLocation(id,"envmap"),4);

  // Send Height
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_2D,_texIds[5]);
  glUniform1i(glGetUniformLocation(id,"heightmap"),5);
}


void Viewer::disableShader() {
  // desactivate all shaders 
  glUseProgram(0);
}

void Viewer::paintGL() {
  // clear the color and depth buffers 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // tell the GPU to use this specified shader and send custom variables (matrices and others)
  enableShader(_currentshader);

  // actually draw the scene 
  drawVAO();

  // tell the GPU to stop using this shader 
  disableShader(); 
}

void Viewer::resizeGL(int width,int height) {
  _cam->initialize(width,height,false);
  glViewport(0,0,width,height);
  updateGL();
}

void Viewer::mousePressEvent(QMouseEvent *me) {
  const glm::vec2 p((float)me->x(),(float)(height()-me->y()));

  if(me->button()==Qt::LeftButton) {
    _cam->initRotation(p);
    _mode = false;
  } else if(me->button()==Qt::MidButton) {
    _cam->initMoveZ(p);
    _mode = false;
  } else if(me->button()==Qt::RightButton) {
    _light[0] = (p[0]-(float)(width()/2))/((float)(width()/2));
    _light[1] = (p[1]-(float)(height()/2))/((float)(height()/2));
    _light[2] = 1.0f-std::max(fabs(_light[0]),fabs(_light[1]));
    _light = glm::normalize(_light);
    _mode = true;
  } 

  updateGL();
}

void Viewer::mouseMoveEvent(QMouseEvent *me) {
  const glm::vec2 p((float)me->x(),(float)(height()-me->y()));
 
  if(_mode) {
    // light mode
    _light[0] = (p[0]-(float)(width()/2))/((float)(width()/2));
    _light[1] = (p[1]-(float)(height()/2))/((float)(height()/2));
    _light[2] = 1.0f-std::max(fabs(_light[0]),fabs(_light[1]));
    _light = glm::normalize(_light);
  } else {
    // camera mode
    _cam->move(p);
  }

  updateGL();
}

void Viewer::keyPressEvent(QKeyEvent *ke) {
  
  // key a: play/stop animation
  if(ke->key()==Qt::Key_A) {
    if(_timer->isActive()) 
      _timer->stop();
    else 
      _timer->start();
  }

  // key i: init camera
  if(ke->key()==Qt::Key_I) {
    _cam->initialize(width(),height(),true);
  }
  
  // key f: compute FPS
  if(ke->key()==Qt::Key_F) {
    int elapsed;
    QTime timer;
    timer.start();
    unsigned int nb = 500;
    for(unsigned int i=0;i<nb;++i) {
      paintGL();
    }
    elapsed = timer.elapsed();
    double t = (double)nb/((double)elapsed);
    cout << "FPS : " << t*1000.0 << endl;
  }

  // key r: reload shaders 
  if(ke->key()==Qt::Key_R) {
    for(unsigned int i=0;i<_vertexFilenames.size();++i) {
      _shaders[i]->reload(_vertexFilenames[i].c_str(),_fragmentFilenames[i].c_str());
    }
  }

  // space: next shader
  if(ke->key()==Qt::Key_Space) {
    _currentshader = (_currentshader+1)%_shaders.size();
  }

  updateGL();
}

void Viewer::initializeGL() {
  // make this window the current one
  makeCurrent();

  // init and chack glew
  glewExperimental = GL_TRUE;

  if(glewInit()!=GLEW_OK) {
    cerr << "Warning: glewInit failed!" << endl;
  }

  // init OpenGL settings
  glClearColor(0.0,0.0,0.0,1.0);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glViewport(0,0,width(),height());

  // initialize camera
  _cam->initialize(width(),height(),true);

  // load shader files
  createShaders();

  // init and load all shader files
  for(unsigned int i=0;i<_vertexFilenames.size();++i) {
    _shaders.push_back(new Shader());
    _shaders[i]->load(_vertexFilenames[i].c_str(),_fragmentFilenames[i].c_str());
  }

  // init the first shader 
  createVAO();
  createTextures();

  // starts the timer 
  _timer->start();
}

