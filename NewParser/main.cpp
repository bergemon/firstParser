#include "downloadImages.hpp"
#include "getEmails.hpp"
#include "firstParse.hpp"

int main(const int argc, const char* argv[]) {
	setlocale(LC_ALL, "Rus");

	namespace asio = boost::asio;

	auto start = std::chrono::high_resolution_clock::now();

	Network::FirstParse parseClient(std::make_unique<boost::asio::io_context>(), "https://vodochet.ru/");
	parseClient.start();

	preParse();
	downloadImages();
	getEmails();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;
	std::cout << "Application worked for " << duration.count() << " seconds.\n";

	system("pause>nul");
 
	return 0;
}