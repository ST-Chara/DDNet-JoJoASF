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

	m_TestStand[pChr->GetPlayer()->GetCID()] = new CStar(GameWorld(), pChr);
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
	
	for(int i = 0; i < MAX_CLIENTS; i ++)
	{
		if(!GameServer()->m_apPlayers[i])
			continue;

		if(!GameServer()->m_apPlayers[i]->GetCharacter())
			continue;
		
		m_TestStand[i]->SmoothMove(vec2(GameServer()->GetPlayerChar(i)->m_LatestInput.m_TargetX+GameServer()->GetPlayerChar(i)->GetPos().x, GameServer()->GetPlayerChar(i)->m_LatestInput.m_TargetY+GameServer()->GetPlayerChar(i)->GetPos().y), 27.0f, false);
		if(distance(m_TestStand[i]->GetPos(), GameServer()->m_apPlayers[i]->GetCharacter()->GetPos()) >= 800)
			m_TestStand[i]->MoveTo(GameServer()->m_apPlayers[i]->GetCharacter()->GetPos());
	}
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