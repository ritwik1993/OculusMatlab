#include "Common.h"

RenderUtils::RenderUtils() {
}

void RenderUtils:: RenderHeatMap(glm::uvec2 eyeSize, int i){
    eyeSize.x /= 2;
    int k;
    Platform::sleepMillis(1000);
    if (True) {
      k = i%3;
      switch (k){
      case 0:
        glEnable(GL_SCISSOR_TEST);
        glScissor(0, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor( eyeSize.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_SCISSOR_TEST);
        i++;
	break;
      case 1:
        glEnable(GL_SCISSOR_TEST);
        glScissor(0, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(eyeSize.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_SCISSOR_TEST);
        i++;
	break;
      case 2:
        glEnable(GL_SCISSOR_TEST);
        glScissor(0 , 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(0, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(eyeSize.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(0, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_SCISSOR_TEST);
        i++;
	break;
      }      
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
   glScissor(100, 0+100,eyeSize.x/2,eyeSize.y/2);
   glClearColor(0.5,0.5,0.5,0.5);
   glClear(GL_COLOR_BUFFER_BIT);
   glDisable(GL_SCISSOR_TEST);
}

void RenderUtils::RenderFinal(glm::uvec2 eyeSize, int i){
  RenderHeatMap(eyeSize, i);
  RenderPositionMap(eyeSize);
  RenderOdomData(eyeSize, 50);
}

void RenderUtils::RenderOdomData(glm::uvec2 eyeSize, int distance = 0){
 std::string message = Platform::format(
					     "Estimated Distance: %0.2f \n"
					     "Object Type: Conductor",
					     distance);
 RenderMessageAt(message, eyeSize, glm::vec2(0.05f, -0.05f));
   
}



