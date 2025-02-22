#include "cbase.h"
#include "monster_scientist_locomotion.h"
#include "monster_scientist.h"

float CMonsterScientistLocomotion::GetRunSpeed()
{
	return 120.0f;
}
float CMonsterScientistLocomotion::GetStepHeight()
{
	return 12.0f;
}
float CMonsterScientistLocomotion::GetMaxJumpHeight()
{
	return 32.0f;
}

bool CMonsterScientistLocomotion::ShouldCollideWith(const CBaseEntity* pEntity)
{
	return true;
}

float CMonsterScientistLocomotion::GetMaxYawRate()
{
	return 15.0f;
}