#include "Common.h"

class MainApp {
public:
  int run() {
    int i = 0;
    ovrHmd hmd = ovrHmd_Create(0);    
    if (!hmd) {
      SAY_ERR("Unable to open Rift device");
      return -1;
    }
    //Initialise a tracker object using current HMD
    YawTracker *trackObj = new YawTracker(hmd);
    //Initialise a Tcp server to send data on port (def: 1700)
    TcpServer *socketObj = new TcpServer(1700);

    while ( i < 100 && trackObj->ReadTrackingState() != 0) {
      //SAY("Current Yaw - %.02f", trackObj->CurrentYaw());
      socketObj->UpdateYaw(trackObj->CurrentYaw());
      socketObj->WriteData();
      Platform::sleepMillis(1000);
      i++;
    }
    delete trackObj;
    delete socketObj;
    ovrHmd_Destroy(hmd);
    return 0;
  }
};

RUN_OVR_APP(MainApp);
