#include "bar_list.h"

BarList::BarList(int beats, int bpm) {
  std::chrono::milliseconds msPerMinute(60000);
  std::chrono::milliseconds msPerBeat = msPerMinute / bpm;
  msPerBar = msPerBeat * beats;
}

void BarList::addBar(SDL_Renderer &renderer, SDL_Rect destination, int beats,
                     int bpm) {
  bars.push_back({renderer, destination, beats, bpm});
}

std::chrono::time_point<std::chrono::high_resolution_clock> BarList::setEpoch(
    std::chrono::time_point<std::chrono::high_resolution_clock> epoch) {
  for (auto &b : bars) {
    b.setSpan(epoch, epoch + msPerBar);
    epoch += msPerBar;
  }
  return epoch;
}