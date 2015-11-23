/************************************************************************************

 Authors     :   Ritwik Ummalaneni <ritwik1993@gmail.com>
 Copyright   :   Copyright Ritwik Ummalaneni. All Rights reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 ************************************************************************************/

#include "Common.h"

class MainAppOld {
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
      SAY("Current Yaw - %.02f", trackObj->CurrentYaw());
      socketObj->UpdateYaw(trackObj->CurrentYaw());
      socketObj->Write_Data();
      socketObj->Read_Data();
      Platform::sleepMillis(100);
      i++;
      
    }
    delete trackObj;
    delete socketObj;
    ovrHmd_Destroy(hmd);
    return 0;
  }
};

RUN_OVR_APP(MainAppOld);
