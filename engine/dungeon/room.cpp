#include "room.h"

bool overlaps(const Room &a, const Room &b) {
  bool well_separated = (a.position.x + a.size.x < b.position.x) ||
                        (b.position.x + b.size.x < a.position.x) ||
                        (a.position.y + a.size.y < b.position.y) ||
                        (b.position.y + b.size.y < a.position.y);
  return !well_separated;
}

std::ostream &operator<<(std::ostream &os, const Room &room) {
  return os << "Room(pos=" << room.position << ", size=" << room.size << ")";
}
