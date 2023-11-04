#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif _WIN32

#define BOOST_NETWORK_ENABLE_HTTPS
#pragma comment (lib, "crypt32")

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/url.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <filesystem>
#include <vector>
#include <chrono>

//#define DEBUG