#include "clientInterface.hpp"

std::vector<Network::websiteChapters> Network::parseSiteChapters() {
	{
		std::ofstream("chaptersTemp.txt", std::ios::out);
	}
	std::ifstream file("tempFile.txt");
	std::ofstream chaptersTemp("chaptersTemp.txt", std::ios::out | std::ios::app);

	std::string tempString;
	std::string siteChapters;
	while (getline(file, tempString)) {
		if (tempString.find("first active") != -1) {
			siteChapters = tempString.substr(tempString.find("<ul class=\"\">"), tempString.length() - 13);

			break;
		}
	}

	std::string firstChapter = siteChapters.substr(siteChapters.find("<span>") + 6, siteChapters.find("</span>") - (siteChapters.find("<span>") + 6));
	chaptersTemp << firstChapter << '\n';
	siteChapters = siteChapters.substr(siteChapters.find("<li>"), siteChapters.length() - siteChapters.find("<li>"));

	std::string tempStr{ siteChapters };
	std::string intermediateStr;
	std::vector<std::string> siteChaptersBuffer;
	while (tempStr.find("<li>") != -1) {
		std::string temp = tempStr.substr(tempStr.find("<li>") + 4, tempStr.find("</li>") - (tempStr.find("<li>") + 4));
		tempStr = tempStr.substr(tempStr.find("</li>") + 4, tempStr.length() - (tempStr.find("</li>") + 4));

		chaptersTemp << temp << '\n';
	}

	if (chaptersTemp.is_open())
		chaptersTemp.close();

	std::vector<Network::websiteChapters> chaptersBuffer;
	{
		std::ofstream websiteChapters("websiteChapters.txt", std::ios::out);

		{
			std::ifstream readChapters("chaptersTemp.txt");
			std::ofstream websiteChapters("websiteChapters.txt", std::ios::out | std::ios::app);

			std::string chaptersTmp;
			int chaptersCount = 1;
			std::string tmpName;
			std::string tmpUrl;
			while (getline(readChapters, chaptersTmp)) {
				if (chaptersCount < 2) {
					websiteChapters << "Chapter" << ++chaptersCount << " name/> " << chaptersTmp << '\n';
					websiteChapters << "Chapter" << chaptersCount << " url/> https://vodochet.ru/";
					chaptersBuffer.push_back(Network::websiteChapters("https://vodochet.ru/", chaptersTmp));
				}
				else {
					tmpName = chaptersTmp.substr(chaptersTmp.find(" >") + 2, chaptersTmp.length() - (chaptersTmp.find(" >") + 6));
					websiteChapters << "\n\n" << "Chapter" << ++chaptersCount << " name/> " << tmpName << '\n';
				#ifdef DEBUG
					std::cout << "Name: " << tmpName << '\n';
				#endif

					int endLength = static_cast<int>(chaptersTmp.length() - (chaptersTmp.find("\" >") - chaptersTmp.find("\"") - 1));
					tmpUrl = chaptersTmp.substr(chaptersTmp.find("href=\"") + 6, chaptersTmp.length() - endLength);
					websiteChapters << "Chapter" << chaptersCount << " url/> " << "https://vodochet.ru/" << tmpUrl;

					chaptersBuffer.push_back(Network::websiteChapters(tmpName, "https://vodochet.ru/" + tmpUrl));
				}
			}
			readChapters.close();
			websiteChapters.close();
		}
	}

	std::filesystem::remove("chaptersTemp.txt");
	return chaptersBuffer;
}