#include "star.h"

CStar::CStar(CGameWorld *pGameWorld, CCharacter *pOwner) : IStand(pGameWorld, CDumbEntity::TYPE_PICKUP_LASER, vec2(0,0))
{
    m_Pos = pOwner->GetPos();
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
    
}