/*
Rest In Peace ApocalypseCheats
*/

#pragma once

// It's actually in DLLMain but w/e
extern bool DoUnload;

#include "Utilities.h"
#include "Hacks.h"
extern bool islbyupdate;
extern float ProxyLBYtime;
extern int LBYBreakerTimer;
extern float enemysLastProxyTimer[65];
extern float fsnLBY;
extern float enemyLBYTimer[65];
extern int hittedLogHits[65];
extern int missedLogHits[65];
extern bool switchAntiAimSide;
extern float aaDistanceNew;
extern float aaDistanceOld;
extern int shotsfired;
//Resolver
extern int ResolverStage[65];
extern bool rWeInFakeWalk;
extern float testFloat1;
extern float testFloat2;
extern float testFloat3;
extern float testFloat4;
extern bool antiAimSide;

// Console Vars for LBY TESTS
extern float consoleProxyLbyLASTUpdateTime; // This is in ProxyLBY AntiAim.cpp
extern float consoleFsnMyPredictTime; // This is in FSN POSTSTART UPDATE
extern float consoleWhenLBYCalledOnAntiAim; // this is in CreateMove

extern float saveLastHeadshotFloat[65];
extern float saveLastBaimFloat[65];
extern float saveLastBaim30Float[65];
extern bool shotFakeHeadOnce[65];

extern float lineLBY;
extern float lineRealAngle;
extern float lineFakeAngle;
extern int fakeshotMissedLast[65];
extern float testVariable;
extern float pEntityPredLbyTime[65];
extern bool canBacktrack;
extern int enemysCurrentAmmo[65];
// Interfaces::Globals->interval_per_tick
extern float intervalPerTick;
extern int cmd_tick_count;
extern bool antiAimLookingForward;
extern bool BacktrackCanShotEnemy;

namespace Globals
{
	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;
	
	extern int Shots;
	extern bool change;
	extern int TargetID;
	extern int getShotsFired;
}

namespace Hooks
{
	void Initialise();
	void UndoHooks();
	
	class HooksXD;
	// VMT Managers
	extern Utilities::Memory::VMTManager VMTPanel; // Hooking drawing functions
	extern Utilities::Memory::VMTManager VMTClient; // Maybe CreateMove
	extern Utilities::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	extern Utilities::Memory::VMTManager VMTModelRender; // DrawModelEx for chams
	extern Utilities::Memory::VMTManager VMTPrediction; // InPrediction for no vis recoil
	extern Utilities::Memory::VMTManager VMTPlaySound; // Autoaccept shit
	extern Utilities::Memory::VMTManager VMTRenderView;
	extern Utilities::Memory::VMTManager VMTMDLCache;
	
};


struct logSomeData
{
	int index;
	float lby;
	float lastLbyUpdateTime;
	float delta;
	float ResolvedAngle;
	int tick;
	float m_fSimulationTime;
	Vector headSpot;
};

