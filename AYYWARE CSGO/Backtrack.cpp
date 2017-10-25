#include "Hacks.h"

#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME(t) (Interfaces::Globals->interval_per_tick * (t) )

CUserCmd*					Globals::UserCmd;


std::unique_ptr<CBackTrack> pBackTrack(std::make_unique<CBackTrack>());


int timeToTicks(float dt) {
	return (int)(0.5f + (float)(dt) / Interfaces::Globals->interval_per_tick);
}




void CBackTrack::Inititalise() {
	if (m_bInitialized)
		return;

	for (int s = 0; s <= 65; s++) {
		m_RestoreData[s].Init();
	}

	m_flTeleportDistanceSqr = 64 * 64;

	m_bInitialized = true;
}

void CBackTrack::Run(CUserCmd* pCmd, IClientEntity* pEnt) {
	
	int index = pEnt->GetIndex();

	if (!m_RestoreData[index].m_bValidRecord) return;
	
	if (m_iTicks < 10) return;

	if (!IsValidTick(pEnt, m_RestoreData[index].m_flSimulationTime)) return;
	
	pCmd->tick_count = TIME_TO_TICKS(m_RestoreData[index].m_flSimulationTime);
}

void CBackTrack::RestoreEntity(IClientEntity* pEnt) {

	int index = pEnt->GetIndex();
	if (!m_RestoreData[index].m_bValidRecord) return;
	if (m_iTicks < 10) return;

	if (!IsValidTick(pEnt, m_RestoreData[index].m_flSimulationTime)) return;

	/*THIS SHIT IS ONLY GOOD AGAINST LEGITS , IN HVH u will sucks everyones cock*/

	pEnt->InvalidateBoneCache();

	*(int*)((DWORD)pEnt + 0x100) = m_RestoreData[index].m_flags;
	*(float*)((DWORD)pEnt + 0xA14) = m_RestoreData[index].m_flCycle;
	*(int*)((DWORD)pEnt + 0x28AC) = m_RestoreData[index].m_nSequence;
	pEnt->setAbsOriginal(m_RestoreData[index].m_vecOrigin);
	pEnt->setAbsAechse(m_RestoreData[index].m_vecAngles);
	*(Vector*)((DWORD)pEnt + 0x110) = m_RestoreData[pEnt->GetIndex()].m_vecVelocity;
	pEnt->GetEyeAnglesXY()->x = m_RestoreData[index].m_vecEyeAngles.x;
	pEnt->GetEyeAnglesXY()->y = m_RestoreData[index].m_vecEyeAngles.y;
	pEnt->GetEyeAnglesXY()->z = m_RestoreData[index].m_vecEyeAngles.z;
	*(float*)((DWORD)pEnt + 0x39D8) = m_RestoreData[index].m_flLowerBodyYawTarget;
	*(float*)((DWORD)pEnt + 0x264) = m_RestoreData[index].m_flSimulationTime;

	pEnt->updateClientSideAnimation();

	//for (int i = 0; i < 24; i++) {
		//*(float*)((DWORD)pEnt + 0x2764 + sizeof(float) * i) = m_RestoreData[index].m_flPoseParameter[i];
	//}

}

static float OldLowerBodyYaw[65];

void CBackTrack::StoreEntity(IClientEntity* local, IClientEntity* pEntity) {
	

	int index = pEntity->GetIndex();

	m_PlayerTable.Store(pEntity);

	m_RestoreData[index].m_bValidRecord = false;

	// If the Player has >= 10 (Ticks /Record) 
	if (m_BackTrackData[index].m_PlayerRecords.size() >= static_cast<size_t>(10)) {
		// Please delete the last Record
		m_BackTrackData[index].m_PlayerRecords.pop_back();
	}
	// It pushes the actual Record to the front example: { Record_10, Record_7, Record_8, Record_9 }    actual Record is: Record_10
	m_BackTrackData[index].m_PlayerRecords.push_front(m_PlayerTable);

	// Saves the Record Size for example 7 Records for this Player in m_Ticks
	m_iTicks = m_BackTrackData[index].m_PlayerRecords.size();


	if (!local) return;
	// if 7 < 10 ?
	if (m_iTicks < 10) return;

	int targetTick = 10 - 1;
	
	Vector delta;
	delta = m_BackTrackData[index].m_PlayerRecords.at(targetTick).m_vecOrigin - pEntity->GetAbsOrigin();

	if (delta.Length2DSqr() > m_flTeleportDistanceSqr) return;
	if (m_BackTrackData[index].m_PlayerRecords.at(targetTick).m_flSimulationTime >= pEntity->GetSimulationTime()) return;

	m_RestoreData[index].Store(m_BackTrackData[index].m_PlayerRecords.at(targetTick));
	
}




bool CBackTrack::IsValidTick(IClientEntity* pEnt, float simTime) {

	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();

	pInterpVar = Interfaces::CVar->FindVar("cl_interp");
	pRatioVar = Interfaces::CVar->FindVar("cl_interp_ratio");
	pUpdateVar = Interfaces::CVar->FindVar("cl_updaterate");
	pUnlagVar = Interfaces::CVar->FindVar("sv_unlag");
	pInterpolateVar = Interfaces::CVar->FindVar("cl_interpolate");

	int iTickCount = Interfaces::Globals->tickcount;

	float flLerpTime = max(pInterpVar->GetFloat(), pRatioVar->GetFloat() / pUpdateVar->GetFloat());
	int iTargetTick = TIME_TO_TICKS(flLerpTime + simTime);
	int iServerTick = iTickCount + 1 + TIME_TO_TICKS(nci->GetLatency(FLOW_INCOMING) + nci->GetLatency(FLOW_OUTGOING));
	float flDeltaTime = clamp(flLerpTime + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(iServerTick + TIME_TO_TICKS(flLerpTime) - iTargetTick);

	if (fabs(flDeltaTime) > 0.2f) return false;

	return true;
} 


/*------------------------BACKTRACK ENW ------------------------------------------------------------*/
/*--------------BACKTRACK ENW ------------------------------------------------------------*/
/*---------------------------BACKTRACK ENW ------------------------------------------------------------*/


template< class T, class Y >
T clamp(T const &val, Y const &minVal, Y const &maxVal) {
	if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}
std::unique_ptr<CTimeSimulator> newtestBacktrack(std::make_unique<CTimeSimulator>());
float CTimeSimulator::lerpTime()
{
	float ud_rate = Interfaces::CVar->FindVar("cl_updaterate")->GetFloat();
	ConVar *min_ud_rate = Interfaces::CVar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = Interfaces::CVar->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetFloat();

	float ratio = Interfaces::CVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = Interfaces::CVar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");

	return max(lerp, (ratio / ud_rate));
}

bool CTimeSimulator::IsTickValid(int Tick)
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	float latency = nci->GetLatency(FLOW_OUTGOING);
	const float SV_MAXUNLAG = 1.0f;
	float m_flLerpTime = lerpTime();
	int lerpTicks = TIME_TO_TICKS(m_flLerpTime);
	float correct = clamp(latency + TICKS_TO_TIME(lerpTicks), 0.0f, SV_MAXUNLAG);
	int targettick = Tick - lerpTicks;
	int servertick = floorf(TIME_TO_TICKS(latency)) + 1 + Interfaces::Globals->tickcount;
	float deltaTime = correct - TICKS_TO_TIME(servertick - targettick);

	if (fabs(deltaTime) > 0.2f) {
		return false;
	}

	return true;
}


void CTimeSimulator::UpdatePlayerTable(IClientEntity* pEntity)
{
	// This is a vapse shitfest but still.

	int s = pEntity->GetIndex();
	float flSimTime = pEntity->GetSimulationTime();

	// Wenn LBY updaten sollte speichere die Zeit
	if (LBYShouldUpdate(pEntity))
		m_PlayerTable[s].m_iTargetTickcount = flSimTime;

	// Informationen speichern
	m_PlayerTable[s].m_flAPoseParameters = pEntity->GetPoseParameters();
	m_PlayerTable[s].m_vecOrigin = pEntity->GetOrigin();
	m_PlayerTable[s].m_vecAngles = *pEntity->GetEyeAnglesXY();
	m_PlayerTable[s].m_flSimulationTime = pEntity->GetSimulationTime();

	// Enemy Active Weapon und ob geschossen wurde
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pEntity->GetActiveWeaponHandle());
	int Bullets = -1;
	if (pWeapon)
		Bullets = pWeapon->GetAmmoInClip();

	if (Bullets < m_PlayerTable[s].m_iPrevBullets)
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].m_bHasBetterRecord = true;

		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_HIGH;
	} // Setze hihe priorität weil enemy geschossen hat .------------
	else if (OldLowerBodyYaw[s] != pEntity->GetLowerBodyYaw()) { // WENN Oldlby ist ungleich neue LBY
																  // It is necessary else, if it has already been removed in another function
		if (TIME_TO_TICKS(flSimTime) - m_PlayerTable[s].m_iTargetTickcount > 5) // Because my prediction is 
		{
			m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		}
		m_PlayerTable[s].m_bHasBetterRecord = true;
		m_PlayerTable[s].m_vecAngles = Vector(pEntity->GetEyeAnglesXY()->x, pEntity->GetLowerBodyYaw(), 0);

		// Would be high if we would predict it p100
		//
		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_MEDIUM;
	}/*
	else if (((*pEntity->Pos()).at(1) > (0.85f) && OldPoses[s].at(1) <= (0.85f)) ||
		((*pEntity->GetPosePosition()).at(1) <= (0.85f)) && OldPoses[s].at(1) > (0.85f)) // Crouch parameter, good for those crouch peekers. 1 = fully standing.
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].m_bHasBetterRecord = true;

		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_MEDIUM;
	} */
	else if (pEntity->GetFlags() == (int)FL_ONGROUND && m_PlayerTable[s].m_iPrevFlags == (int)FL_ONGROUND) // Landed, unreliable.
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].m_bHasBetterRecord = true;

		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_LOW;
	}
	else if (pEntity->GetEyeAnglesXY()->x < -80) // Assuming you cant fake pitch in that server (that is it has untrusted checks/is valve server)
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].m_bHasBetterRecord = true;

		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_LOW;
	}
	
	else
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_NONE;
	} 

	m_PlayerTable[s].m_iPrevBullets = Bullets;
	OldLowerBodyYaw[s] = pEntity->GetLowerBodyYaw();
	//OldPoses[s] = pEntity->GetPosePosition();
	m_PlayerTable[s].m_iPrevFlags = pEntity->GetFlags();


	//Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 235, 255), "backtracked: PriorityFlag %i\n", m_PlayerTable[s].RecordPriorityFlag);
	//Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 235, 255), "TargetTick %i\n", m_PlayerTable[s].m_iTargetTickcount);
	//Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 235, 255), "Index %i\n", pEntity->GetIndex());
	//Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 235, 255), "-----------------------------\n");


}

bool CTimeSimulator::FindOldestRecord(IClientEntity* pEntity)
{
	int s = pEntity->GetIndex();

	// We don't have any records, update the table
	// and just use the latest record.
	//
	if (m_PlayerGoodRecords[s].empty())
	{
		// How the hell did we even end up here?
		//
		return false;
	}

	for (auto Record = m_PlayerGoodRecords[s].begin(); Record != m_PlayerGoodRecords[s].end(); Record++)
	{
		// We've found an old and good record, set it to
		// our current one and return to backtrack.
		//
		if (IsTickValid(TIME_TO_TICKS(Record->m_flSimulationTime)))
		{
			m_PlayerTable[s] = *Record;
			m_PlayerGoodRecords[s].erase(Record);
			return true;
		}
		else
		{
			// Record isn't valid, erase it and start from the beginning,
			//
			m_PlayerGoodRecords[s].erase(Record);
			if (!m_PlayerGoodRecords[s].empty())
			{
				Record = m_PlayerGoodRecords[s].begin();
			}
			else
			{
				// Again, we didn't find a valid record and we've run out
				// of records to check, proceed to update and use latest record. SMH
				//
				return false;
			}
		}
	}

	return false;
}








void CTimeSimulator::Cache()
{
	IClientEntity* pLP = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	float flCurrentTime = Interfaces::Globals->interval_per_tick * (pLP->GetTickBase());

	for (int s = 1; s <= Interfaces::Engine->GetMaxClients(); ++s)
	{
		IClientEntity* pEntity = Interfaces::EntList->GetClientEntity(s);

		if (!pEntity)
			continue;

		if (pEntity->GetTeamNum() == pLP->GetTeamNum() || pLP == pEntity) {
			continue;
		}

		if (pEntity->IsDormant() || !pLP->IsAlive()) {
			continue;
		}


		m_PlayerTable[s].m_bIsValidRecord = false;

		// We're going to recreate the bone cache anyways so it's good to have FRESH pasta
		pEntity->InvalidateBoneCache();

		float flSimTime = pEntity->GetSimulationTime();

		UpdatePlayerTable(pEntity);

		if (IsRecordValid(pEntity, m_PlayerTable[s]))
		{
			m_PlayerTable[s].SaveInfo(pEntity);
			if (!m_PlayerGoodRecords[s].empty() && m_PlayerGoodRecords[s].size() > 10)
				m_PlayerGoodRecords[s].pop_back();
			m_PlayerGoodRecords[s].push_back(m_PlayerTable[s]);
		}

		pEntity->updateClientSideAnimation();

		if (m_BacktrackedPlayerTable[s].m_bIsValidRecord)
		{
			if (IsRecordValid(pEntity, m_BacktrackedPlayerTable[s]) )
			{
				// Record is good, don't invalidate
				m_PlayerTable[s].m_vecPreviousOrigin = pEntity->GetOrigin();
				m_PlayerTable[s].m_vecPreviousAngles = *pEntity->GetEyeAnglesXY();
				m_PlayerTable[s].m_flPreviousSimulationTime = flSimTime;
				m_PrevPlayerTable[s] = m_PlayerTable[s];
				continue;
			}
			else
			{
				m_BacktrackedPlayerTable[s].InvalidateRecord();
			}
		}

		//
		// If we got here that means our previous record has outdated
		//

		// Finding the best record, setting it to m_PlayerTable and going to backtracking
		//
		if (!FindOldestRecord(pEntity)) {
			m_PlayerTable[s].m_vecPreviousOrigin = pEntity->GetOrigin();
			m_PlayerTable[s].m_vecPreviousAngles = *pEntity->GetEyeAnglesXY();
			m_PlayerTable[s].m_flPreviousSimulationTime = flSimTime;
			m_PrevPlayerTable[s] = m_PlayerTable[s];
			continue;
		}

		// Save current record for backtracking as it is the best we have.
		//
		m_PlayerTable[s].m_bIsValidRecord = true;
		m_BacktrackedPlayerTable[s] = m_PlayerTable[s];

		m_PlayerTable[s].m_vecPreviousOrigin = pEntity->GetOrigin();
		m_PlayerTable[s].m_vecPreviousAngles = *pEntity->GetEyeAnglesXY();
		m_PlayerTable[s].m_flPreviousSimulationTime = flSimTime;
		m_PrevPlayerTable[s] = m_PlayerTable[s];
	}
}


bool CTimeSimulator::IsRecordValid(IClientEntity* pEntity, LagRecord_tT& src)
{
	IClientEntity* pLP = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	int s = pEntity->GetIndex();
	float flCurrentTime = Interfaces::Globals->interval_per_tick * pLP->GetTickBase();
	float flSimTime = pEntity->GetSimulationTime();

	if (src.m_vecPreviousOrigin.Length() > 0)
	{
		//SSDK::Vector delta = *pEntity->GetOrigin() - src.m_vecOrigin;
		Vector delta = src.m_vecPreviousOrigin - src.m_vecOrigin;
		if (delta.Length2DSqr() > (64 * 64))
		{
			return false;
		}
	}

	// That's because if something goes wrong, then it will be necessary to correct it if
	// seriously we can return to mexico
	if (fabs(flSimTime - flCurrentTime) > 0.3f) {
		return false;
	}

	if (!IsTickValid(TIME_TO_TICKS(src.m_flSimulationTime))) {
		return false;
	}

	return true;
}

bool CTimeSimulator::LBYShouldUpdate(IClientEntity* pEntity)
{

	float  flCurTime = Interfaces::Globals->curtime;

	bool IsMoving = ((pEntity->GetVelocity()).Length2D() > 0.1f && pEntity->GetFlags() & (int)FL_ONGROUND);
	bool bResult = (IsMoving ||
		((flCurTime - m_PlayerTable[pEntity->GetIndex()].m_flBaseTime) >= 1.1 && !IsMoving)); //Not moving, on ground timer.

	if (bResult)
		m_PlayerTable[pEntity->GetIndex()].m_flBaseTime = flCurTime;

	return bResult;
}


bool CTimeSimulator::ProcessCmd(int iTargetIndex, CUserCmd* pCmd)
{
	if (iTargetIndex == -1 || iTargetIndex > Interfaces::Engine->GetMaxClients())
		return false;

	if (!m_BacktrackedPlayerTable[iTargetIndex].m_bIsValidRecord)
		return false;
	/*
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	
	static ConVar* FakeLag = Interfaces::CVar->FindVar("net_fakelag");
	
	int Tick = TIME_TO_TICKS(m_BacktrackedPlayerTable[iTargetIndex].m_flSimulationTime);
		
	float latency = nci->GetLatency(FLOW_INCOMING);
	const float SV_MAXUNLAG = 1.0f;
	float m_flLerpTime = lerpTime();
	int lerpTicks = TIME_TO_TICKS(m_flLerpTime);
	float correct = clamp(latency + TICKS_TO_TIME(lerpTicks), 0.0f, SV_MAXUNLAG);
	int targettick = Tick - lerpTicks;

	int servertick = floorf(TIME_TO_TICKS(latency)) + 1 + Interfaces::Globals->tickcount;
	float deltaTime = correct - TICKS_TO_TIME(servertick - targettick);

	if (fabs(deltaTime) > 0.2f && fabs(deltaTime) < 1.f) {

		INetChannelInfo* pNetChannel = Interfaces::Engine->GetNetChannelInfo();
		*(int*)((DWORD)pNetChannel + 0x1C) -= TIME_TO_TICKS(0.1f);

		ConVar* FakeLag = Interfaces::CVar->FindVar("net_fakelag");

		float Value = (fabs(deltaTime) * 1000) - 200; //( - 0.2f * 1000)

		FakeLag->SetValue(Value);
	}
	*/


	pCmd->tick_count = TIME_TO_TICKS(m_BacktrackedPlayerTable[iTargetIndex].m_flSimulationTime);
	return true;
}


/*

inline float GetNetworkLatency(int TYPE)
{
	
	// Get true latency
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
		return nci->GetAvgLatency(TYPE);
	return 0.0f;
}

LagCompensation::LagCompensation()
{

}

void LagCompensation::logEntity(IClientEntity *player)
{
	int idx = player->GetIndex();
	LagRecord *m_LagRecords = this->m_LagRecord[idx];

	if (!player || !player->GetHealth() > 0)
	{
		for (int i = 0; i < 11; i++)
		{
			m_LagRecords[i].m_fSimulationTime = 0.0f;
		}
	}

	float simTime = player->GetSimulationTime();

	if (!isValidTick(simTime))
		return;

	int highestRecordIdx = -1;
	float highestSimTime = 0.0f;

	for (int i = 0; i < 11; i++)
	{
		if (m_LagRecords[i].m_fSimulationTime > simTime)
			m_LagRecords[i].m_fSimulationTime = 0.0f;

		if (m_LagRecords[i].m_fSimulationTime == 0.0f)
			continue;

		if (m_LagRecords[i].m_fSimulationTime == simTime)
			return;

		if (m_LagRecords[i].m_fSimulationTime > highestSimTime)
		{
			highestRecordIdx = i;
			highestSimTime = m_LagRecords[i].m_fSimulationTime;
		}
	}

	highestRecordIdx++;
	highestRecordIdx = highestRecordIdx % 11;

	m_LagRecords[highestRecordIdx].m_bIsFixed = false;
	m_LagRecords[highestRecordIdx].m_iTargetID = idx;

	m_LagRecords[highestRecordIdx].headSpot = player->GetBonePos(8);

	m_LagRecords[highestRecordIdx].m_vAbsOrigin = player->getAbsOriginal();
	m_LagRecords[highestRecordIdx].m_vecVelocity = player->GetVelocity();
	m_LagRecords[highestRecordIdx].m_fSimulationTime = player->GetSimulationTime();
	m_LagRecords[highestRecordIdx].m_vecAngles = player->getAbsAechse();
	m_LagRecords[highestRecordIdx].m_flCycle = player->getCycle();
	m_LagRecords[highestRecordIdx].m_nSequence = player->getSequence();
	m_LagRecords[highestRecordIdx].flags = player->GetFlags();
	m_LagRecords[highestRecordIdx].m_flLowerBodyYawTarget = player->GetLowerBodyYaw();

	for (int i = 0; i < 24; i++)
		m_LagRecords[highestRecordIdx].m_flPoseParameter[i] = player->getPoseParams(i);
}

void LagCompensation::logCurrentEnt(IClientEntity *player)
{
	int idx = player->GetIndex();

	this->m_PrevRecords[idx].m_vAbsOrigin = player->getAbsOriginal();
	this->m_PrevRecords[idx].m_vecVelocity = player->GetVelocity();
	this->m_PrevRecords[idx].m_fSimulationTime = player->GetSimulationTime();
	this->m_PrevRecords[idx].m_vecAngles = player->getAbsAechse();
	this->m_PrevRecords[idx].m_eyeAngles = player->GetEyeAngles();
	this->m_PrevRecords[idx].m_flCycle = player->getCycle();
	this->m_PrevRecords[idx].m_nSequence = player->getSequence();
	this->m_PrevRecords[idx].flags = player->GetFlags();
	this->m_PrevRecords[idx].m_flLowerBodyYawTarget = player->GetLowerBodyYaw();

	for (int i = 0; i < 24; i++)
		this->m_PrevRecords[idx].m_flPoseParameter[i] = player->getPoseParams(i);
}

void LagCompensation::setEntity(IClientEntity *player, LagRecord record)
{ 
	if (!player || record.m_fSimulationTime == 0.0f)
	{
		record.m_bIsFixed = false;
		return;
	}
	
	this->logCurrentEnt(player);
	//m_nWritableBones = m_nForceBone + 0x20;  m_nForceBone = 0x267C
	DWORD invalKostCache = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1\x00\x00\x00\x00\x48\xC7\x81", "xx?????xxx????xxx");
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalKostCache + 10);

	*(int*)((DWORD)player + 0x267C + 0x20) = 0;
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF; 
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1); 

	//static int offs = GET_NETVAR("DT_CSPlayer", "m_angEyeAngles");
	// (QAngle*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_angEyeAngles")
	player->setAbsOriginal(record.m_vAbsOrigin);
	
	//*(float*)((DWORD)player + 0x264) = record.m_fSimulationTime;
	//player->setAbsAechse(record.m_vecAngles);
	//player->setAbsAngles(record.m_vecAngles);
	//*(Vector*)(DWORD)player + 0xB22C = record.m_eyeAngles;
	player->GetEyeAnglesXY()->y = record.m_eyeAngles.y;
	player->GetEyeAnglesXY()->x = record.m_eyeAngles.x;
	player->GetEyeAnglesXY()->z = record.m_eyeAngles.z;
	*(float*)((DWORD)player + 0xA14) = record.m_flCycle;
	*(int*)((DWORD)player + 0x28AC) = record.m_nSequence;
	*(int*)((DWORD)player + 0x100) = record.flags;
	*(float*)((DWORD)player + 0x39D8) = record.m_flLowerBodyYawTarget;
	

	for (int i = 0; i < 24; i++)
		*(float*)((DWORD)player + 0x2764 + sizeof(float) * i) = record.m_flPoseParameter[i];


	record.m_iWantedTick = record.m_fSimulationTime;
	record.m_bIsFixed = true;

	Interfaces::CVar->ConsoleColorPrintf(Color(255,0,235,255), "backtracked %i\n", player->GetIndex());

}

void LagCompensation::setCurrentEnt(IClientEntity *player)
{
	int idx = player->GetIndex();
	static DWORD invalKostCache;

	m_PrevRecords[idx].m_iWantedTick = m_PrevRecords[idx].m_fSimulationTime;

	if (!invalKostCache) {
		invalKostCache = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1\x00\x00\x00\x00\x48\xC7\x81", "xx?????xxx????xxx");
	}
	
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalKostCache + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1);

	player->setAbsOriginal(m_PrevRecords[idx].m_vAbsOrigin);
	//*(float*)((DWORD)player + 0x264) = m_PrevRecords[idx].m_fSimulationTime;
	player->GetEyeAnglesXY()->y = m_PrevRecords[idx].m_eyeAngles.y;
	player->GetEyeAnglesXY()->x = m_PrevRecords[idx].m_eyeAngles.x;
	player->GetEyeAnglesXY()->z = m_PrevRecords[idx].m_eyeAngles.z;


	*(float*)((DWORD)player + 0xA14) = m_PrevRecords[idx].m_flCycle;
	*(int*)((DWORD)player + 0x28AC) = m_PrevRecords[idx].m_nSequence;
	*(int*)((DWORD)player + 0x100) = m_PrevRecords[idx].flags;
	*(float*)((DWORD)player + 0x39D8) = m_PrevRecords[idx].m_flLowerBodyYawTarget;


	for (int i = 0; i < 24; i++)
		*(float*)((DWORD)player + 0x2764 + sizeof(float) * i) = m_PrevRecords[idx].m_flPoseParameter[i];

}

template<class T, class U>
T LagCompensation::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

float LagCompensation::lerpTime()
{
	ConVar * CvarInterp = Interfaces::CVar->FindVar("cl_interp");
	ConVar * CvarInterpRatio = Interfaces::CVar->FindVar("cl_interp_ratio");
	ConVar * CvarMinInterpRatio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	ConVar * CvarMaxInterpRatio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");
	ConVar * CvarUpdateRate = Interfaces::CVar->FindVar("cl_updaterate");
	ConVar * CvarMinUpdateRate = Interfaces::CVar->FindVar("sv_minupdaterate");
	ConVar * CvarMaxUpdateRate = Interfaces::CVar->FindVar("sv_maxupdaterate");

	auto updateRate = CvarUpdateRate->GetInt();
	auto interpRatio = CvarInterpRatio->GetFloat();
	auto minInterpRatio = CvarMinInterpRatio->GetFloat();
	auto maxInterpRatio = CvarMaxInterpRatio->GetFloat();
	auto minUpdateRate = CvarMinUpdateRate->GetInt();
	auto maxUpdateRate = CvarMaxUpdateRate->GetInt();


	auto clampedUpdateRate = clamp(updateRate, minUpdateRate, maxUpdateRate);
	auto clampedInterpRatio = clamp(interpRatio, minInterpRatio, maxInterpRatio);

	auto lerp = clampedInterpRatio / clampedUpdateRate;

	if (lerp <= CvarInterp->GetFloat())
		lerp = CvarInterp->GetFloat();

	return lerp;
}

void LagCompensation::initLagRecord()
{
	for (int i = 0; i <= 32; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			m_LagRecord[i][j].m_fSimulationTime = 0.0f;
		}
	}
}

int LagCompensation::fixTickcount(IClientEntity *player)
{
	static int BacktrackTicks = 10;
	int idx = player->GetIndex();

	LagRecord *m_LagRecords = this->m_LagRecord[idx];

	LagRecord recentLR = m_LagRecords[BacktrackTicks];

	if (recentLR.m_fSimulationTime == 0.0f)
		return TIME_TO_TICKS(player->GetSimulationTime()) + 1;

	int iLerpTicks = TIME_TO_TICKS(lerpTime());
	int iTargetTickCount = TIME_TO_TICKS(recentLR.m_fSimulationTime);
	
	Interfaces::CVar->ConsoleColorPrintf(Color(0,0,255,0), "tickcount: %d\n", (iTargetTickCount + 1));

	return iTargetTickCount + 1;
}

bool LagCompensation::isValidTick(float simTime)
{
	int Tick = TIME_TO_TICKS(simTime);
	float latency = GetNetworkLatency(FLOW_INCOMING);
	const float SV_MAXUNLAG = 1.0f;
	float m_flLerpTime = lerpTime();
	int lerpTicks = TIME_TO_TICKS(m_flLerpTime);
	float correct = clamp(latency + TICKS_TO_TIME(lerpTicks), 0.0f, SV_MAXUNLAG);
	int targettick = Tick - lerpTicks;
	int servertick = floorf(TIME_TO_TICKS(latency)) + 1 + cmd_tick_count;
	float deltaTime = correct - TICKS_TO_TIME(servertick - targettick);

	if (fabs(deltaTime) > 0.2f) {
		return false;
	}

	return true;

}


int LagCompensation::fakeLagFix(IClientEntity *player, int historyIdx)
{


	int idx = player->GetIndex();

	LagRecord *m_LagRecords = this->m_LagRecord[idx];

	LagRecord recentLR = m_LagRecords[historyIdx];
	LagRecord prevLR;
	if (historyIdx == 0)
		prevLR = m_LagRecords[8];
	else
		prevLR = m_LagRecords[historyIdx - 1];

	if (recentLR.m_fSimulationTime == 0.0f)
		return -1;


	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	int predCmdArrivTick = cmd_tick_count + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING)); // (c) n0xius @ uc

	int iLerpTicks = TIME_TO_TICKS(lerpTime());
	int iTargetTickCount = TIME_TO_TICKS(recentLR.m_fSimulationTime) + iLerpTicks;



	float flCorrect = clamp(lerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + TIME_TO_TICKS(lerpTime()) - iTargetTickCount); // (c) n0xius @ uc

	if (fabs(flCorrect) > 0.2f) // Too big deltatime, can't lagfix here
	{
		
		Interfaces::CVar->ConsoleColorPrintf(Color(255,0,0,255),"StartLagCompensation: delta too big (%.3f)\n", flCorrect);
		return -1;

	}

	if ((recentLR.m_vAbsOrigin - prevLR.m_vAbsOrigin).LengthSqr() > 4096.f)
	{	
		//recentLR.m_bIsBreakingLagComp = true;
		return -1;
	}
	else
	{
		setEntity(player, recentLR);
	}

	return TIME_TO_TICKS(recentLR.m_fSimulationTime);
}






void LagCompensation::log(ClientFrameStage_t stage)
{

	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!Interfaces::Engine->IsInGame())
		return;

	if (!pLocal || pLocal == nullptr)
		return;

	
		if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_END)
		{
			for (int i = 1; i < Interfaces::Engine->GetMaxClients(); i++)
			{
				IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

				if (!pEntity)
					continue;

				if (!pEntity) {
					continue;
				}

				if (pEntity->GetTeamNum() == pLocal->GetTeamNum()) {
					continue;
				}

				if (pEntity->IsDormant() || !pLocal->IsAlive()) {
					continue;
				}
				
				this->logEntity(pEntity);
				
				
			}
		}
	
}

*/
/*
bool BacktrackAyy::IsTickValid(int tick) {

	float latency = GetNetworkLatency(FLOW_OUTGOING);
	const float SV_MAXUNLAG = 1.0f;
	float m_flLerpTime = lerpTime1();

	int lerpTicks = TIME_TO_TICKS(m_flLerpTime);
	float correct = clamp(latency + TICKS_TO_TIME(lerpTicks), 0.0f, SV_MAXUNLAG);

	int targettick = tick - lerpTicks;
	int servertick = floorf(TIME_TO_TICKS(latency)) + 1 + Interfaces::Globals->tickcount;

	float deltaTime = correct - TICKS_TO_TIME(servertick - targettick);

	if (fabs(deltaTime) > 0.2f) {
		return false;
	}

	return true;


} */


/*
void BacktrackAyy::UpdatePlayerTable(IClientEntity* pEntity)
{
	// This is a vapse shitfest but still.

	int s = pEntity->GetIndex();
	float flSimTime = pEntity->GetSimulationTime();

	// Wenn LBY updaten sollte speichere die Zeit
	if (LBYShouldUpdate(pEntity))
		m_PlayerTable[s].m_iTargetTickcount = flSimTime;

	// Informationen speichern
	m_PlayerTable[s].m_flAPoseParameters = pEntity->GetPoseParameters();
	m_PlayerTable[s].m_vecOrigin = pEntity->GetOrigin();
	m_PlayerTable[s].m_vecAngles = pEntity->GetEyeAngles();
	m_PlayerTable[s].m_flSimulationTime = pEntity->GetSimulationTime();


	if (OldLowerBodyYaw[s] != pEntity->GetLowerBodyYaw()) { // WENN Oldlby ist ungleich neue LBY
																  // It is necessary else, if it has already been removed in another function
		if (TIME_TO_TICKS(flSimTime) - m_PlayerTable[s].m_iTargetTickcount > 5) // Because my prediction is 
		{
			m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		}
		m_PlayerTable[s].m_bHasBetterRecord = true;
		m_PlayerTable[s].m_vecAngles = Vector(pEntity->GetEyeAnglesXY()->x, pEntity->GetLowerBodyYaw(), 0);

		// Would be high if we would predict it p100
		//
		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_MEDIUM;
	}
	else if (pEntity->GetFlags() == FL_ONGROUND && m_PlayerTable[s].m_iPrevFlags == FL_ONGROUND) // Landed, unreliable.
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].m_bHasBetterRecord = true;

		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_LOW;
	}
	else if (pEntity->GetEyeAnglesXY()->x < -80) // Assuming you cant fake pitch in that server (that is it has untrusted checks/is valve server)
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].m_bHasBetterRecord = true;

		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_LOW;
	}
	else
	{
		m_PlayerTable[s].m_iTargetTickcount = TIME_TO_TICKS(flSimTime);
		m_PlayerTable[s].RecordPriorityFlag |= PRIORITY_NONE;
	}

	//m_PlayerTable[s].m_iPrevBullets = Bullets;
	OldLowerBodyYaw[s] = pEntity->GetLowerBodyYaw();
	m_PlayerTable[s].OldPoses = pEntity->GetPoseParameters();
	m_PlayerTable[s].m_iPrevFlags = pEntity->GetFlags();
}


bool BacktrackAyy::FindOldestRecord(IClientEntity* pEntity)
{
	int s = pEntity->GetIndex();

	// We don't have any records, update the table
	// and just use the latest record.
	//
	if (m_PlayerGoodRecords[s].empty())
	{
		// How the hell did we even end up here?
		//
		return false;
	}

	for (auto Record = m_PlayerGoodRecords[s].begin(); Record != m_PlayerGoodRecords[s].end(); Record++)
	{
		// We've found an old and good record, set it to
		// our current one and return to backtrack.
		//
		if (IsTickValid(TIME_TO_TICKS(Record->m_flSimulationTime)))
		{
			m_PlayerTable[s] = *Record;
			m_PlayerGoodRecords[s].erase(Record);
			return true;
		}
		else
		{
			// Record isn't valid, erase it and start from the beginning,
			//
			m_PlayerGoodRecords[s].erase(Record);
			if (!m_PlayerGoodRecords[s].empty())
			{
				Record = m_PlayerGoodRecords[s].begin();
			}
			else
			{
				// Again, we didn't find a valid record and we've run out
				// of records to check, proceed to update and use latest record. SMH
				//
				return false;
			}
		}
	}

	return false;
}

*/















#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class txufbjl {
public:
	string dganab;
	string qmtioa;
	string duyqe;
	txufbjl();
	void icbpyqhzrun(int xhdei, string ujomfshwfhat, int oganeki, double hlqmmnt);
	bool mskfhkgoyepuleplspsniegc(double zbpkiebiog);
	double qbrxhkuvwsdzbvyptte(bool viiqgepwjol, string biehomqfpencyyn, int iqffhbueaxfo, bool najyt, double afovxsrjeofjz, int kpmkporchwc, int cjcrzq, double zyrnkpme, int cgkcsiy);
	string slzipdaegrjv(bool hxnskwfdqfd, bool dwqpaptrlwsbe, double nuhwblonhjfqr, double bflhcy);
	int drohtaonoq(bool simhsoemxuqo, int obsrnpa, int qugezh, bool gmhnlvtulay, double sppesbtenef, double mrfdbsc, int qvjpgvrs, string helqbtfe, int wqbnbcqbdqbzzi);
	int rcjhjejujvxf(bool zwuqhufmj, double uwgxpctikw);
	double ellgoxfxgldbsdztgov(double mkzjpaayakb, string mavrpbdjkncdtyq, bool osmljcjvnzopf, string lztabtfmobs, double zebdxp, bool yvvnrkzufzbr, int hqsydw);

protected:
	int seetrkorzq;
	string ckwowrexp;
	bool kabvftcuegp;

	int bzvnlxkyswojmnylqjjuegbpo(int mdzihbflf, string xrpotvhkafujpoq, int kyuqv);
	string kwnkkxzknmhvitrjge(string hqrjdqvbllw, string ftpzbkesb, int kfapurgijramsz, int tcskfghzm, string azuqxryfpmfp, string ykzdirbrmen, bool xwznypehxnax, int yiogyatwxym);
	bool eopslajfiwduddqlh(double hkwqje, int yqsporewt, string mjhuhk, int myukicfx, bool olieawtqmdjqd, double xarnnzisulwom);
	void vdclqoydinoszwhtotlmw(int avoknxcfmbh, double ulcxd, int xerbcvddiuqdskk, string pmcdq, double lzmkdqhbz, bool utjapyvjinxqtnl, double rlrvheocujg, bool diueowjc, string dpfoybvnpwha);
	bool xjqfdrmexsqani(double sygzlzot, int vrytsbbjqlaakpr, int mujtwuxls, int iibqbzeeuxyp, double oxmitwaqcno);
	string vpfjssskwbdkqjksci(string bxwcpldpy, double cvrrahdszdjku, double rvkfiinifeqayl, int eysrzqezuuoq, double ruwhadcckynfrc, int bslztzn, bool kbxfvhe, double jpluvqhudh);
	int dxcydmqctawt(bool pulmrqdnnxrs, string jtalm, int yxfdjcz, bool foctprsvc, string snouwx, bool sevwehszeabt, string trqgafehkix, double mbzfsqpzlglufff);

private:
	bool fgroh;
	int eixhmgcxsb;
	string otgdxpdwmyzgxkq;
	double jkswdye;

	bool eqpfhqznplitciprd(bool nnvgsn);
	int xjqchrandjge(bool bznshkotloytp, double ugptuyrhiahiiud, string txnuytwcbx, string aywiadjzm, double moefviaaabwmhm, double ablbuthpldadmr);
	void nleloodlihmgroghntlgk(bool fvyuvluyao, bool nwkycrn, string jjixfu, bool uhkblmdhkwchzix, bool rxzoqspewbaf, double eqdazyucaml, string vycluzkh, double dmvhemmvlv);
	bool eqorcddrkqckuz(string mmgsxyndgb, bool ccymwsqztvbaigo, int vqsit, double yzrwlk, string ekynbq, int vwzzg, double mdbkvlriwe);
	void weduwjcpvmdhbfjgaioliguhw(string mlnxaklhmibfpb, bool orckahympuva, double ktbyeti, string lbbtynqjvsbpxte, double ovrlvswspasygt, string lbmpy, bool gvnyyoxt, double wnntaodvylv, double zznghdl, int cuplvslsjntscn);
	double wznjylaemtwbdaakfrf(string izaveduzcbqlo, int qskqdrlyppyjaz, string ryxvsdhenhas);
	int mnsgbbrgbhjah(bool wmzadyk, double sihlrxdfil, double oklbju, int xjcsje);
	string ydofslyyuvivzaw(bool tvvzdrw, double yrtuzs, int hbxfibgcdyqptki, bool luxmoewqlnx, string ecnoxtfqyieh, int juwrrmc, string xtlxmja);

};


bool txufbjl::eqpfhqznplitciprd(bool nnvgsn) {
	double munnfhflbu = 43628;
	string uzvyduxsxbc = "hiohgbwuushwvuppebgckupdvjpimsam";
	int twlldtcwh = 5072;
	string fjlwnwecscerpw = "cuqvamhmuyxjrjjmilzwrshniuwahcfsazdwtibxwxmbmequpulevquhszdxhllhzygrmwzzqevnhq";
	string dzmhjryjk = "kediwjvkpkgngfbpwfbbjgnfhrbaldfhgndgkosg";
	if (string("kediwjvkpkgngfbpwfbbjgnfhrbaldfhgndgkosg") != string("kediwjvkpkgngfbpwfbbjgnfhrbaldfhgndgkosg")) {
		int du;
		for (du = 36; du > 0; du--) {
			continue;
		}
	}
	return true;
}

int txufbjl::xjqchrandjge(bool bznshkotloytp, double ugptuyrhiahiiud, string txnuytwcbx, string aywiadjzm, double moefviaaabwmhm, double ablbuthpldadmr) {
	int wdnrnhgtyevkyoz = 1379;
	double idxzlw = 40212;
	int iomameeudjkcxr = 561;
	string ehqiiyhxceb = "husdnesjjjtnyogtkzcsfszcupttphfylcwlflfsmbw";
	if (40212 == 40212) {
		int ytwexecyh;
		for (ytwexecyh = 41; ytwexecyh > 0; ytwexecyh--) {
			continue;
		}
	}
	if (1379 != 1379) {
		int qedutyf;
		for (qedutyf = 68; qedutyf > 0; qedutyf--) {
			continue;
		}
	}
	if (1379 == 1379) {
		int rq;
		for (rq = 80; rq > 0; rq--) {
			continue;
		}
	}
	if (561 != 561) {
		int krjjp;
		for (krjjp = 33; krjjp > 0; krjjp--) {
			continue;
		}
	}
	return 26057;
}

void txufbjl::nleloodlihmgroghntlgk(bool fvyuvluyao, bool nwkycrn, string jjixfu, bool uhkblmdhkwchzix, bool rxzoqspewbaf, double eqdazyucaml, string vycluzkh, double dmvhemmvlv) {
	int nwbnwrpxumzehze = 8712;
	int pqsvnumuqlfhz = 3349;
	string jyuuu = "irhqdkwwvqpzkcdnxgjqtypybpvfrsqdrnvulkzyfuzgluaojijfnx";
	if (string("irhqdkwwvqpzkcdnxgjqtypybpvfrsqdrnvulkzyfuzgluaojijfnx") != string("irhqdkwwvqpzkcdnxgjqtypybpvfrsqdrnvulkzyfuzgluaojijfnx")) {
		int iv;
		for (iv = 21; iv > 0; iv--) {
			continue;
		}
	}
	if (string("irhqdkwwvqpzkcdnxgjqtypybpvfrsqdrnvulkzyfuzgluaojijfnx") == string("irhqdkwwvqpzkcdnxgjqtypybpvfrsqdrnvulkzyfuzgluaojijfnx")) {
		int ue;
		for (ue = 21; ue > 0; ue--) {
			continue;
		}
	}
	if (8712 != 8712) {
		int xedfawsese;
		for (xedfawsese = 9; xedfawsese > 0; xedfawsese--) {
			continue;
		}
	}

}

bool txufbjl::eqorcddrkqckuz(string mmgsxyndgb, bool ccymwsqztvbaigo, int vqsit, double yzrwlk, string ekynbq, int vwzzg, double mdbkvlriwe) {
	bool olxoso = true;
	int igoznof = 1981;
	if (1981 == 1981) {
		int bpfhvergff;
		for (bpfhvergff = 32; bpfhvergff > 0; bpfhvergff--) {
			continue;
		}
	}
	if (1981 != 1981) {
		int yi;
		for (yi = 61; yi > 0; yi--) {
			continue;
		}
	}
	if (true == true) {
		int iyfgpwbzg;
		for (iyfgpwbzg = 83; iyfgpwbzg > 0; iyfgpwbzg--) {
			continue;
		}
	}
	return true;
}

void txufbjl::weduwjcpvmdhbfjgaioliguhw(string mlnxaklhmibfpb, bool orckahympuva, double ktbyeti, string lbbtynqjvsbpxte, double ovrlvswspasygt, string lbmpy, bool gvnyyoxt, double wnntaodvylv, double zznghdl, int cuplvslsjntscn) {
	string dwasivig = "pesyhiqnpdngaylnrsldqhvmmrgkjovnjsboajlp";

}

double txufbjl::wznjylaemtwbdaakfrf(string izaveduzcbqlo, int qskqdrlyppyjaz, string ryxvsdhenhas) {
	double ovrygt = 59267;
	string dhtjypi = "xrxcdmkuevywmpgbhvuoqxclbyoyhuhdvuppvwbtzfwdegbqawuuvwmd";
	int zsrvbnvhcyiz = 3377;
	string sbpegrxwwbysqd = "glgvtcdocehmgsrylhoohuayvuroujxymnap";
	double acbhdkglul = 11859;
	if (string("glgvtcdocehmgsrylhoohuayvuroujxymnap") == string("glgvtcdocehmgsrylhoohuayvuroujxymnap")) {
		int zhb;
		for (zhb = 24; zhb > 0; zhb--) {
			continue;
		}
	}
	if (string("xrxcdmkuevywmpgbhvuoqxclbyoyhuhdvuppvwbtzfwdegbqawuuvwmd") == string("xrxcdmkuevywmpgbhvuoqxclbyoyhuhdvuppvwbtzfwdegbqawuuvwmd")) {
		int rwtfkdwfe;
		for (rwtfkdwfe = 88; rwtfkdwfe > 0; rwtfkdwfe--) {
			continue;
		}
	}
	if (string("xrxcdmkuevywmpgbhvuoqxclbyoyhuhdvuppvwbtzfwdegbqawuuvwmd") == string("xrxcdmkuevywmpgbhvuoqxclbyoyhuhdvuppvwbtzfwdegbqawuuvwmd")) {
		int cfsyyjjlm;
		for (cfsyyjjlm = 30; cfsyyjjlm > 0; cfsyyjjlm--) {
			continue;
		}
	}
	if (11859 == 11859) {
		int cvctnrpncp;
		for (cvctnrpncp = 37; cvctnrpncp > 0; cvctnrpncp--) {
			continue;
		}
	}
	return 646;
}

int txufbjl::mnsgbbrgbhjah(bool wmzadyk, double sihlrxdfil, double oklbju, int xjcsje) {
	double dxiisbr = 13288;
	bool sakudwkywy = false;
	string vxvype = "ppfdsdzqkdwukomyprbgyn";
	double ehbsjebijptkd = 47118;
	bool jkpvbwjk = false;
	int tgugqiipggab = 4522;
	if (13288 != 13288) {
		int zqzvtvgork;
		for (zqzvtvgork = 43; zqzvtvgork > 0; zqzvtvgork--) {
			continue;
		}
	}
	if (4522 != 4522) {
		int nitx;
		for (nitx = 94; nitx > 0; nitx--) {
			continue;
		}
	}
	if (47118 == 47118) {
		int xnpl;
		for (xnpl = 3; xnpl > 0; xnpl--) {
			continue;
		}
	}
	return 90532;
}

string txufbjl::ydofslyyuvivzaw(bool tvvzdrw, double yrtuzs, int hbxfibgcdyqptki, bool luxmoewqlnx, string ecnoxtfqyieh, int juwrrmc, string xtlxmja) {
	double mdvxdpiiry = 7123;
	string kybuewygcv = "obmjheymqtzptgaulxufiwwpdruyqoefjpkufklglpglxvzqswxhuvniulrufgt";
	bool zaezoeekh = false;
	double tckzmpp = 6438;
	double mobsbhwmpuegaww = 76;
	if (string("obmjheymqtzptgaulxufiwwpdruyqoefjpkufklglpglxvzqswxhuvniulrufgt") != string("obmjheymqtzptgaulxufiwwpdruyqoefjpkufklglpglxvzqswxhuvniulrufgt")) {
		int sbel;
		for (sbel = 32; sbel > 0; sbel--) {
			continue;
		}
	}
	if (false == false) {
		int qcuijih;
		for (qcuijih = 25; qcuijih > 0; qcuijih--) {
			continue;
		}
	}
	if (false != false) {
		int zsfjpgostz;
		for (zsfjpgostz = 50; zsfjpgostz > 0; zsfjpgostz--) {
			continue;
		}
	}
	if (7123 == 7123) {
		int gotmyvn;
		for (gotmyvn = 98; gotmyvn > 0; gotmyvn--) {
			continue;
		}
	}
	if (6438 == 6438) {
		int ksh;
		for (ksh = 12; ksh > 0; ksh--) {
			continue;
		}
	}
	return string("pn");
}

int txufbjl::bzvnlxkyswojmnylqjjuegbpo(int mdzihbflf, string xrpotvhkafujpoq, int kyuqv) {
	bool cutaftfssg = false;
	bool uroclxvig = true;
	int clijfj = 2133;
	int snydmt = 1221;
	string xgctbscmewr = "snguovbwf";
	string ebpsdongrp = "woptrttntknstwzvygzkcptyhvabvsjtfrjbzldypuffcgorivgodvdtoiwbedbrykmmjdpqlnndzrhaehdehgakoxurrgkaamut";
	double djnjqqwendpwret = 15109;
	return 54236;
}

string txufbjl::kwnkkxzknmhvitrjge(string hqrjdqvbllw, string ftpzbkesb, int kfapurgijramsz, int tcskfghzm, string azuqxryfpmfp, string ykzdirbrmen, bool xwznypehxnax, int yiogyatwxym) {
	bool dapjqy = false;
	int bxajaunvaskojq = 2389;
	string hcuyydadwpx = "wgnbsrufhbzwovnxmlrpcsxtqgjppqalatpfetnehdtnsosxkimtrtgverztakyrrziynlmoic";
	bool cqsbcneqn = false;
	bool kkzfmaldu = false;
	bool kmllzd = true;
	if (string("wgnbsrufhbzwovnxmlrpcsxtqgjppqalatpfetnehdtnsosxkimtrtgverztakyrrziynlmoic") != string("wgnbsrufhbzwovnxmlrpcsxtqgjppqalatpfetnehdtnsosxkimtrtgverztakyrrziynlmoic")) {
		int kuntugxpz;
		for (kuntugxpz = 75; kuntugxpz > 0; kuntugxpz--) {
			continue;
		}
	}
	if (true != true) {
		int nusesf;
		for (nusesf = 34; nusesf > 0; nusesf--) {
			continue;
		}
	}
	return string("nqlv");
}

bool txufbjl::eopslajfiwduddqlh(double hkwqje, int yqsporewt, string mjhuhk, int myukicfx, bool olieawtqmdjqd, double xarnnzisulwom) {
	int negii = 4399;
	bool cipnl = true;
	bool rkchmqpdc = true;
	double hvkndhm = 20352;
	bool wvkmcwicekyzdqw = false;
	bool cfnfxvj = true;
	double podwgom = 13055;
	double xqhqriwqdqro = 49689;
	int xzjzmws = 2255;
	double vpehoopvqsdcuo = 303;
	return false;
}

void txufbjl::vdclqoydinoszwhtotlmw(int avoknxcfmbh, double ulcxd, int xerbcvddiuqdskk, string pmcdq, double lzmkdqhbz, bool utjapyvjinxqtnl, double rlrvheocujg, bool diueowjc, string dpfoybvnpwha) {

}

bool txufbjl::xjqfdrmexsqani(double sygzlzot, int vrytsbbjqlaakpr, int mujtwuxls, int iibqbzeeuxyp, double oxmitwaqcno) {
	bool uvyfylcbih = false;
	if (false != false) {
		int bse;
		for (bse = 3; bse > 0; bse--) {
			continue;
		}
	}
	if (false != false) {
		int zlkugkzf;
		for (zlkugkzf = 61; zlkugkzf > 0; zlkugkzf--) {
			continue;
		}
	}
	if (false != false) {
		int mbeq;
		for (mbeq = 35; mbeq > 0; mbeq--) {
			continue;
		}
	}
	if (false != false) {
		int bslbovikrk;
		for (bslbovikrk = 27; bslbovikrk > 0; bslbovikrk--) {
			continue;
		}
	}
	return false;
}

string txufbjl::vpfjssskwbdkqjksci(string bxwcpldpy, double cvrrahdszdjku, double rvkfiinifeqayl, int eysrzqezuuoq, double ruwhadcckynfrc, int bslztzn, bool kbxfvhe, double jpluvqhudh) {
	string ezyctnajtsbuas = "wzcqfrxwvmfrpddcs";
	double jpaut = 53492;
	int pvnzsluoeez = 3425;
	bool wgfbabvcsnvmyv = true;
	if (string("wzcqfrxwvmfrpddcs") == string("wzcqfrxwvmfrpddcs")) {
		int jyrsw;
		for (jyrsw = 18; jyrsw > 0; jyrsw--) {
			continue;
		}
	}
	return string("");
}

int txufbjl::dxcydmqctawt(bool pulmrqdnnxrs, string jtalm, int yxfdjcz, bool foctprsvc, string snouwx, bool sevwehszeabt, string trqgafehkix, double mbzfsqpzlglufff) {
	return 47906;
}

void txufbjl::icbpyqhzrun(int xhdei, string ujomfshwfhat, int oganeki, double hlqmmnt) {
	bool hphuiayaec = false;
	bool kkkre = true;
	double rupwpfylnphfhq = 19719;
	double mkcdmvboakyggln = 22536;
	if (19719 != 19719) {
		int xnve;
		for (xnve = 15; xnve > 0; xnve--) {
			continue;
		}
	}
	if (false == false) {
		int rlbb;
		for (rlbb = 75; rlbb > 0; rlbb--) {
			continue;
		}
	}
	if (false == false) {
		int yki;
		for (yki = 42; yki > 0; yki--) {
			continue;
		}
	}

}

bool txufbjl::mskfhkgoyepuleplspsniegc(double zbpkiebiog) {
	double szfxixpgbhllug = 11856;
	int nvfleatvhxjor = 3331;
	string yjhts = "gqrsirkklcsnqbbntoddndehmyjyxtoiogjllzptutdgrcoqpnrdutppdrnsla";
	int yitvrnbfzcauc = 2421;
	if (11856 != 11856) {
		int tmzhlxtwr;
		for (tmzhlxtwr = 75; tmzhlxtwr > 0; tmzhlxtwr--) {
			continue;
		}
	}
	if (2421 == 2421) {
		int rnio;
		for (rnio = 28; rnio > 0; rnio--) {
			continue;
		}
	}
	if (3331 != 3331) {
		int il;
		for (il = 35; il > 0; il--) {
			continue;
		}
	}
	if (11856 != 11856) {
		int uyjnpim;
		for (uyjnpim = 30; uyjnpim > 0; uyjnpim--) {
			continue;
		}
	}
	return true;
}

double txufbjl::qbrxhkuvwsdzbvyptte(bool viiqgepwjol, string biehomqfpencyyn, int iqffhbueaxfo, bool najyt, double afovxsrjeofjz, int kpmkporchwc, int cjcrzq, double zyrnkpme, int cgkcsiy) {
	bool htwbdnkapgirhvo = false;
	return 75324;
}

string txufbjl::slzipdaegrjv(bool hxnskwfdqfd, bool dwqpaptrlwsbe, double nuhwblonhjfqr, double bflhcy) {
	return string("kipabwxalslmykfssow");
}

int txufbjl::drohtaonoq(bool simhsoemxuqo, int obsrnpa, int qugezh, bool gmhnlvtulay, double sppesbtenef, double mrfdbsc, int qvjpgvrs, string helqbtfe, int wqbnbcqbdqbzzi) {
	double acanaeyfqakrl = 27035;
	int fyxufvbyrt = 8416;
	int zciymnoybhhnu = 5202;
	double wqgcgkio = 54039;
	return 88848;
}

int txufbjl::rcjhjejujvxf(bool zwuqhufmj, double uwgxpctikw) {
	bool xcbaecusiqdhp = false;
	string itprurfzr = "brpjlljeuycubqefbrwouxfaaevewfmzzlavnthsgxglpmzikdackllwtiuzqvmnjoswmvkdoyuwjnof";
	double dnfnhcjgnv = 330;
	double rubxkfuuoait = 25940;
	string ktlgdooofndpv = "nldacvqaqkiygzqzyjawzfufvpofvfefsefyeepvjcwexlmbftuxbo";
	bool natieb = true;
	string bhinlgxmyso = "zbbnghmybvuhqnyraxxvugajtgljsreivnrkmkuzehnkhtguoafbtvbqmtfcfgfdbbekrxlorferayqnlmzwzvyipup";
	if (false != false) {
		int ivfbppiwj;
		for (ivfbppiwj = 20; ivfbppiwj > 0; ivfbppiwj--) {
			continue;
		}
	}
	if (false == false) {
		int prqivmk;
		for (prqivmk = 70; prqivmk > 0; prqivmk--) {
			continue;
		}
	}
	if (string("brpjlljeuycubqefbrwouxfaaevewfmzzlavnthsgxglpmzikdackllwtiuzqvmnjoswmvkdoyuwjnof") == string("brpjlljeuycubqefbrwouxfaaevewfmzzlavnthsgxglpmzikdackllwtiuzqvmnjoswmvkdoyuwjnof")) {
		int ruyszqrhy;
		for (ruyszqrhy = 23; ruyszqrhy > 0; ruyszqrhy--) {
			continue;
		}
	}
	if (false != false) {
		int qubnga;
		for (qubnga = 51; qubnga > 0; qubnga--) {
			continue;
		}
	}
	if (true != true) {
		int flrrrexj;
		for (flrrrexj = 5; flrrrexj > 0; flrrrexj--) {
			continue;
		}
	}
	return 44974;
}

double txufbjl::ellgoxfxgldbsdztgov(double mkzjpaayakb, string mavrpbdjkncdtyq, bool osmljcjvnzopf, string lztabtfmobs, double zebdxp, bool yvvnrkzufzbr, int hqsydw) {
	int paklfhgowwm = 2322;
	double lfbjvhyyotbasp = 21140;
	bool zcmsvbx = false;
	int rvtrhndnxxdpd = 4896;
	double hhapudqubtnmla = 30398;
	string uzbdsgap = "ztsrfaqdbklkbzgugyobytftmxwseofddgtbiofqvyadvlkafemwsyqjqdywcbttbgui";
	double okboea = 25441;
	double dnoikhbx = 74566;
	if (30398 == 30398) {
		int duodf;
		for (duodf = 58; duodf > 0; duodf--) {
			continue;
		}
	}
	if (25441 != 25441) {
		int pe;
		for (pe = 42; pe > 0; pe--) {
			continue;
		}
	}
	if (2322 == 2322) {
		int bcscqrlj;
		for (bcscqrlj = 57; bcscqrlj > 0; bcscqrlj--) {
			continue;
		}
	}
	if (74566 == 74566) {
		int wcoy;
		for (wcoy = 51; wcoy > 0; wcoy--) {
			continue;
		}
	}
	return 92392;
}

txufbjl::txufbjl() {
	this->icbpyqhzrun(4105, string("eooflareoticlijyntdjyjchmxpgygsomdj"), 1834, 11128);
	this->mskfhkgoyepuleplspsniegc(26560);
	this->qbrxhkuvwsdzbvyptte(false, string("affvuaujyrnfnhtkxndfujfhpllpnndldci"), 1240, true, 6485, 728, 1552, 722, 2122);
	this->slzipdaegrjv(false, false, 7426, 67594);
	this->drohtaonoq(false, 1983, 863, true, 32094, 35397, 2802, string("flibkgvfrxdoapdyiegsqzbbkhocrtzrrshypdazfjanctzrrlpfqecloulhgkxybzujbtutnjhgxkefzarvvphpzjcwajh"), 2125);
	this->rcjhjejujvxf(false, 1895);
	this->ellgoxfxgldbsdztgov(49729, string("tulrhlkewllhpyrvtcqdtrqorlhfcwvfamzdfwewhzvcrykwga"), true, string("eebsvwwgnoedhcujlwoqzwocumfxwtzovuelanwgg"), 20188, true, 3544);
	this->bzvnlxkyswojmnylqjjuegbpo(2042, string("jiezsgdvidwfddtmmkyohmarsqmtafwzuvnlafaejfuixfis"), 1323);
	this->kwnkkxzknmhvitrjge(string("tfvdeudrqeorldfxebekpyoqkozurvgqpmngqaak"), string("fccgolvilupdcfqfmmxxenvnneavmlttli"), 5844, 1412, string("nxhtbijobrfoqnsmgxvobmbmvbsnuy"), string("dihvjwduaaxpyijiqrrhbofeddhjbocvxjmaqoievvzsejyecedcqrgodskuikzthfkrnbshbuzqgxyebbstiujiww"), true, 3177);
	this->eopslajfiwduddqlh(28696, 4342, string("fxmftdaznokpqwhskqqgbjdlsbkazvnrcifoawwkwmvqmvefhrumhpnldyutzrzjidnnhvfva"), 5653, true, 11388);
	this->vdclqoydinoszwhtotlmw(1791, 9314, 3788, string("eybituuezakewucwildddxcabqbxyhjnjtwzejgtdnrhvpyvxnksyuzqciisgfklkiqavavbtspqcanjpbrumytobucpe"), 35507, false, 9485, false, string("zdfbennhzprvsrtmgsgjtxungptqfgss"));
	this->xjqfdrmexsqani(36120, 2683, 4735, 1508, 27295);
	this->vpfjssskwbdkqjksci(string("wyfswhcb"), 6596, 2555, 3724, 47888, 3357, true, 39964);
	this->dxcydmqctawt(true, string("avpqfjrlachsxjjxzicismezovvlnj"), 6810, true, string("k"), false, string("wktklezsijjozhxeqooyltqyuahksaxebqoaeyofgpwnalhvbyheqptpizsjtrxwpgr"), 16052);
	this->eqpfhqznplitciprd(true);
	this->xjqchrandjge(false, 4213, string("dwivnwktpsdujzwvzlnkwmpnacswqbpspmoqarpkupgctoceggxnlnztffshdhtdvnjwwjmxmppkvrjevzsmew"), string("rsfgnxikbbadpydezplekcbsobmruytsjqcfobvgahtstoqrapnpzifkiyvxgtykcdpvumyzgluuqnpj"), 50182, 39710);
	this->nleloodlihmgroghntlgk(false, false, string("zkpnjjyudkspczoxktwbrkdtzdceollhbxrkcbeq"), true, true, 7234, string("sblvydnqqkbwgqevvnfabmwxuqhoekvfyqaxhoskpjgljkks"), 5283);
	this->eqorcddrkqckuz(string("nrbicmaakhhxpdoofnuk"), true, 205, 30479, string("cbupzhazvgzhlhidspvpthtntpmgalkulw"), 780, 668);
	this->weduwjcpvmdhbfjgaioliguhw(string("xpyxoczbyxcegzmkpnmunqdcvpwlxuwvsppngcbspwvjxykhykxpygeqhindjhimcrjpqnitfqvtopmrptrgnfxhvblgxbryvrze"), false, 14186, string("vcviuruqqqlmlnpqpylhsbldhwuksgnoexivlunbqbcsunkfmfmcfwogbwseopfgljbmznoxzesttrxontzumb"), 7461, string("gkknzosbphhlrovtdoqnaurdlqarsvwfuzlbrmttwi"), false, 21550, 20250, 8224);
	this->wznjylaemtwbdaakfrf(string("ypqeloyuugxpfgydcubstoanjzaqzatmxigpzrxuuvauflqornmvvs"), 5942, string("noiovokvyxqhkvdxtkxbosnuagqvbfemzitflczttgvfkwgbbjcvecnyrxibc"));
	this->mnsgbbrgbhjah(true, 10351, 59027, 2016);
	this->ydofslyyuvivzaw(false, 36413, 5302, true, string("zreklkmbtrircztillmahjkijzmmyuprnqjsyawvavu"), 1921, string("htzrwualmrgybtqqbxzmvrnhywcrsfpqmejuyiozswdmsoqtwaylpxcuxzxsmngozlj"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class afwxoup {
public:
	string ryvntt;
	int zrxgwggdwud;
	double cgphay;
	afwxoup();
	string gmmfpsfmhwimkpccudnt(double ppbiyjfpaxmwv, double dgzildouu, double ilhqwkai, string ttjdfrbdvgofb);
	double kjvwlknfnihpziufyivwzkqo(bool qsvmapxazbruh, double gkgvw, int lqcyrzro, string ydfpcunsnxnmnza, double ehdlmqpay, double nnsioenej, int yumvwaikmzvlevz, string jeuyelnzdhjuqu);
	string bvwtdkvods(bool qlgtgew, double crkjcnyjuudqmd, double wjyshlybaijfc, string fymqpkymlkgrohn, int maoqvaxrhhul, int mfhgzuhhmfsvkbk, int badcb, double vhjphzz, string fvveuxoefzjhccs);
	bool axkjzkfxnyclydzieyagjow(string bvpxqsidwlnsmv, double dcvuuaqcuxouf, bool jbvtnzvvnnmhg, bool cuqkxvtpzadclr, double tmgfoizqxiq);
	double praxzcdbmimhntlj(double gtafsnaqyqcizm, bool skobkjlgkx, string puhiirvhz);
	string warjtckddcbmag(int gojkrlcqaxnm, double rnpdistjdqjlhxe, int hgxurua, bool tkbffezmyalwira, int koiyvx, bool orusqva, bool ubuovnpf);

protected:
	bool vzottl;

	int tjjdqjegzomnlchpdpkz(double wdcubgx, string zsshumaqcqnx, bool jvdsrbcutuob, bool iylrfcqahxqlzkj, string tasulvknde, int mzqqu, bool dbonsii);
	double cqelabkfkjfxwrqhv(bool fzhfdumfdjcyw);
	int xudivzkxjpxhlthuryjtej(double ujvosnwhhwvu, bool ociwbyce, double lazahssryebgct, bool ixdrm, bool ildtmswngrwng, int ihdktgoom, double wqyjzxqsxaj);
	bool tvhpssnqotgoqa();
	string wnodmynvquq(int iyrmccfpwyupjc, double cezrj, bool moqbojsdhvqi, double eamcaohzhl, bool qybevomzwr, int pqltkfvmtxlbbxf);
	string hiezgoilbqazxpjqqlt(bool zyhdj, string aekadwbzbqr, bool aggnncgzdep, bool gavpuqa, string jsvveeyy, int jwfzfgcfos, double ztausmummdefx, double papzchhn, double mialodaimcb);
	double oqnclstwetoezciwnvxowutr(bool gcxkvfb, double zftrxtmvbeovpn, int hsbzykjkxlw, int qshgtdsfagfxwa, bool ukwmmghfmvdeo, string ybpgyrb);
	string czdvfmyllgtxgctbwsvkybo(string msnfufwiprddyl, int zsnuhkpxk, int houpwoss, double rakexu, bool tnnzborwqcizl, string qvovu, double xodbkfiaxlhium, bool mlvfljtlhwaovf, int kgqrvlq, double rjdbqmjefeftv);
	void dnaxsjrothpaoivinocggg(double qkratymvjfsf, string sjjdqkj, bool decvft, string zhlnoyp);
	string omycibqncorvyltdhwjc(double keubbcgm, bool ouuvvdvoqhlg, int wjeaiup, int lymesit);

private:
	double dfdhwsimyqn;

	int qzvfpkziggsanciw(int mrwzuswqihf, int ljgdcszsa, bool nqofhytuzsxfpfu, double spikkxw, bool moxjyzwk, string ztivuupk, int jekzi, string nwqyscqx, bool pjavfkmdf, string gaqcpgfcbuwigs);

};


int afwxoup::qzvfpkziggsanciw(int mrwzuswqihf, int ljgdcszsa, bool nqofhytuzsxfpfu, double spikkxw, bool moxjyzwk, string ztivuupk, int jekzi, string nwqyscqx, bool pjavfkmdf, string gaqcpgfcbuwigs) {
	double xvdhbs = 19068;
	double opfohksujl = 65365;
	int kvcvqyuhakehytt = 1172;
	string vdbrkka = "vnzfyd";
	bool uniwzr = false;
	int qjknpnn = 2167;
	int mdzpuqprgfeurm = 214;
	double xagulhgwk = 32340;
	bool vfwaazjczopmswr = false;
	string prcxboeprxmy = "hyudhdksjnanybixrhgkpwwcvrmwqaeuz";
	if (string("vnzfyd") != string("vnzfyd")) {
		int af;
		for (af = 44; af > 0; af--) {
			continue;
		}
	}
	if (214 == 214) {
		int gfuz;
		for (gfuz = 86; gfuz > 0; gfuz--) {
			continue;
		}
	}
	if (32340 != 32340) {
		int qcdqilea;
		for (qcdqilea = 4; qcdqilea > 0; qcdqilea--) {
			continue;
		}
	}
	if (2167 != 2167) {
		int hn;
		for (hn = 54; hn > 0; hn--) {
			continue;
		}
	}
	return 71430;
}

int afwxoup::tjjdqjegzomnlchpdpkz(double wdcubgx, string zsshumaqcqnx, bool jvdsrbcutuob, bool iylrfcqahxqlzkj, string tasulvknde, int mzqqu, bool dbonsii) {
	bool eamea = true;
	double nrewrxcvftvxa = 5794;
	if (5794 != 5794) {
		int ezq;
		for (ezq = 61; ezq > 0; ezq--) {
			continue;
		}
	}
	return 25019;
}

double afwxoup::cqelabkfkjfxwrqhv(bool fzhfdumfdjcyw) {
	int vlufhjexwedau = 1448;
	double ihpmssvtvox = 13992;
	double scjnv = 26531;
	if (26531 != 26531) {
		int kpfuw;
		for (kpfuw = 66; kpfuw > 0; kpfuw--) {
			continue;
		}
	}
	if (26531 != 26531) {
		int xeir;
		for (xeir = 45; xeir > 0; xeir--) {
			continue;
		}
	}
	if (26531 != 26531) {
		int kf;
		for (kf = 46; kf > 0; kf--) {
			continue;
		}
	}
	if (13992 != 13992) {
		int kh;
		for (kh = 67; kh > 0; kh--) {
			continue;
		}
	}
	if (26531 != 26531) {
		int mihwbvesy;
		for (mihwbvesy = 5; mihwbvesy > 0; mihwbvesy--) {
			continue;
		}
	}
	return 4356;
}

int afwxoup::xudivzkxjpxhlthuryjtej(double ujvosnwhhwvu, bool ociwbyce, double lazahssryebgct, bool ixdrm, bool ildtmswngrwng, int ihdktgoom, double wqyjzxqsxaj) {
	bool qdtempvemqr = false;
	string xwxqzsopyytowa = "erznttptdsiccxu";
	int ismwoz = 83;
	double zefxcpvruacrg = 66936;
	double ssehwonab = 4816;
	int jhqemzon = 2046;
	string yhldnnjlkkknn = "dfzot";
	int eatheevt = 4134;
	int mgusxfb = 2280;
	if (string("erznttptdsiccxu") == string("erznttptdsiccxu")) {
		int ahsgpgjn;
		for (ahsgpgjn = 98; ahsgpgjn > 0; ahsgpgjn--) {
			continue;
		}
	}
	if (4134 == 4134) {
		int hg;
		for (hg = 68; hg > 0; hg--) {
			continue;
		}
	}
	if (string("erznttptdsiccxu") != string("erznttptdsiccxu")) {
		int sym;
		for (sym = 12; sym > 0; sym--) {
			continue;
		}
	}
	if (string("dfzot") != string("dfzot")) {
		int ei;
		for (ei = 31; ei > 0; ei--) {
			continue;
		}
	}
	return 76087;
}

bool afwxoup::tvhpssnqotgoqa() {
	int fdktofrxpnl = 6434;
	bool yvtzlvwwk = false;
	bool rjsjrndbhgxml = false;
	string hcduqpyvrf = "dpiigcpabizfdguuzoiinykcwexigoyogejmgzlewbprrcxehkkvesdlptvdsaomlfqleoxeb";
	int dvrxm = 2009;
	string ivcvgfefkpyw = "rnuojujwfqifqudrizwgfabnqncnldjpaohbgrqoyrhwlvmteyz";
	double gwvnr = 45137;
	if (string("rnuojujwfqifqudrizwgfabnqncnldjpaohbgrqoyrhwlvmteyz") == string("rnuojujwfqifqudrizwgfabnqncnldjpaohbgrqoyrhwlvmteyz")) {
		int lsnaz;
		for (lsnaz = 100; lsnaz > 0; lsnaz--) {
			continue;
		}
	}
	if (false != false) {
		int shpukbb;
		for (shpukbb = 48; shpukbb > 0; shpukbb--) {
			continue;
		}
	}
	if (string("dpiigcpabizfdguuzoiinykcwexigoyogejmgzlewbprrcxehkkvesdlptvdsaomlfqleoxeb") != string("dpiigcpabizfdguuzoiinykcwexigoyogejmgzlewbprrcxehkkvesdlptvdsaomlfqleoxeb")) {
		int ufaxjzl;
		for (ufaxjzl = 37; ufaxjzl > 0; ufaxjzl--) {
			continue;
		}
	}
	if (6434 != 6434) {
		int msawmug;
		for (msawmug = 24; msawmug > 0; msawmug--) {
			continue;
		}
	}
	return true;
}

string afwxoup::wnodmynvquq(int iyrmccfpwyupjc, double cezrj, bool moqbojsdhvqi, double eamcaohzhl, bool qybevomzwr, int pqltkfvmtxlbbxf) {
	int sdptbtzb = 9039;
	double vfcswkxdnizdb = 11548;
	string vbnlwmghssqspvi = "klyocqzqvfzbfhlotxaxcxuvdovtvtjmxqpcetjiuzc";
	double eoknluhzixfo = 15729;
	double eddfluobnrbv = 2805;
	bool dxawdwmf = false;
	int wxovcvkcnd = 5494;
	int kfskyzbalc = 537;
	string ytrvij = "gsyexfmvfqjxewgzhpqzwgnzxeqzejpiuxgdxwhb";
	bool emxfmdclowmoi = true;
	if (537 == 537) {
		int hgyumgbps;
		for (hgyumgbps = 81; hgyumgbps > 0; hgyumgbps--) {
			continue;
		}
	}
	if (true == true) {
		int scd;
		for (scd = 73; scd > 0; scd--) {
			continue;
		}
	}
	if (15729 == 15729) {
		int mbofti;
		for (mbofti = 87; mbofti > 0; mbofti--) {
			continue;
		}
	}
	if (string("gsyexfmvfqjxewgzhpqzwgnzxeqzejpiuxgdxwhb") == string("gsyexfmvfqjxewgzhpqzwgnzxeqzejpiuxgdxwhb")) {
		int rs;
		for (rs = 98; rs > 0; rs--) {
			continue;
		}
	}
	if (string("klyocqzqvfzbfhlotxaxcxuvdovtvtjmxqpcetjiuzc") == string("klyocqzqvfzbfhlotxaxcxuvdovtvtjmxqpcetjiuzc")) {
		int nmaom;
		for (nmaom = 57; nmaom > 0; nmaom--) {
			continue;
		}
	}
	return string("pbqfnbfilour");
}

string afwxoup::hiezgoilbqazxpjqqlt(bool zyhdj, string aekadwbzbqr, bool aggnncgzdep, bool gavpuqa, string jsvveeyy, int jwfzfgcfos, double ztausmummdefx, double papzchhn, double mialodaimcb) {
	string lrfbawvwancfueo = "gspogagkvmjznbipswqdixszxcbkqywustfeupcuhn";
	double rjvad = 1431;
	double vauresacuxwva = 65839;
	bool vfppnpqnybxp = true;
	int wbgnyz = 2577;
	double hdeeakrxyrz = 9073;
	bool nqpiwpgndslsyom = false;
	bool vaturfizboyrbpe = false;
	if (false != false) {
		int rprju;
		for (rprju = 28; rprju > 0; rprju--) {
			continue;
		}
	}
	if (true == true) {
		int oysezzbxgf;
		for (oysezzbxgf = 72; oysezzbxgf > 0; oysezzbxgf--) {
			continue;
		}
	}
	return string("hztnj");
}

double afwxoup::oqnclstwetoezciwnvxowutr(bool gcxkvfb, double zftrxtmvbeovpn, int hsbzykjkxlw, int qshgtdsfagfxwa, bool ukwmmghfmvdeo, string ybpgyrb) {
	string lsyozchijxuzkr = "zsxzrovwbyhogwetqqfhvmmezficxkninelkfrzjnjbcnwfvslackmltypfamfbeqvzwubhcwszvzgjpof";
	bool xpdocdcskzrzy = true;
	string bentgwrs = "luzmqeyvohzrblqwkqkabkixlwhwmatwjwgdtzviqqoirdhxdg";
	if (string("zsxzrovwbyhogwetqqfhvmmezficxkninelkfrzjnjbcnwfvslackmltypfamfbeqvzwubhcwszvzgjpof") != string("zsxzrovwbyhogwetqqfhvmmezficxkninelkfrzjnjbcnwfvslackmltypfamfbeqvzwubhcwszvzgjpof")) {
		int sroxopl;
		for (sroxopl = 18; sroxopl > 0; sroxopl--) {
			continue;
		}
	}
	if (string("zsxzrovwbyhogwetqqfhvmmezficxkninelkfrzjnjbcnwfvslackmltypfamfbeqvzwubhcwszvzgjpof") == string("zsxzrovwbyhogwetqqfhvmmezficxkninelkfrzjnjbcnwfvslackmltypfamfbeqvzwubhcwszvzgjpof")) {
		int lihsfn;
		for (lihsfn = 19; lihsfn > 0; lihsfn--) {
			continue;
		}
	}
	if (string("zsxzrovwbyhogwetqqfhvmmezficxkninelkfrzjnjbcnwfvslackmltypfamfbeqvzwubhcwszvzgjpof") != string("zsxzrovwbyhogwetqqfhvmmezficxkninelkfrzjnjbcnwfvslackmltypfamfbeqvzwubhcwszvzgjpof")) {
		int idshvjeauf;
		for (idshvjeauf = 79; idshvjeauf > 0; idshvjeauf--) {
			continue;
		}
	}
	return 23993;
}

string afwxoup::czdvfmyllgtxgctbwsvkybo(string msnfufwiprddyl, int zsnuhkpxk, int houpwoss, double rakexu, bool tnnzborwqcizl, string qvovu, double xodbkfiaxlhium, bool mlvfljtlhwaovf, int kgqrvlq, double rjdbqmjefeftv) {
	string aeleu = "bwbllagaydgfvbrmwwkgjtlkjmimrfvfgykmysecpnmfdxgjlzwdtaxoiclaaktleaoahfoeqcl";
	bool kgphpqn = false;
	int buxlh = 191;
	bool rvqmodwrf = true;
	int veazjcstur = 7009;
	string yqgyzenbjmqurg = "pgfdpnzymlbfahrybhsucrkvkzrjdbrffalibclkryoyahjvligmzhpovprf";
	int jlqynxou = 2366;
	string hmxwxmskam = "wyrnbgzbtvcitxktxvaidytdfbczifzbcphjkqnhgofjcvocya";
	bool anijkeycd = true;
	double wrwleaqkswo = 39317;
	return string("jw");
}

void afwxoup::dnaxsjrothpaoivinocggg(double qkratymvjfsf, string sjjdqkj, bool decvft, string zhlnoyp) {
	double lkituhzuyiw = 85308;
	string bcxasxg = "ysiesfqmruahgxnxtexedqbv";
	int wwzlssgaulvkn = 4306;
	int ejrbsufptzxl = 1249;
	string tlxxzemqkcdn = "pjmlxclllfyqajsfyvuexooqcaxfftltylsnzshsmgmcmhhaqrxciscbfouojgrlbjfortrmdaarbfqsjg";
	double eamxfdbhclf = 44674;
	bool mftgtdhid = false;
	double ykjzrkab = 23279;
	double hqwvggnoqqxrt = 25740;
	int jpgltsvlrjt = 2198;
	if (false != false) {
		int kvx;
		for (kvx = 97; kvx > 0; kvx--) {
			continue;
		}
	}

}

string afwxoup::omycibqncorvyltdhwjc(double keubbcgm, bool ouuvvdvoqhlg, int wjeaiup, int lymesit) {
	double akluwvcdbbbozx = 14565;
	int aimukcjdhsuta = 2334;
	int ihsdaixeizjq = 5419;
	string dyzftkttiwwugj = "yyqnbvgadznmgjbrzlwdvxhvvwaophswwkrwfcnjirsmpcglkgytuscbbagcxqkjijejfmmhiwwjpbggygwcdboiapdntlhmqv";
	double hfmsenkpiiqvoto = 1418;
	if (14565 != 14565) {
		int uo;
		for (uo = 75; uo > 0; uo--) {
			continue;
		}
	}
	if (5419 == 5419) {
		int lsldovfnrp;
		for (lsldovfnrp = 35; lsldovfnrp > 0; lsldovfnrp--) {
			continue;
		}
	}
	return string("wlcsgbaz");
}

string afwxoup::gmmfpsfmhwimkpccudnt(double ppbiyjfpaxmwv, double dgzildouu, double ilhqwkai, string ttjdfrbdvgofb) {
	string iuqmwcaybi = "hfdvprcrzxlwszzeqnezhldapkygugxjpmbfbebcjukypnouxulrkqqttrzkjuawkfarokhdcsfdugpmumghygrjvwvbycbe";
	int lptwmfbrc = 6295;
	int ggvndctjnwp = 1344;
	int snyuugiq = 6156;
	int gjbzduf = 8678;
	double gdqamzalcs = 4148;
	if (string("hfdvprcrzxlwszzeqnezhldapkygugxjpmbfbebcjukypnouxulrkqqttrzkjuawkfarokhdcsfdugpmumghygrjvwvbycbe") == string("hfdvprcrzxlwszzeqnezhldapkygugxjpmbfbebcjukypnouxulrkqqttrzkjuawkfarokhdcsfdugpmumghygrjvwvbycbe")) {
		int nyugmbs;
		for (nyugmbs = 97; nyugmbs > 0; nyugmbs--) {
			continue;
		}
	}
	if (6156 == 6156) {
		int qxfy;
		for (qxfy = 61; qxfy > 0; qxfy--) {
			continue;
		}
	}
	if (4148 != 4148) {
		int rvpple;
		for (rvpple = 68; rvpple > 0; rvpple--) {
			continue;
		}
	}
	return string("trnjxdvpqxaglh");
}

double afwxoup::kjvwlknfnihpziufyivwzkqo(bool qsvmapxazbruh, double gkgvw, int lqcyrzro, string ydfpcunsnxnmnza, double ehdlmqpay, double nnsioenej, int yumvwaikmzvlevz, string jeuyelnzdhjuqu) {
	double nnhbpclxfjmgcv = 29562;
	string mycwmedfinams = "gfb";
	bool gxwpxk = true;
	int aneecarxxh = 2404;
	int jecszby = 891;
	bool zstknpttu = false;
	int hwkgamtkp = 450;
	if (450 != 450) {
		int oxosfwpmea;
		for (oxosfwpmea = 12; oxosfwpmea > 0; oxosfwpmea--) {
			continue;
		}
	}
	if (450 == 450) {
		int bzvquy;
		for (bzvquy = 99; bzvquy > 0; bzvquy--) {
			continue;
		}
	}
	if (string("gfb") == string("gfb")) {
		int vjlpcestr;
		for (vjlpcestr = 32; vjlpcestr > 0; vjlpcestr--) {
			continue;
		}
	}
	if (891 == 891) {
		int rhuf;
		for (rhuf = 98; rhuf > 0; rhuf--) {
			continue;
		}
	}
	return 40766;
}

string afwxoup::bvwtdkvods(bool qlgtgew, double crkjcnyjuudqmd, double wjyshlybaijfc, string fymqpkymlkgrohn, int maoqvaxrhhul, int mfhgzuhhmfsvkbk, int badcb, double vhjphzz, string fvveuxoefzjhccs) {
	int jpzzg = 3874;
	bool quxntup = false;
	int tecewlrkxotvkeb = 4467;
	double lvdutlw = 15117;
	bool jlvtklugmtjysro = true;
	double gudwap = 15125;
	string gmbvkhzrndlik = "xbgfehphlljuneinnkhjisckocpbzjcnmxbdvakbuwgqkwskdoqrepkfsdreyjknzdqvyhfpw";
	string wnpinuq = "mwcryru";
	if (15117 == 15117) {
		int mihjdv;
		for (mihjdv = 87; mihjdv > 0; mihjdv--) {
			continue;
		}
	}
	if (string("xbgfehphlljuneinnkhjisckocpbzjcnmxbdvakbuwgqkwskdoqrepkfsdreyjknzdqvyhfpw") == string("xbgfehphlljuneinnkhjisckocpbzjcnmxbdvakbuwgqkwskdoqrepkfsdreyjknzdqvyhfpw")) {
		int kznd;
		for (kznd = 32; kznd > 0; kznd--) {
			continue;
		}
	}
	return string("ehgfxvhpkeutkffgrxog");
}

bool afwxoup::axkjzkfxnyclydzieyagjow(string bvpxqsidwlnsmv, double dcvuuaqcuxouf, bool jbvtnzvvnnmhg, bool cuqkxvtpzadclr, double tmgfoizqxiq) {
	double mrktz = 72514;
	double smewzatphczrq = 26854;
	bool tbjubb = false;
	string kvtayahlfk = "";
	bool eyevsthdjzvcas = true;
	string fvieizwxmsye = "hadmscpurhpvehemutuoha";
	int zpwxcttxhclz = 465;
	string hkezlsntfolwub = "ktdnarcomyjlndvrfsejefrniaiqttazufebmqgvdfzhtjyiezynayjlucjcfmdpxddaldbsnvjogftotrbxskjxsjnudfgidvxp";
	if (string("ktdnarcomyjlndvrfsejefrniaiqttazufebmqgvdfzhtjyiezynayjlucjcfmdpxddaldbsnvjogftotrbxskjxsjnudfgidvxp") != string("ktdnarcomyjlndvrfsejefrniaiqttazufebmqgvdfzhtjyiezynayjlucjcfmdpxddaldbsnvjogftotrbxskjxsjnudfgidvxp")) {
		int uawfzfwgx;
		for (uawfzfwgx = 44; uawfzfwgx > 0; uawfzfwgx--) {
			continue;
		}
	}
	if (string("ktdnarcomyjlndvrfsejefrniaiqttazufebmqgvdfzhtjyiezynayjlucjcfmdpxddaldbsnvjogftotrbxskjxsjnudfgidvxp") != string("ktdnarcomyjlndvrfsejefrniaiqttazufebmqgvdfzhtjyiezynayjlucjcfmdpxddaldbsnvjogftotrbxskjxsjnudfgidvxp")) {
		int oitru;
		for (oitru = 84; oitru > 0; oitru--) {
			continue;
		}
	}
	if (465 != 465) {
		int hi;
		for (hi = 47; hi > 0; hi--) {
			continue;
		}
	}
	if (string("ktdnarcomyjlndvrfsejefrniaiqttazufebmqgvdfzhtjyiezynayjlucjcfmdpxddaldbsnvjogftotrbxskjxsjnudfgidvxp") == string("ktdnarcomyjlndvrfsejefrniaiqttazufebmqgvdfzhtjyiezynayjlucjcfmdpxddaldbsnvjogftotrbxskjxsjnudfgidvxp")) {
		int xzltihz;
		for (xzltihz = 24; xzltihz > 0; xzltihz--) {
			continue;
		}
	}
	return false;
}

double afwxoup::praxzcdbmimhntlj(double gtafsnaqyqcizm, bool skobkjlgkx, string puhiirvhz) {
	int jatiivqunggllrs = 1643;
	int djghvhzknbhz = 9783;
	double xelufg = 13321;
	double wydywob = 18596;
	return 3790;
}

string afwxoup::warjtckddcbmag(int gojkrlcqaxnm, double rnpdistjdqjlhxe, int hgxurua, bool tkbffezmyalwira, int koiyvx, bool orusqva, bool ubuovnpf) {
	string tljbq = "kmfzkueqfkjgbldfnxkybanqxyyuuenahiikdlbozvxjiuqbfku";
	double liofxyrlm = 66826;
	bool mtzuxl = false;
	return string("kqeazuyxdtume");
}

afwxoup::afwxoup() {
	this->gmmfpsfmhwimkpccudnt(6163, 28637, 22981, string("oijmwztcrnhpsrmzmhjjepbkgugwhgeavkmnjqhcjmlzygaljpldwqcuzdisolmabludxkiorlhae"));
	this->kjvwlknfnihpziufyivwzkqo(false, 15872, 865, string("jxhkcfbsbztpkjhwu"), 36940, 64342, 1300, string("ouvtrfaxmgrrwcsmytvpckhpnioyioclwgcttrpblbeqgfrktlibqksdximmmersdzsasdhvrtn"));
	this->bvwtdkvods(false, 51180, 49861, string("qfacfuplwslvhndbyeileluclndrkjgkwcltqrwwvtpuzylkwgookawtvhpzntmnlaphtzkkm"), 6507, 5827, 161, 2350, string("uzbqksqg"));
	this->axkjzkfxnyclydzieyagjow(string("fjryiqeewvleclxrzugknnhrppyyixbbavplvwcykfquerrjsvomapsgaoghwipyjnhohaplnaspqvjoktkmiubjitqsku"), 25812, true, false, 27449);
	this->praxzcdbmimhntlj(21796, true, string("pqlaclirhejhylastrdvpgzoonfkkdmmrhormmhtqbuqtpnvpngqslfgdvplcwbchff"));
	this->warjtckddcbmag(943, 29051, 1219, false, 2497, true, false);
	this->tjjdqjegzomnlchpdpkz(13445, string("rwxdblzfzysqddotxciasmrfnuxsqqpqctzmqlqqxgomjypxkjmeuirpbh"), true, false, string("kxnpoeldoivwsmxarwugsijombmpcqdwcafnlfiktznicrqjyhcxxhuhcbbap"), 2620, true);
	this->cqelabkfkjfxwrqhv(true);
	this->xudivzkxjpxhlthuryjtej(18913, true, 26361, false, true, 2768, 3645);
	this->tvhpssnqotgoqa();
	this->wnodmynvquq(3273, 57541, false, 13806, false, 5824);
	this->hiezgoilbqazxpjqqlt(false, string("scaqmtzdpmtjmwtpdikxyalpuslfdmtuhneswyqhgdmmqfxxhqolnwyoksoiovysdmjyotratiovaitmtrd"), false, false, string("zrbblrscofqftciknhtxkqhcnbhmxmsmbdqvas"), 6168, 16988, 28747, 2738);
	this->oqnclstwetoezciwnvxowutr(true, 49076, 2445, 7284, true, string("mrafmmphlsfmfrzwcyvnedgyantdzwssqzogasrhviwpyeuzsvpzeeiuszpzsxiznivwxbbtnqptfkbdehbbkf"));
	this->czdvfmyllgtxgctbwsvkybo(string("jorhrquahmtmwws"), 2325, 7163, 1772, false, string("blrqdauzswllaceiozczxsnoeujgjquxmftrfclktjqppjizpzxmbldwiwopvvwcdlivrsc"), 21219, false, 823, 73171);
	this->dnaxsjrothpaoivinocggg(19036, string("vyekpbpzsqdaloffgkpslelbcjvtxsyxzfyxlbjqslbdqgxdprcxfzdf"), false, string("xcaosgfakldnyrslhrewcuciooijvfjiofjdctoyomqyakjthhctsdrgxhdycgylwawbfqmdyubvgzazhxjyk"));
	this->omycibqncorvyltdhwjc(37477, false, 3565, 2134);
	this->qzvfpkziggsanciw(1695, 4031, false, 40635, false, string("qlesnjbzcmkxgvquwwdgqbnlaergavpowekhatzeudtqiybfeszwjqthpznkfxhqlwlomhcbjlsukqfxhnyrbrqtezw"), 4263, string("xaerffmjmecuhvlutgamnygwmzshfovtdxhdgvdeqdadncxenspoaopctussspbxjhzmdarxbrwouoajc"), false, string("sgwwkensybkw"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class xissdvn {
public:
	int unizngxzufrom;
	int fysdcxr;
	bool qnjzwfzkzg;
	string edlrllh;
	double kslwddrtfol;
	xissdvn();
	bool amzccfxqbxouk(double calpss, double udvacro, int mkunvp, int lfdsbcemui, int dqfoxmooyjne, bool bdmpibzotxq, double hrejbsyzoj);
	double rsurjxulsjkh(double kkxzh, string kigpwrlmmqwued, string iqrldjtmfdsdbox, double jxpuzee, bool mtskivon, int qfofnpvoflfnhsr, int cirjswqvp, int lseyzufpo);
	int xdhdtfowqszdrmdyofulk(double zybatmomdfvj, double rqzqlrfpqlgkn, int ezmjn, string xeorojfsedecw, bool gyjcgjaocknqc, int eozxepaxc);
	void esmtyblhzsvdtwarjlpjkqgsv(double pirbwo, double kjvhhxmpvgtp, bool vvorock, string loqwizgu, double ihtugyhq);
	void zinvsezqqgvxgap();
	void hlwvribjbru(bool igyfzaontoc, int pdomrlms, int nmozhombsxby, string hjharjvtqnqvek, int ibqxzvqqvw, double zjbynohwsa, double imiyesolfhvtmwm, string dyyrynniqlb, double yixbcyvw);
	bool ruvyaanlkcscpdcaczox(int rcgoe, int lxqkdejoajidc, int osftjmrnasldh, string blnwpllwen, string xnwlgnybnxdzt, int gdcdvsuyytdjci, bool yveks);
	double jvsidhkurtbltpjhb(int zxjpxhamvxyo, string plivhmil, double aonrw, bool rnexnijov, int wbutzgmfxjdj, double xmunsuzssjienl, string anialmzicdsaqw);

protected:
	double gbntozuhlgd;
	string tejdlsuljvulsa;
	double rajgfhhoarnshpq;

	void ijdcyvoyitiippgzeiydvb(double iovviskqu, double safmrn, int vaaqwvh, int ognkutcrxxgio, bool xckvlhpedgym, string fhuvthuzrd, string rhatxojv);
	void vxvrszllbpnx(int hppdlp, double objgq, double wegrxpr, double bdasvyntfizbr, bool pjwnfosg, bool sukcx, string osjpzo, bool xawiuai);
	bool vbxavgnjztmi(bool mmfaj, int ovtzillnf, string xmugwlghkce, string brfktlnkm, bool kmvyowafsix, double ufhbkotikb, bool gulyojbxfbqawii, bool klouattlrs, bool leejvjbwbzqsk, bool ivjpbys);
	string huczlqpinhmznvahywqh(string tdpuerjzclnao, int ksjgtp, string qdjnj);
	double vgeezddezomblgjwymjlkl(int rpyqi, string qkpbseaubxrr, double giqupolibofqa, int jrmpktagk);
	double mlmvfltogtcqzbebfl(string uhjchkgyxjd, int tyxlaf, double mbchqhgwi, double sluorixkis, double eslobl);
	bool jcszbxunytzznkgds(string phicjivigqq, string vqtrbgj, int bwsgqjndtuj, bool zsfrplv, bool yppemzqbxqtwdiv, bool qizhvadxkx);
	void vdwxpvfcysqrmzrrepveh(string mkiea, string aebodxzudp, string vazzr, string esswnvn, bool xnerulaba, double gfvfh, int gmjueq, double xotkuwzuhanvz);
	int zzfogqxnajdijzukguwojqsp(bool bgkkgenk, double fijeidujbyj, string vrquhfxivrf, double jfqmhydhce, string qwprhpi);

private:
	double asrwdeoalizejg;
	string rwlot;

	bool cubzrvcagwnhgmjpb(int xqakzzsfdcs);
	string mezuoeyojqnuyvujw(double nxqokyif, string rmxzwrvulbmiok, string iiciqoanxfib, int kyafjgat, int gcyqoxueowdyl, string vvsjqhqlmtb);
	int wbbyelbqcoqrrwprsfcmyi(string veouxnnfejzlcq, double cqxrps, int kvwdlrafidvpa);

};


bool xissdvn::cubzrvcagwnhgmjpb(int xqakzzsfdcs) {
	int iqwcwzvgewwbbe = 6423;
	string uucajooamyxwexb = "i";
	double yowgylyjwcuu = 21990;
	if (6423 != 6423) {
		int neri;
		for (neri = 68; neri > 0; neri--) {
			continue;
		}
	}
	if (21990 != 21990) {
		int pidfaloh;
		for (pidfaloh = 73; pidfaloh > 0; pidfaloh--) {
			continue;
		}
	}
	if (6423 != 6423) {
		int de;
		for (de = 75; de > 0; de--) {
			continue;
		}
	}
	if (21990 == 21990) {
		int cmxdkhfjw;
		for (cmxdkhfjw = 8; cmxdkhfjw > 0; cmxdkhfjw--) {
			continue;
		}
	}
	return true;
}

string xissdvn::mezuoeyojqnuyvujw(double nxqokyif, string rmxzwrvulbmiok, string iiciqoanxfib, int kyafjgat, int gcyqoxueowdyl, string vvsjqhqlmtb) {
	string glivwxibrxec = "gteirzxohtswagmzovexaubphiffinbzstfequxipwovejocchzljtiatnvfmjlmrrkhbhmnzagjedimcwcpmdhsjiztw";
	return string("o");
}

int xissdvn::wbbyelbqcoqrrwprsfcmyi(string veouxnnfejzlcq, double cqxrps, int kvwdlrafidvpa) {
	double kfyhfghw = 12389;
	string xoqzjypxkf = "ldaxvpdsbtqaehxcazcyijwsjbbbrubmvfn";
	if (12389 != 12389) {
		int cjlm;
		for (cjlm = 23; cjlm > 0; cjlm--) {
			continue;
		}
	}
	if (string("ldaxvpdsbtqaehxcazcyijwsjbbbrubmvfn") != string("ldaxvpdsbtqaehxcazcyijwsjbbbrubmvfn")) {
		int sflp;
		for (sflp = 75; sflp > 0; sflp--) {
			continue;
		}
	}
	if (12389 == 12389) {
		int zahhyzc;
		for (zahhyzc = 24; zahhyzc > 0; zahhyzc--) {
			continue;
		}
	}
	if (string("ldaxvpdsbtqaehxcazcyijwsjbbbrubmvfn") == string("ldaxvpdsbtqaehxcazcyijwsjbbbrubmvfn")) {
		int rgqsz;
		for (rgqsz = 17; rgqsz > 0; rgqsz--) {
			continue;
		}
	}
	return 70179;
}

void xissdvn::ijdcyvoyitiippgzeiydvb(double iovviskqu, double safmrn, int vaaqwvh, int ognkutcrxxgio, bool xckvlhpedgym, string fhuvthuzrd, string rhatxojv) {
	string tcwxphknmym = "wibeozmlgemktwagauwksphon";
	double yzueneoksfzdrij = 36366;
	double evrigwbcmoc = 74181;
	double cmuehfiazqren = 34363;
	if (string("wibeozmlgemktwagauwksphon") == string("wibeozmlgemktwagauwksphon")) {
		int cmvxsp;
		for (cmvxsp = 91; cmvxsp > 0; cmvxsp--) {
			continue;
		}
	}
	if (string("wibeozmlgemktwagauwksphon") == string("wibeozmlgemktwagauwksphon")) {
		int oylk;
		for (oylk = 31; oylk > 0; oylk--) {
			continue;
		}
	}
	if (74181 != 74181) {
		int pcahugbbz;
		for (pcahugbbz = 13; pcahugbbz > 0; pcahugbbz--) {
			continue;
		}
	}
	if (74181 != 74181) {
		int utwxg;
		for (utwxg = 43; utwxg > 0; utwxg--) {
			continue;
		}
	}

}

void xissdvn::vxvrszllbpnx(int hppdlp, double objgq, double wegrxpr, double bdasvyntfizbr, bool pjwnfosg, bool sukcx, string osjpzo, bool xawiuai) {

}

bool xissdvn::vbxavgnjztmi(bool mmfaj, int ovtzillnf, string xmugwlghkce, string brfktlnkm, bool kmvyowafsix, double ufhbkotikb, bool gulyojbxfbqawii, bool klouattlrs, bool leejvjbwbzqsk, bool ivjpbys) {
	bool napsuiejnxlod = true;
	double aixdakygniqki = 48649;
	string opzzxdtt = "dufw";
	double qeqlkz = 16839;
	string vumkovdn = "rtrgvlmhzrkddafrfqehqzgbbcmbjokz";
	bool sgwobpx = true;
	bool cgviuwvmzfqrle = false;
	double dmgtyxyzxjx = 55709;
	if (string("dufw") != string("dufw")) {
		int yglgtqq;
		for (yglgtqq = 29; yglgtqq > 0; yglgtqq--) {
			continue;
		}
	}
	if (false == false) {
		int gmr;
		for (gmr = 21; gmr > 0; gmr--) {
			continue;
		}
	}
	return false;
}

string xissdvn::huczlqpinhmznvahywqh(string tdpuerjzclnao, int ksjgtp, string qdjnj) {
	string pnncek = "tmrebxzfngiewtqbpbjiboxuhrwnuctnobnyfbqjgtdihdfwfvvsrsa";
	int ympxvv = 3204;
	double eylsv = 17487;
	double gukxufn = 17722;
	double zzkebwkqdjqjqs = 17629;
	if (17629 != 17629) {
		int tgjftmdsvw;
		for (tgjftmdsvw = 73; tgjftmdsvw > 0; tgjftmdsvw--) {
			continue;
		}
	}
	if (17487 == 17487) {
		int mchywh;
		for (mchywh = 6; mchywh > 0; mchywh--) {
			continue;
		}
	}
	return string("pnxjwyswfa");
}

double xissdvn::vgeezddezomblgjwymjlkl(int rpyqi, string qkpbseaubxrr, double giqupolibofqa, int jrmpktagk) {
	double qzplqv = 84035;
	double rrxkx = 33567;
	int boqmztgfypdiijr = 3033;
	string zdryatnstadiya = "nxxrxqsonvzoebrojuzonxotpztebbrfrzbkmpronisailtrazphvzwkvzisqecoirwafqitsvfqkfrfkbqhzhzw";
	string ozlbxkhpw = "mieapywvsfdudddywfrdkprdwpkiavymfcvdbyiabwarponh";
	if (string("mieapywvsfdudddywfrdkprdwpkiavymfcvdbyiabwarponh") != string("mieapywvsfdudddywfrdkprdwpkiavymfcvdbyiabwarponh")) {
		int lu;
		for (lu = 100; lu > 0; lu--) {
			continue;
		}
	}
	if (33567 == 33567) {
		int aa;
		for (aa = 3; aa > 0; aa--) {
			continue;
		}
	}
	if (3033 == 3033) {
		int lm;
		for (lm = 13; lm > 0; lm--) {
			continue;
		}
	}
	if (3033 == 3033) {
		int hrzkrtiaj;
		for (hrzkrtiaj = 28; hrzkrtiaj > 0; hrzkrtiaj--) {
			continue;
		}
	}
	if (3033 != 3033) {
		int rgsqo;
		for (rgsqo = 7; rgsqo > 0; rgsqo--) {
			continue;
		}
	}
	return 95223;
}

double xissdvn::mlmvfltogtcqzbebfl(string uhjchkgyxjd, int tyxlaf, double mbchqhgwi, double sluorixkis, double eslobl) {
	return 9177;
}

bool xissdvn::jcszbxunytzznkgds(string phicjivigqq, string vqtrbgj, int bwsgqjndtuj, bool zsfrplv, bool yppemzqbxqtwdiv, bool qizhvadxkx) {
	int upaxi = 2228;
	double lpsosd = 25600;
	return true;
}

void xissdvn::vdwxpvfcysqrmzrrepveh(string mkiea, string aebodxzudp, string vazzr, string esswnvn, bool xnerulaba, double gfvfh, int gmjueq, double xotkuwzuhanvz) {
	double lpfse = 46219;
	bool azqfvuhn = true;
	double rgyinub = 5619;
	int qumyurzayw = 960;
	double pcsxatu = 47747;

}

int xissdvn::zzfogqxnajdijzukguwojqsp(bool bgkkgenk, double fijeidujbyj, string vrquhfxivrf, double jfqmhydhce, string qwprhpi) {
	bool ievoifakse = false;
	int oxrsmwcxcdsan = 6381;
	bool qirupkehawqabhd = true;
	double uspfalyhcr = 41981;
	double vprldhswqou = 31913;
	string csjcfegpdprfsr = "ws";
	string njhfvforcnqgq = "peekkddkvdoeeywrxzfcxg";
	string ndziowp = "eewldguzqqtblymmftvgirkoufkcoyxhhmgbzzmwykxkiyunbaanijdhleyinxrr";
	string dmeswcpn = "rvamstfhneiorhhpgxblibsalpdbogunjoxeftlkhewmfcbpdqzkkyjebptsbnewwyblbvuroozsrtddaigsmrcirbru";
	string xxfucpwzeqsq = "lugsquldggxnhqixwoawjfumzliamedrwuzhxbimsjpjv";
	return 91898;
}

bool xissdvn::amzccfxqbxouk(double calpss, double udvacro, int mkunvp, int lfdsbcemui, int dqfoxmooyjne, bool bdmpibzotxq, double hrejbsyzoj) {
	bool shjomxvlqkvxt = false;
	double yrmycjna = 8266;
	int gobumgyxci = 2811;
	double kjbhsbbmula = 88;
	if (88 != 88) {
		int mtbdluiwws;
		for (mtbdluiwws = 66; mtbdluiwws > 0; mtbdluiwws--) {
			continue;
		}
	}
	if (88 == 88) {
		int ekfefl;
		for (ekfefl = 44; ekfefl > 0; ekfefl--) {
			continue;
		}
	}
	if (88 == 88) {
		int ghfj;
		for (ghfj = 81; ghfj > 0; ghfj--) {
			continue;
		}
	}
	return false;
}

double xissdvn::rsurjxulsjkh(double kkxzh, string kigpwrlmmqwued, string iqrldjtmfdsdbox, double jxpuzee, bool mtskivon, int qfofnpvoflfnhsr, int cirjswqvp, int lseyzufpo) {
	bool rislko = true;
	bool fmiaaluult = true;
	string rfxjdsu = "eolxwnludjfbkxejdo";
	bool rzyghia = false;
	bool cehpv = true;
	int nytdqfcistu = 860;
	string hkhmvgsglqxyp = "fivlozwehbrsnximdibbeoroujvnyggrjixnjvkrlbvhacympeqntebtqwqrcmalwyiyflguefksrxpifdzsobdkshvkgrrq";
	double wcmhqtwatpcm = 23722;
	bool gxlmlbawtene = true;
	if (true != true) {
		int gli;
		for (gli = 87; gli > 0; gli--) {
			continue;
		}
	}
	if (string("eolxwnludjfbkxejdo") != string("eolxwnludjfbkxejdo")) {
		int fmht;
		for (fmht = 53; fmht > 0; fmht--) {
			continue;
		}
	}
	if (true != true) {
		int swzkgm;
		for (swzkgm = 41; swzkgm > 0; swzkgm--) {
			continue;
		}
	}
	if (860 != 860) {
		int egfyrhgxk;
		for (egfyrhgxk = 42; egfyrhgxk > 0; egfyrhgxk--) {
			continue;
		}
	}
	if (true == true) {
		int mms;
		for (mms = 11; mms > 0; mms--) {
			continue;
		}
	}
	return 99344;
}

int xissdvn::xdhdtfowqszdrmdyofulk(double zybatmomdfvj, double rqzqlrfpqlgkn, int ezmjn, string xeorojfsedecw, bool gyjcgjaocknqc, int eozxepaxc) {
	int uuwpjyb = 760;
	double zdnbhnn = 2349;
	int yevnvz = 2842;
	if (2842 == 2842) {
		int zkxhphmdh;
		for (zkxhphmdh = 97; zkxhphmdh > 0; zkxhphmdh--) {
			continue;
		}
	}
	if (760 == 760) {
		int vyhbah;
		for (vyhbah = 4; vyhbah > 0; vyhbah--) {
			continue;
		}
	}
	if (2842 != 2842) {
		int vif;
		for (vif = 47; vif > 0; vif--) {
			continue;
		}
	}
	if (760 != 760) {
		int vg;
		for (vg = 53; vg > 0; vg--) {
			continue;
		}
	}
	return 67442;
}

void xissdvn::esmtyblhzsvdtwarjlpjkqgsv(double pirbwo, double kjvhhxmpvgtp, bool vvorock, string loqwizgu, double ihtugyhq) {

}

void xissdvn::zinvsezqqgvxgap() {
	bool qdohiuz = false;
	bool czsbov = true;

}

void xissdvn::hlwvribjbru(bool igyfzaontoc, int pdomrlms, int nmozhombsxby, string hjharjvtqnqvek, int ibqxzvqqvw, double zjbynohwsa, double imiyesolfhvtmwm, string dyyrynniqlb, double yixbcyvw) {
	double cprauxlijwbqyms = 63890;
	int vgrmf = 1182;
	int shgqm = 2090;
	int uesdgvtkalcwwgw = 100;
	int yowidhr = 498;
	double svwaujvmuatel = 94050;
	string yqwtbnmsukj = "luhvstgoazitavlfwnwufqqsim";
	if (100 == 100) {
		int khlpeb;
		for (khlpeb = 53; khlpeb > 0; khlpeb--) {
			continue;
		}
	}
	if (2090 != 2090) {
		int ijkhreduhp;
		for (ijkhreduhp = 79; ijkhreduhp > 0; ijkhreduhp--) {
			continue;
		}
	}
	if (2090 == 2090) {
		int acbweghzxh;
		for (acbweghzxh = 22; acbweghzxh > 0; acbweghzxh--) {
			continue;
		}
	}
	if (94050 == 94050) {
		int icdl;
		for (icdl = 55; icdl > 0; icdl--) {
			continue;
		}
	}
	if (63890 == 63890) {
		int hwoilej;
		for (hwoilej = 42; hwoilej > 0; hwoilej--) {
			continue;
		}
	}

}

bool xissdvn::ruvyaanlkcscpdcaczox(int rcgoe, int lxqkdejoajidc, int osftjmrnasldh, string blnwpllwen, string xnwlgnybnxdzt, int gdcdvsuyytdjci, bool yveks) {
	int xhygoaxcytnvosw = 6288;
	bool tryhjaiapxpgrtl = false;
	int qrxmqmbhe = 2882;
	double whdavmzsapgdoz = 75083;
	double reqluzy = 56018;
	int ivcsofpabuhi = 164;
	double vxeifsxhul = 36311;
	if (false != false) {
		int nrbpdryf;
		for (nrbpdryf = 100; nrbpdryf > 0; nrbpdryf--) {
			continue;
		}
	}
	if (56018 != 56018) {
		int jg;
		for (jg = 34; jg > 0; jg--) {
			continue;
		}
	}
	if (2882 != 2882) {
		int hv;
		for (hv = 9; hv > 0; hv--) {
			continue;
		}
	}
	if (75083 != 75083) {
		int dlknpoadwy;
		for (dlknpoadwy = 42; dlknpoadwy > 0; dlknpoadwy--) {
			continue;
		}
	}
	if (56018 != 56018) {
		int ytcyvsws;
		for (ytcyvsws = 27; ytcyvsws > 0; ytcyvsws--) {
			continue;
		}
	}
	return true;
}

double xissdvn::jvsidhkurtbltpjhb(int zxjpxhamvxyo, string plivhmil, double aonrw, bool rnexnijov, int wbutzgmfxjdj, double xmunsuzssjienl, string anialmzicdsaqw) {
	string vvkitmwqbhsj = "dylqcdbromyvqzopcblvhvgchsmj";
	double xhvkh = 43957;
	int bhyuntmvgfxsnr = 3750;
	bool sipoxb = false;
	double kibcolav = 53417;
	double qmfzvnqsi = 28304;
	int ssqczprh = 2260;
	if (2260 != 2260) {
		int pgfcv;
		for (pgfcv = 21; pgfcv > 0; pgfcv--) {
			continue;
		}
	}
	return 10693;
}

xissdvn::xissdvn() {
	this->amzccfxqbxouk(7983, 5842, 2676, 558, 2809, true, 74);
	this->rsurjxulsjkh(2820, string("vtomhxgbnlmcbmpueljxljaippmctawkctqpqylnkut"), string("voiyc"), 9438, false, 277, 5141, 4785);
	this->xdhdtfowqszdrmdyofulk(76968, 14782, 1487, string("wahpibymyqqwmpvutdbwvby"), true, 1803);
	this->esmtyblhzsvdtwarjlpjkqgsv(38545, 49946, false, string("t"), 39703);
	this->zinvsezqqgvxgap();
	this->hlwvribjbru(true, 2557, 5145, string("vrroyvgufsmmbzruweixststthvjxkjjkavnfcjagfckxbinbeenmjcydnmlgviluivbvg"), 833, 3727, 16786, string("bzozozxjudldcqoxffyozwgedqamvdqgaflnbxjdwgboquidpkgkdkmkvqahpyiaopoodkaquvqtqkxgrtywyoxxq"), 35764);
	this->ruvyaanlkcscpdcaczox(1053, 5170, 2464, string("grbxmtjxciaafiomvcppluptk"), string("skphsnebaoevsvflxhqbloqiiaohhsmupcjsijpkkuxycutdfsksfsiiu"), 7695, false);
	this->jvsidhkurtbltpjhb(691, string("wuqsjatlrdvxowbrjpqbqzgfz"), 67901, false, 646, 46533, string("ulpafggsrwugjwgzh"));
	this->ijdcyvoyitiippgzeiydvb(2678, 5459, 4786, 4445, true, string("oogjqfmpdzvdfbdhuimesmkudyxsxurwtgqjawedqijhhutcsekzfztswtymlsocmicekbpyykbrftswj"), string("dxqacdsxnvylqtvmftzmcmdhhazbawfolaidiyhwaqfrlrwqupdlbqckxhafwombslgggbtrj"));
	this->vxvrszllbpnx(2163, 61850, 24198, 3958, false, true, string("dlgnqaanbsxapshwfpcbwyrdzwcsizobwfatxfjni"), true);
	this->vbxavgnjztmi(true, 854, string("w"), string("xqfmxvxbutgdevxjyladfxbanrbilgwvjaufotxilmlocefxztvnaalismffotjkofqvjkpvxzt"), false, 80942, true, true, false, true);
	this->huczlqpinhmznvahywqh(string("py"), 430, string("efeilrigvdppvqwejhuavvhfydpluzyqmsekmueeeyuixyigeuzpoprtiloiilxsajzmqswejpvvlrbiiyhuiqfm"));
	this->vgeezddezomblgjwymjlkl(73, string("ajhzntjjpjrfchnloakuwccbqrnxxthtdrwdfyjokedyrcjyetdfzhowtgygiuhffniljqzvidiopmwoimpyi"), 22620, 1513);
	this->mlmvfltogtcqzbebfl(string("ogshaqzvczftiyeaxcq"), 715, 25491, 54630, 39397);
	this->jcszbxunytzznkgds(string("tpvkljymdjedupuoffebfnvwovmxwpdiggrtdpzhqbkrcvbsuqmhhcgunhemawdazklopoccrwrrdnnilssoyaasdtfybvgpd"), string("hwbnyvdtkmruimtfunhymkqlpzehuwkpikwvhfomfuqgrwplxopwggdoosfjdohcyhsgttcvjiicbkdcztpyz"), 1499, false, true, false);
	this->vdwxpvfcysqrmzrrepveh(string("cwleeorrphjqqlsvejgkrzeatejltmxhfskwblszgfpzuolbwmroswwtalaifzzgalnwjvzhecfrmqskwpxy"), string("qufzpuhsfehgvknhrvwnq"), string("zulgcpsobpznjterkehoizwruayjebcqcymnvmrniuqokioafpzqeoowgir"), string("wdvhf"), true, 10968, 4974, 47050);
	this->zzfogqxnajdijzukguwojqsp(true, 11231, string("hjpeqskuliikvpfnltzhidkxbjzjdrlwhobmqfalee"), 18462, string("rrcsnmpxylyexvurkyqucmhsthffuyukqkvialx"));
	this->cubzrvcagwnhgmjpb(387);
	this->mezuoeyojqnuyvujw(25927, string("qdpkzanlwisylcmdizpdeilwvnybgzplnpcfutdokiwwonbhnkvfxhfljqkkwirktewifovbjyolekepisv"), string("ibylkkrlkfoyqrsinioabuudmnrzmdmxqjirtcdxnmxigietbuvvvnbmacmjxilnhwkelgakdthxmafgwy"), 4706, 1554, string("ukbffdxxvswizhwoqgzhszohbwmhvczexvkxfsbmnkotbkptsyfcas"));
	this->wbbyelbqcoqrrwprsfcmyi(string("pswmxbdkfiawyqlwkgwkmywvdopivsfyuaozcm"), 13537, 786);
}
