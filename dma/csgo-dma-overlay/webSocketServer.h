#pragma once

#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <json.hpp>

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class webSocketServer
{
private:
	websocketpp::server<websocketpp::config::asio> m_server;
	std::set<connection_hdl, std::owner_less<connection_hdl>> m_connections;

public:
	webSocketServer() {

		// Disable all logging
		m_server.clear_access_channels(websocketpp::log::alevel::all);
		m_server.clear_error_channels(websocketpp::log::alevel::all);

		m_server.init_asio();
		m_server.set_open_handler(bind(&webSocketServer::on_open, this, ::_1));
		m_server.set_close_handler(bind(&webSocketServer::on_close, this, ::_1));
		m_server.set_message_handler(bind(&webSocketServer::on_message, this, ::_1, ::_2));
	}

	void on_open(connection_hdl hdl);
	void on_close(connection_hdl hdl);
	void on_message(connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg);
	void on_timer(websocketpp::lib::error_code const& ec);
	void set_timer();

	void run(uint16_t port);

	static void Start();
};