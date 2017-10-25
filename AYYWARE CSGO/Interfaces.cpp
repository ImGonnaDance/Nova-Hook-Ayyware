/*
ApocalypseCheats
*/

#include "Interfaces.h"
#include "Utilities.h"
#include "GameEventManager.h"

#include "Resolver.h"
#include "Backtrack.h"
#include "lagcomp.h"
//SDK Specific Definitions
typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
//DWORD Offsets::Modules::InputLib;
//Some globals for later
CreateInterface_t EngineFactory = NULL; // These are used to store the individual
CreateInterface_t ClientFactory = NULL; //  CreateInterface functions for each game
CreateInterface_t VGUISurfaceFactory = NULL; //  dll that we need access to. Can call
CreateInterface_t VGUI2Factory = NULL; //  them to recieve pointers to game classes.
CreateInterface_t MatFactory = NULL;
CreateInterface_t PhysFactory = NULL;
CreateInterface_t StdFactory = NULL;
CreateInterface_t InputFactory = NULL;
CreateInterface_t DataCaching = NULL;
CreateInterface_t InputSystemPointer = NULL;

void Interfaces::Initialise()
{

	//Get function pointers to the CreateInterface function of each module
	EngineFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Engine, "CreateInterface");
	ClientFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Client, "CreateInterface");
	VGUI2Factory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUI2, "CreateInterface");
	VGUISurfaceFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUISurface, "CreateInterface");
	MatFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Material, "CreateInterface");
	PhysFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VPhysics, "CreateInterface");
	StdFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Stdlib, "CreateInterface");
	InputFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::InputLib, "CreateInterface");
	DataCaching = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::DataCaches, "CreateInterface");

	//Get the interface names regardless of their version number by scanning for each string
	char* CHLClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("client.dll", "VClient0");
	char* VGUI2PanelsInterfaceName = (char*)Utilities::Memory::FindTextPattern("vgui2.dll", "VGUI_Panel0");
	char* VGUISurfaceInterfaceName = (char*)Utilities::Memory::FindTextPattern("vguimatsurface.dll", "VGUI_Surface0");
	char* EntityListInterfaceName = (char*)Utilities::Memory::FindTextPattern("client.dll", "VClientEntityList0");
	char* EngineDebugThingInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VDebugOverlay0");
	char* EngineClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineClient0");
	char* ClientPredictionInterface = (char*)Utilities::Memory::FindTextPattern("client.dll", "VClientPrediction0");
	char* MatSystemInterfaceName = (char*)Utilities::Memory::FindTextPattern("materialsystem.dll", "VMaterialSystem0");
	char* EngineRenderViewInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineRenderView0");
	char* EngineModelRenderInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineModel0");
	char* EngineModelInfoInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VModelInfoClient0");
	char* EngineTraceInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "EngineTraceClient0");
	char* PhysPropsInterfaces = (char*)Utilities::Memory::FindTextPattern("client.dll", "VPhysicsSurfaceProps0");
	char* VEngineCvarName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineCvar00");
	char* SInputSystem = (char*)Utilities::Memory::FindTextPattern("inputsystem.dll", "InputSystemVersion001");
	char* pDataCache = (char*)Utilities::Memory::FindTextPattern("datacache.dll", "MDLCache00");


	GameEventManager = (IGameEventManager2*)EngineFactory("GAMEEVENTSMANAGER002", NULL);


	MaterialSystem = reinterpret_cast<CMaterialSystem*>(Utilities::Memory::CaptureInterface(XorStr("materialsystem.dll"), XorStr("VMaterialSystem")));
	printf("0x%X\n", reinterpret_cast<DWORD>(MaterialSystem));

	// Use the factory function pointers along with the interface versions to grab
	//  pointers to the interfaces
	Client = (IBaseClientDLL*)ClientFactory(CHLClientInterfaceName, NULL);
	Engine = (IVEngineClient*)EngineFactory(EngineClientInterfaceName, NULL);
	Panels = (IPanel*)VGUI2Factory(VGUI2PanelsInterfaceName, NULL);
	Surface = (ISurface*)VGUISurfaceFactory(VGUISurfaceInterfaceName, NULL);
	EntList = (IClientEntityList*)ClientFactory(EntityListInterfaceName, NULL);
	DebugOverlay = (IVDebugOverlay*)EngineFactory(EngineDebugThingInterface, NULL);
	Prediction = (DWORD*)ClientFactory(ClientPredictionInterface, NULL);
	MaterialSystem = (CMaterialSystem*)MatFactory(MatSystemInterfaceName, NULL);
	RenderView = (CVRenderView*)EngineFactory(EngineRenderViewInterface, NULL);
	ModelRender = (IVModelRender*)EngineFactory(EngineModelRenderInterface, NULL);
	ModelInfo = (CModelInfo*)EngineFactory(EngineModelInfoInterface, NULL);
	Trace = (IEngineTrace*)EngineFactory(EngineTraceInterfaceName, NULL);
	PhysProps = (IPhysicsSurfaceProps*)PhysFactory(PhysPropsInterfaces, NULL);
	CVar = (ICVar*)StdFactory(VEngineCvarName, NULL);
	ClientMode = **(IClientModeShared***)((*(DWORD**)Interfaces::Client)[10] + 0x5);
	InputSystem = (IInputSystem*)InputFactory(SInputSystem, NULL);
	ModelCache = (IMDLCache*)DataCaching(pDataCache, NULL);
	InputSystemPointer = (CreateInterface_t)GetProcAddress((HMODULE)Utilities::Memory::WaitOnModuleHandle("inputsystem.dll"), "CreateInterface");
	InputSystem = (IInputSystem*)InputSystemPointer("InputSystemVersion001", NULL);

	// Get ClientMode Pointer
	DWORD p = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xA8\x01\x75\x1A\x83\xC8\x01\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56", "xx????????xxxxxxxx????x????x????x????xxxx????x????x");

	GameEventManager = (IGameEventManager2*)EngineFactory("GAMEEVENTSMANAGER002", NULL);
	// Search through the first entry of the Client VTable
	// The initializer contains a pointer to the 'GlobalsVariables' Table

	Globals = **(CGlobalVarsBase***)((*(DWORD**)Interfaces::Client)[0] + 0x1B);

	PDWORD pdwClientVMT = *(PDWORD*)Client;
	pInput = *(CInput**)((*(DWORD**)Client)[15] + 0x1);


	lagComp = new LagCompensation;
	lagComp->initLagRecord();

	Utilities::Log("Interfaces Ready");
}

// Namespace to contain all the valve interfaces
namespace Interfaces
{
	IBaseClientDLL* Client;
	IVEngineClient* Engine;
	IPanel* Panels;
	IClientEntityList* EntList;
	ISurface* Surface;
	IVDebugOverlay* DebugOverlay;
	IClientModeShared* ClientMode;
	CGlobalVarsBase *Globals;
	IInputSystem* InputSystem;
	DWORD *Prediction;
	CMaterialSystem* MaterialSystem;
	CVRenderView* RenderView;
	IVModelRender* ModelRender;
	CModelInfo* ModelInfo;
	IEngineTrace* Trace;
	IPhysicsSurfaceProps* PhysProps;
	ICVar *CVar;
	IGameMovement* GameMovement;
	IMoveHelper* MoveHelper;
	IPrediction* Prediction1;
	IGameEventManager2 *Interfaces::GameEventManager;
	CInput* pInput;
	IMDLCache* ModelCache;
	IInputSystem* InputSystem;
};

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class vtyinxt {
public:
	bool rtiqa;
	string expetyyuwcfk;
	int ehvse;
	vtyinxt();
	int jwopldynriyveqqwqitu(string gslvztufvuspt, double zqxiyu);
	double coivjdsowft(double vkluodgenwm);
	double lrwdtkxzvmqqbx(int swuunmoim);
	int nzdgphonomvimjfnfh(double dzsikqdhrtjutjp, bool yotwncga, bool iuwvnzdbmcepkze, string sorwhtaqyvh);
	string qbdniykkzcjbbdfkzm(int smqvxfbhwqb, bool ylukivijahg, string wnlrsherf, int xelxstadrrab, string pxckkz, bool yflycvaannl, int tioyyyoet, double nnomfqrxzer);
	void hetohkbaostljunhmyfq(string cnekjlap);
	void flrrrpyqnnatlzfcmqjei(bool votcsezt, double sfupb, bool yqikbmxsotdy, double vkmlg, bool pyynzftonkynauw, int htzjuxmivprbpc, string ylhrd);

protected:
	int pwoskul;
	int xsqwcvkhftkbzzx;
	bool lwxzpaua;
	double bjtwpkkvpqyl;

	bool ewkwzojhhueetwq(bool mhttgp, int ulwjdlndxlzq, double kyduewleegpzye, string flqvqycqrqusno, string mwzgsdhmiuehq);
	int mutoxeqtresdy(int unnevy);

private:
	double iezwkmebfxqcayg;

	string vtvcjaskcjnitvovq(string ecugocemf, string khjabvncodjcup, int oivpycakzfka, bool jnzhzzhrdke, double bulfwafmfps, bool jwhiuc, double hbhrsrvjzbj, bool fuxuxil, bool vlawxgjnlkbmew, double avufyu);
	bool xpsuueqwhn(bool vfzvmcznhqtpnl);
	void thrmhbbarynivcuhnvbip(int sqfitqjikdrkn, double vfqdpiaikikv);
	void iycqkmrbypyifsk(double muwkaspkrjgkfar, int unoemfd, bool mjyubyzruluxahr);
	double gqevbmjfsjentmbaslf(bool bmzlrcdrpdml, string sbhmsl, double zrnuqbjpsiuoggw, double uowezac, string kczxb, bool ihzyvu);
	double pgtwtgxgkwrvi(bool gosakg, double giqzagyhfjf, bool ulibseh, bool mhcxmaooju, double uzteplojsllmo, bool zkjaplmmrhudmr);

};


string vtyinxt::vtvcjaskcjnitvovq(string ecugocemf, string khjabvncodjcup, int oivpycakzfka, bool jnzhzzhrdke, double bulfwafmfps, bool jwhiuc, double hbhrsrvjzbj, bool fuxuxil, bool vlawxgjnlkbmew, double avufyu) {
	int emmhmbijmku = 228;
	int xxipzxyjrt = 3363;
	if (3363 != 3363) {
		int wmls;
		for (wmls = 21; wmls > 0; wmls--) {
			continue;
		}
	}
	if (228 == 228) {
		int yjiomitj;
		for (yjiomitj = 33; yjiomitj > 0; yjiomitj--) {
			continue;
		}
	}
	if (228 != 228) {
		int sbtdtsloc;
		for (sbtdtsloc = 79; sbtdtsloc > 0; sbtdtsloc--) {
			continue;
		}
	}
	if (228 == 228) {
		int skb;
		for (skb = 20; skb > 0; skb--) {
			continue;
		}
	}
	return string("jt");
}

bool vtyinxt::xpsuueqwhn(bool vfzvmcznhqtpnl) {
	double klnnhu = 8987;
	bool zmmuv = false;
	string dhlhhizs = "aoljnkrnhjmachikoillzdzucwofcspzdjnphbsonvrdvsdmw";
	return true;
}

void vtyinxt::thrmhbbarynivcuhnvbip(int sqfitqjikdrkn, double vfqdpiaikikv) {
	double lbnto = 30074;
	int jhwthenjc = 4459;
	double iweidcvyrqi = 38013;
	double ghvdtqocj = 33717;

}

void vtyinxt::iycqkmrbypyifsk(double muwkaspkrjgkfar, int unoemfd, bool mjyubyzruluxahr) {
	string jhmqqlo = "tnxhzhbbwuijejistufioizgmbnqdojbtqggvevvwkzatxhobizhkm";
	string uowtymxdozhmknu = "ekbskqqdunngsvs";
	double htyyolwhqol = 39385;
	double ighxfbvrj = 62890;
	int owgtq = 4656;
	string ydlqzmudjujr = "nnkvvvpycdrpxqfatsiyonmpambaxmudptxzadfnfttmqapvkzotspsouawh";
	bool bjbllczdbkhlqc = false;
	int nvthkovrzsugirw = 1170;
	bool hbgwtlgbxgxoy = false;
	string xiofprqreurk = "oikxurncbxflpqumhbpedkvinmigbneumaiuvybhxphjuhhhicjqmwupkrhxmgprajxnnyoznfshhjcdwhaegh";

}

double vtyinxt::gqevbmjfsjentmbaslf(bool bmzlrcdrpdml, string sbhmsl, double zrnuqbjpsiuoggw, double uowezac, string kczxb, bool ihzyvu) {
	string ilmayxvezbg = "qhgofbamumhxlqggqjhvmqmynozxkfxzxwsysgpfjlrsazfpwkapqzhrukxednkaqqjt";
	bool mytqvufiqebmqwf = false;
	if (false != false) {
		int qtehvf;
		for (qtehvf = 2; qtehvf > 0; qtehvf--) {
			continue;
		}
	}
	if (false != false) {
		int maqxhmhvk;
		for (maqxhmhvk = 15; maqxhmhvk > 0; maqxhmhvk--) {
			continue;
		}
	}
	return 2787;
}

double vtyinxt::pgtwtgxgkwrvi(bool gosakg, double giqzagyhfjf, bool ulibseh, bool mhcxmaooju, double uzteplojsllmo, bool zkjaplmmrhudmr) {
	return 84349;
}

bool vtyinxt::ewkwzojhhueetwq(bool mhttgp, int ulwjdlndxlzq, double kyduewleegpzye, string flqvqycqrqusno, string mwzgsdhmiuehq) {
	bool pmfmn = false;
	string lnjdatceprbhj = "klcveqlpmwvzcsgwktvjoslchpxbdblxeapglqiokpkuhekdccfxeehrzocsjwetjcvadhjqjxwj";
	string wnmtv = "ecktixvdsgucpvsmtjnqemvmnglxbiuxqcbuvbywrpnfqgvcckiuamuw";
	double pcsbjuzpcyc = 6636;
	bool eqsjgy = false;
	string jkoqmafwrpvc = "fvimiaiekfyrkmbhwlfpiyrauiocuxmnknqhyzevdbfetnetdjezbniebvezrmpopttjgvjlwnmmgdvtoqetwep";
	string ptukcqybiajl = "kfnxmbtbfwcxmbk";
	if (string("fvimiaiekfyrkmbhwlfpiyrauiocuxmnknqhyzevdbfetnetdjezbniebvezrmpopttjgvjlwnmmgdvtoqetwep") != string("fvimiaiekfyrkmbhwlfpiyrauiocuxmnknqhyzevdbfetnetdjezbniebvezrmpopttjgvjlwnmmgdvtoqetwep")) {
		int kybbiaso;
		for (kybbiaso = 66; kybbiaso > 0; kybbiaso--) {
			continue;
		}
	}
	return true;
}

int vtyinxt::mutoxeqtresdy(int unnevy) {
	int tmrzej = 3577;
	bool goblrkgdu = false;
	if (false == false) {
		int usgh;
		for (usgh = 89; usgh > 0; usgh--) {
			continue;
		}
	}
	if (3577 != 3577) {
		int bjtjpcmpv;
		for (bjtjpcmpv = 35; bjtjpcmpv > 0; bjtjpcmpv--) {
			continue;
		}
	}
	if (false == false) {
		int yvqgpjy;
		for (yvqgpjy = 12; yvqgpjy > 0; yvqgpjy--) {
			continue;
		}
	}
	if (3577 != 3577) {
		int itu;
		for (itu = 37; itu > 0; itu--) {
			continue;
		}
	}
	return 65563;
}

int vtyinxt::jwopldynriyveqqwqitu(string gslvztufvuspt, double zqxiyu) {
	double feqylwevf = 54491;
	string zwgvez = "vieukqgk";
	int tlrlsaoibnna = 2499;
	int uwvmrfzfxe = 2967;
	bool tusdrnqnc = false;
	string xxavrqjnovk = "rnoukplcaulmpgdrddvrehnqptpakeelfagnaqygwcfzuzmoqchfclyglymafhbsivuidzsolpsq";
	int bdwfrbv = 5030;
	if (2499 != 2499) {
		int ph;
		for (ph = 72; ph > 0; ph--) {
			continue;
		}
	}
	if (2499 != 2499) {
		int tgpzminomn;
		for (tgpzminomn = 9; tgpzminomn > 0; tgpzminomn--) {
			continue;
		}
	}
	if (2499 != 2499) {
		int trj;
		for (trj = 51; trj > 0; trj--) {
			continue;
		}
	}
	return 1959;
}

double vtyinxt::coivjdsowft(double vkluodgenwm) {
	string mwhnygg = "oofrvlqadtmihkxrnjwqaxddkyjnowfkeyuqfnwrubukpscjnnpvlilrxzgfuvrcqsoddpprhpw";
	int nqsjrgrrjthyp = 2632;
	int stmpd = 4786;
	double fniqhjdrt = 28245;
	bool thzipbewffk = true;
	bool kqtkf = false;
	string gdofumejv = "zwqxeynxtkomwnzscsgjayeeongzjyfhrhlvbawexceowwsjyckmonmkwxhgmznlkuqtauqehnpjmzplfhol";
	bool ydtdwmt = true;
	double lfvsz = 23196;
	string girtzclaeveos = "tecegwunbismebvwiktxqydfkbexasqtokhvdjndryryhgozhblahbsqwbjj";
	if (string("tecegwunbismebvwiktxqydfkbexasqtokhvdjndryryhgozhblahbsqwbjj") != string("tecegwunbismebvwiktxqydfkbexasqtokhvdjndryryhgozhblahbsqwbjj")) {
		int rvg;
		for (rvg = 23; rvg > 0; rvg--) {
			continue;
		}
	}
	if (2632 == 2632) {
		int jxnptf;
		for (jxnptf = 37; jxnptf > 0; jxnptf--) {
			continue;
		}
	}
	if (true == true) {
		int ndld;
		for (ndld = 82; ndld > 0; ndld--) {
			continue;
		}
	}
	if (string("oofrvlqadtmihkxrnjwqaxddkyjnowfkeyuqfnwrubukpscjnnpvlilrxzgfuvrcqsoddpprhpw") == string("oofrvlqadtmihkxrnjwqaxddkyjnowfkeyuqfnwrubukpscjnnpvlilrxzgfuvrcqsoddpprhpw")) {
		int hlyfo;
		for (hlyfo = 47; hlyfo > 0; hlyfo--) {
			continue;
		}
	}
	return 63943;
}

double vtyinxt::lrwdtkxzvmqqbx(int swuunmoim) {
	string dbxfzclzi = "pqpvclmatgytkekpfixzstveqzcjidgjciknubiqx";
	int cbhbak = 2758;
	if (2758 == 2758) {
		int plmix;
		for (plmix = 1; plmix > 0; plmix--) {
			continue;
		}
	}
	if (2758 == 2758) {
		int tb;
		for (tb = 62; tb > 0; tb--) {
			continue;
		}
	}
	if (string("pqpvclmatgytkekpfixzstveqzcjidgjciknubiqx") == string("pqpvclmatgytkekpfixzstveqzcjidgjciknubiqx")) {
		int umzpdoi;
		for (umzpdoi = 78; umzpdoi > 0; umzpdoi--) {
			continue;
		}
	}
	if (string("pqpvclmatgytkekpfixzstveqzcjidgjciknubiqx") == string("pqpvclmatgytkekpfixzstveqzcjidgjciknubiqx")) {
		int qnvnbyat;
		for (qnvnbyat = 85; qnvnbyat > 0; qnvnbyat--) {
			continue;
		}
	}
	if (string("pqpvclmatgytkekpfixzstveqzcjidgjciknubiqx") != string("pqpvclmatgytkekpfixzstveqzcjidgjciknubiqx")) {
		int ko;
		for (ko = 49; ko > 0; ko--) {
			continue;
		}
	}
	return 45347;
}

int vtyinxt::nzdgphonomvimjfnfh(double dzsikqdhrtjutjp, bool yotwncga, bool iuwvnzdbmcepkze, string sorwhtaqyvh) {
	double ynbujeybgfgwb = 34807;
	double tgjxhyccnxzheiq = 9738;
	double mtzing = 1816;
	bool qaakf = true;
	string gdklpvbqmzz = "qcidlpgwjgenagxyhgmpgpbzumssqwbkanlupciaqilblbjohdsogmjsotwujhof";
	return 59037;
}

string vtyinxt::qbdniykkzcjbbdfkzm(int smqvxfbhwqb, bool ylukivijahg, string wnlrsherf, int xelxstadrrab, string pxckkz, bool yflycvaannl, int tioyyyoet, double nnomfqrxzer) {
	int yhmdltfmkz = 2014;
	bool tusrejkffa = false;
	if (2014 == 2014) {
		int aafbughd;
		for (aafbughd = 57; aafbughd > 0; aafbughd--) {
			continue;
		}
	}
	return string("a");
}

void vtyinxt::hetohkbaostljunhmyfq(string cnekjlap) {
	double eotohrhysqrog = 27939;
	double mcilqhtv = 50689;
	double bajinfymsua = 17098;
	int qarbpwopsddijw = 6446;
	bool jdbpwca = false;
	bool etmevobhfep = false;
	int nngxmkvztnc = 363;
	double oyhgvghn = 82163;
	if (false == false) {
		int zpm;
		for (zpm = 67; zpm > 0; zpm--) {
			continue;
		}
	}
	if (27939 != 27939) {
		int whhyicaz;
		for (whhyicaz = 82; whhyicaz > 0; whhyicaz--) {
			continue;
		}
	}
	if (27939 != 27939) {
		int pjeh;
		for (pjeh = 45; pjeh > 0; pjeh--) {
			continue;
		}
	}

}

void vtyinxt::flrrrpyqnnatlzfcmqjei(bool votcsezt, double sfupb, bool yqikbmxsotdy, double vkmlg, bool pyynzftonkynauw, int htzjuxmivprbpc, string ylhrd) {

}

vtyinxt::vtyinxt() {
	this->jwopldynriyveqqwqitu(string("nleysbkboyazahthliqyweacxrvfyyetvcvlciybgfyzcjqvpzgvtheefvovemvynk"), 25610);
	this->coivjdsowft(70866);
	this->lrwdtkxzvmqqbx(301);
	this->nzdgphonomvimjfnfh(13137, false, false, string("yjuprkwcumtqwaimbvhwtgbgftgfmbjjqsnuhzlhbyljgqbitlackgunmuavnbyqraudxtavfoel"));
	this->qbdniykkzcjbbdfkzm(147, false, string("mguwtuxfkersivlnlehpuxhvwvaxrzlqnowldd"), 92, string("uwzkcoltbbvdjqgalywmkipgktuizbkejepqesss"), false, 913, 55538);
	this->hetohkbaostljunhmyfq(string("wcmjiuxlbvufspocflmpalzrhgyxusokmpxvk"));
	this->flrrrpyqnnatlzfcmqjei(true, 2107, false, 8585, true, 1219, string("xrc"));
	this->ewkwzojhhueetwq(false, 6142, 283, string("mkbgkokjjcxrwzvroutlzndswcfbwlrzxinqqeysfebiqtasawgrtcrrjg"), string("avzgluyaqvdbpjbhomjvkjqdzwvzcrlwfwlknnlhyeffnhzjckzyvdokeazgyramgfumorpguwayfpbo"));
	this->mutoxeqtresdy(2169);
	this->vtvcjaskcjnitvovq(string("fjzadrxhecmkpwbceaau"), string("wagpvrjsolvcghgdhznoaecokmyajykiv"), 450, true, 6608, true, 9150, true, true, 86151);
	this->xpsuueqwhn(true);
	this->thrmhbbarynivcuhnvbip(6327, 19945);
	this->iycqkmrbypyifsk(53633, 410, false);
	this->gqevbmjfsjentmbaslf(false, string("ydalpsdeyfymzadenajrrtsursqznrrqiwqfrufahw"), 37765, 30960, string("z"), false);
	this->pgtwtgxgkwrvi(false, 3875, false, true, 26079, true);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class vkjjkmz {
public:
	bool kmiuqlu;
	int jlyssflwxv;
	string lxzwj;
	vkjjkmz();
	bool xtqjjsikxclgskm(int kgulxvzzs, double mbrhpwhsthtyp, double dtnxmq, int xeeoovmx);
	double acsnwcdvqurpb(int pahwri, int wggruhthstgimj, int mfdytwrnmysc, int yovkwzuvuqtt, double xszjl, int mxxbxmyiiexltbe, bool crmujgjic);
	string fugroporplfwmfxrdctsv(string lwkthx);
	string syjuwhtrtpvsvdlyn(double vnkghdc, bool zdeqmsrfrava);
	void wdbicwynwafawaklquipyjaci(bool kleufqe, double yptymlfebnpukft, string fukqohmgvbl, int kmnqswhdrjua);
	void dvsyqokvzljyufyuzvpm(bool alpzl, int iqdjiv, int epaiciv, bool tlasbmiaszpo, double hyztqfxapjnwlf, string ctltqhjhvyn, double zvjtcertnfh, bool pwahovqy);
	void kxeewspypmzhipmcphpqnau(int xrdyhg, string nqwuvpcrvhg, bool gesbbkwnqvtrhc, int oiwrcxqvbz, bool eglurh);

protected:
	bool ythruvrc;
	string tanpqrajoowysom;
	double lbjtr;

	int venbheakqmthpdabasa(bool asjraidyzdjrs, bool ithzwlzrgmoz, int uivnvyrtby, double uvjlrgqfc);
	double wslzldihvq(bool ufgufovwbigqxdg, int zqxiprhsp, double ubqjcbwoj, double dtmkojgfocdm, double mdxfafmj, int ubdzlvwtf, double ozwwsnonb, int jwxqckolfiksyi);
	bool aslbqvvywml(int xyotcenjeofwlhu, string lmjobsxnxt, int eieja, int bqqniyjnhj, string gzotngrpm, int mnvbprxmrykh, bool wjers);
	int aexqpxvalne(string fbemyrzbh, double xzmhjlldyg, int fgnkiolw, int gfcngquhnupon);
	string lklaijrebifhzqhbpvfkdvouy(double wjyrlqyopags, bool tvanp, string lkaildsagrifw, double lvuhwelbk);
	void zwtzbwgntbwisxmvooc(int uzfjrtgvcpqxr, string cttgv, double rojnipxctiohgn);
	string xsqrdroxvdymszaehyru();
	string opkssunzhxetrqqnoadt(double schusgfrpktgb, int neqynzmj, int wvcntpy, int chpuiby, bool iughmzarikvl, double birzvd, string lxebmivn, int twzsmxvsqwsgx, double ljpqumeyw, string vwkxpunkna);

private:
	bool lxfckdfzkjifzbu;
	string cbhixrnvi;
	string xcpkho;
	int obsfoowobjyt;
	bool vvedgkcrnd;

	double lqewvduymzfudnkxhpxefqdq(string riqaobvwparldt, double rzbexgwo, double oddtmdq, bool qqwiqpibxjniuy, int dwwgp, bool kadyeztgc, double criksfcmfdnofn);
	bool ucvorfxrvuw(double odwbkjnpll, bool aorczbeju, string qfcgwy, double pcvdjm, string ruvcztpe, bool zwlvvfz, string ndzqfwwvdvblnzp, double blildlswf, int fcvmwhezho, int sfkcqccdwfk);
	double esvwxvicucspjdq(double uaqiqf, double iupccgkyfjrtwvg, double ckxlbojryz, string ishbzi, string rrxnpjabilgsn, double ajwfclyxd, string ymaqjiu);

};


double vkjjkmz::lqewvduymzfudnkxhpxefqdq(string riqaobvwparldt, double rzbexgwo, double oddtmdq, bool qqwiqpibxjniuy, int dwwgp, bool kadyeztgc, double criksfcmfdnofn) {
	string dmuqlq = "xqplniulbcrpah";
	int ddgcknjrqid = 5404;
	double beueegqsnzkw = 8829;
	string clhifnv = "dljcenqkmzizapstkictoqtgkgtjcgftgacqdzkltfefppiawuxciaf";
	bool fzvawqyqm = false;
	bool kzpxwxntuwogs = false;
	double jicduhfmladup = 19104;
	double tnhnhpcadstgn = 38446;
	string nnrzn = "rnryimqotodhfpazjkrvtwfowwzujvwhatesbrwbchjikyaowswduamaxxbesicrygiagmbcbrmrlbquyqfwfu";
	double tmpbc = 22209;
	if (string("rnryimqotodhfpazjkrvtwfowwzujvwhatesbrwbchjikyaowswduamaxxbesicrygiagmbcbrmrlbquyqfwfu") != string("rnryimqotodhfpazjkrvtwfowwzujvwhatesbrwbchjikyaowswduamaxxbesicrygiagmbcbrmrlbquyqfwfu")) {
		int rg;
		for (rg = 16; rg > 0; rg--) {
			continue;
		}
	}
	return 66119;
}

bool vkjjkmz::ucvorfxrvuw(double odwbkjnpll, bool aorczbeju, string qfcgwy, double pcvdjm, string ruvcztpe, bool zwlvvfz, string ndzqfwwvdvblnzp, double blildlswf, int fcvmwhezho, int sfkcqccdwfk) {
	string eshgox = "rsryjpnvlignfxyqtl";
	string wsypienqcv = "ido";
	if (string("ido") == string("ido")) {
		int icnuv;
		for (icnuv = 59; icnuv > 0; icnuv--) {
			continue;
		}
	}
	if (string("ido") == string("ido")) {
		int gzveznip;
		for (gzveznip = 10; gzveznip > 0; gzveznip--) {
			continue;
		}
	}
	if (string("ido") != string("ido")) {
		int lvvkngj;
		for (lvvkngj = 3; lvvkngj > 0; lvvkngj--) {
			continue;
		}
	}
	if (string("rsryjpnvlignfxyqtl") == string("rsryjpnvlignfxyqtl")) {
		int vnmnu;
		for (vnmnu = 49; vnmnu > 0; vnmnu--) {
			continue;
		}
	}
	if (string("rsryjpnvlignfxyqtl") == string("rsryjpnvlignfxyqtl")) {
		int nzbufow;
		for (nzbufow = 14; nzbufow > 0; nzbufow--) {
			continue;
		}
	}
	return false;
}

double vkjjkmz::esvwxvicucspjdq(double uaqiqf, double iupccgkyfjrtwvg, double ckxlbojryz, string ishbzi, string rrxnpjabilgsn, double ajwfclyxd, string ymaqjiu) {
	string ndboaomdhtcy = "rydzjzzhpgflrnehmtmtoflpqjfqbesdteukbnrbevanznurtjxbgfqzblkcqhctpifkhqlrbdnptgjvgyaafzmdihk";
	int ivxpnhcqdpqv = 1758;
	int iijgtokm = 1886;
	string txlkoaujfdw = "jsmgndyoicbqeeiybkdqkmlaucb";
	bool dcwns = false;
	if (1886 != 1886) {
		int zpxfgs;
		for (zpxfgs = 59; zpxfgs > 0; zpxfgs--) {
			continue;
		}
	}
	if (string("rydzjzzhpgflrnehmtmtoflpqjfqbesdteukbnrbevanznurtjxbgfqzblkcqhctpifkhqlrbdnptgjvgyaafzmdihk") != string("rydzjzzhpgflrnehmtmtoflpqjfqbesdteukbnrbevanznurtjxbgfqzblkcqhctpifkhqlrbdnptgjvgyaafzmdihk")) {
		int uklfghiubx;
		for (uklfghiubx = 8; uklfghiubx > 0; uklfghiubx--) {
			continue;
		}
	}
	if (1886 == 1886) {
		int twzavu;
		for (twzavu = 75; twzavu > 0; twzavu--) {
			continue;
		}
	}
	if (string("jsmgndyoicbqeeiybkdqkmlaucb") == string("jsmgndyoicbqeeiybkdqkmlaucb")) {
		int imntgpuzx;
		for (imntgpuzx = 14; imntgpuzx > 0; imntgpuzx--) {
			continue;
		}
	}
	return 51879;
}

int vkjjkmz::venbheakqmthpdabasa(bool asjraidyzdjrs, bool ithzwlzrgmoz, int uivnvyrtby, double uvjlrgqfc) {
	int aboirpu = 2738;
	int qkqmqai = 916;
	bool btmlr = false;
	int avlajstkc = 144;
	string sbrlhxcbywqo = "ulyjojzeadupyelyletaxosslldqzpxjigvakwmhqhdktvosqrrsascxckittfjaaabwrhbyjwvedqf";
	double wihnhufpiwl = 20611;
	string agsenm = "vsflwnbroipgkximotcliyxmagstyvfzgqupvtmfodzazfablehsnyeaxdgkchnuvyhodksqnhrlpqbjyvmfmv";
	if (144 != 144) {
		int pnnfwvhxlb;
		for (pnnfwvhxlb = 4; pnnfwvhxlb > 0; pnnfwvhxlb--) {
			continue;
		}
	}
	if (2738 != 2738) {
		int mvubj;
		for (mvubj = 12; mvubj > 0; mvubj--) {
			continue;
		}
	}
	if (916 != 916) {
		int ud;
		for (ud = 50; ud > 0; ud--) {
			continue;
		}
	}
	if (string("ulyjojzeadupyelyletaxosslldqzpxjigvakwmhqhdktvosqrrsascxckittfjaaabwrhbyjwvedqf") == string("ulyjojzeadupyelyletaxosslldqzpxjigvakwmhqhdktvosqrrsascxckittfjaaabwrhbyjwvedqf")) {
		int xagm;
		for (xagm = 61; xagm > 0; xagm--) {
			continue;
		}
	}
	return 66792;
}

double vkjjkmz::wslzldihvq(bool ufgufovwbigqxdg, int zqxiprhsp, double ubqjcbwoj, double dtmkojgfocdm, double mdxfafmj, int ubdzlvwtf, double ozwwsnonb, int jwxqckolfiksyi) {
	int zpbzrlyvtced = 3622;
	string bwlzbwygfyzhk = "lggsnzshunsuvuwrgvvfwjmtulfoteendoefjrihqo";
	if (string("lggsnzshunsuvuwrgvvfwjmtulfoteendoefjrihqo") != string("lggsnzshunsuvuwrgvvfwjmtulfoteendoefjrihqo")) {
		int omp;
		for (omp = 45; omp > 0; omp--) {
			continue;
		}
	}
	if (string("lggsnzshunsuvuwrgvvfwjmtulfoteendoefjrihqo") != string("lggsnzshunsuvuwrgvvfwjmtulfoteendoefjrihqo")) {
		int un;
		for (un = 6; un > 0; un--) {
			continue;
		}
	}
	if (string("lggsnzshunsuvuwrgvvfwjmtulfoteendoefjrihqo") != string("lggsnzshunsuvuwrgvvfwjmtulfoteendoefjrihqo")) {
		int ijawrl;
		for (ijawrl = 59; ijawrl > 0; ijawrl--) {
			continue;
		}
	}
	if (3622 == 3622) {
		int kzmxxlj;
		for (kzmxxlj = 84; kzmxxlj > 0; kzmxxlj--) {
			continue;
		}
	}
	return 21307;
}

bool vkjjkmz::aslbqvvywml(int xyotcenjeofwlhu, string lmjobsxnxt, int eieja, int bqqniyjnhj, string gzotngrpm, int mnvbprxmrykh, bool wjers) {
	double xpprzhn = 41988;
	if (41988 == 41988) {
		int nasslx;
		for (nasslx = 38; nasslx > 0; nasslx--) {
			continue;
		}
	}
	if (41988 != 41988) {
		int gfhiykvic;
		for (gfhiykvic = 6; gfhiykvic > 0; gfhiykvic--) {
			continue;
		}
	}
	if (41988 == 41988) {
		int nytnylk;
		for (nytnylk = 50; nytnylk > 0; nytnylk--) {
			continue;
		}
	}
	if (41988 != 41988) {
		int fqkgvlsvr;
		for (fqkgvlsvr = 91; fqkgvlsvr > 0; fqkgvlsvr--) {
			continue;
		}
	}
	if (41988 != 41988) {
		int gj;
		for (gj = 81; gj > 0; gj--) {
			continue;
		}
	}
	return false;
}

int vkjjkmz::aexqpxvalne(string fbemyrzbh, double xzmhjlldyg, int fgnkiolw, int gfcngquhnupon) {
	int ifcdtpnqleyfgrl = 5538;
	double ihaxjmqfak = 44710;
	double smjekkwusqon = 21828;
	double fykwhsfn = 21455;
	bool ykbbviuttatlo = false;
	int buorr = 2077;
	double gjcshk = 2102;
	if (false == false) {
		int bpsqqh;
		for (bpsqqh = 30; bpsqqh > 0; bpsqqh--) {
			continue;
		}
	}
	if (2077 != 2077) {
		int cokypnau;
		for (cokypnau = 64; cokypnau > 0; cokypnau--) {
			continue;
		}
	}
	if (2077 == 2077) {
		int ymwlmpgqwm;
		for (ymwlmpgqwm = 60; ymwlmpgqwm > 0; ymwlmpgqwm--) {
			continue;
		}
	}
	if (21455 != 21455) {
		int btgxyo;
		for (btgxyo = 60; btgxyo > 0; btgxyo--) {
			continue;
		}
	}
	return 12628;
}

string vkjjkmz::lklaijrebifhzqhbpvfkdvouy(double wjyrlqyopags, bool tvanp, string lkaildsagrifw, double lvuhwelbk) {
	string jvylivmhox = "hlzfachyxmmopdpforjpeqqerorfmljvzadcalggfpjidovdqhiiivsnzommvrjfrduxmrrvvxfpsrlpfqxezhepjkg";
	string lfjpbsvmtpmjc = "igeqakfzzcrsseskofsofjfkubmnajkixlnzupggnbyecnsskypnjwdhanqibhhxbhoaoil";
	string xroqncnnwbrc = "ccynhcwciggjfrofmttzppzyptkbapghlnhominlputxqsaawzpqsqnkypnhgfp";
	bool jbluzcvftxnz = true;
	string lvqwbpulo = "mobuqegnecmjpsmrpskntpyhbjuyqwlslgtazayltrlvnjpthqbnxzehhthcyunelvyalmnkmpkknuzhshgetayn";
	if (string("igeqakfzzcrsseskofsofjfkubmnajkixlnzupggnbyecnsskypnjwdhanqibhhxbhoaoil") == string("igeqakfzzcrsseskofsofjfkubmnajkixlnzupggnbyecnsskypnjwdhanqibhhxbhoaoil")) {
		int buukkdnk;
		for (buukkdnk = 20; buukkdnk > 0; buukkdnk--) {
			continue;
		}
	}
	if (string("ccynhcwciggjfrofmttzppzyptkbapghlnhominlputxqsaawzpqsqnkypnhgfp") != string("ccynhcwciggjfrofmttzppzyptkbapghlnhominlputxqsaawzpqsqnkypnhgfp")) {
		int ytypfbwhq;
		for (ytypfbwhq = 21; ytypfbwhq > 0; ytypfbwhq--) {
			continue;
		}
	}
	if (string("ccynhcwciggjfrofmttzppzyptkbapghlnhominlputxqsaawzpqsqnkypnhgfp") != string("ccynhcwciggjfrofmttzppzyptkbapghlnhominlputxqsaawzpqsqnkypnhgfp")) {
		int yplpp;
		for (yplpp = 10; yplpp > 0; yplpp--) {
			continue;
		}
	}
	if (string("hlzfachyxmmopdpforjpeqqerorfmljvzadcalggfpjidovdqhiiivsnzommvrjfrduxmrrvvxfpsrlpfqxezhepjkg") == string("hlzfachyxmmopdpforjpeqqerorfmljvzadcalggfpjidovdqhiiivsnzommvrjfrduxmrrvvxfpsrlpfqxezhepjkg")) {
		int secakxai;
		for (secakxai = 36; secakxai > 0; secakxai--) {
			continue;
		}
	}
	if (string("ccynhcwciggjfrofmttzppzyptkbapghlnhominlputxqsaawzpqsqnkypnhgfp") != string("ccynhcwciggjfrofmttzppzyptkbapghlnhominlputxqsaawzpqsqnkypnhgfp")) {
		int dkvv;
		for (dkvv = 35; dkvv > 0; dkvv--) {
			continue;
		}
	}
	return string("vfcmcymknogtlz");
}

void vkjjkmz::zwtzbwgntbwisxmvooc(int uzfjrtgvcpqxr, string cttgv, double rojnipxctiohgn) {
	double gmizj = 70162;
	int rnqdfgxum = 1056;
	bool pwqkrmevo = true;
	int bqqonmle = 1390;
	bool pfsnzoccr = false;
	bool wjrqvp = true;
	bool snkiszmppkjope = true;
	if (false == false) {
		int tquwdg;
		for (tquwdg = 2; tquwdg > 0; tquwdg--) {
			continue;
		}
	}
	if (1056 != 1056) {
		int erkzaimhq;
		for (erkzaimhq = 74; erkzaimhq > 0; erkzaimhq--) {
			continue;
		}
	}
	if (true == true) {
		int exe;
		for (exe = 54; exe > 0; exe--) {
			continue;
		}
	}

}

string vkjjkmz::xsqrdroxvdymszaehyru() {
	return string("evakibp");
}

string vkjjkmz::opkssunzhxetrqqnoadt(double schusgfrpktgb, int neqynzmj, int wvcntpy, int chpuiby, bool iughmzarikvl, double birzvd, string lxebmivn, int twzsmxvsqwsgx, double ljpqumeyw, string vwkxpunkna) {
	double xxmdt = 3226;
	bool vjulb = true;
	int kezefplkecqrzp = 2745;
	bool llqpuvxbjywlsy = true;
	bool wxuoj = true;
	double ipucogpdcofjize = 7031;
	return string("sfmy");
}

bool vkjjkmz::xtqjjsikxclgskm(int kgulxvzzs, double mbrhpwhsthtyp, double dtnxmq, int xeeoovmx) {
	string llhkwbyuemrz = "mtxitfukoblpjcwroljjfhqmxuqbxtisrpxbwlfodycegv";
	int nevxfiwmvocj = 7877;
	if (string("mtxitfukoblpjcwroljjfhqmxuqbxtisrpxbwlfodycegv") == string("mtxitfukoblpjcwroljjfhqmxuqbxtisrpxbwlfodycegv")) {
		int llbqnputx;
		for (llbqnputx = 45; llbqnputx > 0; llbqnputx--) {
			continue;
		}
	}
	if (string("mtxitfukoblpjcwroljjfhqmxuqbxtisrpxbwlfodycegv") != string("mtxitfukoblpjcwroljjfhqmxuqbxtisrpxbwlfodycegv")) {
		int fxqbv;
		for (fxqbv = 82; fxqbv > 0; fxqbv--) {
			continue;
		}
	}
	if (string("mtxitfukoblpjcwroljjfhqmxuqbxtisrpxbwlfodycegv") == string("mtxitfukoblpjcwroljjfhqmxuqbxtisrpxbwlfodycegv")) {
		int aurymnue;
		for (aurymnue = 43; aurymnue > 0; aurymnue--) {
			continue;
		}
	}
	if (7877 != 7877) {
		int ohjy;
		for (ohjy = 78; ohjy > 0; ohjy--) {
			continue;
		}
	}
	return true;
}

double vkjjkmz::acsnwcdvqurpb(int pahwri, int wggruhthstgimj, int mfdytwrnmysc, int yovkwzuvuqtt, double xszjl, int mxxbxmyiiexltbe, bool crmujgjic) {
	string smgjpsprazxi = "cupugivnejirgurbghzk";
	bool nwfnxasuap = false;
	bool zzkgupxnpeu = false;
	if (string("cupugivnejirgurbghzk") == string("cupugivnejirgurbghzk")) {
		int rzp;
		for (rzp = 63; rzp > 0; rzp--) {
			continue;
		}
	}
	if (false == false) {
		int mc;
		for (mc = 98; mc > 0; mc--) {
			continue;
		}
	}
	return 80059;
}

string vkjjkmz::fugroporplfwmfxrdctsv(string lwkthx) {
	int dhmkrihj = 1674;
	string nphxclkl = "gwwlrcnsaxvltuqiavfd";
	return string("vdnlmfco");
}

string vkjjkmz::syjuwhtrtpvsvdlyn(double vnkghdc, bool zdeqmsrfrava) {
	string wkuhyvxbsqru = "ksowqzndyzvizlqfyntbrxwqhnobghcejoexcbhhjoddxwtpkggpkgeeawxuwgmjbinhlmqicuzolj";
	string lthdvw = "ulwgqchpbhqmacpcpthuoyllgjnosascqc";
	double ctucysmfo = 13471;
	bool rppgvefcnkglp = true;
	if (string("ulwgqchpbhqmacpcpthuoyllgjnosascqc") != string("ulwgqchpbhqmacpcpthuoyllgjnosascqc")) {
		int rdksm;
		for (rdksm = 58; rdksm > 0; rdksm--) {
			continue;
		}
	}
	if (string("ksowqzndyzvizlqfyntbrxwqhnobghcejoexcbhhjoddxwtpkggpkgeeawxuwgmjbinhlmqicuzolj") != string("ksowqzndyzvizlqfyntbrxwqhnobghcejoexcbhhjoddxwtpkggpkgeeawxuwgmjbinhlmqicuzolj")) {
		int ata;
		for (ata = 21; ata > 0; ata--) {
			continue;
		}
	}
	return string("wluexjedisjawmcrr");
}

void vkjjkmz::wdbicwynwafawaklquipyjaci(bool kleufqe, double yptymlfebnpukft, string fukqohmgvbl, int kmnqswhdrjua) {
	string smlawttyj = "hirrlpgdxcp";
	int cubqdzamio = 7570;
	bool kswbvmrjf = true;
	string terfxbo = "zfdlpdoplw";
	int zopvmznaq = 4744;
	string ghpvjiaofnlbewo = "fwrfjzplx";
	double ttprn = 36352;
	int iudus = 750;
	string gcirkdhlufyi = "zcajlcefihbujqtimgysvuvukdwutnwnxwwjdpvahwcnznkijyzshjctbuvnebrilvbykpqgx";
	if (string("hirrlpgdxcp") != string("hirrlpgdxcp")) {
		int ibxoyrer;
		for (ibxoyrer = 27; ibxoyrer > 0; ibxoyrer--) {
			continue;
		}
	}

}

void vkjjkmz::dvsyqokvzljyufyuzvpm(bool alpzl, int iqdjiv, int epaiciv, bool tlasbmiaszpo, double hyztqfxapjnwlf, string ctltqhjhvyn, double zvjtcertnfh, bool pwahovqy) {
	int fqnbxmmm = 4529;
	int jjqxd = 1885;
	double exxsew = 9728;
	bool fsbznnh = false;
	string ottwzvwc = "swcvxoimairi";
	int ksnnofdf = 2199;
	int kuetzhsy = 1262;
	bool jxwguukapjdqqyl = false;
	if (false == false) {
		int ygcs;
		for (ygcs = 5; ygcs > 0; ygcs--) {
			continue;
		}
	}
	if (false != false) {
		int widncg;
		for (widncg = 68; widncg > 0; widncg--) {
			continue;
		}
	}
	if (4529 == 4529) {
		int eggjj;
		for (eggjj = 38; eggjj > 0; eggjj--) {
			continue;
		}
	}

}

void vkjjkmz::kxeewspypmzhipmcphpqnau(int xrdyhg, string nqwuvpcrvhg, bool gesbbkwnqvtrhc, int oiwrcxqvbz, bool eglurh) {
	string ttlrzfw = "xkspyuoiopbntwntkifw";
	double bdaqkqvf = 4866;
	int ezzjksr = 1028;
	if (1028 != 1028) {
		int sifee;
		for (sifee = 59; sifee > 0; sifee--) {
			continue;
		}
	}

}

vkjjkmz::vkjjkmz() {
	this->xtqjjsikxclgskm(1020, 5099, 30277, 7010);
	this->acsnwcdvqurpb(64, 960, 2937, 6478, 27053, 7900, false);
	this->fugroporplfwmfxrdctsv(string("oosvfjzuvqxqxaujgpuafertpcglngvdmpjugnidrpufqxyjjdaxfoolnsurpaqqnymt"));
	this->syjuwhtrtpvsvdlyn(2947, false);
	this->wdbicwynwafawaklquipyjaci(false, 11331, string("gbneofipcurfpgtclklyk"), 1070);
	this->dvsyqokvzljyufyuzvpm(false, 281, 52, false, 32746, string("iuadmsrjdvxexofjitjgzjqigefiqczudozvtxutfpwatedslgnhytcrptxwbomaxgu"), 2184, true);
	this->kxeewspypmzhipmcphpqnau(2013, string("eolgtabirknzwxvmxloexpigchqzempnleyyqwmplbeerbnjpaeykbbykgllh"), false, 2932, false);
	this->venbheakqmthpdabasa(true, true, 1721, 32729);
	this->wslzldihvq(true, 2130, 28937, 17312, 51383, 3844, 5788, 8938);
	this->aslbqvvywml(2488, string("ahyennneyzvvnigvisykswdrrxojyiigorpttwdcbpdpsfbfskfyicxbddqustssbhsmkpk"), 2702, 6021, string("jdsaswlzjfbqkycuexsfhsfoiuexpootobdmcfbdwmhpoiuryyaoqywvehjuwlmxhiqtpngmoexgvfwjrieaokqglnqrqsymhi"), 1289, false);
	this->aexqpxvalne(string("fjmbxxw"), 5010, 107, 6671);
	this->lklaijrebifhzqhbpvfkdvouy(51977, true, string("jhjktbyvlrdjcfedmbwomdbqzggpsyhmhcysjlpkbjmpiqtfixrivgjagouuqrdfulslawogcpk"), 4841);
	this->zwtzbwgntbwisxmvooc(835, string("qnymsqiasqgkwapzypgmyifixrfbkiaxcvbhfysujaiuem"), 5131);
	this->xsqrdroxvdymszaehyru();
	this->opkssunzhxetrqqnoadt(15047, 148, 477, 1135, true, 43580, string("aivkyjubysjsfkjqfaxambliwnoqzfxquuswzmzsxmhncim"), 5654, 8543, string("fkqahvqjamzqqpiwzhbdewyivayrefngbgvrcufhhmtbgidvqlsmnqmyjwifisesojkyadbtpvpijxxmbrqx"));
	this->lqewvduymzfudnkxhpxefqdq(string("wxisyozfpkfgzidvrwcqbeolvvdvpmgzejlvcglcmkdclsganjczbeimebulwnpgeaxtbcspvrydgxuindzhicz"), 60214, 13204, false, 1112, false, 30322);
	this->ucvorfxrvuw(12363, false, string("pujlppvlptxvebpvcznmgpcddfxynvlrhisvtzkemzzsbdcxgflywekrpdnruihtwypcufhsuyoorhiulnstcnsniudtjbhcgw"), 12711, string("cehlhuzelbrsbbmvwasqzekkeqheaunqmssrd"), false, string("ibdigxrzceuxnxqswqrkfnbbboryzkhuqwjcnnnpanewsowibkto"), 9961, 291, 1380);
	this->esvwxvicucspjdq(62047, 64342, 58138, string("pbiqqywgeewropnuqkogqvwqylkfzphgotntlcplyu"), string("vbwblgiiyfqurshrbiotdpnatpmpcd"), 74330, string("uzdsw"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class lgtilht {
public:
	int jhxjoillqdghzcc;
	bool sohxhigfhzzt;
	lgtilht();
	double kdzijtvlegtja(double kftnfzsgjoa, string impcjgmbeufcjn, string rzgtdwidhpau, string lnytvnzxhe, double qjyrwbjzzkxslvt, int amrlrjmxdyijh);
	void oehvqaazuh(double slapnjgwqcr, double tmfutyzntnilsin, int swmqzbqibduxr, string tpxbolytegqyyyp, double xsyrd, double szhnobjbtessf);
	void orzzczafceovdcuo(int fdcqiccfd, string dgxzyobmlwig, double gombnbjcjianyk, int gfvyvblniapkhfy, bool gkwjxtfl, bool eflqz);

protected:
	bool onxrvmcfkne;
	bool reqruqjs;
	bool nfvsaxbvrwj;

	string macdjrothk();
	void yuenxgsibwcnopbhvl(double scvqxkjsgy, string btuwvhwj, int xasrrxamcxcjr, string nwhhgdodvn, bool cagyrjamtu, int lxahqvr, string odgrkqaxpn, string mllgmpoztljcbb);
	double pbioivneedxzhpxhgfhhg(string whkbvjk, string nchlapdfkys, string byzogrz, double jdyxgfqbfoqjot, bool jvmiy, bool ulglq, int fwjvmgfylfn, double mluokdscdkn, string hiadmn);
	int msounbvtynhrnoepjcvsk(bool jpfidnuusyo, int detjzois);
	int rqftevyfrgrllnnrtt(int csplffiv, double qmirjoivww, double vhbbjpyvzmwgn, bool sfoix, int dhkwusfn);
	int lcctlfmexpfumyvvmgr(double vsoridiwxhh);
	bool lgwsmnbtuclcdfkohx(int jvqldfwc, string cgjavyrgqa, double qjtsffrkg, int vffxbxiovrac, bool ltbxhnnm, string nmwufkex, int shifoaz, double fgwlnzev, string uharuxeojr);

private:
	string oasymroztqwl;
	int qkhmbd;
	bool wxcdtwtz;
	int aspvysnpnq;

	int xgbutehgxozmqlqvbqguuujwe(bool ucoeh, int wyjlifunzurt, int ozqjhg, bool uiruztlykdte, int nymzhtxbbtkae, int oxsybl);
	bool mhsgrexqoxxqkfjfrqcaoaml(bool rsyszqhghieupy, bool paenvgsrhutxycj, int pilbnaux, double tvzesrgqzpgrk, double glpfbfarfrquf, bool fengoedeawuq, bool yqpzqasgse);

};


int lgtilht::xgbutehgxozmqlqvbqguuujwe(bool ucoeh, int wyjlifunzurt, int ozqjhg, bool uiruztlykdte, int nymzhtxbbtkae, int oxsybl) {
	int jdchkcxyvmwfp = 5974;
	bool pdoitdemvoh = true;
	string ysjnurnqybqftni = "fwigunsmkvbpozkpeksjkybuilliacszvazobqpzlpqjzddhr";
	if (true == true) {
		int gzgvrd;
		for (gzgvrd = 16; gzgvrd > 0; gzgvrd--) {
			continue;
		}
	}
	if (true == true) {
		int cdi;
		for (cdi = 54; cdi > 0; cdi--) {
			continue;
		}
	}
	if (true != true) {
		int fkdclfcvkg;
		for (fkdclfcvkg = 23; fkdclfcvkg > 0; fkdclfcvkg--) {
			continue;
		}
	}
	if (5974 == 5974) {
		int islxse;
		for (islxse = 14; islxse > 0; islxse--) {
			continue;
		}
	}
	return 92668;
}

bool lgtilht::mhsgrexqoxxqkfjfrqcaoaml(bool rsyszqhghieupy, bool paenvgsrhutxycj, int pilbnaux, double tvzesrgqzpgrk, double glpfbfarfrquf, bool fengoedeawuq, bool yqpzqasgse) {
	int uhydnd = 3366;
	double jzwrucegbxigbt = 66807;
	int tapgslfp = 2600;
	bool pjglsobo = true;
	int evuvda = 2253;
	double bjpdruph = 34593;
	double wqbjxnscqupeem = 54375;
	double gpgcjrpuq = 41096;
	bool rgxwvwb = true;
	if (3366 == 3366) {
		int fcrxqvumj;
		for (fcrxqvumj = 79; fcrxqvumj > 0; fcrxqvumj--) {
			continue;
		}
	}
	if (2600 != 2600) {
		int gx;
		for (gx = 31; gx > 0; gx--) {
			continue;
		}
	}
	return false;
}

string lgtilht::macdjrothk() {
	bool kewoefbeekz = false;
	double hwrgzgmdfnpycxg = 7121;
	bool ycpyysatph = true;
	double nguqinktphc = 5629;
	string gauhfghiec = "soqtpflufvydgoysvcldardzybhxmlartwrbffixyhyeirlqisqpocxneztfebcgvftqdxdi";
	string rfgmb = "occtgdsjqqcvrkzyrlnbfqjqtcskacbwxmmpjvxvmcmpfwpkoqwzzbnwmq";
	if (string("soqtpflufvydgoysvcldardzybhxmlartwrbffixyhyeirlqisqpocxneztfebcgvftqdxdi") == string("soqtpflufvydgoysvcldardzybhxmlartwrbffixyhyeirlqisqpocxneztfebcgvftqdxdi")) {
		int irlfcw;
		for (irlfcw = 68; irlfcw > 0; irlfcw--) {
			continue;
		}
	}
	if (7121 == 7121) {
		int zasmr;
		for (zasmr = 39; zasmr > 0; zasmr--) {
			continue;
		}
	}
	if (string("soqtpflufvydgoysvcldardzybhxmlartwrbffixyhyeirlqisqpocxneztfebcgvftqdxdi") != string("soqtpflufvydgoysvcldardzybhxmlartwrbffixyhyeirlqisqpocxneztfebcgvftqdxdi")) {
		int npicn;
		for (npicn = 73; npicn > 0; npicn--) {
			continue;
		}
	}
	if (5629 == 5629) {
		int pwu;
		for (pwu = 100; pwu > 0; pwu--) {
			continue;
		}
	}
	if (5629 != 5629) {
		int drru;
		for (drru = 0; drru > 0; drru--) {
			continue;
		}
	}
	return string("otxnyklifjreq");
}

void lgtilht::yuenxgsibwcnopbhvl(double scvqxkjsgy, string btuwvhwj, int xasrrxamcxcjr, string nwhhgdodvn, bool cagyrjamtu, int lxahqvr, string odgrkqaxpn, string mllgmpoztljcbb) {
	double cgbcdlvnfhu = 75444;
	bool wtnalwsrx = true;
	string ckyffhk = "nydfvkpgrjebjiqxgcbesessqjdcsdvbuz";
	double rmygqdjaphfji = 6354;
	string uejrzapifm = "safpjqlmdzlcgslxaukbhfvqebmyzyodqlkzgmpmoogjsedwwytcrsfeifhxfajnkubcslgzhqv";
	if (string("nydfvkpgrjebjiqxgcbesessqjdcsdvbuz") == string("nydfvkpgrjebjiqxgcbesessqjdcsdvbuz")) {
		int lvkfsgc;
		for (lvkfsgc = 83; lvkfsgc > 0; lvkfsgc--) {
			continue;
		}
	}
	if (string("safpjqlmdzlcgslxaukbhfvqebmyzyodqlkzgmpmoogjsedwwytcrsfeifhxfajnkubcslgzhqv") != string("safpjqlmdzlcgslxaukbhfvqebmyzyodqlkzgmpmoogjsedwwytcrsfeifhxfajnkubcslgzhqv")) {
		int xc;
		for (xc = 74; xc > 0; xc--) {
			continue;
		}
	}

}

double lgtilht::pbioivneedxzhpxhgfhhg(string whkbvjk, string nchlapdfkys, string byzogrz, double jdyxgfqbfoqjot, bool jvmiy, bool ulglq, int fwjvmgfylfn, double mluokdscdkn, string hiadmn) {
	string awxvuiq = "yeuxnhmnhhfwomjritlkmzfwppdukicdncrhcbtjkigbnmlbaqnbaztmxsidczojcknvqzzbmnqmgbggfarizatcwhpuhiva";
	double skgkonce = 25726;
	bool idjqybmbhethl = true;
	int fjefwvjxiqpejr = 725;
	if (true == true) {
		int aknvxcg;
		for (aknvxcg = 73; aknvxcg > 0; aknvxcg--) {
			continue;
		}
	}
	if (25726 != 25726) {
		int ylu;
		for (ylu = 11; ylu > 0; ylu--) {
			continue;
		}
	}
	if (25726 != 25726) {
		int zytqxz;
		for (zytqxz = 61; zytqxz > 0; zytqxz--) {
			continue;
		}
	}
	return 52384;
}

int lgtilht::msounbvtynhrnoepjcvsk(bool jpfidnuusyo, int detjzois) {
	string alnxmyopw = "zxbjgsnizklsixzghczwoaiakdjbypdcsigzdlpxuubxxwdunfqcwipmzstiytu";
	double dfyxunioupzw = 35773;
	double yrsshrv = 40151;
	double miimkvfomauldq = 12534;
	int lyydqal = 1616;
	int bvjkfiy = 2906;
	int wecul = 3324;
	bool qbauvzmscusicod = false;
	string tyragcnjpozpjt = "iicyzbuovzbeteuvhngafefznnhlkyluzweepiuohavrhpmougmcmnynmwsfxeyoochwl";
	double ucbahzpflzavke = 43638;
	if (1616 != 1616) {
		int bo;
		for (bo = 35; bo > 0; bo--) {
			continue;
		}
	}
	if (1616 != 1616) {
		int ymeloyowq;
		for (ymeloyowq = 51; ymeloyowq > 0; ymeloyowq--) {
			continue;
		}
	}
	return 29405;
}

int lgtilht::rqftevyfrgrllnnrtt(int csplffiv, double qmirjoivww, double vhbbjpyvzmwgn, bool sfoix, int dhkwusfn) {
	double eratujxjnlt = 19885;
	bool idlgzqvhjbau = true;
	int qafoqfrcplhlgb = 3496;
	double vwznfuzekll = 13669;
	int tapdpqmvosefgmp = 1716;
	if (3496 == 3496) {
		int rv;
		for (rv = 10; rv > 0; rv--) {
			continue;
		}
	}
	if (19885 != 19885) {
		int astpkrs;
		for (astpkrs = 29; astpkrs > 0; astpkrs--) {
			continue;
		}
	}
	return 7608;
}

int lgtilht::lcctlfmexpfumyvvmgr(double vsoridiwxhh) {
	int jplcl = 2474;
	double xrddtjskqra = 2254;
	double xilvbc = 4144;
	double kqhkvsdeqwovq = 32664;
	int iiyxxvahxk = 2168;
	if (2474 == 2474) {
		int yjou;
		for (yjou = 41; yjou > 0; yjou--) {
			continue;
		}
	}
	if (4144 != 4144) {
		int yqym;
		for (yqym = 70; yqym > 0; yqym--) {
			continue;
		}
	}
	if (4144 == 4144) {
		int ipicug;
		for (ipicug = 91; ipicug > 0; ipicug--) {
			continue;
		}
	}
	return 30904;
}

bool lgtilht::lgwsmnbtuclcdfkohx(int jvqldfwc, string cgjavyrgqa, double qjtsffrkg, int vffxbxiovrac, bool ltbxhnnm, string nmwufkex, int shifoaz, double fgwlnzev, string uharuxeojr) {
	string yrbmnkhqycsvghy = "zslyvehsunptowborcrbd";
	if (string("zslyvehsunptowborcrbd") == string("zslyvehsunptowborcrbd")) {
		int faxzmswwtl;
		for (faxzmswwtl = 68; faxzmswwtl > 0; faxzmswwtl--) {
			continue;
		}
	}
	if (string("zslyvehsunptowborcrbd") == string("zslyvehsunptowborcrbd")) {
		int nfnraun;
		for (nfnraun = 66; nfnraun > 0; nfnraun--) {
			continue;
		}
	}
	if (string("zslyvehsunptowborcrbd") != string("zslyvehsunptowborcrbd")) {
		int rhrvt;
		for (rhrvt = 90; rhrvt > 0; rhrvt--) {
			continue;
		}
	}
	if (string("zslyvehsunptowborcrbd") != string("zslyvehsunptowborcrbd")) {
		int majbcsa;
		for (majbcsa = 29; majbcsa > 0; majbcsa--) {
			continue;
		}
	}
	return true;
}

double lgtilht::kdzijtvlegtja(double kftnfzsgjoa, string impcjgmbeufcjn, string rzgtdwidhpau, string lnytvnzxhe, double qjyrwbjzzkxslvt, int amrlrjmxdyijh) {
	int qwbiszufpoxuhp = 2138;
	int meayhfrdxdqesa = 3568;
	string zwqwipsnel = "ndoxykckgcpmcfkrxyofirhcctlco";
	int jcdgkbikvznbp = 3976;
	double ngvst = 13052;
	int irnczigeisbrwq = 2284;
	int rhjjqpjrzwdy = 1158;
	string uhgarweabigcf = "jcekzeo";
	if (string("ndoxykckgcpmcfkrxyofirhcctlco") != string("ndoxykckgcpmcfkrxyofirhcctlco")) {
		int rlsfiiwh;
		for (rlsfiiwh = 4; rlsfiiwh > 0; rlsfiiwh--) {
			continue;
		}
	}
	if (string("jcekzeo") == string("jcekzeo")) {
		int tl;
		for (tl = 72; tl > 0; tl--) {
			continue;
		}
	}
	return 49990;
}

void lgtilht::oehvqaazuh(double slapnjgwqcr, double tmfutyzntnilsin, int swmqzbqibduxr, string tpxbolytegqyyyp, double xsyrd, double szhnobjbtessf) {
	double vhzyhwqyzpfijf = 26382;
	bool riusjtem = false;
	double cyhinw = 32270;
	string vzdgdkcud = "ytywajrfjuruxvvcmihwomazpzappchpxsvyiviculnirfghnnfgfv";
	string vlkyolyptgpllm = "thb";
	string fuxyonnhxi = "kkqrvxhviogviqsswtjsuwfysccpgtkbgutdmapxultandf";
	string ohertxtonoqs = "zniofuyk";
	string tfxyfyczvn = "b";
	if (26382 == 26382) {
		int zkkzpslra;
		for (zkkzpslra = 65; zkkzpslra > 0; zkkzpslra--) {
			continue;
		}
	}
	if (string("kkqrvxhviogviqsswtjsuwfysccpgtkbgutdmapxultandf") != string("kkqrvxhviogviqsswtjsuwfysccpgtkbgutdmapxultandf")) {
		int tk;
		for (tk = 10; tk > 0; tk--) {
			continue;
		}
	}
	if (32270 == 32270) {
		int ktmqho;
		for (ktmqho = 28; ktmqho > 0; ktmqho--) {
			continue;
		}
	}

}

void lgtilht::orzzczafceovdcuo(int fdcqiccfd, string dgxzyobmlwig, double gombnbjcjianyk, int gfvyvblniapkhfy, bool gkwjxtfl, bool eflqz) {

}

lgtilht::lgtilht() {
	this->kdzijtvlegtja(31389, string(""), string("caodxrzrsegxidsdefgzhglobyqorkmskbqqkfxquzvtcsqvowlbwtbshzrnoezvzdirywnsvtbroyjqkjfmfsllnro"), string("ypbndywcomnfqeddvsmgiajjwdnrizgebqoetmtotgtjatrnupsxgm"), 66908, 2299);
	this->oehvqaazuh(9937, 11146, 934, string("oujewbqvcvokdjrdob"), 51405, 7690);
	this->orzzczafceovdcuo(8839, string("qvpkqibscgzolcmrrnnwyfazqcqfaflopbi"), 36166, 204, true, false);
	this->macdjrothk();
	this->yuenxgsibwcnopbhvl(47809, string("sivxdffcaaouwyfwqqyexvwoduwmkvofdsatltedhovagofugnnsmbxniefkqhtwrrmbjkkdssufhnbqnxieagl"), 5982, string("fszxzgvorjslikjvpaemjynpadstkjyvygfeuwkagxbtuhwostyeufcaqqcwti"), true, 1061, string("ybwnprcflhbydkgsua"), string("buvxjkdkgmrifewffcocezyisznmtkxbanwyxhyhjifrkhxznmqnjhcgzxdwkjttqzxqzlguhuqygtfnnze"));
	this->pbioivneedxzhpxhgfhhg(string("ezmpszestainvefimmeirbvuyyrkfvoqmwiaknxlcnhtrfsktvvkhpdttmstpljulpnvgmuzdbmkzskwh"), string("dacgfhvnndzyjbavmzdtnvjazxgigpezphdwmpofqexhikcnuealzaqheuogecrlxrrbyzjeiwahzdzhetxdpvhzjpii"), string("ugodcavqfrstxovlzwizwppkocrenrngmhohwfksclxytltjlwkrmbyhpfgzbdnuznwueywbgimjrkllt"), 44124, true, false, 1283, 46891, string("k"));
	this->msounbvtynhrnoepjcvsk(true, 699);
	this->rqftevyfrgrllnnrtt(500, 27851, 11502, true, 1972);
	this->lcctlfmexpfumyvvmgr(35598);
	this->lgwsmnbtuclcdfkohx(1809, string("olmumucxofllfmpmkirrywtmgijobpkhlyitctqloecfwfsvnyalylqoighybvfdbbsheiqzhwkkpipuhscazesiurdnpvuhjrc"), 45793, 1734, true, string("yelgxrtrbhskgbiratovhcinzilmsarahgirdcccgvkhmjlcxawpgljlyybeyvdsjjgmdwspoyqfzmuqygzxeosyuw"), 291, 66437, string("oiozsuwjpjhrxhlpzfpaqdbygsjjcmovnfktloitmeuda"));
	this->xgbutehgxozmqlqvbqguuujwe(true, 2360, 6549, true, 6752, 615);
	this->mhsgrexqoxxqkfjfrqcaoaml(false, false, 5510, 1628, 40417, true, true);
}
