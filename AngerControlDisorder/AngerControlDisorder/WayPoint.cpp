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

const UINT& WayPoint::GetNWayPoints() const
{
	return nWayPoints;
}

void WayPoint::SetNWayPoints(const UINT& nWayPoints)
{
	this->nWayPoints = nWayPoints;
}

XMFLOAT3* WayPoint::GetWayPoints()
{
	return wayPoints;
}

const XMFLOAT3& WayPoint::GetWayPoint(int index) const
{
	return wayPoints[index];
}

void WayPoint::SetWayPoint(XMFLOAT3 way, int animation)
{
	wayPoints[nWayPoints] = way;
	animations[nWayPoints++] = animation;
}

const UINT& WayPoint::GetCurWayPoints() const
{
	return curWayPoints;
}

void WayPoint::SetCurWayPoints(const UINT& cur)
{
	curWayPoints = cur;
}

const UINT& WayPoint::GetAnimations() const
{
	return animations[curWayPoints];
}

void WayPoint::SetAnimations(const UINT& animations)
{
	this->animations[nAnimations++] = animations;
}

bool WayPoint::IsMove()
{
	return nowState == IDLE;
}