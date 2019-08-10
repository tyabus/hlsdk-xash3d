/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "player.h"
#include "items.h"
#include "gamerules.h"

extern int gmsgItemPickup;

class CHealthKitMega : public CItem
{
	void Spawn( void );
	void Precache( void );
	BOOL MyTouch( CBasePlayer *pPlayer );
	void Think( void );
};

LINK_ENTITY_TO_CLASS( item_healthkitmega, CHealthKitMega )

void CHealthKitMega::Spawn( void )
{
	Precache();
	pev->renderfx = kRenderFxGlowShell;
	pev->rendermode = kRenderTransColor;
	SET_MODEL( ENT( pev ), "models/w_medkit.mdl" );

	CItem::Spawn();

	SetThink( &CHealthKitMega::Think );
	pev->nextthink = gpGlobals->time + 0.20;
}

void CHealthKitMega::Precache( void )
{
	PRECACHE_MODEL( "models/w_medkit.mdl" );
	PRECACHE_SOUND( "items/smallmedkit1.wav" );
}

void CHealthKitMega::Think( void )
{
	pev->rendercolor.x = random();
	pev->rendercolor.y = random();
	pev->rendercolor.z = random();
	pev->nextthink = gpGlobals->time + 0.20;
}

BOOL CHealthKitMega::MyTouch( CBasePlayer *pPlayer )
{
	if( pPlayer->pev->deadflag != DEAD_NO )
	{
		return FALSE;
	}

	if( pPlayer->pev->health >= 200 )
	{
		return FALSE;
	}

	if( pPlayer->pev->health += 100 )
	{
		pPlayer->pev->armorvalue = 100;

		if( pPlayer->pev->waterlevel != 0 )
			pPlayer->pev->air_finished = gpGlobals->time + 15;

		MESSAGE_BEGIN( MSG_ONE, gmsgItemPickup, NULL, pPlayer->pev );
			WRITE_STRING( STRING( pev->classname ) );
		MESSAGE_END();

		EMIT_SOUND_DYN( ENT( pPlayer->pev ), CHAN_ITEM, "items/smallmedkit1.wav", 1, ATTN_NORM, 0, 75 );

		SetThink( NULL );
		pev->nextthink = gpGlobals->time;
		UTIL_Remove( this );

		return TRUE;
	}
	return FALSE;
}
