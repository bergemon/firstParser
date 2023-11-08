#include "dependencies.hpp"
#include "downloadImages.hpp"

// class methods
void Network::DownloadImages::readHeaderLinesHandler(const boost::system::error_code& ec) {
	std::ofstream file(m_city + '/' + m_fullPathName, std::ios::out);

	if (!ec) {
		std::istream is(&m_response);

		std::string header_line;

		while (getline(is, header_line) && header_line != "\r") {
		#ifdef DEBUG
			std::cout << header_line;
		#endif
		}

		asio::async_read(m_socket, m_response, asio::transfer_at_least(1),
			boost::bind(&DownloadImages::readResponseBodyHandler, this, asio::placeholders::error));
	}
	else if (ec && ec != asio::error::eof) {
		std::cout << "[Read header lines] Error: " << ec.what() << '\n';
	}
}
void Network::DownloadImages::readResponseBodyHandler(const boost::system::error_code& ec) {
	std::ofstream file(m_city + '/' + m_fullPathName, std::ios::out | std::ios::app | std::ios::binary);

	if (!ec) {
		file << &m_response;

		asio::async_read(m_socket, m_response, asio::transfer_at_least(1),
			boost::bind(&DownloadImages::readResponseBodyHandler, this, asio::placeholders::error));
	}
	else if (ec == asio::error::eof) {
		if (file.is_open())
			file.close();
	#ifdef DEBUG
			std::cout << ">> [DOWNLOAD] File " << m_fileName << " has been downloaded.\n\n";
	#endif
	}
	else if (ec && ec != asio::error::eof) {
		std::cout << "[Read response body] Error: " << ec.what() << '\n';
	}
}

// func
void Network::downloadImages(Network::ClientInterface& srcClass) {
	std::ifstream file(srcClass.m_city + '/' + srcClass.m_chapterName + "/intermediateFile.txt");
	std::filesystem::path imagesDir(srcClass.m_city + '/' + srcClass.m_chapterName + "/images");
	bool exists = std::filesystem::directory_entry(imagesDir).exists();
	if (exists) {
		std::filesystem::directory_iterator dirIter(imagesDir);
		for (const auto& elem : dirIter) {
			if (!elem.is_directory())
				std::filesystem::remove(elem);
		}
	}

	if (!exists)
		std::filesystem::create_directory(imagesDir);

	std::string parsedStr;
	std::string imageURI;
	std::string imageName;
	std::vector<Network::Images> imagesURI;
	if (file.is_open()) {
		while (getline(file, parsedStr)) {
			if (parsedStr.find("?ih>") != -1) {
				imageURI = parsedStr.substr(parsedStr.find(" ") + 1, parsedStr.length() - 4);
				imageName = imageURI.substr(imageURI.rfind("/") + 1, imageURI.length());
				imagesURI.push_back(Network::Images(imageURI, imageName));
			}
		}
		for (const auto& elem : imagesURI) {
			Network::DownloadImages di(std::make_unique<boost::asio::io_context>(), elem.m_uri.c_str(), srcClass.m_chapterName, elem.m_name);
			di.start();
		}
	}
}