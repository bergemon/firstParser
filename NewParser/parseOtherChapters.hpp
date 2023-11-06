#pragma once
#include "clientInterface.hpp"

namespace Network {
	class ParseOtherChapters : public ClientInterface {
	public:
		ParseOtherChapters(std::unique_ptr<asio::io_context> context, const char* url, std::string chapter)
			: ClientInterface(std::move(context), url, chapter)
		{ }
	protected:
		void readHeaderLinesHandler(const boost::system::error_code& ec) override;
		void readResponseBodyHandler(const boost::system::error_code& ec) override;
	};
}