#ifndef MONSTER_SCIENTIST_ROAM_H
#define MONSTER_SCIENTIST_ROAM_H

#include "../monster_scientist.h"
#include "Path/NextBotPathFollow.h"

class CMonsterScientistRoam : public Action< CMonsterScientist >
{
public:

	virtual const char* GetName() const { return "Roam"; };

	virtual ActionResult< CMonsterScientist > OnStart( CMonsterScientist* pScientist, Action< CMonsterScientist >* pPreviousAction );
	virtual ActionResult< CMonsterScientist > Update( CMonsterScientist* pScientist, float flInterval );

	virtual EventDesiredResult< CMonsterScientist > OnStuck( CMonsterScientist *pScientist );
	
	virtual EventDesiredResult< CMonsterScientist > OnContact( 
		CMonsterScientist *pScientist, 
		CBaseEntity *pContactEnt, 
		CGameTrace *pResult 
	);

	virtual EventDesiredResult< CMonsterScientist > OnOtherKilled( 
		CMonsterScientist *pScientist, 
		CBaseCombatCharacter *pVictim, 
		const CTakeDamageInfo &pDmgInfo 
	);

private:

	PathFollower m_path;

	CountdownTimer m_tNextRoam;

};

#endif // MONSTER_SCIENTIST_ROAM_H