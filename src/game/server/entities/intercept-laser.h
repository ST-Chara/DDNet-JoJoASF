/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_ENTITIES_ILASER_H
#define GAME_SERVER_ENTITIES_ILASER_H

#include <game/server/entity.h>
#include "projectile.h"

class CInterceptLaser : public CEntity
{
public:
	CInterceptLaser(
		CGameWorld *pGameWorld,
		int Owner,
		vec2 Pos,
		CProjectile *pTarget,
		vec2 TargetPos);
	~CInterceptLaser();

	virtual bool NetworkClipped(int SnappingClient) override;
	virtual void Snap(int SnappingClient, int OtherMode) override;
	virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Tick() override;

private:
	int m_StartTick;
	vec2 m_From;
	vec2 m_Dir;
	int m_ID;
	int m_Owner;
};

#endif