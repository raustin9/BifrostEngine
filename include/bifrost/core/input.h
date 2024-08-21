#pragma once
#include "types.h"
#include "key.h"
#include "buttons.h"
#include <tuple>
#include <qlogger/qlogger.h>

using namespace bifrost::core::types;

namespace bifrost {

namespace core {

// State of the keys for the keyboard
struct KeyboardState {
    bool keys[Keys::KEYS_MAX_KEY] = { false };
};

// State of the mouse
struct MouseState {
    i32 x, y;
    bool buttons[MouseButtons::MAX_BUTTONS] = { false };
};

// The state for the input handler
struct InputState {
    bool is_initialized;
    u32 mouse_x, 
        mouse_y;
    
    KeyboardState keyboard_curr_state;
    KeyboardState keyboard_prev_state;
    MouseState mouse_curr_state;
    MouseState mouse_prev_state;
};

// TODO: Input Handler
class InputHandler {
public:
    InputHandler();
    ~InputHandler();

    void update(f64 delta_time);
    void process_key(Keys key, bool pressed);
    void process_buttons(MouseButtons button, bool pressed);
    void process_mouse_move(i32 x, i32 y);
    void process_mouse_wheel(i32 z_delta);
    std::tuple<i32, i32> get_mouse_position();
    void process_window_resize(u32 width, u32 height);
private:
    bool _is_button_down(MouseButtons button);
    bool _is_button_up(MouseButtons button);
    bool _was_button_down(MouseButtons button);
    bool _was_button_up(MouseButtons button);
    
    bool _is_key_down(Keys key);
    bool _is_key_up(Keys key);
    bool _was_key_down(Keys key);
    bool _was_key_up(Keys key);

    qlogger::Logger m_logger;
    InputState m_state;
};

} // core namespace

} // bifrost namespace
