#pragma once
#include "client.hpp"

void preParse();

namespace Network {
	class FirstParse : public Network::Client {
	public:
		FirstParse(std::unique_ptr<asio::io_context> context, const char* url) : Client(std::move(context), url)
		{
		}
	protected:
		void readHeaderLinesHandler(const boost::system::error_code& ec) override;
		void readResponseBodyHandler(const boost::system::error_code& ec) override;
	};
}