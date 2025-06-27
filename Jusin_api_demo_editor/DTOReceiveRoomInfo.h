#pragma once
#include "pch.h"

struct DTOReceiveRoomInfo
{
    int id;
    bool isStart;
    bool isClose;
    vector<string> accountList;
};

inline void to_json(nlohmann::json& j, const DTOReceiveRoomInfo& p)
{
	j["id"] = p.id;
	j["isStart"] = p.isStart;
	j["isClose"] = p.isClose;
	j["accountList"] = p.accountList;
}

inline void from_json(const nlohmann::json& j, DTOReceiveRoomInfo& p)
{
	j.at("id").get_to(p.id);
	j.at("isStart").get_to(p.isStart);
	j.at("isClose").get_to(p.isClose);
	j.at("accountList").get_to(p.accountList);
}