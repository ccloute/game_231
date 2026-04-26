#include "events.h"
#include <algorithm>

void Events::add(const std::shared_ptr<Event>& event) {
    events.push_back(event);
}

void Events::execute(Engine& engine) {
    if (empty()) {
        return;
    }

    // When an event is done, there may be some subsequent events to
    // run. Collect those in here, then add them to the event queue.
    std::vector<std::shared_ptr<Event>> next_events;
    
    // make a copy since event->execute() or event->when_done() could
    // modify events by adding new ones
    auto current_events = events;

    // process all current events
    for (const auto& event : current_events) {
        if (!event->initialized) {
            event->on_start(engine);
            event->initialized = true;
        }
        
        event->execute(engine);
        event->update();
        
        if (event->is_done()) {
            event->when_done(engine); // event clean-up

            // collect any next events
            next_events.insert(next_events.end(),
                               std::make_move_iterator(event->next_events.begin()),
                               std::make_move_iterator(event->next_events.end()));
            // the contents of event->next_events are undefined after move, empty it
            event->next_events.clear();
        }
    }

    remove_done_events();

    // add any new events to event queue
    events.insert(events.end(),
                  std::make_move_iterator(next_events.begin()),
                  std::make_move_iterator(next_events.end()));
}

bool Events::empty() const {
    return events.empty();
}

void Events::remove_done_events() {
    std::erase_if(events, [](const std::shared_ptr<Event>& event) { return event->is_done(); });
}
