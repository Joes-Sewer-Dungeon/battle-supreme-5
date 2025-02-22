#ifndef MONSTER_SCIENTIST_INTENTION_H
#define MONSTER_SCIENTIST_INTENTION_H

#include "NextBotIntentionInterface.h"
#include "NextBotBehavior.h"

class CMonsterScientist;

class CMonsterScientistIntention : public IIntention
{
public:
	CMonsterScientistIntention( INextBot* pScientist );
	virtual ~CMonsterScientistIntention() {};

	virtual void Reset();
	virtual void Update();


private:
	Behavior<CMonsterScientist>* m_pCurrentBehavior;
	CMonsterScientist* m_pScientist;
};

#endif // MONSTER_SCIENTIST_INTENTION_H