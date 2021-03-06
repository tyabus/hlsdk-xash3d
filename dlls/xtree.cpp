#include "extdll.h"
#include "util.h"
#include "cbase.h"

class CXTreeModel: public CBaseEntity
{
public:
	void Precache();
	void Spawn();
};

LINK_ENTITY_TO_CLASS( ggm_xtreemodel, CXTreeModel );

void CXTreeModel::Precache()
{
	PRECACHE_MODEL( "models/xtree.mdl" );
}

void CXTreeModel::Spawn()
{
	Precache();

	SET_MODEL( ENT( pev ), "models/xtree.mdl" );
	DROP_TO_FLOOR( ENT( pev ) );
}

class CXTreeSpawner: public CBaseEntity
{
public:
	void Precache();
	void Spawn();
	void EXPORT SpawnTrees();
	void EXPORT ChangeColor();
};

LINK_ENTITY_TO_CLASS( ggm_xtree, CXTreeSpawner )

void CXTreeSpawner::Precache()
{
	UTIL_PrecacheOther("ggm_xtreemodel");
}

void CXTreeSpawner::Spawn()
{
	Precache();

	pev->nextthink = gpGlobals->time + 5;
	DROP_TO_FLOOR( ENT( pev ) );
	SetThink( &CXTreeSpawner::SpawnTrees );
}

void CXTreeSpawner::SpawnTrees()
{
	TraceResult tr;
	UTIL_TraceHull( pev->origin + Vector( 0, 0, 1), pev->origin + Vector( RANDOM_FLOAT( -150, 150 ), RANDOM_FLOAT( -150, 150 ), -200 ), ignore_monsters, human_hull, NULL, &tr);

	if( tr.vecEndPos.z < pev->origin.z + 0.1 )
		pev->origin = tr.vecEndPos;

	DROP_TO_FLOOR( ENT( pev ) );

	pev->euser1 = CREATE_NAMED_ENTITY(MAKE_STRING("ggm_xtreemodel"));
	pev->euser1->v.origin = pev->origin;
	pev->euser1->v.effects = 0;
	pev->euser1->v.rendercolor = Vector(32,128,0);
	pev->euser1->v.renderfx = kRenderFxGlowShell;
	DispatchSpawn( pev->euser1 );
	pev->euser2 = CREATE_NAMED_ENTITY(MAKE_STRING("ggm_xtreemodel"));
	pev->euser2->v.origin = pev->origin;
	pev->euser2->v.effects = 0;
	pev->euser2->v.rendercolor = Vector(0,255,0);
	pev->euser2->v.renderamt = 255;
	pev->euser2->v.rendermode  = kRenderTransColor;
	pev->euser2->v.angles.y = 135;
	DispatchSpawn( pev->euser2 );
	pev->euser3 = CREATE_NAMED_ENTITY(MAKE_STRING("ggm_xtreemodel"));
	pev->euser3->v.origin = pev->origin;
	pev->euser3->v.effects = 0;
	pev->euser3->v.rendercolor = Vector(0,255,0);
	pev->euser3->v.renderamt = 255;
	pev->euser3->v.rendermode  = kRenderTransAdd;
	pev->euser3->v.angles.y = 135;
	DispatchSpawn( pev->euser3 );
	SetThink(&CXTreeSpawner::ChangeColor);
	ChangeColor();
}

void CXTreeSpawner::ChangeColor()
{
	pev->nextthink = gpGlobals->time + 5;
	Vector vecSrc = pev->origin;
	static int iColor = 0;
	if( iColor == 3 ) iColor = 0;
	int rgrgiColors[3][3] =
	{
		{ 255, 0, 0 },
		{ 0, 255, 0 },
		{ 0, 0, 255 }
	};

	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, vecSrc );
		WRITE_BYTE( TE_DLIGHT );
		WRITE_COORD( vecSrc.x );	// X
		WRITE_COORD( vecSrc.y );	// Y
		WRITE_COORD( vecSrc.z );	// Z
		WRITE_BYTE( 12 );	// radius * 0.1
		WRITE_BYTE( rgrgiColors[iColor][0] );	// r
		WRITE_BYTE( rgrgiColors[iColor][1] );	// g
		WRITE_BYTE( rgrgiColors[iColor][2] );	// b
		WRITE_BYTE( 150 );	// time * 10
		WRITE_BYTE( 1 );	// decay * 0.1
	MESSAGE_END();
	iColor++;
}
