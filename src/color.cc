#include "color.hpp"

#include <stdexcept>

Color::Color(int r, int g, int b): red_(r), green_(g), blue_(b) {
  const int kMax = 255;
  if (r > kMax || r < 0 || g > kMax || g < 0 || b > kMax || b < 0) {
    throw std::runtime_error("out of bounds color");
  }
}

// do not modify
bool operator==(const Color& rhs, const Color& lhs) {
  return (rhs.Red() == lhs.Red() && rhs.Green() == lhs.Green() &&
          rhs.Blue() == lhs.Blue());
}