add_executable(websocket_example main.cpp)

# Set C++ standard
set_target_properties(websocket_example PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(websocket_example PRIVATE beam)
target_compile_definitions(websocket_example PRIVATE ASIO_STANDALONE _WEBSOCKETPP_CPP11_STRICT_)
