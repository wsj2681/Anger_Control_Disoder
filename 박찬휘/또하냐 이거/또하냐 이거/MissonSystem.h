#pragma once


// 머리 때리기
// 몸 때리기
// 막기 성공

enum MissionType
{
	Hit_Head,
	Hit_Body,
	Gaurd
};

struct Mission
{
	Mission(UINT type);
	int count = 0;
	UINT missionType = 0;

	void MissionClear();
	void MissionFail();
};

class MissonSystem final
{
public:
	MissonSystem() = default;
	MissonSystem(const MissonSystem&) = delete;
	MissonSystem& operator=(const MissonSystem) = delete;
	~MissonSystem() = default;

private:

	map<string, Mission*> mission;

public:

	void SetMissionType(const char* missionName);
	const string& GetMissionType();
	void MissionUpdate();
	void MissionReward();
};

