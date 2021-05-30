#include "stdafx.h"
#include "MissonSystem.h"

Mission::Mission(UINT type)
{
	this->missionType = type;
}

void Mission::MissionClear()
{
	// TODO : Clear 조건
}

void Mission::MissionFail()
{
	// TODO : Fail 조건
}

void MissonSystem::SetMissionType(const char* missionName)
{
	mission[missionName] = new Mission(Hit_Head);
}

const string& MissonSystem::GetMissionType()
{
	return string();
}

void MissonSystem::MissionUpdate()
{
}
