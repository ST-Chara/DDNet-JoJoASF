#include <game/server/entities/projectile.h>
#include <game/server/entities/intercept-laser.h>
#include <game/server/weapons.h>
#include <game/server/entities/textentity.h>

#include "star.h"
CStar::CStar(CGameWorld *pGameWorld, CCharacter *pOwner) : IStand(pGameWorld, CDumbEntity::TYPE_PICKUP_LASER, vec2(0,0))
{
    m_Pos = pOwner->GetPos();
    m_Owner = pOwner->GetPlayer()->GetCID();
    SetHide(false);
}

void CStar::Attack(CCharacter *pTarget)
{

}

void CStar::Attack(vec2 TargetPos)
{

}

void CStar::OnStandTick()
{
    SmoothMove(vec2(GameServer()->GetPlayerChar(GetOwner())->m_LatestInput.m_TargetX+GameServer()->GetPlayerChar(GetOwner())->GetPos().x, GameServer()->GetPlayerChar(GetOwner())->m_LatestInput.m_TargetY+GameServer()->GetPlayerChar(GetOwner())->GetPos().y), 27.0f, false);
	if(distance(GetPos(), GameServer()->m_apPlayers[GetOwner()]->GetCharacter()->GetPos()) >= 800)
		MoveTo(GameServer()->m_apPlayers[GetOwner()]->GetCharacter()->GetPos());

    HandleInterceptLaser();
}

void CStar::HandleInterceptLaser()
{
    if(GetFireDelay() > 0)
        return;
    for(CProjectile *pTarget = (CProjectile*) GameWorld()->FindFirst(CGameWorld::ENTTYPE_PROJECTILE); pTarget; pTarget = (CProjectile *)pTarget->TypeNext())
    {
        if(pTarget)
        {
            if(distance(pTarget->GetRealPos(), m_Pos) > 600.0f)
                continue;
            
            if(GameServer()->Collision()->IntersectLine(GetPos(), pTarget->GetRealPos(), NULL, NULL))
                continue;
            
            if(pTarget->GetOwner() == GetOwner())
                continue;

            //vec2 Direction = normalize(pTarget->GetRealPos() - GetPos());
            //dbg_msg("TargetPos-1010101000", "X: %f  Y: %f", pTarget->GetRealPos().x, pTarget->GetRealPos().y);
            //dbg_msg("OwnerPos-1010101000", "X: %f  Y: %f", GameServer()->GetPlayerChar(GetOwner())->GetPos().x, GameServer()->GetPlayerChar(GetOwner())->GetPos().y);
            
            new CTextEntity(GameWorld(), GetPos(), CTextEntity::TYPE_LASER, CTextEntity::SIZE_XLARGE, CTextEntity::ALIGN_MIDDLE, "GOT  U!", 2.0f);
            new CInterceptLaser(GameWorld(), GetOwner(), m_Pos, pTarget, pTarget->GetRealPos());
            GameServer()->SendChatTarget(GetOwner(), "白Tee之星已成功为您拦截了一发子弹.");
            SetFireDelay(2);
        }
    }
}