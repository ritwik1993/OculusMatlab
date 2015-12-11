/************************************************************************************

 Authors     :   Ritwik Ummalaneni <ritwik1993@gmail.com>
 Copyright   :   Copyright Ritwik Ummalaneni. All Rights reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 ************************************************************************************/
#include "Common.h"

// A lot of vertex definitions in this file look weird - this is because a few different functions of OpenGL use contrasting coordinate systems

// Vertex representation of the gantry
static const GLfloat g_vertex_buffer_data[] = {
  650.0f/960.0f - 1.0f, 370.0f/540.0f - 1.0f, 0.0f,
  662.5f/960.0f - 1.0f, 365.0f/540.0f - 1.0f, 0.0f,
  675.0f/960.0f - 1.0f, 362.5f/540.0f - 1.0f, 0.0f,
  682.5f/960.0f - 1.0f, 365.0f/540.0f - 1.0f, 0.0f,
  700.0f/960.0f - 1.0f, 370.0f/540.0f - 1.0f, 0.0f,
  700.0f/960.0f - 1.0f, 410.0f/540.0f - 1.0f, 0.0f,
  682.5f/960.0f - 1.0f, 415.0f/540.0f - 1.0f, 0.0f,
  675.0f/960.0f - 1.0f, 417.5f/540.0f - 1.0f, 0.0f,
  662.5f/960.0f - 1.0f, 415.0f/540.0f - 1.0f, 0.0f,
  650.0f/960.0f - 1.0f, 410.0f/540.0f - 1.0f, 0.0f,
};

// Vertex representation of the phantom - RU todo: Make this automated
// by reading in data during initialisation of the application
static const GLfloat g_phantom_buffer_data[] = {
  //800.0f/960.0f - 1.0f, 395.0f/540.0f - 1.0f, 0.0f,
  -0.3f, -0.275f, 0.0f
};

// This wait counter is used for generating the time interval for the breadcrumb
int waitCounter = 0;

GLfloat g_vertex_rotated_buffer_data[30];
// Vector breadcrumb because data is added sequentially
std::vector< GLfloat> g_breadcrumb_buffer_dataVec;

RenderUtils::RenderUtils() { 
}

// Method that rotates and translates the vertex representation of the gantry
void RenderUtils::RotateObject(float angle, float x, float y){
  int i;
  //std::cout << g_vertex_rotated_buffer_data << std::endl;
  for (i = 0;i < 30; i = i+3)
    { //-0.29f and -0.27f are the x and y coord of the
      // center of the position map and the /400.0f controls the
      // scale
      g_vertex_rotated_buffer_data[i] = (cos(angle) * (g_vertex_buffer_data[i]+0.29f)
					 - sin(angle) * (g_vertex_buffer_data[i+1]+0.277f)) - 0.29f + x/350.0f;
      g_vertex_rotated_buffer_data[i+1] = (sin(angle) * (g_vertex_buffer_data[i]+0.29f)
					   + cos(angle) * (g_vertex_buffer_data[i+1]+0.277f)) - 0.27f + y/400.0f;
       g_vertex_rotated_buffer_data[i+2] =  g_vertex_buffer_data[i+2];
    }
  // push back the data into breadcrumb vector
  if (waitCounter == 0){
  g_breadcrumb_buffer_dataVec.push_back(g_vertex_rotated_buffer_data[6]);
  g_breadcrumb_buffer_dataVec.push_back(g_vertex_rotated_buffer_data[7]);
  g_breadcrumb_buffer_dataVec.push_back(g_vertex_rotated_buffer_data[8]);
  waitCounter = 100;
  }
  waitCounter = waitCounter - 1;
}



void RenderUtils::InitVAO() {
   //Method to initialise a vertex array object
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);  
}

// This gives a rgb value for a 0-100 ranged integer with 0 being Red and
// 100 being Green
void RenderUtils::ConvertInt2RG(float i){
  g = double(i) / 100.0;
  r = (100.0 - double(i)) / 100.0;
  b = 0;
}

// Method to create the colormap to indicate ES info.
void RenderUtils:: RenderHeatMap(glm::uvec2 eyeSize, float i){
    Platform::sleepMillis(1);
    ConvertInt2RG(i);
    if (True) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(0, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(r, g, b, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);      
    }
}

// Method to render a string on the OR
void RenderUtils::RenderMessageAt(std::string str, glm::uvec2 windowSize, glm::vec2  pos){
  windowAspect = aspect(windowSize);
  windowAspectInverse = 1.0f / windowAspect;
  MatrixStack & mv = Stacks::modelview();
  MatrixStack & pr = Stacks::projection();
  mv.push().identity();
  pr.push().top() = glm::ortho(
    -1.0f, 1.0f,
    -windowAspectInverse, windowAspectInverse,
    -100.0f, 100.0f);
  glm::vec2 cursor(pos.x, windowAspectInverse * pos.y);
  oria::renderString(str, cursor, 12.0f);
  pr.pop();
  mv.pop();
}

// Method to render a position map - Like I said at the top
// the coordinates used here are for a different system
void RenderUtils::RenderPositionMap(glm::uvec2 eyeSize) { 
   glEnable(GL_SCISSOR_TEST);
   glScissor(250, 350, 320, 320);
   glClearColor(0.5,0.5,0.5,0.1);
   glClear(GL_COLOR_BUFFER_BIT);
   glDisable(GL_SCISSOR_TEST);
}

// Method that calls the other render functions and creates the final display
void RenderUtils::RenderFinal(glm::uvec2 eyeSize, float Yaw, float POS,
			      float sES, float dist){
  RenderHeatMap(eyeSize, sES);
  RenderPositionMap(eyeSize);
  RenderGantry(eyeSize, Yaw, dist, POS);
  RenderBreadCrumb(eyeSize);
  RenderPhantom(eyeSize);
  RenderOdomData(eyeSize, dist);
}

void RenderUtils::RenderOdomData(glm::uvec2 eyeSize, float distance){
 std::string message = Platform::format(
					     "Estimated X pos: %0.2f cm \n"
					     "Object Type: Resistive",
					     distance);
 RenderMessageAt(message, eyeSize, glm::vec2(0.02f, -0.02f));
   
}

// Render Gantry based on the vertex representation defined at the top
void RenderUtils::RenderGantry(glm::uvec2 eyeSize, float angle, float x, float y){
  InitVAO();
  // This will identify our vertex buffer  
  GLuint vertexbuffer;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
 // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  RotateObject(angle* PI / 180, x, y);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_rotated_buffer_data), g_vertex_rotated_buffer_data, GL_STATIC_DRAW); 
  glEnableVertexAttribArray(0); 
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  //std::cout << "checked" << std::endl;
  glVertexAttribPointer(
 			0,
 			3,
 			GL_FLOAT,
 			GL_FALSE,
 			0,
 			(void*)0
 			);
  //glRotatef(45,0.0f,0.0f,1.0f);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
  glDisableVertexAttribArray(0);
}

//Render the phantom based on the vertex representation defined at the top
void RenderUtils::RenderPhantom(glm::uvec2 eyeSize){
  InitVAO();
  glEnable(GL_PROGRAM_POINT_SIZE);
  glPointSize(20);
  // This will identify our vertex buffer  
  GLuint vertexbuffer;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
 // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_phantom_buffer_data), g_phantom_buffer_data, GL_STATIC_DRAW); 
  glEnableVertexAttribArray(0); 
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  //std::cout << "checked" << std::endl;
  glVertexAttribPointer(
 			0,
 			3,
 			GL_FLOAT,
 			GL_FALSE,
 			0,
 			(void*)0
 			);
  //glRotatef(45,0.0f,0.0f,1.0f);
  glDrawArrays(GL_POINTS, 0, 1);
  glDisableVertexAttribArray(0);
}

// Render the breadcrumb based on the vector generated and appended
// sequentially
void RenderUtils::RenderBreadCrumb(glm::uvec2 eyeSize){
  InitVAO();
  glEnable(GL_PROGRAM_POINT_SIZE);
  glPointSize(3);
  int start = 0;
  GLfloat g_breadcrumb_buffer_data[g_breadcrumb_buffer_dataVec.size()];
  std::copy(g_breadcrumb_buffer_dataVec.begin(),
	    g_breadcrumb_buffer_dataVec.end(),
	    g_breadcrumb_buffer_data);
  // This will identify our vertex buffer  
  GLuint vertexbuffer;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
 // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_breadcrumb_buffer_data), g_breadcrumb_buffer_data, GL_DYNAMIC_DRAW); 
  glEnableVertexAttribArray(0); 
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
 			0,
 			3,
 			GL_FLOAT,
 			GL_FALSE,
 			0,
 			(void*)0
 			);
  int n = g_breadcrumb_buffer_dataVec.size();
  if (n > 20)
    start = n - 20;	   
  glDrawArrays(GL_POINTS, 0, n);
  glDisableVertexAttribArray(0);
}

