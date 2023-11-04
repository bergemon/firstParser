#include "dependencies.hpp"
#include "firstParse.hpp"

void preParse() {
	{
		std::ofstream moddedFile("intermediateFile.txt", std::ios::out);
	}
	std::ifstream file("tempFile.txt");
	std::ofstream moddedFile("intermediateFile.txt", std::ios::out | std::ios::app);

	if (file.is_open() && moddedFile.is_open()) {
		std::string tmpStr;
		std::string nameStr;
		std::string emailHrefStr;
		std::string imgHrefStr;
		bool firstIteration = true;

		while (getline(file, tmpStr)) {
			if (tmpStr.find("<td class=\"tbLogo\">") != -1 && tmpStr.find("elementHtml") == -1) {
				int firstPos = static_cast<int>(tmpStr.find("data-basicimg="));
				int secondPos = static_cast<int>(tmpStr.find("src="));
				int nameLength = secondPos - firstPos;

				if (tmpStr.find(".svg") != -1)
					imgHrefStr = tmpStr.substr(firstPos + 15, nameLength - 17);
				else
					imgHrefStr = tmpStr.substr(firstPos + 16, nameLength - 18);

				if (firstIteration)
					moddedFile << "?ih> https://vodochet.ru/" << imgHrefStr << '\n';
				else
					moddedFile << '\n' << "?ih> https://vodochet.ru/" << imgHrefStr << '\n';

				firstIteration = false;
			}
			if (tmpStr.find("<td class=\"tbName\">") != -1 && tmpStr.find("elementHtml") == -1) {
				int secondPos = static_cast<int>(tmpStr.find("</a>"));
				int emailHrefPos = static_cast<int>(tmpStr.find("href="));
				int firstPos = static_cast<int>(tmpStr.rfind("\">"));
				int nameLength = secondPos - firstPos;
				int emailHrefLength = firstPos - emailHrefPos;

				nameStr = tmpStr.substr(firstPos + 2, nameLength - 2);
				emailHrefStr = tmpStr.substr(emailHrefPos + 6, emailHrefLength - 6);

				moddedFile << "?n> " << nameStr << '\n';
				moddedFile << "?eh> https://vodochet.ru/" << emailHrefStr << '\n';
			}
		}
		file.close();
		moddedFile.close();

		std::filesystem::remove(std::filesystem::path("tempFile.txt"));
	}
}