#include "clientInterface.hpp"
#include <string>
#include <codecvt>
#include <locale>

std::string utf8_to_string(const char* utf8str, const std::locale& loc)
{
	// UTF-8 to wstring
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	std::wstring wstr = wconv.from_bytes(utf8str);
	// wstring to string
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	return std::string(buf.data(), buf.size());
}

std::vector<Network::websiteChapters> Network::parseSiteChapters(const char* url) {
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

	{
		std::ofstream websiteChapters("websiteChapters.txt", std::ios::out);
	}
	std::vector<Network::websiteChapters> chaptersBuffer;
	std::ifstream readChapters("chaptersTemp.txt");
	std::ofstream websiteChapters("websiteChapters.txt", std::ios::out | std::ios::app);

	std::string chaptersTmp;
	int chaptersCount = 1;
	std::string tmpName;
	std::string tmpUrl;
	while (getline(readChapters, chaptersTmp)) {
		if (chaptersCount < 2) {
			websiteChapters << "Chapter" << ++chaptersCount << " name/> " << chaptersTmp << '\n';
			websiteChapters << "Chapter" << chaptersCount << " url/> " << url;
			chaptersBuffer.push_back(Network::websiteChapters(url, chaptersTmp));
		}
		else {
			tmpName = chaptersTmp.substr(chaptersTmp.find(" >") + 2, chaptersTmp.length() - (chaptersTmp.find(" >") + 6));
		#ifdef DEBUG
			std::cout << "Name: " << tmpName << '\n';
		#endif
			websiteChapters << "\n\n" << "Chapter" << ++chaptersCount << " name/> " << tmpName << '\n';

			int endLength = static_cast<int>(chaptersTmp.length() - (chaptersTmp.find("\" >") - chaptersTmp.find("\"") - 1));
			tmpUrl = chaptersTmp.substr(chaptersTmp.find("href=\"") + 6, chaptersTmp.length() - endLength);
			websiteChapters << "Chapter" << chaptersCount << " url/> " << url << tmpUrl;

			std::string ansiName = utf8_to_string(tmpName.c_str(), std::locale(".1251"));
			chaptersBuffer.push_back(Network::websiteChapters(url + tmpUrl + '/', ansiName));
		}
	}
	readChapters.close();
	websiteChapters.close();

	//std::filesystem::remove("tempFile.txt");
	std::filesystem::remove("chaptersTemp.txt");
	return chaptersBuffer;
}