#ifndef GAME_SERVER_ENTITIES_STAND_H
#define GAME_SERVER_ENTITIES_STAND_H

#include <game/server/entities/dumbentity.h>
#include "../character.h"

enum
{
    EVENT_FOLLOW_MASTER,
    EVENT_AUTO_ATK,
    EVENT_HIDE,
};

class IStand : public CDumbEntity
{
public:
    IStand(CGameWorld *pGameWorld, int Type, vec2 Pos);

    virtual void Attack(CCharacter *pTarget) { };
    virtual void Attack(vec2 Pos) { };

    virtual void Tick() override;
    virtual void OnStandTick() { };

    void SetState(int State) { m_State = State; };
    void SetEvent(int Event) { m_Event = Event; };
    void SetHide(bool Hide) { m_Hide = Hide; SetEvent(EVENT_HIDE); };

    int GetOwner() { return m_Owner; };

private:
    int m_Owner;
    int m_State;
    int m_Event;
    bool m_Hide;
};

#endif