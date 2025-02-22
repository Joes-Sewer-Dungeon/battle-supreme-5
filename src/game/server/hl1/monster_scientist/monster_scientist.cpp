#include "cbase.h"
#include "monster_scientist.h"

LINK_ENTITY_TO_CLASS(nb_monster_scientist, CMonsterScientist);

CMonsterScientist::CMonsterScientist()
{
	m_pIntention = new CMonsterScientistIntention( this );
	m_pLocomotion = new CMonsterScientistLocomotion( this );
}

CMonsterScientist::~CMonsterScientist()
{
	if (m_pIntention)
		delete m_pIntention;
	if (m_pLocomotion)
		delete m_pLocomotion;
}

void CMonsterScientist::Spawn()
{
	Precache();
	SetModel("models/scientist.mdl");

	BaseClass::Spawn();

	AddFlag(FL_NPC);
}

void CMonsterScientist::Precache()
{
	BaseClass::Precache();
	PrecacheModel("models/scientist.mdl");

}

bool CMonsterScientist::ShouldGib(const CTakeDamageInfo& info)
{
	if (info.GetDamageType() & DMG_NEVERGIB)
			return false;

	if ((g_pGameRules->Damage_ShouldGibCorpse(info.GetDamageType()) && m_iHealth < GIB_HEALTH_VALUE) || (info.GetDamageType() & DMG_ALWAYSGIB))
		return true;

	return false;
}