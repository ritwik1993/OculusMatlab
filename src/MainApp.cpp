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

class MainApp: public RiftGlfwApp {
  PerEyeArg eyes[2];
  ovrTexture eyeTextures[2];
  RenderUtils *oresRender = new RenderUtils(); // RU todo: Destroy this
  float ipd, eyeHeight;
  //Initialise a tracker object using current HMD
    YawTracker *trackObj = new YawTracker(hmd);
    //Initialise a Tcp server to send data on port (def: 1700)
    TcpServer *socketObj = new TcpServer(1700);
public:
  MainApp() {
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
    //oresRender->InitVAO();
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
    //SAY("Current Yaw - %.02f", trackObj->CurrentYaw());	
    socketObj->UpdateYaw(trackObj->CurrentYaw());
    socketObj->UpdateX(trackObj->CurrentX() * 100); // convert to cm
    socketObj->UpdateY(trackObj->CurrentY() * 100); //  convert to cm
    socketObj->Write_Data();
    socketObj->Read_Data();
    float yaw = socketObj->Get_EsDataYaw();
    float esPOS = socketObj->Get_EsDataPOS();
    float sES = socketObj->Get_EsDatasES();
    float esDist = socketObj->Get_EsDataDist();
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
	oresRender->RenderFinal(eyeSize, yaw, esPOS, sES, esDist);	
      });
    }

    k++;
    ovrHmd_EndFrame(hmd, eyePoses, eyeTextures);
  }
};

RUN_OVR_APP(MainApp);
