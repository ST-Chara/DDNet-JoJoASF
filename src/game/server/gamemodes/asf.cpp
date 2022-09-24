/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "asf.h"

#include <game/server/entities/textentity.h>
#include <game/server/entities/character.h>
#include <game/server/weapons.h>

#include <game/server/entities/stand/star.h>
CGameControllerASF::CGameControllerASF() :
	IGameController()
{
	m_pGameType = "JOJO";
	m_GameFlags = IGF_SUDDENDEATH;
}

void CGameControllerASF::OnCharacterSpawn(CCharacter *pChr)
{
	pChr->IncreaseHealth(10);

	pChr->GiveWeapon(WEAPON_GUN, WEAPON_ID_PISTOL, 10);
	pChr->GiveWeapon(WEAPON_HAMMER, WEAPON_ID_HAMMER, -1);

	m_Stand[pChr->GetPlayer()->GetCID()] = new CStar(GameWorld(), pChr);
}

void CGameControllerASF::OnInit()
{
	/*for(int i = 1; i < MAX_CLIENTS; i++)
	{
		GameServer()->m_apPlayers[i] = new(i) CPlayer(GameServer(), i, false);
		Server()->SetClientName(i, "Hello");
		Server()->SetClientClan(i, "Tests");
		str_copy(GameServer()->m_apPlayers[i]->m_TeeInfos.m_SkinName, "blacktee", sizeof(GameServer()->m_apPlayers[i]->m_TeeInfos.m_SkinName));
	}*/
}

void CGameControllerASF::OnPlayerJoin(CPlayer *pPlayer)
{
	char aBuf[64];
	str_format(aBuf, sizeof(aBuf), "欢迎来到【男子汉的世界】, %s!!", Server()->ClientName(pPlayer->GetCID()));
	SendChatTarget(pPlayer->GetCID(), aBuf);
}

void CGameControllerASF::OnPreTick()
{
	if(!IsGameRunning())
		return;
	
	/*for(int i = 0; i < MAX_CLIENTS; i ++)
	{
		if(!GameServer()->m_apPlayers[i])
			continue;

		if(!GameServer()->m_apPlayers[i]->GetCharacter())
			continue;
		
	}*/
}

void CGameControllerASF::OnPlayerLeave(CPlayer *pPlayer)
{
	if(pPlayer->GetCharacter())
	{
		GameWorld()->CreateExplosionParticle(pPlayer->GetCharacter()->GetPos());
		GameWorld()->CreateDamageIndCircle(pPlayer->GetCharacter()->GetPos(), true, (float)(rand()%360), 32, 32, 3.0F);
	}
}

void CGameControllerASF::OnCharacterSwitchWeapon(CCharacter *pChr, int Wheel)
{
	//new CTextEntity(GameWorld(), pChr->GetPos(), CTextEntity::TYPE_SHOTGUN, CTextEntity::SIZE_LARGE, CTextEntity::ALIGN_MIDDLE, (char *)"BBB :::");

}

int CGameControllerASF::OnCharacterDeath(CCharacter *pVictim, CPlayer *pKiller, int Weapon)
{
	if(m_Stand[pVictim->GetPlayer()->GetCID()])
		m_Stand[pVictim->GetPlayer()->GetCID()]->Destroy();
}

void CGameControllerASF::OnCharacterFire(class CCharacter *pChr, int WeaponSlot, int WeaponID)
{
	if(m_Stand[pChr->GetPlayer()->GetCID()])
	{
		IStand *m_pStand = m_Stand[pChr->GetPlayer()->GetCID()];
		m_pStand->OnOwnerFire(WeaponSlot, WeaponID);
	}
}

void CGameControllerASF::OnGameStart(bool IsRound)
{
	for(int i = 0; i < MAX_CLIENTS; i ++)
	{
		if(m_Stand[i])
			m_Stand[i]->Destroy();
	}
}