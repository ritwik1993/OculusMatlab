#pragma once
#include "Common.h"

class RenderUtils {
 public:
  RenderUtils();
  void RenderDraft(glm::uvec2, int, int);
  void RenderMessageAt( std::string, glm::uvec2, glm::vec2);
};
