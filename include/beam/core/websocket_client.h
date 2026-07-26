#pragma once

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif
#ifndef _WEBSOCKETPP_CPP11_STRICT_
#define _WEBSOCKETPP_CPP11_STRICT_
#endif
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <thread>
#include <string>
#include <functional>

namespace beam {

    class WebSocketClient {
    public:
        using client = websocketpp::client<websocketpp::config::asio_client>;
        using message_ptr = websocketpp::config::asio_client::message_type::ptr;

        WebSocketClient();
        ~WebSocketClient();

        // Connect to a WebSocket server
        bool connect(const std::string& uri);

        // Disconnect from the server
        void disconnect();

        // Send a message to the server
        void send_message(const std::string& message);

        // Set callback for when a message is received
        void set_message_handler(std::function<void(const std::string&)> handler);

        // Set callback for when connection is opened
        void set_open_handler(std::function<void()> handler);

        // Set callback for when connection is closed
        void set_close_handler(std::function<void()> handler);

        // Check if connected
        bool is_connected() const;

    private:
        client m_client;
        websocketpp::connection_hdl m_hdl;
        std::thread m_thread;
        bool m_connected;

        std::function<void(const std::string&)> m_message_handler;
        std::function<void()> m_open_handler;
        std::function<void()> m_close_handler;

        void on_open(websocketpp::connection_hdl);
        void on_close(websocketpp::connection_hdl);
        void on_message(websocketpp::connection_hdl, message_ptr msg);
        void run();
    };

} // namespace beam
