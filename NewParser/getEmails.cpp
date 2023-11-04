#include "getEmails.hpp"

// methods
void Network::GetEmail::readHeaderLinesHandler(const boost::system::error_code& ec) {
	std::ofstream file("tempEmailHtml.txt", std::ios::out);

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
	std::ofstream file("tempEmailHtml.txt", std::ios::out | std::ios::app | std::ios::binary);
	std::ifstream iFile("tempEmailHtml.txt");

	if (!ec) {
		file << &m_response;
		std::string tempStr;
		std::string email;
		bool found = false;
		while (getline(iFile, tempStr)) {
			if (tempStr.find("class=\"mail\"") != -1 && tempStr.find("</a>") != -1) {
				std::ofstream oFile("parsedEmails.txt", std::ios::out | std::ios::app);
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
	}
	else if (ec == asio::error::eof) {
		if (file.is_open())
			file.close();
	}
	else if (ec && ec != asio::error::eof) {
		std::cout << "[Read response body] Error: " << ec.what() << '\n';
	}
}

void getEmails() {
	{
		std::ofstream oFile("parsedEmails.txt", std::ios::out);
	}
	std::ifstream iFile("intermediateFile.txt");

	std::string tempStr;
	std::string url;
	while (getline(iFile, tempStr)) {
		if (tempStr.find("?eh>") != -1) {
			url = tempStr.substr(tempStr.find("?eh>") + 5, tempStr.length() - 5);
			Network::GetEmail ge(std::make_unique<boost::asio::io_context>(), url.c_str());
			ge.start();
		}
	}

	std::filesystem::remove(std::filesystem::path("tempEmailHtml.txt"));
}