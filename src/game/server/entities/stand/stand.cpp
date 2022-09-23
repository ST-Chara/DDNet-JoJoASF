#include "stand.h"

IStand::IStand(CGameWorld *pGameWorld, int Type, vec2 Pos, int m_Owner) : CDumbEntity(pGameWorld, Type, Pos)
{

}

void IStand::Tick()
{
    if(m_FireDelay > 0)
        m_FireDelay--;
    OnStandTick();
}