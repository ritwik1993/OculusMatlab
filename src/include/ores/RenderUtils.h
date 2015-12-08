#pragma once
#include "Common.h"

class RenderUtils {
 private:
  float         windowAspect{ 1.0f };
  float         windowAspectInverse{ 1.0f };
  double r,g,b;
 public:
  RenderUtils();
  void InitVAO();
  void ConvertInt2RG(float);
  void RenderHeatMap(glm::uvec2, float);
  void RenderMessageAt( std::string, glm::uvec2, glm::vec2);
  void RenderPositionMap(glm::uvec2);
  void RenderFinal(glm::uvec2, float, float, float, float);
  void RenderOdomData(glm::uvec2, float);
  void RenderGantry(glm::uvec2, float, float, float);
  void RenderPhantom(glm::uvec2);
  void RotateObject(float, float, float);
  void RenderBreadCrumb(glm::uvec2);
};
