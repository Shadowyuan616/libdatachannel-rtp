#include "rtc/rtc.hpp"
#include "common/idGen.hpp"
#include "signalConnection/wsDataHandler.hpp"
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <algorithm>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
typedef int SOCKET;
#endif

using std::shared_ptr;
using std::weak_ptr;

using nlohmann::json;

std::string localId;

const int BUFFER_SIZE = 2048;

int main()
{
	rtc::Configuration config;

	IdGen idGenerator;
	localId = idGenerator.randomId(4);
	std::cout << "The local ID is " << localId << std::endl;

	auto ws = std::make_shared<rtc::WebSocket>();

	std::promise<void> wsPromise;
	auto wsFuture = wsPromise.get_future();

	ws->onOpen([&wsPromise]()
			   {
		std::cout << "WebSocket connected, signaling ready" << std::endl;
		wsPromise.set_value(); });

	ws->onError([&wsPromise](std::string s)
				{
		std::cout << "WebSocket error" << std::endl;
		wsPromise.set_exception(std::make_exception_ptr(std::runtime_error(s))); });

	ws->onClosed([]()
				 { std::cout << "WebSocket closed" << std::endl; });

	ws->onMessage([&config, wws = make_weak_ptr(ws)](auto data)
				  { 
		auto handler = std::make_shared<wsDataHandler>(config, wws, peerConnectionMap);
		handler->handleMessage(data); });

	ws->open("ws://127.0.0.1:8000");

	std::cout << "Waiting for signaling to be connected..." << std::endl;
	wsFuture.get();

	while (1)
	{
	}
}