#include "downloadImages.hpp"
#include "getEmails.hpp"
#include "firstParse.hpp"

int main(const int argc, const char* argv[]) {
	system("chcp 1251>nul");

	namespace asio = boost::asio;

	auto start = std::chrono::high_resolution_clock::now();

	Network::FirstParse parseClient(std::make_unique<boost::asio::io_context>(), "https://vodochet.ru/");
	parseClient.start();

	std::vector<Network::websiteChapters> chaptersBuffer = Network::parseSiteChapters();
	Network::ClientInterface clientClass(std::make_unique<boost::asio::io_context>(), chaptersBuffer[0].m_url.c_str(), chaptersBuffer[0].m_name);

	Network::preParse(clientClass);
	Network::downloadImages(clientClass);
	Network::getEmails(clientClass);
	Network::finalParse(clientClass);
	
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;
	std::cout << "Application worked for " << duration.count() << " seconds.\n";

	system("pause>nul");
 
	return 0;
}