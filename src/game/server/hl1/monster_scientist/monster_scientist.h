#ifndef MONSTER_SCIENTIST_H
#define MONSTER_SCIENTIST_H

#include "monster_scientist_intention.h"
#include "monster_scientist_locomotion.h"
#include "monster_scientist_body.h"
#include "NextBot.h"
#include "util_shared.h"

class CMonsterScientist : public NextBotCombatCharacter
{
public:

	DECLARE_CLASS(CMonsterScientist, NextBotCombatCharacter);

	CMonsterScientist();
	virtual ~CMonsterScientist();

	virtual void Spawn();
	virtual void Precache();

	virtual CMonsterScientistIntention* GetIntentionInterface() const { return m_pIntention; };
	virtual CMonsterScientistLocomotion* GetLocomotionInterface() const { return m_pLocomotion; };

	virtual bool HasHumanGibs() { return true; };

	virtual bool ShouldGib(const CTakeDamageInfo& info);

private:

	CMonsterScientistIntention* m_pIntention;
	CMonsterScientistLocomotion* m_pLocomotion;

	CountdownTimer m_tFearTimer;
};

class CMonsterScientistPathCost : public IPathCost
{
public:
	CMonsterScientistPathCost(CMonsterScientist* pScientist) : m_pScientist(pScientist) {};

	float GetJumpPenalty() const { return 5.0f; };

	// Taken from simple bot. This calculates the cost between two areas.
	// If there is a ladder or elevator in the way it will add it to the path cost.
	// We dont support elevators for now.
	virtual float operator()(
		CNavArea* pNextArea,
		CNavArea* pPrevArea,
		const CNavLadder* pLadder,
		const CFuncElevator* pElevator,
		float flLength
		) const
	{
		// First area on our path so theres no cost.
		if (pPrevArea == NULL)
		{
			return 0.0f;
		}

		// Can't reach.
		if (!m_pScientist->GetLocomotionInterface()->IsAreaTraversable(pNextArea))
		{
			return -1.0f;
		}

		float flCalcDist = 0.0f;

		if (pLadder)
		{
			flCalcDist += pLadder->m_length;
		}
		else if (flLength < 0.0)
		{
			flCalcDist += (pNextArea->GetCenter() - pPrevArea->GetCenter()).Length();
		}
		else
		{
			flCalcDist = flLength;
		}

		float flCost = flCalcDist + pPrevArea->GetCostSoFar();

		// Height change check
		float flDeltaZ = pPrevArea->ComputeAdjacentConnectionHeightChange(pNextArea);
		if (flDeltaZ >= m_pScientist->GetLocomotionInterface()->GetStepHeight())
		{
			if (flDeltaZ >= m_pScientist->GetLocomotionInterface()->GetMaxJumpHeight())
			{
				// Too high for a jump.
				return -1.0f;
			}

			flCost += GetJumpPenalty() * flCalcDist;
		}
		else if (flDeltaZ < -m_pScientist->GetLocomotionInterface()->GetDeathDropHeight())
		{
			// Too far to drop.
			return -1.0f;
		}

		return flCost;
	};

private:

	CMonsterScientist* m_pScientist;
};

#endif // MONSTER_SCIENTIST_H