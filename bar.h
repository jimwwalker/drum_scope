#include <chrono>
#include <memory>

#include <SDL2/SDL.h>

struct SDL_Texture_Deleter {
    void operator()(SDL_Texture* texture) const {
        SDL_DestroyTexture(texture);
    };
};

class Bar {
public:
    Bar(SDL_Renderer& renderer,
        SDL_Rect destination,
        std::chrono::time_point<std::chrono::high_resolution_clock> scoreEpoch,
        int barIndex,
        int beats,
        int bpm);

    void makeRenderTarget();

    bool beat(int scale, std::chrono::time_point<std::chrono::high_resolution_clock> beatTime);

    void draw();

private:

    void drawBeats();

    std::unique_ptr<SDL_Texture, SDL_Texture_Deleter> texture;
    SDL_Renderer& renderer;
    SDL_Rect destination;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    int beats;
    int bpm;
    int pixelsPerMs;
};