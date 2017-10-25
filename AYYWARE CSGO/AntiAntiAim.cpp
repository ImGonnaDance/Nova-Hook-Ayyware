
#include "Interfaces.h"
#include "Menu.h"
#include "Hacks.h"

#include "Resolver.h"

void FixY(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float *flPitch = (float*)((DWORD)pOut - 4);
	float flYaw = pData->m_Value.m_Float;
	IClientEntity* player = (IClientEntity*)pStruct;
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (!player || !pLocal) {
		return;
	}
	*(float*)(pOut) = flYaw;

}

void FixX(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	float* ang = (float*)pOut;
	*ang = pData->m_Value.m_Float;


	if (pData->m_Value.m_Float > 180.0f)
		*ang -= 360.0f;
	else if (pData->m_Value.m_Float < -180.0f)
		*ang += 360.0f;

	if (pData->m_Value.m_Float > 89.0f && pData->m_Value.m_Float < 91.0f)
		*ang -= 90.0f;
	else if (pData->m_Value.m_Float > -89.0f && pData->m_Value.m_Float < -91)
		*ang += 90.0f;

	if (*ang < -179.648438f || *ang > 179.648438f)
	{
		*ang = -10.0f;
	}

	if (*ang <= -88.945313f && *ang >= -179.648438f)
	{
		*ang = -89.0f;
	}

	if (*ang >= 88.945313f && *ang <= 179.648438f)
	{
		*ang = 89.0f;
	}


	if (*ang > 89)
	{
		*ang = 270;
	}
	if (*ang < -89)
	{
		*ang = 90;
	}

	if (*ang > 36000088.000000)
	{
		*ang = 89;
	}

	if (*ang > 1800089)
	{
		*ang = 89;
	}

}


#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

ayyResolver* g_Resolver1 = new ayyResolver;
#include "Proxies.h"
static float OldLBYEnemys[65];
void LowerBodyYawHook(const CRecvProxyData* data, void *pStruct, void *out)
{
	

	IClientEntity* player = (IClientEntity*)pStruct;
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	static float savedLBY;


	if (!pLocal)
		return;
	
	if (player != pLocal && player->GetTeamNum() != pLocal->GetTeamNum()) {
		
		float NowLBY = data->m_Value.m_Float;
		float testLBY = NowLBY;

		if (NowLBY > 180.0)
		{
			do
				testLBY = testLBY - 360.0;
			while (testLBY > 180.0);
			NowLBY = testLBY;
		}
		if (testLBY < -180.0)
		{
			do
				testLBY = testLBY + 360.0;
			while (testLBY < -180.0);
			NowLBY = testLBY;
		}
		enemysLastProxyTimer[player->GetIndex()] = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick - nci->GetAvgLatency(FLOW_OUTGOING);

		if (OldLBYEnemys[player->GetIndex()] != NowLBY) {
			// lby Updated
			OldLBYEnemys[player->GetIndex()] = NowLBY;
			enemysLbyUpdatedIndicator[player->GetIndex()] = true;
			//Interfaces::CVar->ConsoleColorPrintf(Color(0, 255, 0, 255), "Player: %i - LBY\n", player->GetIndex());
			*(float*)out = NowLBY;
		}
		else {
			// Not Updated use old
			//Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 0, 255), "Player: %i - LBY\n", player->GetIndex());
			enemysLbyUpdatedIndicator[player->GetIndex()] = false;
			*(float*)out = OldLBYEnemys[player->GetIndex()];
			
		}

		
		

	}


	
	if (player == pLocal) {
		float NowLBY = data->m_Value.m_Float;
		consoleProxyLbyLASTUpdateTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick - nci->GetAvgLatency(FLOW_OUTGOING);
		testFloat2 = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;
		float testLBY;
		testLBY = NowLBY;
		if (NowLBY > 180.0)
		{
			do
				testLBY = testLBY - 360.0;
			while (testLBY > 180.0);
			NowLBY = testLBY;
		}
		if (testLBY < -180.0)
		{
			do
				testLBY = testLBY + 360.0;
			while (testLBY < -180.0);
			NowLBY = testLBY;
		}
		
		*(float*)out = NowLBY;

	}

			
}




RecvVarProxyFn oRecvnModelIndex;

void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	// Get the knife view model id's
	int default_t = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	int iGunGame = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	// Get local player (just to stop replacing spectators knifes)
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Menu::Window.MiscTab.SkinEnable.GetState() && pLocal)
	{
		// If we are alive and holding a default knife(if we already have a knife don't worry about changing)
		if (pLocal->IsAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == iBowie))
		{
			// Set whatever knife we want
			if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 0)
				pData->m_Value.m_Int = iBayonet;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 1)
				pData->m_Value.m_Int = iBowie;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 2)
				pData->m_Value.m_Int = iButterfly;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 3)
				pData->m_Value.m_Int = iFalchion;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 4)
				pData->m_Value.m_Int = iFlip;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 5)
				pData->m_Value.m_Int = iGut;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 6)
				pData->m_Value.m_Int = iHuntsman;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 7)
				pData->m_Value.m_Int = iKarambit;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 8)
				pData->m_Value.m_Int = iM9Bayonet;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 9)
				pData->m_Value.m_Int = iDagger;
		}
	}

	// Carry on the to original proxy
	oRecvnModelIndex(pData, pStruct, pOut);
}





void ApplyAAAHooks()
{
	ClientClass *pClass = Interfaces::Client->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
		if (!strcmp(pszName, "DT_CSPlayer"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				if (!strcmp(name, "m_flLowerBodyYawTarget"))
				{
					pProp->m_ProxyFn = LowerBodyYawHook;
				}

				// Pitch Fix
				if (!strcmp(name, "m_angEyeAngles[0]"))
				{
					pProp->m_ProxyFn = FixX;
				}

				// Yaw Fix
				if (!strcmp(name, "m_angEyeAngles[1]"))
				{
					Utilities::Log("Yaw Fix Applied");
					pProp->m_ProxyFn = FixY;
				}
			}
		}
		else if (!strcmp(pszName, "DT_BaseViewModel"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				// Knives
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
				}

				


			}
			for (ClientClass* pClass = Interfaces::Client->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
				if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel")) {
					// Search for the 'm_nModelIndex' property.
					RecvTable* pClassTable = pClass->m_pRecvTable;

					for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
						RecvProp* pProp = &pClassTable->m_pProps[nIndex];

						if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
							continue;

						// Store the original proxy function.
						fnSequenceProxyFn = (RecvVarProxyFn)pProp->m_ProxyFn;

						// Replace the proxy function with our sequence changer.
						pProp->m_ProxyFn = SetViewModelSequence;

						break;
					}

					break;
				}
			}
		}
		pClass = pClass->m_pNext;
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qigrjdi {
public:
	int laumopuzonrzg;
	int hisnrqbiox;
	bool bvpitucbodjn;
	qigrjdi();
	bool xzwjwhcgphyfvdbdficgis(bool ogjombxwnfwz, double ullsde, string kqgcrjlbqanx);
	bool hxcihksfjwubbmnklcoym(bool dozllzaomj, bool itprobezixhgd, double mpfnppfqccjq, int ongsxba);
	void qmypenbwrvoezuivbqazmcghu(bool ztwltbpwjvnxhe, bool vanzwmfkop, double ugkkoroonpby, double noxzhcgph, string dhatuutziqmhq, int bdmpzhwoot, bool khbqwoxenvzlkdv, bool ekdqzol);
	double dieirlvwpmwifgl(string aezbarbrynkiiq, string bmwrwjsilrrlwaj, int dapqekxgfqmueyw, bool qqdouenlmr, int wotkvvcczep, int vdnjje, double ttjkprvknx, int ikkmutglgmexfj);
	double oteohmwnpbeeksakzgstuth(string cpespz, bool vthfwsdzfr, bool luzzvflooxrdvh, int drwblzwjkhxlb, string sueaczcxxzk, int rphcpepssn, double isbntgulosrtil, string aaezasdyqv, string sqxmvuzsq, bool uezsyodhclje);

protected:
	string rirnxc;
	string welmybhv;
	string fogtzgmbvlf;

	string iadeafxrjahbg(int zuxlaty, double censexa, int cqtea);
	double cvsftdocxguphmoursbb(double helgiqtpis);
	bool tnwiurbcai(double vktegjbopaqq, double bercloce, int ldppz, double lddeiirwkwtujbi, int glbvccjtu, double zovilcqwzqirng, int udolyiulzn, double xyxjauhmp);
	int atgtohecoejjvhcvecwaitwai(string tjvisxj, int axehhma, string gtamxwo, double nbfjypzilwgg, double thrtxn, bool aevkjm, bool yymsfioe);
	string ueqojnkkbzzegclfqltaxao(int uiytpahd, int ykxgjpvudaxbpa, string yomofgwheg, int khlgofwvyp, string pqbihhlawr, double smgwkrose);
	int chimkxqrdqjxoxcqbgaqfhqlb(int yexnio, int ahgodccbv, string xqhld, int lkgjcqmctt, double mughgoplxxhgf);
	int hdzxctcrrmtzvxdpqsozimeh();

private:
	double ohaweomz;
	string gjccerfwfecn;
	bool coflwgys;

	bool effysreroau();
	string jdjysmjxabkrvarliajj(int cbwlzo, int rzvwpe, bool bwbsyyztfk, double fcibxiwwlkoumr, int rpaigbxm, double ctlirmqkbvzkx, int zojszpipiqyfve, bool apsgylar, bool egkctyxlkrp, bool zmvuhbkbbchtp);
	int itvgiefcbzgmsbjkb(string agqks, int lbvzxxwxgre);
	int iimuuqtaqjkgoiwaorqmlzdt(double vbnxxajgrrlt, int mmjgrs, double qwnizfsnznr, bool whbam);

};


bool qigrjdi::effysreroau() {
	return false;
}

string qigrjdi::jdjysmjxabkrvarliajj(int cbwlzo, int rzvwpe, bool bwbsyyztfk, double fcibxiwwlkoumr, int rpaigbxm, double ctlirmqkbvzkx, int zojszpipiqyfve, bool apsgylar, bool egkctyxlkrp, bool zmvuhbkbbchtp) {
	double jrpyoxkkf = 67985;
	bool tffzvjo = false;
	string kdwym = "bjsetygohtwquxrmuvrpmohftebnfnvnnvdqeoxbmbbbewwyrbvihbngpnzxtmwtp";
	double pqttzdcchgqi = 29252;
	double mtemirrebnpd = 15150;
	bool gxtgyohsgszk = false;
	string xjkyoocvgcyim = "epjlqythmfawsgavcqdunsdwcecjsxjqwrqqwgzysalnwyuffiqspodexylmmsfsliqzotyasmiqlbelea";
	string ftwfyofkbrjivui = "apvfgbcysaypmcdgzmumhgyuiiqicyivgi";
	bool dtzakibcionwwc = false;
	string ghffti = "pgmbaszkxqtkknttpkhjdhgzhuokmzjawqrewxqzkudkzlfofkbmenlzaimkqfttvngbsoffcn";
	if (string("pgmbaszkxqtkknttpkhjdhgzhuokmzjawqrewxqzkudkzlfofkbmenlzaimkqfttvngbsoffcn") == string("pgmbaszkxqtkknttpkhjdhgzhuokmzjawqrewxqzkudkzlfofkbmenlzaimkqfttvngbsoffcn")) {
		int yfgzddbuw;
		for (yfgzddbuw = 22; yfgzddbuw > 0; yfgzddbuw--) {
			continue;
		}
	}
	if (string("pgmbaszkxqtkknttpkhjdhgzhuokmzjawqrewxqzkudkzlfofkbmenlzaimkqfttvngbsoffcn") == string("pgmbaszkxqtkknttpkhjdhgzhuokmzjawqrewxqzkudkzlfofkbmenlzaimkqfttvngbsoffcn")) {
		int kzlt;
		for (kzlt = 85; kzlt > 0; kzlt--) {
			continue;
		}
	}
	if (false != false) {
		int ehic;
		for (ehic = 68; ehic > 0; ehic--) {
			continue;
		}
	}
	return string("dmnostuomfmwxmjfhnb");
}

int qigrjdi::itvgiefcbzgmsbjkb(string agqks, int lbvzxxwxgre) {
	bool lqregsclw = false;
	double iahmrbxgjnihsit = 7075;
	int fcbym = 1897;
	bool nqfxayvedm = false;
	double nbdahx = 38208;
	double gvibwauiaodo = 19237;
	double bbrffyasbc = 9046;
	bool eulpzgatuwogyug = true;
	return 98732;
}

int qigrjdi::iimuuqtaqjkgoiwaorqmlzdt(double vbnxxajgrrlt, int mmjgrs, double qwnizfsnznr, bool whbam) {
	int ckykdcrejoan = 3719;
	string aqqnvxhpxndema = "gzmojrwkyvghoypezoknycroqtltrjryttzydzzxjhmzoewinczezpqhzcsernlhabcwjkbxffuuvzahfsxihrhckujfeocqa";
	int sxejkzkbncizfw = 1111;
	string gmisitcwhpyb = "wkxbarbxvwmftjzyuziemhbmpiym";
	string kjfkg = "pspqmkiikvmhxabdgg";
	string nbhgmvbu = "dlcevntqtpvfdcqtuyjterccntxkgygpamtfvm";
	bool xpwahrhclhypw = false;
	double ownkzydi = 3526;
	int iuuxnsqojykyjse = 158;
	if (1111 == 1111) {
		int gr;
		for (gr = 30; gr > 0; gr--) {
			continue;
		}
	}
	if (158 != 158) {
		int tradn;
		for (tradn = 49; tradn > 0; tradn--) {
			continue;
		}
	}
	if (3719 == 3719) {
		int lkaxcmqdy;
		for (lkaxcmqdy = 83; lkaxcmqdy > 0; lkaxcmqdy--) {
			continue;
		}
	}
	return 42028;
}

string qigrjdi::iadeafxrjahbg(int zuxlaty, double censexa, int cqtea) {
	int zluweckqgrfnr = 4065;
	return string("pug");
}

double qigrjdi::cvsftdocxguphmoursbb(double helgiqtpis) {
	double anandgftktiu = 45768;
	string ejwrc = "qxekjevpkmhimv";
	double wzilwhd = 24445;
	double witcau = 20581;
	int gtarziotonjl = 1607;
	bool piyfgqk = true;
	int oygcldzftbyxnml = 557;
	string znifqtsapmt = "rwflgpfnfcwgjrmeikuqcjkpeocijnzfcyywknbkdxfbgovbjbvvcafugvnyqeqezqtzlzuttx";
	bool ruwikbwsi = false;
	if (true == true) {
		int jagluj;
		for (jagluj = 82; jagluj > 0; jagluj--) {
			continue;
		}
	}
	if (20581 == 20581) {
		int llfzzps;
		for (llfzzps = 52; llfzzps > 0; llfzzps--) {
			continue;
		}
	}
	if (string("rwflgpfnfcwgjrmeikuqcjkpeocijnzfcyywknbkdxfbgovbjbvvcafugvnyqeqezqtzlzuttx") != string("rwflgpfnfcwgjrmeikuqcjkpeocijnzfcyywknbkdxfbgovbjbvvcafugvnyqeqezqtzlzuttx")) {
		int xp;
		for (xp = 18; xp > 0; xp--) {
			continue;
		}
	}
	if (string("rwflgpfnfcwgjrmeikuqcjkpeocijnzfcyywknbkdxfbgovbjbvvcafugvnyqeqezqtzlzuttx") != string("rwflgpfnfcwgjrmeikuqcjkpeocijnzfcyywknbkdxfbgovbjbvvcafugvnyqeqezqtzlzuttx")) {
		int liqkzpzuz;
		for (liqkzpzuz = 70; liqkzpzuz > 0; liqkzpzuz--) {
			continue;
		}
	}
	if (45768 == 45768) {
		int bvv;
		for (bvv = 17; bvv > 0; bvv--) {
			continue;
		}
	}
	return 70464;
}

bool qigrjdi::tnwiurbcai(double vktegjbopaqq, double bercloce, int ldppz, double lddeiirwkwtujbi, int glbvccjtu, double zovilcqwzqirng, int udolyiulzn, double xyxjauhmp) {
	int vgdxysthgxlcuci = 584;
	bool xipveeik = false;
	if (584 == 584) {
		int iqd;
		for (iqd = 80; iqd > 0; iqd--) {
			continue;
		}
	}
	if (false == false) {
		int rgfjmnwny;
		for (rgfjmnwny = 19; rgfjmnwny > 0; rgfjmnwny--) {
			continue;
		}
	}
	if (584 != 584) {
		int dfhgg;
		for (dfhgg = 10; dfhgg > 0; dfhgg--) {
			continue;
		}
	}
	if (false != false) {
		int xoxhla;
		for (xoxhla = 33; xoxhla > 0; xoxhla--) {
			continue;
		}
	}
	if (false == false) {
		int qnxhda;
		for (qnxhda = 26; qnxhda > 0; qnxhda--) {
			continue;
		}
	}
	return true;
}

int qigrjdi::atgtohecoejjvhcvecwaitwai(string tjvisxj, int axehhma, string gtamxwo, double nbfjypzilwgg, double thrtxn, bool aevkjm, bool yymsfioe) {
	int aioxpzn = 745;
	double maypgu = 28944;
	string cahft = "dzgrwviyjxeiktmfhxfxrmbbtlelzdjpbbq";
	int quazrh = 5488;
	int tnjrkliygnfsbh = 2486;
	bool katfgntox = false;
	if (string("dzgrwviyjxeiktmfhxfxrmbbtlelzdjpbbq") == string("dzgrwviyjxeiktmfhxfxrmbbtlelzdjpbbq")) {
		int ddiby;
		for (ddiby = 78; ddiby > 0; ddiby--) {
			continue;
		}
	}
	return 84743;
}

string qigrjdi::ueqojnkkbzzegclfqltaxao(int uiytpahd, int ykxgjpvudaxbpa, string yomofgwheg, int khlgofwvyp, string pqbihhlawr, double smgwkrose) {
	bool kbmmhw = true;
	string bliivypvyx = "kwcixki";
	double erlyogo = 44467;
	int iuenjtpxuwxzfi = 10;
	bool taaqbglo = false;
	if (44467 != 44467) {
		int hxpa;
		for (hxpa = 8; hxpa > 0; hxpa--) {
			continue;
		}
	}
	if (string("kwcixki") == string("kwcixki")) {
		int abrukha;
		for (abrukha = 1; abrukha > 0; abrukha--) {
			continue;
		}
	}
	if (true == true) {
		int nfom;
		for (nfom = 0; nfom > 0; nfom--) {
			continue;
		}
	}
	return string("trluahry");
}

int qigrjdi::chimkxqrdqjxoxcqbgaqfhqlb(int yexnio, int ahgodccbv, string xqhld, int lkgjcqmctt, double mughgoplxxhgf) {
	double pzzgkzoqwcpi = 66800;
	double obwgqhzbaaf = 47769;
	double gemlzfufxcj = 31086;
	string zpwdsnqqa = "svvwwmfrmflebzacmgqfegdzyoyrfazniqbggdff";
	int vnxxwqmttp = 975;
	bool fewqgtwvnc = false;
	if (66800 != 66800) {
		int sydzanhan;
		for (sydzanhan = 72; sydzanhan > 0; sydzanhan--) {
			continue;
		}
	}
	if (string("svvwwmfrmflebzacmgqfegdzyoyrfazniqbggdff") == string("svvwwmfrmflebzacmgqfegdzyoyrfazniqbggdff")) {
		int iuwozu;
		for (iuwozu = 37; iuwozu > 0; iuwozu--) {
			continue;
		}
	}
	if (47769 != 47769) {
		int gigcjgv;
		for (gigcjgv = 7; gigcjgv > 0; gigcjgv--) {
			continue;
		}
	}
	if (string("svvwwmfrmflebzacmgqfegdzyoyrfazniqbggdff") != string("svvwwmfrmflebzacmgqfegdzyoyrfazniqbggdff")) {
		int xyzhpqg;
		for (xyzhpqg = 8; xyzhpqg > 0; xyzhpqg--) {
			continue;
		}
	}
	return 33852;
}

int qigrjdi::hdzxctcrrmtzvxdpqsozimeh() {
	double omvxkmjkustcmqg = 28015;
	double gpxbfuwxefu = 434;
	string ncwtxudnlyqzl = "xlfnvrseseaixyqsnlpsfojwakkohmkuiuicgyhkdhhdcxcgztpevrfogmxbtiwfjeqwo";
	bool koqyist = false;
	return 17844;
}

bool qigrjdi::xzwjwhcgphyfvdbdficgis(bool ogjombxwnfwz, double ullsde, string kqgcrjlbqanx) {
	double qeqxqtdpkhuh = 3948;
	bool kiqjodjloyts = false;
	string whbcigbzngfwkki = "kzoidgsyxklhyolqpskskkeuyrvzopwlgmmgizjlxevswv";
	int wocypaxcreuxmxg = 1217;
	double lgjfdtsnmouvf = 18250;
	if (1217 != 1217) {
		int tggvzjrtws;
		for (tggvzjrtws = 40; tggvzjrtws > 0; tggvzjrtws--) {
			continue;
		}
	}
	if (3948 == 3948) {
		int wfa;
		for (wfa = 45; wfa > 0; wfa--) {
			continue;
		}
	}
	return true;
}

bool qigrjdi::hxcihksfjwubbmnklcoym(bool dozllzaomj, bool itprobezixhgd, double mpfnppfqccjq, int ongsxba) {
	double rhacn = 27400;
	int dwxmuugdqvfc = 4498;
	int meoeb = 5807;
	int xlspjcqbanhdga = 3039;
	string wflimyxvvlna = "jjsgorcqsicfhwgccergksellvwuqlbwudbbeqevndktoileieyeppjilkzjlobssscwreqtlxzvgfjw";
	int csslcqvrjqkkhv = 84;
	string dnwrhsjwbu = "ktshcnvtfhvygafjixjbiddedurdnfxwsnwjvnjfkugfvlbn";
	string kdvwdvhjsq = "tkmoxdsrcbbqvkqnzxipfwqjcufcmcogrlawcxdikoijzqmwkcapnixgbapqyzuimnacrwgweef";
	bool oirjjmlqtvapx = true;
	if (string("tkmoxdsrcbbqvkqnzxipfwqjcufcmcogrlawcxdikoijzqmwkcapnixgbapqyzuimnacrwgweef") != string("tkmoxdsrcbbqvkqnzxipfwqjcufcmcogrlawcxdikoijzqmwkcapnixgbapqyzuimnacrwgweef")) {
		int zsmlaydcix;
		for (zsmlaydcix = 17; zsmlaydcix > 0; zsmlaydcix--) {
			continue;
		}
	}
	return false;
}

void qigrjdi::qmypenbwrvoezuivbqazmcghu(bool ztwltbpwjvnxhe, bool vanzwmfkop, double ugkkoroonpby, double noxzhcgph, string dhatuutziqmhq, int bdmpzhwoot, bool khbqwoxenvzlkdv, bool ekdqzol) {
	bool bjptj = true;
	bool dodstgjaifstc = true;
	string tqrgoolz = "vjrbwrrgmlcnavbgvrxojqencenyqwhgcmqiphxmgyjdolj";
	if (true != true) {
		int bdwpwex;
		for (bdwpwex = 42; bdwpwex > 0; bdwpwex--) {
			continue;
		}
	}
	if (true != true) {
		int pwlfl;
		for (pwlfl = 35; pwlfl > 0; pwlfl--) {
			continue;
		}
	}
	if (true != true) {
		int rtdshqqikj;
		for (rtdshqqikj = 15; rtdshqqikj > 0; rtdshqqikj--) {
			continue;
		}
	}
	if (true == true) {
		int lc;
		for (lc = 16; lc > 0; lc--) {
			continue;
		}
	}
	if (true == true) {
		int sstajcafqj;
		for (sstajcafqj = 91; sstajcafqj > 0; sstajcafqj--) {
			continue;
		}
	}

}

double qigrjdi::dieirlvwpmwifgl(string aezbarbrynkiiq, string bmwrwjsilrrlwaj, int dapqekxgfqmueyw, bool qqdouenlmr, int wotkvvcczep, int vdnjje, double ttjkprvknx, int ikkmutglgmexfj) {
	double wmtru = 32097;
	int ffhpzagyuub = 985;
	if (985 == 985) {
		int rblxbhp;
		for (rblxbhp = 24; rblxbhp > 0; rblxbhp--) {
			continue;
		}
	}
	return 88762;
}

double qigrjdi::oteohmwnpbeeksakzgstuth(string cpespz, bool vthfwsdzfr, bool luzzvflooxrdvh, int drwblzwjkhxlb, string sueaczcxxzk, int rphcpepssn, double isbntgulosrtil, string aaezasdyqv, string sqxmvuzsq, bool uezsyodhclje) {
	bool czyrh = false;
	double cnbvzdgplzbhms = 23919;
	if (false != false) {
		int wictma;
		for (wictma = 94; wictma > 0; wictma--) {
			continue;
		}
	}
	if (23919 == 23919) {
		int xenefj;
		for (xenefj = 27; xenefj > 0; xenefj--) {
			continue;
		}
	}
	if (23919 != 23919) {
		int xp;
		for (xp = 56; xp > 0; xp--) {
			continue;
		}
	}
	if (false == false) {
		int ow;
		for (ow = 36; ow > 0; ow--) {
			continue;
		}
	}
	return 20502;
}

qigrjdi::qigrjdi() {
	this->xzwjwhcgphyfvdbdficgis(true, 43915, string("jvgsflvhjuvfwpiwnhjliuujhtylaplkfdhlvapuromtvrslohfzvbuskgjnxznzvtnobutrawrbzflbtk"));
	this->hxcihksfjwubbmnklcoym(true, false, 51186, 2300);
	this->qmypenbwrvoezuivbqazmcghu(true, true, 5632, 13237, string("rjflydhrotvesetfcbwptbgdoinbqhlffdqehlonvuwqqiqudnnfwvnjycikzjpftnpldrziqwxvflrvogitsqmswwqbgoze"), 3461, true, true);
	this->dieirlvwpmwifgl(string("hjkchjyosrfjscnzx"), string("rxhelbkvlecpptyyqoatfmzlxpcyufocggwvkudlyi"), 1689, false, 1208, 4385, 30110, 7108);
	this->oteohmwnpbeeksakzgstuth(string("pxjnyhkgrctxziuibya"), false, true, 5813, string("aulsbxf"), 2070, 9917, string("aqjgnkdozavktogznrmvfksoxulkvuptgujstyqajiakqqxfmjiazg"), string("zfgyqyxaeefgtkpgurjddxmuvhovgxlwuzetsivirfzrjkdwjeprnmghevwzpwocsuprzeezhxgzsvyprvudtqiwu"), true);
	this->iadeafxrjahbg(763, 7753, 1002);
	this->cvsftdocxguphmoursbb(31630);
	this->tnwiurbcai(8079, 85705, 1272, 5541, 1009, 17504, 1180, 35108);
	this->atgtohecoejjvhcvecwaitwai(string("pmhpzlbfkbcypswksafjlzqffgwlyczejel"), 1433, string("ndlcjhybx"), 25669, 8514, true, false);
	this->ueqojnkkbzzegclfqltaxao(880, 5476, string("hlcilbqagsfpmboctgffzvaf"), 1534, string("lwevgfugsord"), 61663);
	this->chimkxqrdqjxoxcqbgaqfhqlb(838, 3066, string("bmaeeucdwfodbidsgttwapppjhwdlxrroqsrlrwlqichnewssyyzosnzotiwbixoyxzjyvebotiyk"), 3207, 2716);
	this->hdzxctcrrmtzvxdpqsozimeh();
	this->effysreroau();
	this->jdjysmjxabkrvarliajj(3172, 2894, false, 32462, 618, 20119, 4673, false, false, true);
	this->itvgiefcbzgmsbjkb(string("haarilvteqxadlvivksjetvhulxryimxgyurfvutqebmgepsefwtvpwbkircmlzy"), 4674);
	this->iimuuqtaqjkgoiwaorqmlzdt(22259, 1138, 38724, false);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zpvexhc {
public:
	int zqvainhid;
	zpvexhc();
	string tzyohxjeggleuwdfynclwsqxh(double nzoijtnzxj, double gvmip, bool ttipgdtlxj, double keyautluxs, bool xheonmg, string cadexpctusc, bool nxnnb, double kzmplfollymkuwl);
	void ahanxcfrmlav(int xryyl, double gctglknpyivaf, string zgbpu, double xydri, double uulfnvzarrei, bool zrseoshhiutkijb);
	double tcmcxzmtiwojeewbvkdcki(double gcpytdou, bool eqoylo, double kghbabpcfeg, bool ptqdmxh, string anwtzkbjbzpz);
	bool qxuhizslesrlvtzvoewqghhq(int ranayqa, string ducvvtpl, double tzqzwgnommtrf, double raosssno, double meazaeumnbn, bool weqvligum);
	void howxhfbjjlogfc(string vpdesmdtg);
	void nckozlrppjwcfnv(string qjoktazjeklfhm, double rxnznuibzv, bool zhxovdq, double ewzifauozmz, int ndcfa, double jqswve, int qltradchu);
	bool wqumhazieutytwbwzyu(double bkjifeky);
	int bqkcctdbabn(bool sbcajzjgf, string jcgll, string jphgfcempkwvtkt, bool gkmqthjsij, bool gitroz);
	int pzgrktcfijkodzbfskqtbjb();
	bool gcmwmqhvnd(string jobslqbe, int dmeuhddqzkqb, double ikvhnnd, int kcvgskqthahn, double fstmkuhn);

protected:
	int yglnocoolja;
	int sgqlpyq;
	double cvbrxbsucocnc;
	string zveaouw;
	string ucylfx;

	int dczvhwqyonyzonufxlsbikqhd(string yaogebsi, bool ddlinpuemmcqn, string bafrvwnt, double pgewk, bool ileazyuwcn, double xaxrgrnqkip, int mxsizv);
	void eixbithgfqnjui(bool fmnxthwgylf, bool vikpjmltfkog, string phjjv, double egfsybqsuibmqi, bool wvzraxp, int jdhvd, int tecfzzfiiuqn, int omtijeeusdleo);
	bool lhfbwupzwjihrjp(string bncpfg);
	double hthdxjtmanvvbabxib(string jyruqn, bool kadgji, bool ppxkqfugzmwv, double tdohjarmozh);
	bool gpbpkngmwgfpllssrzwg(int tkijk, double wtxqlpyziyj, int zktynqfcf, string htymanvs, string rsyjsh, string olnpalriyuhg, int xwvzxnirglz);
	string lrzqmwzrlqvbqobjvjfx(int qhjjhk, string syyeaeoftjqlex, string xvwqrhurry, bool abypjhwhixzhgwb);
	string stluahzpar();
	string jakymltubewvwvxuuezti(string yktok, int rnzolsfas, bool jaqymilxrxfko, string zorbnr, string cyonyyerybsprn, bool xubywyo, string cizctwfcxbwgxwp, string dvzfarnruzo);
	string buvczmhhwcbgbdhdpt(double pcopymfd);

private:
	string ebnaoa;
	bool isnixs;
	bool slsru;

	string kfbzryjybyzonzwljfl(string vhvvbwbibbwugzg, bool ngescfuewvqzo, string miigaylembxvfr, bool syhxdjebcfvb, double rixlnaowisslxlg, bool pbqjuhgzz, double lmhpbbz, bool kprngkkpmvpcqve);
	bool ftkjmemwcrsrxdtpycy(int pjigfcqdentd, string rlzhva, int irobygmaba, double vydnwvzrq, string lctvofrya, int qwlagcxlztbkwvd, string yotjarc, double awhinv, double npelhryslunm);
	void qvhrmvmytfiye(string nxrsx, bool kbpekzvflmmtx, bool yvlenwk, double tejhcgflqhkzmf, double bxbfbuxydrzy, int kixqhtt, int fvdgyprjanksao, string cehsahs, bool pcioryxzbuwa);
	string rqnilfxjuhwguhjau(double zwoqywohd, double asqvjbccflzvrm, int xhnxptaovjjewz);

};


string zpvexhc::kfbzryjybyzonzwljfl(string vhvvbwbibbwugzg, bool ngescfuewvqzo, string miigaylembxvfr, bool syhxdjebcfvb, double rixlnaowisslxlg, bool pbqjuhgzz, double lmhpbbz, bool kprngkkpmvpcqve) {
	int wliqqt = 1577;
	double phthvrzdlil = 53357;
	double gkvarjf = 91530;
	int pbrqn = 3735;
	return string("naqqmtsshxr");
}

bool zpvexhc::ftkjmemwcrsrxdtpycy(int pjigfcqdentd, string rlzhva, int irobygmaba, double vydnwvzrq, string lctvofrya, int qwlagcxlztbkwvd, string yotjarc, double awhinv, double npelhryslunm) {
	bool diqklslnvg = true;
	bool gdyvcrfprhy = false;
	if (false != false) {
		int yqgsgv;
		for (yqgsgv = 56; yqgsgv > 0; yqgsgv--) {
			continue;
		}
	}
	return true;
}

void zpvexhc::qvhrmvmytfiye(string nxrsx, bool kbpekzvflmmtx, bool yvlenwk, double tejhcgflqhkzmf, double bxbfbuxydrzy, int kixqhtt, int fvdgyprjanksao, string cehsahs, bool pcioryxzbuwa) {
	double bwcbvutacpbybx = 58874;
	bool kybkzidha = true;
	if (58874 != 58874) {
		int iuiunxkqha;
		for (iuiunxkqha = 72; iuiunxkqha > 0; iuiunxkqha--) {
			continue;
		}
	}
	if (58874 == 58874) {
		int foja;
		for (foja = 54; foja > 0; foja--) {
			continue;
		}
	}
	if (58874 != 58874) {
		int fcrgpnhd;
		for (fcrgpnhd = 10; fcrgpnhd > 0; fcrgpnhd--) {
			continue;
		}
	}
	if (58874 == 58874) {
		int maerp;
		for (maerp = 52; maerp > 0; maerp--) {
			continue;
		}
	}
	if (58874 == 58874) {
		int ccqdzqkss;
		for (ccqdzqkss = 15; ccqdzqkss > 0; ccqdzqkss--) {
			continue;
		}
	}

}

string zpvexhc::rqnilfxjuhwguhjau(double zwoqywohd, double asqvjbccflzvrm, int xhnxptaovjjewz) {
	double eusaimertl = 37365;
	double dmucpegef = 22423;
	double pcrnqqvvc = 62202;
	int jmihcxqxzvr = 2116;
	if (2116 == 2116) {
		int xwzxbq;
		for (xwzxbq = 0; xwzxbq > 0; xwzxbq--) {
			continue;
		}
	}
	if (37365 != 37365) {
		int egzl;
		for (egzl = 38; egzl > 0; egzl--) {
			continue;
		}
	}
	if (22423 != 22423) {
		int mxknh;
		for (mxknh = 96; mxknh > 0; mxknh--) {
			continue;
		}
	}
	if (62202 == 62202) {
		int koflghde;
		for (koflghde = 90; koflghde > 0; koflghde--) {
			continue;
		}
	}
	if (37365 != 37365) {
		int pir;
		for (pir = 29; pir > 0; pir--) {
			continue;
		}
	}
	return string("xzangzueejouk");
}

int zpvexhc::dczvhwqyonyzonufxlsbikqhd(string yaogebsi, bool ddlinpuemmcqn, string bafrvwnt, double pgewk, bool ileazyuwcn, double xaxrgrnqkip, int mxsizv) {
	bool sqkpnvmnc = true;
	int xaontid = 5093;
	double zxaifo = 28800;
	bool jxwhxkhuldcrs = false;
	bool lxtcntxyilx = true;
	double qlhldgjbadbs = 35213;
	if (28800 != 28800) {
		int kwfyurbuz;
		for (kwfyurbuz = 11; kwfyurbuz > 0; kwfyurbuz--) {
			continue;
		}
	}
	if (true != true) {
		int ftgrymp;
		for (ftgrymp = 90; ftgrymp > 0; ftgrymp--) {
			continue;
		}
	}
	return 97624;
}

void zpvexhc::eixbithgfqnjui(bool fmnxthwgylf, bool vikpjmltfkog, string phjjv, double egfsybqsuibmqi, bool wvzraxp, int jdhvd, int tecfzzfiiuqn, int omtijeeusdleo) {
	bool pophyfkvqqqtrm = false;
	if (false == false) {
		int cgwposgwak;
		for (cgwposgwak = 55; cgwposgwak > 0; cgwposgwak--) {
			continue;
		}
	}

}

bool zpvexhc::lhfbwupzwjihrjp(string bncpfg) {
	bool vfssciivmm = true;
	int ltabsbcgetljygi = 2341;
	if (true == true) {
		int tuifvlvm;
		for (tuifvlvm = 48; tuifvlvm > 0; tuifvlvm--) {
			continue;
		}
	}
	return false;
}

double zpvexhc::hthdxjtmanvvbabxib(string jyruqn, bool kadgji, bool ppxkqfugzmwv, double tdohjarmozh) {
	string voprqtnssmyhm = "tekyppmxuxnc";
	double lwoytdnwngppgo = 4017;
	string svnrxoliqeeo = "npbydqsisvmazrxvlkeaujxzpnzwfadzkufhpgyakkhbxpknsefmfkhqshuyknodbuejqasosihtvkvrrcddmtjhl";
	string yhqoxo = "ueqraxtxpquyygqcnmnq";
	int phfowqmxesiioz = 5460;
	bool nqigtkboobxrhdn = false;
	bool kpehhjlqavwfw = false;
	return 74160;
}

bool zpvexhc::gpbpkngmwgfpllssrzwg(int tkijk, double wtxqlpyziyj, int zktynqfcf, string htymanvs, string rsyjsh, string olnpalriyuhg, int xwvzxnirglz) {
	bool xvuujjene = true;
	int kiizobrfrm = 2121;
	double jrexqrz = 43146;
	bool firaibylbyuhu = true;
	int mdvpxoos = 3648;
	bool ifnbycn = false;
	double qfmip = 7452;
	string eicoiublfhdnec = "zwctxmprobsp";
	bool ncevaqbbrhnxk = false;
	double ohiisbscq = 62809;
	return false;
}

string zpvexhc::lrzqmwzrlqvbqobjvjfx(int qhjjhk, string syyeaeoftjqlex, string xvwqrhurry, bool abypjhwhixzhgwb) {
	double eylbacf = 60116;
	if (60116 != 60116) {
		int dlykw;
		for (dlykw = 65; dlykw > 0; dlykw--) {
			continue;
		}
	}
	if (60116 == 60116) {
		int vdhstfa;
		for (vdhstfa = 29; vdhstfa > 0; vdhstfa--) {
			continue;
		}
	}
	return string("ymjvsgyrdv");
}

string zpvexhc::stluahzpar() {
	double aqyqbr = 1191;
	string ooahpmrxsf = "ypulvjxfpeuvezdnvrdocfnbeddprdrmhodzrzstnrfmhqhdepyqnhobsugkfdajapmsjghubwmrkiotrnyyiapfcz";
	if (1191 != 1191) {
		int nwohipnfq;
		for (nwohipnfq = 98; nwohipnfq > 0; nwohipnfq--) {
			continue;
		}
	}
	return string("vloghschqip");
}

string zpvexhc::jakymltubewvwvxuuezti(string yktok, int rnzolsfas, bool jaqymilxrxfko, string zorbnr, string cyonyyerybsprn, bool xubywyo, string cizctwfcxbwgxwp, string dvzfarnruzo) {
	return string("xacvxyjenuhpsxv");
}

string zpvexhc::buvczmhhwcbgbdhdpt(double pcopymfd) {
	string kkdzxzeem = "xfw";
	int comzbhmkfsebm = 2102;
	double vltzcgznrpjbs = 62107;
	int etvnnc = 1120;
	double ffywhebleefcrl = 39739;
	int eknhtcxxgitmpj = 963;
	bool oebyc = false;
	bool xphmmdbexaqybo = true;
	return string("godbtkrqraahnl");
}

string zpvexhc::tzyohxjeggleuwdfynclwsqxh(double nzoijtnzxj, double gvmip, bool ttipgdtlxj, double keyautluxs, bool xheonmg, string cadexpctusc, bool nxnnb, double kzmplfollymkuwl) {
	string lobxkhkqho = "ckvfwwwd";
	double vbzcm = 2432;
	bool ncbhld = false;
	double gqiaiztiwh = 31605;
	string nelcf = "buhkaqxxsnhywhkacmryoyusdfogephffbvgbhxiznxe";
	string hrxcpyaozunksa = "csbujntbhmepzhaflhjacslotoxxwhwrsdcnkzeiasczyaj";
	bool elyctnczatbndbg = true;
	string vizqry = "jhrqkctgrafeywsoqa";
	double sjjbzsgi = 5869;
	bool rvhjqn = true;
	if (2432 != 2432) {
		int eonvu;
		for (eonvu = 58; eonvu > 0; eonvu--) {
			continue;
		}
	}
	return string("bmh");
}

void zpvexhc::ahanxcfrmlav(int xryyl, double gctglknpyivaf, string zgbpu, double xydri, double uulfnvzarrei, bool zrseoshhiutkijb) {
	int bvamhlpuhq = 1021;
	double okpeqofrmhx = 64865;
	int cpjhnq = 1938;
	string rpltt = "pelnjpbrybokglxhcearwmwvdxmydlsuwkhepursgupafqpnxztupatsdkeoaqkkzqnl";
	double uitnbpgm = 38144;
	bool qqgoqsooieijhjw = true;
	if (1938 != 1938) {
		int eqqc;
		for (eqqc = 29; eqqc > 0; eqqc--) {
			continue;
		}
	}
	if (1021 == 1021) {
		int eohp;
		for (eohp = 73; eohp > 0; eohp--) {
			continue;
		}
	}

}

double zpvexhc::tcmcxzmtiwojeewbvkdcki(double gcpytdou, bool eqoylo, double kghbabpcfeg, bool ptqdmxh, string anwtzkbjbzpz) {
	string ofjleuzrbfqlmw = "fgjtsciarfjjfacxdqbfocmgjudcfmxykmupcodywcesqvxkxgnnfgaidyxq";
	int zifgtpxfbxp = 3257;
	string qvgntlyivq = "edyfbgezgzzweakuznhnvxtyviqfjxfuyquclwgefntpyqbvlqiwchsdtfzshnksojbpaejg";
	string zpipaftsylgrujy = "phpslxeadfvyiycigmrsfzzidxlxsxjhlsbtzjzumimjzjatxtiftxbesawabaqhjebxjx";
	if (3257 == 3257) {
		int yosoydfwtr;
		for (yosoydfwtr = 75; yosoydfwtr > 0; yosoydfwtr--) {
			continue;
		}
	}
	if (string("fgjtsciarfjjfacxdqbfocmgjudcfmxykmupcodywcesqvxkxgnnfgaidyxq") != string("fgjtsciarfjjfacxdqbfocmgjudcfmxykmupcodywcesqvxkxgnnfgaidyxq")) {
		int vavap;
		for (vavap = 6; vavap > 0; vavap--) {
			continue;
		}
	}
	if (string("fgjtsciarfjjfacxdqbfocmgjudcfmxykmupcodywcesqvxkxgnnfgaidyxq") != string("fgjtsciarfjjfacxdqbfocmgjudcfmxykmupcodywcesqvxkxgnnfgaidyxq")) {
		int vzgnztz;
		for (vzgnztz = 54; vzgnztz > 0; vzgnztz--) {
			continue;
		}
	}
	if (3257 != 3257) {
		int ujmrcsmwas;
		for (ujmrcsmwas = 59; ujmrcsmwas > 0; ujmrcsmwas--) {
			continue;
		}
	}
	if (string("phpslxeadfvyiycigmrsfzzidxlxsxjhlsbtzjzumimjzjatxtiftxbesawabaqhjebxjx") == string("phpslxeadfvyiycigmrsfzzidxlxsxjhlsbtzjzumimjzjatxtiftxbesawabaqhjebxjx")) {
		int lfsipfm;
		for (lfsipfm = 84; lfsipfm > 0; lfsipfm--) {
			continue;
		}
	}
	return 8855;
}

bool zpvexhc::qxuhizslesrlvtzvoewqghhq(int ranayqa, string ducvvtpl, double tzqzwgnommtrf, double raosssno, double meazaeumnbn, bool weqvligum) {
	bool oqxbjmcef = false;
	string gamjz = "vuyigmfhshgxzohfyndeckyshfgddnqjegx";
	int komzvsy = 1104;
	int vitqitnp = 4451;
	int cpvcacxwfigbjz = 3704;
	int lnxrxgapo = 3997;
	int glpum = 2557;
	double twlcynnyhrtkb = 46713;
	double bnnrtyxil = 52878;
	if (3997 != 3997) {
		int au;
		for (au = 98; au > 0; au--) {
			continue;
		}
	}
	return true;
}

void zpvexhc::howxhfbjjlogfc(string vpdesmdtg) {
	double cvpwxmn = 20261;
	string qiwolylvxgkh = "qdhpyfrhttzefmreksxmjzzzdqp";
	int mpioe = 2051;
	double huetuvpgndss = 2027;
	if (2051 != 2051) {
		int jxkquv;
		for (jxkquv = 57; jxkquv > 0; jxkquv--) {
			continue;
		}
	}

}

void zpvexhc::nckozlrppjwcfnv(string qjoktazjeklfhm, double rxnznuibzv, bool zhxovdq, double ewzifauozmz, int ndcfa, double jqswve, int qltradchu) {
	bool vkkhp = true;
	bool yncnbvie = false;
	bool qyazmt = true;
	string xkkdeili = "thqpcifzrmvpdjpdsaxbbvmfocohbkcunotctx";
	int qrdnipjheuqa = 3188;
	bool uikfywkifllsyw = false;
	int iokiykrv = 2099;
	string drnpzpidr = "myjxfdrtcchrawmtqnesvxhip";
	if (true == true) {
		int njvpfvb;
		for (njvpfvb = 97; njvpfvb > 0; njvpfvb--) {
			continue;
		}
	}
	if (string("thqpcifzrmvpdjpdsaxbbvmfocohbkcunotctx") != string("thqpcifzrmvpdjpdsaxbbvmfocohbkcunotctx")) {
		int ut;
		for (ut = 92; ut > 0; ut--) {
			continue;
		}
	}

}

bool zpvexhc::wqumhazieutytwbwzyu(double bkjifeky) {
	int llfzvgtczc = 2858;
	int xapgwtyj = 1040;
	int uszjpnf = 910;
	int emwcjqniayeqlk = 274;
	bool tgfcyqav = false;
	double esilfgc = 36844;
	bool qpkhofmrjabfmri = true;
	string hhxzmkwx = "bbebmfprgawxuxisetysnputbchhqmjkocqfbjvwpqjsqfbftsqrlqwgbzlixwccxzidjebfbzylyfwvbklpaphzecwsoeklt";
	string tqhgc = "wxf";
	double kkoexjomiq = 14623;
	if (2858 != 2858) {
		int xgrzij;
		for (xgrzij = 95; xgrzij > 0; xgrzij--) {
			continue;
		}
	}
	if (string("bbebmfprgawxuxisetysnputbchhqmjkocqfbjvwpqjsqfbftsqrlqwgbzlixwccxzidjebfbzylyfwvbklpaphzecwsoeklt") == string("bbebmfprgawxuxisetysnputbchhqmjkocqfbjvwpqjsqfbftsqrlqwgbzlixwccxzidjebfbzylyfwvbklpaphzecwsoeklt")) {
		int gteshu;
		for (gteshu = 94; gteshu > 0; gteshu--) {
			continue;
		}
	}
	if (910 == 910) {
		int jtpewglne;
		for (jtpewglne = 58; jtpewglne > 0; jtpewglne--) {
			continue;
		}
	}
	return false;
}

int zpvexhc::bqkcctdbabn(bool sbcajzjgf, string jcgll, string jphgfcempkwvtkt, bool gkmqthjsij, bool gitroz) {
	string qnsnlr = "nabsliyxhgsivsitfmmuntkzldkfxwrxwrpigqxcfrpbozkmyiuyrqqs";
	bool fkudxkgjaawiprl = false;
	bool kpwey = false;
	bool eqljgxvrnd = false;
	bool emsundikxloq = false;
	string hpwpsbmcit = "riqabyyrvyjwvggmmixlhyawojsclqvyjcmj";
	double vawlqaq = 35332;
	bool rzaglwkf = true;
	if (false != false) {
		int xizschh;
		for (xizschh = 67; xizschh > 0; xizschh--) {
			continue;
		}
	}
	return 1136;
}

int zpvexhc::pzgrktcfijkodzbfskqtbjb() {
	string owlvqeouxekif = "uzmdtebuzyrkoxtaooar";
	bool iwijtmhxa = false;
	int eaukybz = 923;
	double qbuwfma = 18496;
	int pzlrzzybykmw = 22;
	string zqbsu = "pyojwomecvicpxoaznfthvmzapbuwhubswfmzszpoze";
	int syrtc = 1019;
	if (22 == 22) {
		int fidjqw;
		for (fidjqw = 54; fidjqw > 0; fidjqw--) {
			continue;
		}
	}
	if (string("uzmdtebuzyrkoxtaooar") == string("uzmdtebuzyrkoxtaooar")) {
		int mh;
		for (mh = 65; mh > 0; mh--) {
			continue;
		}
	}
	return 31581;
}

bool zpvexhc::gcmwmqhvnd(string jobslqbe, int dmeuhddqzkqb, double ikvhnnd, int kcvgskqthahn, double fstmkuhn) {
	return true;
}

zpvexhc::zpvexhc() {
	this->tzyohxjeggleuwdfynclwsqxh(22942, 34848, true, 56432, true, string(""), true, 21268);
	this->ahanxcfrmlav(1373, 3407, string(""), 5471, 16215, false);
	this->tcmcxzmtiwojeewbvkdcki(2917, true, 41193, false, string("aiixunefzuwjqczaifnnwgqlpxppelmrhzroquvfqzqrmimqqtghaecmanmyubdrrtpgpivvquounaeexdmqqsczmbrasdekxk"));
	this->qxuhizslesrlvtzvoewqghhq(1370, string("opcyptyhcfafbzdxqguztwtoxzxfdfvtmonkvjdkfcpwgpmluqmlbokbvvleowtfuriyeipcmlrcvsaqrmuotdblvceoatk"), 57615, 54522, 17692, true);
	this->howxhfbjjlogfc(string("uhbqfawhlwfgnrjhkkufnpqhsvjkrqahecsausspxsnqjcuwhwhwahfkfkonoqqdshttrh"));
	this->nckozlrppjwcfnv(string("fnltsxvudqhedtemnbkvpekddsdmrkxwvxflkukxmkhbdicuwqjejeuglattdgvhqduaezqeaaxibgzjydubxgywve"), 32839, false, 8118, 7501, 41296, 2221);
	this->wqumhazieutytwbwzyu(263);
	this->bqkcctdbabn(true, string("ryhzaxuzyesdrvbncdyghsigxfdetlbwvlf"), string("giwpizapmjyugqrymtnnqxoxzxctfmgtrphwktrdqfvdttbazguiohsbwflbqrchdaomrwzqnppzscwunzghudneftsiqdwsai"), false, false);
	this->pzgrktcfijkodzbfskqtbjb();
	this->gcmwmqhvnd(string("tczvqalfhsesmczjsnyhimmwykwespdyvkmkemkhclucpskvyuaxdggrxffxkvegmkvvmqznabivogtbykvqjmiwkojraj"), 115, 23823, 1959, 21603);
	this->dczvhwqyonyzonufxlsbikqhd(string("xirhxjirndauchfjypbtwkylnrvdtnrewljxfxcgqacnppklujpfseohyxgsyeuotmdjzszcxuamuyhtkgxdlzzuy"), true, string("amovqbtucrrpbaqyuhwgbly"), 12388, false, 12917, 683);
	this->eixbithgfqnjui(true, true, string("nnmlvsyvaqyuzcchisondheechqnksgdcekqsnkxtawbvbtvnhltdpkudjooteggtzlyqhvphnxxfzdsrkexhzfelnnnzz"), 46487, true, 897, 101, 227);
	this->lhfbwupzwjihrjp(string("lgcukijpjcrlikgpfczkvhmklszkdfyoevurmvjbzcipfxfovvccdleecrkejv"));
	this->hthdxjtmanvvbabxib(string("peycwjuphznqpzzeahojknqqjnqofdtgyymaddldnoejloudl"), true, false, 18882);
	this->gpbpkngmwgfpllssrzwg(380, 65916, 2045, string("jgxyjofbtjuhwgkbdgqpkmpfzzwscrxsvvdexhvvuoemuxcaxwkhkmmadphcwgkiibhyeqdfcspxpr"), string("tzzr"), string("oybuqsclceifxrpqptuzoqldosrglmywbvdiasbpkrwpyqjaycgznavbngfkzgjpnjtxfhaiiqwwklsurmpkvmkddckknbv"), 357);
	this->lrzqmwzrlqvbqobjvjfx(5566, string("jvtcsasevytwxzmwllxxaksltxodcjnmjrwmw"), string("ivvngypqszdhrbvrfmfkuq"), false);
	this->stluahzpar();
	this->jakymltubewvwvxuuezti(string("btvbirtcokrngduqijngjthpxtffvhpaqpnfqiktiefnrha"), 546, false, string("horaxszoyyghbgglwbfqihzvxiopvimtuthlivmhcfxmbgakznzgqjlmxbnhjfzugisk"), string("yyfoviqdkldwevnnbhovvbydhgwuzoqmqnxohpnrcxubpealdt"), true, string("kxblmetoofqhfctaq"), string("bktqvqwmwphixovddlcbnjauqejfiiqktozajnsxfqjxrfwzqtscyznmlvrsqyshicspygwufvzxzkldifegkcvb"));
	this->buvczmhhwcbgbdhdpt(57670);
	this->kfbzryjybyzonzwljfl(string("ofyrziapibufdwsjaen"), false, string("qfqmfannnahalvdwgoiacdbcf"), false, 61899, true, 36297, true);
	this->ftkjmemwcrsrxdtpycy(3926, string("zshpxmnubyhheawn"), 37, 17879, string("eyhbwzahqteypwgqn"), 1325, string("cqbyjlpzmhdnmlapltayefuqmkoebdcvxfknttbjcwjhnxkpxmdqruqvtyzkoclwcqhidlomw"), 13157, 26721);
	this->qvhrmvmytfiye(string("csaxasmtudvjggewsihqpuwubvkusbqrupcvvgqowhaboxguaszl"), false, false, 13950, 22217, 1429, 526, string("jkhjgmlvujjvsszujibnrxuaadxsr"), true);
	this->rqnilfxjuhwguhjau(3926, 21186, 601);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class dokbccf {
public:
	int zpbgudxnqmpxc;
	bool rddujxilfkw;
	string ygbmdpqmbqaikf;
	dokbccf();
	double tbngmyddijssqwm(string qoyxtnojfdkqdzf, double cpevav, double gejtxv, string jdrftngbedcctg, double odpemk, int smtctoqygbufkbx);
	double tmtsoxygfalvwe();
	string wpsqqazcrswfk();
	int vabasctjspv(bool cfbzq);
	void gglgvlagjdqegputauun(int ufhtzndvjiytcb, double adgfgaepttt, string yndfvveqltgngm, double engyygkfedsuswy, int ufukfrzusiq, string vgeghd, double yuvbqsfnbq, string woihdjqksggu, bool gfmsyv);
	void qjormkdilzeq(double bntsaskhyjh, double sundxfiprluay, int mcrhluoqncbrz, string wkjybzvy, string uhpjjy);

protected:
	double lffbfostrw;
	string mjtdudyc;
	int gnowllrlzqlb;
	double wydwy;
	double wqksiesyonnfcjo;

	string gnyidgobcdhazuttcbhzs(bool obanabc, bool gctrgln, int iarewlq, double ihaznu, double bxbbjf, double jtjmrqrxzn, string kcsoopcch, double vxepmlimdbgdzap);
	bool mgikrhxgdpjggbphbrqa(int ouqbfgx, int uebilgdmtzndzsy, bool ripdvgptcqhor, string ivvtcepnf, bool glomj, string svpyzllqxarmo, string babsxmopjmkpq, bool wnlnxmngrxjzplt);
	int saeqblmkevyldxwpjwp(string uqzvkbuslti, string cfgqczexluwcgl, bool xnbdnwqwpgxjqrc, int nxupghbusewc, string ksureblgc, string iddgatulvgdoz);
	void xghytfsacch();

private:
	string hiukchpa;
	double swrsnbg;
	string ytfoffpnc;
	double pxxoedrsyexxqfb;

	void hofnulzrxqtenutadt(string ngnjwntnnbfdsj, double abnnkj, bool nieukaggkft, double yqdixbhnsdnvg, bool zzxxbljktnrg, int tinlaqas, bool dzsnkvov, double ahyijkqljzadkg, bool ijoyzkoonsecp, string whbwonenpbfig);
	string jlykpopycspch();
	double wfrzlalpalkhnsyeaghyvvfv(int leldipwvmmpe, bool embaod, string hicrkqgwuin);
	void nonkkwbikjfwjii(double bbgmcl, int fhluy, bool witbmjtclc, int oyllwiyspnrxwto, bool yhgwspmtdcwk, double whxrfgkikkelg, bool hxmhywj);
	void zcnoysxmkoadtrqjwkq(string nctrhnimymdhohe, bool kwonunqahsykwt, bool pnuemq, string ukzgmzhckxwraoc, string zsvqqrxdizumjh, int dchnooplckulnu, bool divkdbqwarcala, double twsbglk);
	void edpoductfrdxlgqznigy(int zagbswke, int xojcfi, int itoqeobbfv, int wvudsxgbwbwnmtc, double cgncspssd, string qjcbgwbfzy, string jagdpimuyslb, bool chgrmqct, double xzyjxmnt);
	double jmdbnznblolhvvorptoyeon(string ewmbllbfgjkrcyy, bool zejli, bool mssxpizgllicsa, int yjtpspbepfj, bool mqeqh, double eskrosmymrmfh, string vvnmof, double kldhxxxefz);
	bool hwqdngroadhtekakxyinbo(double nrdvrdcos, double moxaissy, bool dtmymxxa, double soxklc, bool kdkcwkahpxng, double cvocg, int jnwjlvm, bool ouskhrmkwopbox);
	double fzstlcliwlltew(bool yxaryortsq, bool bvxkwohtodqht, int mqwhl, double mhhfzx);

};


void dokbccf::hofnulzrxqtenutadt(string ngnjwntnnbfdsj, double abnnkj, bool nieukaggkft, double yqdixbhnsdnvg, bool zzxxbljktnrg, int tinlaqas, bool dzsnkvov, double ahyijkqljzadkg, bool ijoyzkoonsecp, string whbwonenpbfig) {
	double nchgjrpiren = 759;
	double ojkcetgokjjvnyc = 8976;
	bool xousdvwvokeb = false;
	bool ntbsxhytpwbivvh = true;
	double aqbbwpxhsq = 86991;
	int mqjdjfplp = 1597;
	if (false == false) {
		int we;
		for (we = 7; we > 0; we--) {
			continue;
		}
	}
	if (86991 == 86991) {
		int ouqzvh;
		for (ouqzvh = 79; ouqzvh > 0; ouqzvh--) {
			continue;
		}
	}

}

string dokbccf::jlykpopycspch() {
	double kdepksnl = 28409;
	bool sushhq = false;
	if (28409 == 28409) {
		int ny;
		for (ny = 49; ny > 0; ny--) {
			continue;
		}
	}
	return string("lyqhbbkbmmkgcqxoeefe");
}

double dokbccf::wfrzlalpalkhnsyeaghyvvfv(int leldipwvmmpe, bool embaod, string hicrkqgwuin) {
	bool hmeceokdhlp = false;
	double drghrabyem = 10484;
	bool avbnmq = true;
	if (true != true) {
		int fpjnqxpe;
		for (fpjnqxpe = 82; fpjnqxpe > 0; fpjnqxpe--) {
			continue;
		}
	}
	if (10484 == 10484) {
		int pdv;
		for (pdv = 25; pdv > 0; pdv--) {
			continue;
		}
	}
	if (true != true) {
		int tt;
		for (tt = 57; tt > 0; tt--) {
			continue;
		}
	}
	return 8302;
}

void dokbccf::nonkkwbikjfwjii(double bbgmcl, int fhluy, bool witbmjtclc, int oyllwiyspnrxwto, bool yhgwspmtdcwk, double whxrfgkikkelg, bool hxmhywj) {
	bool epbeh = true;
	bool cgdkxhvxochvfjq = true;
	double xvvsiercgqccta = 18;
	bool okyedpwyzrdnxu = false;
	string uhchqud = "qvwlksflhdbrowryucokbeenryfbgocmccvvgmwdhzuyhzrenlncwdxpekdubefklxsz";
	string zonmv = "amk";
	string irkhdknmihei = "gmoydeuxiixemamljqiqmxhcfvvzenbanihyxymxpavqskzoujqvsmdapuveojprxxogmhezrga";
	double andscoqh = 11190;
	if (string("amk") == string("amk")) {
		int qqboxrc;
		for (qqboxrc = 75; qqboxrc > 0; qqboxrc--) {
			continue;
		}
	}
	if (false != false) {
		int lotw;
		for (lotw = 86; lotw > 0; lotw--) {
			continue;
		}
	}
	if (string("amk") != string("amk")) {
		int asuxvtqnsb;
		for (asuxvtqnsb = 66; asuxvtqnsb > 0; asuxvtqnsb--) {
			continue;
		}
	}
	if (string("amk") == string("amk")) {
		int br;
		for (br = 52; br > 0; br--) {
			continue;
		}
	}

}

void dokbccf::zcnoysxmkoadtrqjwkq(string nctrhnimymdhohe, bool kwonunqahsykwt, bool pnuemq, string ukzgmzhckxwraoc, string zsvqqrxdizumjh, int dchnooplckulnu, bool divkdbqwarcala, double twsbglk) {
	string irerbxqvdkwzx = "eoidrkyleokyivch";
	int ssiumxw = 938;
	int ynnmluh = 677;
	int hrpfyobco = 325;
	double xqbfyek = 19944;
	string mxvytoau = "sxykamvaetkhlyfkddeudxtqjjbjtfmhvlkocqujwbxgcx";
	string ldpdwdkuc = "ugcywpvvyahcxyznuknwspsaqyz";
	int rzuztlssu = 546;
	if (677 == 677) {
		int wbqtawajb;
		for (wbqtawajb = 74; wbqtawajb > 0; wbqtawajb--) {
			continue;
		}
	}
	if (string("eoidrkyleokyivch") != string("eoidrkyleokyivch")) {
		int yoj;
		for (yoj = 51; yoj > 0; yoj--) {
			continue;
		}
	}
	if (677 == 677) {
		int xgknqzxhg;
		for (xgknqzxhg = 55; xgknqzxhg > 0; xgknqzxhg--) {
			continue;
		}
	}
	if (677 != 677) {
		int rhg;
		for (rhg = 86; rhg > 0; rhg--) {
			continue;
		}
	}
	if (19944 == 19944) {
		int iatwodd;
		for (iatwodd = 14; iatwodd > 0; iatwodd--) {
			continue;
		}
	}

}

void dokbccf::edpoductfrdxlgqznigy(int zagbswke, int xojcfi, int itoqeobbfv, int wvudsxgbwbwnmtc, double cgncspssd, string qjcbgwbfzy, string jagdpimuyslb, bool chgrmqct, double xzyjxmnt) {

}

double dokbccf::jmdbnznblolhvvorptoyeon(string ewmbllbfgjkrcyy, bool zejli, bool mssxpizgllicsa, int yjtpspbepfj, bool mqeqh, double eskrosmymrmfh, string vvnmof, double kldhxxxefz) {
	double mrpvlcqpvrc = 12623;
	string yfkjxozdifu = "vvlqharqepsayuzkhrmcxihmzslsltxcbtxefynpfgmswhkailjnwnjkc";
	int wiekfbra = 3379;
	int lxpuludzfyrfoj = 3318;
	if (3318 != 3318) {
		int zi;
		for (zi = 13; zi > 0; zi--) {
			continue;
		}
	}
	if (3318 == 3318) {
		int xmhoptvpyt;
		for (xmhoptvpyt = 34; xmhoptvpyt > 0; xmhoptvpyt--) {
			continue;
		}
	}
	if (3379 == 3379) {
		int shnhzokuhk;
		for (shnhzokuhk = 68; shnhzokuhk > 0; shnhzokuhk--) {
			continue;
		}
	}
	if (string("vvlqharqepsayuzkhrmcxihmzslsltxcbtxefynpfgmswhkailjnwnjkc") == string("vvlqharqepsayuzkhrmcxihmzslsltxcbtxefynpfgmswhkailjnwnjkc")) {
		int ltiqutd;
		for (ltiqutd = 77; ltiqutd > 0; ltiqutd--) {
			continue;
		}
	}
	return 40068;
}

bool dokbccf::hwqdngroadhtekakxyinbo(double nrdvrdcos, double moxaissy, bool dtmymxxa, double soxklc, bool kdkcwkahpxng, double cvocg, int jnwjlvm, bool ouskhrmkwopbox) {
	double bcxhfejeyiyx = 74904;
	double echuyutnjnqr = 78791;
	int vsoop = 1745;
	double rcihzbhefkfpc = 18763;
	string uzfqkjgelldxkk = "nvg";
	bool xlmlywhew = true;
	if (78791 == 78791) {
		int esrldw;
		for (esrldw = 61; esrldw > 0; esrldw--) {
			continue;
		}
	}
	if (18763 == 18763) {
		int ikpxtdcnx;
		for (ikpxtdcnx = 68; ikpxtdcnx > 0; ikpxtdcnx--) {
			continue;
		}
	}
	if (string("nvg") == string("nvg")) {
		int ny;
		for (ny = 73; ny > 0; ny--) {
			continue;
		}
	}
	if (18763 != 18763) {
		int tiwa;
		for (tiwa = 28; tiwa > 0; tiwa--) {
			continue;
		}
	}
	return false;
}

double dokbccf::fzstlcliwlltew(bool yxaryortsq, bool bvxkwohtodqht, int mqwhl, double mhhfzx) {
	double ygkjj = 15390;
	bool xbjcrgpjm = true;
	double ypzsjzbbm = 50836;
	double mlhvqwkvup = 6401;
	return 57021;
}

string dokbccf::gnyidgobcdhazuttcbhzs(bool obanabc, bool gctrgln, int iarewlq, double ihaznu, double bxbbjf, double jtjmrqrxzn, string kcsoopcch, double vxepmlimdbgdzap) {
	bool duekqygupvgy = false;
	string tzfukhtrbz = "jexyatdaibmrujebubhpfoegiy";
	string gawwwlvqxhpcpwd = "zmpzsmgeepyfizsowqaxfnhqllkjgvhfolkywpvgxyh";
	string sgdof = "dnkszxhmwbcjbonaapkjdluqtzbvvzrmsbwrkqzcblfrulqiqeh";
	bool uyxfjtxsmflm = true;
	string wwrkrgqdc = "qopxfasvamwrpfrwgjoufqmjdwpsxllrelkecvxomorefpydbxugymcxvublblagazrdscabhah";
	bool sleqt = true;
	if (false == false) {
		int jqe;
		for (jqe = 10; jqe > 0; jqe--) {
			continue;
		}
	}
	if (false == false) {
		int efqwtv;
		for (efqwtv = 73; efqwtv > 0; efqwtv--) {
			continue;
		}
	}
	if (true == true) {
		int okbzoccprr;
		for (okbzoccprr = 21; okbzoccprr > 0; okbzoccprr--) {
			continue;
		}
	}
	return string("mitpwnttqqmzvcfs");
}

bool dokbccf::mgikrhxgdpjggbphbrqa(int ouqbfgx, int uebilgdmtzndzsy, bool ripdvgptcqhor, string ivvtcepnf, bool glomj, string svpyzllqxarmo, string babsxmopjmkpq, bool wnlnxmngrxjzplt) {
	bool orowps = true;
	bool aaxduhzhnzax = false;
	int ewzfgqums = 293;
	string bokqgvmiuzurll = "faiyvuqpkeyvmf";
	string nmfdalsauoul = "tzouwdwuxdihkrfghtnjjefcxujokoszfrntyd";
	bool qjgloflipr = false;
	bool gntkakpzhzty = false;
	int hkhtiyqn = 200;
	int ocatjzt = 711;
	bool siqfi = true;
	if (false != false) {
		int iou;
		for (iou = 1; iou > 0; iou--) {
			continue;
		}
	}
	if (true != true) {
		int zhddz;
		for (zhddz = 14; zhddz > 0; zhddz--) {
			continue;
		}
	}
	if (false != false) {
		int mdg;
		for (mdg = 81; mdg > 0; mdg--) {
			continue;
		}
	}
	if (false == false) {
		int sygcxgynou;
		for (sygcxgynou = 54; sygcxgynou > 0; sygcxgynou--) {
			continue;
		}
	}
	if (string("faiyvuqpkeyvmf") != string("faiyvuqpkeyvmf")) {
		int iabfvuxo;
		for (iabfvuxo = 47; iabfvuxo > 0; iabfvuxo--) {
			continue;
		}
	}
	return false;
}

int dokbccf::saeqblmkevyldxwpjwp(string uqzvkbuslti, string cfgqczexluwcgl, bool xnbdnwqwpgxjqrc, int nxupghbusewc, string ksureblgc, string iddgatulvgdoz) {
	double ekfipwqmwxmyohx = 48048;
	if (48048 != 48048) {
		int hcykhszx;
		for (hcykhszx = 93; hcykhszx > 0; hcykhszx--) {
			continue;
		}
	}
	if (48048 == 48048) {
		int shy;
		for (shy = 58; shy > 0; shy--) {
			continue;
		}
	}
	if (48048 == 48048) {
		int sdvttvgn;
		for (sdvttvgn = 22; sdvttvgn > 0; sdvttvgn--) {
			continue;
		}
	}
	return 72094;
}

void dokbccf::xghytfsacch() {
	bool thywmn = false;
	double qvzacitnvysbl = 8270;
	int nockttqi = 2033;
	int fbkscemgrprz = 2432;
	string ffannyf = "rrhsghecdovwglzooymvrcbhlqpyzmrxmdswxjzsyszxibfqjvi";
	int prdgouwkustpygt = 2974;
	double mouhyzvzf = 43938;
	string joztzuswtgus = "lnhcowleveuznkuuuhqszgockpaxba";
	bool hfpvcyjhojh = true;
	double wltmsoodizkcwh = 18626;
	if (false != false) {
		int afv;
		for (afv = 58; afv > 0; afv--) {
			continue;
		}
	}

}

double dokbccf::tbngmyddijssqwm(string qoyxtnojfdkqdzf, double cpevav, double gejtxv, string jdrftngbedcctg, double odpemk, int smtctoqygbufkbx) {
	int bjlbwxywtcuqbv = 7792;
	int lnbyl = 3108;
	if (7792 == 7792) {
		int xgqbuf;
		for (xgqbuf = 45; xgqbuf > 0; xgqbuf--) {
			continue;
		}
	}
	if (3108 != 3108) {
		int fckivj;
		for (fckivj = 31; fckivj > 0; fckivj--) {
			continue;
		}
	}
	return 19662;
}

double dokbccf::tmtsoxygfalvwe() {
	double thjwxhaew = 23903;
	bool cjzkfs = true;
	bool gygmeyolt = true;
	double lzzgmiruguqj = 40493;
	if (40493 != 40493) {
		int elhv;
		for (elhv = 53; elhv > 0; elhv--) {
			continue;
		}
	}
	if (40493 != 40493) {
		int pidvvl;
		for (pidvvl = 55; pidvvl > 0; pidvvl--) {
			continue;
		}
	}
	return 88764;
}

string dokbccf::wpsqqazcrswfk() {
	double znnnjlvhtdsm = 14981;
	int lndfcipqzuuoibj = 6272;
	string ihapvkjqwclwttn = "rvnvqnwrrkkbiykzmmbuporzbbggtgzmxvccbbasbhgrhnymjrrxf";
	if (string("rvnvqnwrrkkbiykzmmbuporzbbggtgzmxvccbbasbhgrhnymjrrxf") != string("rvnvqnwrrkkbiykzmmbuporzbbggtgzmxvccbbasbhgrhnymjrrxf")) {
		int acpwytkany;
		for (acpwytkany = 52; acpwytkany > 0; acpwytkany--) {
			continue;
		}
	}
	if (6272 == 6272) {
		int ta;
		for (ta = 100; ta > 0; ta--) {
			continue;
		}
	}
	if (14981 == 14981) {
		int vbj;
		for (vbj = 34; vbj > 0; vbj--) {
			continue;
		}
	}
	if (string("rvnvqnwrrkkbiykzmmbuporzbbggtgzmxvccbbasbhgrhnymjrrxf") != string("rvnvqnwrrkkbiykzmmbuporzbbggtgzmxvccbbasbhgrhnymjrrxf")) {
		int dfrn;
		for (dfrn = 36; dfrn > 0; dfrn--) {
			continue;
		}
	}
	return string("zmvlydlgrn");
}

int dokbccf::vabasctjspv(bool cfbzq) {
	double tdpenbk = 18555;
	int elkvagtwug = 951;
	bool onzshlrcijiucem = false;
	if (18555 != 18555) {
		int dcrrjhdj;
		for (dcrrjhdj = 9; dcrrjhdj > 0; dcrrjhdj--) {
			continue;
		}
	}
	if (18555 == 18555) {
		int wiaal;
		for (wiaal = 41; wiaal > 0; wiaal--) {
			continue;
		}
	}
	if (18555 != 18555) {
		int vw;
		for (vw = 67; vw > 0; vw--) {
			continue;
		}
	}
	if (false == false) {
		int msjkpxxg;
		for (msjkpxxg = 99; msjkpxxg > 0; msjkpxxg--) {
			continue;
		}
	}
	return 4346;
}

void dokbccf::gglgvlagjdqegputauun(int ufhtzndvjiytcb, double adgfgaepttt, string yndfvveqltgngm, double engyygkfedsuswy, int ufukfrzusiq, string vgeghd, double yuvbqsfnbq, string woihdjqksggu, bool gfmsyv) {

}

void dokbccf::qjormkdilzeq(double bntsaskhyjh, double sundxfiprluay, int mcrhluoqncbrz, string wkjybzvy, string uhpjjy) {
	bool htpyoauqkopoc = false;
	bool zdodagsm = true;
	string qdutbzbrvoy = "afjanphodeqtptmqzyghrhtzyrltddfpuhur";
	bool hbkwqjwdorj = true;
	string rmhzcpgqxmi = "kqlxehqluvhkjbhnujrqfgufgcfdfndsgkqxzxfwxjnfzmshaftjhjzrgvudpleaeycc";
	string pfqjlk = "bcsldyyvpcmrkcxbhcezouldcgvasfgvjanwzdyknzfkqguxiubcpnwgltivcybmlneeoybxiedqxmez";
	double sqtevayowyknwqh = 29800;
	bool qoklocj = false;
	if (false != false) {
		int lrv;
		for (lrv = 18; lrv > 0; lrv--) {
			continue;
		}
	}

}

dokbccf::dokbccf() {
	this->tbngmyddijssqwm(string("steaxxfnurmocasbrehrfsoimcajiwpketnwgtrqv"), 14104, 41028, string("roislsziopjzlddfgikjzlzriltyyeppkhlxivmqempecl"), 16697, 15);
	this->tmtsoxygfalvwe();
	this->wpsqqazcrswfk();
	this->vabasctjspv(true);
	this->gglgvlagjdqegputauun(1145, 30562, string("kmfrbncxlngxucwwpsoukyyesdxptzadftzkx"), 16864, 4927, string("thhttoxrkaaraxgmmexonaygptyunoldtckcsgygaskskktywktuskyyfjdp"), 3755, string("jgagcrlcbgmpdgkyfwvtcofvcvakaolttscthaxgwgfbgzenwqshivubsossnxqwamqkkbysqoyhchkddvvx"), true);
	this->qjormkdilzeq(60318, 42663, 5333, string("dxf"), string("sqlmjdqrzhximsjkdzkulcrlsrabwvtlchibvhigocrosejefkkpppyfbmnwnyeyanwolpzzpcmewf"));
	this->gnyidgobcdhazuttcbhzs(false, true, 4793, 54109, 32105, 5777, string("zudprqzkegjaay"), 57673);
	this->mgikrhxgdpjggbphbrqa(588, 4964, false, string("fsbtitqlloaewejwjweondenakpnohqjijhztsgwat"), true, string("ndyazgufxrcpniojjblokllxbmbkmogfdcbbkbkkfgxctanv"), string("mjybfvvilokixvz"), false);
	this->saeqblmkevyldxwpjwp(string("hcdnwdvgnxxqnohbpoolghxxuruxylohxcqhggbilpdvdyqwacrsbjgqxbhegrxemvykehumphsjdqmfkzxpwm"), string("sckclftlhyjxaiiuu"), false, 1088, string("cbqswtqswnnqbdjzrnhuefhahonvczcahoj"), string("v"));
	this->xghytfsacch();
	this->hofnulzrxqtenutadt(string("rrncodgkotrqubcsvdjqgqbadxjudnidbcztigddrltcyftxjntseagwdoct"), 22221, false, 78665, true, 390, true, 26086, true, string("cadyqzaawwctwmcsha"));
	this->jlykpopycspch();
	this->wfrzlalpalkhnsyeaghyvvfv(710, false, string("lgnmqrmxzwbpesgbbzriupvcuhfsxyaunzyopkmipfshqwgfszytla"));
	this->nonkkwbikjfwjii(1736, 3550, false, 394, true, 6785, false);
	this->zcnoysxmkoadtrqjwkq(string("lllmvuwkqhxpnmwjdvvmfkaddvmvxmpojglvjjiqctafdonnkcvvinqgkwebqciutgsgi"), true, false, string("ruoptsvlxqtmxagjzxwshinr"), string("lor"), 876, false, 45930);
	this->edpoductfrdxlgqznigy(5691, 326, 493, 7350, 83276, string("viyethrjnmpmezvebszpiswncgnqymulrnbsjofbmbbleflnfqqwdawzihsgmupwtewtzkoxvuzc"), string("khclmzjxkewjfrqmu"), true, 9874);
	this->jmdbnznblolhvvorptoyeon(string("fnblzauioszwoinapcaewnyxtzbdiotddvzgjbnmd"), true, false, 3479, true, 55304, string("rungdaiknwibimheytuueharafkfvabbbczxadpuvzhetftbulkqrxzchpcwzfkzzscvvnuwcmariijshpnv"), 15932);
	this->hwqdngroadhtekakxyinbo(7708, 42362, true, 75097, true, 22300, 4724, true);
	this->fzstlcliwlltew(true, true, 279, 2190);
}
