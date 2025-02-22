#include "cbase.h"
#include "monster_scientist_roam.h"
#include "../monster_scientist.h"

// To iterate through the navmesh you need a functor class.
// It will run code on this class every time it iterates through
// a navmesh area. This one specificly will stop at a certain
// area in the list.
class SelectNthAreaFunctor
{
public:
	SelectNthAreaFunctor( int count )
	{
		m_count = count;
		m_area = NULL;
	}

	bool operator() ( CNavArea* area )
	{
		m_area = area;
		return ( m_count-- > 0 );
	}

	int m_count;
	CNavArea* m_area;
};

ActionResult< CMonsterScientist > CMonsterScientistRoam::OnStart( CMonsterScientist* pScientist, Action< CMonsterScientist >* pPreviousAction )
{
	m_path.SetMinLookAheadDistance( 300.0f );
	return Continue();
}

ActionResult< CMonsterScientist > CMonsterScientistRoam::Update( CMonsterScientist* pScientist, float flInterval )
{
	if( !pScientist->IsAlive() )
	{
		return Done();
	}

	if( m_path.IsValid() && !m_tNextRoam.IsElapsed() )
	{
		m_path.Update( pScientist );
	}
	else
	{
		// Find a random area that we should go to.
		SelectNthAreaFunctor selection( RandomInt( 0, TheNavMesh->GetNavAreaCount() - 1 ) );
		TheNavMesh->ForAllAreas( selection );

		if (selection.m_area)
		{
			CMonsterScientistPathCost cost( pScientist );
			m_path.Compute( pScientist, selection.m_area->GetRandomPoint(), cost );
		}

		m_tNextRoam.Start( RandomFloat( 5.0f, 10.0f ) );
	}
	return Continue();
}

EventDesiredResult< CMonsterScientist > CMonsterScientistRoam::OnStuck( CMonsterScientist* pScientist )
{
	m_path.Invalidate();

	return TryContinue( RESULT_TRY );
}

EventDesiredResult< CMonsterScientist > CMonsterScientistRoam::OnContact(
	CMonsterScientist* pScientist,
	CBaseEntity* pContactEnt,
	CGameTrace* pResult = NULL
)
{
	return TryContinue( RESULT_TRY );
}

EventDesiredResult< CMonsterScientist > CMonsterScientistRoam::OnOtherKilled(
	CMonsterScientist* pScientist,
	CBaseCombatCharacter* pVictim,
	const CTakeDamageInfo& pDmgInfo
)
{
	return TryContinue( RESULT_TRY );
}