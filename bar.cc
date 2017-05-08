#include "bar.h"
#include <SDL2/SDL.h>

Bar::Bar(SDL_Renderer& renderer, std::pair<int, int> dimension)
    : renderer(renderer), dimension(dimension) {
    texture.reset(SDL_CreateTexture(&renderer,
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    dimension.first,
                                    dimension.second));
    start = std::chrono::high_resolution_clock::now();
}

void Bar::makeRenderTarget() {
    SDL_SetRenderTarget(&renderer, texture.get());
}

void Bar::beat(int scale, std::chrono::time_point<std::chrono::high_resolution_clock> beatTime) {
    // map the scale to the y coord
    int y = scale * 10;

    // map the beatTime to the x

    // 1 get the "distance into the bar in milliseconds"
    auto diff =
        std::chrono::duration_cast<std::chrono::milliseconds>(beatTime - start);

    int x = diff.count() / dimension.first;

    //x = scale * 10;

    SDL_RenderDrawPoint(&renderer, x, y);
}

void Bar::draw() {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {0, 0, 0, 0};

    //Render to screen
    SDL_RenderCopy(&renderer, texture.get(), nullptr, nullptr);
}