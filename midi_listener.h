#include <thread>

#include "RtMidi.h"

class BarDrawer;

#pragma once

class MidiListener {
public:
    MidiListener(BarDrawer& bd);

private:

    static void input(double deltatime, std::vector<unsigned char> *message, void* ud);

    BarDrawer& bd;
    std::thread thread;
    std::unique_ptr<RtMidiIn> midiin;
};