#ifndef MONSTER_SCIENTIST_LOCOMOTION_H
#define MONSTER_SCIENTIST_LOCOMOTION_H

#include "NextBotGroundLocomotion.h"
#include "Path/NextBotPathFollow.h"

class CMonsterScientist;

class CMonsterScientistLocomotion : public NextBotGroundLocomotion
{
public:
	CMonsterScientistLocomotion::CMonsterScientistLocomotion( INextBot* pScientist ) :
		NextBotGroundLocomotion(pScientist),
		m_pScientist( (CMonsterScientist*)(pScientist) )
	{};
	virtual ~CMonsterScientistLocomotion() {};

	virtual float GetRunSpeed();
	virtual float GetStepHeight();
	virtual float GetMaxJumpHeight();

	virtual bool ShouldCollideWith(const CBaseEntity* pEntity);

private:
	virtual float GetMaxYawRate();
	CMonsterScientist* m_pScientist;
};



#endif // MONSTER_SCIENTIST_LOCOMOTION_H