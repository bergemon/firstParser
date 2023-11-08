#include "firstParse.hpp"
#include "parseChapter.hpp"
#include <iostream>

int main(const int argc, const char* argv[]) {
	system("chcp 1251>nul");
	namespace asio = boost::asio;

	if (argc < 2) {
		std::cout << "Not enough parameters for this application.\n";
		std::cout << "Please enter an urls of cities on website https://vodochet.ru/ you want to parse.\n";
		std::cout << "For example https://spb.vodochet.ru/ to parse companies of Saint-Petersburg on this website.\n";
	}
	else {
		// chrono
		auto start = std::chrono::high_resolution_clock::now();
		/////////

		int currentUrl = 1;
		while (currentUrl < argc) {
			Network::FirstParse parseClient(std::make_unique<boost::asio::io_context>(), argv[currentUrl]);
			parseClient.start();

			// Buffer of website chapters
			std::vector<Network::websiteChapters> chaptersBuffer = Network::parseSiteChapters(parseClient, argv[currentUrl++]);
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
		}

		// chrono
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = end - start;
		std::cout << "Application worked for " << duration.count() << " seconds.\n";
		/////////
	}

	system("pause>nul");
 
	return 0;
}