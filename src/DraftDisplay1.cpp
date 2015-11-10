#include "Common.h"

static const glm::uvec2 WINDOW_SIZE(1280, 800);
static const glm::ivec2 WINDOW_POS(100, 100);
static const glm::uvec2 EYE_SIZE(
    WINDOW_SIZE.x / 2, WINDOW_SIZE.y);
static const float EYE_ASPECT = 
    aspect(EYE_SIZE);

struct PerEyeArg {
  glm::ivec2 viewportPosition;
  glm::mat4 modelviewOffset;
};
int k = 0;

class DraftDisplay1 : public GlfwApp {
  PerEyeArg eyes[2];

public:
  DraftDisplay1() {
    Stacks::projection().top() = glm::perspective(
      PI / 2.0f, EYE_ASPECT, 0.01f, 100.0f);

    Stacks::modelview().top() = glm::lookAt(
      vec3(0, OVR_DEFAULT_EYE_HEIGHT, 0.5f),
      vec3(0, OVR_DEFAULT_EYE_HEIGHT, 0),
      Vectors::UP);

    glm::vec3 offset(OVR_DEFAULT_IPD / 2.0f, 0, 0);
    eyes[ovrEye_Left] = {
      glm::ivec2(0, 0),
      glm::translate(glm::mat4(), offset)
    };
    eyes[ovrEye_Right] = {
      glm::ivec2(WINDOW_SIZE.x / 2, 0),
      glm::translate(glm::mat4(), -offset)
    };
  }

  virtual GLFWwindow * createRenderingTarget(
    glm::uvec2 & outSize, glm::ivec2 & outPosition) 
  {
    outSize = WINDOW_SIZE;
    outPosition = WINDOW_POS;
    return glfw::createWindow(outSize, outPosition);
  }

 virtual  void draw() {
   //oglplus::Context::Clear().ColorBuffer().DepthBuffer();
    MatrixStack & mv = Stacks::modelview();
     std::string message = Platform::format(
					     "Oculus Rift \n"
					     "Electrosense");
    for (int i = 0; i < 2; ++i) {
      PerEyeArg & eyeArgs = eyes[i];
      viewport(EYE_SIZE, eyeArgs.viewportPosition);
      Stacks::withPush(mv, [&]{
	  mv.preMultiply(eyeArgs.modelviewOffset);
	  //oria::renderExampleScene(
         //OVR_DEFAULT_IPD, OVR_DEFAULT_EYE_HEIGHT);
	  //RenderDraft(EYE_SIZE, k, 0);
	  std::string message = Platform::format(
					     "Oculus Rift \n"
					     "Electrosense");
	  //glm::vec2 test(100,100);
	//oria::renderString(message, test, 100);
	//GlfwApp::renderStringAt(message, glm::vec2(0.5f,-0.5f));
      });
    }
    k++;
  }
};

RUN_APP(DraftDisplay1);
