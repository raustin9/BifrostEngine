#include "core/window.h"
#include "core/defines.h"
#include <qlogger.h>

#ifdef Q_PLATFORM_LINUX

#include <X11/Xlib.h>

namespace bifrost {
namespace core {

qlogger::Logger logger = qlogger::Logger();

// Initialization behavior for the Linux implementation of the Windowing
void Window::_init() {
    logger.info("Creating Window [%u, %u]", m_width, m_height);
    m_display = XOpenDisplay(nullptr);
    if (!m_display) {
        // TODO: log failure
        return;
    }
    logger.debug("Window: X display created");

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
    logger.debug("Window: X Window created");

    m_is_initialized = true;
    logger.info("Window Created");

    XStoreName(m_display, m_window, m_title.c_str());
    XFlush(m_display);
}

// Destructor
Window::~Window() {
    if (m_is_initialized) {
        // Window has not been shutdown yet, so shut it down
        logger.warn("Window: Window's destructor called before explicitly shutdown");
        this->shutdown();
    }
}

// Shutdown behavior for the Window
void Window::shutdown() {
    if (!m_is_initialized) {
        logger.warn("Window: attempting to shutdown window that is not initialized.");
        return;
    }

    logger.info("Shutting down window...");
    XDestroyWindow(m_display, m_window);
    XCloseDisplay(m_display);
    m_is_initialized = false;
    logger.info("Window shutdown succesfully");
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
        logger.warn("Window: Attempted to set the title of the window on unitialized Window");
        return;
    }

    m_title = title;
    XStoreName(m_display, m_window, title.c_str());
    XFlush(m_display);
    logger.info("Set Window title to %s", title.c_str());
}

// Return whether the window should close or not
bool Window::should_close() {
    return false;
}

} // core namespace
} // bifrost namespace

#endif // Q_PLATFORM_LINUX
