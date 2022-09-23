#include "stand.h"

IStand::IStand(CGameWorld *pGameWorld, int Type, vec2 Pos, int Owner) : CDumbEntity(pGameWorld, Type, Pos)
{
    m_Owner = Owner;
}

void IStand::Tick()
{
    if(m_FireDelay > 0)
        m_FireDelay--;
    OnStandTick();
}