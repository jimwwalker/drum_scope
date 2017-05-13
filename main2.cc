#include "bar_drawer.h"
#include "midi_listener.h"

int main() {
    BarDrawer bd(1200, 400, 2, 1, 4, 30);
    MidiListener ml(bd);
    bd.run();
    return 0;
}