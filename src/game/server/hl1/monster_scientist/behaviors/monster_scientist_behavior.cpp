#include "cbase.h"
#include "monster_scientist_behavior.h"
#include "monster_scientist_roam.h"

Action< CMonsterScientist >* CMonsterScientistBehavior::InitialContainedAction( CMonsterScientist* pScientist )
{
	return new CMonsterScientistRoam;
}

ActionResult< CMonsterScientist > CMonsterScientistBehavior::Update( CMonsterScientist* pScientist, float flInterval )
{
	return Continue();
}