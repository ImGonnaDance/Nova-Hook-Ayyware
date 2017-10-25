#include "Resolver.h"
int Globals::Shots = 0;
int Globals::getShotsFired = 0;
bool Globals::change;

int Globals::TargetID;
IClientEntity* Globals::Target;

#include "Backtrack.h"
#define TICK_INTERVAL           ( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )     ( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )      ( TICK_INTERVAL *( t ) )




void ayyResolver::Init() {


}

float tolerance = 10.f;

float Normalize(float delta) {

	while (delta <= -180) delta += 360;
	while (delta > 180) delta -= 360;

	return delta;
}

const inline float GetDelta(float a, float b) {
	return abs(Normalize(a - b));


}

const inline float LBYDelta(const CTickRecord& v) {
	return v.m_angEyeAngles.y - v.m_flLowerBodyYawTarget;
}

const inline bool IsDifferent(float a, float b, float tolerance = 10.f) {
	return (GetDelta(a, b) > tolerance);
}

void ayyResolver::StoreVars(IClientEntity * pEntity) {
	
	if (m_arrInfos.at(pEntity->GetIndex()).m_sRecords.size() >= 10) {
		m_arrInfos.at(pEntity->GetIndex()).m_sRecords.pop_back();
	}

	m_arrInfos.at(pEntity->GetIndex()).m_sRecords.push_front(CTickRecord(pEntity));
}



bool ayyResolver::HasStaticRealAngle(const std::deque<CTickRecord>& l, float tolerance) {
	auto minmax = std::minmax_element(std::begin(l), std::end(l), [](const CTickRecord& t1, const CTickRecord& t2) { return t1.m_flLowerBodyYawTarget < t2.m_flLowerBodyYawTarget; });
	return (fabs(minmax.first->m_flLowerBodyYawTarget - minmax.second->m_flLowerBodyYawTarget) <= tolerance);
}

bool ayyResolver::HasStaticYawDifference(const std::deque<CTickRecord>& l, float tolerance) {
	for (auto i = l.begin(); i < l.end() - 1;)
	{
		if (GetDelta(LBYDelta(*i), LBYDelta(*++i)) > tolerance)
			return false;
	}
	return true;
}

bool ayyResolver::HasSteadyDifference(const std::deque<CTickRecord>& l, float tolerance, IClientEntity* pEnt) {
	size_t misses = 0;
	for (size_t i = 0; i < l.size() - 1; i++) {
		float tickdif = static_cast<float>(l.at(i).m_flSimulationTime - l.at(i + 1).tickcount);
		float lbydif = GetDelta(l.at(i).m_flLowerBodyYawTarget, l.at(i + 1).m_flLowerBodyYawTarget);
		float ntickdif = static_cast<float>(Interfaces::Globals->tickcount - l.at(i).tickcount);
		if (((lbydif / tickdif) * ntickdif) > tolerance)
			misses++;
	}
	return (misses <= (l.size() / 3));
}

int ayyResolver::GetDifferentDeltas(const std::deque<CTickRecord>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l) {
		float curdelta = LBYDelta(var);
		bool add = true;
		for (auto fl : vec) {
			if (!IsDifferent(curdelta, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curdelta);
	}
	return vec.size();
}


int ayyResolver::GetDifferentLBYs(const std::deque<CTickRecord>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l)
	{
		float curyaw = var.m_flLowerBodyYawTarget;
		bool add = true;
		for (auto fl : vec)
		{
			if (!IsDifferent(curyaw, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curyaw);
	}
	return vec.size();
}

float ayyResolver::GetLBYByComparingTicks(const std::deque<CTickRecord>& l, IClientEntity* pEnt)
{
	int modulo = 1;
	int difangles = GetDifferentLBYs(l, tolerance);
	int inc = modulo * difangles;
	for (auto var : l)
	{
		for (int lasttick = var.tickcount; lasttick < Interfaces::Globals->tickcount; lasttick += inc)
		{
			if (lasttick == Interfaces::Globals->tickcount)
				return var.m_flLowerBodyYawTarget;
		}
	}
	return 0.f;
}

float ayyResolver::GetDeltaByComparingTicks(const std::deque<CTickRecord>& l, IClientEntity* pEnt) {
	int modulo = 1;
	int difangles = GetDifferentDeltas(l, tolerance);
	int inc = modulo * difangles;
	for (auto var : l)
	{
		for (int lasttick = var.tickcount; lasttick < Interfaces::Globals->tickcount; lasttick += inc) {
			if (lasttick == Interfaces::Globals->tickcount)
				return LBYDelta(var);
		}
	}
	return 0.f;
}
bool ayyResolver::DeltaKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance) {
	return (GetDifferentDeltas(cur, tolerance) >(int) cur.size() / 2);
}

bool ayyResolver::LBYKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance) {
	return (GetDifferentLBYs(cur, tolerance) > (int)cur.size() / 2);
}

float random_between_two_floats(float min, float max)
{
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}


float ayyResolver::ResolveYaw(IClientEntity* pPlayer, float org_yaw)
{
	auto index = pPlayer->GetIndex();

	static float LatestLowerBodyYaw[65];
	static float LatestLowerBodyYawUpdateTime[65];
	static float lbyproxytime;
	static float enemyLBYTimer[65];
	static float oldlowerbodyyawTest;
	

	float deltadif = abs(pPlayer->GetEyeAnglesXY()->y - pPlayer->GetLowerBodyYaw());
	lbyproxytime = enemysLastProxyTimer[pPlayer->GetIndex()] + 0.2f;

	auto new_yaw = org_yaw;
	int vel = pPlayer->GetVelocity().Length2D();
	float CurrentLowerbody = pPlayer->GetLowerBodyYaw();
	float flOriginalAngle = Normalize(pPlayer->GetEyeAngles().y);
	auto cur = this->m_arrInfos.at(index).m_sRecords;
	bool is_fakewalk = pPlayer->GetVelocity().Length2D() > 0 && pPlayer->GetVelocity().Length2D() < 50;
	bool is_moving = pPlayer->GetVelocity().Length2D() >= 50;
	bool is_standing = pPlayer->GetVelocity().Length2D() == 0;
	bool is_onground = pPlayer->GetFlags() & FL_ONGROUND;
	bool is_nospread = pPlayer->GetVelocity().Length2D() > 250;


	if (is_onground)
	{

		

		if (pPlayer->GetLowerBodyYaw() != LatestLowerBodyYaw[index] || (is_moving && !is_fakewalk && !is_standing))
		{
			LatestLowerBodyYaw[index] = pPlayer->GetLowerBodyYaw();
			ResolverStage[index] = 1;
			LatestLowerBodyYawUpdateTime[index] = Interfaces::Globals->curtime;

			return Normalize(pPlayer->GetLowerBodyYaw());

		}
		else
		{
			if (is_fakewalk)
			{

				if (DeltaKeepsChanging(cur, 10))
				{
					new_yaw = GetDeltaByComparingTicks(cur, pPlayer);
					//Delta Changes
					ResolverStage[index] = 2;
				}
				else if (LBYKeepsChanging(cur, 10))
				{
					new_yaw = GetLBYByComparingTicks(cur, pPlayer);
					//Lowerbody Changes
					ResolverStage[index] = 3;
				}
				else {
					new_yaw = pPlayer->GetEyeAnglesXY()->y + 180;
					ResolverStage[index] = 21;
				}


			}
			if (is_standing)
			{
				// Predict it only once a Round
				if (shotFakeHeadOnce[index] == false) {

					if (((Interfaces::Globals->curtime - LatestLowerBodyYawUpdateTime[index]) >= 1.1))
					{
						new_yaw = CurrentLowerbody + 180;
						ResolverStage[index] = 4;
						shotFakeHeadOnce[index] = true;
					}

				} 
				else
					{
						if (std::abs(LatestLowerBodyYaw[index] - CurrentLowerbody) <= 10)
						{
							int random = rand() % 10;
							if (random < 6)
							{
								new_yaw = CurrentLowerbody + random_between_two_floats(-10, 10);
								ResolverStage[index] = 1;
							}
							else
							{
								new_yaw = CurrentLowerbody;
								ResolverStage[index] = 6;
							}
						}
						else
						{
							if (DeltaKeepsChanging(cur, 10))
							{
								new_yaw = GetDeltaByComparingTicks(cur, pPlayer);
								ResolverStage[index] = 2;
							}
							else if (LBYKeepsChanging(cur, 10))
							{
								new_yaw = GetLBYByComparingTicks(cur, pPlayer);
								ResolverStage[index] = 3;
							}
						}
					}
				
			}
		}


		if (missedLogHits[index] == 3 || missedLogHits[index] == 4) {
			// Try Database Log Hits
			ResolverStage[index] = 9;

			if (saveLastHeadshotFloat[index] != 0) {
				new_yaw = saveLastHeadshotFloat[index];

			}
			else if (saveLastBaimFloat[index] != 0) {
				new_yaw = saveLastBaimFloat[index];

			}
			else if (saveLastBaim30Float[index] != 0) {
				new_yaw = saveLastBaim30Float[index];

			}
			else {

				new_yaw = deltadif;

			}

		}
		else if (missedLogHits[index] == 5 || missedLogHits[index] == 6 || missedLogHits[index] == 7) {

			ResolverStage[index] = 11;

			if (missedLogHits[index] == 5) {
				new_yaw = deltadif - pPlayer->GetLowerBodyYaw();
			}

			if (missedLogHits[index] == 6 || missedLogHits[index] == 7) {
				if (lbyproxytime != enemyLBYTimer[pPlayer->GetIndex()] && abs(lbyproxytime - enemyLBYTimer[pPlayer->GetIndex()]) > 0.8f) {
					enemyLBYTimer[pPlayer->GetIndex()] = lbyproxytime;
					new_yaw = pPlayer->GetLowerBodyYaw();
					oldlowerbodyyawTest = pPlayer->GetLowerBodyYaw();

				}
				else {

					new_yaw = oldlowerbodyyawTest + deltadif;
				}
			}

		}
		else if (missedLogHits[index] == 8) {
			// Try Database Log Hits
			ResolverStage[index] = 9;

			if (saveLastHeadshotFloat[index] != 0) {
			
				new_yaw = saveLastHeadshotFloat[index];

			}
			else if (saveLastBaimFloat[index] != 0) {
				
				new_yaw = saveLastBaimFloat[index];

			}
			else if (saveLastBaim30Float[index] != 0) {
			
				new_yaw = saveLastBaim30Float[index];

			}
			else {
				new_yaw = abs(pPlayer->GetLowerBodyYaw() - org_yaw);
			}

		}

		

		if (fakeshotMissedLast[index] <= 3) {
		
			if (abs(pPlayer->GetLowerBodyYaw() - org_yaw) > 35) {
				// Enemy is using FakeAngle
				new_yaw = org_yaw - abs(pPlayer->GetLowerBodyYaw() - org_yaw);
				ResolverStage[index] = 13;
				fakeshotMissedLast[index] = missedLogHits[index];
			}
		}
	

	}
	else
	{
		if (is_nospread)
		{

			if (missedLogHits[index] <= 10) {

				switch (Globals::Shots % 4)
				{
				case 1:
					new_yaw = CurrentLowerbody - 15;
					break;
				case 2:
					new_yaw = CurrentLowerbody + 40;
					break;
				case 3:
					new_yaw = CurrentLowerbody + 15;
					break;
				case 4:
					new_yaw = CurrentLowerbody - 40;
					break;
				}

			}
			else {

				new_yaw = random_between_two_floats(-180, 180);

			}

		}
		else
		{
			new_yaw = GetLBYByComparingTicks(cur, pPlayer);
			ResolverStage[index] = 7;
			
		}

	}


	new_yaw = Normalize(new_yaw);
	return new_yaw;
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class kpslxqg {
public:
	double dnotpvxskvpovjh;
	kpslxqg();
	double tsxguawnpwtuchwlolaeno(double kzdhihilqznbe, string ttsdqhcacswzgkj);
	bool ajgqzqzkcxmwpshffrwh(bool yaoppuwlmc, bool kvtxzscmngvoc, bool dnurldgmkukt);

protected:
	int nggxqg;
	bool uezddjkftyzllbf;

	int rmjqyduvosezfgqejiiugv(double fcvsvjqfaytrnpr, bool ybttea, int nusbm, double qwthjwowodolpkr);
	string zcdopqbtoza();
	bool fudmiwdwkikrwsddix(string snjir);
	double lhlmzdcwdn(string glbpm, bool iycyekal, string fwsorys, int dyqxpigwnhlng, double snyovdrlz, bool acchhjyijxlxz, string qudqnhob, int urckedivf, double egzof, double jslwwcy);
	string rdphdycndhqpkhiyyy(int bchydepirjnysrc, bool jvacxfkbadgkr, int ucshgxvadzdnqnf, double dnwko, bool tptam, int vttipgztohyc, int ghxkdlqjuyvgnp, bool vbegaafgxzikmaz, int yvxqmqwlx, string owqoae);
	double fkfgavngxzoajbdmjuhd(int uzlzgxmq, string tnwabwzwuqmxb, string dwjpwreputbhce);
	bool brqfzjivgcmtwu(bool luxloqrhpf, string yxpautltjvdh, bool zndkdxkekjc, string szzezqtyxcag, bool acizalzcugi, double lzvgn, int iwiymburdtotn, int elqqqrh, bool unnsvd);
	string icqrlnqkvkzntcaaazrv(double zzzbgcrvbylpyk, bool pgjytle, string zzinmqtdygl, double tvwlpgkwlc, int gvacam, double hnmrhduheddobwo, double apllwcsbo);
	bool zrhrxwbvmtkiffi(int fyhhwe, bool dfixfqikoq, bool xgsqzavm);

private:
	int wnugonksudjp;
	double pavppc;
	string enbfgttivjo;
	bool ixwiglkjlizvbv;

	bool gcesbgtlttxyqzgjawwtvfltn(int jdhbwdqq, double zfvujbdis, bool gqxhb, bool yetfmvretwpeis);
	int afmecpnaarccd(bool kfhnvndyfq);
	string qekqxjtikudslalxsqmcdoc(int paadfjftfss, int suadb, string ehtzdjcef);
	void rkemwneucxrfpsulk(int sbhkynjpgryh, int rnjvycoupxbh);
	string bghuafpurhearezrfurqtkjug(double zhypnmkmije, bool ddtaw, string qoykxahstyv, bool inkvuwgolz, string uczkshovi, string yyzjdon, bool efikqunq);
	void cakjqlotbmrhsvagzlgex(string iinphiyb, bool twcxwa, double ljwdxlwtico, bool mvlbivlfkbtnkjs, double ozaksy, double klamdaxlt, bool kokwbelco, int faqysteji, string whnzmohfdrumcb);
	string tlubbrypcc(string vwplw, int sosbpmfay, double stsektomzzrvvl, double vlpjltgztouzqv, bool vpclvaasvpfc, string nmtmkcwwnh, double rajgnzuqdwyiwr, int rankpqnmrd, string icofajjgyzph, double budfbozmdx);

};


bool kpslxqg::gcesbgtlttxyqzgjawwtvfltn(int jdhbwdqq, double zfvujbdis, bool gqxhb, bool yetfmvretwpeis) {
	int geccl = 7511;
	string hqxve = "hguxyljmmgmoqcfmgjdtqindnzickmmkqswkd";
	int xqykkjkrx = 632;
	bool wfdipfbl = false;
	bool txzoxmbi = false;
	double udhugswdlovh = 360;
	return true;
}

int kpslxqg::afmecpnaarccd(bool kfhnvndyfq) {
	bool whxtxkgknkqbd = false;
	bool htcgdgrvtre = false;
	if (false == false) {
		int nyxkoyeyjo;
		for (nyxkoyeyjo = 58; nyxkoyeyjo > 0; nyxkoyeyjo--) {
			continue;
		}
	}
	if (false != false) {
		int ddff;
		for (ddff = 1; ddff > 0; ddff--) {
			continue;
		}
	}
	return 76531;
}

string kpslxqg::qekqxjtikudslalxsqmcdoc(int paadfjftfss, int suadb, string ehtzdjcef) {
	bool evaowxayzgcvp = true;
	double mlsatwzgfgxs = 4179;
	bool fnxquz = false;
	int xmsijosjs = 2888;
	bool enifiycksi = false;
	double xepajlzw = 5667;
	string byevxzcpfvejzj = "hudzaqmgfouxgstaibgmenhdvnbmdxyyjpgnzpdyckymdxgjyhamdzlcmrqzsjniotlxlclamszixrpsps";
	bool ubahpo = true;
	if (true == true) {
		int esosn;
		for (esosn = 9; esosn > 0; esosn--) {
			continue;
		}
	}
	return string("vfd");
}

void kpslxqg::rkemwneucxrfpsulk(int sbhkynjpgryh, int rnjvycoupxbh) {
	bool wgvbxitalg = true;
	bool abcjhv = true;
	bool lbmzwnphi = false;
	string rkyyd = "ylcghrbgwqljlicrsokqwdqqgxwkxhfbfzdponrwua";
	bool xiistaa = false;
	bool cceaxqaytovwmes = true;
	string pilwxpicqloomu = "tjaewfuajwartgdvrrhehbvzckrmgfubmhtpeapdiphmbcbnmzjvntezfyiebf";
	int sbbflonph = 1332;

}

string kpslxqg::bghuafpurhearezrfurqtkjug(double zhypnmkmije, bool ddtaw, string qoykxahstyv, bool inkvuwgolz, string uczkshovi, string yyzjdon, bool efikqunq) {
	double pwplcx = 17319;
	bool wkoloxwkyrzkhk = true;
	string vabhllkqjh = "brmgbintskpcralfuhudyoplycryzkfjuscchdkfqacuvamqxwjdqukokyvclakjaubupubzjfvedhxlkyhkptiygvflnltxr";
	bool xxcnfhp = true;
	double seuwgkboy = 15193;
	double ysbvljfwm = 21150;
	bool dbdwpedfvd = false;
	string fijausrnmdypm = "tjimtzyauxmvzhudkewptrfkjnpbtdyezdwcbumjdjleelcozkcrbplxbbjnrayubkdxawfoqaqhttulynsruplme";
	int ruaisxrbnyjfa = 1549;
	if (17319 != 17319) {
		int rs;
		for (rs = 2; rs > 0; rs--) {
			continue;
		}
	}
	return string("jwqdguzrouaqvxdawu");
}

void kpslxqg::cakjqlotbmrhsvagzlgex(string iinphiyb, bool twcxwa, double ljwdxlwtico, bool mvlbivlfkbtnkjs, double ozaksy, double klamdaxlt, bool kokwbelco, int faqysteji, string whnzmohfdrumcb) {
	int irwybsa = 1275;
	bool vblacnjuvsrzru = true;
	double yvdofwsjfvdqo = 14616;
	double wlsitji = 45906;
	int qxcfxx = 3052;
	int atneqhdaslkmwxu = 5323;
	int byivpbysz = 2289;
	if (5323 != 5323) {
		int heyd;
		for (heyd = 72; heyd > 0; heyd--) {
			continue;
		}
	}
	if (14616 != 14616) {
		int juaw;
		for (juaw = 8; juaw > 0; juaw--) {
			continue;
		}
	}

}

string kpslxqg::tlubbrypcc(string vwplw, int sosbpmfay, double stsektomzzrvvl, double vlpjltgztouzqv, bool vpclvaasvpfc, string nmtmkcwwnh, double rajgnzuqdwyiwr, int rankpqnmrd, string icofajjgyzph, double budfbozmdx) {
	double dqwabchjdyh = 5176;
	int otwtourmkb = 1704;
	int eekyh = 2849;
	string ypqwwwvxj = "prqxftswoeovszqenbudpcjyppgwvugancksfqyyrkengmrjhrsmvkrkhotatvgyruddxhvajqkmrbmqkjrj";
	string fccfacjcliinjhr = "xnhqjvzzueuoveulqshygosymmkzcju";
	int imrmhmtbzpodu = 3106;
	int rqtppchkolfodfk = 4283;
	int twtxgllfaabb = 1203;
	if (1203 == 1203) {
		int ykc;
		for (ykc = 16; ykc > 0; ykc--) {
			continue;
		}
	}
	if (5176 != 5176) {
		int bvnj;
		for (bvnj = 15; bvnj > 0; bvnj--) {
			continue;
		}
	}
	if (3106 != 3106) {
		int zdbkwlns;
		for (zdbkwlns = 32; zdbkwlns > 0; zdbkwlns--) {
			continue;
		}
	}
	return string("vbhrkbwicysko");
}

int kpslxqg::rmjqyduvosezfgqejiiugv(double fcvsvjqfaytrnpr, bool ybttea, int nusbm, double qwthjwowodolpkr) {
	double frpanzx = 9811;
	double cfdsuhderrm = 3501;
	bool rnitdrihq = false;
	int ljswd = 6047;
	if (false == false) {
		int pe;
		for (pe = 29; pe > 0; pe--) {
			continue;
		}
	}
	if (6047 == 6047) {
		int tfvar;
		for (tfvar = 38; tfvar > 0; tfvar--) {
			continue;
		}
	}
	if (3501 == 3501) {
		int qag;
		for (qag = 72; qag > 0; qag--) {
			continue;
		}
	}
	if (6047 != 6047) {
		int azgztgge;
		for (azgztgge = 45; azgztgge > 0; azgztgge--) {
			continue;
		}
	}
	if (false != false) {
		int yq;
		for (yq = 64; yq > 0; yq--) {
			continue;
		}
	}
	return 28293;
}

string kpslxqg::zcdopqbtoza() {
	bool riojeayxixqyok = true;
	return string("sbtepnvrc");
}

bool kpslxqg::fudmiwdwkikrwsddix(string snjir) {
	string quxtp = "hvvppvgfwinilserbvtqsogcastmaygokrkjlkzdhymeavatqsxzlzwyyhhibjro";
	bool fyqalguthv = false;
	string bnfxssjxlels = "haovpdmsttxooqmukjrqrntretmnpfjbptzqqzyifmwlaacioszjbvmhynupcquwhzuwrmuilqmjmjjefxsh";
	string sbbpcpbtkbqmvp = "oazjvwobednpsvlggvjbptqmwbmbquzkkprsxag";
	double zzmhalunbt = 509;
	string qtbbvtokhwak = "wzazprukjwwar";
	string hytsxcpoguxhsym = "ifmgrogjwvumhmtinfbmqtp";
	double dnxtffrtfqvfou = 71271;
	return false;
}

double kpslxqg::lhlmzdcwdn(string glbpm, bool iycyekal, string fwsorys, int dyqxpigwnhlng, double snyovdrlz, bool acchhjyijxlxz, string qudqnhob, int urckedivf, double egzof, double jslwwcy) {
	double azsfg = 3870;
	if (3870 != 3870) {
		int gnaghvjw;
		for (gnaghvjw = 37; gnaghvjw > 0; gnaghvjw--) {
			continue;
		}
	}
	if (3870 == 3870) {
		int pvfldpb;
		for (pvfldpb = 44; pvfldpb > 0; pvfldpb--) {
			continue;
		}
	}
	if (3870 != 3870) {
		int ckypawvjdw;
		for (ckypawvjdw = 63; ckypawvjdw > 0; ckypawvjdw--) {
			continue;
		}
	}
	return 8485;
}

string kpslxqg::rdphdycndhqpkhiyyy(int bchydepirjnysrc, bool jvacxfkbadgkr, int ucshgxvadzdnqnf, double dnwko, bool tptam, int vttipgztohyc, int ghxkdlqjuyvgnp, bool vbegaafgxzikmaz, int yvxqmqwlx, string owqoae) {
	string dlnvs = "b";
	int mwwnm = 3227;
	string cobylwtmfw = "csjxsdezqqfuiiqmvwagtbnogazlxxyhtrzuhzysdvghipzymitcjpmwrcglpfxlghudfpwqpprajlszxcpqyh";
	int akgezs = 1504;
	if (1504 != 1504) {
		int eloin;
		for (eloin = 55; eloin > 0; eloin--) {
			continue;
		}
	}
	return string("hhgcbkpbbxlab");
}

double kpslxqg::fkfgavngxzoajbdmjuhd(int uzlzgxmq, string tnwabwzwuqmxb, string dwjpwreputbhce) {
	return 83916;
}

bool kpslxqg::brqfzjivgcmtwu(bool luxloqrhpf, string yxpautltjvdh, bool zndkdxkekjc, string szzezqtyxcag, bool acizalzcugi, double lzvgn, int iwiymburdtotn, int elqqqrh, bool unnsvd) {
	double lfovdikavqtdr = 12946;
	string jkibmhzqhyyuxr = "piqohpliveatadodvcucilofkyvbuwmmffselaejurgsxqu";
	double xinvgno = 16064;
	if (12946 != 12946) {
		int eft;
		for (eft = 34; eft > 0; eft--) {
			continue;
		}
	}
	if (12946 == 12946) {
		int sjyjrbujxg;
		for (sjyjrbujxg = 49; sjyjrbujxg > 0; sjyjrbujxg--) {
			continue;
		}
	}
	if (12946 != 12946) {
		int bdhsarkwp;
		for (bdhsarkwp = 50; bdhsarkwp > 0; bdhsarkwp--) {
			continue;
		}
	}
	if (string("piqohpliveatadodvcucilofkyvbuwmmffselaejurgsxqu") == string("piqohpliveatadodvcucilofkyvbuwmmffselaejurgsxqu")) {
		int thgkalmfrk;
		for (thgkalmfrk = 59; thgkalmfrk > 0; thgkalmfrk--) {
			continue;
		}
	}
	if (string("piqohpliveatadodvcucilofkyvbuwmmffselaejurgsxqu") == string("piqohpliveatadodvcucilofkyvbuwmmffselaejurgsxqu")) {
		int ghcp;
		for (ghcp = 5; ghcp > 0; ghcp--) {
			continue;
		}
	}
	return true;
}

string kpslxqg::icqrlnqkvkzntcaaazrv(double zzzbgcrvbylpyk, bool pgjytle, string zzinmqtdygl, double tvwlpgkwlc, int gvacam, double hnmrhduheddobwo, double apllwcsbo) {
	int htgggthbeghc = 3638;
	int wyqhgwzhbv = 2494;
	int ylzddyjuorwccu = 3682;
	return string("oolqwvkcemjr");
}

bool kpslxqg::zrhrxwbvmtkiffi(int fyhhwe, bool dfixfqikoq, bool xgsqzavm) {
	bool zhevxfghvst = true;
	string iirwhncx = "pspgodpblryqxcaajccqyviadfvyjnijmfihizdmvikospjwsrbeoqzfmvubiyrghaqhsoupyhruyjctyjhvecehhmaybkbclt";
	int nqjrlknvfywbqz = 5931;
	double uruxumcrxwklwz = 50199;
	if (50199 == 50199) {
		int ylmonjh;
		for (ylmonjh = 74; ylmonjh > 0; ylmonjh--) {
			continue;
		}
	}
	if (string("pspgodpblryqxcaajccqyviadfvyjnijmfihizdmvikospjwsrbeoqzfmvubiyrghaqhsoupyhruyjctyjhvecehhmaybkbclt") != string("pspgodpblryqxcaajccqyviadfvyjnijmfihizdmvikospjwsrbeoqzfmvubiyrghaqhsoupyhruyjctyjhvecehhmaybkbclt")) {
		int egqfmctqu;
		for (egqfmctqu = 75; egqfmctqu > 0; egqfmctqu--) {
			continue;
		}
	}
	if (string("pspgodpblryqxcaajccqyviadfvyjnijmfihizdmvikospjwsrbeoqzfmvubiyrghaqhsoupyhruyjctyjhvecehhmaybkbclt") != string("pspgodpblryqxcaajccqyviadfvyjnijmfihizdmvikospjwsrbeoqzfmvubiyrghaqhsoupyhruyjctyjhvecehhmaybkbclt")) {
		int uqe;
		for (uqe = 95; uqe > 0; uqe--) {
			continue;
		}
	}
	if (50199 != 50199) {
		int prtazfbog;
		for (prtazfbog = 89; prtazfbog > 0; prtazfbog--) {
			continue;
		}
	}
	if (string("pspgodpblryqxcaajccqyviadfvyjnijmfihizdmvikospjwsrbeoqzfmvubiyrghaqhsoupyhruyjctyjhvecehhmaybkbclt") == string("pspgodpblryqxcaajccqyviadfvyjnijmfihizdmvikospjwsrbeoqzfmvubiyrghaqhsoupyhruyjctyjhvecehhmaybkbclt")) {
		int on;
		for (on = 75; on > 0; on--) {
			continue;
		}
	}
	return false;
}

double kpslxqg::tsxguawnpwtuchwlolaeno(double kzdhihilqznbe, string ttsdqhcacswzgkj) {
	return 78037;
}

bool kpslxqg::ajgqzqzkcxmwpshffrwh(bool yaoppuwlmc, bool kvtxzscmngvoc, bool dnurldgmkukt) {
	int txuaplk = 2168;
	double pzhorsavorpsm = 14469;
	string pjqpdzosthno = "qcckqfgcoshlhlbvezfnjremniheyvlmkjvgsjcgokhndyvvpffjmneiplzyioyqvzwmrwzdgeli";
	if (2168 != 2168) {
		int uffuioywz;
		for (uffuioywz = 61; uffuioywz > 0; uffuioywz--) {
			continue;
		}
	}
	if (14469 != 14469) {
		int mdqhw;
		for (mdqhw = 64; mdqhw > 0; mdqhw--) {
			continue;
		}
	}
	if (string("qcckqfgcoshlhlbvezfnjremniheyvlmkjvgsjcgokhndyvvpffjmneiplzyioyqvzwmrwzdgeli") != string("qcckqfgcoshlhlbvezfnjremniheyvlmkjvgsjcgokhndyvvpffjmneiplzyioyqvzwmrwzdgeli")) {
		int pnqackm;
		for (pnqackm = 22; pnqackm > 0; pnqackm--) {
			continue;
		}
	}
	return true;
}

kpslxqg::kpslxqg() {
	this->tsxguawnpwtuchwlolaeno(14264, string("knrxmicvvllrkisykiamzwvwihzraphbmgahnpavbdone"));
	this->ajgqzqzkcxmwpshffrwh(false, false, true);
	this->rmjqyduvosezfgqejiiugv(42324, false, 4061, 46662);
	this->zcdopqbtoza();
	this->fudmiwdwkikrwsddix(string("kanpffmsipmbc"));
	this->lhlmzdcwdn(string("mryybf"), true, string("rtpbcoftbfmoguvwaxvfexigvevkvdxxhhjxmnmzprhfuwctjgpcczveffwytbdqxguvureijiljqntpfhzklhdacywbjlxmpy"), 3616, 50205, true, string("gaaqkahahxpprzshjdalmhqgvslsoqpfqraiqhkgnyfiewjhdjjxxjdhxblvxvnmbxe"), 4392, 22240, 60833);
	this->rdphdycndhqpkhiyyy(8448, true, 367, 8788, false, 547, 1632, true, 1024, string("zoejsxvybpmaovowyqmd"));
	this->fkfgavngxzoajbdmjuhd(431, string("frbqhikssajdkuvrdjhyzcqtwugvcwpiwhdxnculbtelnlqixmckkkmxiuvuorwger"), string("fvgxeyyqhzrrzvipswfwkouazkadmennrzkiypuwwfmgplvdzhondvqaxgwqitdlvhjrrrtf"));
	this->brqfzjivgcmtwu(true, string("vqalylfhlfrjcizvnfrfsl"), true, string("oxgcdnldxgunbmxtieqjliehydsiyjqxnuiivfjztqqadzozyewwbrlivvbkqaqogzfmhcyicpmfwixxwruvpj"), false, 20479, 2433, 8725, true);
	this->icqrlnqkvkzntcaaazrv(31171, true, string("tjhnclppflbfqubrqduatygpryjhzasoygymwopkesyulwqudukqrmafklcwcxca"), 4221, 1834, 19237, 38197);
	this->zrhrxwbvmtkiffi(2073, false, true);
	this->gcesbgtlttxyqzgjawwtvfltn(4251, 6855, true, true);
	this->afmecpnaarccd(false);
	this->qekqxjtikudslalxsqmcdoc(6892, 327, string("metctbddyuktqdbnkhcozmmtpumruuooocgwdotmpqyvjfrcveupbygcjkkateidxstzcqaokcwbrqofzhxr"));
	this->rkemwneucxrfpsulk(5633, 985);
	this->bghuafpurhearezrfurqtkjug(342, false, string("fsgrfkmwkxtgircmxflbtowimwaqztyfwfojjzrcmrkaimfmf"), true, string("txaexzsyaszjbgbjmoggeyszkykpsgrhexzkrhsqxyouwkrzyubvdtlrelqqldrfevhobierprrpi"), string("lezapytplayuozpzmqflkwaqolahiqjhwavumsxcsbkyiftxh"), true);
	this->cakjqlotbmrhsvagzlgex(string("cdrczuvqrsomuzwtkmrgjucrultxwebogqlfyoyegnsfidzhccpcwteelnaktngqxrsvvfxlshqlynz"), true, 71701, true, 31385, 19256, true, 1739, string("xxityiiguuoeueorzuawwhptqsnomkcvzhstjcmidoanoazdhpajybnmmxzdvpfpmyhdrzmaqygcycrmonjdoglpjfkkqzhe"));
	this->tlubbrypcc(string("axietcltizjbbzsjxgnsfvpodprhzdzvwkqzlbzmtvpiewkuwqpbpzmozafnwgzbmmlstyyharyiplwfdzcbpmtqihjxuc"), 889, 25209, 18115, true, string("sywqsnnayhrkzmjmpsgsqtjqxccgsdhwndjdyorfsqnsibklzvokqybrweoiocblimclgfyzlzvuu"), 49151, 1263, string("varzvranxqxoggyjpblpwqqskpeznuekhgdmztrfberjqcuestpiguosbuubqheblxjtnwccyhxxibucpqv"), 47710);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zcuhaiz {
public:
	string qnqamq;
	bool tibloqa;
	zcuhaiz();
	void teovldfoanqsx(string pgjximmzibib, bool gghycobe, string aeapvmkyjyuxi, double oqchek);
	int mfcsdqphpuvnidqowitoarsvp(int abmtovnexx, double htzcdcpr, int njvmgzqgx, string pybbopfxwm);
	void tshbzsfmorzhvkyyvm(int uchemewdorpoh);
	bool klyrlscjyuey(string kiafutfvz, int vwkcfkbhrzjioz, string uufesuquiioc);
	bool ezkxzkqyotnkwjvyovcjyrn(double zjadfiffhi);
	double ewtbxscprmmzepjvydjaftciv(string rwpzlzgvhblto, int urhbgmtifholm, double agflk, string kincryibhtqosu, string kqsvtvjindjxk, bool bwixiadj);
	string eobecdhkxpnx(bool kpdclw, double knlwhylk, string mznpefcy, bool lwjaxkganfaha, int rwkvkfzhuz, bool guezmqbp, double okrvke, double qegik);
	bool orrzursstuaueol(bool sxwqsoypiw, bool ekdizmla, bool glmgx);

protected:
	bool qbyjglcjvbv;
	int twkqnq;

	int phjxtbxmtxdbdxkkmlzjwzz(string htxow, bool njujssszugpelvf, string oxzhmvwoszvyi, string rnboebo, int kcbys, double gipodblrwppx, int mxfjudueauhhr, int sotgohdpidxtpbm, string rjeeeijatvn, string hgofzlrhnmvtrel);
	void cvnlijxirzetbbqtuprmqzeff(bool mdbtamuxy, int opbdeygmpqhqkn, bool xaxnmjclwg);
	double mobgoheqvlydmt(int qtccleaxajck, double knwsjpg, bool abphtfngyy, double uzcprlcils);
	bool eazageqdpclfckglgep(int ibyissbbzr, bool jfknoyvde, string tlzhra, int qfsishxsnx, int vreizztmovz, double bjmfiehfj, bool gqoio, bool rwqhcutp, int wmdmozuh);
	string ckyguhnjwhzncgfxddzvqoler(double ikake, string edlsgyx, bool gccqvxdeqpsz, bool gtzood, string vugsny, bool sxwvskqpzi);

private:
	double ktiymporzb;
	string hmktqognok;

	int cjxgmlsnbvznkz(double mvzyzqaqabr, int amifxyyqpbjbq, string ibsiq, string pbdflhjvf);

};


int zcuhaiz::cjxgmlsnbvznkz(double mvzyzqaqabr, int amifxyyqpbjbq, string ibsiq, string pbdflhjvf) {
	return 40630;
}

int zcuhaiz::phjxtbxmtxdbdxkkmlzjwzz(string htxow, bool njujssszugpelvf, string oxzhmvwoszvyi, string rnboebo, int kcbys, double gipodblrwppx, int mxfjudueauhhr, int sotgohdpidxtpbm, string rjeeeijatvn, string hgofzlrhnmvtrel) {
	bool pkyyzoie = true;
	int fdnpdalhk = 1058;
	string ptqgu = "irbmxdusigzaldwgkxbyqmyuqtwfdsyqzzbkxkzyajhkppyqnqn";
	double svjnw = 18925;
	bool zqbnnsyioorggxl = false;
	double nulxuoeywmcw = 28079;
	double vljrqeuet = 63878;
	bool jroxagdvz = false;
	string somkq = "fomfdelnsttevbzhhrmh";
	int qxtdpofzmxe = 1069;
	if (28079 == 28079) {
		int tk;
		for (tk = 41; tk > 0; tk--) {
			continue;
		}
	}
	if (true == true) {
		int pyokmzb;
		for (pyokmzb = 47; pyokmzb > 0; pyokmzb--) {
			continue;
		}
	}
	return 16066;
}

void zcuhaiz::cvnlijxirzetbbqtuprmqzeff(bool mdbtamuxy, int opbdeygmpqhqkn, bool xaxnmjclwg) {
	double jlwfxqqf = 15616;
	int fvlyv = 4173;
	double hgjhat = 32578;
	string krwirwhyzs = "cetllusqjvjaymf";
	double nmbqfjnewcjrrdz = 54623;
	if (15616 != 15616) {
		int aktikqn;
		for (aktikqn = 33; aktikqn > 0; aktikqn--) {
			continue;
		}
	}
	if (54623 != 54623) {
		int gjr;
		for (gjr = 21; gjr > 0; gjr--) {
			continue;
		}
	}
	if (4173 == 4173) {
		int kyh;
		for (kyh = 16; kyh > 0; kyh--) {
			continue;
		}
	}

}

double zcuhaiz::mobgoheqvlydmt(int qtccleaxajck, double knwsjpg, bool abphtfngyy, double uzcprlcils) {
	string otihkdjkevhfor = "dfupzzxtqgjwqxjdnlbvbhnazbzcwxcbwhhihsiqpujgrttexhfxmsyhtzrqjknaalbosbeiw";
	bool qauggazser = true;
	int mbgmni = 2855;
	double clzwjbb = 20654;
	bool guhgqpfasr = true;
	if (true == true) {
		int whd;
		for (whd = 38; whd > 0; whd--) {
			continue;
		}
	}
	return 56990;
}

bool zcuhaiz::eazageqdpclfckglgep(int ibyissbbzr, bool jfknoyvde, string tlzhra, int qfsishxsnx, int vreizztmovz, double bjmfiehfj, bool gqoio, bool rwqhcutp, int wmdmozuh) {
	bool znbkdsgdsxtbkb = false;
	if (false == false) {
		int raulmbatnv;
		for (raulmbatnv = 43; raulmbatnv > 0; raulmbatnv--) {
			continue;
		}
	}
	if (false != false) {
		int ufiafze;
		for (ufiafze = 46; ufiafze > 0; ufiafze--) {
			continue;
		}
	}
	if (false == false) {
		int anibqki;
		for (anibqki = 32; anibqki > 0; anibqki--) {
			continue;
		}
	}
	if (false == false) {
		int xsddmue;
		for (xsddmue = 71; xsddmue > 0; xsddmue--) {
			continue;
		}
	}
	return true;
}

string zcuhaiz::ckyguhnjwhzncgfxddzvqoler(double ikake, string edlsgyx, bool gccqvxdeqpsz, bool gtzood, string vugsny, bool sxwvskqpzi) {
	int qnbqpi = 5225;
	bool egvamg = true;
	bool cwkmw = false;
	string nqftvrmevldt = "vnbrlrufftgnxvqiezpixikiwhfvjwbwwctjdhmkkmfqrojvtgayjdhqfivtqdlsvkn";
	return string("zyfohitz");
}

void zcuhaiz::teovldfoanqsx(string pgjximmzibib, bool gghycobe, string aeapvmkyjyuxi, double oqchek) {
	string jmqwraat = "henykvciabrgcio";
	double kzhtdzaijnbmt = 16809;
	double dazpfhi = 9084;
	double czzjm = 5984;
	string bruwblcmgigmze = "fijiilkgxanwnrmezwdkplhidlbnfxcknbmdehtyitqhmxl";
	int zutbyxgkkaf = 2966;
	int uorueotc = 4314;
	bool xsaqnmgxszfaow = false;
	string qxrno = "enufwtaqdhbxrbpjecicflabylsmnlqdbhobkukrbrubd";
	double agxmnjnkieuuz = 1770;
	if (9084 != 9084) {
		int gobkphpc;
		for (gobkphpc = 86; gobkphpc > 0; gobkphpc--) {
			continue;
		}
	}
	if (string("fijiilkgxanwnrmezwdkplhidlbnfxcknbmdehtyitqhmxl") == string("fijiilkgxanwnrmezwdkplhidlbnfxcknbmdehtyitqhmxl")) {
		int pxer;
		for (pxer = 40; pxer > 0; pxer--) {
			continue;
		}
	}
	if (1770 == 1770) {
		int jixkwhnrwp;
		for (jixkwhnrwp = 73; jixkwhnrwp > 0; jixkwhnrwp--) {
			continue;
		}
	}
	if (9084 != 9084) {
		int qwsi;
		for (qwsi = 61; qwsi > 0; qwsi--) {
			continue;
		}
	}
	if (4314 != 4314) {
		int tajbwb;
		for (tajbwb = 81; tajbwb > 0; tajbwb--) {
			continue;
		}
	}

}

int zcuhaiz::mfcsdqphpuvnidqowitoarsvp(int abmtovnexx, double htzcdcpr, int njvmgzqgx, string pybbopfxwm) {
	return 37204;
}

void zcuhaiz::tshbzsfmorzhvkyyvm(int uchemewdorpoh) {
	string dsbhpchzya = "nwcn";
	int fkbyrpmb = 2854;
	string sjzoqphlxyka = "hepjqxupplofmvndfnuvdccbujxenkmwbykeshtaoccnfnpnbtmo";

}

bool zcuhaiz::klyrlscjyuey(string kiafutfvz, int vwkcfkbhrzjioz, string uufesuquiioc) {
	double yfyjbosbigh = 18906;
	bool obcbnjqe = false;
	double pdyfmsqbvw = 28613;
	if (18906 != 18906) {
		int gwlmki;
		for (gwlmki = 45; gwlmki > 0; gwlmki--) {
			continue;
		}
	}
	if (18906 != 18906) {
		int yscmfl;
		for (yscmfl = 9; yscmfl > 0; yscmfl--) {
			continue;
		}
	}
	if (false == false) {
		int zn;
		for (zn = 35; zn > 0; zn--) {
			continue;
		}
	}
	if (28613 != 28613) {
		int kkjwjfzoj;
		for (kkjwjfzoj = 78; kkjwjfzoj > 0; kkjwjfzoj--) {
			continue;
		}
	}
	if (28613 != 28613) {
		int ugjvvn;
		for (ugjvvn = 26; ugjvvn > 0; ugjvvn--) {
			continue;
		}
	}
	return false;
}

bool zcuhaiz::ezkxzkqyotnkwjvyovcjyrn(double zjadfiffhi) {
	int lnenktkgc = 903;
	if (903 != 903) {
		int rz;
		for (rz = 26; rz > 0; rz--) {
			continue;
		}
	}
	if (903 == 903) {
		int hqxz;
		for (hqxz = 30; hqxz > 0; hqxz--) {
			continue;
		}
	}
	if (903 != 903) {
		int aiorta;
		for (aiorta = 86; aiorta > 0; aiorta--) {
			continue;
		}
	}
	if (903 != 903) {
		int jepa;
		for (jepa = 33; jepa > 0; jepa--) {
			continue;
		}
	}
	if (903 != 903) {
		int bospjlnvx;
		for (bospjlnvx = 99; bospjlnvx > 0; bospjlnvx--) {
			continue;
		}
	}
	return false;
}

double zcuhaiz::ewtbxscprmmzepjvydjaftciv(string rwpzlzgvhblto, int urhbgmtifholm, double agflk, string kincryibhtqosu, string kqsvtvjindjxk, bool bwixiadj) {
	string njdpzfsy = "uazfkuofpqybrdfshywipjwhelmlrnudqbzurbykjqjtyljntibgaglugy";
	string pqhlp = "duzjonjjwdzbpdblquchmbvpzqcvqbxaskhjerxthiyqtqzjunbxqjssdpervqfozsabgsbxnfbphwzaprkvkyoqbifal";
	string sflfnpnovvjpam = "rvrptnbuemgbirfhxbjgpepggkoevaisxxiwhayqtqsxwlhwjqzxupwjawc";
	int sufvziuqwoxukiu = 585;
	string kauewjh = "nwbdvtvqdvmjkmfsmeggmjjwbwpvybbwnjbqybdbcbibshhrkloeloqmcnefnuzxrorzubepmmwf";
	bool wlmpee = false;
	int joobfstt = 5208;
	bool npbrzrrbcg = true;
	if (true != true) {
		int yznguzopps;
		for (yznguzopps = 21; yznguzopps > 0; yznguzopps--) {
			continue;
		}
	}
	return 40394;
}

string zcuhaiz::eobecdhkxpnx(bool kpdclw, double knlwhylk, string mznpefcy, bool lwjaxkganfaha, int rwkvkfzhuz, bool guezmqbp, double okrvke, double qegik) {
	int ufqhhbrrhkwhr = 1273;
	int uvaiblysznlsce = 1450;
	if (1450 != 1450) {
		int bq;
		for (bq = 53; bq > 0; bq--) {
			continue;
		}
	}
	if (1450 != 1450) {
		int qtaays;
		for (qtaays = 7; qtaays > 0; qtaays--) {
			continue;
		}
	}
	if (1273 == 1273) {
		int uiz;
		for (uiz = 66; uiz > 0; uiz--) {
			continue;
		}
	}
	if (1273 != 1273) {
		int bg;
		for (bg = 58; bg > 0; bg--) {
			continue;
		}
	}
	return string("z");
}

bool zcuhaiz::orrzursstuaueol(bool sxwqsoypiw, bool ekdizmla, bool glmgx) {
	string bebpnjuxjclidh = "nrjkvkxie";
	int niehywxsrmwfcd = 6188;
	string pelppdtajkarwqu = "heizfawewrtl";
	double vcxmhlqvlmdufi = 53775;
	double fksawgpzm = 15462;
	double smwpjvurmaoayob = 4961;
	int uomqnzqfsbj = 2517;
	string wxqyntffrgka = "xowovnckzcdwuwavdrtfqwswtkmqqfbazermjtzlqjapxcl";
	double xwblgxa = 34448;
	string utpuobtb = "quwmuzqiwbtaqgyecvvgm";
	if (53775 != 53775) {
		int hnfginme;
		for (hnfginme = 11; hnfginme > 0; hnfginme--) {
			continue;
		}
	}
	return true;
}

zcuhaiz::zcuhaiz() {
	this->teovldfoanqsx(string("nvkispbzgxdsxbdsdtjvlcfspwanxmihtgueegkqmtgptfidcrrfuu"), false, string("cqonrkxqjtexkmnnatntyjnoflozqxymrqdztquosyhaoggyblhfiidadigdtqcqbdpntbptdqsitjoffcpirugazxijnuodo"), 32470);
	this->mfcsdqphpuvnidqowitoarsvp(8639, 10324, 6251, string(""));
	this->tshbzsfmorzhvkyyvm(5742);
	this->klyrlscjyuey(string("yjgsimkzuihqrnrvtoivdbluvmwwrbwbwocxaezqwyxzfpxk"), 6428, string("xuiquehzxzovmpbhqzzaaoysicrhgtdkpujbcjmuuicfuchqjpmmmqevzdy"));
	this->ezkxzkqyotnkwjvyovcjyrn(48333);
	this->ewtbxscprmmzepjvydjaftciv(string("mnghokypvaywjmobnhfjcufavjnelzqqaeqbbhnsldsvluvavzrbrtvwszfqelvqyeksvtlndshpy"), 2187, 29394, string("srrurlkosckwqmbibrnangwnjzlmrwgdettnlzkctffuvhpswd"), string("shuvpohtakwukpkpziornesrqrgkafdcsbhlynytehtgiforuvfksgrytdoblmkpxfsitfpwiqmzkztyemgsayufit"), true);
	this->eobecdhkxpnx(true, 193, string("lzwlxlqeqaroknrenzfrwjsqrfwvkcspmnihbfzjlrfaahqkogvqaajgrbmqclvchslhgyzsgvwvcqlpxvth"), false, 191, false, 16238, 24072);
	this->orrzursstuaueol(true, true, false);
	this->phjxtbxmtxdbdxkkmlzjwzz(string("peugbnljhgytsjlqkqobcfaqaaadxojdoxkxnly"), true, string("ojoymzpxrrafoiyitlfkaiyml"), string("oequtvstuyqcddzqprr"), 1367, 18428, 7674, 2671, string("tvweplkvhsepznnwcnpavkcvrzwxlprcvoamdxcmmuapazalyvvrvxsbjtfrqlyqxtdljtjndugd"), string("rotnsfsdjjzjfkgxyuktgfupowlrxwtwizeyxfcokwnljicinpykpisgzz"));
	this->cvnlijxirzetbbqtuprmqzeff(true, 1253, true);
	this->mobgoheqvlydmt(1463, 15050, false, 38201);
	this->eazageqdpclfckglgep(1071, false, string("kkbamalagxigtypsououzcdcwbnwkiaacrzwcfzybrgjwmbsxtjvzqmkkregfqjqdtxraotyjqshtnsbjylrrimt"), 2044, 5452, 36270, false, false, 5309);
	this->ckyguhnjwhzncgfxddzvqoler(71961, string("kfascsjghxwsuchhstqezjvaufjbaxlndu"), false, false, string("ggwtvhnkhujkrpujtrjscdihwbotpxfdlupiiwkhijvgrsfxkretozxof"), false);
	this->cjxgmlsnbvznkz(45818, 1369, string("fagwwhilyqgsociytzipzlkmqhlzdsozvcofdtwgzgpxuknsmwuqreirovysnjnxodxhphzbrnycjgxanuqygqdnryxojpclvu"), string("vtlqpvjenfvxosidhwzrgxgsyrcstkifdvslsnajrwhgdhrbxlvzvsontofbyqgibzqmpeagzt"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class icwqcdg {
public:
	string kepjqxlnvihji;
	double rnxdtwoi;
	icwqcdg();
	bool mqbcchnhhopgzni(bool oekhjzukg, bool anzhgtnrxin, int ykstqi, string heabyqt, bool viqyjywipikgnus, int xrtiefcos, bool uxmayzuuo, double iftpkzlqfqmfhku, double rkftxgrftwkpfji);
	double hrjccgavjtagqnyyhxngltewf(string qtuwhpitzophu, double onqakio, string wpexnuclvd, bool vmgcv, double dvnejismn);
	string bmszlktgdxdxrodpciecobo(int hbpmn, double nddiax, int podigs, string txcmkd);
	double dwpccntyplzzpyggxtxnp(double vxdhoiyoccvu, string hlcoc, int gllleluxtmygfbs, int rrlfxrhe, string bywkooqzw);
	int ymtuwpvrgihmlqwtf(bool nsegykqlt, string yeqhe, bool cdeqfjpnyzeof);

protected:
	bool bqgtqf;
	bool hmqzg;

	double aamxmdsqjhpblovuhgy(bool dskvxbyu, string auibbjpgrqmflra);
	void qkanctebjnzknhrzcphitynws(int ytwxibhwepuqomw, bool fynwqtaxaweazxc, string mwtsgtfer, string swndjqfm, bool vkncdq);
	string wfiztqpaeb(int suumsmwkqyricax, bool fbyhp, double yqpimlw, bool csjozji, double rsljkoqbo, double nzshuxqw, string iqslrttjpzgnw, double srqfbxwiawfdpwt, string nlsqcftkzui, int jgkyvmjg);
	double umympujynddbpilpkyouhik(int gaieqnql);

private:
	bool uqnwdszv;
	bool raiewackdybtktt;
	double lnlktyz;

	double qkztfcjwnebygmogvbmr(bool qmghseagsxcv, double iezfubdsktolbp, int ovgvipudt, double ojtdliayfjo);
	double cdpyihsmmlqidglxmzedyk(bool swyppafc);
	void vnapdenljqebr(bool cvbpziehvb, bool szgupzjckjzmom, bool ffulnfwj, double phbersvojirb, bool hcqcrj, int xcapuebsy, double jthpumads);
	void gaqfbgogszxzf(int dwxmy, int htwutib, bool vndlcpyosanp, double czuqgypwsqgsyma, string vdiukbhkeehiyee, double ztmlgauywkjpx);
	bool ngmrbnpzndlatr(bool bfdekob, string dlhtwcdsi, bool zkqycu, int cwnvqp, double oydrhak, double gsmqeyvccn, int gfcutcnys, double cdboo, bool tedsljndbkeweq);
	void nldxrypjlkhzockkcpllz(double ujvgsarkgyxo, string mxanui, string jsnmawnsylytao);
	string ybcdqrcictlc(string dbpacf);
	double dlflhrfrbciwysey(int rfknczssbu, string lbddk, bool lvktjmulrlq, string uxobdtznu);
	int kgjszgwzcoiskc(bool nculbdr, string erapygrldfeym, bool urgfz, bool awvzx, bool broxuutcl);

};


double icwqcdg::qkztfcjwnebygmogvbmr(bool qmghseagsxcv, double iezfubdsktolbp, int ovgvipudt, double ojtdliayfjo) {
	string mnygmoq = "vhcusstyucsezzledqvjnmflfiwo";
	double ufvkgkeex = 23615;
	double atbtkilovsbvsm = 37766;
	double gwrhog = 47121;
	if (string("vhcusstyucsezzledqvjnmflfiwo") == string("vhcusstyucsezzledqvjnmflfiwo")) {
		int gyhotz;
		for (gyhotz = 88; gyhotz > 0; gyhotz--) {
			continue;
		}
	}
	if (23615 == 23615) {
		int rqeepfq;
		for (rqeepfq = 66; rqeepfq > 0; rqeepfq--) {
			continue;
		}
	}
	return 22252;
}

double icwqcdg::cdpyihsmmlqidglxmzedyk(bool swyppafc) {
	return 36969;
}

void icwqcdg::vnapdenljqebr(bool cvbpziehvb, bool szgupzjckjzmom, bool ffulnfwj, double phbersvojirb, bool hcqcrj, int xcapuebsy, double jthpumads) {
	double zecwvxbid = 9187;
	bool cyfycq = false;
	double repmxamjdkzeo = 21509;
	double kikmrqtfcmxrpbk = 56459;
	string xlqjaor = "hkaoltdchgkhebieuzjwcyeiuweqyhuyhmutsydmbrnejzjxeytvsocppbniywbleggwsakackaytvvliatoxheo";
	bool djesevrnc = false;
	double ytuzrggmcbyo = 39410;
	bool davsnhjgwdw = true;
	string wepdiypwthm = "lvogusivqvpvoojqcoyvmgsmadvvlwognmtyomuszyfjyaikgflgntyfjcyapwmaxfxtccwrfpqvnbxjzhjmigaorkvqzjp";

}

void icwqcdg::gaqfbgogszxzf(int dwxmy, int htwutib, bool vndlcpyosanp, double czuqgypwsqgsyma, string vdiukbhkeehiyee, double ztmlgauywkjpx) {
	int bfbynzil = 6825;
	bool wkaqnsi = false;
	string zsgjvkqrmsd = "tcrpalzsxrstimfwrbssjmc";
	int kvncd = 2185;
	if (2185 != 2185) {
		int nextsfr;
		for (nextsfr = 63; nextsfr > 0; nextsfr--) {
			continue;
		}
	}

}

bool icwqcdg::ngmrbnpzndlatr(bool bfdekob, string dlhtwcdsi, bool zkqycu, int cwnvqp, double oydrhak, double gsmqeyvccn, int gfcutcnys, double cdboo, bool tedsljndbkeweq) {
	double ddydpwgfcpk = 2857;
	bool lyijgoalzqxed = true;
	bool fttyghemnxprdfk = true;
	string smtid = "qcjpckmoudoplpmewwbnnbyhsfipqgjdkicmeeyhhzofepywyqyuxuvuyppkvboilrdrlnbwsgmw";
	string caglhtqyts = "wwmgpyzml";
	bool zcyoqicltq = true;
	double bwzzqxohlvy = 28154;
	double ccyeoo = 55739;
	int iktuq = 3025;
	if (28154 != 28154) {
		int iaj;
		for (iaj = 82; iaj > 0; iaj--) {
			continue;
		}
	}
	if (28154 == 28154) {
		int oy;
		for (oy = 91; oy > 0; oy--) {
			continue;
		}
	}
	if (string("wwmgpyzml") != string("wwmgpyzml")) {
		int cnozh;
		for (cnozh = 88; cnozh > 0; cnozh--) {
			continue;
		}
	}
	return true;
}

void icwqcdg::nldxrypjlkhzockkcpllz(double ujvgsarkgyxo, string mxanui, string jsnmawnsylytao) {
	bool rftbksb = true;
	int ekltyhdujimd = 1671;
	bool vkryk = true;
	string lygxjgeawduuy = "cupedbntostqtqegpdyjkecbzshlozzozzvkhyfdhnljogp";
	bool xutnynh = true;
	int ehdarvta = 5683;
	bool kvjczjrfxawyy = false;
	if (1671 == 1671) {
		int fkdlcoeogq;
		for (fkdlcoeogq = 96; fkdlcoeogq > 0; fkdlcoeogq--) {
			continue;
		}
	}
	if (true == true) {
		int loniyn;
		for (loniyn = 18; loniyn > 0; loniyn--) {
			continue;
		}
	}

}

string icwqcdg::ybcdqrcictlc(string dbpacf) {
	int kueemvqxvdks = 453;
	if (453 == 453) {
		int lnvucy;
		for (lnvucy = 54; lnvucy > 0; lnvucy--) {
			continue;
		}
	}
	return string("wrc");
}

double icwqcdg::dlflhrfrbciwysey(int rfknczssbu, string lbddk, bool lvktjmulrlq, string uxobdtznu) {
	double eztjkcwdzmaqke = 869;
	string tndvidbq = "nmxyxzbujyaoejyhmgqlrltnmtsca";
	if (869 != 869) {
		int xqcadn;
		for (xqcadn = 90; xqcadn > 0; xqcadn--) {
			continue;
		}
	}
	return 61279;
}

int icwqcdg::kgjszgwzcoiskc(bool nculbdr, string erapygrldfeym, bool urgfz, bool awvzx, bool broxuutcl) {
	string bgumw = "oeojjrfepmjwt";
	int hkboifjlzglkr = 1300;
	bool sxixklyqxdea = true;
	string omasulwkdim = "sazpmvpkzkzjzsaoqthdmqhjqqjiuwpihvznhntimitte";
	int irgzmvxoalzw = 201;
	double shdnwtmqnqbr = 27987;
	int dkucf = 3858;
	bool rbnqeqklloa = false;
	string awqegjzifuri = "r";
	if (1300 == 1300) {
		int tfvlk;
		for (tfvlk = 90; tfvlk > 0; tfvlk--) {
			continue;
		}
	}
	if (1300 == 1300) {
		int vkakkoi;
		for (vkakkoi = 20; vkakkoi > 0; vkakkoi--) {
			continue;
		}
	}
	if (string("oeojjrfepmjwt") != string("oeojjrfepmjwt")) {
		int ir;
		for (ir = 79; ir > 0; ir--) {
			continue;
		}
	}
	if (201 != 201) {
		int wvfitvxqwx;
		for (wvfitvxqwx = 50; wvfitvxqwx > 0; wvfitvxqwx--) {
			continue;
		}
	}
	if (27987 == 27987) {
		int gzohnexc;
		for (gzohnexc = 99; gzohnexc > 0; gzohnexc--) {
			continue;
		}
	}
	return 28170;
}

double icwqcdg::aamxmdsqjhpblovuhgy(bool dskvxbyu, string auibbjpgrqmflra) {
	bool mvmsaxbtimj = false;
	double wweueiqqgwzax = 21447;
	int jdxtjah = 5638;
	double yxadsn = 32980;
	double wmncaproivigsim = 17986;
	double eoisrlkpeb = 51497;
	double njirsabrgp = 15227;
	if (51497 != 51497) {
		int jhepixrtf;
		for (jhepixrtf = 65; jhepixrtf > 0; jhepixrtf--) {
			continue;
		}
	}
	if (17986 == 17986) {
		int ddtjnhucs;
		for (ddtjnhucs = 17; ddtjnhucs > 0; ddtjnhucs--) {
			continue;
		}
	}
	if (51497 == 51497) {
		int jxmu;
		for (jxmu = 62; jxmu > 0; jxmu--) {
			continue;
		}
	}
	if (32980 == 32980) {
		int zial;
		for (zial = 96; zial > 0; zial--) {
			continue;
		}
	}
	if (32980 == 32980) {
		int wmiwfhpxq;
		for (wmiwfhpxq = 31; wmiwfhpxq > 0; wmiwfhpxq--) {
			continue;
		}
	}
	return 82067;
}

void icwqcdg::qkanctebjnzknhrzcphitynws(int ytwxibhwepuqomw, bool fynwqtaxaweazxc, string mwtsgtfer, string swndjqfm, bool vkncdq) {
	bool fbvvz = true;
	bool slcnloxzrvz = true;
	string jjuulvxbldjacfu = "tqeqstpm";
	if (string("tqeqstpm") != string("tqeqstpm")) {
		int wx;
		for (wx = 8; wx > 0; wx--) {
			continue;
		}
	}
	if (true == true) {
		int qp;
		for (qp = 21; qp > 0; qp--) {
			continue;
		}
	}
	if (true != true) {
		int vk;
		for (vk = 1; vk > 0; vk--) {
			continue;
		}
	}
	if (true != true) {
		int xq;
		for (xq = 21; xq > 0; xq--) {
			continue;
		}
	}

}

string icwqcdg::wfiztqpaeb(int suumsmwkqyricax, bool fbyhp, double yqpimlw, bool csjozji, double rsljkoqbo, double nzshuxqw, string iqslrttjpzgnw, double srqfbxwiawfdpwt, string nlsqcftkzui, int jgkyvmjg) {
	double ztkdmphfaa = 540;
	double iknzhecdhit = 16406;
	double asqbetvhrknrmk = 14265;
	double exeahmn = 6683;
	string bkkgtkbhwomjvf = "jmmanncjmddrtsqipmuxzxlesogpammveprqsujnuzyvnqocjgouizfeznokk";
	string xlulbj = "wln";
	bool zwtjwpgcpkna = true;
	string mbaxfj = "ncavwbdbavzoquiovynm";
	if (6683 != 6683) {
		int zgkaa;
		for (zgkaa = 10; zgkaa > 0; zgkaa--) {
			continue;
		}
	}
	return string("rjoedfnmbmnpp");
}

double icwqcdg::umympujynddbpilpkyouhik(int gaieqnql) {
	int dctmjshlbqag = 2854;
	string tszpr = "vgfqtswxxrmncbobolfjjvhsqojsnpqczujzuixtpaiiizci";
	bool lvqfkuhfp = true;
	bool bdtvlltjxlkqw = false;
	bool tsyacu = false;
	if (false != false) {
		int ckuk;
		for (ckuk = 31; ckuk > 0; ckuk--) {
			continue;
		}
	}
	if (2854 == 2854) {
		int pcrhi;
		for (pcrhi = 49; pcrhi > 0; pcrhi--) {
			continue;
		}
	}
	if (true != true) {
		int gyeyhu;
		for (gyeyhu = 5; gyeyhu > 0; gyeyhu--) {
			continue;
		}
	}
	if (false == false) {
		int feam;
		for (feam = 11; feam > 0; feam--) {
			continue;
		}
	}
	if (2854 != 2854) {
		int aitr;
		for (aitr = 56; aitr > 0; aitr--) {
			continue;
		}
	}
	return 28633;
}

bool icwqcdg::mqbcchnhhopgzni(bool oekhjzukg, bool anzhgtnrxin, int ykstqi, string heabyqt, bool viqyjywipikgnus, int xrtiefcos, bool uxmayzuuo, double iftpkzlqfqmfhku, double rkftxgrftwkpfji) {
	string lgweayeguxtgir = "zhbltqosritdupbztpahnpgjmywwum";
	string yejwgzkffmug = "wcfvmgqybmopylqwyylcb";
	if (string("wcfvmgqybmopylqwyylcb") == string("wcfvmgqybmopylqwyylcb")) {
		int wczgmzxvhl;
		for (wczgmzxvhl = 18; wczgmzxvhl > 0; wczgmzxvhl--) {
			continue;
		}
	}
	if (string("wcfvmgqybmopylqwyylcb") != string("wcfvmgqybmopylqwyylcb")) {
		int spxuc;
		for (spxuc = 46; spxuc > 0; spxuc--) {
			continue;
		}
	}
	if (string("zhbltqosritdupbztpahnpgjmywwum") != string("zhbltqosritdupbztpahnpgjmywwum")) {
		int uftyzkf;
		for (uftyzkf = 17; uftyzkf > 0; uftyzkf--) {
			continue;
		}
	}
	if (string("wcfvmgqybmopylqwyylcb") != string("wcfvmgqybmopylqwyylcb")) {
		int rimvbn;
		for (rimvbn = 25; rimvbn > 0; rimvbn--) {
			continue;
		}
	}
	return true;
}

double icwqcdg::hrjccgavjtagqnyyhxngltewf(string qtuwhpitzophu, double onqakio, string wpexnuclvd, bool vmgcv, double dvnejismn) {
	double npryrbbwoatvv = 7004;
	bool mkuvohwbobw = false;
	string mdfymqzi = "eajpnhrfzyztqiueofpcnniefbfytdsfcbopsmokvdqatjynensnpflfbjtgfhgiumztpczqmstnynbkrjqcpomppz";
	int fjejskluqo = 1004;
	string eexux = "vgsieudsxzejydlicobbmelzrofdqeqyodpxkpi";
	int wpofcp = 2040;
	bool drjvmucaowzd = false;
	if (string("vgsieudsxzejydlicobbmelzrofdqeqyodpxkpi") != string("vgsieudsxzejydlicobbmelzrofdqeqyodpxkpi")) {
		int kzzgbolp;
		for (kzzgbolp = 77; kzzgbolp > 0; kzzgbolp--) {
			continue;
		}
	}
	if (7004 != 7004) {
		int bbtewym;
		for (bbtewym = 49; bbtewym > 0; bbtewym--) {
			continue;
		}
	}
	return 73438;
}

string icwqcdg::bmszlktgdxdxrodpciecobo(int hbpmn, double nddiax, int podigs, string txcmkd) {
	double rgwiptpqing = 40288;
	int dqizbxhyyugg = 2098;
	string grtdbq = "auiurdwewokyonzpclstmdvjbjepbqxswznotzkyofhygigqttfkdeorbbibhpwdbnavdgmxdpjqvdlyozahikc";
	int gvxidyafktnijy = 5285;
	int lwnysrthyvghmn = 1166;
	bool lzwnrlbwcjmgefb = false;
	bool vyqhubkulqvgxvr = false;
	int nvycuzd = 644;
	if (5285 != 5285) {
		int pdxxl;
		for (pdxxl = 98; pdxxl > 0; pdxxl--) {
			continue;
		}
	}
	if (false != false) {
		int ovuoxuoic;
		for (ovuoxuoic = 23; ovuoxuoic > 0; ovuoxuoic--) {
			continue;
		}
	}
	if (1166 == 1166) {
		int cwts;
		for (cwts = 10; cwts > 0; cwts--) {
			continue;
		}
	}
	if (string("auiurdwewokyonzpclstmdvjbjepbqxswznotzkyofhygigqttfkdeorbbibhpwdbnavdgmxdpjqvdlyozahikc") == string("auiurdwewokyonzpclstmdvjbjepbqxswznotzkyofhygigqttfkdeorbbibhpwdbnavdgmxdpjqvdlyozahikc")) {
		int bhtixdp;
		for (bhtixdp = 48; bhtixdp > 0; bhtixdp--) {
			continue;
		}
	}
	return string("goppgzoxoikgudg");
}

double icwqcdg::dwpccntyplzzpyggxtxnp(double vxdhoiyoccvu, string hlcoc, int gllleluxtmygfbs, int rrlfxrhe, string bywkooqzw) {
	int gsywnpu = 511;
	return 69316;
}

int icwqcdg::ymtuwpvrgihmlqwtf(bool nsegykqlt, string yeqhe, bool cdeqfjpnyzeof) {
	string bgamorebkakyhpg = "owqqkicrybpvadaquitgpxj";
	int qslahyqlakhag = 1765;
	string orzjbmkrh = "otawdnxtdocpgsvgonnrkdrnbgnxcgineglldstdmzrcqgcgocbnmfsofzbzejrjxgumfelnwcrrhbvucnwnwdkxgx";
	string nlgeb = "zezviqqchytgzjjzwfxkvuxgyhgbosscldmecilsyebzkvljooxtmjllemqfnlxqiogzkgamtnxtkbvfxefeh";
	int cfvtizk = 1923;
	double idjhza = 7086;
	double steukpjbbtplsab = 61118;
	double pbxhv = 6544;
	if (7086 == 7086) {
		int lezul;
		for (lezul = 48; lezul > 0; lezul--) {
			continue;
		}
	}
	if (1923 != 1923) {
		int twjunsoin;
		for (twjunsoin = 24; twjunsoin > 0; twjunsoin--) {
			continue;
		}
	}
	return 99409;
}

icwqcdg::icwqcdg() {
	this->mqbcchnhhopgzni(true, true, 330, string("dmclpzznqonrhchiovlzmqgoifjwcagpodtoqwntxtfrqwrjoidhuabgkkwttzqlcnwfbtfitzmafqdrizhoazyhsvrbpgs"), false, 157, false, 19258, 26042);
	this->hrjccgavjtagqnyyhxngltewf(string("yrtuofgezkvozxbqqfwnxagwexknd"), 15053, string("qreffel"), true, 31116);
	this->bmszlktgdxdxrodpciecobo(294, 6960, 412, string("nbqtdxfsjrwgqvphbemmlafvezi"));
	this->dwpccntyplzzpyggxtxnp(19826, string("plgqfmchocnjvwbrdbuzembcuevmfgxbld"), 4452, 2214, string("kzpazvccyizekqltbbmgcjohkrhljwttqopmetqgvecwvpcijymdonletqzavojhfdbeipvbemyorsmndwiedtgnvexzzhhcx"));
	this->ymtuwpvrgihmlqwtf(true, string("cqbjfclaamvyhnhegfdkdpirjfppaoiypmqffoainhewvgqlsdptpxfysmv"), true);
	this->aamxmdsqjhpblovuhgy(false, string("mfumjxcmpngwrfewkempjxdkdjlforkpcpqevmoppnpufwdnwitxllqsngxvmgypw"));
	this->qkanctebjnzknhrzcphitynws(3637, false, string("zsczglscckelpvuvbpxeqocvlmsqooqrlxdmlqnttazsqzgebuul"), string("psgykmunplqklxfdjkyztwiuhxofwiacomcgelyvqyravbremtsrdznpwnjjfhnzvauqslrctgktvrgudradddyvvtdzfnjuzshb"), true);
	this->wfiztqpaeb(1578, false, 5507, false, 16802, 8979, string("bcmr"), 17197, string("pytvlmdpmmhlsexkdlmlnhbhrxvmwrjuvvonbaaujrrqoavpecsiouvdxvsohwwqcvugabkuckxyfpmxlhbymt"), 188);
	this->umympujynddbpilpkyouhik(3726);
	this->qkztfcjwnebygmogvbmr(true, 15915, 2794, 2563);
	this->cdpyihsmmlqidglxmzedyk(true);
	this->vnapdenljqebr(false, true, true, 5803, true, 4526, 43056);
	this->gaqfbgogszxzf(861, 4231, false, 20898, string("paaibowhsyndgzsjhuabaqkfqozeqhdpikyuyjgmrmmoaegjjvznptgymlwaj"), 66832);
	this->ngmrbnpzndlatr(false, string("znhjodpkzlylxgmusdomxcrsynneirssgkjlwcxxejiigvimxvkcyzwvvpfelpmlkdjdvmq"), true, 1015, 13381, 12402, 879, 28769, false);
	this->nldxrypjlkhzockkcpllz(69338, string("egiupeubyqhrwtyghzjtmojcjyhgzcmfqlqfylnowfmqomvctdnebxihbsoddrueddtxlgddp"), string("dkdbhssiavyeslmgqydfesursdlwjtjuluwmmlyyakjgdobqpxigs"));
	this->ybcdqrcictlc(string("uxrqirkavggnjhlepdzuxefqureqctrgokobmlsaotmetzxshzejwevroqidxkfzbbjkmwvewhagnqzkrtuincomhjewcdhhnpz"));
	this->dlflhrfrbciwysey(6955, string("nyajgyyblrxnjfitombmmvcnovegzlrabmyocgkxoufehwnfhqrx"), true, string("qjmrszzwgaxvbawcqhkbmpevcomszczxkoevcvitzxcpvwhdnzpaecm"));
	this->kgjszgwzcoiskc(true, string("cebkirowhmhqcwurioqnqiyatnzusnacnqxdmmtpoqaquyekskbsidjyamcvdadohxlsjhiksvkhybsvjrjjpkzfiwanejpgkl"), true, false, false);
}
