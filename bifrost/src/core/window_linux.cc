#include "core/input.h"
#include "core/window.h"
#include "core/defines.h"

#ifdef Q_PLATFORM_LINUX

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <X11/X.h>
/* #include <X11/Xlib.h> */
#include <X11/Xutil.h>
#include <X11/Xatom.h>

namespace bifrost {
namespace core {

// The input handler shared reference
InputHandler* input_handler = InputHandler::get_reference();

// Initialization behavior for the Linux implementation of the Windowing
void Window::_init() {
    m_logger.info("Creating Window [%u, %u]", m_width, m_height);
    m_display = XOpenDisplay(nullptr);
    if (!m_display) {
        // TODO: log failure
        return;
    }
    m_logger.debug("Window: X display created");

    m_screen = DefaultScreen(m_display);
    m_window = XCreateSimpleWindow(
        m_display, 
        RootWindow(m_display, m_screen), 
        10, 10, 
        m_width, m_height, 
        1, 
        BlackPixel(m_display, m_screen), 
        WhitePixel(m_display, m_screen)
    );

    XSelectInput(m_display, m_window, KeyPressMask | KeyReleaseMask | StructureNotifyMask | ExposureMask);
    Atom wm_protocols = XInternAtom(m_display, "WM_PROTOCOLS", true);
    (void) wm_protocols;

    Atom wm_delete_window = XInternAtom(m_display, "WM_DELETE_WINDOW", true);

    XSetWMProtocols(m_display, m_window, &wm_delete_window, 1);

    // Set the Size Hints with the minimum window size
    XSizeHints sizehints;
    sizehints.flags = PMinSize;
    sizehints.min_width = m_width;
    sizehints.min_height = m_height;

    // Tell window manager our hints about the minimum window size
    XSetWMSizeHints(m_display, m_window, &sizehints, XA_WM_NORMAL_HINTS);

    // Request to display the window on the screen, and flush teh request buffer
    XMapWindow(m_display, m_window);

    m_logger.debug("Window: X Window created");

    m_is_initialized = true;
    m_logger.info("Window Created");

    // Set the name of the window
    XStoreName(m_display, m_window, m_title.c_str());
    XFlush(m_display);
}

// Destructor
Window::~Window() {
    if (m_is_initialized) {
        // Window has not been shutdown yet, so shut it down
        m_logger.warn("Window: Window's destructor called before explicitly shutdown");
        this->shutdown();
    }
}

// Shutdown behavior for the Window
void Window::shutdown() {
    if (!m_is_initialized) {
        m_logger.warn("Window: attempting to shutdown window that is not initialized.");
        return;
    }

    m_logger.info("Shutting down window...");
    XDestroyWindow(m_display, m_window);
    XCloseDisplay(m_display);
    m_is_initialized = false;
    m_logger.info("Window shutdown succesfully");
}

// Open and display the window
void Window::show() {
    XMapWindow(m_display, m_window);
    XFlush(m_display);
}

// Set the title of the window. This will mostly be displayed at 
// the top of the window.
void Window::set_title(const std::string& title) {
    if (!m_is_initialized) {
        m_logger.warn("Window: Attempted to set the title of the window on unitialized Window");
        return;
    }

    m_title = title;
    XStoreName(m_display, m_window, title.c_str());
    XFlush(m_display);
    m_logger.info("Set Window title to %s", title.c_str());
}

// Return whether the window should close or not
bool Window::should_close() {
    pump_messages();
    return m_should_close;
}

// Handle the X11 Window events
bool Window::pump_messages() {
    XEvent event;
    while ((XPending(m_display) > 0)) {
        XNextEvent(m_display, &event);
        _handle_x11_event(event);
    }
    XFlush(m_display);

    return true;
}

// Handle an incoming event from the X server
void Window::_handle_x11_event(XEvent& ev) {
    Keys key = {};
    KeySym keysym;
    xcb_keycode_t keycode = 0;
    
    switch (ev.type) {
        case ClientMessage:
        {
            m_should_close = true;
        } break;
        
        case ConfigureNotify:
        {
            // TODO: handle resizing the window
        } break;
       
        case KeyPress:
        {
            keycode = ev.xkey.keycode;
            keysym = XkbKeycodeToKeysym(m_display, keycode, 0, keycode & ShiftMask ? 1: 0);
            key = _translate_key(keysym);
            InputHandler::get_reference()->process_key(key, true);
        } break;
        case KeyRelease:
        {
            keycode = ev.xkey.keycode;
            keysym = XkbKeycodeToKeysym(m_display, keycode, 0, keycode & ShiftMask ? 1: 0);
            key = _translate_key(keysym);
            InputHandler::get_reference()->process_key(key, false);
        } break;

        default:
            break;
    }
}

Keys Window::_translate_key(u32 code) {
    switch (code) {
        case XK_BackSpace:
            return KEY_BACKSPACE;
        case XK_Return:
            return KEY_ENTER;
        case XK_Tab:
            return KEY_TAB;
            //case XK_Shift: return KEY_SHIFT;
            //case XK_Control: return KEY_CONTROL;

        case XK_Pause:
            return KEY_PAUSE;
        case XK_Caps_Lock:
            return KEY_CAPITAL;

        case XK_Escape:
            return KEY_ESCAPE;

            // Not supported
            // case : return KEY_CONVERT;
            // case : return KEY_NONCONVERT;
            // case : return KEY_ACCEPT;

        case XK_Mode_switch:
            return KEY_MODECHANGE;

        case XK_space:
            return KEY_SPACE;
        case XK_Prior:
            return KEY_PRIOR;
        case XK_Next:
            return KEY_NEXT;
        case XK_End:
            return KEY_END;
        case XK_Home:
            return KEY_HOME;
        case XK_Left:
            return KEY_LEFT;
        case XK_Up:
            return KEY_UP;
        case XK_Right:
            return KEY_RIGHT;
        case XK_Down:
            return KEY_DOWN;
        case XK_Select:
            return KEY_SELECT;
        case XK_Print:
            return KEY_PRINT;
        case XK_Execute:
            return KEY_EXECUTEKEY;
        // case XK_snapshot: return KEY_SNAPSHOT; // not supported
        case XK_Insert:
            return KEY_INSERT;
        case XK_Delete:
            return KEY_DELETE;
        case XK_Help:
            return KEY_HELP;

        case XK_Meta_L:
            return KEY_LWIN;  // TODO: not sure this is right
        case XK_Meta_R:
            return KEY_RWIN;
            // case XK_apps: return KEY_APPS; // not supported

            // case XK_sleep: return KEY_SLEEP; //not supported

        case XK_KP_0:
            return KEY_NUMPAD0;
        case XK_KP_1:
            return KEY_NUMPAD1;
        case XK_KP_2:
            return KEY_NUMPAD2;
        case XK_KP_3:
            return KEY_NUMPAD3;
        case XK_KP_4:
            return KEY_NUMPAD4;
        case XK_KP_5:
            return KEY_NUMPAD5;
        case XK_KP_6:
            return KEY_NUMPAD6;
        case XK_KP_7:
            return KEY_NUMPAD7;
        case XK_KP_8:
            return KEY_NUMPAD8;
        case XK_KP_9:
            return KEY_NUMPAD9;
        case XK_multiply:
            return KEY_MULTIPLY;
        case XK_KP_Add:
            return KEY_ADD;
        case XK_KP_Separator:
            return KEY_SEPARATOR;
        case XK_KP_Subtract:
            return KEY_SUBTRACT;
        case XK_KP_Decimal:
            return KEY_DECIMAL;
        case XK_KP_Divide:
            return KEY_DIVIDE;
        case XK_F1:
            return KEY_F1;
        case XK_F2:
            return KEY_F2;
        case XK_F3:
            return KEY_F3;
        case XK_F4:
            return KEY_F4;
        case XK_F5:
            return KEY_F5;
        case XK_F6:
            return KEY_F6;
        case XK_F7:
            return KEY_F7;
        case XK_F8:
            return KEY_F8;
        case XK_F9:
            return KEY_F9;
        case XK_F10:
            return KEY_F10;
        case XK_F11:
            return KEY_F11;
        case XK_F12:
            return KEY_F12;
        case XK_F13:
            return KEY_F13;
        case XK_F14:
            return KEY_F14;
        case XK_F15:
            return KEY_F15;
        case XK_F16:
            return KEY_F16;
        case XK_F17:
            return KEY_F17;
        case XK_F18:
            return KEY_F18;
        case XK_F19:
            return KEY_F19;
        case XK_F20:
            return KEY_F20;
        case XK_F21:
            return KEY_F21;
        case XK_F22:
            return KEY_F22;
        case XK_F23:
            return KEY_F23;
        case XK_F24:
            return KEY_F24;

        case XK_Num_Lock:
            return KEY_NUMLOCK;
        case XK_Scroll_Lock:
            return KEY_SCROLL;

        case XK_KP_Equal:
            return KEY_NUMPAD_EQUAL;

        case XK_Shift_L:
            return KEY_LSHIFT;
        case XK_Shift_R:
            return KEY_RSHIFT;
        case XK_Control_L:
            return KEY_LCONTROL;
        case XK_Control_R:
            return KEY_RCONTROL;
        // case XK_Menu: return KEY_LMENU;
        /* case XK_Menu: */
        /*     return KEY_RMENU; */
        case XK_semicolon:
            return KEY_SEMICOLON;
        case XK_plus:
            return KEY_PLUS;
        case XK_comma:
            return KEY_COMMA;
        case XK_minus:
            return KEY_MINUS;
        case XK_period:
            return KEY_PERIOD;
        case XK_slash:
            return KEY_SLASH;
        case XK_grave:
            return KEY_GRAVE;

        case XK_a:
        case XK_A:
            return KEY_A;
        case XK_b:
        case XK_B:
            return KEY_B;
        case XK_c:
        case XK_C:
            return KEY_C;
        case XK_d:
        case XK_D:
            return KEY_D;
        case XK_e:
        case XK_E:
            return KEY_E;
        case XK_f:
        case XK_F:
            return KEY_F;
        case XK_g:
        case XK_G:
            return KEY_G;
        case XK_h:
        case XK_H:
            return KEY_H;
        case XK_i:
        case XK_I:
            return KEY_I;
        case XK_j:
        case XK_J:
            return KEY_J;
        case XK_k:
        case XK_K:
            return KEY_K;
        case XK_l:
        case XK_L:
            return KEY_L;
        case XK_m:
        case XK_M:
            return KEY_M;
        case XK_n:
        case XK_N:
            return KEY_N;
        case XK_o:
        case XK_O:
            return KEY_O;
        case XK_p:
        case XK_P:
            return KEY_P;
        case XK_q:
        case XK_Q:
            return KEY_Q;
        case XK_r:
        case XK_R:
            return KEY_R;
        case XK_s:
        case XK_S:
            return KEY_S;
        case XK_t:
        case XK_T:
            return KEY_T;
        case XK_u:
        case XK_U:
            return KEY_U;
        case XK_v:
        case XK_V:
            return KEY_V;
        case XK_w:
        case XK_W:
            return KEY_W;
        case XK_x:
        case XK_X:
            return KEY_X;
        case XK_y:
        case XK_Y:
            return KEY_Y;
        case XK_z:
        case XK_Z:
            return KEY_Z;
        default:
            return KEYS_MAX_KEY;
    }
}

} // core namespace
} // bifrost namespace

#endif // Q_PLATFORM_LINUX
