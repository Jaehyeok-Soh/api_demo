#pragma once
#include "pch.h"
#include "DTOReceiveRoomInfo.h"

struct DTOReceivePack
{
	DTOReceiveRoomInfo currentRoom;
	vector<string> accountList;
};

inline void to_json(nlohmann::json& j, const DTOReceivePack& p)
{
	j["currentRoom"] = p.currentRoom;
	j["accountList"] = p.accountList;
}

inline void from_json(const nlohmann::json& j, DTOReceivePack& p)
{
	j.at("currentRoom").get_to(p.currentRoom);
	j.at("accountList").get_to(p.accountList);
}