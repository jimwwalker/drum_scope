#include "bar.h"
#include <SDL2/SDL.h>

Bar::Bar(SDL_Renderer &renderer, SDL_Rect destination, int beats, int bpm)
    : renderer(renderer), destination(destination), beats(beats), bpm(bpm) {
  texture.reset(SDL_CreateTexture(&renderer, SDL_PIXELFORMAT_RGBA8888,
                                  SDL_TEXTUREACCESS_TARGET, destination.w,
                                  destination.h));

  SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);

  point.reset(SDL_CreateTexture(&renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, 3, 3));
  SDL_SetRenderTarget(&renderer, point.get());
  SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 0xFF); // black
  SDL_Rect dest{0, 0, 10, 10};
  SDL_RenderFillRect(&renderer, &dest);

  // So how many pixels millisecond?
  std::chrono::milliseconds msPerMinute(60000);
  std::chrono::milliseconds msPerBeat = msPerMinute / bpm;
  msPerBar = msPerBeat * beats;
  pixelsPerMs = msPerBar.count() / destination.w;

  staffs = 10;
  pixelsPerStaff = destination.h / staffs;

  makeRenderTarget();
  drawBeats(255);
}

void Bar::makeRenderTarget() { SDL_SetRenderTarget(&renderer, texture.get()); }

void Bar::setSpan(
    std::chrono::time_point<std::chrono::high_resolution_clock> s,
    std::chrono::time_point<std::chrono::high_resolution_clock> e) {
  start = s;
  end = e;
}

bool Bar::beat(
    int scale,
    std::chrono::time_point<std::chrono::high_resolution_clock> beatTime) {
  if (beatTime > end) {
    printf("beat off bar? %lld %lld\n", beatTime.time_since_epoch(),
           end.time_since_epoch());
    return true;
  } else if (scale > staffs) {
    printf("Skipping beat off scale? %d\n", scale);
    return false;
  }

  // map the scale to the y coord (@todo better)
  int y = scale * pixelsPerStaff;

  // map the beatTime to the x

  // 1 get the "distance into the bar in milliseconds"
  auto diff =
      std::chrono::duration_cast<std::chrono::milliseconds>(beatTime - start);

  // 2 get the x value for the beat
  int x = diff.count() / pixelsPerMs;

  // printf("Beat x:%d, y:%d pixelsPerMs:%d diff:%d\n", x, y, pixelsPerMs,
  // diff.count());

  SDL_Rect dest{x, y - 5, 3, 10};
  SDL_RenderCopy(&renderer, point.get(), nullptr, &dest);
  return false;
}

bool Bar::pulse(
    std::chrono::time_point<std::chrono::high_resolution_clock> pulseTime) {
  return pulseTime > end;
}

void Bar::drawBeats(int alpha) {

  // Make white
  SDL_Rect entireBar{0, 0, destination.w, destination.h};
  SDL_SetRenderDrawColor(&renderer, 255, 255, 255, alpha); // white
  SDL_RenderFillRect(&renderer, &entireBar);

  int divisions = destination.w / beats;

  // Vertical beat markers
  SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 0xFF); // r
  for (int b = 0; b < beats; b++) {
    int bx = b * divisions;
    SDL_RenderDrawLine(&renderer, bx, 0, bx, destination.h);
  }

  // Horizontal "staffs"
  // SDL_SetRenderDrawColor(&renderer, 255, 255, 204, 0xFF); // pale yellow
  for (int b = 0; b < staffs; b++) {
    int y = b * pixelsPerStaff;
    SDL_RenderDrawLine(&renderer, 0, y, destination.w, y);
  }
  // SDL_Rect rect{0, destination.h - 4, destination.w, destination.h};
  // SDL_RenderFillRect(&renderer, &rect);
}

void Bar::fade() {
  makeRenderTarget();
  drawBeats(10);
}

void Bar::draw() {
  // Render to destination quad
  SDL_RenderCopy(&renderer, texture.get(), nullptr, &destination);
}