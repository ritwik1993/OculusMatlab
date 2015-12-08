#include "Common.h"

// An array of 3 vectors which represents 3 vertices
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

// A vectors which represents a phantom
static const GLfloat g_phantom_buffer_data[] = {
  //800.0f/960.0f - 1.0f, 395.0f/540.0f - 1.0f, 0.0f,
  -0.29f, -0.27f, 0.0f
};


int waitCounter = 0;

GLfloat g_vertex_rotated_buffer_data[30];
std::vector< GLfloat> g_breadcrumb_buffer_dataVec;

RenderUtils::RenderUtils() { 
}

void RenderUtils::RotateObject(float angle, float x, float y){
  int i;
  //std::cout << g_vertex_rotated_buffer_data << std::endl;
  for (i = 0;i < 30; i = i+3)
    {
      g_vertex_rotated_buffer_data[i] = (cos(angle) * (g_vertex_buffer_data[i]+0.29f)
					 - sin(angle) * (g_vertex_buffer_data[i+1]+0.277f)) - 0.29f + x/300.0f;
      g_vertex_rotated_buffer_data[i+1] = (sin(angle) * (g_vertex_buffer_data[i]+0.29f)
					   + cos(angle) * (g_vertex_buffer_data[i+1]+0.277f)) - 0.27f + y/300.0f;
       g_vertex_rotated_buffer_data[i+2] =  g_vertex_buffer_data[i+2];
    }
  if (waitCounter == 0){
  g_breadcrumb_buffer_dataVec.push_back(g_vertex_rotated_buffer_data[6]);
  g_breadcrumb_buffer_dataVec.push_back((g_vertex_rotated_buffer_data[7]);
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

void RenderUtils::ConvertInt2RG(float i){
  g = double(i) / 100.0;
  r = (100.0 - double(i)) / 100.0;
  b = 0;
}

void RenderUtils:: RenderHeatMap(glm::uvec2 eyeSize, float i){
    Platform::sleepMillis(1);
    ConvertInt2RG(i);
    //std::cout << sqrt(4) << std::endl;
    if (True) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(0, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(r, g, b, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);      
    }
}

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

void RenderUtils::RenderPositionMap(glm::uvec2 eyeSize) { 
   glEnable(GL_SCISSOR_TEST);
   glScissor(250, 350, 320, 320);
   glClearColor(0.5,0.5,0.5,0.1);
   glClear(GL_COLOR_BUFFER_BIT);
   glDisable(GL_SCISSOR_TEST);
}

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
					     "Estimated Distance: %0.2f cm \n"
					     "Object Type: Conductor",
					     distance);
 RenderMessageAt(message, eyeSize, glm::vec2(0.02f, -0.02f));
   
}

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

void RenderUtils::RenderPhantom(glm::uvec2 eyeSize){
  InitVAO();
  glEnable(GL_PROGRAM_POINT_SIZE);
  glPointSize(10);
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

