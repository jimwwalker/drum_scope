#include <SDL2/SDL.h>
#include <atomic>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

#include "bar_list.h"

#pragma once

struct SDL_Renderer_Deleter {
  void operator()(SDL_Renderer *r) const { SDL_DestroyRenderer(r); };
};

struct SDL_Window_Deleter {
  void operator()(SDL_Window *w) const { SDL_DestroyWindow(w); };
};

class BarDrawer {
public:
  BarDrawer(int screenX, int screenY, int barX, int barY, int beats, int bpm);

  void run();

  void beat(int midiId,
            std::chrono::time_point<std::chrono::high_resolution_clock> time);

private:
  std::unique_ptr<SDL_Window, SDL_Window_Deleter> window;
  std::unique_ptr<SDL_Renderer, SDL_Renderer_Deleter> renderer;

  // We will draw barX * barY Bar objects on the screen
  // std::vector<Bar> bars;
  BarList list;

  std::mutex lock;
  std::condition_variable cv;
  std::queue<std::pair<
      int, std::chrono::time_point<std::chrono::high_resolution_clock>>>
      beatQueue;
  std::atomic_bool notified;
};