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
    Bar(SDL_Renderer& renderer, std::pair<int, int> dimension);

    void makeRenderTarget();

    void beat(int scale, std::chrono::time_point<std::chrono::high_resolution_clock> beatTime);

    void draw();

private:
    std::unique_ptr<SDL_Texture, SDL_Texture_Deleter> texture;
    SDL_Renderer& renderer;
    std::pair<int, int> dimension;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};