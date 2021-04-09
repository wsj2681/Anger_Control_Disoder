#include "stdafx.h"
#include "Object.h"
#include "WayPoint.h"

const UINT& WayPoint::GetNowState() const
{
	return nowState;
}

void WayPoint::SetNowState(const UINT& state)
{
	nowState = state;
}

XMFLOAT3* WayPoint::GetWayPoints()
{
	return wayPoints;
}

const XMFLOAT3& WayPoint::GetWayPoint(int index) const
{
	return wayPoints[index];
}

void WayPoint::SetWayPoint(XMFLOAT3 way)
{
	wayPoints[nWayPoints++] = way;
}

void WayPoint::SetWayPoint(XMFLOAT3 way, int index)
{
	wayPoints[index] = way;
	if (nWayPoints <= index)
		nWayPoints = index;
}

const UINT& WayPoint::GetCurWayPoints() const
{
	return curWayPoints;
}

void WayPoint::SetCurWayPoints(const UINT& cur)
{
	curWayPoints = cur;
}

bool WayPoint::IsMove()
{
	return nowState == STATE_MOVE;
}