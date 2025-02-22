#ifndef MONSTER_SCIENTIST_BEHAVIOR_H
#define MONSTER_SCIENTIST_BEHAVIOR_H

#include "../monster_scientist.h"

class CMonsterScientistBehavior : public Action< CMonsterScientist >
{
public:

	virtual const char* GetName() const { return "ScientistBehavior"; };

	virtual Action< CMonsterScientist >* InitialContainedAction( CMonsterScientist* pScientist );

	virtual ActionResult< CMonsterScientist > Update( CMonsterScientist* pScientist, float flInterval );

};

#endif // MONSTER_SCIENTIST_BEHAVIOR_H