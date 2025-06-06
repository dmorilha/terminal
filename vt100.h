#pragma once

#include <array>

#include "terminal.h"

struct vt100 : public Terminal {
  vt100(Screen * const);
  void pollin() override;
  std::vector<char> escapeSequence_;
  enum {
    DEFAULT,
    ESCAPE,
    CSI,
    UPPER_BOUND,
  } state_ = DEFAULT;
};
