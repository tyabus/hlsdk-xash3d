//++ BulliT

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "game.h"
#include "player.h"

#include "agtimer.h"
#include "agglobal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AgTimer::AgTimer()
{
	m_fNextTimerUpdate = gpGlobals->time + 3.0; // Dont start timer directly.
	m_fLastTimeCheck = gpGlobals->time;
	m_fEffectiveTime = 0.0;
	m_pmp_timelimit = CVAR_GET_POINTER("mp_timelimit");
}

AgTimer::~AgTimer()
{

}

void AgTimer::Think()
{
	// Calculate effective time
	m_fEffectiveTime += gpGlobals->time - m_fLastTimeCheck;

	m_fLastTimeCheck = gpGlobals->time;

	if( m_fNextTimerUpdate <= m_fEffectiveTime )
	{
		// Sanity time check. Some dudes tends to put timelimit weird.
		if( timelimit.value > 2880 )
        		CVAR_SET_FLOAT( "mp_timelimit", 2880 ); //Max two days.

		// Write the time. (negative turns off timer on client)
		long lTime = (m_pmp_timelimit->value * 60) - m_fEffectiveTime;

		if( lTime > 0 )
		{
			char szTime[128];
			if( 86400 < lTime )
			{
				//More than one day. Print days, hour, minutes and seconds
				ldiv_t d1 = ldiv( lTime, 86400 );
				ldiv_t d2 = ldiv( d1.rem, 3600 );
				ldiv_t d3 = ldiv( d2.rem, 60 );
				sprintf( szTime, "%ldd %ldh %02ldm %02lds\n", d1.quot, d2.quot, d3.quot, d3.rem );
			}
			else if( 3600 < lTime )
			{
				// More than one hour. Print hour, minutes and seconds
				ldiv_t d1 = ldiv( lTime, 3600 );
				ldiv_t d2 = ldiv( d1.rem, 60 );
				sprintf( szTime, "%ldh %02ldm %02lds\n", d1.quot, d2.quot, d2.rem );
			}
			else if( 60 < lTime )
			{
				// More than one minute. Print minutes and seconds.
				ldiv_t d = ldiv( lTime, 60 );
				sprintf( szTime, "%ld:%02ld\n", d.quot, d.rem );
			}
			else
			{
				// Less than a minute left. Print seconds.
				sprintf( szTime,"%ld\n", lTime );
			}

			AgSay( NULL, szTime, NULL, 60, 0.5, 0.01, 1 );
				m_fNextTimerUpdate += 1;
		}
	}
}

//-- Martin Webrant
