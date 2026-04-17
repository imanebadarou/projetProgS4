#pragma once

enum class Color { black, white };

struct coords {
  int x{};
  int y{};
};

inline bool operator==(coords const &lhs, coords const &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}