#pragma once
#include "Common.h"

class RenderUtils {
 private:
  float         windowAspect{ 1.0f };
  float         windowAspectInverse{ 1.0f };
 public:
  RenderUtils();
  void RenderHeatMap(glm::uvec2, int);
  void RenderMessageAt( std::string, glm::uvec2, glm::vec2);
  void RenderPositionMap(glm::uvec2);
  void RenderFinal(glm::uvec2, int, int);
  void RenderOdomData(glm::uvec2, int);
};
