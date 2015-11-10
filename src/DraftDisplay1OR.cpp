#include "Common.h"

static const glm::uvec2 WINDOW_SIZE(1280, 800);
static const glm::ivec2 WINDOW_POS(100, 100);
static const glm::uvec2 EYE_SIZE(
    WINDOW_SIZE.x / 2, WINDOW_SIZE.y);

struct PerEyeArg {
  FramebufferWrapperPtr  framebuffer;
  glm::mat4 projection;
  glm::mat4 modelviewOffset;
};

int k =0;

class DraftDisplay1OR: public RiftGlfwApp {
  PerEyeArg eyes[2];
  ovrTexture eyeTextures[2];

  float ipd, eyeHeight;

public:
  DraftDisplay1OR() {
    eyeHeight = ovrHmd_GetFloat(hmd, OVR_KEY_EYE_HEIGHT, eyeHeight);
    ipd = ovrHmd_GetFloat(hmd, OVR_KEY_IPD, ipd);

    Stacks::modelview().top() = glm::lookAt(
      vec3(0, eyeHeight, 0.5f),
      vec3(0, eyeHeight, 0),
      Vectors::UP);
  }

  virtual void initGl() {
    RiftGlfwApp::initGl();

    ovrRenderAPIConfig cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.Header.API = ovrRenderAPI_OpenGL;
    cfg.Header.BackBufferSize = ovr::fromGlm(getSize());
    cfg.Header.Multisample = 1;

    int distortionCaps = ovrDistortionCap_Vignette;
    ovrEyeRenderDesc eyeRenderDescs[2];
    int configResult = ovrHmd_ConfigureRendering(hmd, &cfg,
        distortionCaps, hmd->DefaultEyeFov, eyeRenderDescs);

    for_each_eye([&](ovrEyeType eye){
      PerEyeArg & eyeArgs = eyes[eye];
      ovrFovPort fov = hmd->DefaultEyeFov[eye];
      ovrSizei texSize = ovrHmd_GetFovTextureSize(hmd, eye, fov, 1.0f);
      eyeArgs.framebuffer = FramebufferWrapperPtr(new FramebufferWrapper());
      eyeArgs.framebuffer->init(ovr::toGlm(texSize));

      ovrTextureHeader & textureHeader = eyeTextures[eye].Header;
      textureHeader.API = ovrRenderAPI_OpenGL;
      textureHeader.TextureSize = texSize;
      textureHeader.RenderViewport.Size = texSize;
      textureHeader.RenderViewport.Pos.x = 0;
      textureHeader.RenderViewport.Pos.y = 0;
      ((ovrGLTextureData&)eyeTextures[eye]).TexId =
        oglplus::GetName(eyeArgs.framebuffer->color);

      eyeArgs.modelviewOffset = glm::translate(glm::mat4(), 
        ovr::toGlm(eyeRenderDescs[eye].HmdToEyeViewOffset));

      ovrMatrix4f projection = ovrMatrix4f_Projection(fov, 0.01f, 100, true);
      eyeArgs.projection = ovr::toGlm(projection);
    });
  }

  virtual void finishFrame() {
  }

  virtual void draw() {
    glm::uvec2 eyeSize = getSize();
    static ovrPosef eyePoses[2];    
   std::string message = Platform::format(
					     "Oculus Rift \n"
					     "Electrosense");
   
    ovrHmd_BeginFrame(hmd, getFrame());
    MatrixStack & mv = Stacks::modelview();
    for (int i = 0; i < ovrEye_Count; ++i) {
      ovrEyeType eye = hmd->EyeRenderOrder[i];
      PerEyeArg & eyeArgs = eyes[eye];
      Stacks::projection().top() = eyeArgs.projection;

      eyeArgs.framebuffer->Bind();
      glClearColor(0.0f,0.0f,0.0f,0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      oglplus::Context::Clear().DepthBuffer();
      Stacks::withPush(mv, [&]{
        mv.preMultiply(eyeArgs.modelviewOffset);
	RenderDraft(eyeSize, 0, 0);	
        RenderMessageAt(message, eyeSize, glm::vec2(0.0f, 0.0f));
      });
    }

    k++;
    ovrHmd_EndFrame(hmd, eyePoses, eyeTextures);
  }
};

RUN_OVR_APP(DraftDisplay1OR);
