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

// Initialise the head tracker
YawTracker::YawTracker(ovrHmd riftObj)
  : hmd(riftObj){
  if (!ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
			   ovrTrackingCap_MagYawCorrection |
				ovrTrackingCap_Position, 0)) {
    SAY_ERR("Unable to start the Head tracker");
    trackingState = 0;
  }
  else trackingState = 1;
}
 
//Method to read current yaw in degrees and X,Y states
void YawTracker::ReadState() {
  state = ovrHmd_GetTrackingState(hmd, 0);
  orientation = state.HeadPose.ThePose.Orientation;
  position = state.HeadPose.ThePose.Position;
  currentX = position.x;
  currentY = -position.z; //we need Y to be along the axis of the sensorpod
  // which is in the -ve z direction of the OR
  q = glm::make_quat(&orientation.x);
  euler = glm::eulerAngles(q);
  currentYaw =  euler.y * RADIANS_TO_DEGREES;
}

// Accessor for the Yaw
double YawTracker::CurrentYaw() {
  ReadState();
  return ( currentYaw);
}

// Accessor for the X pose
float YawTracker::CurrentX() {
  if (!ovrStatus_PositionConnected && !ovrStatus_PositionTracked)
    return (0.0);
  ReadState();
  return ( currentX);
}

// Accessor for the Y pose
float YawTracker::CurrentY() {
   if (!ovrStatus_PositionConnected && !ovrStatus_PositionTracked)
     return (0.0);
  ReadState();
  return ( currentY);
}

// Accessor for the boolean tracking state
bool YawTracker::ReadTrackingState() {
  return trackingState;
    }

  
