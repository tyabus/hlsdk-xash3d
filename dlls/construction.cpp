/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// human construction workerer
//=========================================================

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"talkmonster.h"
#include	"schedule.h"
#include	"defaultai.h"
#include	"scripted.h"
#include	"animation.h"
#include	"soundent.h"
#include	"game.h"

enum
{
	SCHED_HIDE = LAST_TALKMONSTER_SCHEDULE + 1,
	SCHED_PANIC,
	SCHED_STARTLE,
	SCHED_TARGET_CHASE_SCARED,
};

enum
{
	TASK_RUN_PATH_SCARED,
	TASK_MOVE_TO_TARGET_RANGE_SCARED
};

//=======================================================
// Construction
//=======================================================
class CConstruction : public CTalkMonster
{
public:
	void Spawn( void );
	void Precache( void );

	void SetYawSpeed( void );
	int Classify( void );
	void HandleAnimEvent( MonsterEvent_t *pEvent );
	void RunTask( Task_t *pTask );
	void StartTask( Task_t *pTask );
	int ObjectCaps( void ) { return CTalkMonster::ObjectCaps() | FCAP_IMPULSE_USE; }
	int TakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType );
	virtual int FriendNumber( int arrayNumber );
	void SetActivity( Activity newActivity );
	Activity GetStoppedActivity( void );
	int ISoundMask( void );

	float CoverRadius( void ) { return 1200; }
	
	// Override these to set behavior
	Schedule_t *GetScheduleOfType( int Type );
	Schedule_t *GetSchedule( void );
	MONSTERSTATE GetIdealState( void );

	void DeathSound( void );
	void PainSound( void );

	void TalkInit( void );

	void Killed( entvars_t *pevAttacker, int iGib );

	virtual int Save( CSave &save );
	virtual int Restore( CRestore &restore );
	static TYPEDESCRIPTION m_SaveData[];

	CUSTOM_SCHEDULES

private:	
	float m_painTime;
};

LINK_ENTITY_TO_CLASS( monster_construction, CConstruction )

TYPEDESCRIPTION	CConstruction::m_SaveData[] =
{
	DEFINE_FIELD( CConstruction, m_painTime, FIELD_TIME ),
};

IMPLEMENT_SAVERESTORE( CConstruction, CTalkMonster )

//=========================================================
// AI Schedules Specific to this monster
//=========================================================
Task_t	cFollow[] =
{
	{ TASK_SET_FAIL_SCHEDULE, (float)SCHED_CANT_FOLLOW },	// If you fail, bail out of follow
	{ TASK_MOVE_TO_TARGET_RANGE, (float)128 },	// Move within 128 of target ent (client)
	//{ TASK_SET_SCHEDULE, (float)SCHED_TARGET_FACE },
};

Schedule_t clFollow[] =
{
	{
		cFollow,
		ARRAYSIZE( cFollow ),
		bits_COND_NEW_ENEMY |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |
		bits_COND_HEAR_SOUND,
		bits_SOUND_COMBAT |
		bits_SOUND_DANGER,
		"Follow"
	},
};

Task_t cFollowScared[] =
{
	{ TASK_SET_FAIL_SCHEDULE, (float)SCHED_TARGET_CHASE },// If you fail, follow normally
	{ TASK_MOVE_TO_TARGET_RANGE_SCARED, (float)128 },	// Move within 128 of target ent (client)
	//{ TASK_SET_SCHEDULE, (float)SCHED_TARGET_FACE_SCARED },
};

Schedule_t clFollowScared[] =
{
	{
		cFollowScared,
		ARRAYSIZE( cFollowScared ),
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE,
		bits_SOUND_DANGER,
		"FollowScared"
	},
};


Task_t clStopFollowing[] =
{
	{ TASK_CANT_FOLLOW, (float)0 },
};

Schedule_t slStopFollowing1[] =
{
	{
		clStopFollowing,
		ARRAYSIZE( clStopFollowing ),
		0,
		0,
		"StopFollowing"
	},
};

Task_t clFaceTarget[] =
{
	{ TASK_STOP_MOVING, (float)0 },
	{ TASK_FACE_TARGET, (float)0 },
	{ TASK_SET_ACTIVITY, (float)ACT_IDLE },
	{ TASK_SET_SCHEDULE, (float)SCHED_TARGET_CHASE },
};

Schedule_t slFaceTarget1[] =
{
	{
		clFaceTarget,
		ARRAYSIZE( clFaceTarget ),
		bits_COND_CLIENT_PUSH |
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND,
		bits_SOUND_COMBAT |
		bits_SOUND_DANGER,
		"FaceTarget"
	},
};

Task_t tlSciPanic1[] =
{
	{ TASK_STOP_MOVING, (float)0 },
	{ TASK_FACE_ENEMY, (float)0 },
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY, (float)ACT_EXCITED },	// This is really fear-stricken excitement
	{ TASK_SET_ACTIVITY, (float)ACT_IDLE },
};

Schedule_t slSciPanic1[] =
{
	{
		tlSciPanic1,
		ARRAYSIZE( tlSciPanic1 ),
		0,
		0,
		"SciPanic"
	},
};

Task_t tlIdleSciStand11[] =
{
	{ TASK_STOP_MOVING, 0 },
	{ TASK_SET_ACTIVITY, (float)ACT_IDLE },
	{ TASK_WAIT, (float)2 }, // repick IDLESTAND every two seconds.
	{ TASK_TLK_HEADRESET, (float)0 }, // reset head position
};

Schedule_t slIdleSciStand1[] =
{
	{
		tlIdleSciStand11,
		ARRAYSIZE( tlIdleSciStand11 ),
		bits_COND_NEW_ENEMY |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |
		bits_COND_HEAR_SOUND |
		bits_COND_SMELL |
		bits_COND_CLIENT_PUSH |
		bits_COND_PROVOKED,
		bits_SOUND_COMBAT |// sound flags
		//bits_SOUND_PLAYER |
		//bits_SOUND_WORLD |
		bits_SOUND_DANGER |
		bits_SOUND_MEAT |// scents
		bits_SOUND_CARCASS |
		bits_SOUND_GARBAGE,
		"IdleSciStand"

	},
};

Task_t tlScientistCover1[] =
{
	{ TASK_SET_FAIL_SCHEDULE, (float)SCHED_PANIC },		// If you fail, just panic!
	{ TASK_STOP_MOVING, (float)0 },
	{ TASK_FIND_COVER_FROM_ENEMY, (float)0 },
	{ TASK_RUN_PATH_SCARED, (float)0 },
	{ TASK_TURN_LEFT, (float)179 },
	{ TASK_SET_SCHEDULE, (float)SCHED_HIDE },
};

Schedule_t slScientistCover1[] =
{
	{
		tlScientistCover1,
		ARRAYSIZE( tlScientistCover1 ),
		bits_COND_NEW_ENEMY,
		0,
		"ScientistCover"
	},
};

Task_t tlScientistHide1[] =
{
	{ TASK_SET_FAIL_SCHEDULE, (float)SCHED_PANIC },		// If you fail, just panic!
	{ TASK_STOP_MOVING, (float)0 },
	{ TASK_PLAY_SEQUENCE, (float)ACT_CROUCH },
	{ TASK_SET_ACTIVITY, (float)ACT_CROUCHIDLE },	// FIXME: This looks lame
	{ TASK_WAIT_RANDOM, (float)10.0 },
};

Schedule_t slScientistHide1[] =
{
	{
		tlScientistHide1,
		ARRAYSIZE( tlScientistHide1 ),
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND |
		bits_COND_SEE_ENEMY |
		bits_COND_SEE_HATE |
		bits_COND_SEE_FEAR |
		bits_COND_SEE_DISLIKE,
		bits_SOUND_DANGER,
		"ScientistHide"
	},
};

Task_t tlScientistStartle1[] =
{
	{ TASK_SET_FAIL_SCHEDULE, (float)SCHED_PANIC },		// If you fail, just panic!
	{ TASK_STOP_MOVING, (float)0 },
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY, (float)ACT_CROUCH },
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY, (float)ACT_CROUCHIDLE },
	{ TASK_WAIT_RANDOM, (float)1.0 },
};

Schedule_t slScientistStartle1[] =
{
	{
		tlScientistStartle1,
		ARRAYSIZE( tlScientistStartle1 ),
		bits_COND_NEW_ENEMY |
		bits_COND_SEE_ENEMY |
		bits_COND_SEE_HATE |
		bits_COND_SEE_FEAR |
		bits_COND_SEE_DISLIKE,
		0,
		"ScientistStartle"
	},
};

DEFINE_CUSTOM_SCHEDULES( CConstruction )
{
	clFollow,
	slFaceTarget1,
	slIdleSciStand1,
	slScientistCover1,
	slScientistHide1,
	slScientistStartle1,
	slStopFollowing1,
	slSciPanic1,
	clFollowScared,
};

IMPLEMENT_CUSTOM_SCHEDULES( CConstruction, CTalkMonster )

Activity CConstruction::GetStoppedActivity( void )
{ 
	if( m_hEnemy != 0 ) 
		return ACT_EXCITED;
	return CTalkMonster::GetStoppedActivity();
}

void CConstruction::StartTask( Task_t *pTask )
{
	switch( pTask->iTask )
	{
	case TASK_RUN_PATH_SCARED:
		m_movementActivity = ACT_RUN_SCARED;
		break;
	case TASK_MOVE_TO_TARGET_RANGE_SCARED:
		{
			if( ( m_hTargetEnt->pev->origin - pev->origin).Length() < 1 )
				TaskComplete();
			else
			{
				m_vecMoveGoal = m_hTargetEnt->pev->origin;
				if( !MoveToTarget( ACT_WALK_SCARED, 0.5 ) )
					TaskFail();
			}
		}
		break;
	default:
		CTalkMonster::StartTask( pTask );
		break;
	}
}

void CConstruction::RunTask( Task_t *pTask )
{
	switch( pTask->iTask )
	{
	case TASK_RUN_PATH_SCARED:
		if( MovementIsComplete() )
			TaskComplete();
		break;
	case TASK_MOVE_TO_TARGET_RANGE_SCARED:
		{
			if( m_hEnemy == 0 )
			{
				TaskFail();
			}
			else
			{
				float distance;

				distance = ( m_vecMoveGoal - pev->origin ).Length2D();
				// Re-evaluate when you think your finished, or the target has moved too far
				if( ( distance < pTask->flData ) || ( m_vecMoveGoal - m_hTargetEnt->pev->origin ).Length() > pTask->flData * 0.5 )
				{
					m_vecMoveGoal = m_hTargetEnt->pev->origin;
					distance = ( m_vecMoveGoal - pev->origin ).Length2D();
					FRefreshRoute();
				}

				// Set the appropriate activity based on an overlapping range
				// overlap the range to prevent oscillation
				if( distance < pTask->flData )
				{
					TaskComplete();
					RouteClear();		// Stop moving
				}
				else if( distance < 190 && m_movementActivity != ACT_WALK_SCARED )
					m_movementActivity = ACT_WALK_SCARED;
				else if( distance >= 270 && m_movementActivity != ACT_RUN_SCARED )
					m_movementActivity = ACT_RUN_SCARED;
			}
		}
		break;
	default:
		CTalkMonster::RunTask( pTask );
		break;
	}
}

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int CConstruction::Classify( void )
{
	return CLASS_HUMAN_PASSIVE;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CConstruction::SetYawSpeed( void )
{
	int ys;

	ys = 90;

	switch( m_Activity )
	{
	case ACT_IDLE:
		ys = 120;
		break;
	case ACT_WALK:
		ys = 180;
		break;
	case ACT_RUN:
		ys = 150;
		break;
	case ACT_TURN_LEFT:
	case ACT_TURN_RIGHT:
		ys = 120;
		break;
	default:
		break;
	}

	pev->yaw_speed = ys;
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CConstruction::HandleAnimEvent( MonsterEvent_t *pEvent )
{
	switch( pEvent->event )
	{		
	default:
		CTalkMonster::HandleAnimEvent( pEvent );
	}
}

//=========================================================
// Spawn
//=========================================================
void CConstruction::Spawn( void )
{
	Precache();

	SET_MODEL( ENT( pev ), "models/construction.mdl" );
	UTIL_SetSize( pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX );

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = BLOOD_COLOR_RED;
	pev->health = 100;
	pev->view_ofs = Vector( 0, 0, 50 );// position of the eyes relative to monster's origin.
	m_flFieldOfView = VIEW_FIELD_WIDE; // NOTE: we need a wide field of view so scientists will notice player and say hello
	m_MonsterState = MONSTERSTATE_NONE;

	//m_flDistTooFar = 256.0;

	m_afCapability = bits_CAP_HEAR | bits_CAP_TURN_HEAD | bits_CAP_OPEN_DOORS | bits_CAP_AUTO_DOORS | bits_CAP_USE;

	MonsterInit();
	SetUse( &CTalkMonster::FollowerUse );
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CConstruction::Precache( void )
{
	PRECACHE_MODEL( "models/construction.mdl" );

	PRECACHE_SOUND( "barney/ba_pain1.wav" );
	PRECACHE_SOUND( "barney/ba_pain2.wav" );
	PRECACHE_SOUND( "barney/ba_pain3.wav" );

	// every new scientist must call this, otherwise
	// when a level is loaded, nobody will talk (time is reset to 0)
	TalkInit();

	CTalkMonster::Precache();
}	

// Init talk data
void CConstruction::TalkInit()
{
	CTalkMonster::TalkInit();

	// scientists speach group names (group names are in sentences.txt)

	m_szGrp[TLK_ANSWER] = "SC_ANSWER";
	m_szGrp[TLK_QUESTION] = "SC_QUESTION";
	m_szGrp[TLK_IDLE] = "SC_IDLE";
	m_szGrp[TLK_STARE] = "SC_STARE";
	m_szGrp[TLK_USE] = "SC_OK";
	m_szGrp[TLK_UNUSE] = "SC_WAIT";
	m_szGrp[TLK_STOP] = "SC_STOP";
	m_szGrp[TLK_NOSHOOT] = "SC_SCARED";
	m_szGrp[TLK_HELLO] = "SC_HELLO";

	m_szGrp[TLK_PLHURT1] = "!SC_CUREA";
	m_szGrp[TLK_PLHURT2] = "!SC_CUREB"; 
	m_szGrp[TLK_PLHURT3] = "!SC_CUREC";

	m_szGrp[TLK_PHELLO] = "SC_PHELLO";
	m_szGrp[TLK_PIDLE] = "SC_PIDLE";
	m_szGrp[TLK_PQUESTION] = "SC_PQUEST";
	m_szGrp[TLK_SMELL] = "SC_SMELL";

	m_szGrp[TLK_WOUND] = "SC_WOUND";
	m_szGrp[TLK_MORTAL] = "SC_MORTAL";

	m_voicePitch = 100;

}

int CConstruction::TakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType )
{
	if( pevInflictor && pevInflictor->flags & FL_CLIENT )
	{
		if( !mp_coop_noangry.value )
			Remember( bits_MEMORY_PROVOKED );

		StopFollowing( TRUE );
	}

	// make sure friends talk about it if player hurts scientist...
	return CTalkMonster::TakeDamage( pevInflictor, pevAttacker, flDamage, bitsDamageType );
}

//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. In the base class implementation,
// monsters care about all sounds, but no scents.
//=========================================================
int CConstruction::ISoundMask( void )
{
	return bits_SOUND_WORLD |
			bits_SOUND_COMBAT |
			bits_SOUND_DANGER |
			bits_SOUND_PLAYER;
}

//=========================================================
// PainSound
//=========================================================
void CConstruction::PainSound( void )
{
	if( gpGlobals->time < m_painTime )
		return;

	m_painTime = gpGlobals->time + RANDOM_FLOAT( 0.5, 0.75 );

	switch( RANDOM_LONG( 0, 2 ) )
	{
	case 0:
		EMIT_SOUND_DYN( ENT( pev ), CHAN_VOICE, "barney/ba_pain1.wav", 1, ATTN_NORM, 0, GetVoicePitch() );
		break;
	case 1:
		EMIT_SOUND_DYN( ENT( pev ), CHAN_VOICE, "barney/ba_pain2.wav", 1, ATTN_NORM, 0, GetVoicePitch() );
		break;
	case 2:
		EMIT_SOUND_DYN( ENT( pev ), CHAN_VOICE, "barney/ba_pain3.wav", 1, ATTN_NORM, 0, GetVoicePitch() );
		break;
	}
}

//=========================================================
// DeathSound 
//=========================================================
void CConstruction::DeathSound( void )
{
	PainSound();
}

void CConstruction::Killed( entvars_t *pevAttacker, int iGib )
{
	SetUse( NULL );	
	CTalkMonster::Killed( pevAttacker, iGib );
}

void CConstruction::SetActivity( Activity newActivity )
{
	int iSequence;

	iSequence = LookupActivity( newActivity );

	// Set to the desired anim, or default anim if the desired is not present
	if( iSequence == ACTIVITY_NOT_AVAILABLE )
		newActivity = ACT_IDLE;
	CTalkMonster::SetActivity( newActivity );
}

Schedule_t *CConstruction::GetScheduleOfType( int Type )
{
	Schedule_t *psched;

	switch( Type )
	{
	// Hook these to make a looping schedule
	case SCHED_TARGET_FACE:
		// call base class default so that scientist will talk
		// when 'used'
		psched = CTalkMonster::GetScheduleOfType( Type );

		if( psched == slIdleStand )
			return slFaceTarget1;	// override this for different target face behavior
		else
			return psched;
	case SCHED_TARGET_CHASE:
		return clFollow;
	case SCHED_CANT_FOLLOW:
		return slStopFollowing1;
	case SCHED_PANIC:
		return slSciPanic1;
	case SCHED_TARGET_CHASE_SCARED:
		return clFollowScared;
	case SCHED_IDLE_STAND:
		// call base class default so that scientist will talk
		// when standing during idle
		psched = CTalkMonster::GetScheduleOfType( Type );

		if( psched == slIdleStand )
			return slIdleSciStand1;
		else
			return psched;
	case SCHED_HIDE:
		return slScientistHide1;
	case SCHED_STARTLE:
		return slScientistStartle1;
	}

	return CTalkMonster::GetScheduleOfType( Type );
}

Schedule_t *CConstruction::GetSchedule( void )
{
	// so we don't keep calling through the EHANDLE stuff
	CBaseEntity *pEnemy = m_hEnemy;

	if( HasConditions( bits_COND_HEAR_SOUND ) )
	{
		CSound *pSound;
		pSound = PBestSound();

		ASSERT( pSound != NULL );
		if( pSound && ( pSound->m_iType & bits_SOUND_DANGER ) )
			return GetScheduleOfType( SCHED_TAKE_COVER_FROM_BEST_SOUND );
	}

	switch( m_MonsterState )
	{
	case MONSTERSTATE_ALERT:
	case MONSTERSTATE_IDLE:

		if( HasConditions( bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE ) )
		{
			// flinch if hurt
			return GetScheduleOfType( SCHED_SMALL_FLINCH );
		}

		// Cower when you hear something scary
		if( HasConditions( bits_COND_HEAR_SOUND ) )
		{
			CSound *pSound;
			pSound = PBestSound();

			ASSERT( pSound != NULL );
		}

		// Behavior for following the player
		if( IsFollowing() )
		{
			if( !m_hTargetEnt->IsAlive() )
			{
				// UNDONE: Comment about the recently dead player here?
				StopFollowing( FALSE );
				break;
			}

			int relationship = R_NO;

			// Nothing scary, just me and the player
			if( pEnemy != NULL )
				relationship = IRelationship( pEnemy );

			// UNDONE: Model fear properly, fix R_FR and add multiple levels of fear
			if( relationship != R_DL && relationship != R_HT )
			{
				// If I'm already close enough to my target
				if( TargetDistance() <= 128 )
				{
					if( HasConditions( bits_COND_CLIENT_PUSH ) )	// Player wants me to move
						return GetScheduleOfType( SCHED_MOVE_AWAY_FOLLOW );
				}
				return GetScheduleOfType( SCHED_TARGET_FACE );	// Just face and follow.
			}
		}

		if( HasConditions( bits_COND_CLIENT_PUSH ) )	// Player wants me to move
			return GetScheduleOfType( SCHED_MOVE_AWAY );

		break;
	case MONSTERSTATE_COMBAT:
		if( HasConditions( bits_COND_NEW_ENEMY ) )
			return slScientistCover1;
		if( HasConditions( bits_COND_SEE_ENEMY ) )
			return slScientistCover1;		// Take Cover

		if( HasConditions( bits_COND_HEAR_SOUND ) )
			return slTakeCoverFromBestSound;	// Cower and panic from the scary sound!

		return slScientistCover1;			// Run & Cower
		break;
	default:
		break;
	}
	
	return CTalkMonster::GetSchedule();
}

MONSTERSTATE CConstruction::GetIdealState( void )
{
	switch( m_MonsterState )
	{
	case MONSTERSTATE_ALERT:
	case MONSTERSTATE_IDLE:
		if( HasConditions( bits_COND_NEW_ENEMY ) )
		{
			if( IsFollowing() )
			{
				int relationship = IRelationship( m_hEnemy );
				if( relationship != R_FR || ( relationship != R_HT && !HasConditions( bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE ) ) )
				{
					// Don't go to combat if you're following the player
					m_IdealMonsterState = MONSTERSTATE_ALERT;
					return m_IdealMonsterState;
				}
				StopFollowing( TRUE );
			}
		}
		else if( HasConditions( bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE ) )
		{
			// Stop following if you take damage
			if( IsFollowing() )
				StopFollowing( TRUE );
		}
		break;
	case MONSTERSTATE_COMBAT:
		{
			CBaseEntity *pEnemy = m_hEnemy;
			if( pEnemy != NULL )
			{

				// Follow if only scared a little
				if( m_hTargetEnt != 0 )
				{
					m_IdealMonsterState = MONSTERSTATE_ALERT;
					return m_IdealMonsterState;
				}

				if( HasConditions( bits_COND_SEE_ENEMY ) )
				{
					m_IdealMonsterState = MONSTERSTATE_COMBAT;
					return m_IdealMonsterState;
				}
			}
		}
		break;
	default:
		break;
	}

	return CTalkMonster::GetIdealState();
}

int CConstruction::FriendNumber( int arrayNumber )
{
	static int array[3] = { 1, 2, 0 };
	if( arrayNumber < 3 )
		return array[arrayNumber];
	return arrayNumber;
}