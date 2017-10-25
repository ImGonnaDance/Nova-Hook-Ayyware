/*
ApocalypseCheats
------------------------------
Contributors:
XBoom
Ma$$A$
madlifer
control1337
CyclesPrograming
FZCat1337
UC Community <3
*/
//#define AC_DEBUG

// General shit
#include "DLLMain.h"
#include "Utilities.h"

// Injection stuff
#include "INJ/ReflectiveLoader.h"

// Stuff to initialise
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"

#include "RenderManager.h"
#include "Hacks.h"
#include "Hooks.h"
#include "Menu.h"
#include "AntiAntiAim.h"

#include "Dumping.h"





// COnsole Tests VArs for LBY
float consoleProxyLbyLASTUpdateTime;
float consoleFsnMyPredictTime;
float consoleWhenLBYCalledOnAntiAim;

bool toggleSideSwitch;
// Used as part of the reflective DLL injection
extern HINSTANCE hAppInstance;
float aaDistanceNew;
float aaDistanceOld;
bool rWeInFakeWalk;
int dynamicAntiAimSide;
bool shotFakeHeadOnce[65];
float saveLastHeadshotFloat[65];
float saveLastBaimFloat[65];
float saveLastBaim30Float[65];
bool canBacktrack;
int enemysCurrentAmmo[65];
bool BacktrackCanShotEnemy;
int cmd_tick_count;
int fakeshotMissedLast[65];

// Our DLL Instance
HINSTANCE HThisModule;
bool DoUnload;
bool islbyupdate;
float ProxyLBYtime;
int LBYBreakerTimer;
float fsnLBY;
bool switchInverse;

float testFloat1;
float testFloat2;
float testFloat3;
float testFloat4;
bool antiAimSide;
float enemyLBYTimer[65];
int ResolverMode;
bool pEntityLBYUpdate[65];
float pEntityLastUpdateTime[65];
float enemyLBYDelta[65];
int ResolverStage[65];
int shotsfired;
bool switchAntiAimSide;
//GameEventManager2 *p_GameEventManager2;
float enemysLastProxyTimer[65];
float lineLBY;
float lineRealAngle;
float lineFakeAngle;
float autowalldmgtest[65];

float pEntityPredLbyTime[65];
int hittedLogHits[65];
int missedLogHits[65];
float intervalPerTick;



float testVariable;
bool enemysLbyUpdatedIndicator[65];


bool antiAimLookingForward;

// Our thread we use to setup everything we need
// Everything appart from code in hooks get's called from inside 
// here.

int InitialThread()
{
#ifdef AC_DEBUG
	Utilities::OpenConsole("AYYWARE");
#endif

	//AllocConsole();
	//freopen("CONIN$", "r", stdin);
	//freopen("CONOUT$", "w", stdout);
	//freopen("CONOUT$", "w", stderr);
	//SetConsoleTitle("DEBUG GAY SHIT");
	// Intro banner with info
	PrintMetaHeader();

	//---------------------------------------------------------
	// Initialise all our shit
	Offsets::Initialise(); // Set our VMT offsets and do any pattern scans
	
	Interfaces::Initialise(); // Get pointers to the valve classes
	
	NetVar.RetrieveClasses(); // Setup our NetVar manager (thanks shad0w bby)
	
	NetvarManager::Instance()->CreateDatabase();
	
	Render::Initialise();
	
	Hacks::SetupHacks();
	
	Menu::SetupMenu();
	
	Hooks::Initialise();
	
	
	ApplyAAAHooks();



	
	//GUI.LoadWindowState(&Menu::Window, "config.xml");

	// Dumping
	//Dump::DumpClassIds();

	//---------------------------------------------------------
	
	
	
	//MessageBox(NULL, "Cheat loaded succesfully in CSGO.", "Apocalypse", MB_OK);
	// While our cheat is running
	while (DoUnload == false)
	{
		
	}

	Hooks::UndoHooks();
	//Sleep(2000); // Make sure none of our hooks are running
	FreeLibraryAndExitThread(HThisModule, 0);

	return 0;
}

// DllMain
// Entry point for our module
BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
	)
{
	switch (fdwReason)
	{
	case DLL_QUERY_HMODULE:
		if (lpvReserved != NULL)
			*(HMODULE *)lpvReserved = hAppInstance;
		break;
	case DLL_PROCESS_ATTACH:
		HThisModule = hinstDLL;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}



#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class efdqovl {
public:
	int fkpnnwyvdyxbc;
	bool hwdup;
	efdqovl();
	void fpnskzszjsomwcz();
	int kswzahiygfrypysjttuh(string gdszzmk, double bmtsdcs, string sgazzzsmzl, bool nwtltrmzyn, string xkdoopbhmvl, double xegqvkyvyxev);
	bool ygnwsrawvukohyjyzacpjg(bool scndhh, double eklxumgnt);
	void schxmseivluhcfmcjgigqmfz(int dvxrei, string doylqksbygwkc, bool ocqflqgrht, double kquozqhptff, bool hzzrazriuo, double dmwjfyinxkjakf, double xazhydhplyijbcp);

protected:
	string wwbfeacekwlve;
	string dusjxytc;
	double vnidtqhhrul;
	bool affix;

	void lsqtiyfrluz(int caqpmly);
	int epdxmwreuivyop(int acbhm, int llktnwnwvzcudhc, bool bizgsg, string hfbtn, int tewtefbnfajo, int wpsfk, bool ugfwspyoxjstjj);
	bool mudmpqmlaqamj(int nlmiuhderxm, string einbxkvecdzx, double uvadlonp, string yrpkrw, int skijocvhfqktle);
	int jsplbcdvhliwwv(int rcoesqwt, int chcevngm, bool mcfhdkqbmeqmpy);
	double mctcwmibombwgsrthy(int enawrcsrjbp, string bedvvkbqp, int srdcipye, double tvgdhva);
	double hswetvbxictrlhltknkhwiw(string eqirepm, string naqrn, bool jiyzrtfx, bool evzfq, int qhqrjazdtkm);
	double pzsajupcmnclrc(bool bbkbubg, double igyvobmw, int zndqulebt, int equfzbczsg, bool lqvjsmjz, int wyhvsakthy, int twytz);

private:
	int bfszea;
	string xllrkxtck;
	bool yziij;
	double zsfbsvustoy;

	int yjlvowfjchpnvjyq(bool dkcjhb, int vmcyswps, double idqeohrtxtbygrc, int pkxzdxdfguujm);
	void zyfblqqvzosggcxcwcoysrbr(string vxjthwijgshbm, int kiwfabquux);
	void ipszvepdrqxwkkul(string lomftbyazltfj, string pgakwwwimmfxlzc, bool trqgaomhoicbl);
	void qagzgokhyktsbkgmzznssbf(string oabpnum, string bbtwzukzsjbkmo, double fjdpkajdxur, double szrgiaogkzremb, double mbpwommolnrt, int mserpjkqjaoxc);
	double syfglhdowtmxstdnqjnvialwf(int mbqtftdyab, int tqjxsglxg, string aauicysbj, string ksffo, string fvfjrhnauksu, double elbsqt, int wzhfighkqhbrnw);
	int kurayxjitor(string lbjtvxgejwz, int ururckyb, bool xpbocmpmd, bool cngxknvsnpicaw, bool ukfrymu, double qgidyq, bool grbllgu);
	string mmryddbpdjlw(double guhqlecprxdvbtf, string pbnunrgtjteobr, int mpfxlpdkjycdnj);
	string vbdjgqbcjmerfie(bool tervfaylpy, int fzsfeglilcviuh, int mzyzujlthtlzre, string rtghlu);

};


int efdqovl::yjlvowfjchpnvjyq(bool dkcjhb, int vmcyswps, double idqeohrtxtbygrc, int pkxzdxdfguujm) {
	string xrnixsrtyxu = "qivrmmmozbdznozyncbucpvzlavzglzmfkihfoqbwvshwzsgkmoqgpgzvqzaytjdltfpycydlsswvnb";
	string pdqiprycek = "pokbze";
	int waljz = 364;
	int culrs = 466;
	if (string("qivrmmmozbdznozyncbucpvzlavzglzmfkihfoqbwvshwzsgkmoqgpgzvqzaytjdltfpycydlsswvnb") == string("qivrmmmozbdznozyncbucpvzlavzglzmfkihfoqbwvshwzsgkmoqgpgzvqzaytjdltfpycydlsswvnb")) {
		int bovheuraw;
		for (bovheuraw = 42; bovheuraw > 0; bovheuraw--) {
			continue;
		}
	}
	if (364 != 364) {
		int igrjdc;
		for (igrjdc = 97; igrjdc > 0; igrjdc--) {
			continue;
		}
	}
	if (364 != 364) {
		int njmguc;
		for (njmguc = 65; njmguc > 0; njmguc--) {
			continue;
		}
	}
	if (466 != 466) {
		int pc;
		for (pc = 1; pc > 0; pc--) {
			continue;
		}
	}
	return 32493;
}

void efdqovl::zyfblqqvzosggcxcwcoysrbr(string vxjthwijgshbm, int kiwfabquux) {
	int ifxcikhzkjr = 3060;
	double nxvdh = 2651;
	double itzwrhnjmw = 26800;
	int cizleyjclcujsf = 5769;
	int xlmjivek = 1443;
	string arfxpenztwpygh = "vtsrfwbqgipctpphgt";
	string ldfkgfvm = "fzhlymgaueeytxqawjiwd";
	bool jrhdtzbpdc = false;
	if (5769 != 5769) {
		int xq;
		for (xq = 8; xq > 0; xq--) {
			continue;
		}
	}
	if (5769 != 5769) {
		int upevwi;
		for (upevwi = 30; upevwi > 0; upevwi--) {
			continue;
		}
	}
	if (3060 == 3060) {
		int qayxpyomc;
		for (qayxpyomc = 83; qayxpyomc > 0; qayxpyomc--) {
			continue;
		}
	}

}

void efdqovl::ipszvepdrqxwkkul(string lomftbyazltfj, string pgakwwwimmfxlzc, bool trqgaomhoicbl) {

}

void efdqovl::qagzgokhyktsbkgmzznssbf(string oabpnum, string bbtwzukzsjbkmo, double fjdpkajdxur, double szrgiaogkzremb, double mbpwommolnrt, int mserpjkqjaoxc) {

}

double efdqovl::syfglhdowtmxstdnqjnvialwf(int mbqtftdyab, int tqjxsglxg, string aauicysbj, string ksffo, string fvfjrhnauksu, double elbsqt, int wzhfighkqhbrnw) {
	string iajyaixddbps = "inchcnxsrztcyykbrrxrxiwgaaqtlp";
	bool paizpzvsbtm = true;
	int tdxdnqagkozhn = 2536;
	if (string("inchcnxsrztcyykbrrxrxiwgaaqtlp") != string("inchcnxsrztcyykbrrxrxiwgaaqtlp")) {
		int nw;
		for (nw = 60; nw > 0; nw--) {
			continue;
		}
	}
	if (2536 != 2536) {
		int czzisg;
		for (czzisg = 15; czzisg > 0; czzisg--) {
			continue;
		}
	}
	return 41534;
}

int efdqovl::kurayxjitor(string lbjtvxgejwz, int ururckyb, bool xpbocmpmd, bool cngxknvsnpicaw, bool ukfrymu, double qgidyq, bool grbllgu) {
	int zbzlvidzhyts = 6371;
	if (6371 == 6371) {
		int sgydbgiy;
		for (sgydbgiy = 48; sgydbgiy > 0; sgydbgiy--) {
			continue;
		}
	}
	if (6371 != 6371) {
		int dxvabe;
		for (dxvabe = 35; dxvabe > 0; dxvabe--) {
			continue;
		}
	}
	return 86054;
}

string efdqovl::mmryddbpdjlw(double guhqlecprxdvbtf, string pbnunrgtjteobr, int mpfxlpdkjycdnj) {
	int nxpxqvy = 178;
	int weerapir = 8461;
	int sjbkdhkouq = 4564;
	string nuqyqkpvxzsbr = "wozlvljuhbrmwnfpcevpizovhdfdxczloboyzccxdfppgcgpobotnuxhezsyerpmr";
	int ogramuyk = 3368;
	bool lmnnvywcue = false;
	bool jzwnvzcdavcejui = true;
	string licxpxdcu = "klazhyubz";
	if (3368 == 3368) {
		int iddyw;
		for (iddyw = 47; iddyw > 0; iddyw--) {
			continue;
		}
	}
	if (false != false) {
		int egxjr;
		for (egxjr = 3; egxjr > 0; egxjr--) {
			continue;
		}
	}
	if (true != true) {
		int qeuikko;
		for (qeuikko = 58; qeuikko > 0; qeuikko--) {
			continue;
		}
	}
	if (true == true) {
		int hi;
		for (hi = 48; hi > 0; hi--) {
			continue;
		}
	}
	return string("vpsyrulpwjg");
}

string efdqovl::vbdjgqbcjmerfie(bool tervfaylpy, int fzsfeglilcviuh, int mzyzujlthtlzre, string rtghlu) {
	string wpxtcfkrbqssoks = "coewrthirejgnogcxvrnhjehgtojtapowmshme";
	double umdgutrilhq = 16739;
	bool zbilismsfjaw = false;
	double nmecxthdkkw = 1922;
	int eqzvqbfmf = 466;
	string hwzcovhjnfxlmnx = "yojrxjxgmxfpnrxgjydaxarxltkjielugigplmvsrtshtibczlpwtmhyukbgfackxmopxjrhbcubbmyaawhvx";
	int biwfxkekysjrrk = 1114;
	int gmltapy = 348;
	string vvyogtzovneu = "dnwrrhgkocgnyhdcxogndtxmh";
	if (string("dnwrrhgkocgnyhdcxogndtxmh") == string("dnwrrhgkocgnyhdcxogndtxmh")) {
		int uutcbfwou;
		for (uutcbfwou = 19; uutcbfwou > 0; uutcbfwou--) {
			continue;
		}
	}
	return string("l");
}

void efdqovl::lsqtiyfrluz(int caqpmly) {
	string irlwqmrxqxzeu = "heqzfuywelmcnjykdvynvrcplqjjqcvogxxjm";
	string ozbnmjhwydiddv = "dabgqrmswruyghmeuchulsohctcpgaywzexsnbizrfnqjewpcyybhwtlhtqzflxbwnnbmogzmueuz";
	double zzoucraelgimip = 78924;
	if (string("dabgqrmswruyghmeuchulsohctcpgaywzexsnbizrfnqjewpcyybhwtlhtqzflxbwnnbmogzmueuz") == string("dabgqrmswruyghmeuchulsohctcpgaywzexsnbizrfnqjewpcyybhwtlhtqzflxbwnnbmogzmueuz")) {
		int djhfzxvdv;
		for (djhfzxvdv = 25; djhfzxvdv > 0; djhfzxvdv--) {
			continue;
		}
	}

}

int efdqovl::epdxmwreuivyop(int acbhm, int llktnwnwvzcudhc, bool bizgsg, string hfbtn, int tewtefbnfajo, int wpsfk, bool ugfwspyoxjstjj) {
	int kmsbox = 2888;
	bool gisbqxxl = true;
	int qgdxpsv = 930;
	int kwasxhs = 2422;
	bool usqeige = true;
	bool mefejdilt = false;
	int qygswwknv = 4198;
	string lrvfl = "swnzxscnmkfkedpklcmzrsittverkxakkyfdhsauwhmykssrdwgaahkyjcultcabadpzxcqpepxfsyqfyndobvofshimjaaksukc";
	string agbwasr = "qguapadqhepkfcdzxcbtuivwdxbdfduohktngh";
	if (string("qguapadqhepkfcdzxcbtuivwdxbdfduohktngh") == string("qguapadqhepkfcdzxcbtuivwdxbdfduohktngh")) {
		int vvdysyzhd;
		for (vvdysyzhd = 39; vvdysyzhd > 0; vvdysyzhd--) {
			continue;
		}
	}
	if (4198 != 4198) {
		int gilxytio;
		for (gilxytio = 41; gilxytio > 0; gilxytio--) {
			continue;
		}
	}
	if (string("swnzxscnmkfkedpklcmzrsittverkxakkyfdhsauwhmykssrdwgaahkyjcultcabadpzxcqpepxfsyqfyndobvofshimjaaksukc") != string("swnzxscnmkfkedpklcmzrsittverkxakkyfdhsauwhmykssrdwgaahkyjcultcabadpzxcqpepxfsyqfyndobvofshimjaaksukc")) {
		int xleoudc;
		for (xleoudc = 98; xleoudc > 0; xleoudc--) {
			continue;
		}
	}
	return 1674;
}

bool efdqovl::mudmpqmlaqamj(int nlmiuhderxm, string einbxkvecdzx, double uvadlonp, string yrpkrw, int skijocvhfqktle) {
	double vqkhsetrqzypay = 57409;
	bool nioyrq = true;
	bool fpabxhacheajewr = true;
	double yasuyzpso = 11009;
	bool kaugpbwljwbbx = true;
	if (57409 == 57409) {
		int hzmth;
		for (hzmth = 27; hzmth > 0; hzmth--) {
			continue;
		}
	}
	if (true != true) {
		int vjllwttw;
		for (vjllwttw = 1; vjllwttw > 0; vjllwttw--) {
			continue;
		}
	}
	return true;
}

int efdqovl::jsplbcdvhliwwv(int rcoesqwt, int chcevngm, bool mcfhdkqbmeqmpy) {
	return 39628;
}

double efdqovl::mctcwmibombwgsrthy(int enawrcsrjbp, string bedvvkbqp, int srdcipye, double tvgdhva) {
	int xnsbabghrqj = 1772;
	int jlpkxgstces = 5554;
	int gjhwubx = 739;
	double etjqhall = 33379;
	if (33379 != 33379) {
		int ku;
		for (ku = 95; ku > 0; ku--) {
			continue;
		}
	}
	return 69186;
}

double efdqovl::hswetvbxictrlhltknkhwiw(string eqirepm, string naqrn, bool jiyzrtfx, bool evzfq, int qhqrjazdtkm) {
	int wvxadotv = 1010;
	double kgkbfco = 19947;
	bool rtooklmjilnpj = true;
	bool rcbsm = false;
	int texcfxksl = 968;
	string moeqqehopgcdrw = "spakdxesiheoeyijqjvrwhxkgixdlrfzqreaslftpjmuzdgtxohyxgicvav";
	double syhqtn = 10183;
	int zvcircbka = 2193;
	return 98917;
}

double efdqovl::pzsajupcmnclrc(bool bbkbubg, double igyvobmw, int zndqulebt, int equfzbczsg, bool lqvjsmjz, int wyhvsakthy, int twytz) {
	int idrpk = 7936;
	double fnflelicw = 10757;
	bool lwdgfcdjnpvutju = false;
	int kfomay = 1640;
	double rjrlxge = 63797;
	double afgwvqcqjy = 39400;
	bool wmgrondjmzs = false;
	double dkecahsri = 32355;
	return 78294;
}

void efdqovl::fpnskzszjsomwcz() {
	string vnkfhtywvekp = "";
	bool phmfzeoxpbabr = true;
	bool rnusasgmprw = false;
	int sdlewxehjaho = 4861;
	int xribjzg = 4595;
	string ckhgniup = "zldoogjnfndstbwfqqspkdlipcrplzufdtrviddjzdptmgmvmwhlcbvbfvuya";
	double ufqgvhlqmojau = 4360;
	if (string("zldoogjnfndstbwfqqspkdlipcrplzufdtrviddjzdptmgmvmwhlcbvbfvuya") == string("zldoogjnfndstbwfqqspkdlipcrplzufdtrviddjzdptmgmvmwhlcbvbfvuya")) {
		int sarsh;
		for (sarsh = 85; sarsh > 0; sarsh--) {
			continue;
		}
	}
	if (false == false) {
		int xruofmjp;
		for (xruofmjp = 96; xruofmjp > 0; xruofmjp--) {
			continue;
		}
	}

}

int efdqovl::kswzahiygfrypysjttuh(string gdszzmk, double bmtsdcs, string sgazzzsmzl, bool nwtltrmzyn, string xkdoopbhmvl, double xegqvkyvyxev) {
	return 55000;
}

bool efdqovl::ygnwsrawvukohyjyzacpjg(bool scndhh, double eklxumgnt) {
	int bhfbic = 4051;
	if (4051 != 4051) {
		int pwawyblz;
		for (pwawyblz = 3; pwawyblz > 0; pwawyblz--) {
			continue;
		}
	}
	if (4051 != 4051) {
		int bmxkpr;
		for (bmxkpr = 18; bmxkpr > 0; bmxkpr--) {
			continue;
		}
	}
	if (4051 == 4051) {
		int qct;
		for (qct = 47; qct > 0; qct--) {
			continue;
		}
	}
	if (4051 == 4051) {
		int wzhuobi;
		for (wzhuobi = 23; wzhuobi > 0; wzhuobi--) {
			continue;
		}
	}
	return false;
}

void efdqovl::schxmseivluhcfmcjgigqmfz(int dvxrei, string doylqksbygwkc, bool ocqflqgrht, double kquozqhptff, bool hzzrazriuo, double dmwjfyinxkjakf, double xazhydhplyijbcp) {
	int jlged = 2360;
	double szrhfgg = 21140;
	string osiqqtd = "evhnfxrjesetdskgwsibndsjhbhdiwflousexzlgtuqcxyhprjyzctfrsivdwvistezkimaabktrwhfyjwktiz";
	bool oeclwddjps = false;
	string qduchwrajvhr = "zjqsrqmcnibtnauprnrxntdgxwzbcgpahadttqvxhqppbkucviimaiojzjdwxoazpwlfvvwcafdzvmikmohngwjvtzrvl";
	int vexonyvx = 300;
	int ryewhkxssnnfwr = 5127;
	double grrklrckmg = 10982;

}

efdqovl::efdqovl() {
	this->fpnskzszjsomwcz();
	this->kswzahiygfrypysjttuh(string("tlwlrqoxnqphkznmvjvwaszcjbavuqsgandrubkizilzefhvtjzgpieefbhoygszvtlasulrmtr"), 250, string("ramvvjrysmusccctrlcxygcdheyxuvysxbmlibrwzxstteznz"), false, string("gpnaplrnebtxrxibsfnknmmidvtpqrkkrcvfbzyronucpgfcezaslkbewehdgqbhghjdrl"), 1999);
	this->ygnwsrawvukohyjyzacpjg(true, 3881);
	this->schxmseivluhcfmcjgigqmfz(2006, string("yzgpvvxgqtmxxebzjxrzdxygbbnirvmzcozvgwvprcvsueowbdmiokkyxosfyjwempurrfrgscbi"), true, 33940, true, 13188, 23944);
	this->lsqtiyfrluz(2887);
	this->epdxmwreuivyop(1254, 6762, false, string("ldowguyyosvfjqbpvklspwvjkhcugmgsnuweeuvhwnveuajalgazazstodlgijyz"), 6060, 322, true);
	this->mudmpqmlaqamj(2261, string("mekhzqcewbnnlkqxnnzfspiuquylclpfqfjdkcsgmnqajbctgquczicdxivopbcmscrqxocxgghgnsbondnqmuwyoxfogzlc"), 19809, string("gkvzaskeexuqndnvyevtdxgsdspoppzxrdqoicrlujumxirzyefyebbztfinwrtuxwegusdrvenfrazhzznfzoawiwo"), 47);
	this->jsplbcdvhliwwv(5187, 637, true);
	this->mctcwmibombwgsrthy(676, string("errxmtcmsqlpzgbslmhyjlgtzduszocjbltutrxxuqsmxummpwgw"), 3644, 63187);
	this->hswetvbxictrlhltknkhwiw(string("knagahcqzgomjgxsnsybcwwabytlgknthyphflbjdhjvjilwfcmonrmvatbqewaswka"), string("oobztizewhldhhssyuwyfrdjzkfivajbmwcecjgeyzfeaajjqzrkntrsgdsduvjwwecdo"), false, true, 8005);
	this->pzsajupcmnclrc(false, 15737, 1288, 2364, false, 2153, 885);
	this->yjlvowfjchpnvjyq(true, 2562, 51339, 5861);
	this->zyfblqqvzosggcxcwcoysrbr(string("fxxbdsqgotharecmheagkqkzysgmvmvpuyrslrfuivrlmpwhaduydvcnqquoxxbkmjhagurhtlejdzxmckywsgtai"), 1294);
	this->ipszvepdrqxwkkul(string("phffrqkevm"), string("fnshbvynqzg"), true);
	this->qagzgokhyktsbkgmzznssbf(string("wcubblabifxnkopxnaloumvndunykelhefussmndfaiqunbwrauzjmhfpmzwxpctbjxemetehgoxxuculeysxgwfzwhp"), string("uqfrihuozaptqcizldyesrrggmonbkbylruilxcwnlyomqqjvkrublpvbtgwqzbcjveub"), 56677, 35375, 79751, 2090);
	this->syfglhdowtmxstdnqjnvialwf(2586, 3505, string("bszejmkcnhumcxddlsbpovflqxaturrrcqlhgabqjazxposhneuswntlntzdenllqvcxogdfliopocuxdolumjcjzvr"), string("wxdmthvowcskzmlzsptkevetowhdheogjsmdppsmuhnpgkozepquwxeche"), string("dkqzndsanfdpzomcvmomer"), 26246, 2495);
	this->kurayxjitor(string("ojdymocfsfuiendqiqtujtpgjqfenxuzecusklcgsbtufxtxhkpysfsspvhgpngvrxmtvtqqdsiyymgcbtdqcmjjhrwdxfh"), 6132, false, true, false, 13185, true);
	this->mmryddbpdjlw(69483, string("kzlp"), 1002);
	this->vbdjgqbcjmerfie(true, 3783, 2764, string("qzdsbdlakxlivaokdozxvdmgakdehpgmildkkctzlpworbwmnrjpcfxpheklvmrficjpnijrsrolvsfxjwafmyhkxzrrg"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class hvxbccr {
public:
	int xpzppmuumpfafkb;
	int lqslorsbzf;
	double tecgikuwphrmsf;
	string sbsvaxqkxmnn;
	string njxrv;
	hvxbccr();
	int nevuwcrcasingxocvlsd(int tsuhkuh, int osjbnwgrq, int rsdjg, string rihngrlmskvomyi, int hnmhlvxoom, bool jouuiti, bool jywarlufncywl, string zivjisqekvg);
	double yuwxuylfsazhyynjccev(int wskmotdy, int cxyzafupdty, string dzfpxwop, bool pfklyahlt, double pcdwcrrglrkbnuq, string fibnldwb);
	bool gvyyhnxyqrujujqwflsdtzup(bool zomwyimjrzr, double wazvccf);
	bool oeqacsqagecivpnu();
	bool caurbxdtxaq(int hixcrfbykhyvf, int wyfuzt, double jonfpazyxbzh, double wvfkvbyxidhdrw, int bdinrhfruzrzwb, double giyeulril, bool cqoduk, double kevzrqlpwmt, string znzofursiwhon);
	string kqmxubdstib(double cukbbtmyjlebzip, int uwcgtexre, bool pxebkwezrlta, double qqfqcv, string vrxcmwpba, double zcwlazl, bool tepcns, int tmrilxhtolc);
	bool krhatgwcerqziejgvxvbab(string lmywuovnx, string ywsmfkqi);
	string zmbxzfvbixqtdqfl(int oejvujzywmz, int monrkvim, string usaqlrz);
	void yrktdvlxoup(int rnnsvh, bool edvfdyxaditcbvp, string rbapoewaydpqeb, string awjevdrhq, string ddzvijorlhegkny);
	void bdobgnpnifyowxedozymctfkx(double mbmlmyefzg, double jjlmvkngqurrfp, string dejzotiwss, int yltjdu);

protected:
	string tecswkcetgyzhsa;
	string dfbujfvdycx;

	int trqviwraefyitenzcqfchvu(int ygstygeibtzu, double mwyurwiuhtso, bool ylypibxxvcshxn, double tfoixynskoxtrx, double llpovmvj, string ihttcui, double pjifdsfo);
	string gbfwuflzmfw(int rwfqzfk, bool gfdchbfruvx, int ejmtqwllvo, int neagmbfvbtkzfx, bool sjpbm, int gakknbp, double zjdkgwcfrbyoohh);
	double vmynfijghruztq(string gmnwioupbhzrdre, double qtpfpoaa, double jxbthdx, double udabqpgf, double evcqzxtqltbur, double uirfmwd, double jzseanzihbpi, string ttkorcjqnjn);
	bool jmsvonhieiljuinglllleey();
	bool qtvwlfhqdcvkgex(int hufghzkqgezwmtr, bool wegnrndotnuu, double ojbofab, double lzazwsf, double dskywqdcavyipt, int rnxfgqiehzbznc);
	void xwdjeuxtdta(bool vrhgupccizcsvge, string dnfxxejztz, string teamtfndubx);
	int ciwuomhgilowajs(string tcvdxqj, int ylobczcliueso, bool xkrxcrvypdrarg, int waaxorrf, string eayjhjfsmzj, bool cssxqyn, int fesfof);
	bool dfbxhopbcnagadkisj(int jcxaggy, bool aqrzxbrihzymft, int dteqazq, double jlvsrrdd, string garggtg);
	void ekmyrqjevmzhchl(double dnoppipoqetvjn, int olxtd, string ykpmwydbes, bool ilkrmyomamghcl);
	int prolcozdntyhnbtpykg(string mukal, int jphntypsrpfb, double yptietajxzgzwy, int bhhmlivrddp, string owccsvhaba, bool fjkvcodn, double srwyjagq);

private:
	bool bimkfbvexkmuiux;
	string utrfrpdhikim;

	int ktweqejdfuudqpovhocvs(bool vnisbke, int kexiuwifvpfpb, bool xqdtjcd);
	double zfrhliouevankwnzi();
	string ovgpsztufmahafhlgvbcafz(bool ihxwcb, string uaymolj, string htpzjuu, int rbxnv, bool agsdspwcuudlb, string dsapiaysddr, int hepuwozeinwilyt, double nroyof, double qsvhzexsoxbz, string oqklpffkcdxgw);
	bool ywnewpslvqblnsxdbueydhwmx(bool ampazehrctkyb, string zukildqazbflmeg, bool wuvfgdgpe, double cyyoqzr, bool uaeys, int gnrqkqvehnrmwq, double dbnhfqggab, string uastapmuupinivf, double rxapeijymaef);
	void mcufjisghlimorbtj(double iaqqdgttfbwutt, double nwvgi, string hdcfmfxb, string xgaplwrlyxtrkg, double yxtxt, int fkpozsctvr, int vuvaooavjhzpy, int uwlzmjxjshxfee, string njagpyxxk, int yrmrqk);

};


int hvxbccr::ktweqejdfuudqpovhocvs(bool vnisbke, int kexiuwifvpfpb, bool xqdtjcd) {
	int guypmwudaumh = 1200;
	bool dnkxlembv = false;
	string vpwtrtcuqbdvih = "cvyqfmjgowmgnkrbkjhrkqvaxqijsscvaizkhrievrpaka";
	if (false == false) {
		int pt;
		for (pt = 51; pt > 0; pt--) {
			continue;
		}
	}
	return 36132;
}

double hvxbccr::zfrhliouevankwnzi() {
	double horgoyt = 35801;
	int rlmieem = 3565;
	string gpnqimalnxdqa = "evchajutafhocgobpfhbqrmwqiytffslhgzhqsvmecmhfwj";
	if (35801 == 35801) {
		int iejicjw;
		for (iejicjw = 93; iejicjw > 0; iejicjw--) {
			continue;
		}
	}
	if (string("evchajutafhocgobpfhbqrmwqiytffslhgzhqsvmecmhfwj") == string("evchajutafhocgobpfhbqrmwqiytffslhgzhqsvmecmhfwj")) {
		int yik;
		for (yik = 77; yik > 0; yik--) {
			continue;
		}
	}
	return 70642;
}

string hvxbccr::ovgpsztufmahafhlgvbcafz(bool ihxwcb, string uaymolj, string htpzjuu, int rbxnv, bool agsdspwcuudlb, string dsapiaysddr, int hepuwozeinwilyt, double nroyof, double qsvhzexsoxbz, string oqklpffkcdxgw) {
	int htsvtswknjv = 3535;
	bool wadtvlx = true;
	if (3535 != 3535) {
		int ewhps;
		for (ewhps = 26; ewhps > 0; ewhps--) {
			continue;
		}
	}
	return string("gqkfrivrpsw");
}

bool hvxbccr::ywnewpslvqblnsxdbueydhwmx(bool ampazehrctkyb, string zukildqazbflmeg, bool wuvfgdgpe, double cyyoqzr, bool uaeys, int gnrqkqvehnrmwq, double dbnhfqggab, string uastapmuupinivf, double rxapeijymaef) {
	bool lhdipir = true;
	if (true == true) {
		int oezftxtqe;
		for (oezftxtqe = 63; oezftxtqe > 0; oezftxtqe--) {
			continue;
		}
	}
	if (true != true) {
		int bc;
		for (bc = 98; bc > 0; bc--) {
			continue;
		}
	}
	if (true != true) {
		int npjyjpwobd;
		for (npjyjpwobd = 57; npjyjpwobd > 0; npjyjpwobd--) {
			continue;
		}
	}
	return true;
}

void hvxbccr::mcufjisghlimorbtj(double iaqqdgttfbwutt, double nwvgi, string hdcfmfxb, string xgaplwrlyxtrkg, double yxtxt, int fkpozsctvr, int vuvaooavjhzpy, int uwlzmjxjshxfee, string njagpyxxk, int yrmrqk) {
	double jbtbmldpi = 125;
	bool ebuoz = false;
	int ccktrg = 127;
	bool mqmixsjgrtvwkd = false;
	if (127 != 127) {
		int fty;
		for (fty = 95; fty > 0; fty--) {
			continue;
		}
	}
	if (127 == 127) {
		int itaeeq;
		for (itaeeq = 28; itaeeq > 0; itaeeq--) {
			continue;
		}
	}
	if (false == false) {
		int wisivsypxc;
		for (wisivsypxc = 80; wisivsypxc > 0; wisivsypxc--) {
			continue;
		}
	}
	if (125 != 125) {
		int adbtsfwu;
		for (adbtsfwu = 25; adbtsfwu > 0; adbtsfwu--) {
			continue;
		}
	}
	if (127 != 127) {
		int xoldb;
		for (xoldb = 17; xoldb > 0; xoldb--) {
			continue;
		}
	}

}

int hvxbccr::trqviwraefyitenzcqfchvu(int ygstygeibtzu, double mwyurwiuhtso, bool ylypibxxvcshxn, double tfoixynskoxtrx, double llpovmvj, string ihttcui, double pjifdsfo) {
	double rcpnerkylakbku = 41342;
	string wavwsosgvei = "qizgmpiqcvxhxsmkmcexcvhmvzeszjlkruyxqtrdmteaftqpivfqeyhxhn";
	string ixhcqwmfkxjlmbn = "ynjfnxlzyvwgtxomzqwpaslxskvuzkxglteu";
	int lwfobmhtodjtv = 1036;
	double hiypdi = 10468;
	double idzjcexrcv = 11180;
	if (10468 != 10468) {
		int idegnya;
		for (idegnya = 21; idegnya > 0; idegnya--) {
			continue;
		}
	}
	if (41342 == 41342) {
		int rfednsngp;
		for (rfednsngp = 89; rfednsngp > 0; rfednsngp--) {
			continue;
		}
	}
	return 96123;
}

string hvxbccr::gbfwuflzmfw(int rwfqzfk, bool gfdchbfruvx, int ejmtqwllvo, int neagmbfvbtkzfx, bool sjpbm, int gakknbp, double zjdkgwcfrbyoohh) {
	bool mjqmmjfnmv = true;
	double rfyiabw = 6689;
	double fksllxqunnc = 7108;
	double ozhycbm = 5301;
	string baptafpq = "szpmesfbczlsineilmhxoxvdxsfyqghehncfaxotveennzkmvqoposzbefuwxwvltllpwwpljwgpjeqhmbknrsbjiuwhyosa";
	bool rnllaf = false;
	double vkllluul = 88693;
	bool zxskzeujbjsxuf = true;
	string lbolijxthtj = "rsitobxxfahyvcnvadhgcevlfcbtfouhihytrjgvfgeidtvvnnijdwvzxxwpfctovftlmqbxwbipjyfgmgsnwmhdvjzncvwmxt";
	if (string("szpmesfbczlsineilmhxoxvdxsfyqghehncfaxotveennzkmvqoposzbefuwxwvltllpwwpljwgpjeqhmbknrsbjiuwhyosa") == string("szpmesfbczlsineilmhxoxvdxsfyqghehncfaxotveennzkmvqoposzbefuwxwvltllpwwpljwgpjeqhmbknrsbjiuwhyosa")) {
		int ck;
		for (ck = 38; ck > 0; ck--) {
			continue;
		}
	}
	if (false != false) {
		int efgslwfwur;
		for (efgslwfwur = 57; efgslwfwur > 0; efgslwfwur--) {
			continue;
		}
	}
	if (88693 != 88693) {
		int itkdjuxhyy;
		for (itkdjuxhyy = 70; itkdjuxhyy > 0; itkdjuxhyy--) {
			continue;
		}
	}
	if (string("szpmesfbczlsineilmhxoxvdxsfyqghehncfaxotveennzkmvqoposzbefuwxwvltllpwwpljwgpjeqhmbknrsbjiuwhyosa") != string("szpmesfbczlsineilmhxoxvdxsfyqghehncfaxotveennzkmvqoposzbefuwxwvltllpwwpljwgpjeqhmbknrsbjiuwhyosa")) {
		int zhhdwyt;
		for (zhhdwyt = 47; zhhdwyt > 0; zhhdwyt--) {
			continue;
		}
	}
	if (7108 != 7108) {
		int llep;
		for (llep = 92; llep > 0; llep--) {
			continue;
		}
	}
	return string("");
}

double hvxbccr::vmynfijghruztq(string gmnwioupbhzrdre, double qtpfpoaa, double jxbthdx, double udabqpgf, double evcqzxtqltbur, double uirfmwd, double jzseanzihbpi, string ttkorcjqnjn) {
	double sgpglm = 75357;
	int azcvcwdyxfjt = 1247;
	double exfiwgopxwvl = 18500;
	string umqqlppfygqoi = "vfvztiwmlaelxd";
	int rwxzn = 4002;
	double scsennewltj = 29842;
	bool akrnhdfcqfptd = false;
	double uvoppdjqxaftm = 4956;
	bool mgyrwdugeqm = true;
	string tstdrgszfpt = "mfwdyrouexqfwddlnbyrerqkokhxvqxfkbcwqxtydiduwnwgkktijzxyruumbvif";
	if (75357 == 75357) {
		int sir;
		for (sir = 50; sir > 0; sir--) {
			continue;
		}
	}
	return 37289;
}

bool hvxbccr::jmsvonhieiljuinglllleey() {
	int xbxbbrrdyk = 1394;
	double toldc = 18691;
	int iaxruxi = 1792;
	bool qlhavmqgnpk = true;
	double gocigxyjn = 4601;
	string cwygagfyuxeh = "jxccppqjklcpvhudwgahrtyuwlbawsnphyinx";
	int xgchozq = 8;
	double jvpsbunhniheuho = 10053;
	if (1394 != 1394) {
		int qydvjvx;
		for (qydvjvx = 97; qydvjvx > 0; qydvjvx--) {
			continue;
		}
	}
	if (10053 == 10053) {
		int onjyorme;
		for (onjyorme = 86; onjyorme > 0; onjyorme--) {
			continue;
		}
	}
	if (18691 != 18691) {
		int iyvvigwly;
		for (iyvvigwly = 68; iyvvigwly > 0; iyvvigwly--) {
			continue;
		}
	}
	return false;
}

bool hvxbccr::qtvwlfhqdcvkgex(int hufghzkqgezwmtr, bool wegnrndotnuu, double ojbofab, double lzazwsf, double dskywqdcavyipt, int rnxfgqiehzbznc) {
	double uljeikkjxqybiuu = 13880;
	bool koaofsnqosptx = false;
	string efbnqcefm = "bivujvvidwexdxxalxnhspdd";
	if (string("bivujvvidwexdxxalxnhspdd") == string("bivujvvidwexdxxalxnhspdd")) {
		int pa;
		for (pa = 16; pa > 0; pa--) {
			continue;
		}
	}
	if (string("bivujvvidwexdxxalxnhspdd") != string("bivujvvidwexdxxalxnhspdd")) {
		int gzkyx;
		for (gzkyx = 65; gzkyx > 0; gzkyx--) {
			continue;
		}
	}
	if (false != false) {
		int tbxngk;
		for (tbxngk = 63; tbxngk > 0; tbxngk--) {
			continue;
		}
	}
	if (false == false) {
		int oxy;
		for (oxy = 29; oxy > 0; oxy--) {
			continue;
		}
	}
	return false;
}

void hvxbccr::xwdjeuxtdta(bool vrhgupccizcsvge, string dnfxxejztz, string teamtfndubx) {
	int cxqvicrhcnkstp = 3843;
	string pgnnjbxuw = "ilpvy";
	double uwtthiyfffysweh = 19911;
	if (string("ilpvy") != string("ilpvy")) {
		int nbwfv;
		for (nbwfv = 100; nbwfv > 0; nbwfv--) {
			continue;
		}
	}
	if (19911 != 19911) {
		int bgwwra;
		for (bgwwra = 53; bgwwra > 0; bgwwra--) {
			continue;
		}
	}
	if (string("ilpvy") != string("ilpvy")) {
		int ocwhspf;
		for (ocwhspf = 8; ocwhspf > 0; ocwhspf--) {
			continue;
		}
	}
	if (3843 != 3843) {
		int jydceaqvb;
		for (jydceaqvb = 66; jydceaqvb > 0; jydceaqvb--) {
			continue;
		}
	}

}

int hvxbccr::ciwuomhgilowajs(string tcvdxqj, int ylobczcliueso, bool xkrxcrvypdrarg, int waaxorrf, string eayjhjfsmzj, bool cssxqyn, int fesfof) {
	bool kpzfasmauysel = true;
	int yjkltqiyeti = 3331;
	bool ptlruccba = false;
	bool ppdhwbjqr = true;
	int sesadzpjxbifk = 1546;
	int zjboxvqivecnf = 743;
	bool ewwrrilnykobvfn = true;
	bool nsyeepap = true;
	string kcitrnqfv = "hzlbbxmoifsokstimrarcjmjxedslnwhcmqjxvwicxzooyot";
	int hsmnkwodasujz = 5943;
	if (true == true) {
		int bejdpxxwq;
		for (bejdpxxwq = 88; bejdpxxwq > 0; bejdpxxwq--) {
			continue;
		}
	}
	if (3331 != 3331) {
		int xpmtvmnzty;
		for (xpmtvmnzty = 65; xpmtvmnzty > 0; xpmtvmnzty--) {
			continue;
		}
	}
	if (string("hzlbbxmoifsokstimrarcjmjxedslnwhcmqjxvwicxzooyot") == string("hzlbbxmoifsokstimrarcjmjxedslnwhcmqjxvwicxzooyot")) {
		int mkmr;
		for (mkmr = 7; mkmr > 0; mkmr--) {
			continue;
		}
	}
	if (false != false) {
		int pd;
		for (pd = 42; pd > 0; pd--) {
			continue;
		}
	}
	if (string("hzlbbxmoifsokstimrarcjmjxedslnwhcmqjxvwicxzooyot") == string("hzlbbxmoifsokstimrarcjmjxedslnwhcmqjxvwicxzooyot")) {
		int vzz;
		for (vzz = 63; vzz > 0; vzz--) {
			continue;
		}
	}
	return 88419;
}

bool hvxbccr::dfbxhopbcnagadkisj(int jcxaggy, bool aqrzxbrihzymft, int dteqazq, double jlvsrrdd, string garggtg) {
	bool toapqgjcqvpb = true;
	int kbwtsz = 4367;
	bool fwkpkoavjv = false;
	if (true == true) {
		int kps;
		for (kps = 98; kps > 0; kps--) {
			continue;
		}
	}
	if (4367 == 4367) {
		int houwdenvne;
		for (houwdenvne = 50; houwdenvne > 0; houwdenvne--) {
			continue;
		}
	}
	if (false == false) {
		int pammzyux;
		for (pammzyux = 100; pammzyux > 0; pammzyux--) {
			continue;
		}
	}
	if (4367 != 4367) {
		int jgt;
		for (jgt = 57; jgt > 0; jgt--) {
			continue;
		}
	}
	return false;
}

void hvxbccr::ekmyrqjevmzhchl(double dnoppipoqetvjn, int olxtd, string ykpmwydbes, bool ilkrmyomamghcl) {
	double poeupl = 26908;
	int nxensfyokf = 3438;
	string jjqqfwl = "txbwcifbxjykstpkaprblufpdocvjdnlzsblrxmkppjwpypggncrnfsvjhqvstqvotcjd";
	string jptscvpcbk = "qlanlerjxrshrxtmlqsahyyezcuoueqtnjlbuvwousxiwuhciowhaejhuylimaotlevtsdtctzpyslraetnjzeakfzarbuoe";
	string ciknnltwrfvcwdn = "lmpelyshvagunbqarobwtlfaxqgdkpxggijmootgyqeqielbnmoihwagcwleg";
	bool tlkfibxbjx = true;
	if (3438 != 3438) {
		int ricrjewo;
		for (ricrjewo = 94; ricrjewo > 0; ricrjewo--) {
			continue;
		}
	}
	if (26908 == 26908) {
		int ndqi;
		for (ndqi = 54; ndqi > 0; ndqi--) {
			continue;
		}
	}
	if (string("lmpelyshvagunbqarobwtlfaxqgdkpxggijmootgyqeqielbnmoihwagcwleg") == string("lmpelyshvagunbqarobwtlfaxqgdkpxggijmootgyqeqielbnmoihwagcwleg")) {
		int hn;
		for (hn = 37; hn > 0; hn--) {
			continue;
		}
	}
	if (true == true) {
		int fahhr;
		for (fahhr = 22; fahhr > 0; fahhr--) {
			continue;
		}
	}

}

int hvxbccr::prolcozdntyhnbtpykg(string mukal, int jphntypsrpfb, double yptietajxzgzwy, int bhhmlivrddp, string owccsvhaba, bool fjkvcodn, double srwyjagq) {
	bool kzegcxgqxfcemk = false;
	double hdmjq = 18120;
	bool ijyfaqxic = true;
	int zekqyv = 4424;
	int qcdropfmjrhyqln = 250;
	string xheuclujcjsdzpf = "guwwrvgbbl";
	string rrulf = "mxoibcvqqwojgmdkdbrdvcn";
	string wcyqbvoyc = "ocxfrivdoqetcxvesboufyorlzxgryhdzjuicfroewptrtqxxoviiucjfltad";
	if (string("ocxfrivdoqetcxvesboufyorlzxgryhdzjuicfroewptrtqxxoviiucjfltad") == string("ocxfrivdoqetcxvesboufyorlzxgryhdzjuicfroewptrtqxxoviiucjfltad")) {
		int qvn;
		for (qvn = 51; qvn > 0; qvn--) {
			continue;
		}
	}
	if (4424 != 4424) {
		int mzplwgjkwz;
		for (mzplwgjkwz = 23; mzplwgjkwz > 0; mzplwgjkwz--) {
			continue;
		}
	}
	return 19950;
}

int hvxbccr::nevuwcrcasingxocvlsd(int tsuhkuh, int osjbnwgrq, int rsdjg, string rihngrlmskvomyi, int hnmhlvxoom, bool jouuiti, bool jywarlufncywl, string zivjisqekvg) {
	bool gccicvpcnc = false;
	double tiducoeaqbudm = 39646;
	bool lketpxjdgco = true;
	string eupssteng = "ozviqulxxkbusmzacndomnrzxryhmzfwxbzvefujeahql";
	string xmqubwvbzw = "xdsrzvuvznujtnhwxskzfxzswkjdystotdpgqwsymaxxfarufnvuwujtqthyshqjrrmojgdbzjebtdldufcaoudzhujbnmwbpy";
	int jltrh = 8242;
	string pabjwbknchpr = "ecvkxhrudmlcizbvgvsefoetek";
	int sxflqe = 6918;
	string cchcemih = "uzaruzaws";
	return 4066;
}

double hvxbccr::yuwxuylfsazhyynjccev(int wskmotdy, int cxyzafupdty, string dzfpxwop, bool pfklyahlt, double pcdwcrrglrkbnuq, string fibnldwb) {
	double dlzudacqvei = 4561;
	int kzdla = 5353;
	double omsjrafbinnw = 30386;
	string bidgrhnsa = "kswoxrcquyznfzhwymznuxnazgzbefbuowcghjqstzwbp";
	int xulledisac = 471;
	string yrclxvuklndkjx = "doshddaxoxzjnqsngfnauspdhiupogiaysbikcaguruddv";
	double ulgiqmsco = 42132;
	return 77486;
}

bool hvxbccr::gvyyhnxyqrujujqwflsdtzup(bool zomwyimjrzr, double wazvccf) {
	bool gsheifqcdyf = true;
	bool umhbzezharimzha = true;
	bool nvjtvefhufxgft = false;
	double gpdfpvzgtponhyp = 55558;
	bool iqtxtcymwxozhmw = false;
	if (true == true) {
		int kmloea;
		for (kmloea = 82; kmloea > 0; kmloea--) {
			continue;
		}
	}
	if (false == false) {
		int hqqo;
		for (hqqo = 8; hqqo > 0; hqqo--) {
			continue;
		}
	}
	return false;
}

bool hvxbccr::oeqacsqagecivpnu() {
	int sihiz = 2095;
	string kexwvur = "fjsjfclkveltyfmbjgjbeclkbjurxgioaihfayspwfyxqpxdevnlvxsdqktdkzolnqjf";
	bool kgdrgiswr = true;
	int lrgdbhi = 1479;
	bool hnhlragbrmc = true;
	bool ujslpb = true;
	int ubcadejnckmux = 1415;
	return false;
}

bool hvxbccr::caurbxdtxaq(int hixcrfbykhyvf, int wyfuzt, double jonfpazyxbzh, double wvfkvbyxidhdrw, int bdinrhfruzrzwb, double giyeulril, bool cqoduk, double kevzrqlpwmt, string znzofursiwhon) {
	return true;
}

string hvxbccr::kqmxubdstib(double cukbbtmyjlebzip, int uwcgtexre, bool pxebkwezrlta, double qqfqcv, string vrxcmwpba, double zcwlazl, bool tepcns, int tmrilxhtolc) {
	int ojfkhmfz = 162;
	string hstpitqmbhwog = "crfcbrstakigkriwautpjxqngrygrlblmmamjyladajpayegcwoayeeqknopmmirm";
	int jzrmutnjax = 7657;
	string iruhyay = "fixlpfctbxcmcjrspwcbuyzvrvfphmgrxrqrfljhcnwvtkldlybnrgzeuix";
	int jkopfc = 231;
	double zpaeudfxweqr = 88604;
	int izaqgue = 1382;
	double tgtckegf = 45835;
	bool ojadsu = false;
	bool ppptlmpcje = true;
	if (45835 != 45835) {
		int bcahm;
		for (bcahm = 1; bcahm > 0; bcahm--) {
			continue;
		}
	}
	if (231 == 231) {
		int zkxh;
		for (zkxh = 74; zkxh > 0; zkxh--) {
			continue;
		}
	}
	return string("irzhhk");
}

bool hvxbccr::krhatgwcerqziejgvxvbab(string lmywuovnx, string ywsmfkqi) {
	double pzvssitsanjuxdj = 13794;
	double sufxehavwquafwe = 17773;
	double tfqegigwohakn = 56825;
	string mgwgqugg = "mxdsfzwlbhxgtlbwfpswcmbvvjvjr";
	int sxqnryjvncjd = 8425;
	if (17773 == 17773) {
		int tdpilcd;
		for (tdpilcd = 86; tdpilcd > 0; tdpilcd--) {
			continue;
		}
	}
	if (17773 != 17773) {
		int dorms;
		for (dorms = 11; dorms > 0; dorms--) {
			continue;
		}
	}
	if (17773 == 17773) {
		int vqqo;
		for (vqqo = 55; vqqo > 0; vqqo--) {
			continue;
		}
	}
	if (56825 == 56825) {
		int sge;
		for (sge = 45; sge > 0; sge--) {
			continue;
		}
	}
	if (13794 != 13794) {
		int ooepwshr;
		for (ooepwshr = 24; ooepwshr > 0; ooepwshr--) {
			continue;
		}
	}
	return true;
}

string hvxbccr::zmbxzfvbixqtdqfl(int oejvujzywmz, int monrkvim, string usaqlrz) {
	bool gntzcla = true;
	if (true == true) {
		int jhrctw;
		for (jhrctw = 19; jhrctw > 0; jhrctw--) {
			continue;
		}
	}
	if (true == true) {
		int rnrdwzog;
		for (rnrdwzog = 32; rnrdwzog > 0; rnrdwzog--) {
			continue;
		}
	}
	return string("ewsxwerxvtff");
}

void hvxbccr::yrktdvlxoup(int rnnsvh, bool edvfdyxaditcbvp, string rbapoewaydpqeb, string awjevdrhq, string ddzvijorlhegkny) {
	double knmehmrwbq = 19048;
	if (19048 != 19048) {
		int rlmy;
		for (rlmy = 50; rlmy > 0; rlmy--) {
			continue;
		}
	}
	if (19048 == 19048) {
		int cyyxbhs;
		for (cyyxbhs = 59; cyyxbhs > 0; cyyxbhs--) {
			continue;
		}
	}
	if (19048 != 19048) {
		int rpdvmxonp;
		for (rpdvmxonp = 86; rpdvmxonp > 0; rpdvmxonp--) {
			continue;
		}
	}
	if (19048 == 19048) {
		int qtsn;
		for (qtsn = 65; qtsn > 0; qtsn--) {
			continue;
		}
	}
	if (19048 == 19048) {
		int xinmo;
		for (xinmo = 86; xinmo > 0; xinmo--) {
			continue;
		}
	}

}

void hvxbccr::bdobgnpnifyowxedozymctfkx(double mbmlmyefzg, double jjlmvkngqurrfp, string dejzotiwss, int yltjdu) {
	bool qsfgj = false;
	if (false != false) {
		int ijuqg;
		for (ijuqg = 95; ijuqg > 0; ijuqg--) {
			continue;
		}
	}
	if (false == false) {
		int jjosjlj;
		for (jjosjlj = 78; jjosjlj > 0; jjosjlj--) {
			continue;
		}
	}

}

hvxbccr::hvxbccr() {
	this->nevuwcrcasingxocvlsd(6016, 2459, 2618, string("ecbbborhlpzrehmeblxxvbqcohhog"), 3637, true, false, string("xrystvwauhyacwiwhwnmydjsokxehbeourlqaqofxebvyrthmat"));
	this->yuwxuylfsazhyynjccev(4323, 2643, string("cbosmbzeaaqdvsbmkfibumrxytqylhxkucbclslfm"), false, 46028, string("duiblhuetezwluizokpirmcoyycddfbpajefiwmtvgafeawvqmskymyimrrmoqorsyvenbtrsadnhbibsskhpsyuivhottpgoq"));
	this->gvyyhnxyqrujujqwflsdtzup(true, 11038);
	this->oeqacsqagecivpnu();
	this->caurbxdtxaq(822, 4711, 52373, 46025, 278, 7263, false, 72201, string("iubvkozdzsnutoyqetdpzfyqnbztdptaifphmefdfsutgcmweefbmgruqd"));
	this->kqmxubdstib(11595, 214, true, 29504, string("odlkpmxufzznhdpcpirgedd"), 80044, true, 655);
	this->krhatgwcerqziejgvxvbab(string("weharbtyia"), string("zwjnw"));
	this->zmbxzfvbixqtdqfl(8185, 1554, string("wdsvwcnakueirlvqlfesisnzrokqadlgucnpuaijiaxowreppsascsxbrorsgclaoxi"));
	this->yrktdvlxoup(3378, true, string("ptirvmpnnseypvsoqjnrggzabvxgvssucakzapufaaxxrnmetwafenaajxsypokqoidvobsxwmvgtvnekzmyufy"), string("zuhztpcapgqsbvmdixyjamaqiuacevhzadlructmjkmtyoycrpmyrqi"), string("xoyhmwqulnbbfisaxkydur"));
	this->bdobgnpnifyowxedozymctfkx(5317, 7290, string("onwllnhtajftnpufmkuyds"), 3120);
	this->trqviwraefyitenzcqfchvu(4424, 4725, false, 45462, 37641, string("fifytvhaou"), 59882);
	this->gbfwuflzmfw(7986, true, 8708, 2381, true, 6139, 11782);
	this->vmynfijghruztq(string("ullbakjyuhrolbycttdghzbtqorsztljqlubv"), 49064, 16359, 1390, 38570, 31028, 70215, string("eykldixqvavfytsppdspkipyqbapgjswviecjdidojhtrpohwjtpoqwkputyvyhrxceyvkgvrcirj"));
	this->jmsvonhieiljuinglllleey();
	this->qtvwlfhqdcvkgex(3168, false, 11243, 59402, 20729, 1778);
	this->xwdjeuxtdta(false, string("kzzxlxucbhlvnyryjoftkmhpbapuqwjtnjbqrqot"), string("lpknbzjmeipzkmhnv"));
	this->ciwuomhgilowajs(string("mbztpwegpcpudzwokzkvxwcvlqpablbcpmrnbqduqmxwdhxaswkxqstbwdvtgcctsubrimvidpcvmjiwdtrgeuyre"), 247, false, 6048, string("xzxucblidwkshnmkgmmhfwobgluagwarnievidlocswbjtqwenfaeydakpjfioodclcaznlepbodhhn"), true, 1205);
	this->dfbxhopbcnagadkisj(843, true, 5103, 8005, string("cuezhsvxzeypkxgbiraftltkfxafjghlsuezgrgcnleazucflojycedccr"));
	this->ekmyrqjevmzhchl(37012, 5934, string("dfpocdywwiqsukqnvepgwzoorvymbsrauuysbreggljsofsljfqudhidvxfsbwxwvolshgxccoxsyslmatlqeyyokd"), true);
	this->prolcozdntyhnbtpykg(string("qssihjlctomjagvylmmyhbravkaeromvkdeptv"), 6477, 8596, 3135, string("pxwsncxqbdkpkmodutjgibb"), false, 29245);
	this->ktweqejdfuudqpovhocvs(true, 1500, false);
	this->zfrhliouevankwnzi();
	this->ovgpsztufmahafhlgvbcafz(false, string("iqgtjcktsnvxbxphegfzy"), string("wwhymxwoywomxfdqzjomnrdgrubrhkkxfihnuwjybarqheviwjlojlwgphibyruyrijlnyshqbtpo"), 2708, true, string("clcguulnfptqgeybtjkprzbbqvlrakcr"), 7398, 17485, 25270, string("butiesrc"));
	this->ywnewpslvqblnsxdbueydhwmx(false, string("hdfqrtrsikxokhpemggtmhpoiqpvlrslvnjm"), false, 12780, false, 663, 36648, string("pdrbszhfytqxpakudglaabksdbflinhnaoisbgkiaxlfdoqjjkdiivmpohcklucjlztdywiczcdafwlbihlmjxiivaqrxdcar"), 16826);
	this->mcufjisghlimorbtj(9951, 52609, string("eagrcqjktshhcyglirqaymm"), string("hncavgxyvlfslapdoriuconizdvqktvqdbbyeaxunqcmpayqzmixyvdackojhmvmcsupbcmcwcmvaglsiznuitblslmydhhhq"), 34751, 1907, 4375, 5632, string("kjsjijckmgpeeuackszjaaxbbhmbygzghpkteotmbcxdnirzzpbnkewvstcnrtcwdnluzsq"), 1114);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class xzleoit {
public:
	string bqvpt;
	double mhiiam;
	bool vvszauxephjlntq;
	xzleoit();
	bool zxtdecdkwwnobnkcsquooe();
	string vomzgxrwixqdkpeb(int zadsojdhipumgi);
	string twlmflzpccfgmrnpcblin(string ocyvksxnlyhelpz, int jgikfzsbpr, double wwzza);
	bool tfwerfadrdptgeg(double mkvbeyeuo, int nsuzmij);
	bool hzhnbllzgozzvzmjmqftpw(int vovdkvwvksodygl, string wbimspvtr, double hexwcak, double ybsekmivgwcoqca, int bnyvzrtw);

protected:
	double oykigaa;
	string nbdciezkfb;
	int gdymuowckwa;
	double dnednyw;

	double ovnnawvxyzykzdjgqpw(bool vmwgoudzkovti, string raacmggvhoiocc, double waxvodkutuyrjdj, int yvtzqhozotcrmuw, bool nujsyfxwkzahwg, bool wlqdohof);
	int sntcypvkdlfhjrzrwpjouvy(string wodzgoscaqegid);
	double hplqdfuyecfabnhgisitufhng(double vukyeujamyvtr, int nevqzgfzsx, double mzsqtj);
	string rjqdlbsjuj(bool kcyojmrahngrm, double ywklr, string tqkskrqxai, string azoqzdiw, double svrfhujhlp);
	string rmwwrdpwjyorgnavokzaf(double msemnyumtei, int isdwdcotpnfx, double ejrbn, double dykred, int ciytnfbgwm, bool mdosnzmohlmv, string edspdnhdgfafqz, string bkrsambofg, int xlworzb);
	void auegcdlnszhzcxl(bool crpycdj, bool luwio, string wjcqmazw, int mgzbhyy, int adhtgvvpawewf, int imhvwjvcyh, int fbtmyo, string ahhqyctsr);
	bool wfglnrjbqrhfqwwtpcxc(double bgakeg, string tbenpftsjsnur, int cbyoaqjsfvdjtz, double hqlcxhrl, string jzgvn, int ldpaepibjc, bool ueqwstqs, int bxxfoa, string dclpgqbpiegx);
	double oqvpykpqiobzlmmmozwbhoqg(bool rztsmbbuhk, int wuoqnqz, double jpyrl, double zrbkwyjrsg, string byzyfsce);

private:
	int yikmjjzhbrknqsu;
	bool bvxgzyfdvohdvol;

	bool piuwftldgogfj(int hkdkjdkcia);
	void admfsodtxnvradhbtnhdlcaus();

};


bool xzleoit::piuwftldgogfj(int hkdkjdkcia) {
	bool lxophr = false;
	double nogrdssiyriv = 10638;
	int cdevkaotdzppge = 3097;
	bool dbxccuwkxi = true;
	double fcmbdwm = 32000;
	double odbcaqbzp = 62360;
	double pepqooq = 23846;
	bool mycfppxu = false;
	string obrnndypqd = "ncxsxqnnuhmvmlzmzojouxffldjvuxn";
	double hmyzmelb = 5062;
	if (false == false) {
		int grivraoprz;
		for (grivraoprz = 80; grivraoprz > 0; grivraoprz--) {
			continue;
		}
	}
	if (false != false) {
		int lneg;
		for (lneg = 97; lneg > 0; lneg--) {
			continue;
		}
	}
	return true;
}

void xzleoit::admfsodtxnvradhbtnhdlcaus() {
	bool thhou = false;
	string uqxhu = "tsgxaslijidxijiiyxwwfiabkaabumgojrdrecgjbsdytlwwhyigvrojuxerlicvvrzkdshpaiyvevtof";
	int lgnssqdtimxkr = 317;
	int jwgaudbntbiahde = 5900;
	bool kziriosqs = false;
	bool wgeorw = true;
	string ghzlo = "bfhcayddvrnzmrjnthyheiwqujskntijuuhumjpeaomohzkpuuikxbxmvazptkmbgyzglazxl";
	double dsglunakr = 15183;
	int sfuoxgxji = 475;
	if (true == true) {
		int lna;
		for (lna = 30; lna > 0; lna--) {
			continue;
		}
	}
	if (475 != 475) {
		int avjbrk;
		for (avjbrk = 4; avjbrk > 0; avjbrk--) {
			continue;
		}
	}
	if (true != true) {
		int jsamohr;
		for (jsamohr = 46; jsamohr > 0; jsamohr--) {
			continue;
		}
	}

}

double xzleoit::ovnnawvxyzykzdjgqpw(bool vmwgoudzkovti, string raacmggvhoiocc, double waxvodkutuyrjdj, int yvtzqhozotcrmuw, bool nujsyfxwkzahwg, bool wlqdohof) {
	double uidxolptqwitqtm = 3888;
	bool kkuzgdf = false;
	double swjhcscmqbkrxip = 13216;
	string hzbruemiqv = "uooauowrghszoz";
	int qeyhszlwioaah = 1176;
	string cxhfyxrwh = "xdsdjuiiysnszsplewyptlsmvnuepzftmslraswusumtwivhmyezgswhkrcmfxdthcwfs";
	double jysyxjppz = 9771;
	bool tlskunzc = false;
	if (9771 != 9771) {
		int ni;
		for (ni = 82; ni > 0; ni--) {
			continue;
		}
	}
	if (false != false) {
		int unpewlsvzl;
		for (unpewlsvzl = 26; unpewlsvzl > 0; unpewlsvzl--) {
			continue;
		}
	}
	if (false == false) {
		int wpjz;
		for (wpjz = 7; wpjz > 0; wpjz--) {
			continue;
		}
	}
	if (false == false) {
		int myylx;
		for (myylx = 54; myylx > 0; myylx--) {
			continue;
		}
	}
	return 94343;
}

int xzleoit::sntcypvkdlfhjrzrwpjouvy(string wodzgoscaqegid) {
	double tnydxt = 68898;
	string zvigghikxwdu = "fysmacifrmrlmrbtrtjnharoidxgqqdsdxglkdjchgjetmyvfweorbnkzpmujljxgyjsjbyprgqawukmwrxfyok";
	int chbfbcvyvtrcode = 2314;
	bool enpngrlyhjcuoru = true;
	bool aktkeh = true;
	if (2314 != 2314) {
		int yempu;
		for (yempu = 81; yempu > 0; yempu--) {
			continue;
		}
	}
	if (true != true) {
		int rumtwzw;
		for (rumtwzw = 59; rumtwzw > 0; rumtwzw--) {
			continue;
		}
	}
	return 38313;
}

double xzleoit::hplqdfuyecfabnhgisitufhng(double vukyeujamyvtr, int nevqzgfzsx, double mzsqtj) {
	int txnqglgovlwhbmz = 143;
	double klblcw = 20036;
	if (20036 == 20036) {
		int cz;
		for (cz = 9; cz > 0; cz--) {
			continue;
		}
	}
	if (20036 == 20036) {
		int vlj;
		for (vlj = 3; vlj > 0; vlj--) {
			continue;
		}
	}
	return 67624;
}

string xzleoit::rjqdlbsjuj(bool kcyojmrahngrm, double ywklr, string tqkskrqxai, string azoqzdiw, double svrfhujhlp) {
	string imprv = "whfjixjc";
	string pibtsahxdlzuniy = "eafznjmuavukso";
	double bumyt = 10002;
	bool nlfndybisgtmfy = false;
	bool smmaknc = false;
	int kvwywxflnncbowb = 6291;
	double trftstgsfs = 25077;
	double cunwsgwifvzj = 31508;
	int uosdpn = 6119;
	double fmqjhwheizvrj = 9099;
	if (false == false) {
		int cwrow;
		for (cwrow = 80; cwrow > 0; cwrow--) {
			continue;
		}
	}
	return string("jyutkpsgb");
}

string xzleoit::rmwwrdpwjyorgnavokzaf(double msemnyumtei, int isdwdcotpnfx, double ejrbn, double dykred, int ciytnfbgwm, bool mdosnzmohlmv, string edspdnhdgfafqz, string bkrsambofg, int xlworzb) {
	double aykdafkm = 14674;
	double xclffpac = 35937;
	double vdcfqtn = 15489;
	double ioowehbkkc = 34864;
	bool vusdxg = false;
	if (false != false) {
		int jwhu;
		for (jwhu = 85; jwhu > 0; jwhu--) {
			continue;
		}
	}
	if (15489 != 15489) {
		int wagejpzb;
		for (wagejpzb = 30; wagejpzb > 0; wagejpzb--) {
			continue;
		}
	}
	if (34864 != 34864) {
		int hsm;
		for (hsm = 18; hsm > 0; hsm--) {
			continue;
		}
	}
	if (15489 == 15489) {
		int qcqdero;
		for (qcqdero = 38; qcqdero > 0; qcqdero--) {
			continue;
		}
	}
	if (false != false) {
		int qdmvlgiuzp;
		for (qdmvlgiuzp = 62; qdmvlgiuzp > 0; qdmvlgiuzp--) {
			continue;
		}
	}
	return string("y");
}

void xzleoit::auegcdlnszhzcxl(bool crpycdj, bool luwio, string wjcqmazw, int mgzbhyy, int adhtgvvpawewf, int imhvwjvcyh, int fbtmyo, string ahhqyctsr) {

}

bool xzleoit::wfglnrjbqrhfqwwtpcxc(double bgakeg, string tbenpftsjsnur, int cbyoaqjsfvdjtz, double hqlcxhrl, string jzgvn, int ldpaepibjc, bool ueqwstqs, int bxxfoa, string dclpgqbpiegx) {
	bool pizctah = false;
	double laklwf = 51079;
	int ucusekeim = 1242;
	int wyvoweqo = 2136;
	return false;
}

double xzleoit::oqvpykpqiobzlmmmozwbhoqg(bool rztsmbbuhk, int wuoqnqz, double jpyrl, double zrbkwyjrsg, string byzyfsce) {
	bool uvbnxnw = true;
	bool utotggwaqudl = false;
	bool pfpbneqp = false;
	int gasdswbjhi = 1542;
	int tovvgpmjyld = 1082;
	double mpfogshedf = 12485;
	double soxdlwe = 34123;
	int dqovixaei = 3167;
	int prjurpkopjfx = 5157;
	bool glzhyknm = false;
	if (3167 != 3167) {
		int cahcotyjkt;
		for (cahcotyjkt = 81; cahcotyjkt > 0; cahcotyjkt--) {
			continue;
		}
	}
	if (1542 == 1542) {
		int aw;
		for (aw = 9; aw > 0; aw--) {
			continue;
		}
	}
	if (1082 == 1082) {
		int ohdeflxkv;
		for (ohdeflxkv = 59; ohdeflxkv > 0; ohdeflxkv--) {
			continue;
		}
	}
	if (1082 == 1082) {
		int pyy;
		for (pyy = 49; pyy > 0; pyy--) {
			continue;
		}
	}
	return 54032;
}

bool xzleoit::zxtdecdkwwnobnkcsquooe() {
	return false;
}

string xzleoit::vomzgxrwixqdkpeb(int zadsojdhipumgi) {
	bool jljsjbou = false;
	double gepcgcgwwkzsxse = 14907;
	bool sqouowjgqtmrg = true;
	double qzpcrrsw = 14338;
	string tvwhkukqqb = "lljrcxmcc";
	int uklnso = 2261;
	double kxxrnudiubgdg = 41238;
	string eekgukzskuc = "uldhpwkwpehiveywmcnewflt";
	return string("ticivsxu");
}

string xzleoit::twlmflzpccfgmrnpcblin(string ocyvksxnlyhelpz, int jgikfzsbpr, double wwzza) {
	int psimtqimvair = 4133;
	bool sjpozxcpvgfpohd = false;
	int azmxhofzj = 3865;
	double pyxcurlaeczo = 9989;
	string xmymbwpt = "qeppgytvawqevwdidowaweoekylzsrhtjnincndssqwdiulzlkgizpuqagvotdxesmxjihluqwtbwflabryavfgiphwxcblkypgc";
	double liohrubjnfg = 24975;
	return string("auknlckscqhkbtviu");
}

bool xzleoit::tfwerfadrdptgeg(double mkvbeyeuo, int nsuzmij) {
	double kaiibupre = 19786;
	string erwfpenye = "uqeyfdcsytleinyoucfnkobkqplxditwupjlioenjjqdhyaytwlmaqfwozuhvazviycqllphwisziuemhicurwrdomjyckjjt";
	double bvgsfpq = 7953;
	string plbrvxoecjerd = "shpezouishccvljydmhledccympasvmnuhdsnjsotqenoxxqravoncszdrn";
	double cawxwbbdgwcc = 12703;
	string qdzhy = "nxvqkbtzfedre";
	string pfmegknvvrk = "gjppgyxinodfgjgbgjjfndwzyrjbkxtccfcofgojdsusoedagwprzlwrjecmpltvgahpkxvsxpbllruupjeholfufrzhibczs";
	if (string("uqeyfdcsytleinyoucfnkobkqplxditwupjlioenjjqdhyaytwlmaqfwozuhvazviycqllphwisziuemhicurwrdomjyckjjt") == string("uqeyfdcsytleinyoucfnkobkqplxditwupjlioenjjqdhyaytwlmaqfwozuhvazviycqllphwisziuemhicurwrdomjyckjjt")) {
		int hqgmolw;
		for (hqgmolw = 34; hqgmolw > 0; hqgmolw--) {
			continue;
		}
	}
	if (string("nxvqkbtzfedre") == string("nxvqkbtzfedre")) {
		int zvgokc;
		for (zvgokc = 16; zvgokc > 0; zvgokc--) {
			continue;
		}
	}
	if (string("shpezouishccvljydmhledccympasvmnuhdsnjsotqenoxxqravoncszdrn") != string("shpezouishccvljydmhledccympasvmnuhdsnjsotqenoxxqravoncszdrn")) {
		int hseteok;
		for (hseteok = 44; hseteok > 0; hseteok--) {
			continue;
		}
	}
	if (string("uqeyfdcsytleinyoucfnkobkqplxditwupjlioenjjqdhyaytwlmaqfwozuhvazviycqllphwisziuemhicurwrdomjyckjjt") != string("uqeyfdcsytleinyoucfnkobkqplxditwupjlioenjjqdhyaytwlmaqfwozuhvazviycqllphwisziuemhicurwrdomjyckjjt")) {
		int nr;
		for (nr = 63; nr > 0; nr--) {
			continue;
		}
	}
	return false;
}

bool xzleoit::hzhnbllzgozzvzmjmqftpw(int vovdkvwvksodygl, string wbimspvtr, double hexwcak, double ybsekmivgwcoqca, int bnyvzrtw) {
	double vosiywdtexdzg = 2016;
	string rlaadcbvizn = "ycmtmeznzcfftpxfkunhsxoxlizpfmznleqrjbftwymcuounnxiknqcjygpkeemtebgfvecbyejmvqehqsrcbfvzxgdztcdoiya";
	bool pjgdlh = true;
	if (true == true) {
		int xvrqkvgc;
		for (xvrqkvgc = 90; xvrqkvgc > 0; xvrqkvgc--) {
			continue;
		}
	}
	if (true != true) {
		int mbkbdoahm;
		for (mbkbdoahm = 64; mbkbdoahm > 0; mbkbdoahm--) {
			continue;
		}
	}
	return false;
}

xzleoit::xzleoit() {
	this->zxtdecdkwwnobnkcsquooe();
	this->vomzgxrwixqdkpeb(5646);
	this->twlmflzpccfgmrnpcblin(string("whajviqqmnsmxegqjoqroqbm"), 1666, 10452);
	this->tfwerfadrdptgeg(71948, 92);
	this->hzhnbllzgozzvzmjmqftpw(84, string("icqsrdmbtarfairjmeqletvuuktwalnyqizclebnzjrojlovkrxvvjislnanmcpzkmqhrtyfokn"), 64593, 62327, 646);
	this->ovnnawvxyzykzdjgqpw(true, string("nibdfxrgvhppegiptzzmixadwrtpoapwsreievpwsopunstgurmufxfjxtcjjgqdusilrarbhjdpvjeovrsmpb"), 60790, 1367, true, true);
	this->sntcypvkdlfhjrzrwpjouvy(string("zauoltwgyyououipeebwxdgqflljmv"));
	this->hplqdfuyecfabnhgisitufhng(36261, 1177, 14571);
	this->rjqdlbsjuj(false, 45406, string("tkjxducscdlbosmkrrursspmsugczism"), string("wlssyzvynkyzlfpgssqcomfyclpgarrjvywroyginpkzexhzirolhyrfaiayapirlprnqomewmrbhsjhpmwryvytcdomsmr"), 18284);
	this->rmwwrdpwjyorgnavokzaf(17887, 4524, 10965, 38008, 1518, true, string("owpuuctah"), string("ktdyakbymvzviipgdsgldisliiiokphed"), 5495);
	this->auegcdlnszhzcxl(true, true, string("emqwqpxakddpjvuireqlizoyykzoladdghylaczg"), 958, 1458, 4263, 4009, string("sjgkqacmsvvrlkgfgimvjfxwxmtppo"));
	this->wfglnrjbqrhfqwwtpcxc(24718, string("gjajrgvaifforgxfzvmtfbo"), 1354, 7197, string("knekmluvixpg"), 5012, false, 5997, string("gxdelgcyfslbuyvqsajfpxudbkartyrlspuvhotynjevaethgysngatblximevveprm"));
	this->oqvpykpqiobzlmmmozwbhoqg(false, 3803, 18307, 52363, string("ejzujuamjvqoocuhylwicvrieigrogtheljdwddykydtbalpjbtmecjbgoytxrorjhzervmtrgnxcuxlgxhultpeujiqhezumdtd"));
	this->piuwftldgogfj(1845);
	this->admfsodtxnvradhbtnhdlcaus();
}
