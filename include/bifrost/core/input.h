#pragma once
#include "types.h"
#include "key.h"
#include "buttons.h"

namespace bifrost {

namespace core {

// State of the keys for the keyboard
struct KeyboardState {
    bool keys[Keys::KEYS_MAX_KEY] = { false };
};

struct MouseState {
    types::i32 x, y;
    bool buttons[MouseButtons::MAX_BUTTONS] = { false };
};

// TODO: Input Handler

} // core namespace

} // bifrost namespace
