#include "cbase.h"
#include "bs5_weapon_russell_pistol.h"
#ifdef GAME_DLL
#include "tf_player.h"
#include "ammodef.h"
#endif

IMPLEMENT_NETWORKCLASS_ALIASED(BS5Projectile_RussellPistol, DT_BS5Projectile_RussellPistol)
BEGIN_NETWORK_TABLE(CBS5Projectile_RussellPistol, DT_BS5Projectile_RussellPistol)
END_NETWORK_TABLE()

LINK_ENTITY_TO_CLASS(bs5_projectile_russell_pistol, CBS5Projectile_RussellPistol);
PRECACHE_WEAPON_REGISTER(bs5_projectile_russell_pistol);

CBS5Projectile_RussellPistol::CBS5Projectile_RussellPistol()
{
#ifdef GAME_DLL
	m_bFired = false;
#endif
}

void CBS5Projectile_RussellPistol::Precache()
{
	PrecacheModel(BS5_RUSSELL_PISTOL_MODEL);
	PrecacheSound("Weapon_Pistol.Single");
}

void CBS5Projectile_RussellPistol::SetCustomPipebombModel()
{
	SetModel(BS5_RUSSELL_PISTOL_MODEL);
}

#ifdef GAME_DLL
void CBS5Projectile_RussellPistol::OnHit(CBaseEntity* pOther)
{
	// Dont fire twice.
	if (m_bFired)
		return;

	m_bFired = true;

	CBaseCombatCharacter* pShootTarget = ToBaseCombatCharacter(pOther);
	ShootTarget(pShootTarget);
}

void CBS5Projectile_RussellPistol::OnHitWorld(void)
{
	// Dont fire twice.
	if (m_bFired)
		return;

	m_bFired = true;

	CBaseEntity* pListOfEntities[MAX_PLAYERS_ARRAY_SAFE];
	int iEntities = UTIL_EntitiesInSphere(pListOfEntities, ARRAYSIZE(pListOfEntities), GetAbsOrigin(), GetDamageRadius(), FL_CLIENT | FL_NPC);
	for (int i = 0; i < iEntities; ++i)
	{
		CTFPlayer* pPlayer = ToTFPlayer(pListOfEntities[i]);
		if (pPlayer)
		{
			if (!pPlayer->IsAlive())
				continue;

			CBaseCombatCharacter* pShootTarget = ToBaseCombatCharacter(pPlayer);

			ShootTarget(pShootTarget);
			break;
		}
		else if (pListOfEntities[i]->IsNPC())
		{
			CBaseCombatCharacter* pNPC = ToBaseCombatCharacter(pListOfEntities[i]);
			if (!pNPC)
				continue;
			
			if (!pNPC->IsAlive())
				continue;

			ShootTarget(pNPC);
			break;
		}
	}
}

void CBS5Projectile_RussellPistol::ShootTarget(CBaseCombatCharacter* pTarget)
{
	if (!pTarget)
		return;

	Vector vTargetPos = pTarget->WorldSpaceCenter();
	Vector vCurPos = GetAbsOrigin();
	Vector vToTargetForward = vTargetPos - vCurPos;
	VectorNormalize(vToTargetForward);

	QAngle vToTargetAngle;
	VectorAngles(vToTargetForward, vToTargetAngle);
	SetAbsAngles(vToTargetAngle);

	Vector vAttachPos;
	QAngle vAttachAng;
	GetAttachment("muzzle_pistol", vAttachPos, vAttachAng);

	Vector vAttachForward;
	AngleVectors(vAttachAng, &vAttachForward);

	FireBulletsInfo_t info;
	info.m_flDamage = GetBulletDamage();
	info.m_vecSrc = vAttachPos;
	info.m_vecDirShooting = vAttachForward;
	info.m_iTracerFreq = 1;
	info.m_iShots = 1;
	info.m_pAttacker = GetOwnerEntity();
	info.m_vecSpread = vec3_origin;
	info.m_flDistance = vTargetPos.DistTo(vCurPos) + 100.0f;
	info.m_iAmmoType = GetAmmoDef()->Index("TF_AMMO_PRIMARY");
	info.m_flDamageForceScale = 1.0f;

	FireBullets(info);

	EmitSound("Weapon_Pistol.Single");
}

CBS5Projectile_RussellPistol* CBS5Projectile_RussellPistol::Create(const Vector& position, const QAngle& angles,
	const Vector& velocity, const AngularImpulse& angVelocity,
	CBaseCombatCharacter* pOwner, const CTFWeaponInfo& weaponInfo, int nSkin)
{
	CBS5Projectile_RussellPistol* pGrenade = static_cast<CBS5Projectile_RussellPistol*>(CBaseEntity::CreateNoSpawn("tf_projectile_russell_pistol", position, angles, pOwner));
	if (pGrenade)
	{
		// Set the pipebomb mode before calling spawn, so the model & associated vphysics get setup properly.
		pGrenade->SetPipebombMode();
		DispatchSpawn(pGrenade);

		pGrenade->m_nSkin = nSkin;

		pGrenade->InitGrenade(velocity, angVelocity, pOwner, weaponInfo);

		pGrenade->m_flFullDamage = 0;

		pGrenade->ApplyLocalAngularVelocityImpulse(angVelocity);
	}

	return pGrenade;
}

void CBS5Projectile_RussellPistol::RemovePistol()
{
	AddEffects(EF_NODRAW);
	SetAbsVelocity(vec3_origin);
	SetContextThink(&CBaseGrenade::SUB_Remove, gpGlobals->curtime + 2, "RemoveThink");
	SetTouch(NULL);
}
#endif

#ifdef CLIENT_DLL
const char* CBS5Projectile_RussellPistol::GetTrailParticleName(void)
{
	if (GetTeamNumber() == TF_TEAM_BLUE)
	{
		return "peejar_trail_blu_glow";
	}
	else
	{
		return "peejar_trail_red_glow";
	}
}
#endif