#include <exception>
#include <string>

#include "bar.h"
#include "bar_drawer.h"


BarDrawer::BarDrawer(int screenX, int screenY, int barX, int barY, int beats, int bpm)
      : list(beats, bpm) {

    const int border = 20;

    if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        throw std::runtime_error("SDL_Init failed " + std::string(SDL_GetError()));
    }

    window.reset(SDL_CreateWindow("Jim's Drum Scope", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenX+(border*2), screenY+(border*2), SDL_WINDOW_SHOWN));

    if (!window)  {
        throw std::runtime_error("SDL_CreateWindow failed " + std::string(SDL_GetError()));
    }

    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

    if (!renderer)  {
        throw std::runtime_error("SDL_CreateRenderer failed " + std::string(SDL_GetError()));
    }

    // calc how big the bars should be.
    int barWidth = screenX/barX;
    int barHeight = screenY/barY;
    for (int i = 0; i < barY; i++) {
        int posY = i * barHeight;
        for (int j = 0; j < barX; j++) {
            int posX = j * barWidth;
            list.addBar(*renderer, {posX+border, posY+border, barWidth, barHeight}, beats, bpm);
        }
    }
    notified = false;
}

void BarDrawer::beat(int midiId, std::chrono::time_point<std::chrono::high_resolution_clock> t) {
    std::unique_lock<std::mutex> lg(lock);
    beatQueue.push(std::make_pair(midiId, t));

    bool expected = false;
    if (notified.compare_exchange_strong(expected, true)) {
    }
}

static int mapMidiToScale(int midiId) {
    switch (midiId) {
        case 42:
        case 46:
        case 22: // hi-hat
            return 3;
        case 48: // hi tom
            return 4;
        case 45: // m tom
            return 5;
        case 41: // low tom
            return 6;
        case 38:
            return 7; // snare
        case 36:
            return 8; // BD
        case 51:
        case 59:
            return 1; //crash
        case 55:
        case 49:
            return 2; //crash
        default:
            printf("Unknown %d\n", midiId );
            return 0;
    }
}

void BarDrawer::run() {
    auto currentBar = list.end(); // first beat sets this to begin
    auto beat1 = std::chrono::high_resolution_clock::now();

    SDL_SetRenderTarget(renderer.get(), NULL);
    for (auto& b: list) {
        b.draw();
    }
    SDL_RenderPresent(renderer.get());

    while (true) {
        bool beat = false;
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if(e.type == SDL_QUIT) {
                return;
            }
        }

        if (notified.load()) {

            notified = false;
            std::unique_lock<std::mutex> lk(lock);
            while (!beatQueue.empty()) {
                if (currentBar == list.end()) {
                    beat1 = std::chrono::high_resolution_clock::now();
                    beat1 = list.setEpoch(beat1);
                    currentBar = list.begin();
                }
                auto b = beatQueue.front();
                currentBar->makeRenderTarget();
                if (currentBar->beat(mapMidiToScale(b.first), b.second)) {
                    currentBar++;
                    if (currentBar == list.end()) {
                        beat1 = list.setEpoch(beat1);
                        currentBar = list.begin();
                        list.wash();
                    }
                    currentBar->makeRenderTarget();
                    currentBar->beat(mapMidiToScale(b.first), b.second);
                }
                beatQueue.pop();
            }
        }

        SDL_SetRenderTarget(renderer.get(), NULL);
        for (auto& b: list) {
            b.draw();
        }
        SDL_RenderPresent(renderer.get());
    }
}