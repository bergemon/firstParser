#pragma once
#include "client.hpp"

void getCurrentEmail();
void getEmails();

namespace Network {
	class GetEmail : public Client {
		void readHeaderLinesHandler(const boost::system::error_code& ec) override;
		void readResponseBodyHandler(const boost::system::error_code& ec) override;
	public:
		GetEmail(std::unique_ptr<asio::io_context> context, const char* url) : Client(std::move(context), url)
		{
		}
	};
}