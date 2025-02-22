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

	virtual INextBotEventResponder* FirstContainedResponder(void) const 
	{
		return m_pCurrentBehavior;
	} 

	virtual INextBotEventResponder* NextContainedResponder(INextBotEventResponder* current) const 
	{
		return 0;
	}

	virtual void Reset();
	virtual void Update();


private:
	Behavior<CMonsterScientist>* m_pCurrentBehavior;
	CMonsterScientist* m_pScientist;
};

#endif // MONSTER_SCIENTIST_INTENTION_H