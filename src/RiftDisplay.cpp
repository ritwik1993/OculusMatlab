#include "Common.h"

class RiftDisplay : public GlfwApp {
  ovrHmd hmd;

  int i =0;
public:

  RiftDisplay() {
    hmd = ovrHmd_Create(0);
    if (!hmd) {
      hmd = ovrHmd_CreateDebug(ovrHmd_DK2);
    }
    if (!hmd) {
      FAIL("Unable to detect Rift display");
    }
  }

  virtual GLFWwindow * createRenderingTarget(
      glm::uvec2 & outSize, glm::ivec2 & outPosition) {
    GLFWwindow * window;
    bool extendedMode = ovrHmdCap_ExtendDesktop & hmd->HmdCaps;

    outPosition = glm::ivec2(hmd->WindowsPos.x, hmd->WindowsPos.y);
    outSize = glm::uvec2(hmd->Resolution.w, hmd->Resolution.h);  
    if (extendedMode) {
      GLFWmonitor * monitor = glfw::getMonitorAtPosition(outPosition);
      if (nullptr != monitor) {
        const GLFWvidmode * mode = glfwGetVideoMode(monitor);
	 outSize = glm::uvec2(mode->width, mode->height);
      }
      glfwWindowHint(GLFW_DECORATED, 0);
      std::cout << outSize.x << std::endl;
      std::cout << outSize.y << std::endl;
      window = glfw::createWindow(outSize, outPosition);
    } else {

      // A bug in some versions of the SDK (0.4.x) prevents Direct Mode from 
      // engaging properly unless you call the GetEyePoses function.
      {
        static ovrVector3f offsets[2];
        static ovrPosef poses[2];
        ovrHmd_GetEyePoses(hmd, 0, offsets, poses, nullptr);
      }

      window = glfw::createSecondaryScreenWindow(outSize);
      void * nativeWindowHandle = glfw::getNativeWindowHandle(window);
      if (nullptr != nativeWindowHandle) {
        ovrHmd_AttachToWindow(hmd, nativeWindowHandle, nullptr, nullptr);
      }
    }

    return window;
  }

  void draw() {
    glm::uvec2 eyeSize = getSize();
    eyeSize.x /= 2;
    int k;
    Platform::sleepMillis(500);
    if (i<1000) {
      k = i%3;
      switch (k){
      case 0:
        glEnable(GL_SCISSOR_TEST);
        glScissor(0, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glScissor(eyeSize.x, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
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
        glScissor(0, 2*eyeSize.y/3, eyeSize.x, eyeSize.y/3);
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
};

RUN_OVR_APP(RiftDisplay);
