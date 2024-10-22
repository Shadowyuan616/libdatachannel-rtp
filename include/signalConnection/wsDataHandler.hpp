#pragma once

#include "rtc\rtc.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <nlohmann/json.hpp> // Assuming you are using nlohmann/json for JSON parsing
#include <unordered_map>
#include <algorithm>

using std::shared_ptr;
using std::weak_ptr;

template <class T>
weak_ptr<T> make_weak_ptr(shared_ptr<T> ptr) { return ptr; }

std::unordered_map<std::string, shared_ptr<rtc::PeerConnection>> peerConnectionMap;
std::unordered_map<std::string, shared_ptr<rtc::DataChannel>> dataChannelMap;

using json = nlohmann::json;

class wsDataHandler
{

private:
    const rtc::Configuration &config;
    std::weak_ptr<rtc::WebSocket> wws;
    std::unordered_map<std::string, shared_ptr<rtc::PeerConnection>> &peerConnectionMap;

public:
    wsDataHandler(const rtc::Configuration &config, std::weak_ptr<rtc::WebSocket> wws, std::unordered_map<std::string, shared_ptr<rtc::PeerConnection>> &peerConnectionMap)
        : config(config), wws(wws), peerConnectionMap(peerConnectionMap) {}

    void handleMessage(const std::variant<rtc::binary, std::string> &data)
    {

        if (!std::holds_alternative<std::string>(data))
            return;

        json message = json::parse(std::get<std::string>(data));

        auto it = message.find("id");
        if (it == message.end())
            return;

        auto id = it->get<std::string>();

        it = message.find("type");
        if (it == message.end())
            return;

        auto type = it->get<std::string>();

        std::shared_ptr<rtc::PeerConnection> pc;
        if (auto jt = peerConnectionMap.find(id); jt != peerConnectionMap.end())
        {
            pc = jt->second;
        }
        else if (type == "offer")
        {
            std::cout << "Answering to " + id << std::endl;
            // pc = createPeerConnection(config, wws, id);
        }
        else
        {
            return;
        }

        if (type == "offer" || type == "answer")
        {
            auto sdp = message["description"].get<std::string>();
            pc->setRemoteDescription(rtc::Description(sdp, type));
        }
        else if (type == "candidate")
        {
            auto sdp = message["candidate"].get<std::string>();
            auto mid = message["mid"].get<std::string>();
            pc->addRemoteCandidate(rtc::Candidate(sdp, mid));
        }
    }
};