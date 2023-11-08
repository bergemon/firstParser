#pragma once
#include "dependencies.hpp"

namespace Network {
	namespace asio = boost::asio;
	using asio::ip::tcp;

	class Client {
	protected:
		std::unique_ptr<asio::io_context> m_context;
		asio::ssl::context m_sslContext;
		asio::ssl::stream<tcp::socket> m_socket;
		boost::urls::url m_url;
		std::string m_fixedUrl;
		tcp::resolver m_resolver;
		tcp::resolver::query m_query;
		asio::streambuf m_request;
		asio::streambuf m_response;

	public:
		Client(std::unique_ptr<asio::io_context> context, const char* url) : m_context(std::move(context)),
			m_url(fixUrlString(url)), m_sslContext(asio::ssl::context::method::sslv23_client), m_socket(*m_context, m_sslContext),
			m_resolver(*m_context), m_query(m_url.host(), m_url.scheme())
		{
			m_sslContext.set_default_verify_paths();
		}

		virtual void start() {
			m_resolver.async_resolve(m_query, boost::bind(&Client::resolveHandler, this, asio::placeholders::error, asio::placeholders::iterator));
			m_context->run();
		}

	protected:
		std::string fixUrlString(std::string srcStr);
		void resolveHandler(const boost::system::error_code& ec, tcp::resolver::iterator ep_iterator);
		void connectionHandler(const boost::system::error_code& ec);
		void handshakeHandler(const boost::system::error_code& ec);
		void requestHandler(const boost::system::error_code& ec, size_t bytes);
		void readStatusHandler(const boost::system::error_code& ec);
		virtual void readHeaderLinesHandler(const boost::system::error_code& ec);
		virtual void readResponseBodyHandler(const boost::system::error_code& ec);
	};
}