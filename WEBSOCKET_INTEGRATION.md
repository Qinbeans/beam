# WebSocket++ Integration Summary

This document summarizes the integration of websocketpp into the Beam CMake project.

## What was added:

### 1. CMakeLists.txt Changes
- Added FetchContent for standalone ASIO library (asio-1-28-0)
- Added FetchContent for websocketpp library (version 0.8.2)
- Added include directories for both websocketpp and ASIO
- Added compile definitions: ASIO_STANDALONE and _WEBSOCKETPP_CPP11_STRICT_
- Added platform-specific link libraries (ws2_32 and wsock32 for Windows)

### 2. WebSocket Client Wrapper
- Created `include/beam/core/websocket_client.h` - Header file with WebSocketClient class
- Created `src/core/websocket_client.cpp` - Implementation of the WebSocket client
- Added websocket_client.cpp to the beam library sources

### 3. Example Application
- Created `examples/websocket_example/main.cpp` - Example usage of the WebSocket client
- Created `examples/websocket_example/CMakeLists.txt` - Build configuration for the example

## Usage Example:

```cpp
#include "beam/core/websocket_client.h"

beam::WebSocketClient client;

// Set event handlers
client.set_open_handler([]() {
    std::cout << "Connected!" << std::endl;
});

client.set_message_handler([](const std::string& message) {
    std::cout << "Received: " << message << std::endl;
});

// Connect to a WebSocket server
if (client.connect("ws://echo.websocket.org/")) {
    client.send_message("Hello WebSocket!");

    // Keep connection alive for receiving messages
    std::this_thread::sleep_for(std::chrono::seconds(2));

    client.disconnect();
}
```

## Building:

To build the example:
```bash
cmake --build build --config Debug --target websocket_example
```

To build everything:
```bash
cmake --build build --config Debug
```

## Dependencies:
- websocketpp (header-only library)
- Standalone ASIO (header-only library)
- Windows: ws2_32, wsock32
- Unix: pthread (automatically linked)

The integration is complete and the websocket functionality is now available in your Beam framework!
