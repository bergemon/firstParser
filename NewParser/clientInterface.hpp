#pragma once
#include "client.hpp"

namespace Network {
	struct websiteChapters {
		std::string m_url;
		std::string m_name;

		websiteChapters(std::string url, std::string name) : m_url(url), m_name(name)
		{}
	};

	class ClientInterface : public Client {
	protected:
		std::string m_city;
		std::string m_chapterName;

		std::string getCity(const char* src);
	public:
		ClientInterface(std::unique_ptr<asio::io_context> context, const char* url, std::string chapter)
			: Client(std::move(context), url), m_chapterName(chapter), m_city(getCity(url))
		{ }

		void start() override {
			m_resolver.async_resolve(m_query, boost::bind(&ClientInterface::resolveHandler, this, asio::placeholders::error, asio::placeholders::iterator));
			if (!std::filesystem::directory_entry(m_city + '/' + m_chapterName).exists())
				std::filesystem::create_directory(m_city + '/' + m_chapterName);
			m_context->run();
		}

		friend void preParse(Network::ClientInterface& srcClass, std::string siteUrl);
		friend void downloadImages(Network::ClientInterface& srcClass);
		friend void getEmails(Network::ClientInterface& srcClass);
		friend void finalParse(Network::ClientInterface& srcClass);
	};

	void preParse(Network::ClientInterface& srcClass, std::string siteUrl);
	void downloadImages(Network::ClientInterface& srcClass);
	void getEmails(Network::ClientInterface& srcClass);
	void finalParse(Network::ClientInterface& srcClass);
}