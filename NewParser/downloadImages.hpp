#pragma once
#include "clientInterface.hpp"

namespace Network{

	struct Images {
		std::string m_uri;
		std::string m_name;

		Images(std::string uri, std::string name) : m_uri(uri), m_name(name) {}
	};

	class DownloadImages : public ClientInterface {
	public:
		DownloadImages(std::unique_ptr<asio::io_context> context, const char* url, std::string chapter, std::string fileName)
			: ClientInterface(std::move(context), url, chapter), m_fileName(fileName), m_fullPathName(chapter + "/images/" + m_fileName)
		{ }
	protected:
		std::string m_fileName;
		std::string m_fullPathName;
		void readHeaderLinesHandler(const boost::system::error_code& ec) override;
		void readResponseBodyHandler(const boost::system::error_code& ec) override;
	};
}