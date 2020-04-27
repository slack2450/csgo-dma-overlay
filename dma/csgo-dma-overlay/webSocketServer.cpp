#include "webSocketServer.h"
#include "Entities.h"

void webSocketServer::on_open(connection_hdl hdl) {
    printf("[+] Client connected to websocket\n");
    m_connections.insert(hdl);
}

void webSocketServer::on_close(connection_hdl hdl) {
    m_connections.erase(hdl);
}

void webSocketServer::on_message(connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg) {
    for (auto it : m_connections) {
        std::cout << msg->get_raw_payload() << std::endl;
        m_server.send(it,msg);
    }
}

void webSocketServer::on_timer(websocketpp::lib::error_code const& ec)
{
    Entities::update();
    std::string data = Entities::serialise();
    for (auto it : m_connections) {
        m_server.send(it, data, websocketpp::frame::opcode::text);
    }

    set_timer();
}

void webSocketServer::set_timer()
{
    m_server.set_timer(
        0,
        websocketpp::lib::bind(
            &webSocketServer::on_timer,
            this,
            websocketpp::lib::placeholders::_1
            )
        );
}

void webSocketServer::run(uint16_t port)
{
    m_server.listen(port);
    m_server.start_accept();

    set_timer();

    m_server.run();
}

void webSocketServer::Start()
{
    printf("[*] Started websocket!\n");
    webSocketServer server;
    server.run(9002);
}