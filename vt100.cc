#include <iostream>
#include <vector>

#include <cassert>

#include "buffer.h"
#include "screen.h"
#include "vt100.h"

vt100::vt100(Screen * const screen) : Terminal(screen) { }

/* does not handle unicode properly */
void vt100::pollin() {
  std::array<char, 1025> buffer{'\0'};
  assert(nullptr != screen_);
  ssize_t length = read(fd_.child, buffer.data(), buffer.size() - 1);
  while (0 < length) {
    for (std::size_t i = 0; i < length; ++i) {
      const char c = buffer[i];
      if (std::isprint(c)) {
        std::cout << c << " ";
      } else {
        std::cout << "* ";
      }
      if (CSI == state_) {
        switch (c) {
        case L'H': // move cursor
          state_ = DEFAULT;
          break;
        case L'J': // clear screen
          state_ = DEFAULT;
          screen_->clear();
          break;
        case L'?':
          state_ = DEFAULT;
          break;
        }
      } else if (ESCAPE == state_) {
        switch (c) {
        case L'[':
          state_ = CSI;
          break;
        default:
          state_ = DEFAULT;
          break;
        }
      } else if (DEFAULT == state_)  {
        switch (c) {
        case L'\e':
          state_ = ESCAPE;
          break;
        default:
          screen_->buffer().pushBack(Rune{c});
          break;
        }
      }
    }
    std::cout << std::endl;
    length = read(fd_.child, buffer.data(), buffer.size() - 1);
  }
  // screen_->write();
}
