#include <chrono>
#include <memory>

#include <SDL2/SDL.h>

#pragma once

struct SDL_Texture_Deleter {
    void operator()(SDL_Texture* texture) const {
        SDL_DestroyTexture(texture);
    };
};

class Bar {
public:
    Bar(SDL_Renderer& renderer,
        SDL_Rect destination,
        int beats,
        int bpm);

    void makeRenderTarget();

    void setSpan(std::chrono::time_point<std::chrono::high_resolution_clock> s,
                 std::chrono::time_point<std::chrono::high_resolution_clock> e);

    bool beat(int scale, std::chrono::time_point<std::chrono::high_resolution_clock> beatTime);

    void draw();

    void fade();

private:

    void drawBeats(int alpha);

    std::unique_ptr<SDL_Texture, SDL_Texture_Deleter> texture;
    std::unique_ptr<SDL_Texture, SDL_Texture_Deleter> point;
    SDL_Renderer& renderer;
    SDL_Rect destination;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    std::chrono::milliseconds msPerBar;
    int beats;
    int bpm;
    int pixelsPerMs;
    int staffs;
    int pixelsPerStaff;
};