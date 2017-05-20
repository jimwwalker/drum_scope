#include "bar_drawer.h"
#include "midi_listener.h"
#include <memory>

int main() {
  BarDrawer bd(1200, 400, 2 /*barX*/, 2 /*barY*/, 4 /*beats*/, 90 /*bpm*/);
  std::unique_ptr<MidiListener> ml;
  try {
    ml = std::make_unique<MidiListener>(bd);
  } catch (std::exception &e) {
    std::cerr << "No midi input " << e.what() << '\n';
  }
  bd.run();
  return 0;
}