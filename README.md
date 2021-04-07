# VLFW

VLFW is a C++ wrapper for the multi-platform window library, [GLFW](https://www.glfw.org/) that allows it to integrate smoothly into a ValkyrieEngine project without exposing `glfw.h`.

## Creating a Window

### Initialization

Before creating a window, one must first create an instance of `VLFWMain`. This class is responsible for initializing GLFW, and processing window and monitor-related events. The `VLFWMainArgs` struct can be used to adjust some initial behaviour but had sensible defaults.

It should be noted that a maximum of one instance of `VLFWMain` may be constructed at any given time. Constructing more will throw an error.

```cpp
#include VLFW/VLFW.hpp

// All VLFW classes are contained within the nested namespace vlk::vlfw
using namespace vlk;
using namespace vlfw;

void main()
{
    // Default-construct an instance of VLFWMainArgs
    VLFWMainArgs  args {};

    // Wait for incoming window events instead of polling
    args.waitMode = WaitMode::Wait;
    
    // Construct an instance of vlfwMain using the args specified.
    VLFWMain vlfwMain(args);
}
```

### Window Object

With that, we have the necessary setup to create a window object, provided no errors were thrown. The `Window` class follows a similar setup procedure to `VLFWMain`: The `WindowHints` struct contains just about every option one would normally specify with `glfwWindowHint`. Most of these options can be changed after the window has been created, but many, (such as those relating to the window context and framebuffer) cannot.

```cpp
WindowHints hints {};
hints.title = u8"My Cool Window";
hints.resizable = false;
hints.openglProfile = OpenGLProfileType::Core;
```

In order for VLFWMain to be able to see your window, you must construct it as a `vlk::Component<>` provided by [ValkyrieEngineCommon](https://github.com/VD-15/ValkyrieEngineCommon). Failure to do so will mean your window will not recieve events, close or swap it's buffers.

After that's done, start the main event loop and your window should function properly.

```cpp
void main()
{
    // Use default args
    VLFWMain vlfwMain;

    // Set up hints how we want
    WindowHints hints { ... };

    // Pass hints to constructor of Window and attach it to a dummy entity
    auto* window = Component<Window>::Create(0, hints);
    
    // Make the window's context current on the main thread
    window->MakeContextCurrent();

    // Start event loop
    vlk::Application::Start(ApplicationStartArgs{});
}
```

### Stopping ValkyrieEngine

By default, closing any window will halt the main event loop. If you *don't* want a window to stop the event loop when closed, specify the behaviour as a window hint before constructing the window.

```cpp
WindowArgs args {};
args.raiseStopOnClose = false;
```

### Using GLFW Yourself

Should you want to forego use of `VLFWMain` or just want a bit more control over your application, most VLFW objects have a way of exposing their internal handle. Since VLFW does not expose `glfw.h`, most of these handles will be of type `void*` or an alias thereof that can be `reinterpret_cast`ed into their appropriate type once you include `glfw.h`.

```cpp
#include "glfw/glfw3.h"

Window* vlfwWindow = new Window();

GLFWwindow* glfwWindow = reinterpret_cast<GLFWwindow*>(
    vlfwWindow->GetHandle());
```

## Recieving Events

More or less every callback that exists in GLFW has a VLFW counterpart that can be listened to like any other ValkyrieEngine event. Most of these events will be sent during a `vlk::PreUpdateEvent` as that's when `VLFWMain` will poll for them.

```cpp
class MyEventListener :
    public EventListener<Window::MoveEvent>
{
    public:
    void OnEvent(const Window::MoveEvent& ev)
    {
        std::cout << "Window Moved: "
                  << std::to_string(ev.newPosition.X())
                  << ", "
                  << std::to_string(ev.newPosition.Y())
                  << std::endl;
    }
};
```

## Recieving Input

Keyboard and Mouse input can be recieved in two ways: listening for the events sent by the `Window` class or using the `Keyboard` and `Mouse` classes. Each have their pros and cons.

The `Keyboard` and `Mouse` classes are most suited to detecting input on a frame-by-frame basis and are what you would typically expect for a game engine. They report what keys are down, up, pressed and released on that particular frame. Input can be polled at any time during the main update loop. Please keep in mind that it is possible for a key to be both pressed and released on the same frame and if your application is running slow, there's a strong possibility this will occur.

Alternatively, event listening is the only way to achieve input on a per-window basis as the `Keyboard` and `Mouse` classes ignore the `window` paramater of the events they recieve. If you're creating a non-game application or are using multiple windows, this may be the option for you. Please note that these events are sent in the first available `PreUpdateEvent`, rather than as they happen.

### Keyboard Input

The `Window` class will send the following key-related events when it recieves the appropriate callback from GLFW:

- `Window::KeyDownEvent`
- `Window::KeyUpEvent`
- `Window::KeyRepeatEvent`
- `Window::CharTypeEvent`

One can either listen for these events or use the `Keyboard` class In order to poll for individual keys. The `Keyboard` class requires an instance of `KeyboardMain` in order to function properly. This must be set up in a similar manor to `VLFWMain`:

```cpp
// KeyboardArgs is currently an empty placeholder struct, but options may be added later
KeyboardArgs args {};
KeyboardMain kbdMain(args);
```

### Text Input

Using the `Keyboard` class for text input is discouraged for various reasons, instead it is recommended to listen for `Window::CharTypeEvent`.

```cpp
class MyTextReciever :
    public EventListener<Window::CharTypeEvent>
{
    public:
    // The codepoint is a UTF-32 character, so we're using a u32string
    std::u32string data;

    OnEvent(const Window::CharTypeEvent& ev)
    {
        data.append(ev.codepoint);
    }
};
```

### Mouse Input

The `Window` class will send the following key-related events when it recieves the appropriate callback from GLFW:

- `Window::CursorEnterEvent`
- `Window::CursorLeaveEvent`
- `Window::MouseButtonDownEvent`
- `Window::MouseButtonUpEvent`
- `Window::MouseMoveEvent`
- `Window::ScrollEvent`

One can either listen for these events or use the `Mouse` class In order to poll for individual buttons or movements. The `Mouse` class requires an instance of `MouseMain` in order to function properly. This must be set up in a similar manor to `VLFWMain`:

```cpp
// MouseArgs is currently an empty placeholder struct, but options may be added later
MouseArgs args {};
MouseMain mseMain(args);
```

## Limitations

Now you know what VLFW can do, here's a list of things it currently can't do but may be implemented in the future:

- Shared context
- File Drop callback
- Gamepad support
- Joystick support
- GLFW extensions
