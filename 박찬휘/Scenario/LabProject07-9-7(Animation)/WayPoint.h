#pragma once

class WayPoint final
{
public:
	WayPoint() = default;
	virtual ~WayPoint() = default;
private:
	UINT nowState{ STATE_IDLE };
	UINT nWayPoints{ 0 };
	UINT curWayPoints{ 0 };
	XMFLOAT3 wayPoints[20]{ XMFLOAT3(0.0f, 0.0f, 0.0f) };
public:
	const UINT& GetNowState() const;
	void SetNowState(const UINT& state);

	XMFLOAT3* GetWayPoints();
	const XMFLOAT3& GetWayPoint(int index) const;
	void SetWayPoint(XMFLOAT3 way);
	void SetWayPoint(XMFLOAT3 way, int index);

	const UINT& GetCurWayPoints() const;
	void SetCurWayPoints(const UINT& cur);

	bool IsMove();
};