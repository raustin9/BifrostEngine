/// BIFROST GAME ENGINE
/// core is the library that handles platform specific interfaces
/// and other utilities used across the library


#pragma once
#include "types.h"
#include "defines.h"

#include <string>

using namespace bifrost::common::types;

namespace bifrost {
namespace core {

/// Window is the GUI window that is opened and contains all the events
class Window {
public:
    Window(u32 width, u32 height, std::string title)
        : m_width(width)
        , m_height(height)
        , m_title(title)
        , m_can_resize(false) 
    {
        _init();
    }
    Window(const Window&) = delete; // do not allow the window to be copied
    Window(Window&& other);

    ~Window();

    // allow the window to be resized
    void allow_resize() { m_can_resize = true; }

    // open and show the window
    void show();
private:

    u32 m_width, m_height; // the dimensions of the window
    std::string m_title;   // the  title of the window to be displayed at the top
    bool m_can_resize;     // whether we are allowed to resize the window

    void _init();
};

} // core namespace
} // bifrost namespace
