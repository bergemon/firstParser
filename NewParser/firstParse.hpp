#pragma once
#include "clientInterface.hpp"

namespace Network {
	class FirstParse : public Client {
	public:
		FirstParse(std::unique_ptr<asio::io_context> context, const char* url) : Client(std::move(context), url), m_city(getCity(url))
		{
			if (!std::filesystem::directory_entry(m_city).exists())
				std::filesystem::create_directory(m_city);
		}
	protected:
		std::string m_city;

		std::string getCity(const char* src);
		void readHeaderLinesHandler(const boost::system::error_code& ec) override;
		void readResponseBodyHandler(const boost::system::error_code& ec) override;
		friend std::vector<websiteChapters> parseSiteChapters(Network::FirstParse& srcClass, const char* url);
	};

	std::vector<websiteChapters> parseSiteChapters(Network::FirstParse& srcClass, const char* url);
}