#ifndef BS5_WEAPON_RUSSELL_PISTOL_H
#define BS5_WEAPON_RUSSELL_PISTOL_H

#include "tf_weapon_jar.h"

#ifdef CLIENT_DLL
#define CBS5WeaponRussellPistol C_BS5WeaponRussellPistol
#define CBS5Projectile_RussellPistol C_BS5Projectile_RussellPistol
#endif // CLIENT_DLL

// Temporary use the cleaver
#define BS5_RUSSELL_PISTOL_MODEL "models/weapons/c_models/c_pistol/c_pistol.mdl"

class CBS5WeaponRussellPistol : public CTFJar
{
public:
	DECLARE_CLASS(CBS5WeaponRussellPistol, CTFJar);
	DECLARE_NETWORKCLASS();

	virtual int			GetWeaponID(void) const { return BS5_WEAPON_RUSSELL_PISTOL; }
	virtual float		GetProjectileSpeed(void);

	virtual void		SecondaryAttack(void);

	virtual const char* GetEffectLabelText(void) { return "#BS5_RUSSELL_PISTOL"; }

	virtual float		InternalGetEffectBarRechargeTime(void) { return 5.1; }

#ifdef GAME_DLL
	virtual const AngularImpulse GetAngularImpulse(void) { return AngularImpulse(0, 500, 0); }
	virtual Vector GetVelocityVector(const Vector& vecForward, const Vector& vecRight, const Vector& vecUp);

	virtual bool		ShouldSpeakWhenFiring(void) { return false; }

	virtual CTFProjectile_Jar* CreateJarProjectile(const Vector& position, const QAngle& angles, const Vector& velocity,
		const AngularImpulse& angVelocity, CBaseCombatCharacter* pOwner, const CTFWeaponInfo& weaponInfo);
#endif

#ifdef CLIENT_DLL
	const char* ModifyEventParticles(const char* token);
	virtual bool		Holster(CBaseCombatWeapon* pSwitchingTo = NULL);
#endif
};

class CBS5Projectile_RussellPistol : public CTFProjectile_Jar
{
public:
	DECLARE_CLASS(CBS5Projectile_RussellPistol, CTFProjectile_Jar);
	DECLARE_NETWORKCLASS();

#ifdef CLIENT_DLL
	virtual const char* GetTrailParticleName(void);
#endif

#ifdef GAME_DLL
	static CBS5Projectile_RussellPistol* Create(const Vector& position, const QAngle& angles, const Vector& velocity,
		const AngularImpulse& angVelocity, CBaseCombatCharacter* pOwner, const CTFWeaponInfo& weaponInfo, int nSkin);
#endif

	CBS5Projectile_RussellPistol();

	virtual int			GetWeaponID(void) const OVERRIDE { return BS5_WEAPON_RUSSELL_PISTOL; }

	virtual void		Precache() OVERRIDE;
	virtual void		SetCustomPipebombModel() OVERRIDE;

#ifdef GAME_DLL

	virtual void		OnHit(CBaseEntity* pOther) OVERRIDE;
	virtual void		OnHitWorld(void) OVERRIDE;

	float				GetBulletDamage() { return 30.0f; };
	virtual float		GetDamage() OVERRIDE { return 15.0f; }
	virtual float		GetDamageRadius() OVERRIDE { return 300.0f; };

	// We dont want to explode.
	virtual bool		ExplodesOnHit() { return false; }
	virtual void		Explode(trace_t* pTrace, int bitsDamageType) OVERRIDE {};
	virtual void		Detonate() OVERRIDE {};

	// No cool impact effects.
	virtual const char* GetImpactEffect() OVERRIDE { return ""; }
	virtual ETFCond		GetEffectCondition(void) OVERRIDE { return TF_COND_INVALID; }

	void				ShootTarget(CBaseCombatCharacter* pTarget);
	void				RemovePistol();

private:

	bool m_bFired;

#endif
};

#endif // BS5_WEAPON_RUSSELL_PISTOL_H