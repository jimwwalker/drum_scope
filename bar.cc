#include "bar.h"
#include <SDL2/SDL.h>

Bar::Bar(SDL_Renderer& renderer,
         SDL_Rect destination,
         std::chrono::time_point<std::chrono::high_resolution_clock> scoreEpoch,
         int barIndex,
         int beats,
         int bpm)
    : renderer(renderer), destination(destination), beats(beats), bpm(bpm) {
    texture.reset(SDL_CreateTexture(&renderer,
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    destination.w,
                                    destination.h));
    // So how many pixels millisecond?
    std::chrono::milliseconds msPerMinute(60000);
    std::chrono::milliseconds msPerBeat = msPerMinute / bpm;
    std::chrono::milliseconds msPerBar = msPerBeat * beats;
    pixelsPerMs = msPerBar.count() / destination.w;

    start = scoreEpoch + (barIndex * msPerBar);
    end = start + msPerBar;

    makeRenderTarget();
    drawBeats();
}

void Bar::makeRenderTarget() {
    SDL_SetRenderTarget(&renderer, texture.get());
}

bool Bar::beat(int scale, std::chrono::time_point<std::chrono::high_resolution_clock> beatTime) {
    if (beatTime > end) {
        return true;
    }

    // map the scale to the y coord (@todo better)
    int y = scale * 10;

    // map the beatTime to the x

    // 1 get the "distance into the bar in milliseconds"
    auto diff =
        std::chrono::duration_cast<std::chrono::milliseconds>(beatTime - start);

    // 2 get the x value for the beat
    int x = diff.count() / pixelsPerMs;

    printf("Beat x:%d pixelsPerMs:%d diff:%d\n", x, pixelsPerMs, diff.count());

    SDL_RenderDrawPoint(&renderer, x, y);
    return false;
}

void Bar::drawBeats() {
    int divisions = destination.w / beats;
    SDL_SetRenderDrawColor(&renderer, 0, 0xff, 0x00, 0xFF ); // green
    for (int b = 0; b < beats; b++) {
        int bx = b * divisions;
        SDL_RenderDrawLine(&renderer,
                            bx, 0,
                            bx, destination.h);
    }
}

void Bar::draw() {
    // Render to destination quad
    SDL_RenderCopy(&renderer, texture.get(), nullptr, &destination);
}