#include "intercept-laser.h"

#include <game/server/weapons.h>

CInterceptLaser::CInterceptLaser(
		CGameWorld *pGameWorld,
		int Owner,
		vec2 Pos,
		CProjectile *pTarget,
		vec2 TargetPos)
		: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Pos = TargetPos;
	m_ID = Server()->SnapNewID();
	m_From = Pos;
	m_StartTick = Server()->Tick();
	m_Owner = Owner;
	GameWorld()->InsertEntity(this);
	pTarget->Destroy();
}

CInterceptLaser::~CInterceptLaser()
{
	Server()->SnapFreeID(m_ID);
}

bool CInterceptLaser::NetworkClipped(int SnappingClient)
{
	return NetworkLineClipped(SnappingClient, m_From, m_Pos);
}

void CInterceptLaser::Tick()
{
	if(Server()->Tick() - m_StartTick > 200)
		Destroy();
}

void CInterceptLaser::Snap(int SnappingClient, int OtherMode)
{
	CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	if(!pObj)
		return;

	//dbg_msg("TEST: m_Pos", "X: %d Y: %d", (int)m_Pos.x, (int)m_Pos.y);
	//dbg_msg("TEST: m_From", "X: %d Y: %d", (int)m_From.x, (int)m_From.y);
	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_FromX = (int)m_From.x;
	pObj->m_FromY = (int)m_From.y;
	pObj->m_StartTick = Server()->Tick() - 25;
}

void CInterceptLaser::Reset()
{
	m_MarkedForDestroy = true;
}

void CInterceptLaser::Destroy()
{
	GameWorld()->CreateExplosion(m_Pos, m_Owner, WEAPON_LASER, WEAPON_ID_LASER, 3, false);
	CEntity::Destroy();
}