#include "extdll.h"
#include "util.h"
#include "cbase.h"

class CHPlayer : public CBaseEntity
{
public:
	void Spawn( void );
	void Precache( void );
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	virtual int ObjectCaps( void ) { return (CBaseEntity::ObjectCaps() | FCAP_CONTINUOUS_USE) & ~FCAP_ACROSS_TRANSITION; }
	float TouchGravGun( CBaseEntity *attacker, int stage )
	{
        	return 1000;
	}
};

LINK_ENTITY_TO_CLASS( h_player, CHPlayer )

void CHPlayer::Precache( void )
{
	PRECACHE_MODEL( "models/h_player.mdl" );
}

void CHPlayer::Spawn( void )
{
	Precache();

        SET_MODEL( ENT( pev ), "models/h_player.mdl" );

	pev->solid = SOLID_BBOX;
	pev->movetype = MOVETYPE_TOSS;

	UTIL_SetSize( pev, Vector( -16, -16, 0), Vector( 16, 16, 36 ) );
	UTIL_SetOrigin( pev, pev->origin );

	pev->takedamage = DAMAGE_NO;

	DROP_TO_FLOOR( edict() );
}

void CHPlayer::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if ( !pActivator )
		return; // make sure we have a activator
	if ( !pActivator->IsPlayer() )
		return; // make sure our activator is a player

	UTIL_ShowMessage( "Dopefish Lives!", pActivator );
}