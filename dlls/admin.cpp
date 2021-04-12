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
                char * time_str = ctime(&mytime);
                time_str[strlen(time_str)-1] = '\0';
		const char *ip = g_engfuncs.pfnInfoKeyValue( g_engfuncs.pfnGetInfoKeyBuffer( pPlayer->edict() ), "ip" );

		if( !ip )
		{
			ip = "UNKNOWN";
		}

                fprintf( fladminlog, "%s %s %s %s %s\n", time_str, LogType, ip, GETPLAYERAUTHID( pPlayer->edict() ), GGM_PlayerName( pPlayer ) ); // Timestamp, LogType, IP Address, XashID, Nickname
                fclose( fladminlog );

		if( admin_kickonfail.value )
		{
			SERVER_COMMAND( UTIL_VarArgs( "kick #%i admin auth failure\n", GETPLAYERUSERID( pPlayer->edict() ) ) );
		}
}

bool Admin_ClientCommand( edict_t *pEntity )
{
        const char *pCmd = CMD_ARGV(0);
        CBasePlayer *pPlayer = (CBasePlayer*)GET_PRIVATE(pEntity);

	if( FStrEq(pCmd, "admin_login") )
	{
	        if( !admin_password.string || pPlayer->m_ggm.IsAdmin )
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
			Admin_LogAttempt( pPlayer, "Became admin:" );
		}
		else
		{
			GGM_ChatPrintf( pPlayer, "^1Login failed^7\n" );
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

		int UserID = atoi( CMD_ARGV( 1 ) );
		const char *Command = (char *)CMD_ARGV( 2 );

		CBasePlayer *pSudoer = GGM_GetPlayerByUID( UserID );

		if( !pSudoer )
		{
			GGM_ChatPrintf( pPlayer, "^1Invalid player!^7\n" );
			return true;
		}

		CLIENT_COMMAND( pSudoer->edict(), "%s\n", Command );
		if( pSudoer->pev->netname )
		{
			GGM_ChatPrintf( pPlayer, "^2Command was sent to^7 %s!^7\n", STRING( pSudoer->pev->netname ) );
		}
		else
		{
			GGM_ChatPrintf( pPlayer, "^1Executor didnt had a netname, but command was sent" );
		}

		return true;
	}
	else if( FStrEq(pCmd, "admin_strip") )
	{
		if( CMD_ARGC() < 1 )
		{
			GGM_ChatPrintf( pPlayer, "^1Usage: admin_strip ^2<1/0>" );
			return true;
		}
		BOOL StripSuit = atoi( CMD_ARGV( 1 ) );

		GGM_ChatPrintf( pPlayer, "^2Your items were removed^7\n" );
		if( StripSuit )
		{
			pPlayer->RemoveAllItems( FALSE );
		}
		else
		{
			pPlayer->RemoveAllItems( TRUE );
		}
		return true;
	}
	else if( FStrEq(pCmd, "admin_notarget") )
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
			pPlayer->pev->takedamage = DAMAGE_AIM;
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
		GGM_ChatPrintf( pPlayer, "^2Successful logout^7\n" );
		Admin_LogAttempt( pPlayer, "Successful logout:" );
		return true;
	}

	return false;
}
#endif
