#pragma once
#include "pch.h"
#include "CObject.h"

struct DTOPLAYER
{
	UINT m_iObjectId;
	UINT m_iTargetId;
	int m_iHp;
	string m_strName;
	float fX, fY;
	int m_iDir;
	string m_strFrameKey;
	int m_iFrameStart;
	int m_iState;
	int m_iBushOption;
	bool m_bIsUsingSkill;
	int m_iCurrentSkill;
};

inline void to_json(nlohmann::json& j, const DTOPLAYER& p)
{
	j["m_iObjectId"] = p.m_iObjectId;
	j["m_iTargetId"] = p.m_iTargetId;
	j["m_iHp"] = p.m_iHp;
	j["m_strName"] = p.m_strName;
	j["fX"] = p.fX;
	j["fY"] = p.fY;
	j["m_iDir"] = p.m_iDir;
	j["m_strFrameKey"] = p.m_strFrameKey;
	j["m_iFrameStart"] = p.m_iFrameStart;
	j["m_iState"] = p.m_iState;
	j["m_iBushOption"] = p.m_iBushOption;
	j["m_bIsUsingSkill"] = p.m_bIsUsingSkill;
	j["m_iCurrentSkill"] = p.m_iCurrentSkill;
}

inline void from_json(const nlohmann::json& j, DTOPLAYER& p)
{
	j.at("m_iObjectId").get_to(p.m_iObjectId);
	j.at("m_iTargetId").get_to(p.m_iTargetId);
	j.at("m_iHp").get_to(p.m_iHp);
	j.at("m_strName").get_to(p.m_strName);
	j.at("fX").get_to(p.fX);
	j.at("fY").get_to(p.fY);
	j.at("m_iDir").get_to(p.m_iDir);
	j.at("m_strFrameKey").get_to(p.m_strFrameKey);
	j.at("m_iFrameStart").get_to(p.m_iFrameStart);
	j.at("m_iState").get_to(p.m_iState);
	j.at("m_iBushOption").get_to(p.m_iBushOption);
	j.at("m_bIsUsingSkill").get_to(p.m_bIsUsingSkill);
	j.at("m_iCurrentSkill").get_to(p.m_iCurrentSkill);
}