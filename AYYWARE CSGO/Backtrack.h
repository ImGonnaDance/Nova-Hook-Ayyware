#pragma once

#include "Hacks.h"
#include <deque>

extern int cmd_tick_count;
namespace Globals
{
	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;
	
	extern int Shots;
	extern bool change;
	extern int TargetID;
	extern int getShotsFired;
}




struct LagRecord_t {
	float m_flSimulationTime;
	bool m_bValidRecord;
	QAngle m_vecAngles;
	Vector m_vecEyeAngles;
	Vector m_vecOrigin;
	Vector m_vecVelocity;
	Vector m_vecMins;
	Vector m_vecMaxs;
	float m_flLowerBodyYawTarget;
	Vector headSpot;
	std::array<float, 24> m_flPoseParameter;
	int m_flags;
	float m_flCycle;
	int m_nSequence;

	void Store(IClientEntity* pEnt) {
		m_vecOrigin = pEnt->GetAbsOrigin();
		m_vecAngles = pEnt->GetAbsAngles();
		m_vecEyeAngles = *pEnt->GetEyeAnglesXY();
		m_vecVelocity = pEnt->GetVelocity();
		//m_vecMins = pEnt->GetCollideable();
		//m_vecMaxs = pEnt->GetCollideable()->OBBMaxs();
		m_flSimulationTime = pEnt->GetSimulationTime();
		m_flLowerBodyYawTarget = pEnt->GetLowerBodyYaw();
		headSpot = pEnt->GetBonePos(8);
		m_flPoseParameter = pEnt->GetPoseParameters();
		m_flags = pEnt->GetFlags();
		m_flCycle = pEnt->getCycle();
		m_nSequence = pEnt->getSequence();

	};

	void Store(LagRecord_t record) {
		m_vecOrigin = record.m_vecOrigin;
		m_vecAngles = record.m_vecAngles;
		m_vecEyeAngles = record.m_vecEyeAngles;
		m_vecVelocity = record.m_vecVelocity;
		//m_vecMins = record.m_vecMins;
		//m_vecMaxs = record.m_vecMaxs;
		m_flSimulationTime = record.m_flSimulationTime;
		m_flLowerBodyYawTarget = record.m_flLowerBodyYawTarget;
		headSpot = record.headSpot;
		m_flPoseParameter = record.m_flPoseParameter;
		m_flags = record.m_flags;
		m_flCycle = record.m_flCycle;
		m_nSequence = record.m_nSequence;
		m_bValidRecord = true;
	};

	void Init() {
		m_vecAngles = Vector(0, 0, 0);
		m_flags = 0;
		m_nSequence = 0;
		m_flCycle = 0;
		m_vecOrigin = Vector(0, 0, 0);
		m_vecVelocity = Vector(0, 0, 0);
		m_vecEyeAngles = QAngle(0, 0, 0);
		headSpot = Vector(0, 0, 0);
		//m_vecMins = Vector(0, 0, 0);
		//m_vecMaxs = Vector(0, 0, 0);
		m_flLowerBodyYawTarget = 0.0f;
		m_flSimulationTime = 0.0f;
		std::array<float, 24> m_flPoseParameter = {};
	}
};


class CBackTrack {
public:
	void Inititalise();
	void Run(CUserCmd* pCmd, IClientEntity* pEnt);
	void RestoreEntity(IClientEntity* pEnt);
	void StoreEntity(IClientEntity* local, IClientEntity* pEntity);
	bool FindOldestRecord(IClientEntity * pEntity);
	bool IsValidTick(IClientEntity* pEnt, float simTime);

	float m_flTeleportDistanceSqr;
	int m_iTicks;

	struct BackTrackData_s {
		std::deque<LagRecord_t> m_PlayerRecords;

		void Init() {
			m_PlayerRecords.erase(m_PlayerRecords.begin(), m_PlayerRecords.end());
		};
	};

	BackTrackData_s m_BackTrackData[65];
	LagRecord_t m_PlayerTable;
	LagRecord_t m_RestoreData[65];

	ConVar*	pInterpVar;
	ConVar*	pRatioVar;
	ConVar*	pUpdateVar;
	ConVar*	pUnlagVar;
	ConVar*	pInterpolateVar;

	template< class T, class Y >
	T clamp(T const &val, Y const &minVal, Y const &maxVal) {
		if (val < minVal)
			return minVal;
		else if (val > maxVal)
			return maxVal;
		else
			return val;
	}
private:
	bool m_bInitialized = false;
};

extern std::unique_ptr<CBackTrack> pBackTrack;


enum PRIORITY : int
{
	PRIORITY_NONE = 0,
	PRIORITY_LOW = 1,
	PRIORITY_MEDIUM,
	PRIORITY_HIGH
};

struct LagRecord_tT
{
	int m_iTargetTickcount;
	int m_iPrevBullets;
	int m_iPrevFlags;
	int RecordPriorityFlag;
	bool m_bHasBetterRecord;
	bool m_bIsValidRecord;
	bool IsHeadVisible;
	std::array<float, 24> m_flAPoseParameters = {};
	float m_flSimulationTime;
	float m_flPreviousSimulationTime;
	float m_flBaseTime;
	Vector m_vecOrigin;
	Vector m_vecAngles;
	Vector m_vecPreviousOrigin;
	Vector m_vecPreviousAngles;

public:
	void InvalidateRecord() {
		m_bIsValidRecord = false;
	};

	void SaveInfo(IClientEntity* pEnt) {
		m_vecOrigin = pEnt->GetOrigin();
		m_vecAngles = *pEnt->GetEyeAnglesXY();
		m_flSimulationTime = pEnt->GetSimulationTime();
		m_flAPoseParameters = pEnt->GetPoseParameters();
		m_bIsValidRecord = false;
	};

};

class CTimeSimulator
{
public:

	LagRecord_tT m_PlayerTable[65];
	float OldLowerBodyYaw[65]; //not sure about array or other container
	float OldPoses[65]; //but as it doesn't complain about anything fuck it
	LagRecord_tT m_PrevPlayerTable[65];
	LagRecord_tT m_BacktrackedPlayerTable[65];
	std::deque<LagRecord_tT> m_PlayerGoodRecords[65];

	void UpdatePlayerTable(IClientEntity* pEntity);
	bool IsRecordValid(IClientEntity* pEntity, LagRecord_tT& Record);
	float lerpTime();
	bool IsTickValid(int Tick);
	bool LBYShouldUpdate(IClientEntity* pEntity);
	bool ProcessCmd(int iTargetIndex, CUserCmd* pCmd);
	void Cache();
	bool FindBestRecord(IClientEntity* pEntity);
	bool FindOldestRecord(IClientEntity* pEntity);
};
extern std::unique_ptr<CTimeSimulator> newtestBacktrack;



/*NEW BACKTRACK-------------------------------------------------





struct playerTickRecord {
	float m_flSimulationTime = 0.f;
	std::array<float, 24> m_flAPoseParameters = {};
	Vector m_vecOrigin = Vector(0, 0, 0);
	int m_iTargetTickcount;
	Vector m_vecAngles = Vector(0, 0, 0);
	bool m_bHasBetterRecord = false;
	int RecordPriorityFlag;
	int m_iPrevFlags;
	std::array<float, 24> OldPoses = {};
};



class BacktrackAyy
{
public:

private:
	bool IsTickValid(int tick);
	void UpdatePlayerTable(IClientEntity * pEntity);
	bool FindOldestRecord(IClientEntity * pEntity);
	std::array<playerTickRecord, 65> m_PlayerTable;
	float OldLowerBodyYaw[65];
	
};


NEW BACKTRACK END---------------------------------------------*/

/*

struct CTickRecord1 {
	CTickRecord1() {}
	CTickRecord1(IClientEntity* ent) {
		m_flLowerBodyYawTarget = ent->GetLowerBodyYaw();
		m_angEyeAngles = ent->GetEyeAngles();
		m_flCycle = ent->getCycle();
		m_flSimulationTime = ent->GetSimulationTime();
		m_nSequence = ent->getSequence();
		m_vecOrigin = ent->GetOrigin();
		m_vecVelocity = ent->GetVelocity();
		m_flPoseParameter = ent->GetPoseParameters();
		m_angAbsAngles = ent->GetAbsAngles();
		m_vecAbsOrigin = ent->GetAbsOrigin();
		m_flDuckSpeed = ent->GetDuckSpeed();
		m_flDuckAmount = ent->GetDuckAmount();
	}

	explicit operator bool() const noexcept {
		return m_flSimulationTime > 0.f;
	}

	bool operator>(const CTickRecord1& others) {
		return (m_flSimulationTime > others.m_flSimulationTime);
	}
	bool operator>=(const CTickRecord1& others) {
		return (m_flSimulationTime >= others.m_flSimulationTime);
	}
	bool operator<(const CTickRecord1& others) {
		return (m_flSimulationTime < others.m_flSimulationTime);
	}
	bool operator<=(const CTickRecord1& others) {
		return (m_flSimulationTime <= others.m_flSimulationTime);
	}
	bool operator==(const CTickRecord1& others) {
		return (m_flSimulationTime == others.m_flSimulationTime);
	}

	float m_flDuckAmount = 0.f;
	float m_flDuckSpeed = 0.f;
	float m_flLowerBodyYawTarget = 0.f;
	QAngle m_angEyeAngles = QAngle(0, 0, 0);
	float m_flCycle = 0.f;
	float m_flSimulationTime = 0.f;
	int m_nSequence = 0;
	Vector m_vecOrigin = Vector(0, 0, 0);
	Vector m_vecAbsOrigin = Vector(0, 0, 0);
	Vector m_vecVelocity = Vector(0, 0, 0);
	std::array<float, 24> m_flPoseParameter = {};
	QAngle m_angAbsAngles = QAngle(0, 0, 0);
	int tickcount = 0;
};

struct C_PlayerStored {
	CBaseCombatWeapon* CurrentWeapon = nullptr;
	int BulletsLeft = 0;
	bool EyeAnglesUpdated = false;
	bool IsFiring = false;
	int TickCount = 0;
	float SimulationTime = 0;
	QAngle EyeAngles = QAngle(0, 0, 0);
	CTickRecord1 ValidTick = CTickRecord1();
};

struct CBacktrackInfo {
	CBacktrackInfo() : shot(false), player(C_PlayerStored()), tick_count(0), temporaryRecord(CTickRecord1()) {}

	bool shot = false;
	C_PlayerStored player = C_PlayerStored();
	int tick_count = 0;
	CTickRecord1 temporaryRecord = CTickRecord1();


	IClientEntity* Entity;
	std::deque<CTickRecord1> TickRecords;
};

//#define Backtracking CBacktracking::GetInstance()

class CBacktracking {
	friend class CLagcompensation;
	friend class CResolver;
	friend class ayyResolver;

	//IMPLEMENT_SINGLETON(CBacktracking);

public:
	int ShotBackTrackedTick(IClientEntity* ent);
	void ShotBackTrackStoreFSN(IClientEntity* ent);
	void OverridePosesFsnRenderStart(IClientEntity* ent);
	void ShotBackTrackBeforeAimbot(IClientEntity* ent);
	void ShotBackTrackAimbotStart(IClientEntity* ent);
	void ShotBackTrackAimbotEnd(IClientEntity* ent);
	void ClearRecord(IClientEntity* Entity);
	bool bTickIsValid(int tick);
	void SaveTemporaryRecord(IClientEntity* ent, CTickRecord1 record = CTickRecord1());
	void RestoreTemporaryRecord(IClientEntity* ent);

private:
	void ApplyTickrecord(IClientEntity* ent, CTickRecord1 record);
	void ApplyTickRecordFSN(IClientEntity* pEntity, CTickRecord1 record);
	void ApplyTickRecordCM(IClientEntity* pEntity, CTickRecord1 record);

private:
	std::array<CBacktrackInfo, 32> arr_infos;
};







#pragma once

#define TIME_TO_TICKS( dt )        ( (int)( 0.5f + (float)(dt) / Interfaces->Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( Interfaces->Globals->interval_per_tick *( t ) )
#define ROUND_TO_TICKS( t )		( Interfaces->Globals->interval_per_tick * TIME_TO_TICKS( t ) )
#define TICK_NEVER_THINK		(-1)


class VarMapEntry_t {
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate; // Set to false when this var doesn't
										  // need Interpolate() called on it anymore.
	void *data;
	void *watcher;
};

struct VarMapping_t {
	VarMapping_t() {
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};




class CLagcompensation {
	friend class CResolver;
	friend class CBacktracking;

	VarMapping_t* GetVarMap(void* pBaseEntity) {
		return reinterpret_cast<VarMapping_t*>((DWORD)pBaseEntity + 0x24); //0x4C );
	}

	//IMPLEMENT_SINGLETON(CLagcompensation);
public:
	float GetLerpTime() const;

	void DisableInterpolation(IClientEntity* pEntity) {
		VarMapping_t* map = GetVarMap(pEntity);
		if (!map) return;
		for (int i = 0; i < map->m_nInterpolatedEntries; i++) {
			VarMapEntry_t *e = &map->m_Entries[i];
			e->m_bNeedsToInterpolate = false;
		}
	}
};

*/