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
  //610.0f/960.0f - 1.0f, 410.0f/540.0f - 1.0f, 0.0f,
};

// A vectors which represents a phantom
static const GLfloat g_phantom_buffer_data[] = {
  0.0f, 0.25f, 0.0f,
  //610.0f/960.0f - 1.0f, 410.0f/540.0f - 1.0f, 0.0f,
};

 GLfloat g_vertex_rotated_buffer_data[30];

RenderUtils::RenderUtils() { 
}

void RenderUtils::RotateObject(float angle){
  int i;
  //std::cout << g_vertex_rotated_buffer_data << std::endl;
  for (i = 0;i < 30; i = i+3)
    {
      g_vertex_rotated_buffer_data[i] = (cos(angle) * (g_vertex_buffer_data[i]+0.29f)
					 - sin(angle) * (g_vertex_buffer_data[i+1]+0.277f)) - 0.29f;
      g_vertex_rotated_buffer_data[i+1] = (sin(angle) * (g_vertex_buffer_data[i]+0.29f)
					   + cos(angle) * (g_vertex_buffer_data[i+1]+0.277f)) - 0.27f;
       g_vertex_rotated_buffer_data[i+2] =  g_vertex_buffer_data[i+2];
    }
}

void RenderUtils::InitVAO() {
   //Method to initialise a vertex array object
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);  
}

void RenderUtils::ConvertInt2RG(int i){
  r = double(i) / 100.0;
  g = (100.0 - double(i)) / 100.0;
  b = 0;
}

void RenderUtils:: RenderHeatMap(glm::uvec2 eyeSize, int i){
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

void RenderUtils::RenderFinal(glm::uvec2 eyeSize, int i, int dist){
  RenderHeatMap(eyeSize, dist);
  RenderPositionMap(eyeSize);
  RenderGantry(eyeSize, dist);
  RenderPhantom(eyeSize);
  RenderOdomData(eyeSize, dist);
}

void RenderUtils::RenderOdomData(glm::uvec2 eyeSize, int distance){
 std::string message = Platform::format(
					     "Estimated Distance: %0.2d \n"
					     "Object Type: Conductor",
					     distance);
 RenderMessageAt(message, eyeSize, glm::vec2(0.02f, -0.02f));
   
}

void RenderUtils::RenderGantry(glm::uvec2 eyeSize, int angle){
  InitVAO();
  // This will identify our vertex buffer  
  GLuint vertexbuffer;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
 // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  RotateObject(angle* PI / 180);
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
  //gl_PointSize = 10.0;
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
  


