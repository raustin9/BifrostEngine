#include "core/events.h"

namespace bifrost {
namespace core {

// Event handler singleton
EventHandler* EventHandler::handler_instance = nullptr;

EventHandler::EventHandler() {
    for (usize i = 0; i < EVENT_CODE_AMOUNT; i++) {
        m_events[i].registered_events.clear();
    }

    m_is_initialized = true;
}

// Return a pointer reference to the singleton instance
EventHandler* EventHandler::get_reference() {
    if (handler_instance == nullptr) {
        handler_instance = new EventHandler();
    }

    return handler_instance;
}

// Register an event
bool EventHandler::register_event(
    EventCode code,
    void* listener,
    event_callback callback
) {
    // Check if event has been registered
    usize code_usize = code_as_usize(code);
    usize amount_registered = m_events[code_usize].registered_events.size();
    for (usize i = 0; i < amount_registered; i++) {
        if (m_events[code_usize].registered_events[i].listener == listener) {
            return false;
        }
    }

    RegisteredEvent event = {};
    event.listener = listener;
    event.callback = callback;
    m_events[code_usize].registered_events.push_back(event);

    return true;
}

// Unregister an event
bool EventHandler::unregister_event(
    EventCode code,
    void* listener
) {
    usize code_usize = code_as_usize(code);
    usize amount_registered = m_events[code_usize].registered_events.size();

    if (m_events[code_usize].registered_events.size() == 0) {
        return false;
    }

    for (usize i = 0; i < amount_registered; i++) {
        RegisteredEvent& ev = m_events[code_usize].registered_events[i];
        if (ev.listener == listener) {
            m_events[code_usize].registered_events.erase(
                m_events[code_usize].registered_events.begin() + i
            );

            return true;
        }
    }

    return false;
}

// Fire an event
bool EventHandler::fire_event(EventCode code, void* sender, EventData data) {
    usize code_usize = code_as_usize(code);
    usize amount_registered = m_events[code_usize].registered_events.size();

    if (amount_registered == 0) {
        return false;
    }
    
    for (usize i = 0; i < amount_registered; i++) {
        RegisteredEvent& ev = m_events[code_usize].registered_events[i];
        // auto& func = ev.callback;
        if (ev.callback(code, sender, ev.listener, data)) {
            // A true result of the callback function means that 
            // the event was fired
            return true;
        }
    }

    return false;
}


} // core namespace
} // bifrost namespace
