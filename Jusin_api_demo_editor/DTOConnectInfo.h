#pragma once
#include "pch.h"

struct DTOConnectInfo
{
	wstring account;
	bool isHost;
	int netId;
	bool team;
	int job;
};

inline void to_json(json& j, const DTOConnectInfo& c)
{
	j["isHost"] = c.isHost;
	j["netId"] = c.netId;
	j["team"] = c.team;
	j["job"] = c.job;
}

inline void from_json(const json& j, DTOConnectInfo& c)
{
	j.at("isHost").get_to(c.isHost);
	j.at("netId").get_to(c.netId);
	j.at("team").get_to(c.team);
	j.at("job").get_to(c.job);
}