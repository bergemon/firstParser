#include "getEmails.hpp"

// methods
void Network::GetEmail::readHeaderLinesHandler(const boost::system::error_code& ec) {
	std::ofstream(m_city + '/' + m_chapterName + "/tempEmailHtml.txt", std::ios::out);

	if (!ec) {
		std::istream is(&m_response);

		std::string header_line;

		while (getline(is, header_line) && header_line != "\r") {
		#ifdef DEBUG
			std::cout << header_line;
		#endif
		}

		asio::async_read(m_socket, m_response, asio::transfer_at_least(1),
			boost::bind(&GetEmail::readResponseBodyHandler, this, asio::placeholders::error));
	}
	else if (ec && ec != asio::error::eof) {
		std::cout << "[Read header lines] Error: " << ec.what() << '\n';
	}
}
void Network::GetEmail::readResponseBodyHandler(const boost::system::error_code& ec) {
	std::ofstream file(m_city + '/' + m_chapterName + "/tempEmailHtml.txt", std::ios::out | std::ios::app);
	std::ofstream oFile(m_city + '/' + m_chapterName + "/parsedEmails.txt", std::ios::out | std::ios::app);
	std::ifstream iFile(m_city + '/' + m_chapterName + "/tempEmailHtml.txt");
	bool found = false;

	if (!ec) {
		file << &m_response;
		std::string tempStr;
		std::string email;
		while (getline(iFile, tempStr)) {
			if (tempStr.find("class=\"mail\"") != -1 && tempStr.find("</a>") != -1) {
				int32_t rLength = tempStr.length() - tempStr.find("</a>");
				email = tempStr.substr(tempStr.find("class=\"mail\">") + 13, tempStr.length() - (tempStr.find("class=\"mail\">") + 13) - rLength);
				oFile << email << '\n';
				oFile.close();
				found = true;

				break;
			}
		}
		if (!found)
			asio::async_read(m_socket, m_response, asio::transfer_at_least(1),
				boost::bind(&GetEmail::readResponseBodyHandler, this, asio::placeholders::error));

		file.close();
		oFile.close();
		iFile.close();
	}
	else if (ec == asio::error::eof) {
		if (!found)
			oFile << "This company haven't email\n";

		file.close();
		oFile.close();
		iFile.close();
	}
	else if (ec && ec != asio::error::eof) {
		std::cout << "[Read response body] Error: " << ec.what() << '\n';
	}
}

void Network::getEmails(Network::ClientInterface& srcClass) {
	{
		std::ofstream oFile(srcClass.m_city + '/' + srcClass.m_chapterName + "/parsedEmails.txt", std::ios::out);
	}
	std::ifstream iFile(srcClass.m_city + '/' + srcClass.m_chapterName + "/intermediateFile.txt");

	std::string tempStr;
	std::string url;
	while (getline(iFile, tempStr)) {
		if (tempStr.find("?eh>") != -1) {
			url = tempStr.substr(tempStr.find("?eh>") + 5, tempStr.length() - 5);
			Network::GetEmail ge(std::make_unique<boost::asio::io_context>(), url.c_str(), srcClass.m_chapterName);
			ge.start();
		}
	}
	iFile.close();

	std::filesystem::remove(srcClass.m_city + '/' + srcClass.m_chapterName + "/tempEmailHtml.txt");
}