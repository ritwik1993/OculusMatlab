#include "Common.h"

RenderUtils::RenderUtils() {
}

void RenderUtils::ConvertInt2RG(int i){
  r = double(i) / 100.0;
  g = (100.0 - double(i)) / 100.0;
  b = 0;
}


void RenderUtils:: RenderHeatMap(glm::uvec2 eyeSize, int i){
    eyeSize.x /= 2;
    Platform::sleepMillis(10);
    ConvertInt2RG(i);
    if (True) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(0, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(r, g, b, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor( eyeSize.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
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
   eyeSize.x /= 2;   
   glEnable(GL_SCISSOR_TEST);
   glScissor(250, 300, eyeSize.x/3,eyeSize.y/3);
   glClearColor(0.5,0.5,0.5,0.5);
   glClear(GL_COLOR_BUFFER_BIT);
   glDisable(GL_SCISSOR_TEST);
}

void RenderUtils::RenderFinal(glm::uvec2 eyeSize, int i, int dist){
  RenderHeatMap(eyeSize, dist);
  RenderPositionMap(eyeSize);
  RenderOdomData(eyeSize, dist);
}

void RenderUtils::RenderOdomData(glm::uvec2 eyeSize, int distance){
 std::string message = Platform::format(
					     "Estimated Distance: %0.2d \n"
					     "Object Type: Conductor",
					     distance);
 RenderMessageAt(message, eyeSize, glm::vec2(0.02f, -0.02f));
   
}



