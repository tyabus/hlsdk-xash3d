#ifndef __ANDROID__
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gravgunmod.h"
#include "player.h"
#include <time.h>

cvar_t admin_password = { "admin_password", "", FCVAR_SERVER | FCVAR_UNLOGGED };

void Admin_RegisterCVars( void )
{
	CVAR_REGISTER( &admin_password );
}

void Admin_LogAttempts( CBasePlayer *pPlayer, char *LogType )
{
                FILE *fladminlog = fopen("logattempts.txt", "a");
		time_t mytime = time(NULL);
                char * time_str = ctime(&mytime);
                time_str[strlen(time_str)-1] = '\0';

                fprintf( fladminlog, "%s %s %s %s\n", time_str, LogType, GETPLAYERAUTHID( pPlayer->edict() ), GGM_PlayerName( pPlayer ) ); // Timestamp, LogType, XashID, Nickname
                fclose( fladminlog );
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
			Admin_LogAttempts( pPlayer, "Became admin:" );
		}
		else
		{
			GGM_ChatPrintf( pPlayer, "^1Login failed^7\n" );
			Admin_LogAttempts( pPlayer, "Failure login:" );
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
		GGM_ChatPrintf( pPlayer, "^2Command was sent to client!^7\n");

		return true;
	}
	else if( FStrEq(pCmd, "admin_strip") )
        {
		GGM_ChatPrintf( pPlayer, "^2Your items were removed^7\n" );
		pPlayer->RemoveAllItems( FALSE );
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
		Admin_LogAttempts( pPlayer, "Successful logout:" );
		return true;
	}

	return false;
}
#endif
