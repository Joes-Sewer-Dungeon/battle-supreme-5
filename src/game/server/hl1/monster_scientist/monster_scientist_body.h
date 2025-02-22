#ifndef MONSTER_SCIENTIST_BODY_H
#define MONSTER_SCIENTIST_BODY_H

#include "NextBotBodyInterface.h"
#include "monster_scientist.h"

class CMonsterScientistBody : public IBody
{
public:
	CMonsterScientistBody(CMonsterScientist* pScientist);
	virtual ~CMonsterScientistBody();

private:
	int iCurActivity;
};

#endif // MONSTER_SCIENTIST_BODY_H