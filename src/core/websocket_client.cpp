#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif
#ifndef _WEBSOCKETPP_CPP11_STRICT_
#define _WEBSOCKETPP_CPP11_STRICT_
#endif
#include "beam/core/websocket_client.h"
#include <iostream>
#include <chrono>

namespace beam {

    WebSocketClient::WebSocketClient() : m_connected(false) {
        // Set logging to only warnings and errors
        m_client.set_access_channels(websocketpp::log::alevel::connect);
        m_client.set_error_channels(websocketpp::log::elevel::all);

        // Initialize ASIO
        m_client.init_asio();

        // Set handlers
        m_client.set_open_handler([this](websocketpp::connection_hdl hdl) {
            this->on_open(hdl);
        });

        m_client.set_close_handler([this](websocketpp::connection_hdl hdl) {
            this->on_close(hdl);
        });

        m_client.set_message_handler([this](websocketpp::connection_hdl hdl, message_ptr msg) {
            this->on_message(hdl, msg);
        });
    }

    WebSocketClient::~WebSocketClient() {
        disconnect();
    }

    bool WebSocketClient::connect(const std::string& uri) {
        try {
            websocketpp::lib::error_code ec;
            client::connection_ptr con = m_client.get_connection(uri, ec);

            if (ec) {
                std::cerr << "Could not create connection: " << ec.message() << std::endl;
                return false;
            }

            m_hdl = con->get_handle();
            m_client.connect(con);

            // Start the ASIO io_service run loop in a separate thread
            m_thread = std::thread([this]() {
                this->run();
            });

            // Wait a bit for connection to establish
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            return true;
        } catch (const std::exception& e) {
            std::cerr << "Connection failed: " << e.what() << std::endl;
            return false;
        }
    }

    void WebSocketClient::disconnect() {
        if (m_connected) {
            websocketpp::lib::error_code ec;
            m_client.close(m_hdl, websocketpp::close::status::normal, "Client disconnecting", ec);

            if (ec) {
                std::cerr << "Error on disconnect: " << ec.message() << std::endl;
            }
        }

        m_client.stop();

        if (m_thread.joinable()) {
            m_thread.join();
        }

        m_connected = false;
    }

    void WebSocketClient::send_message(const std::string& message) {
        if (!m_connected) {
            std::cerr << "Cannot send message: not connected" << std::endl;
            return;
        }

        websocketpp::lib::error_code ec;
        m_client.send(m_hdl, message, websocketpp::frame::opcode::text, ec);

        if (ec) {
            std::cerr << "Error sending message: " << ec.message() << std::endl;
        }
    }

    void WebSocketClient::set_message_handler(std::function<void(const std::string&)> handler) {
        m_message_handler = handler;
    }

    void WebSocketClient::set_open_handler(std::function<void()> handler) {
        m_open_handler = handler;
    }

    void WebSocketClient::set_close_handler(std::function<void()> handler) {
        m_close_handler = handler;
    }

    bool WebSocketClient::is_connected() const {
        return m_connected;
    }

    void WebSocketClient::on_open(websocketpp::connection_hdl hdl) {
        m_connected = true;
        std::cout << "Connection opened" << std::endl;

        if (m_open_handler) {
            m_open_handler();
        }
    }

    void WebSocketClient::on_close(websocketpp::connection_hdl hdl) {
        m_connected = false;
        std::cout << "Connection closed" << std::endl;

        if (m_close_handler) {
            m_close_handler();
        }
    }

    void WebSocketClient::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
        std::string payload = msg->get_payload();

        if (m_message_handler) {
            m_message_handler(payload);
        } else {
            std::cout << "Received message: " << payload << std::endl;
        }
    }

    void WebSocketClient::run() {
        try {
            m_client.run();
        } catch (const std::exception& e) {
            std::cerr << "WebSocket run error: " << e.what() << std::endl;
        }
    }

} // namespace beam
