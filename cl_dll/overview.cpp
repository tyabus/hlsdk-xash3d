//========= Copyright (c) 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "hud.h"
#include "cl_util.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "overview.h"

// these are included for the math functions
#include "com_model.h"
#include "studio_util.h"

#pragma warning(disable: 4244)

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CHudOverview::Init()
{
	gHUD.AddHudElem(this);

	m_iFlags |= HUD_ACTIVE;

	return 1;
}

//-----------------------------------------------------------------------------
// Purpose: Loads new icons
//-----------------------------------------------------------------------------
int CHudOverview::VidInit()
{
	m_hsprPlayer = gEngfuncs.pfnSPR_Load( "sprites/ring.spr" );
	m_hsprViewcone = gEngfuncs.pfnSPR_Load( "sprites/camera.spr" );

	return 1;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : flTime - 
//			intermission - 
//-----------------------------------------------------------------------------
int CHudOverview::Draw( float flTime )
{
	return 1;
}

//-----------------------------------------------------------------------------
// Purpose: called every time a server is connected to
//-----------------------------------------------------------------------------
void CHudOverview::InitHUDData()
{
	//this block would force the spectator view to be on
	//gEngfuncs.Overview_SetDrawOverview( 1 );
	//gEngfuncs.Overview_SetDrawInset( 0 );
}
