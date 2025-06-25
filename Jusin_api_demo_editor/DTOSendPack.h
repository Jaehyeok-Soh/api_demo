#pragma once
#include "pch.h"

struct DTOSendPack
{
	string account;
	bool match = false;
	int roomId = 0;
	bool isHost = false;
	bool team = true;
	bool isStart = false;
	bool isQuit = false;
};

inline void to_json(nlohmann::json& j, const DTOSendPack& p)
{
	j["account"] = p.account;
	j["match"] = p.match;
	j["roomId"] = p.roomId;
	j["isHost"] = p.isHost;
	j["team"] = p.team;
	j["isStart"] = p.isStart;
	j["isQuit"] = p.isQuit;
}

inline void from_json(const nlohmann::json& j, DTOSendPack& p)
{
	j.at("account").get_to(p.account);
	j.at("match").get_to(p.match);
	j.at("roomId").get_to(p.roomId);
	j.at("isHost").get_to(p.isHost);
	j.at("team").get_to(p.team);
	j.at("isStart").get_to(p.isStart);
	j.at("isQuit").get_to(p.isQuit);
}