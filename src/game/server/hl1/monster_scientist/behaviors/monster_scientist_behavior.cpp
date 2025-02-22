#include "cbase.h"
#include "monster_scientist_behavior.h"
#include "monster_scientist_roam.h"
#include "util.h"

Action< CMonsterScientist >* CMonsterScientistBehavior::InitialContainedAction( CMonsterScientist* pScientist )
{
	return new CMonsterScientistRoam;
}

ActionResult< CMonsterScientist > CMonsterScientistBehavior::Update( CMonsterScientist* pScientist, float flInterval )
{
	return Continue();
}

EventDesiredResult< CMonsterScientist > CMonsterScientistBehavior::OnKilled(CMonsterScientist* me, const CTakeDamageInfo& info)
{
	Vector vDmgForce = me->CalcDamageForceVector(info);

	me->BecomeRagdoll(info, vDmgForce);

	return TryDone();
}