// Lamp Entity

#include "extdll.h"
#include "util.h"
#include "cbase.h"

class CLamp : public CBaseEntity
{
public:
	void Spawn( void );
	void Precache( void );
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	virtual int ObjectCaps( void ) { return (CBaseEntity::ObjectCaps() | FCAP_ACROSS_TRANSITION ); }

        float TouchGravGun( CBaseEntity *attacker, int stage )
        {
                return TRUE;
        }
};

LINK_ENTITY_TO_CLASS( lamp, CLamp )

void CLamp::Precache( void )
{
	PRECACHE_MODEL( "models/w_lantern.mdl" );
}

void CLamp::Spawn( void )
{
	Precache();

        SET_MODEL( ENT( pev ), "models/w_lantern.mdl" );

	pev->solid = SOLID_BBOX;
	pev->effects = EF_LIGHT;
	pev->takedamage = DAMAGE_NO;
	pev->movetype = MOVETYPE_TOSS;

	UTIL_SetSize( pev, Vector( -16, -16, 0 ), Vector( 16, 16, 16 ) );
	UTIL_SetOrigin( pev, pev->origin );

	DROP_TO_FLOOR( edict() );
}

void CLamp::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if ( !pActivator )
		return; // make sure we have a activator

	if ( pActivator->IsPlayer() )
		return; // activator is not player

	if ( pev->effects != EF_LIGHT )
	{
		pev->effects = EF_LIGHT;
		EMIT_SOUND_DYN( ENT( pev ), CHAN_WEAPON, "items/flashlight1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM );
		return;
	}
	else
	{
		pev->effects = NULL;
		EMIT_SOUND_DYN( ENT( pev ), CHAN_WEAPON, "items/flashlight1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM );
		return;
	}
}
