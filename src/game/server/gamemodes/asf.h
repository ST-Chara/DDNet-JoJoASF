/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_ASF_H
#define GAME_SERVER_GAMEMODES_ASF_H
#include <game/server/gamecontroller.h>
#include <game/server/entities/dumbentity.h>
#include <game/server/entities/stand/stand.h>

class CGameControllerASF : public IGameController
{
public:
	CGameControllerASF();

	// event
	virtual void OnCharacterSpawn(class CCharacter *pChr) override;
    virtual int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon) override;
    virtual void OnCharacterSwitchWeapon(class CCharacter *pChr, int Wheel) override;
    virtual void OnCharacterFire(class CCharacter *pChr, int WeaponSlot, int WeaponID);

    virtual void OnPlayerJoin(class CPlayer *pPlayer) override;
	virtual void OnPlayerLeave(class CPlayer *pPlayer) override;

    virtual void OnInit() override;
    virtual void OnPreTick() override;
    virtual void OnGameStart(bool IsRound) override;
    
    virtual void DoWincheckRound() override;

    IStand *GetStand(int Owner);

private:
    IStand *m_Stand[MAX_CLIENTS];
};

#endif // GAME_SERVER_GAMEMODES_ASF_H
