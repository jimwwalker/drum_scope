#include <chrono>
#include <vector>
#include <SDL2/SDL.h>

#include "bar.h"


class BarList {
public:
    BarList(int beats, int bpm);

    void addBar(SDL_Renderer& renderer,
                SDL_Rect destination,
                int beats,
                int bpm);

    std::chrono::time_point<std::chrono::high_resolution_clock>
        setEpoch(std::chrono::time_point<std::chrono::high_resolution_clock> epoch);

    void wash() {
        for (auto& b : bars) {
            b.fade();
        }
    }

    std::vector<Bar>::iterator end() {
        return bars.end();
    }

    std::vector<Bar>::iterator begin() {
        return bars.begin();
    }

private:
    std::vector<Bar> bars;
    std::chrono::milliseconds msPerBar;
};