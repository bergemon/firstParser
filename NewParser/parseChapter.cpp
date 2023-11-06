#include "parseChapter.hpp"
#include "parseOtherChapters.hpp"

void parseFunctions::parseChapter(std::string url, std::string name, bool hasPath) {
	std::string finalUrl;

	boost::urls::url_view boostUrl{ url };
	std::string hostUrlTemp1 = boostUrl.scheme();
	std::string hostUrlTemp2 = boostUrl.host();
	std::string hostUrl = hostUrlTemp1 + "://" + hostUrlTemp2 + '/';

	if (hasPath)
		finalUrl = url.substr(0, url.length() - 1);
	else
		finalUrl = url;

	Network::ParseOtherChapters parseClient(std::make_unique<boost::asio::io_context>(), finalUrl.c_str(), name);
	parseClient.start();

	Network::preParse(parseClient, hostUrl);
	Network::downloadImages(parseClient);
	Network::getEmails(parseClient);
	Network::finalParse(parseClient);
}