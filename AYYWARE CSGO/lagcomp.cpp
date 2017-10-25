#include "lagcomp.h"
#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME(t) (Interfaces::Globals->interval_per_tick * (t) )

LagCompensation::LagCompensation()
{

}

void LagCompensation::logEntity(IClientEntity *player)
{
	int idx = player->GetIndex();
	LagRecord_A *m_LagRecords = this->m_LagRecord[idx];

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
	static float oldLBY[65];

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(player->GetActiveWeaponHandle());
	int Bullets = -1;
	if (pWeapon)
		Bullets = pWeapon->GetAmmoInClip();


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
	m_LagRecords[highestRecordIdx].m_iPrevBullets = Bullets;
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

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(player->GetActiveWeaponHandle());
	int Bullets = -1;
	if (pWeapon)
		Bullets = pWeapon->GetAmmoInClip();

	this->m_PrevRecords[idx].m_iPrevBullets = Bullets;
	this->m_PrevRecords[idx].m_vAbsOrigin = player->getAbsOriginal();
	this->m_PrevRecords[idx].m_vecVelocity = player->GetVelocity();
	this->m_PrevRecords[idx].m_fSimulationTime = player->GetSimulationTime();
	this->m_PrevRecords[idx].m_vecAngles = player->getAbsAechse();
	this->m_PrevRecords[idx].m_eyeAngles = *player->GetEyeAnglesXY();
	this->m_PrevRecords[idx].m_flCycle = player->getCycle();
	this->m_PrevRecords[idx].m_nSequence = player->getSequence();
	this->m_PrevRecords[idx].flags = player->GetFlags();
	this->m_PrevRecords[idx].m_flLowerBodyYawTarget = player->GetLowerBodyYaw();

	for (int i = 0; i < 24; i++)
		this->m_PrevRecords[idx].m_flPoseParameter[i] = player->getPoseParams(i);
}

void LagCompensation::setEntity(IClientEntity *player, LagRecord_A record)
{
	if (!player || record.m_fSimulationTime == 0.0f)
	{
		record.m_bIsFixed = false;
		return;
	}

	this->logCurrentEnt(player);


	
	player->setAbsOriginal(record.m_vAbsOrigin);
	*(float*)((DWORD)player + 0x264) = record.m_fSimulationTime;
	*(Vector*)((DWORD)player + 0x110) = record.m_vecVelocity;
	player->setAbsAechse(record.m_vecAngles);

	player->GetEyeAnglesXY()->x = record.m_eyeAngles.x;
	player->GetEyeAnglesXY()->y = record.m_eyeAngles.y;
	player->GetEyeAnglesXY()->z = record.m_eyeAngles.z;

	*(float*)((DWORD)player + 0xA14) = record.m_flCycle;
	*(int*)((DWORD)player + 0x28AC) = record.m_nSequence;
	*(int*)((DWORD)player + 0x100) = record.flags;
	*(float*)((DWORD)player + 0x39D8) = record.m_flLowerBodyYawTarget;

	for (int i = 0; i < 24; i++)
		*(float*)((DWORD)player + 0x2764 + sizeof(float) * i) = record.m_flPoseParameter[i];


	record.m_iWantedTick = record.m_fSimulationTime;
	record.m_bIsFixed = true;

	player->updateClientSideAnimation();
	static DWORD invalKostCache = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1\x00\x00\x00\x00\x48\xC7\x81", "xx?????xxx????xxx");
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalKostCache + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1);

}

void LagCompensation::setCurrentEnt(IClientEntity *player)
{
	int idx = player->GetIndex();

	m_PrevRecords[idx].m_iWantedTick = m_PrevRecords[idx].m_fSimulationTime;


	player->setAbsOriginal(m_PrevRecords[idx].m_vAbsOrigin);
	*(Vector*)((DWORD)player + 0x110) = m_PrevRecords[idx].m_vecVelocity;
	*(float*)((DWORD)player + 0x264) = m_PrevRecords[idx].m_fSimulationTime;
	player->setAbsAechse(m_PrevRecords[idx].m_vecAngles);

	player->GetEyeAnglesXY()->x = m_PrevRecords[idx].m_eyeAngles.x;
	player->GetEyeAnglesXY()->y = m_PrevRecords[idx].m_eyeAngles.y;
	player->GetEyeAnglesXY()->z = m_PrevRecords[idx].m_eyeAngles.z;


	*(float*)((DWORD)player + 0xA14) = m_PrevRecords[idx].m_flCycle;
	*(int*)((DWORD)player + 0x28AC) = m_PrevRecords[idx].m_nSequence;
	*(int*)((DWORD)player + 0x100) = m_PrevRecords[idx].flags;
	*(float*)((DWORD)player + 0x39D8) = m_PrevRecords[idx].m_flLowerBodyYawTarget;


	for (int i = 0; i < 24; i++)
		*(float*)((DWORD)player + 0x2764 + sizeof(float) * i) = m_PrevRecords[idx].m_flPoseParameter[i];

	player->updateClientSideAnimation();

	static DWORD invalKostCache = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1\x00\x00\x00\x00\x48\xC7\x81", "xx?????xxx????xxx");
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalKostCache + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1);

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

	int ud_rate = Interfaces::CVar->FindVar("cl_updaterate")->GetInt();
	ConVar *min_ud_rate = Interfaces::CVar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = Interfaces::CVar->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetInt();

	float ratio = Interfaces::CVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = Interfaces::CVar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

	return max(lerp, (ratio / ud_rate));
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
	static int BacktrackTicks = 3;
	int idx = player->GetIndex();

	LagRecord_A *m_LagRecords = this->m_LagRecord[idx];

	LagRecord_A recentLR = m_LagRecords[BacktrackTicks];

	if (recentLR.m_fSimulationTime == 0.0f)
		return TIME_TO_TICKS(player->GetSimulationTime()) + 1;

	int iLerpTicks = TIME_TO_TICKS(lerpTime());
	int iTargetTickCount = TIME_TO_TICKS(recentLR.m_fSimulationTime);


	return iTargetTickCount + 1;
}

bool LagCompensation::isValidTick(float simTime)
{

	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (!nci)
		return false;

	auto LerpTicks = TIME_TO_TICKS(lerpTime());

	int predCmdArrivTick = Interfaces::Globals->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));

	float flCorrect = clamp(lerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + LerpTicks - (TIME_TO_TICKS(simTime) + TIME_TO_TICKS(lerpTime())));

	return abs(flCorrect) <= 0.2f;

}


int LagCompensation::fakeLagFix(IClientEntity *player, int historyIdx)
{

	int idx = player->GetIndex();

	LagRecord_A *m_LagRecords = this->m_LagRecord[idx];

	LagRecord_A recentLR = m_LagRecords[historyIdx];
	LagRecord_A prevLR;
	if (historyIdx == 0)
		prevLR = m_LagRecords[8];
	else
		prevLR = m_LagRecords[historyIdx - 1];

	if (recentLR.m_fSimulationTime == 0.0f)
		return -1;


	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	int predCmdArrivTick = Interfaces::Globals->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING)); // (c) n0xius @ uc

	int iLerpTicks = TIME_TO_TICKS(lerpTime());
	int iTargetTickCount = TIME_TO_TICKS(recentLR.m_fSimulationTime) + iLerpTicks;



	float flCorrect = clamp(lerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + TIME_TO_TICKS(lerpTime()) - iTargetTickCount); // (c) n0xius @ uc

	if (fabs(flCorrect) > 0.2f) // Too big deltatime, can't lagfix here
	{
		
		//BacktrackCanShotEnemy = false;
		Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 0, 255), "StartLagCompensation: delta too big (%.3f)\n", flCorrect);
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
	ResolverStage[idx] = 20;
	//BacktrackCanShotEnemy = true;
	return TIME_TO_TICKS(recentLR.m_fSimulationTime);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class mboktfc {
public:
	int arror;
	double utiqocm;
	double lkptqv;
	double eyjvwv;
	bool lvffrkkr;
	mboktfc();
	string xoworfkqzglyxxtfpudgb(bool dzlkxi, bool qcyuteao, int bsyvlpccvj, int kjyxbbtndnlxi, double dbfnmwrkoffqbtw, string jiqtfvkp, double glnppxn, double eqixypuqjph, int ejhwcpoxw, string zspqkmpymojlftp);
	int wqyluyfpdoj(int dmgutbmth, bool ptnuceqnbzsyqa, double pkujz, int cermbcdrbn, bool atwwlkwiwnrgt, bool iqayeecdnfdsz, int bswgkuk, int rkjcohuvds);
	bool cyjznjdylzytbp(int bbqherduvl, double jgxxdx, bool zcawhktac, int vewcm, bool jezrhj, double ldmmjunmd);
	bool ojvlcwaxtpgachvjh(string andrpflkpmpfnva, double xrnltbvrcuzzg, bool fwphl, double tlqjfgjn);
	string ypwwqhcgbymnq(string bqutjuyfe, string wroveyrxa, double wjzhwcojetmsp, bool qqzwedkoxwtue);
	int hjwmazaqxvxahd(bool ddazwszuskil, int kzfxlbafgvlvfuh, double wjboflnjvr, double plfuuxyezzehvt, bool soxyeunmebucerx, bool wkcpkpbia, bool cyhpsh, int pdmrxmsjhux, double ziedckr, string emdkbdmmwhbnzd);
	bool pclrszpkkgmbd(string rrgrfrqdov, bool wytcj, double lnleclqbynwi, string ttsoaobgqmtxrp, string clvsfx, int qbqswdsiomyh, string ebjiagedazuli, double nocleyi, double rrlnrtzbizfcz, bool mjrxmlhawocwc);
	string mbeaymaqvxcwlwgkovvwsxef(string dyvueiixq, string ttqlwfv, string ffypdq, int uyvnbyqrjcn, double vyvdryhqddzfr);

protected:
	string eikozll;
	bool yjdxolmpzyped;
	double ktgccsaqndnbusj;
	int wedambnseyc;

	double xpczteubaagw(int ffyqzphskjul, double ahxvqsdeqcp, double wjzuz, string nbundbvusxpa);
	double pqrhlmyjtrevbxek(double qoobeh);
	string agstsokwmjheb(int odufmdozldcd, bool cqrwjjsuvl);
	void afduvrwipzhwquvpotqkbtfl(int yitdth, string zkhndtnloisuzpx, string xnhjnqly, double bjhlnrqmpaeuqg, string tzsjbyksxto, bool nepviymc);
	void gwzawyusjhfdtj(double kciaogdwtp, int gxsyymagyugw, string dktdzvlkltfsgq, double plsrhr, int nerdpguhyb, double sygqssmz, int nrakbzzrxqmjrp, int ggprxbhuooh);

private:
	double jdadbtbxkohv;

	double mowxskfjeuyctppnzhemr(string itknx, int mfinogyzrphx, int ggusjutwmd, double vykfnwvybkrve);
	bool dnzkbfzpmprpbzbueqqomwsl(bool pnqczpaxrlof, string wihabe, double pdtrmucr, string iivphx, bool jvzgwyejtz, int fgccrrmmcpskla, int ujcjsgjcwkb, int dpazjmlpwgfraj, double dhexo);
	int duniyarojarznvzzazykt(double tsufwv, string zjznavpmqxy, string yvvorlh, bool pxwoeboraxpkxct, bool omdhtpdxems, double xeqdeqnco, bool wfxrheozqoupgle, double ebltuhgwzrwv, int jqluyoyr);
	double mzzbvgqzxuvqhlorkrh(double zpgiwfwdny, double hkhbethjlu, bool aqcnwdjq, double mavrysk, double vvarhtry, double vyillxpl, double vleieirqcs);
	int tgoisdhgsyjxjddraldzdhin(double hwfycxauw, string jehostswit, double xxlkjzttwlzsr, string jnpoxntkmwo, bool pefxmvurpoxtjfw, int pkaepgsu, bool depdjxxksctxb);
	double ziulbdvbrlpvkvmotuwysdry(string zwtby, int jmxctslcdtqvwc, int crvpotqfi, double pdnyanlarykz, double oucpclrvajdybo, int yjzbgxlibb, bool ebglu);
	int vffpkdomubsosd(int slimshv, int qfvggehedxavz, string nhjijixdozddsm, string larfootzowgctxj, bool kaobvdmunxov, double sxvkgfezno, bool yqcoaagxxvjtxh, int wbpftk, bool ztppa, int xbrosj);
	void pvkqmglgegugpyswg(double fluzalcxm, string hbjnprsdn, string bamnaixehe, int ihtggdgsrbczj, bool eoxnojael, bool dajdd, bool uvbhtwckpufj, string gawzgrz, bool oalnauihyw, double uwumhfbxw);
	void fingqsnxxq(double ngxfcayrqho, int iyedfmqywytst, string rkalzj);

};


double mboktfc::mowxskfjeuyctppnzhemr(string itknx, int mfinogyzrphx, int ggusjutwmd, double vykfnwvybkrve) {
	return 83014;
}

bool mboktfc::dnzkbfzpmprpbzbueqqomwsl(bool pnqczpaxrlof, string wihabe, double pdtrmucr, string iivphx, bool jvzgwyejtz, int fgccrrmmcpskla, int ujcjsgjcwkb, int dpazjmlpwgfraj, double dhexo) {
	string vfpgwpkq = "ppqknxrjwuqkfrbo";
	if (string("ppqknxrjwuqkfrbo") == string("ppqknxrjwuqkfrbo")) {
		int ud;
		for (ud = 2; ud > 0; ud--) {
			continue;
		}
	}
	if (string("ppqknxrjwuqkfrbo") == string("ppqknxrjwuqkfrbo")) {
		int rfiqrjk;
		for (rfiqrjk = 66; rfiqrjk > 0; rfiqrjk--) {
			continue;
		}
	}
	if (string("ppqknxrjwuqkfrbo") != string("ppqknxrjwuqkfrbo")) {
		int nxgxbcx;
		for (nxgxbcx = 91; nxgxbcx > 0; nxgxbcx--) {
			continue;
		}
	}
	if (string("ppqknxrjwuqkfrbo") != string("ppqknxrjwuqkfrbo")) {
		int gu;
		for (gu = 72; gu > 0; gu--) {
			continue;
		}
	}
	return true;
}

int mboktfc::duniyarojarznvzzazykt(double tsufwv, string zjznavpmqxy, string yvvorlh, bool pxwoeboraxpkxct, bool omdhtpdxems, double xeqdeqnco, bool wfxrheozqoupgle, double ebltuhgwzrwv, int jqluyoyr) {
	string msdemfuhz = "dicgaqesmiqvsbvgaicehygzlynfpqlkmaspkguntixuvbdknmtotchtvxunrmizxsbtdchdjxstocpfuhfrzplmhjrmstlg";
	int pceqcalqglwuuto = 920;
	bool linccoqna = false;
	if (string("dicgaqesmiqvsbvgaicehygzlynfpqlkmaspkguntixuvbdknmtotchtvxunrmizxsbtdchdjxstocpfuhfrzplmhjrmstlg") == string("dicgaqesmiqvsbvgaicehygzlynfpqlkmaspkguntixuvbdknmtotchtvxunrmizxsbtdchdjxstocpfuhfrzplmhjrmstlg")) {
		int pluw;
		for (pluw = 90; pluw > 0; pluw--) {
			continue;
		}
	}
	if (string("dicgaqesmiqvsbvgaicehygzlynfpqlkmaspkguntixuvbdknmtotchtvxunrmizxsbtdchdjxstocpfuhfrzplmhjrmstlg") != string("dicgaqesmiqvsbvgaicehygzlynfpqlkmaspkguntixuvbdknmtotchtvxunrmizxsbtdchdjxstocpfuhfrzplmhjrmstlg")) {
		int esbbecy;
		for (esbbecy = 47; esbbecy > 0; esbbecy--) {
			continue;
		}
	}
	return 30028;
}

double mboktfc::mzzbvgqzxuvqhlorkrh(double zpgiwfwdny, double hkhbethjlu, bool aqcnwdjq, double mavrysk, double vvarhtry, double vyillxpl, double vleieirqcs) {
	bool yrmnwzarryple = true;
	double otdxbmnwbd = 8285;
	bool ziprtp = true;
	double xqgroybvs = 43575;
	if (true == true) {
		int kpal;
		for (kpal = 95; kpal > 0; kpal--) {
			continue;
		}
	}
	if (true != true) {
		int rawpanl;
		for (rawpanl = 12; rawpanl > 0; rawpanl--) {
			continue;
		}
	}
	if (43575 != 43575) {
		int yq;
		for (yq = 21; yq > 0; yq--) {
			continue;
		}
	}
	if (true == true) {
		int schcvj;
		for (schcvj = 21; schcvj > 0; schcvj--) {
			continue;
		}
	}
	return 77993;
}

int mboktfc::tgoisdhgsyjxjddraldzdhin(double hwfycxauw, string jehostswit, double xxlkjzttwlzsr, string jnpoxntkmwo, bool pefxmvurpoxtjfw, int pkaepgsu, bool depdjxxksctxb) {
	string wdgrxuuiwqaxj = "noartkbctozlrtajrxhmabnxkrkdcdrnsxnptmarveyvwnhasbbcfiwigkcuqzefhbn";
	bool gcyyymqjchoxgo = false;
	bool aienwflrzkjrlh = false;
	bool nawcyfntvrfjkkr = false;
	int nqiavuauhw = 3699;
	bool rgbmb = false;
	string wzpdozmy = "tyvyfcpvvvegfvqrdcvmnnaolcbtnvgbgwnubomjicxhtiwlrtmciyftgtonkmbqdjpdrxrvrpifprziqwhqqiuwzqopgdhvjxvd";
	int fjfclpkedgofewj = 1351;
	string ykfvovph = "ulmbyycowiwgrcakjmdzuqzwxuumqnnjazqjfijomverxaslevb";
	int cyhycrul = 4844;
	if (4844 != 4844) {
		int pqleokgc;
		for (pqleokgc = 79; pqleokgc > 0; pqleokgc--) {
			continue;
		}
	}
	return 8671;
}

double mboktfc::ziulbdvbrlpvkvmotuwysdry(string zwtby, int jmxctslcdtqvwc, int crvpotqfi, double pdnyanlarykz, double oucpclrvajdybo, int yjzbgxlibb, bool ebglu) {
	bool hinqdqjzzb = false;
	double iiwhqgcqfsin = 5121;
	string axsygtf = "axyfhxrdlabmgtzernpugzptacwbhrrkncgygebaxephpckyiubswcszghybppu";
	if (false != false) {
		int znirz;
		for (znirz = 15; znirz > 0; znirz--) {
			continue;
		}
	}
	if (5121 != 5121) {
		int urwfu;
		for (urwfu = 89; urwfu > 0; urwfu--) {
			continue;
		}
	}
	if (string("axyfhxrdlabmgtzernpugzptacwbhrrkncgygebaxephpckyiubswcszghybppu") == string("axyfhxrdlabmgtzernpugzptacwbhrrkncgygebaxephpckyiubswcszghybppu")) {
		int jncthc;
		for (jncthc = 32; jncthc > 0; jncthc--) {
			continue;
		}
	}
	return 68173;
}

int mboktfc::vffpkdomubsosd(int slimshv, int qfvggehedxavz, string nhjijixdozddsm, string larfootzowgctxj, bool kaobvdmunxov, double sxvkgfezno, bool yqcoaagxxvjtxh, int wbpftk, bool ztppa, int xbrosj) {
	string mnespdthzavl = "mzfdhmojxxupvbede";
	string qnnucjk = "fbyuqoiibbwmzzlzowwmlcujqvdweqgibatzoyivjed";
	if (string("fbyuqoiibbwmzzlzowwmlcujqvdweqgibatzoyivjed") != string("fbyuqoiibbwmzzlzowwmlcujqvdweqgibatzoyivjed")) {
		int irbuflskwt;
		for (irbuflskwt = 1; irbuflskwt > 0; irbuflskwt--) {
			continue;
		}
	}
	if (string("mzfdhmojxxupvbede") == string("mzfdhmojxxupvbede")) {
		int bkv;
		for (bkv = 91; bkv > 0; bkv--) {
			continue;
		}
	}
	return 29123;
}

void mboktfc::pvkqmglgegugpyswg(double fluzalcxm, string hbjnprsdn, string bamnaixehe, int ihtggdgsrbczj, bool eoxnojael, bool dajdd, bool uvbhtwckpufj, string gawzgrz, bool oalnauihyw, double uwumhfbxw) {
	int ljnong = 6644;
	double rhmbnooapkx = 40093;
	string njiqgswv = "vmpslajgushxfbgeszfrkcwpucvlpwjllkysfhiqivbnhzywffcnwshwrmuvyzrq";
	double uoxsqohauzqeys = 71757;
	string iifkdsyfzukotii = "uyuviandxkvjwbpqlyjwkdsuguywduxyryxnkiixmdbmbuevzjibpcadriwhjbuzmwgmvcfimklihgnrwgapckslbcad";
	double evazj = 55384;
	double jjdfmhmcfxft = 37246;
	double idrkfgerk = 5237;
	int odezecq = 2426;
	string aduphwgqgevscr = "gbfpiejnzxeqlvedhwnitmnzlnahjxtiksnf";
	if (55384 == 55384) {
		int kkde;
		for (kkde = 87; kkde > 0; kkde--) {
			continue;
		}
	}
	if (string("uyuviandxkvjwbpqlyjwkdsuguywduxyryxnkiixmdbmbuevzjibpcadriwhjbuzmwgmvcfimklihgnrwgapckslbcad") == string("uyuviandxkvjwbpqlyjwkdsuguywduxyryxnkiixmdbmbuevzjibpcadriwhjbuzmwgmvcfimklihgnrwgapckslbcad")) {
		int emvyjaox;
		for (emvyjaox = 61; emvyjaox > 0; emvyjaox--) {
			continue;
		}
	}
	if (string("vmpslajgushxfbgeszfrkcwpucvlpwjllkysfhiqivbnhzywffcnwshwrmuvyzrq") == string("vmpslajgushxfbgeszfrkcwpucvlpwjllkysfhiqivbnhzywffcnwshwrmuvyzrq")) {
		int rx;
		for (rx = 84; rx > 0; rx--) {
			continue;
		}
	}
	if (string("uyuviandxkvjwbpqlyjwkdsuguywduxyryxnkiixmdbmbuevzjibpcadriwhjbuzmwgmvcfimklihgnrwgapckslbcad") == string("uyuviandxkvjwbpqlyjwkdsuguywduxyryxnkiixmdbmbuevzjibpcadriwhjbuzmwgmvcfimklihgnrwgapckslbcad")) {
		int idzthcsozi;
		for (idzthcsozi = 38; idzthcsozi > 0; idzthcsozi--) {
			continue;
		}
	}
	if (string("uyuviandxkvjwbpqlyjwkdsuguywduxyryxnkiixmdbmbuevzjibpcadriwhjbuzmwgmvcfimklihgnrwgapckslbcad") == string("uyuviandxkvjwbpqlyjwkdsuguywduxyryxnkiixmdbmbuevzjibpcadriwhjbuzmwgmvcfimklihgnrwgapckslbcad")) {
		int qmkixzyi;
		for (qmkixzyi = 46; qmkixzyi > 0; qmkixzyi--) {
			continue;
		}
	}

}

void mboktfc::fingqsnxxq(double ngxfcayrqho, int iyedfmqywytst, string rkalzj) {
	double kcespfxl = 3849;
	double vglnzcyfmhv = 26311;
	int noijprngsp = 178;
	bool eeqken = true;
	double onbktamgkmeoftu = 24411;

}

double mboktfc::xpczteubaagw(int ffyqzphskjul, double ahxvqsdeqcp, double wjzuz, string nbundbvusxpa) {
	string xhyeibxj = "wobnfpvmztupmrvlsalpmnkvrwllbsvmpeghggnrxmtcstrvdwsdxrnjkwbrcxwcgugqtublotnle";
	double gmoaeypbwqiss = 15569;
	bool meebxnmhdlpm = false;
	bool harolmtiknmt = true;
	return 19938;
}

double mboktfc::pqrhlmyjtrevbxek(double qoobeh) {
	string bjunkowzfq = "buxcgzakllvhyzrdlsbluiy";
	double mkasyjs = 882;
	double uyenpfnzv = 8197;
	int rjggqqsh = 711;
	int xdofnpytthep = 4125;
	string quacx = "ddzokjqfamdspvaihfgtbnkmolodjrivkyxudgfctahdldsqqionrlzpejcyonxhbsnsrvyrkwhajmsnnmbzhjcdej";
	string cgoiafkiovzfth = "vyjdtulmhbq";
	int ubtaqmmedaxxj = 6160;
	double emjbdpbyawlwog = 13314;
	bool rpglxl = true;
	if (string("buxcgzakllvhyzrdlsbluiy") == string("buxcgzakllvhyzrdlsbluiy")) {
		int zl;
		for (zl = 53; zl > 0; zl--) {
			continue;
		}
	}
	if (string("buxcgzakllvhyzrdlsbluiy") != string("buxcgzakllvhyzrdlsbluiy")) {
		int mtafkplnr;
		for (mtafkplnr = 15; mtafkplnr > 0; mtafkplnr--) {
			continue;
		}
	}
	return 59692;
}

string mboktfc::agstsokwmjheb(int odufmdozldcd, bool cqrwjjsuvl) {
	double hsbnr = 10964;
	double zbfocthkivoq = 41055;
	int oforbkyz = 2194;
	if (41055 == 41055) {
		int xlvwyhmxw;
		for (xlvwyhmxw = 76; xlvwyhmxw > 0; xlvwyhmxw--) {
			continue;
		}
	}
	if (2194 != 2194) {
		int pmcxzsuu;
		for (pmcxzsuu = 29; pmcxzsuu > 0; pmcxzsuu--) {
			continue;
		}
	}
	if (41055 != 41055) {
		int tggqlthyek;
		for (tggqlthyek = 90; tggqlthyek > 0; tggqlthyek--) {
			continue;
		}
	}
	return string("oiiukbaxgg");
}

void mboktfc::afduvrwipzhwquvpotqkbtfl(int yitdth, string zkhndtnloisuzpx, string xnhjnqly, double bjhlnrqmpaeuqg, string tzsjbyksxto, bool nepviymc) {
	int elbcsthjpzbwnmg = 6178;
	double jnptrrvicvy = 61716;
	if (6178 != 6178) {
		int ldsqsf;
		for (ldsqsf = 29; ldsqsf > 0; ldsqsf--) {
			continue;
		}
	}
	if (61716 == 61716) {
		int ay;
		for (ay = 30; ay > 0; ay--) {
			continue;
		}
	}

}

void mboktfc::gwzawyusjhfdtj(double kciaogdwtp, int gxsyymagyugw, string dktdzvlkltfsgq, double plsrhr, int nerdpguhyb, double sygqssmz, int nrakbzzrxqmjrp, int ggprxbhuooh) {
	bool lnzypm = false;
	int nnjenjvhf = 778;
	string dgdav = "krelpykdg";
	int udsvnpttqinpk = 1884;
	int mhxzfc = 1705;
	double sdhhfggvunwmzj = 24531;
	if (false != false) {
		int xpbodvn;
		for (xpbodvn = 11; xpbodvn > 0; xpbodvn--) {
			continue;
		}
	}
	if (778 != 778) {
		int seeag;
		for (seeag = 70; seeag > 0; seeag--) {
			continue;
		}
	}
	if (778 == 778) {
		int qmbpy;
		for (qmbpy = 28; qmbpy > 0; qmbpy--) {
			continue;
		}
	}
	if (string("krelpykdg") == string("krelpykdg")) {
		int kfngg;
		for (kfngg = 28; kfngg > 0; kfngg--) {
			continue;
		}
	}
	if (24531 == 24531) {
		int pzuwopuuw;
		for (pzuwopuuw = 91; pzuwopuuw > 0; pzuwopuuw--) {
			continue;
		}
	}

}

string mboktfc::xoworfkqzglyxxtfpudgb(bool dzlkxi, bool qcyuteao, int bsyvlpccvj, int kjyxbbtndnlxi, double dbfnmwrkoffqbtw, string jiqtfvkp, double glnppxn, double eqixypuqjph, int ejhwcpoxw, string zspqkmpymojlftp) {
	double kuhbfw = 12014;
	string csevno = "ekikbyuynqmrltqiznuxuaatgixfedqhwpnvooulqezvxyxcdezvwoiksic";
	return string("embsfyrbgve");
}

int mboktfc::wqyluyfpdoj(int dmgutbmth, bool ptnuceqnbzsyqa, double pkujz, int cermbcdrbn, bool atwwlkwiwnrgt, bool iqayeecdnfdsz, int bswgkuk, int rkjcohuvds) {
	bool qbhawujmqigjs = false;
	if (false == false) {
		int xh;
		for (xh = 18; xh > 0; xh--) {
			continue;
		}
	}
	if (false == false) {
		int uxsyyop;
		for (uxsyyop = 37; uxsyyop > 0; uxsyyop--) {
			continue;
		}
	}
	if (false == false) {
		int bcstwpnvhc;
		for (bcstwpnvhc = 30; bcstwpnvhc > 0; bcstwpnvhc--) {
			continue;
		}
	}
	return 62492;
}

bool mboktfc::cyjznjdylzytbp(int bbqherduvl, double jgxxdx, bool zcawhktac, int vewcm, bool jezrhj, double ldmmjunmd) {
	bool dthfr = true;
	string cnmyptknspfdyx = "dsmulupnorjmbasqbkao";
	double thujhndku = 4274;
	string ntdeu = "kuntokkfiqhidycrxfujustgbreixpvsurjhyecaymtmzeduccedprdurvqeazknhfpqkfqklnjxpajrnyoksrrcknzqhxuxjy";
	bool ncikspiouhg = false;
	double bssfptwilfhezm = 15345;
	bool gdfqanveo = true;
	int fsjwluscggg = 539;
	if (false == false) {
		int qfkdzqr;
		for (qfkdzqr = 83; qfkdzqr > 0; qfkdzqr--) {
			continue;
		}
	}
	if (string("dsmulupnorjmbasqbkao") != string("dsmulupnorjmbasqbkao")) {
		int dolrw;
		for (dolrw = 59; dolrw > 0; dolrw--) {
			continue;
		}
	}
	if (4274 == 4274) {
		int cbrunvw;
		for (cbrunvw = 49; cbrunvw > 0; cbrunvw--) {
			continue;
		}
	}
	return false;
}

bool mboktfc::ojvlcwaxtpgachvjh(string andrpflkpmpfnva, double xrnltbvrcuzzg, bool fwphl, double tlqjfgjn) {
	int vbcfmahiuwfvv = 1877;
	bool czaznrdcvlz = false;
	string toyokqsrzemgep = "tgojqirkcqgivbbcsyomfutpnlvtqwybkurzgxhnmzdhxezxlfdmcygpnsezluyucqmqv";
	if (1877 == 1877) {
		int hdplmihgba;
		for (hdplmihgba = 71; hdplmihgba > 0; hdplmihgba--) {
			continue;
		}
	}
	return false;
}

string mboktfc::ypwwqhcgbymnq(string bqutjuyfe, string wroveyrxa, double wjzhwcojetmsp, bool qqzwedkoxwtue) {
	int jvwqcbe = 1050;
	double yvjsnb = 7752;
	string ckmqnd = "o";
	if (string("o") == string("o")) {
		int mb;
		for (mb = 49; mb > 0; mb--) {
			continue;
		}
	}
	if (1050 != 1050) {
		int tinaogn;
		for (tinaogn = 2; tinaogn > 0; tinaogn--) {
			continue;
		}
	}
	if (string("o") != string("o")) {
		int wv;
		for (wv = 48; wv > 0; wv--) {
			continue;
		}
	}
	return string("wkpwypbmuk");
}

int mboktfc::hjwmazaqxvxahd(bool ddazwszuskil, int kzfxlbafgvlvfuh, double wjboflnjvr, double plfuuxyezzehvt, bool soxyeunmebucerx, bool wkcpkpbia, bool cyhpsh, int pdmrxmsjhux, double ziedckr, string emdkbdmmwhbnzd) {
	int nelgldbanntf = 6772;
	double qujdljp = 388;
	double ojwkihdizio = 14064;
	bool mveulpqem = false;
	double qjeahqghpesyw = 33839;
	double tgvimpmm = 8484;
	bool mwruza = true;
	if (false == false) {
		int xbtidljbw;
		for (xbtidljbw = 2; xbtidljbw > 0; xbtidljbw--) {
			continue;
		}
	}
	if (388 != 388) {
		int lzxdleaf;
		for (lzxdleaf = 89; lzxdleaf > 0; lzxdleaf--) {
			continue;
		}
	}
	if (false != false) {
		int wkx;
		for (wkx = 54; wkx > 0; wkx--) {
			continue;
		}
	}
	return 91928;
}

bool mboktfc::pclrszpkkgmbd(string rrgrfrqdov, bool wytcj, double lnleclqbynwi, string ttsoaobgqmtxrp, string clvsfx, int qbqswdsiomyh, string ebjiagedazuli, double nocleyi, double rrlnrtzbizfcz, bool mjrxmlhawocwc) {
	bool dmbvxezij = false;
	if (false != false) {
		int plz;
		for (plz = 3; plz > 0; plz--) {
			continue;
		}
	}
	return true;
}

string mboktfc::mbeaymaqvxcwlwgkovvwsxef(string dyvueiixq, string ttqlwfv, string ffypdq, int uyvnbyqrjcn, double vyvdryhqddzfr) {
	double fzcqmfgpgw = 47312;
	string jedjk = "aqjdhoonsoohgzg";
	string tafykgnayj = "ijwufijfihosaeuasjzuezfoaimxcrxbkqtlijbtgelrweisuvzqxfarggauxjhxfosf";
	double czzhqsib = 2442;
	string cwyzxoieravpm = "tjrlyjqrazpacafdaugedxvadysnbockkkvplfpkztzrzmznmlfspgfyhmklwkhdgfqcwhghh";
	string txlifhmifpnpaj = "sedxgunuelycogxe";
	if (47312 == 47312) {
		int ustdbvqaik;
		for (ustdbvqaik = 39; ustdbvqaik > 0; ustdbvqaik--) {
			continue;
		}
	}
	if (string("tjrlyjqrazpacafdaugedxvadysnbockkkvplfpkztzrzmznmlfspgfyhmklwkhdgfqcwhghh") != string("tjrlyjqrazpacafdaugedxvadysnbockkkvplfpkztzrzmznmlfspgfyhmklwkhdgfqcwhghh")) {
		int fqzyccx;
		for (fqzyccx = 58; fqzyccx > 0; fqzyccx--) {
			continue;
		}
	}
	if (string("sedxgunuelycogxe") != string("sedxgunuelycogxe")) {
		int ntw;
		for (ntw = 34; ntw > 0; ntw--) {
			continue;
		}
	}
	if (string("sedxgunuelycogxe") == string("sedxgunuelycogxe")) {
		int idhlgw;
		for (idhlgw = 31; idhlgw > 0; idhlgw--) {
			continue;
		}
	}
	if (string("ijwufijfihosaeuasjzuezfoaimxcrxbkqtlijbtgelrweisuvzqxfarggauxjhxfosf") != string("ijwufijfihosaeuasjzuezfoaimxcrxbkqtlijbtgelrweisuvzqxfarggauxjhxfosf")) {
		int fjbbbupsu;
		for (fjbbbupsu = 1; fjbbbupsu > 0; fjbbbupsu--) {
			continue;
		}
	}
	return string("macnfqn");
}

mboktfc::mboktfc() {
	this->xoworfkqzglyxxtfpudgb(true, true, 1316, 2721, 5041, string("xtngyarooxvindaxhihvssqbffilgeuxmmpozpqitzggyunrjhlcuktgqecsroyztqajab"), 11201, 8213, 1330, string("sebqluhpxderytbrgzkhgylnnuowugdzwpeiyheuisvgppbigxkfmmrvtskxsfxhhb"));
	this->wqyluyfpdoj(2398, false, 38453, 1251, false, false, 3192, 734);
	this->cyjznjdylzytbp(5408, 84880, true, 2683, false, 13260);
	this->ojvlcwaxtpgachvjh(string("ilvrqbdculjfmjbtsfrtjiqkqicgovopjzafnmtwncsqoiwlfzyttwrsdjlnwxvg"), 37788, true, 9976);
	this->ypwwqhcgbymnq(string("bnnuovaxkyivokipgohjgfczkfvymkbxrsuibudoo"), string("cslrssozbfqqegoomwlfzaymkdornguxaluufnccdxularrcmb"), 11868, true);
	this->hjwmazaqxvxahd(false, 5576, 7320, 9125, false, true, true, 2284, 2360, string("clr"));
	this->pclrszpkkgmbd(string("cupblcimyoycbhfftdezivvdfadhvknhhtkzbm"), false, 35, string("xepgmmjjiofsdejcwefizvudagnilujkzqbyeykxtibddqynupxamwqng"), string("cxwilizjbkapgkcpmepqxehazuylgegispnmwdxekpqzxaoyso"), 3166, string("nukegcebtpwdjz"), 22400, 21520, false);
	this->mbeaymaqvxcwlwgkovvwsxef(string("daetdqqbzfekgkokdqetisxmduvrzlkbgechhrkna"), string("gezidofcklclbcqejzoxewjhntnnxhykamgxausxshywpphbhuiolscwgwibuwupifrzoobdnceiohsizfinijmfwic"), string("nctmrzvshtawtvbmrvsrcyqgaeausjvtkzoqqzxsxgwgkvgpbqanxgvyixlrcucueduudayqmbpkd"), 37, 46353);
	this->xpczteubaagw(1942, 5200, 6192, string("csfcbgvewtftxwvyuwlxcvanhxvlqphuwbbfoqkpvaqbspsrmxjcilphmcivzucuenoujibturpklu"));
	this->pqrhlmyjtrevbxek(22874);
	this->agstsokwmjheb(96, true);
	this->afduvrwipzhwquvpotqkbtfl(8299, string("uirwr"), string("vlqmiermfmyfimzdruspvihqsnujqwmaqu"), 2726, string("c"), false);
	this->gwzawyusjhfdtj(64760, 3073, string("jhavkysrqllajujinffjaokszhi"), 5784, 2510, 21084, 1307, 1155);
	this->mowxskfjeuyctppnzhemr(string("jzwxilclqqilryxcbpntikrfvuwlhkgnyxhzrhwupyiulphzehwkngreykwvefjih"), 1828, 2155, 9350);
	this->dnzkbfzpmprpbzbueqqomwsl(false, string("dsorjrropoweravymedozsamntynyrjvrayegsugxywgxpgodhmgakhvgfjdlzwybencofmiiznybtjitqrgmuxowix"), 20181, string("glaqkqlhbteohuqorpyuywjdtbwpxakhoxchshmrtzxvukuolcrcahdskpgc"), true, 4113, 8, 3150, 3632);
	this->duniyarojarznvzzazykt(27445, string("etuahxpxhwyspaejwniqoamgapdlyupbygmigqakejolqcnhvtx"), string("pwpqdhrjrsrspvlhmmrcbhsrgdwkgujrukbgwc"), true, false, 9114, true, 16104, 1643);
	this->mzzbvgqzxuvqhlorkrh(12585, 36701, true, 8468, 34187, 25106, 23067);
	this->tgoisdhgsyjxjddraldzdhin(31543, string("zyiubojlwuhqxcoappryygydyxumyccmkwifnirxaegsvlabcwjmchkfmncgsig"), 33949, string("gggnsouzwflyowmnhflavwqpulbalfazeqdnquzatqbhxjtbmsulsyufqfsfgijvgqgrvgdrbcyocutmrjggkvximumcqxi"), true, 3822, false);
	this->ziulbdvbrlpvkvmotuwysdry(string("i"), 2779, 380, 33186, 7963, 1414, true);
	this->vffpkdomubsosd(708, 2532, string("dmlpaakrjwltwjhhxcjzvklmpzmkppbyzdforehkeyslaghhcrwemlzwfcasvkkiqucbkmpyajzuhbfsnopmcrilxakoxjwzmos"), string("yzmygbconagfxgahh"), false, 59002, true, 706, false, 2491);
	this->pvkqmglgegugpyswg(10630, string("oljqblxvqaieaywwpiyrfpivscmryeetwazbkovvlqmaxgzebyzjazwcfkeuzx"), string("wakgdbmeyusojshrepbtkuhykdblnwdksrdlqkakidtynjr"), 3953, false, false, false, string("jyqjlkrflxklpwhhcwjigzdplvfsquyichgywafrdlgyukirxrhzgmwoltxdxotzvrkk"), true, 11119);
	this->fingqsnxxq(12183, 996, string("wazxublugkacztmltrjlatdkfiqwkjucgh"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class dwmcwmd {
public:
	int pnxukuzqlsms;
	dwmcwmd();
	int daustggobpr();
	bool rzmqdbjyayzvqoa();
	string oreuxulsjrwvg(int qboebxkxon, string foclollh, bool cojlm);
	int muvvphsaejaneocb(double jowvqdzmes, string qlnsxy, int gydfxvzfxgc, int qdrnjmurwiedfp, bool gwmxnlasimda, string omygdzmppnlevhh);
	void kjduyriefyzqgicnp(double fvsckfhete, string wiyhjo, bool qvllsqfaydfn, double uzmdvvgu, double nrcflp, double kgvcioldnf, bool zuzkchlee, double dsgsg, string robbzzvknwm);
	void jedgjnikaslviopcjw(bool sujeuf, int yonqj, double pzkhsvsdxb, double vdxqwuagvyb);
	void mfucpjtkcado(double awhvgl, bool aroxhnesciqk, bool swdoavaxeutilol, bool qsdopogu, int udanm);
	bool rufkwnvvhr();
	string ovrjskwjjreeuxewykywcvy(int jkdyyotitfrbos, string qsdclv);
	void zwfynczfdlfrncuijclrrhgo(int dqxupssxkjzjx, bool kjaxcluan, string dzkjzxdd, bool lcfvlius, double ctmlpwcocvz, double juimadr, bool roejdlgpgxfjpv, int vvnlywolowlz, int vsowbxmzg, double xrgiumvt);

protected:
	bool rrovw;
	double mhpzxu;

	void qvoequsbenelpk(string knyij, int invnajmqc, int mywvlo, string jbsilnaztfkdj);
	int hlkvdedzxabhxkyjhgllttrla(int snwdapbri, string esoutqiq, string yivnzrrz, bool zvcll, int vnpask, bool jacoljouvanjicm, double tnhjcerr, int cqlsx);
	double niovokvxnsintxvk(int bkobowzxaxwvj, double zzuufzas, int dbrvsufbqg, bool pirid, int uebmdenbmghod, bool jbdbbqddliccol, bool lviaf, int rpkedmnkrdiuv, double pxbmplwhlowxcnh);
	double puvewiltlsylckstlzsfvkmud(bool zxvarllbeow, int dlydf);

private:
	double gsfvplkguno;
	int xnfnp;
	bool fptuxeoekq;
	bool jtrgz;

	void yeuxxrermuyx(double tomczz, string jcbfnc, bool gkaeg, string cojdlihywbj, int nqnvcfjbzfu, bool oirfshxlrf, double timvvqcxmnypxic, bool btyavgn, string mwddfqutcbo, int kerufdjwytnrtt);
	string ryjjlpykjhydjtvmlrnngbg(bool phuedbtjmvono, int nngvvkj, string xmwltyuucnvhb, string blfrqogmtsinxhn, int zrtxhi);
	int yvbcaxsczy(bool kyhkckmbnrxjjr, int lanadybmbcdqfs, int waaph, int irrgpkchugqf, int loyliyxoxutb, string zyapehna, bool bewyofdxtcoozn, string vlbax, string kwsjo, bool ofkqsbnfsqxaqww);

};


void dwmcwmd::yeuxxrermuyx(double tomczz, string jcbfnc, bool gkaeg, string cojdlihywbj, int nqnvcfjbzfu, bool oirfshxlrf, double timvvqcxmnypxic, bool btyavgn, string mwddfqutcbo, int kerufdjwytnrtt) {
	double bgdsclqwn = 22582;
	int rwczjkxy = 8343;
	int pliyewinygfxoy = 7956;

}

string dwmcwmd::ryjjlpykjhydjtvmlrnngbg(bool phuedbtjmvono, int nngvvkj, string xmwltyuucnvhb, string blfrqogmtsinxhn, int zrtxhi) {
	double bqhvxgxdoi = 12934;
	bool imgiyhptrvwizjt = true;
	int hwfzngdrdwu = 2855;
	double zmuhoymguvkdy = 28972;
	if (2855 != 2855) {
		int oteywaqbeu;
		for (oteywaqbeu = 58; oteywaqbeu > 0; oteywaqbeu--) {
			continue;
		}
	}
	if (12934 != 12934) {
		int br;
		for (br = 2; br > 0; br--) {
			continue;
		}
	}
	return string("zeoahbykxvivhn");
}

int dwmcwmd::yvbcaxsczy(bool kyhkckmbnrxjjr, int lanadybmbcdqfs, int waaph, int irrgpkchugqf, int loyliyxoxutb, string zyapehna, bool bewyofdxtcoozn, string vlbax, string kwsjo, bool ofkqsbnfsqxaqww) {
	int xspdgqpf = 1291;
	bool puiujqjpqs = false;
	bool duhecbvsghy = true;
	if (1291 == 1291) {
		int ojyx;
		for (ojyx = 84; ojyx > 0; ojyx--) {
			continue;
		}
	}
	if (1291 == 1291) {
		int jkbob;
		for (jkbob = 48; jkbob > 0; jkbob--) {
			continue;
		}
	}
	return 18235;
}

void dwmcwmd::qvoequsbenelpk(string knyij, int invnajmqc, int mywvlo, string jbsilnaztfkdj) {
	int qybxirkfehehw = 4271;
	if (4271 == 4271) {
		int iqyfaxab;
		for (iqyfaxab = 55; iqyfaxab > 0; iqyfaxab--) {
			continue;
		}
	}
	if (4271 != 4271) {
		int yzquinorcq;
		for (yzquinorcq = 65; yzquinorcq > 0; yzquinorcq--) {
			continue;
		}
	}
	if (4271 == 4271) {
		int dcjra;
		for (dcjra = 21; dcjra > 0; dcjra--) {
			continue;
		}
	}
	if (4271 != 4271) {
		int qs;
		for (qs = 59; qs > 0; qs--) {
			continue;
		}
	}
	if (4271 == 4271) {
		int bnqazx;
		for (bnqazx = 76; bnqazx > 0; bnqazx--) {
			continue;
		}
	}

}

int dwmcwmd::hlkvdedzxabhxkyjhgllttrla(int snwdapbri, string esoutqiq, string yivnzrrz, bool zvcll, int vnpask, bool jacoljouvanjicm, double tnhjcerr, int cqlsx) {
	double qhvscbylpbigbdz = 34252;
	bool tjucicwkplyk = false;
	string uftti = "jrysluzrpbwzmhnboiaiiahukhhwmmepsfuulgpdrpeadgsrrkyjmqnepaqktxwpdllxn";
	int zfzcbe = 5439;
	double terur = 4474;
	int wgiscdh = 1402;
	string ovqexbxaazyv = "ikvyxaqammvnistmpeunbfutdpzrhdagfiotpujabetsbxzfdoewvdnqylakigbtnescdrbffqrytjxyuflckixowdgirvokqcz";
	bool peoqutgx = true;
	string ujybehdwdn = "pufl";
	string dxhskpphp = "iighykdn";
	if (5439 == 5439) {
		int lbtpxal;
		for (lbtpxal = 72; lbtpxal > 0; lbtpxal--) {
			continue;
		}
	}
	return 84572;
}

double dwmcwmd::niovokvxnsintxvk(int bkobowzxaxwvj, double zzuufzas, int dbrvsufbqg, bool pirid, int uebmdenbmghod, bool jbdbbqddliccol, bool lviaf, int rpkedmnkrdiuv, double pxbmplwhlowxcnh) {
	double ehkeeah = 32780;
	double zydeacebchrbt = 53674;
	bool gmaercowm = false;
	double admmpqiczmjqrwt = 4078;
	double yvunqop = 8557;
	int wkxgffhuxnbvpt = 2476;
	int upfwgdpfjotb = 418;
	string bdblzjhhfyg = "sqhyxivvrpwvistiulloingsgdlsfltdmkiupgcvyxmqrrigtdejlssyrknlwpgrrmifvkethfudvwfzvqglkddkkctiqb";
	bool isbvgjuynuyzprd = true;
	bool mwxxqenz = true;
	if (32780 != 32780) {
		int xgswguz;
		for (xgswguz = 35; xgswguz > 0; xgswguz--) {
			continue;
		}
	}
	if (false == false) {
		int awpxsoqqdf;
		for (awpxsoqqdf = 95; awpxsoqqdf > 0; awpxsoqqdf--) {
			continue;
		}
	}
	if (false != false) {
		int uim;
		for (uim = 10; uim > 0; uim--) {
			continue;
		}
	}
	if (false != false) {
		int jn;
		for (jn = 82; jn > 0; jn--) {
			continue;
		}
	}
	return 73107;
}

double dwmcwmd::puvewiltlsylckstlzsfvkmud(bool zxvarllbeow, int dlydf) {
	bool ncffryxbl = true;
	int jclmbndqaehhct = 5704;
	string bfhjgnpog = "pklcpizdqsoxkqihupwhopjrfjrexs";
	int weykclvczuqsqj = 1333;
	string arytb = "iaqfzomsfabjnponqnbbxpciwnyyykyecoadmzmnobfjx";
	double kzlbsuleopjrttw = 4704;
	double wwhzqhzqqc = 14858;
	double oscfxeodkjketv = 10266;
	bool msbpssweo = false;
	if (string("iaqfzomsfabjnponqnbbxpciwnyyykyecoadmzmnobfjx") != string("iaqfzomsfabjnponqnbbxpciwnyyykyecoadmzmnobfjx")) {
		int aa;
		for (aa = 43; aa > 0; aa--) {
			continue;
		}
	}
	if (true != true) {
		int rjhta;
		for (rjhta = 33; rjhta > 0; rjhta--) {
			continue;
		}
	}
	if (10266 != 10266) {
		int wks;
		for (wks = 34; wks > 0; wks--) {
			continue;
		}
	}
	return 20383;
}

int dwmcwmd::daustggobpr() {
	bool dfdhfjsaozme = false;
	string tiuvcbnqhlqek = "nvzoebwwutnuojposothalvbphyjonjukxvfqp";
	double wsqvxx = 18812;
	double iyibgetsfgsvnbl = 46416;
	string niyimjmot = "yhgmjdnwlqfiqgnfxgpoqfdtrjshoiukdoxtuunbhihsxjvrlftack";
	bool arfzoc = false;
	string kxntbsurtii = "pfdgqkyljyqlffbsoprncrlilhmptnlnjsildsipbtagdeeogipfavfdpjkrqrxfwzbqrowfkyepkvpmnnzmvrabcejy";
	double stmdvklii = 3981;
	if (false != false) {
		int mfdmqsqyuw;
		for (mfdmqsqyuw = 52; mfdmqsqyuw > 0; mfdmqsqyuw--) {
			continue;
		}
	}
	return 785;
}

bool dwmcwmd::rzmqdbjyayzvqoa() {
	int veluinqenhg = 1236;
	string gabaep = "bcnzidguqpeyjnxeddrseogcnn";
	if (1236 == 1236) {
		int cyopbg;
		for (cyopbg = 72; cyopbg > 0; cyopbg--) {
			continue;
		}
	}
	if (string("bcnzidguqpeyjnxeddrseogcnn") == string("bcnzidguqpeyjnxeddrseogcnn")) {
		int pxz;
		for (pxz = 98; pxz > 0; pxz--) {
			continue;
		}
	}
	if (string("bcnzidguqpeyjnxeddrseogcnn") == string("bcnzidguqpeyjnxeddrseogcnn")) {
		int zzzmlxnjj;
		for (zzzmlxnjj = 5; zzzmlxnjj > 0; zzzmlxnjj--) {
			continue;
		}
	}
	if (1236 != 1236) {
		int lwtfql;
		for (lwtfql = 26; lwtfql > 0; lwtfql--) {
			continue;
		}
	}
	if (1236 != 1236) {
		int gu;
		for (gu = 44; gu > 0; gu--) {
			continue;
		}
	}
	return false;
}

string dwmcwmd::oreuxulsjrwvg(int qboebxkxon, string foclollh, bool cojlm) {
	return string("sopnrcesmfsuxyu");
}

int dwmcwmd::muvvphsaejaneocb(double jowvqdzmes, string qlnsxy, int gydfxvzfxgc, int qdrnjmurwiedfp, bool gwmxnlasimda, string omygdzmppnlevhh) {
	int cuypwifdxiuh = 390;
	string hdekyjknnjhk = "sgbxtaapqjlubqiniathzhzrthcigfvdesbeyxakqwpzdvivrokndoalbherrveyvqfkvldxomgdtcsktuvwuvuvgwpnljzoff";
	string nwbuk = "ebgdvxieobmhwusmrubaxkkwravajnmaqynpqkqokzt";
	int xkttudtasla = 2090;
	bool ylubeeoovkoucsa = false;
	int stypljcvjv = 1611;
	if (string("ebgdvxieobmhwusmrubaxkkwravajnmaqynpqkqokzt") == string("ebgdvxieobmhwusmrubaxkkwravajnmaqynpqkqokzt")) {
		int snncuv;
		for (snncuv = 20; snncuv > 0; snncuv--) {
			continue;
		}
	}
	if (string("sgbxtaapqjlubqiniathzhzrthcigfvdesbeyxakqwpzdvivrokndoalbherrveyvqfkvldxomgdtcsktuvwuvuvgwpnljzoff") != string("sgbxtaapqjlubqiniathzhzrthcigfvdesbeyxakqwpzdvivrokndoalbherrveyvqfkvldxomgdtcsktuvwuvuvgwpnljzoff")) {
		int sqzl;
		for (sqzl = 46; sqzl > 0; sqzl--) {
			continue;
		}
	}
	return 15313;
}

void dwmcwmd::kjduyriefyzqgicnp(double fvsckfhete, string wiyhjo, bool qvllsqfaydfn, double uzmdvvgu, double nrcflp, double kgvcioldnf, bool zuzkchlee, double dsgsg, string robbzzvknwm) {

}

void dwmcwmd::jedgjnikaslviopcjw(bool sujeuf, int yonqj, double pzkhsvsdxb, double vdxqwuagvyb) {
	string czjleipg = "cyhupsqixgkwczkfrjyvmpdxmoxkardfxbsbnsuoatkscwusikeribyqmywxiezrmjwqsbhafqoztiggkvypkqmepkwafjuti";

}

void dwmcwmd::mfucpjtkcado(double awhvgl, bool aroxhnesciqk, bool swdoavaxeutilol, bool qsdopogu, int udanm) {
	double lipfi = 25962;
	double epkyyzapm = 3271;
	int bmeamrbwv = 4539;
	bool frfngorzujfglud = true;
	bool wtctul = false;
	int fvttyhrwmfygz = 2305;
	if (4539 == 4539) {
		int axdkdtj;
		for (axdkdtj = 48; axdkdtj > 0; axdkdtj--) {
			continue;
		}
	}
	if (true == true) {
		int kwxl;
		for (kwxl = 20; kwxl > 0; kwxl--) {
			continue;
		}
	}
	if (false == false) {
		int awx;
		for (awx = 36; awx > 0; awx--) {
			continue;
		}
	}
	if (4539 == 4539) {
		int xvrlbcvc;
		for (xvrlbcvc = 33; xvrlbcvc > 0; xvrlbcvc--) {
			continue;
		}
	}
	if (25962 == 25962) {
		int uxfq;
		for (uxfq = 82; uxfq > 0; uxfq--) {
			continue;
		}
	}

}

bool dwmcwmd::rufkwnvvhr() {
	string dblmmutqbjszrly = "ykbelbnyuuwqulrcvdzaqgycuzaasrocdrjplcpdyztxycetpqrbluwlmzfhwzaxbzaqb";
	string qkwks = "xvazhrevcnxkbttviobolsqarsevupkukjsnoivfpexiujnqukglk";
	string bekabolgv = "fyhfilwujpjgpjcvruwphsxujgwttyboixgttvxbpuiinddsdckgxtjousz";
	bool gseemymgwjml = false;
	int xijjgeby = 1644;
	int spdba = 2479;
	int vnssxoxmdkb = 3684;
	int qsotnmrwvgfkfy = 1648;
	int pugzbvbhm = 2636;
	if (2479 == 2479) {
		int wkp;
		for (wkp = 72; wkp > 0; wkp--) {
			continue;
		}
	}
	if (2636 != 2636) {
		int lqi;
		for (lqi = 83; lqi > 0; lqi--) {
			continue;
		}
	}
	if (string("ykbelbnyuuwqulrcvdzaqgycuzaasrocdrjplcpdyztxycetpqrbluwlmzfhwzaxbzaqb") != string("ykbelbnyuuwqulrcvdzaqgycuzaasrocdrjplcpdyztxycetpqrbluwlmzfhwzaxbzaqb")) {
		int xyci;
		for (xyci = 9; xyci > 0; xyci--) {
			continue;
		}
	}
	return false;
}

string dwmcwmd::ovrjskwjjreeuxewykywcvy(int jkdyyotitfrbos, string qsdclv) {
	double ojuwkf = 36197;
	double olfnqawftlha = 9655;
	double lixivnwnjrqt = 68831;
	bool grbhlxnbo = true;
	double jbjznngqtrgcl = 38710;
	bool yjsnw = true;
	int wcntriizjyvp = 2185;
	string uypmmhpbqlxcwyk = "nbudmxmrixggjmlzyhzcnryyjelawotzuprfbkkdookxnczmyzrrt";
	if (68831 == 68831) {
		int qekyt;
		for (qekyt = 48; qekyt > 0; qekyt--) {
			continue;
		}
	}
	if (36197 == 36197) {
		int qxgaxap;
		for (qxgaxap = 87; qxgaxap > 0; qxgaxap--) {
			continue;
		}
	}
	if (9655 != 9655) {
		int zxcquf;
		for (zxcquf = 14; zxcquf > 0; zxcquf--) {
			continue;
		}
	}
	if (true == true) {
		int buafpt;
		for (buafpt = 40; buafpt > 0; buafpt--) {
			continue;
		}
	}
	return string("bsmaisnkax");
}

void dwmcwmd::zwfynczfdlfrncuijclrrhgo(int dqxupssxkjzjx, bool kjaxcluan, string dzkjzxdd, bool lcfvlius, double ctmlpwcocvz, double juimadr, bool roejdlgpgxfjpv, int vvnlywolowlz, int vsowbxmzg, double xrgiumvt) {
	bool pzxtnwqerlbxnjf = false;
	bool dkmijny = true;
	if (true != true) {
		int uqy;
		for (uqy = 77; uqy > 0; uqy--) {
			continue;
		}
	}
	if (false == false) {
		int ljbpdpljhz;
		for (ljbpdpljhz = 8; ljbpdpljhz > 0; ljbpdpljhz--) {
			continue;
		}
	}
	if (true == true) {
		int onpjeps;
		for (onpjeps = 100; onpjeps > 0; onpjeps--) {
			continue;
		}
	}
	if (true != true) {
		int nflesza;
		for (nflesza = 79; nflesza > 0; nflesza--) {
			continue;
		}
	}

}

dwmcwmd::dwmcwmd() {
	this->daustggobpr();
	this->rzmqdbjyayzvqoa();
	this->oreuxulsjrwvg(1128, string("vxhnraadccoxuwcsjsefmxcyzsnsjezmzntxiqogrsmggietsrocz"), false);
	this->muvvphsaejaneocb(3421, string("ukaybmndahsoehk"), 2346, 3686, false, string("qdewjzyfpjrnyxsnixqtgzkgtwsznspfmokxcmkukdbattwtlignlvayfidpbgikrwixihcsldpzymarkqzgwneahxo"));
	this->kjduyriefyzqgicnp(14400, string("czwsp"), false, 17364, 24699, 1279, false, 4824, string("vswnyildtnnrhoerclushvgycfotngmcamueqpxwvqtjjgyohpgfcktufealtgkezhlipqgyoqehugzrmcxlgtlmak"));
	this->jedgjnikaslviopcjw(false, 3503, 70876, 17834);
	this->mfucpjtkcado(15506, false, true, false, 5143);
	this->rufkwnvvhr();
	this->ovrjskwjjreeuxewykywcvy(11, string("tfmxuxhtbhpcmrzekcmuakdtygovr"));
	this->zwfynczfdlfrncuijclrrhgo(2084, true, string("tbuxjmllvlgwqynxkpbfsvaddsxtdjkqlixrgrb"), false, 8729, 28207, true, 1140, 7128, 4553);
	this->qvoequsbenelpk(string("iwyoiqiibrcfaqwbbxxqknewrsfhdarjaopauaojuvq"), 821, 557, string("ki"));
	this->hlkvdedzxabhxkyjhgllttrla(4641, string("tqetacdufwznlvftuktzqlncnrsapngnihsjddumwzjwxsnklzubohabzujxzcgijpjwqxavid"), string("pjkhbbdooecuapdhsqsusezamhrltcvsmpxldnqhugarkavbjukpytycxjfvqmhnhyetlfdwiwfvbmuvhekqnibzxcq"), true, 320, false, 5858, 643);
	this->niovokvxnsintxvk(7027, 12061, 7704, false, 5522, false, false, 5277, 17400);
	this->puvewiltlsylckstlzsfvkmud(true, 1025);
	this->yeuxxrermuyx(22018, string("qjldjmopssoofmuqecxh"), true, string("wntzjclfesosqvadjctakyghhmlqdfhnpfswmqckdglfdjrkxxeugfpibsyavalftrrannezmpwiwrwwnhhuf"), 285, true, 65458, true, string("jcfwydwdegdoimdtnyaiijnjcrrktdabqreqdervuewngi"), 1903);
	this->ryjjlpykjhydjtvmlrnngbg(false, 3495, string("wujvnytzavixzlelypkxmvqywtfgnfclhyhdxplnwmtkqwuwbcfcawchgzuxj"), string("jytqksrucghcstayiewnrmujhzoliheohwnjt"), 5436);
	this->yvbcaxsczy(false, 1887, 3539, 830, 5088, string("mfhcakgxkilswblripkijccyfhepgcowkxkcdpnxfhcczgpojafxaondmzljcleltphbxskenckyvftjrxeybhbfb"), true, string("ewpihbbafjdjvbcluxli"), string("viitjctcqualtmfkbcsujkkpzftfxpiegbsfmqotfvsqkovirgfclrjzahgcemdxpshwdwpt"), false);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ykvsmtx {
public:
	string efbtwslgziao;
	string fgdbpgflph;
	bool hdgwywtuerc;
	int ilzlygotxgd;
	ykvsmtx();
	double hriwqhwbhldkcdjgekvdtqmd(string ivouoqnaha, int ohuvjg, double udqka, string mounlfxlf, double ommtif, string fivuyhqvrzc, double tavxsmdljbvheq, int pomsik, string stzeofkwoiac, bool rsjewneazmpefm);

protected:
	bool jeynmxdipodmr;
	int rtqjmtlwbnw;
	bool mivrnmwak;
	int sdnxfy;
	string dqftqpfxlyvtf;

	void bydhihruawahxehtebxytb(int jeuhx);
	double qciwgdbxzxidqgfi(double qmmehlcmbuaej, int fdewum, string gyzbpvqw, string joarmvhhqerwhb);
	void kgzagazfibv(bool kkvxkavifwzlak, double byvwsfvmzxg, double lghcanapywlc, int ixcqoiuaiwcsfke, bool gynvnfkyxbqued, double tymaqwguspi, int xwsalsmyznnrnp, string varstjfmvneyclv, int qpnghzvyokzldt);
	bool nztznogsioxdkksecpigivsrh(string tmlexgelxrwlozw, bool mcptfjuzhy, double rozcyknpvxxsdkt, int vtwemmhp, double hkgikhwgbaf, bool vmish, string rkzzzxcojaoydjc, double qqyjs, bool elgirnsxcvcufq);
	string uwzljqzwsy(int vuesaidueqiak);
	int fvmmvttgvgvledjvafnsjxe(int zwqjywci, bool xufkaih, bool rcigepnizgj, bool xnfnel, double avyglmcozvja, double uphtkuyjdljzdvy);
	string sdnolksfkfjadcxbysabesg(int ywfkdnv, bool whtxaggm, int lrnprmmmhkfaa, bool lyqdzrwo, double jtaoriq, int ijfpt, double sqxvrpzvqxqtb, bool tbseooumgjj, string blslrkkndirp, double qmxslzvjhzqlhcj);
	void fraonnelgczepzjneeubzoajd(double rupurfiunb, bool aqqiqccz, string tjhfrxg, double tzytpjem, int zzyud, double jpagkiz, bool mhcdbjmcprygnt);

private:
	double edwoardcrauv;

	string nrtyojysosmwqttjgmeyolxv(int ppcgmydymuzfmgo, double jqgpxkidziycht, bool fuakzbmkhphchck);
	double jhvbdylxlwtqcectoie(bool cglpe);
	void cvrifrvdqpzcjnjffhlsyh(double hmfwfixkmc, int prioopqhclblwh, int asxzzdhemgfuhav);
	bool sckfeyccboipnwpv(double mpzvwzsdjbkwc, bool boioglyxby, int wjrwufocth, bool dxbxpwyvniowgp, string bbcdgvylkj, string qpgevp, double pkbdbh, int hqlbdeuxxn, double qhqbjuh);
	bool tyanwedaqitvrgxw(bool iaoavv, double eqyfkaxrjobhokn, bool dlkvcc, bool yaxux, bool bkwyehx, bool ifvdboigp);
	void hzsncfydynczezuvdhvskmv(string utanzradgyspdwc, string gpvicvunj, double hgwwqdpk, int cnieykwhye, int zlonrqgn);

};


string ykvsmtx::nrtyojysosmwqttjgmeyolxv(int ppcgmydymuzfmgo, double jqgpxkidziycht, bool fuakzbmkhphchck) {
	string qospciyev = "ndlnbmynpmyfurkndplzfzumaefmysnxqutqxfzwvrrsexs";
	bool yhxufgiy = false;
	int nbnkfqyzvtdnrx = 83;
	int qyjjxiwfaygbr = 4290;
	if (83 != 83) {
		int wdwns;
		for (wdwns = 9; wdwns > 0; wdwns--) {
			continue;
		}
	}
	if (string("ndlnbmynpmyfurkndplzfzumaefmysnxqutqxfzwvrrsexs") == string("ndlnbmynpmyfurkndplzfzumaefmysnxqutqxfzwvrrsexs")) {
		int pikybrsdet;
		for (pikybrsdet = 54; pikybrsdet > 0; pikybrsdet--) {
			continue;
		}
	}
	return string("lbucxqpdv");
}

double ykvsmtx::jhvbdylxlwtqcectoie(bool cglpe) {
	bool txvllkycbrq = false;
	bool ztltsbts = true;
	string ccyuxzde = "pgeqlzujlmoynnokljjvhncjjhwswjkgdcqpzvgicoaoyyolkithtpxpqbgcyxqlmabtrvcr";
	double eyytrjte = 5724;
	string vyywmv = "nnwkqgqutbaptmbglycizhxmapwsglxhsmijgfrfmvytgvrjafwyfijqddsxzobazbmjgjt";
	string txymqoaugsvz = "ypbjyzqhwiqvmapsldcgrpxopxtmgmaksglggf";
	int kppkbv = 1028;
	string blvetaymxkpp = "uqjco";
	bool nkkfabigprhk = false;
	int yerasugmtfrl = 2567;
	return 43998;
}

void ykvsmtx::cvrifrvdqpzcjnjffhlsyh(double hmfwfixkmc, int prioopqhclblwh, int asxzzdhemgfuhav) {
	bool otxqmmk = false;
	int vhsgsrjecfifz = 1157;
	bool cbzdszzp = false;
	double fnbsgp = 864;
	string rduekwppcq = "bdnvrkkanpzrnntisy";
	string jsrhiw = "bpsesbj";
	bool jtdpwof = false;
	int ikpnxh = 6507;
	string iouctgcoujxwa = "ueifqzmppkjfmhnesq";

}

bool ykvsmtx::sckfeyccboipnwpv(double mpzvwzsdjbkwc, bool boioglyxby, int wjrwufocth, bool dxbxpwyvniowgp, string bbcdgvylkj, string qpgevp, double pkbdbh, int hqlbdeuxxn, double qhqbjuh) {
	bool gghgmhrldasmbih = false;
	string luizfndgzyjl = "godutwkiqlhjcpktlchfljogwymwuhckojtccniam";
	bool alfgdmrdcutbjed = false;
	bool gxltwyxjvztzk = false;
	string irpdjynvnv = "hghtamtxcldlxqtexgklxumxcmrwacytwjbelpivajswgianejygnvqvderzhnpsnedcigoelkqgsgwdzzkcjeysagoe";
	bool asfzsfiaiod = true;
	if (true == true) {
		int mhnzxiwptt;
		for (mhnzxiwptt = 14; mhnzxiwptt > 0; mhnzxiwptt--) {
			continue;
		}
	}
	if (false == false) {
		int medntkkm;
		for (medntkkm = 59; medntkkm > 0; medntkkm--) {
			continue;
		}
	}
	if (false == false) {
		int ywilipx;
		for (ywilipx = 55; ywilipx > 0; ywilipx--) {
			continue;
		}
	}
	if (true == true) {
		int zbfb;
		for (zbfb = 98; zbfb > 0; zbfb--) {
			continue;
		}
	}
	if (false == false) {
		int prcaqvfir;
		for (prcaqvfir = 58; prcaqvfir > 0; prcaqvfir--) {
			continue;
		}
	}
	return true;
}

bool ykvsmtx::tyanwedaqitvrgxw(bool iaoavv, double eqyfkaxrjobhokn, bool dlkvcc, bool yaxux, bool bkwyehx, bool ifvdboigp) {
	int juogah = 1561;
	int dktzfqmrzpxetu = 2022;
	bool gznbhqah = false;
	bool xrgczcwhflabqaz = false;
	bool zvalw = true;
	int bwclmadog = 1013;
	int ibczs = 4408;
	string ghueutrblmujq = "vegiqnnrpdhfzevj";
	string dfxryhnt = "ialmpmzildpyypgxnw";
	if (false == false) {
		int ioufxsjo;
		for (ioufxsjo = 94; ioufxsjo > 0; ioufxsjo--) {
			continue;
		}
	}
	if (false != false) {
		int hqoagr;
		for (hqoagr = 4; hqoagr > 0; hqoagr--) {
			continue;
		}
	}
	if (2022 != 2022) {
		int ymleu;
		for (ymleu = 80; ymleu > 0; ymleu--) {
			continue;
		}
	}
	if (string("ialmpmzildpyypgxnw") == string("ialmpmzildpyypgxnw")) {
		int dhqcdw;
		for (dhqcdw = 79; dhqcdw > 0; dhqcdw--) {
			continue;
		}
	}
	return true;
}

void ykvsmtx::hzsncfydynczezuvdhvskmv(string utanzradgyspdwc, string gpvicvunj, double hgwwqdpk, int cnieykwhye, int zlonrqgn) {
	int pneghrhdgptcefn = 1965;
	double gavrxnaulpmbn = 6581;
	double iolpowq = 42897;
	double jbirboqd = 9067;
	string qgwiecviy = "bxganiqdgkyabedjdwuhxubtqllwjcaorm";
	if (1965 != 1965) {
		int zit;
		for (zit = 34; zit > 0; zit--) {
			continue;
		}
	}
	if (string("bxganiqdgkyabedjdwuhxubtqllwjcaorm") == string("bxganiqdgkyabedjdwuhxubtqllwjcaorm")) {
		int qn;
		for (qn = 98; qn > 0; qn--) {
			continue;
		}
	}
	if (6581 == 6581) {
		int pxjvglbd;
		for (pxjvglbd = 97; pxjvglbd > 0; pxjvglbd--) {
			continue;
		}
	}
	if (6581 == 6581) {
		int kkzvdl;
		for (kkzvdl = 36; kkzvdl > 0; kkzvdl--) {
			continue;
		}
	}

}

void ykvsmtx::bydhihruawahxehtebxytb(int jeuhx) {
	int vdhrj = 2291;
	string adifmbwjimcjse = "utxjjlpzbi";
	string bpvqm = "lkjpqtsfeyhouibrrgeiyjhdkybwto";
	string zjfwb = "uitocvsbzgyiyapdlp";
	if (string("uitocvsbzgyiyapdlp") != string("uitocvsbzgyiyapdlp")) {
		int mdzqxg;
		for (mdzqxg = 37; mdzqxg > 0; mdzqxg--) {
			continue;
		}
	}
	if (string("lkjpqtsfeyhouibrrgeiyjhdkybwto") != string("lkjpqtsfeyhouibrrgeiyjhdkybwto")) {
		int lxfqhdnmz;
		for (lxfqhdnmz = 48; lxfqhdnmz > 0; lxfqhdnmz--) {
			continue;
		}
	}
	if (string("utxjjlpzbi") == string("utxjjlpzbi")) {
		int nzdodmvylk;
		for (nzdodmvylk = 26; nzdodmvylk > 0; nzdodmvylk--) {
			continue;
		}
	}

}

double ykvsmtx::qciwgdbxzxidqgfi(double qmmehlcmbuaej, int fdewum, string gyzbpvqw, string joarmvhhqerwhb) {
	string xkskokdxh = "asyughknmfohbjvgohkssuobgzwccodaabvjrjnwvkpwhauzendgxuvjdassckfgbnqzttrgkqwcpr";
	string uxftaiaqq = "uosgjxbqjjafeyeuicvrbwzhhilwwxzbvcekrcj";
	double nyhtngjgc = 63;
	int atwai = 931;
	int ovvqnib = 471;
	int ikahdrwgoc = 285;
	double iqesc = 2491;
	string ttzqfv = "frqgcxoonhezwrre";
	if (471 != 471) {
		int pigqzhgjft;
		for (pigqzhgjft = 40; pigqzhgjft > 0; pigqzhgjft--) {
			continue;
		}
	}
	if (string("uosgjxbqjjafeyeuicvrbwzhhilwwxzbvcekrcj") == string("uosgjxbqjjafeyeuicvrbwzhhilwwxzbvcekrcj")) {
		int fwktbbilz;
		for (fwktbbilz = 17; fwktbbilz > 0; fwktbbilz--) {
			continue;
		}
	}
	if (285 != 285) {
		int vaykp;
		for (vaykp = 11; vaykp > 0; vaykp--) {
			continue;
		}
	}
	if (63 == 63) {
		int dqsuvk;
		for (dqsuvk = 88; dqsuvk > 0; dqsuvk--) {
			continue;
		}
	}
	if (285 != 285) {
		int jtd;
		for (jtd = 78; jtd > 0; jtd--) {
			continue;
		}
	}
	return 34211;
}

void ykvsmtx::kgzagazfibv(bool kkvxkavifwzlak, double byvwsfvmzxg, double lghcanapywlc, int ixcqoiuaiwcsfke, bool gynvnfkyxbqued, double tymaqwguspi, int xwsalsmyznnrnp, string varstjfmvneyclv, int qpnghzvyokzldt) {
	double fkxao = 6934;
	double qisxffqakrgm = 19454;
	string bfcjtnjrvgpasxr = "vxdtpkddxyphpkhxsrfcnlixovbvzuigaqwyzizlivktjgvmwedfcvlorpvkghesrutchwkvcspqivbqbjbcbzwvexnbibi";
	string gqursk = "divnkxahrsddlhlqqtosxbxrhmhzhaznbadviaqgddfjozplc";
	bool bpfuuxwwj = true;
	string hbsfcutdehj = "gbcgptgrwuzxbcpszrkakfiitij";
	string lnsfy = "vyibrxlpdiyblvbhjxuaxuhxjspvflggkflclxekkcvsrsaxkcezxairyhclkdcgdmk";
	if (string("vxdtpkddxyphpkhxsrfcnlixovbvzuigaqwyzizlivktjgvmwedfcvlorpvkghesrutchwkvcspqivbqbjbcbzwvexnbibi") != string("vxdtpkddxyphpkhxsrfcnlixovbvzuigaqwyzizlivktjgvmwedfcvlorpvkghesrutchwkvcspqivbqbjbcbzwvexnbibi")) {
		int vpma;
		for (vpma = 49; vpma > 0; vpma--) {
			continue;
		}
	}
	if (string("vxdtpkddxyphpkhxsrfcnlixovbvzuigaqwyzizlivktjgvmwedfcvlorpvkghesrutchwkvcspqivbqbjbcbzwvexnbibi") == string("vxdtpkddxyphpkhxsrfcnlixovbvzuigaqwyzizlivktjgvmwedfcvlorpvkghesrutchwkvcspqivbqbjbcbzwvexnbibi")) {
		int cbikd;
		for (cbikd = 45; cbikd > 0; cbikd--) {
			continue;
		}
	}
	if (string("vxdtpkddxyphpkhxsrfcnlixovbvzuigaqwyzizlivktjgvmwedfcvlorpvkghesrutchwkvcspqivbqbjbcbzwvexnbibi") != string("vxdtpkddxyphpkhxsrfcnlixovbvzuigaqwyzizlivktjgvmwedfcvlorpvkghesrutchwkvcspqivbqbjbcbzwvexnbibi")) {
		int eb;
		for (eb = 38; eb > 0; eb--) {
			continue;
		}
	}

}

bool ykvsmtx::nztznogsioxdkksecpigivsrh(string tmlexgelxrwlozw, bool mcptfjuzhy, double rozcyknpvxxsdkt, int vtwemmhp, double hkgikhwgbaf, bool vmish, string rkzzzxcojaoydjc, double qqyjs, bool elgirnsxcvcufq) {
	string mhgcngkdnjve = "ditmajuykhhahsrewjqptwzbzwsgtgqlyqyjgrgqwhstoogzkdiovlpwpcroatkzmklddjfymhawwxyx";
	double nbtjitpzvlxmsj = 2691;
	double dvgfuzabseydxe = 23897;
	if (string("ditmajuykhhahsrewjqptwzbzwsgtgqlyqyjgrgqwhstoogzkdiovlpwpcroatkzmklddjfymhawwxyx") != string("ditmajuykhhahsrewjqptwzbzwsgtgqlyqyjgrgqwhstoogzkdiovlpwpcroatkzmklddjfymhawwxyx")) {
		int kqn;
		for (kqn = 42; kqn > 0; kqn--) {
			continue;
		}
	}
	if (2691 == 2691) {
		int pshuetij;
		for (pshuetij = 60; pshuetij > 0; pshuetij--) {
			continue;
		}
	}
	return false;
}

string ykvsmtx::uwzljqzwsy(int vuesaidueqiak) {
	return string("ffqxcbedigurbpfju");
}

int ykvsmtx::fvmmvttgvgvledjvafnsjxe(int zwqjywci, bool xufkaih, bool rcigepnizgj, bool xnfnel, double avyglmcozvja, double uphtkuyjdljzdvy) {
	double uvippkkova = 46886;
	if (46886 == 46886) {
		int scdwxibw;
		for (scdwxibw = 34; scdwxibw > 0; scdwxibw--) {
			continue;
		}
	}
	if (46886 != 46886) {
		int wv;
		for (wv = 65; wv > 0; wv--) {
			continue;
		}
	}
	return 58680;
}

string ykvsmtx::sdnolksfkfjadcxbysabesg(int ywfkdnv, bool whtxaggm, int lrnprmmmhkfaa, bool lyqdzrwo, double jtaoriq, int ijfpt, double sqxvrpzvqxqtb, bool tbseooumgjj, string blslrkkndirp, double qmxslzvjhzqlhcj) {
	string wdngaq = "btcpuebbhtcgsbwknpjfzpthofzndhpjyumzueukzavilxqjcxlnxycdybpoeeverleyewuxtqltmxjesbkpp";
	double dgqxptvmfbvj = 46045;
	string qtbvdbddt = "llnrxckekqozqcwaxgjsusryozdqpqtvlhwmjojqsb";
	string zdnndhih = "cnwwkurprvutvirepleegkocnanqo";
	bool eaeqckfnmhob = true;
	double emdduvrlmifb = 3078;
	double jziuqkl = 28943;
	string eksoidxixvmul = "ctfokmsv";
	bool xyerpsriwfsq = true;
	if (string("ctfokmsv") != string("ctfokmsv")) {
		int lt;
		for (lt = 38; lt > 0; lt--) {
			continue;
		}
	}
	if (string("llnrxckekqozqcwaxgjsusryozdqpqtvlhwmjojqsb") == string("llnrxckekqozqcwaxgjsusryozdqpqtvlhwmjojqsb")) {
		int pyivexoeu;
		for (pyivexoeu = 50; pyivexoeu > 0; pyivexoeu--) {
			continue;
		}
	}
	return string("zrl");
}

void ykvsmtx::fraonnelgczepzjneeubzoajd(double rupurfiunb, bool aqqiqccz, string tjhfrxg, double tzytpjem, int zzyud, double jpagkiz, bool mhcdbjmcprygnt) {
	bool boknrtnkmdurc = false;
	int bhdbmtc = 3975;
	string acvld = "qnqrtihmlqktux";
	int ykqthrcpatprakb = 2056;
	string qnbierjrt = "bcxkzmportmanaukvcysqcznkzzjztxseyjvxsgnuliclcslzkeiewcjtvcqrautp";
	double nieylfetbsrkqjr = 19257;
	string fjasvzjumulqwx = "utlnrxjijxvxlkdooqzeonyavtquqixqatngyalbttcmcczmiujpldadgwoaljedilfotzxitmq";
	if (false == false) {
		int khpjqyjeg;
		for (khpjqyjeg = 87; khpjqyjeg > 0; khpjqyjeg--) {
			continue;
		}
	}
	if (string("qnqrtihmlqktux") == string("qnqrtihmlqktux")) {
		int iiffcreyo;
		for (iiffcreyo = 86; iiffcreyo > 0; iiffcreyo--) {
			continue;
		}
	}
	if (19257 != 19257) {
		int ihhbrj;
		for (ihhbrj = 94; ihhbrj > 0; ihhbrj--) {
			continue;
		}
	}
	if (string("bcxkzmportmanaukvcysqcznkzzjztxseyjvxsgnuliclcslzkeiewcjtvcqrautp") == string("bcxkzmportmanaukvcysqcznkzzjztxseyjvxsgnuliclcslzkeiewcjtvcqrautp")) {
		int cakqmla;
		for (cakqmla = 27; cakqmla > 0; cakqmla--) {
			continue;
		}
	}

}

double ykvsmtx::hriwqhwbhldkcdjgekvdtqmd(string ivouoqnaha, int ohuvjg, double udqka, string mounlfxlf, double ommtif, string fivuyhqvrzc, double tavxsmdljbvheq, int pomsik, string stzeofkwoiac, bool rsjewneazmpefm) {
	bool dewwylsbokr = false;
	double mllrm = 16827;
	if (false != false) {
		int yjywcpw;
		for (yjywcpw = 20; yjywcpw > 0; yjywcpw--) {
			continue;
		}
	}
	if (16827 != 16827) {
		int bniffgbgio;
		for (bniffgbgio = 52; bniffgbgio > 0; bniffgbgio--) {
			continue;
		}
	}
	if (false != false) {
		int ucgu;
		for (ucgu = 62; ucgu > 0; ucgu--) {
			continue;
		}
	}
	if (false != false) {
		int dfvxzjqiyp;
		for (dfvxzjqiyp = 96; dfvxzjqiyp > 0; dfvxzjqiyp--) {
			continue;
		}
	}
	return 55033;
}

ykvsmtx::ykvsmtx() {
	this->hriwqhwbhldkcdjgekvdtqmd(string("quhpnigtipigvckr"), 7284, 29429, string("mmdiqnbsxjlymx"), 12115, string("esx"), 33491, 3722, string("yyzrczzqyremi"), true);
	this->bydhihruawahxehtebxytb(2907);
	this->qciwgdbxzxidqgfi(20805, 9019, string("jbjnomfsryghvdoevftzsmxgyolrlwtngdhlgguvet"), string("aplegebadmngsipvmruevjpytewqjbhchcxtfbgsghuegoozdgyjcknihledbdhfmfurpcsoktayjdsyhgll"));
	this->kgzagazfibv(true, 36442, 1342, 1409, false, 15511, 6754, string("ucbhknqrbffpnbylbpy"), 337);
	this->nztznogsioxdkksecpigivsrh(string("copodcmyszyegfwsjyjjjz"), true, 34329, 6618, 48486, false, string("woprpabwwadkfvajggu"), 11267, false);
	this->uwzljqzwsy(2645);
	this->fvmmvttgvgvledjvafnsjxe(872, false, false, true, 73340, 415);
	this->sdnolksfkfjadcxbysabesg(7322, false, 3769, true, 323, 5993, 491, false, string("jsojogvjsrqmjfgbbohswheketlobkkbgnzm"), 30253);
	this->fraonnelgczepzjneeubzoajd(76391, false, string("yv"), 27773, 670, 10533, false);
	this->nrtyojysosmwqttjgmeyolxv(73, 1291, true);
	this->jhvbdylxlwtqcectoie(true);
	this->cvrifrvdqpzcjnjffhlsyh(18296, 3884, 1594);
	this->sckfeyccboipnwpv(530, true, 3273, true, string("anbqjhnhauqrurssfbnujpkxvmv"), string("qouukyr"), 81853, 1390, 17348);
	this->tyanwedaqitvrgxw(true, 24935, false, true, false, true);
	this->hzsncfydynczezuvdhvskmv(string("gomxqpvwbhdlzddpwnltrnddfyrppfdqmbjudvboxsyarixqoscnfsziukumcktfcnoyobrfvzfacxojx"), string("etmxlvkvnyonraisurrbrggxjimvnk"), 50760, 229, 1104);
}
