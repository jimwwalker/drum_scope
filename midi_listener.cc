#include "midi_listener.h"

#include "RtMidi.h"
#include <cstdlib>
#include <iostream>

#include "bar_drawer.h"
#include <chrono>

MidiListener::MidiListener(BarDrawer &bd) : bd(bd), midiin(new RtMidiIn()) {
  if (midiin->getPortCount() == 1) {
    std::cout << "Opening " << midiin->getPortName() << std::endl;
  } else {
    throw std::runtime_error("MidiListener too many ports? too few?");
  }

  midiin->openPort(0);

  midiin->setCallback(&MidiListener::input, (void *)&bd);

  midiin->ignoreTypes(false, false, false);
}

void MidiListener::input(double deltatime, std::vector<unsigned char> *message,
                         void *ud) {

  if (message->size() > 1 && (message->at(0) & 0x90) == 0x90) {
    BarDrawer *bd = reinterpret_cast<BarDrawer *>(ud);
    bd->beat(message->at(1), std::chrono::high_resolution_clock::now());
  }
}