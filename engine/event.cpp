#include "event.h"


Event::Event(int number_of_frames)
    :number_of_frames{number_of_frames}, frame_count{0} {}

void Event::update() {
    ++frame_count;
}

bool Event::is_done() const {
    return frame_count == number_of_frames;
}

void Event::on_start(Engine&) {}

void Event::when_done(Engine&) {}

