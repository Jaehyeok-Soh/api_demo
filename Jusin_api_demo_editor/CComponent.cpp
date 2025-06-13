#include "pch.h"
#include "CComponent.h"

CComponent::CComponent()
	: m_pOwner(nullptr), m_bActive(true)
{
}

CComponent::~CComponent()
{
}

void CComponent::Initialize(CObject* _pOwner)
{
	m_pOwner = _pOwner;
}

int CComponent::Update()
{
	return 0;
}
