#include <game/server/entities/projectile.h>
#include <game/server/entities/intercept-laser.h>
#include <game/server/entities/laser.h>
#include <game/server/weapons.h>
#include <game/server/entities/textentity.h>

#include "star.h"
CStar::CStar(CGameWorld *pGameWorld, CCharacter *pOwner) : IStand(pGameWorld, CDumbEntity::TYPE_PICKUP_LASER, vec2(0,0), pOwner->GetPlayer()->GetCID())
{
    m_Pos = pOwner->GetPos();
    SetHide(false);
    SetEvent(EVENT_FOLLOW_POINT);
}

void CStar::Attack(CCharacter *pTarget)
{
    vec2 Direction = normalize(pTarget->GetPos() - GetPos());
    GameWorld()->CreateHammerHit(GetPos());
    vec2 Temp = pTarget->Core()->m_Vel + normalize(Direction + vec2(0.f, -1.1f)) * 10.0f;
	Temp = ClampVel(pTarget->m_MoveRestrictions, Temp);
	Temp -= pTarget->Core()->m_Vel;
    pTarget->TakeDamage((vec2(0.f, -1.0f) + Temp) * -1.0f, 1, GetOwner(), WEAPON_HAMMER, WEAPON_ID_GRENADE, false);
}

void CStar::Attack(vec2 TargetPos)
{
    if(distance(GameServer()->m_apPlayers[GetOwner()]->GetCharacter()->GetPos(), TargetPos) > 800)
        return;
    m_APoint = TargetPos;
    SetEvent(EVENT_APPROACHING_POINT);
    m_ATick = 250;
}

void CStar::OnStandTick()
{
    if(!GameServer()->m_apPlayers[GetOwner()]->GetCharacter())
        return;

    if(GetState() == EVENT_FOLLOW_POINT)
        SmoothMove(vec2(GameServer()->GetPlayerChar(GetOwner())->m_LatestInput.m_TargetX+GameServer()->GetPlayerChar(GetOwner())->GetPos().x, GameServer()->GetPlayerChar(GetOwner())->m_LatestInput.m_TargetY+GameServer()->GetPlayerChar(GetOwner())->GetPos().y), 27.0f, false);

	if(distance(GetPos(), GameServer()->m_apPlayers[GetOwner()]->GetCharacter()->GetPos()) >= 800 && (GetEvent() == EVENT_FOLLOW_POINT || GetEvent() == EVENT_FOLLOW_MASTER))
		MoveTo(GameServer()->m_apPlayers[GetOwner()]->GetCharacter()->GetPos());

    if(GetEvent() == EVENT_APPROACHING_POINT)
        SmoothMove(m_APoint, 17.0f, false);

    if(m_ATick >= 0)
        m_ATick--;

    if((m_ATick <= 0 && GetEvent() == EVENT_APPROACHING_POINT) || distance(GetPos(), GameServer()->m_apPlayers[GetOwner()]->GetCharacter()->GetPos()) >= 1600)
        SetEvent(EVENT_FOLLOW_POINT); // hmmm, should be "SetEvent(m_LastEvent);" but i dont have much time. so added it to TODO

    HandleInterceptLaser();
}

void CStar::HandleInterceptLaser()
{
    // lmao
    if(GetFireDelay() > 0)
        return;

    for(CProjectile *pTarget = (CProjectile*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_PROJECTILE); pTarget; pTarget = (CProjectile *)pTarget->TypeNext())
    {
        if(pTarget)
        {
            if(distance(pTarget->GetRealPos(), m_Pos) > 128.0f)
                continue;

            // cant through walls.
            if(GameServer()->Collision()->IntersectLine(GetPos(), pTarget->GetRealPos(), NULL, NULL))
                continue;
            
            // Don't Intercept master's bullet(what) 
            if(pTarget->GetOwner() == GetOwner())
                continue;

            // Delete pTarget
            new CInterceptLaser(GameWorld(), GetOwner(), m_Pos, pTarget, pTarget->GetRealPos());

            // Effect & Damage
            GameWorld()->CreateExplosion(pTarget->GetRealPos(), GetOwner(), WEAPON_LASER, WEAPON_GRENADE, 5, false);
            GameWorld()->CreateDamageIndCircle(pTarget->GetRealPos(), true, (float)(rand()%360), 32, 32, 3.0F);
            
            char aBuf[64];
            str_format(aBuf, sizeof(aBuf), "白Tee之星成功为您拦截了一发来自'%s'子弹.(冷却6秒)", Server()->ClientName(pTarget->GetOwner()));
            GameServer()->SendChatTarget(GetOwner(), aBuf);
            
            str_format(aBuf, sizeof(aBuf), "'%s'的白Tee之星拦截了你的子弹！", Server()->ClientName(GetOwner()));
            GameServer()->SendChatTarget(pTarget->GetOwner(), aBuf);
            
            // Set fire delay to 6.
            SetFireDelay(6);
        }
    }
}

void CStar::OnOwnerFire(int WeaponSlot, int WeaponID)
{
    if(WeaponID == WEAPON_ID_HAMMER)
	{
        CCharacter *pChr = GameServer()->GetPlayerChar(GetOwner());
		CCharacter *pTarget = (CCharacter *)GameWorld()->ClosestEntity(GetPos(), 32, CGameWorld::ENTTYPE_CHARACTER, pChr);
        if(pTarget)
			Attack(pTarget);
		else
            //Attack(vec2(pChr->m_LatestInput.m_TargetX, pChr->m_LatestInput.m_TargetY) + GetPos());
			Attack(vec2(pChr->m_LatestInput.m_TargetX, pChr->m_LatestInput.m_TargetY) + pChr->GetPos());
	}
}

void CStar::OnStandSnap()
{
    
}