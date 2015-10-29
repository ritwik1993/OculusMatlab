#pragma once


class YawTracker {
 private:
  ovrHmd hmd;
  ovrTrackingState state;
  ovrQuatf orientation;
  glm::quat q;
  glm::vec3 euler;
  bool trackingState;
 public:
  YawTracker(ovrHmd riftObj);
  double CurrentYaw();
  bool ReadTrackingState();
};
  
