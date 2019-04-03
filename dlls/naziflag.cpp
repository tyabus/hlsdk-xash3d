//=========================================================
// nazi flag prop
//=========================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"

class CNaziFlag : public CBaseEntity
{
public:
	void Spawn( void );
	void Precache( void );
};

LINK_ENTITY_TO_CLASS( flag_nazi, CNaziFlag );

void CNaziFlag::Precache()
{
	PRECACHE_MODEL("models/gerflag2.mdl");
}

void CNaziFlag::Spawn()
{
	Precache();
	
	SET_MODEL( edict(), "models/gerflag2.mdl" );
}