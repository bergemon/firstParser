#include "clientInterface.hpp"

std::string Network::ClientInterface::getCity(const char* src) {
	std::string tempStr{ src };

	if (m_url.host().find(".") == m_url.host().rfind(".")) {
		return std::string("msk");
	}

	return tempStr.substr(8, tempStr.size() - (tempStr.size() - tempStr.find(".") + 8));
}