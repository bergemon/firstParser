#pragma once
#include "clientInterface.hpp"

namespace Network {
	class GetEmail : public ClientInterface {
		void readHeaderLinesHandler(const boost::system::error_code& ec) override;
		void readResponseBodyHandler(const boost::system::error_code& ec) override;
	public:
		GetEmail(std::unique_ptr<asio::io_context> context, const char* url, std::string chapter) : ClientInterface(std::move(context), url, chapter)
		{ }
	};
}