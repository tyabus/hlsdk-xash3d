#include "extdll.h"
#include "util.h"
#include "cbase.h"

class CHPlayer : public CBaseEntity
{
	void Spawn( void );
	void Precache( void );	
};

LINK_ENTITY_TO_CLASS( h_player, CHPlayer )

void CHPlayer::Precache( void )
{
	PRECACHE_MODEL( "models/h_player.mdl" );
}

void CHPlayer::Spawn( void )
{
	Precache();

	pev->solid = SOLID_BBOX;

	SET_MODEL( ENT( pev ), "models/h_player.mdl" );
	UTIL_SetSize( pev, Vector( -16, -16, 0), Vector( 16, 16, 36 ) );
	UTIL_SetOrigin( pev, pev->origin );
	
	pev->takedamage = DAMAGE_NO;

	DROP_TO_FLOOR( edict() );
}