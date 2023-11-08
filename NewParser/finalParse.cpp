#include "clientInterface.hpp"

void Network::finalParse(Network::ClientInterface& srcClass) {
	{
		std::ofstream tempParsed(srcClass.m_city + '/' + srcClass.m_chapterName + "/final.txt", std::ios::out);
	}
	std::ifstream interFile(srcClass.m_city + '/' + srcClass.m_chapterName + "/intermediateFile.txt");
	std::ifstream emails(srcClass.m_city + '/' + srcClass.m_chapterName + "/parsedEmails.txt");
	std::ofstream finallyParsed(srcClass.m_city + '/' + srcClass.m_chapterName + "/final.txt", std::ios::out | std::ios::app);

	std::string tempInter;
	std::string tempEmails;
	std::string tempWorkingStr;

	std::vector<std::string> emailsBuff;

	while (getline(emails, tempEmails)) {
		emailsBuff.push_back(tempEmails);
	}

	int cycleCount = 0;
	bool firstIter = true;
	int cycleMaxCount = static_cast<int>(emailsBuff.size());

	while (getline(interFile, tempInter)) {
		if (tempInter.find("?ih>") != -1) {
			tempWorkingStr = tempInter.substr(tempInter.rfind("/") + 1, tempInter.length() - tempInter.rfind("/") + 1);
			if (!firstIter) {
				finallyParsed << "\n\n" << "[Company image name]> " << tempWorkingStr << '\n';
			}
			else {
				finallyParsed << "[Company image name]> " << tempWorkingStr << '\n';
				firstIter = false;
			}
		}
		else if (tempInter.find("?n>") != -1) {
			tempWorkingStr = tempInter.substr(4, tempInter.length() - 4);
			finallyParsed << "[Company name]> " << tempWorkingStr << '\n';
		}
		else if (tempInter.find("?eh>") != -1) {
			(cycleCount < cycleMaxCount)
				? finallyParsed << "[Company email]> " << emailsBuff[cycleCount++]
				: finallyParsed << "[Company email]> " << " Error - empty";
		}
	}

	interFile.close();
	emails.close();
	finallyParsed.close();

	std::filesystem::remove(srcClass.m_city + '/' + srcClass.m_chapterName + "/intermediateFile.txt");
	std::filesystem::remove(srcClass.m_city + '/' + srcClass.m_chapterName + "/parsedEmails.txt");
}