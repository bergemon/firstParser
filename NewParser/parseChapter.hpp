#pragma once
#include "clientInterface.hpp"
#include "downloadImages.hpp"
#include "getEmails.hpp"

namespace parseFunctions {
	void parseChapter(std::string url, std::string name, bool hasPath);
}