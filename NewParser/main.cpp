#include "firstParse.hpp"
#include "parseChapter.hpp"
#include <iostream>

int main(const int argc, const char* argv[]) {
	system("chcp 1251>nul");

	// chrono
	auto start = std::chrono::high_resolution_clock::now();
	/////////

	namespace asio = boost::asio;
	std::string mainUrl{ "https://vodochet.ru/" };

	Network::FirstParse parseClient(std::make_unique<boost::asio::io_context>(), mainUrl.c_str());
	parseClient.start();

	// Buffer of website chapters
	std::vector<Network::websiteChapters> chaptersBuffer = Network::parseSiteChapters(mainUrl.c_str());

	// Another threads
	std::vector<std::unique_ptr<std::thread>> threadsArray;
	for (int i = 1; i < chaptersBuffer.size(); i++) {
		std::thread* ptr = new std::thread(parseFunctions::parseChapter,
			chaptersBuffer[i].m_url, chaptersBuffer[i].m_name, true);
		std::unique_ptr<std::thread> tempThread(std::move(ptr));
		threadsArray.push_back(std::move(tempThread));
	}

	// Main thread
	parseFunctions::parseChapter(chaptersBuffer[0].m_url, chaptersBuffer[0].m_name, false);

	for (const auto& elem : threadsArray) {
		elem->join();
	}

	// chrono
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;
	std::cout << "Application worked for " << duration.count() << " seconds.\n";
	/////////

	system("pause>nul");
 
	return 0;
}