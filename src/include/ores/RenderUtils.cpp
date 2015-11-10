#include "Common.h"

void RenderDraft(glm::uvec2 eyeSize, int i, int eye){
   glm::uvec2 eyeOrigin(eye * eyeSize.x, 0);
  eyeSize.x /= 2;
    int k;
    Platform::sleepMillis(1000);
    if (True) {
      k = i%3;
      switch (k){
      case 0:
        glEnable(GL_SCISSOR_TEST);
        glScissor(eyeOrigin.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(eyeOrigin.x + eyeSize.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_SCISSOR_TEST);
        i++;
	break;
      case 1:
        glEnable(GL_SCISSOR_TEST);
        glScissor(eyeOrigin.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(eyeOrigin.x + eyeSize.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_SCISSOR_TEST);
        i++;
	break;
      case 2:
        glEnable(GL_SCISSOR_TEST);
        glScissor(eyeOrigin.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(0, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(eyeOrigin.x + eyeSize.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(0, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_SCISSOR_TEST);
        i++;
	break;
      }
      glEnable(GL_SCISSOR_TEST);
      glScissor(eyeOrigin.x+100, 0+100,eyeSize.x/2,eyeSize.y/2);
      glClearColor(0.5,0.5,0.5,0.5);
      glClear(GL_COLOR_BUFFER_BIT);
      glDisable(GL_SCISSOR_TEST);      
    }
}
void RenderMessageAt(std::string str, glm::uvec2 windowSize, glm::vec2  pos){
  float         windowAspect{ 1.0f };
  float         windowAspectInverse{ 1.0f };
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
  oria::renderString(str, cursor, 18.0f);
  pr.pop();
  mv.pop();

}



