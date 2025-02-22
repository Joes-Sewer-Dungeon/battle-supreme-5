#include "cbase.h"
#include "monster_scientist_intention.h"
#include "monster_scientist.h"
#include "behaviors/monster_scientist_behavior.h"

CMonsterScientistIntention::CMonsterScientistIntention( INextBot* pScientist ) : IIntention( pScientist )
{
	m_pCurrentBehavior = NULL;
	m_pScientist = NULL;
}

void CMonsterScientistIntention::Reset()
{
	m_pScientist = dynamic_cast< CMonsterScientist* >( GetBot()->GetEntity() );
	if (m_pCurrentBehavior)
	{
		delete m_pCurrentBehavior;
	}

	m_pCurrentBehavior = new Behavior< CMonsterScientist >( new CMonsterScientistBehavior );
}

void CMonsterScientistIntention::Update()
{
	if ( m_pCurrentBehavior )
	{
		m_pCurrentBehavior->Update( m_pScientist, GetUpdateInterval() );
	}
}