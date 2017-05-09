#include "bar.h"
#include <SDL2/SDL.h>

Bar::Bar(SDL_Renderer& renderer,
         std::pair<int, int> dimension,
         int beats,
         int bpm)
    : renderer(renderer), dimension(dimension), beats(beats), bpm(bpm) {
    texture.reset(SDL_CreateTexture(&renderer,
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    dimension.first,
                                    dimension.second));
    start = std::chrono::high_resolution_clock::now();

    // So how many pixels millisecond?
/*
60 bpm 4
1 b per second  (60/60)
1 b every 1000ms

120 bpm 4 b per bar
1 b every 500ms
2 b per second 120/60

30bpm 4 b per bar
0.5 b per second 30/60
1 b every 2000ms

we have 60,000ms per m

so 60,000 / 60 = 1000  4 4000 ms per bar - 400 pixels 1 pixel = 10ms
so 60,000 / 120 = 500
so 60,000 / 30 = 2000 4 2000ms per par 400 pixels 1 pixel = 20ms
*/
    std::chrono::milliseconds msPerMinute(60000);
    std::chrono::milliseconds msPerBeat = msPerMinute / bpm;
    std::chrono::milliseconds msPerBar = msPerBeat * beats;
    pixelsPerMs = msPerBar.count() / dimension.first;


    makeRenderTarget();
    drawBeats();
}

void Bar::makeRenderTarget() {
    SDL_SetRenderTarget(&renderer, texture.get());
}

void Bar::beat(int scale, std::chrono::time_point<std::chrono::high_resolution_clock> beatTime) {
    // map the scale to the y coord (@todo better)
    int y = scale * 10;

    // map the beatTime to the x

    // 1 get the "distance into the bar in milliseconds"
    auto diff =
        std::chrono::duration_cast<std::chrono::milliseconds>(beatTime - start);

    // 2 get the x value for the beat
    int x = diff.count() / pixelsPerMs;

    printf("Beat x:%d pixelsPerMs:%d diff:%d\n", x, pixelsPerMs, diff.count());

    //x = scale * 10;

    SDL_RenderDrawPoint(&renderer, x, y);
}

void Bar::drawBeats() {
    int divisions = dimension.first / beats;
    SDL_SetRenderDrawColor(&renderer, 0, 0xff, 0x00, 0xFF ); // green
    for (int b = 0; b < beats; b++) {
        int bx = b * divisions;
        SDL_RenderDrawLine(&renderer,
                            bx, 0,
                            bx, dimension.second);
    }
}

void Bar::draw() {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {0, 0, dimension.first, dimension.second};

    //Render to screen
    SDL_RenderCopy(&renderer, texture.get(), nullptr, &renderQuad);
}