#include "extdll.h"
#include "util.h"
#include "cbase.h"

class CNaziFlag : public CBaseEntity
{
public:
	void Spawn( void );
	void Precache( void );
};

LINK_ENTITY_TO_CLASS( nazi_flag, CNaziFlag )

void CNaziFlag::Precache( void )
{
	PRECACHE_MODEL( "models/gerflag2.mdl" );
}

void CNaziFlag::Spawn( void )
{
	Precache();

        SET_MODEL( ENT( pev ), "models/gerflag2.mdl" );

	pev->solid = SOLID_BBOX;

	UTIL_SetSize( pev, Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
	UTIL_SetOrigin( pev, pev->origin );

	pev->takedamage = DAMAGE_NO;

	DROP_TO_FLOOR( edict() );
}