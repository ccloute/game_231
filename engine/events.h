#pragma once

#include "event.h"
#include <memory>
#include <vector>

// forward declaration
class Engine;


class Events {
public:
    // add an event to the list of events
    void add(const std::shared_ptr<Event>& event);

    // create a new event to be run immediately
    template <typename T, typename... Args>
    std::shared_ptr<T> create_event(Args&&... args) {
        static_assert(std::is_base_of<Event, T>::value, "T must derive from Event");
        auto event = std::make_shared<T>(std::forward<Args>(args)...);
        if (!event) {
            std::cerr << "Events::create_event(): event is nullptr\n";
        }
        add(event);
        return event;
    }

    // update all currently running events
    void execute(Engine& engine);
    [[nodiscard]] bool empty() const;
    
private:
    std::vector<std::shared_ptr<Event>> events;
    void remove_done_events();
};
