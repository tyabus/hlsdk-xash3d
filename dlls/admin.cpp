#ifndef __ANDROID__
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gravgunmod.h"
#include "player.h"
#include <time.h>

cvar_t admin_password = { "admin_password", "", FCVAR_SERVER | FCVAR_UNLOGGED };
cvar_t admin_kickonfail = { "admin_kickonfail", "0", FCVAR_SERVER };

void Admin_RegisterCVars( void )
{
	CVAR_REGISTER( &admin_kickonfail );
	CVAR_REGISTER( &admin_password );
}

void Admin_LogAttempt( CBasePlayer *pPlayer, char *LogType )
{
		FILE *fladminlog = fopen("adminattempts.log", "a");
		time_t mytime = time(NULL);
		char *time_str = ctime(&mytime);
		time_str[strlen(time_str)-1] = '\0';
		const char *ip = g_engfuncs.pfnInfoKeyValue( g_engfuncs.pfnGetInfoKeyBuffer( pPlayer->edict() ), "ip" );

		if( !ip || !ip[0] )
			ip = "UNKNOWN";

		fprintf( fladminlog, "%s %s %s %s %s\n", time_str, LogType, ip, GETPLAYERAUTHID( pPlayer->edict() ), GGM_PlayerName( pPlayer ) ); // Timestamp, LogType, IP Address, XashID, Nickname
		fclose( fladminlog );

		if( admin_kickonfail.value )
		{
			SERVER_COMMAND( UTIL_VarArgs( "kick #%i authorization failure\n", GETPLAYERUSERID( pPlayer->edict() ) ) );
		}
}

bool Admin_ClientCommand( edict_t *pEntity )
{
        const char *pCmd = CMD_ARGV(0);
        CBasePlayer *pPlayer = (CBasePlayer*)GET_PRIVATE(pEntity);

	if( FStrEq(pCmd, "admin_login") )
	{
		if( !admin_password.string[0] || pPlayer->m_ggm.IsAdmin )
		{
			return false;
		}

		if( CMD_ARGC() != 2 )
		{
			GGM_ChatPrintf( pPlayer, "^1Usage: admin_login ^2<password>^7\n" );
			return true;
		}

		const char *passwordargv = CMD_ARGV( 1 );

		if( !strcmp( passwordargv, admin_password.string ) )
		{
			pPlayer->m_ggm.IsAdmin = true;
			GGM_ChatPrintf( pPlayer, "^2Login successful^7\n" );
			ALERT( at_console, "Player %s: Became admin\n", GGM_PlayerName( pPlayer ) );
			Admin_LogAttempt( pPlayer, "Became admin:" );
		}
		else
		{
			GGM_ChatPrintf( pPlayer, "^1Login failed^7\n" );
			ALERT( at_console, "Player %s: Login failure\n", GGM_PlayerName( pPlayer ) );
			Admin_LogAttempt( pPlayer, "Failure login:" );
		}
		return true;
	}
	if( !pPlayer->m_ggm.IsAdmin )
		return true;

	if( FStrEq(pCmd, "admin_sudo") )
	{
		if( CMD_ARGC() != 3 )
		{
			GGM_ChatPrintf( pPlayer, "^1Usage: admin_sudo ^2<UserID> <Command>^7\n" );
			return true;
		}

		short int UserID = atoi( CMD_ARGV( 1 ) );
		const char *Command = (char *)CMD_ARGV( 2 );

		CBasePlayer *pSudoer = GGM_GetPlayerByUID( UserID );

		if( !pSudoer )
		{
			GGM_ChatPrintf( pPlayer, "^1Invalid player!^7\n" );
			return true;
		}

		CLIENT_COMMAND( pSudoer->edict(), "%s\n", Command );
		GGM_ChatPrintf( pPlayer, "^2Command was sent to %s^7\n", STRING( pSudoer->pev->netname ) );
		return true;
	}
	else if( FStrEq(pCmd, "admin_strip") )
	{
		if( CMD_ARGC() > 3 )
		{
			GGM_ChatPrintf( pPlayer, "^1Usage: admin_strip ^2<1/0> <UserID>^7\n" );
			return true;
		}

		BOOL StripSuit = atoi( CMD_ARGV( 1 ) );
		short int UserID = atoi( CMD_ARGV( 2 ) );
		CBasePlayer *pStripper = NULL;

		if( CMD_ARGV( 2 ) != NULL )
			pStripper = GGM_GetPlayerByUID( UserID );
		else
			pStripper = pPlayer;

		if( !pStripper && CMD_ARGV( 2 ) != NULL )
		{
			GGM_ChatPrintf( pPlayer, "^1Invalid player!^7\n" );
			return true;
		}

		if( pStripper == pPlayer )
			GGM_ChatPrintf( pPlayer, "^2Removed all your items^7\n" );
		else
			GGM_ChatPrintf( pPlayer, "^2Removed all %s items^7\n", STRING( pStripper->pev->netname ) );

		if( StripSuit )
		{
			pStripper->RemoveAllItems( TRUE );
		}
		else
		{
			pStripper->RemoveAllItems( FALSE );
		}

		return true;
	}
	else if( FStrEq(pCmd, "admin_notarget") || FStrEq(pCmd, "admin_notar") )
	{
		if( !FBitSet( pPlayer->pev->flags, FL_NOTARGET ) )
		{
			pPlayer->pev->flags |= FL_NOTARGET;
			GGM_ChatPrintf( pPlayer, "^2Admin notarget ON^7\n" );
		}
		else
		{
			pPlayer->pev->flags &= ~FL_NOTARGET;
			GGM_ChatPrintf( pPlayer, "^2Admin notarget OFF^7\n" );
		}
		return true;
	}
	else if( FStrEq(pCmd, "admin_god") || FStrEq(pCmd, "admin_godmode") )
	{
		if( !FBitSet( pPlayer->pev->flags, FL_GODMODE ) )
		{
			pPlayer->pev->flags |= FL_GODMODE;
			GGM_ChatPrintf( pPlayer, "^2Admin godmode ON^7\n" );
		}
		else
		{
			pPlayer->pev->flags &= ~FL_GODMODE;
			GGM_ChatPrintf( pPlayer, "^2Admin godmode OFF^7\n" );
		}
		return true;
	}
	else if( FStrEq(pCmd, "admin_invis") || FStrEq(pCmd, "admin_invisibility") )
	{
		if( pPlayer->pev->solid != SOLID_NOT )
		{
			pPlayer->pev->movetype = MOVETYPE_NOCLIP;
			pPlayer->pev->solid = SOLID_NOT;
			pPlayer->pev->takedamage = DAMAGE_NO;
			pPlayer->pev->effects |= EF_NODRAW;
			pPlayer->pev->flags |= FL_NOTARGET;
			pPlayer->pev->flags |= FL_GODMODE;
			pPlayer->m_fNoPlayerSound = TRUE;
			GGM_ChatPrintf( pPlayer, "^2Admin invisibility ON^7\n" );
		}
		else
		{
			pPlayer->pev->takedamage = DAMAGE_YES;
			pPlayer->pev->movetype = MOVETYPE_WALK;
			pPlayer->pev->flags &= ~FL_NOTARGET;
			pPlayer->pev->flags &= ~FL_GODMODE;
			pPlayer->pev->effects &= ~EF_NODRAW;
			pPlayer->pev->solid = SOLID_SLIDEBOX;
			pPlayer->m_fNoPlayerSound = FALSE;
			GGM_ChatPrintf( pPlayer, "^2Admin invisibility OFF^7\n" );
		}
		return true;
	}
	else if( FStrEq(pCmd, "admin_noclip") )
	{
		if( pPlayer->pev->movetype != MOVETYPE_NOCLIP )
		{
			pPlayer->pev->movetype = MOVETYPE_NOCLIP;
			GGM_ChatPrintf( pPlayer, "^2Admin noclip ON^7\n" );
		}
		else
		{
			pPlayer->pev->movetype = MOVETYPE_WALK;
			GGM_ChatPrintf( pPlayer, "^2Admin noclip OFF^7\n" );
		}
		return true;
	}
	else if( FStrEq(pCmd, "admin_logout" ) )
	{
		pPlayer->m_ggm.IsAdmin = false;
		GGM_ChatPrintf( pPlayer, "^2Successfully logged out^7\n" );
		ALERT( at_console, "Admin %s: Logging out\n", GGM_PlayerName( pPlayer ) );
		Admin_LogAttempt( pPlayer, "Successful logout:" );
		return true;
	}
	else if( FStrEq(pCmd, "admin_say" ) )
	{
		if( CMD_ARGC() != 2 )
		{
			GGM_ChatPrintf( pPlayer, "^1Usage: admin_say ^2<string>^7\n" );
			return true;
		}

		const char *message = (char *)CMD_ARGV( 1 );
		UTIL_ClientPrintAll( HUD_PRINTTALK, message );
		return true;
	}


	return false;
}
#endif
