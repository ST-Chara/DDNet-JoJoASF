#ifndef GAME_SERVER_ENTITIES_STAR_H
#define GAME_SERVER_ENTITIES_STAR_H

#include "stand.h"

enum
{
    STATE_THE_STAR,
    STATE_THE_WORLD_2,
    STATE_THE_WORLD_4,
    STATE_THE_WORLD_9,
};

// Star Plateenum
class CStar : public IStand
{
public:
    CStar(CGameWorld *pGameWorld, CCharacter *pOwner);

};

#endif