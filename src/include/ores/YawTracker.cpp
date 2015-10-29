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
 
//Method to read current yaw in degrees
double YawTracker::CurrentYaw() {
  state = ovrHmd_GetTrackingState(hmd, 0);
  orientation = state.HeadPose.ThePose.Orientation;
  q = glm::make_quat(&orientation.x);
  euler = glm::eulerAngles(q);
  return ( euler.y * RADIANS_TO_DEGREES);
}

bool YawTracker::ReadTrackingState() {
  return trackingState;
    }

  
