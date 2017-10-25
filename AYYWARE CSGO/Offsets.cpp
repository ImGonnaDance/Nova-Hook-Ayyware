/*
Rest In Peace ApocalypseCheats
*/

#include "Offsets.h"
#include "UTIL Functions.h"

#define strenc( s ) ( s )

void Offsets::Initialise()
{
	// Modules
	Modules::DataCaches = Utilities::Memory::WaitOnModuleHandle("datacache.dll");
	Modules::Client = Utilities::Memory::WaitOnModuleHandle("client.dll");
	Modules::Engine = Utilities::Memory::WaitOnModuleHandle("engine.dll");
	Modules::VGUI2 = Utilities::Memory::WaitOnModuleHandle("vgui2.dll");
	Modules::VGUISurface = Utilities::Memory::WaitOnModuleHandle("vguimatsurface.dll");
	Modules::Material = Utilities::Memory::WaitOnModuleHandle("materialsystem.dll");
	Modules::VPhysics = Utilities::Memory::WaitOnModuleHandle("vphysics.dll");
	Modules::Stdlib = Utilities::Memory::WaitOnModuleHandle("vstdlib.dll");
	Modules::InputLib = Utilities::Memory::WaitOnModuleHandle("inputsystem.dll");

	//------------------------------------------------------------------------
	// VTables
#pragma region VTables
	VMT::CHL_GetAllClasses = 8;

	VMT::Engine_GetScreenSize = 5;
	VMT::Engine_GetPlayerInfo = 8;
	VMT::Engine_GetLocalPlayer = 12;
	VMT::Engine_Time = 14;
	VMT::Engine_GetViewAngles = 18;
	VMT::Engine_SetViewAngles = 19;
	VMT::Engine_GetMaxClients = 20;
	VMT::Engine_IsConnected = 27;
	VMT::Engine_IsInGame = 26;
	VMT::Engine_WorldToScreenMatrix = 37;
	VMT::Engine_ClientCmd_Unrestricted = 114;

	VMT::Panel_GetName = 36;
	VMT::Panel_PaintTraverse = 41;

	VMT::Surface_DrawSetColorA = 14;
	VMT::Surface_DrawSetColorB = 15;
	VMT::Surface_DrawFilledRect = 16;
	VMT::Surface_DrawOutlinedRect = 18;
	VMT::Surface_DrawLine = 19;
	VMT::Surface_DrawSetTextFont = 23;
	VMT::Surface_DrawSetTextColorA = 24;
	VMT::Surface_DrawSetTextColorB = 25;
	VMT::Surface_DrawSetTextPos = 26;
	VMT::Surface_DrawPrintText = 28;
	VMT::Surface_DrawSetTextureRGBA = 37;
	VMT::Surface_DrawSetTexture = 38;
	VMT::Surface_CreateNewTextureID  = 43;
	VMT::Surface_FontCreate = 71;
	VMT::Surface_SetFontGlyphSet = 72;
	VMT::Surface_GetTextSize = 79;
	VMT::Surface_DrawOutlinedCircle = 103;
	VMT::Surface_SurfaceGetCursorPos = 66;
	VMT::Surface_DrawTexturedPolygon = 106;

	VMT::Material_GetName = 0;
	VMT::Material_SetMaterialVarFlag = 30;
	VMT::Material_GetMaterialVarFlag = 31;
	VMT::Material_AlphaModulate = 28;
	VMT::Material_ColorModulate = 29;
	VMT::Material_IncrementReferenceCount = 14;
	
	VMT::MaterialSystem_FindMaterial = 84;
	VMT::MaterialSystem_CreateMaterial = 83;
	
	VMT::ModelRender_ForcedMaterialOverride = 1;
	VMT::ModelRender_DrawModelExecute = 21;
	
	VMT::ModelInfo_GetModelName = 3;
	VMT::ModelInfo_GetStudiomodel = 30;

	VMT::RenderView_SetBlend = 4;
	VMT::RenderView_SetColorModulation = 6;

#pragma endregion Contains the VTable Indexs

	// I cbf trying to get the KeyValues part of the SDK working solo, so we'll just
	// Do some dirty shit
	Functions::KeyValues_KeyValues = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x68\x00\x00\x00\x00\x8B\xC8\xE8\x00\x00\x00\x00\x89\x45\xFC\xEB\x07\xC7\x45\x00\x00\x00\x00\x00\x8B\x03\x56", "x????xxx????xxxxxxx?????xxx");
	Functions::KeyValues_KeyValues += 7;
	Functions::KeyValues_KeyValues = Functions::KeyValues_KeyValues + *reinterpret_cast< PDWORD_PTR >(Functions::KeyValues_KeyValues + 1) + 5;

	Functions::KeyValues_LoadFromBuffer = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\xE8\x00\x00\x00\x00\x80\x7D\xF8\x00\x00\x00\x85\xDB", "x????xxxx??xx");
	Functions::KeyValues_LoadFromBuffer = Functions::KeyValues_LoadFromBuffer + *reinterpret_cast< PDWORD_PTR >(Functions::KeyValues_LoadFromBuffer + 1) + 5;

	Functions::dwCalcPlayerView = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x84\xC0\x75\x08\x57\x8B\xCE\xE8\x00\x00\x00\x00\x8B\x06", "xxxxxxxx????xx");

	Functions::dwGetPlayerCompRank = GameUtils::FindPattern1(strenc("client.dll"), strenc("55 8B EC 8B 0D ? ? ? ? 68 ? ? ? ? "));

	Functions::dwIsReady = GameUtils::FindPattern1(strenc("client.dll"), strenc("55 8B EC 51 56 8B 35 ? ? ? ? 80 7E 58 00"));

	Utilities::Log("Offsets/Indexes Up to Date");
}

namespace Offsets
{
	// Addresses of loaded game modules
	namespace Modules
	{
		DWORD Client;
		DWORD Engine;
		DWORD VGUI2;
		DWORD VGUISurface;
		DWORD Material;
		DWORD VPhysics;
		DWORD Stdlib;
		DWORD Offsets::Modules::InputLib;
		DWORD DataCaches;
	};

	// Virtual Method Table Indexes
	namespace VMT
	{
		//CHL Client
		DWORD CHL_GetAllClasses;

		//Engine Client
		DWORD Engine_GetScreenSize;
		DWORD Engine_GetPlayerInfo;
		DWORD Engine_GetLocalPlayer;
		DWORD Engine_Time;
		DWORD Engine_GetViewAngles;
		DWORD Engine_SetViewAngles;
		DWORD Engine_GetMaxClients;
		DWORD Engine_IsConnected;
		DWORD Engine_IsInGame;
		DWORD Engine_WorldToScreenMatrix;
		DWORD Engine_GetNetChannelInfo;
		DWORD Engine_ClientCmd_Unrestricted;

		// Panels
		DWORD Panel_GetName;
		DWORD Panel_PaintTraverse;

		// Surface
		DWORD Surface_DrawSetColorA;
		DWORD Surface_DrawSetColorB;
		DWORD Surface_DrawFilledRect;
		DWORD Surface_DrawOutlinedRect;
		DWORD Surface_DrawLine;
		DWORD Surface_DrawSetTextFont;
		DWORD Surface_DrawSetTextColorA;
		DWORD Surface_DrawSetTextColorB;
		DWORD Surface_DrawSetTextPos;
		DWORD Surface_DrawPrintText;
		DWORD Surface_DrawSetTextureRGBA;
		DWORD Surface_DrawSetTexture;
		DWORD Surface_CreateNewTextureID;
		DWORD Surface_FontCreate;
		DWORD Surface_SetFontGlyphSet;
		DWORD Surface_GetTextSize;
		DWORD Surface_DrawOutlinedCircle;
		DWORD Surface_SurfaceGetCursorPos;
		DWORD Surface_DrawTexturedPolygon;

		DWORD Material_GetName;
		DWORD Material_SetMaterialVarFlag;
		DWORD Material_GetMaterialVarFlag;
		DWORD Material_AlphaModulate;
		DWORD Material_ColorModulate;
		DWORD Material_IncrementReferenceCount;

		DWORD MaterialSystem_FindMaterial;
		DWORD MaterialSystem_CreateMaterial;

		DWORD ModelRender_ForcedMaterialOverride;
		DWORD ModelRender_DrawModelExecute;

		DWORD ModelInfo_GetModelName;
		DWORD ModelInfo_GetStudiomodel;

		DWORD RenderView_SetBlend;
		DWORD RenderView_SetColorModulation;

		// Weapon entities
		DWORD Weapon_GetSpread;
	};

	// Addresses of engine functions to call
	namespace Functions
	{
		DWORD KeyValues_KeyValues;
		DWORD KeyValues_LoadFromBuffer;
		DWORD dwCalcPlayerView;
		DWORD dwGetPlayerCompRank;
		DWORD dwIsReady;
	};

};

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qrtvtwt {
public:
	string botjsrtpob;
	int brmyenqoexzaka;
	qrtvtwt();
	void kxghuocbcmkpwvhouqehzz(int mzmzgrscfmtqick, int jebgkogy, bool tkpko, string brdbwyx);
	string krbyvkdlzlzlhrfvpzw(string acslywexrryp);
	void fmzmetgrsoptkujgeeejktcu(string gjnxtd, bool ioeqw, double ntigzk, double paoqyyrxxwbg, bool giwhivtxhnny, int lifbpaj, int mxjsygsdnpsnhr, double zerrtzsereyiqa, string wknddyvjhdpmqva, string xpubnrqpto);

protected:
	int ajmuilcgm;

	bool rhqcpsulshlicyvxogfhtizt(double ymczquqspfyctx, bool gmawoaurbhrdeb, string plqxbkyinnmqtg, bool bglzyrkf, bool yzzntyapq);
	void mpyybtlgwvgmknzctbsyakaj(double mcizyiiyor);
	void ffmdeidtyuvmlfy(double yfoglrzuvxx, double rrhlvaj);

private:
	int ovndnbyrqmk;
	int erycrhacuopj;
	bool yygzgp;
	double jmkjmpqcculol;
	bool ideizno;

	string chzfzpchbfnyljn(double bazclcfzx, int yimewrydxre, bool imspvdgz, string neihoi, string uojzqnoictdyub, bool shwmbbczrikbr, bool nkmfdwvubpkth);
	double iqqkgporkgievzv(string kgflfbzu, int yzipyegtey, string ndzcayptvupmifp, string mfwbwxqrzyaufy, bool jqkjuff, bool jmywsdluelgws, double eaktu, int aabxxbtf, string gqmuxlgsir);
	void tltbkjsmsyr(double nsnxo, string bbgsx, double hyonutyshuri, double pbqagt);

};


string qrtvtwt::chzfzpchbfnyljn(double bazclcfzx, int yimewrydxre, bool imspvdgz, string neihoi, string uojzqnoictdyub, bool shwmbbczrikbr, bool nkmfdwvubpkth) {
	double eaaivb = 7173;
	string ujgqgwke = "kcwhlftopswytdokljkxwpuslrwozbbtwoooveaophkzzcmshfuzqjmctanrrbprnzivompwmrexugblrztvcenkjogxzhazwqi";
	int bmcslc = 5780;
	bool sgaxotr = true;
	string pnvtjwcpvgyjib = "hpbetpbxqxiwrgqguxgxlgkainurvehsprijkcyubhyevhhyzgqusubgb";
	double gbexy = 84304;
	double scznzion = 35651;
	if (7173 == 7173) {
		int kzcbihvin;
		for (kzcbihvin = 55; kzcbihvin > 0; kzcbihvin--) {
			continue;
		}
	}
	if (string("kcwhlftopswytdokljkxwpuslrwozbbtwoooveaophkzzcmshfuzqjmctanrrbprnzivompwmrexugblrztvcenkjogxzhazwqi") == string("kcwhlftopswytdokljkxwpuslrwozbbtwoooveaophkzzcmshfuzqjmctanrrbprnzivompwmrexugblrztvcenkjogxzhazwqi")) {
		int ugqumpsmh;
		for (ugqumpsmh = 9; ugqumpsmh > 0; ugqumpsmh--) {
			continue;
		}
	}
	if (7173 == 7173) {
		int lgqmwez;
		for (lgqmwez = 7; lgqmwez > 0; lgqmwez--) {
			continue;
		}
	}
	return string("xnciglrvszhyymnk");
}

double qrtvtwt::iqqkgporkgievzv(string kgflfbzu, int yzipyegtey, string ndzcayptvupmifp, string mfwbwxqrzyaufy, bool jqkjuff, bool jmywsdluelgws, double eaktu, int aabxxbtf, string gqmuxlgsir) {
	bool nciqpvwfqe = false;
	double ytpqpbnzlfx = 35826;
	string jjneqxr = "vssgwelhhvrmzjrwfybbgaukrbvvjjmauluolinweyvqvszyhdepnz";
	int sxtdw = 5200;
	int izpladymbsvuah = 402;
	bool sglmtfygxlcxzi = true;
	string mzmmbqef = "byljpqnkbhczirmtldlptfepjntlkdpghkqponhrpuvreinchew";
	return 68504;
}

void qrtvtwt::tltbkjsmsyr(double nsnxo, string bbgsx, double hyonutyshuri, double pbqagt) {
	int muoympdzjnefmfq = 754;
	int aurtal = 3801;
	int carrinqk = 1647;
	int qqfzcwdqysdvmb = 6180;
	bool zzbgmwzrbe = true;
	bool ynkavemlxi = false;
	int frfilvnewro = 946;
	double apqrygakykjqwtw = 8332;
	int frpsuydegnnfd = 5895;
	if (8332 == 8332) {
		int yjjq;
		for (yjjq = 60; yjjq > 0; yjjq--) {
			continue;
		}
	}
	if (1647 == 1647) {
		int mkjq;
		for (mkjq = 30; mkjq > 0; mkjq--) {
			continue;
		}
	}

}

bool qrtvtwt::rhqcpsulshlicyvxogfhtizt(double ymczquqspfyctx, bool gmawoaurbhrdeb, string plqxbkyinnmqtg, bool bglzyrkf, bool yzzntyapq) {
	int gssxfxc = 1037;
	double xurodvsukinm = 58909;
	int trmmdtzima = 3832;
	double cjgixramquhdnk = 1340;
	int cgdyln = 503;
	int ygajtrnfrrxp = 5407;
	if (5407 == 5407) {
		int vgosdmq;
		for (vgosdmq = 57; vgosdmq > 0; vgosdmq--) {
			continue;
		}
	}
	if (58909 == 58909) {
		int wauvusong;
		for (wauvusong = 5; wauvusong > 0; wauvusong--) {
			continue;
		}
	}
	if (503 != 503) {
		int zvesfynpr;
		for (zvesfynpr = 11; zvesfynpr > 0; zvesfynpr--) {
			continue;
		}
	}
	if (503 == 503) {
		int srsfmpsm;
		for (srsfmpsm = 39; srsfmpsm > 0; srsfmpsm--) {
			continue;
		}
	}
	if (58909 == 58909) {
		int nxgedtg;
		for (nxgedtg = 45; nxgedtg > 0; nxgedtg--) {
			continue;
		}
	}
	return false;
}

void qrtvtwt::mpyybtlgwvgmknzctbsyakaj(double mcizyiiyor) {
	double qnorolximlafedb = 513;
	bool rmgaybrljuunm = false;
	bool rznhzalvhk = true;
	double lbpks = 58554;
	string ozttflqnuuers = "fgtxqlfxloabgibuysdpdaojbfgngxur";
	string rracyrcgcvgeh = "snpfsfvhajyfsrrgxliujbdgauwfjiktliddxzkuzzkcoxrkrxedjunkjborosejuxwzaxgbrfmgrnhcxqhlrhtit";
	double bjkixlocqxg = 70304;
	int xgrvjlcz = 6566;
	if (513 != 513) {
		int gqvihs;
		for (gqvihs = 58; gqvihs > 0; gqvihs--) {
			continue;
		}
	}
	if (513 != 513) {
		int hqg;
		for (hqg = 57; hqg > 0; hqg--) {
			continue;
		}
	}
	if (513 == 513) {
		int eaj;
		for (eaj = 9; eaj > 0; eaj--) {
			continue;
		}
	}
	if (58554 != 58554) {
		int lcemoru;
		for (lcemoru = 28; lcemoru > 0; lcemoru--) {
			continue;
		}
	}

}

void qrtvtwt::ffmdeidtyuvmlfy(double yfoglrzuvxx, double rrhlvaj) {
	bool cjebdgzjda = true;

}

void qrtvtwt::kxghuocbcmkpwvhouqehzz(int mzmzgrscfmtqick, int jebgkogy, bool tkpko, string brdbwyx) {
	string nvpunseyyaq = "xbmjdcnlqninhcpyyiisigiillpogaktnzsypgmurxqdyjybhaxxpjveqiduce";
	bool xqzuzahtegmpyj = true;
	bool lddmumfkiby = false;
	double dyithjitcoqo = 993;
	bool cyxizfltpw = true;
	double rzhelqbdvmcr = 29653;
	string bzuyhhbnhzgie = "wmgj";

}

string qrtvtwt::krbyvkdlzlzlhrfvpzw(string acslywexrryp) {
	int bamfkywcfjrdlhy = 3646;
	bool hawbl = false;
	bool zfdoltz = true;
	bool bwgbgadzkggz = false;
	bool xxbndvdgymy = false;
	string xerauu = "igmbbfbfceuysxaowpiqunknorpmonmahvftwsxiqtcbmsiprkkwajhhctajf";
	string euyfide = "jocciqgdmzogostbeyixbqaiptnrigredbavjt";
	double nohrcrwbifv = 61222;
	double napvdnhbcsj = 7871;
	if (61222 == 61222) {
		int pbraxunf;
		for (pbraxunf = 1; pbraxunf > 0; pbraxunf--) {
			continue;
		}
	}
	return string("kw");
}

void qrtvtwt::fmzmetgrsoptkujgeeejktcu(string gjnxtd, bool ioeqw, double ntigzk, double paoqyyrxxwbg, bool giwhivtxhnny, int lifbpaj, int mxjsygsdnpsnhr, double zerrtzsereyiqa, string wknddyvjhdpmqva, string xpubnrqpto) {
	bool zevdpuvzbmwt = true;
	double nhygpldjiwcoge = 62360;
	double vzqdbttv = 88139;
	if (62360 == 62360) {
		int jhj;
		for (jhj = 97; jhj > 0; jhj--) {
			continue;
		}
	}
	if (62360 == 62360) {
		int xkgtd;
		for (xkgtd = 57; xkgtd > 0; xkgtd--) {
			continue;
		}
	}
	if (88139 == 88139) {
		int khwc;
		for (khwc = 44; khwc > 0; khwc--) {
			continue;
		}
	}
	if (62360 != 62360) {
		int ddmk;
		for (ddmk = 74; ddmk > 0; ddmk--) {
			continue;
		}
	}
	if (88139 == 88139) {
		int czgo;
		for (czgo = 45; czgo > 0; czgo--) {
			continue;
		}
	}

}

qrtvtwt::qrtvtwt() {
	this->kxghuocbcmkpwvhouqehzz(874, 204, true, string("uhjesmtzjcoiwczcedbksjzzbgyxqtyxxrgkhrbzesnradjrvkpfucooouxa"));
	this->krbyvkdlzlzlhrfvpzw(string("cfdbadaztaydcidwsufzugypbtybjpgmavhvosjjblnqzgsuzwykhcytfrwxepegmbjfusxgw"));
	this->fmzmetgrsoptkujgeeejktcu(string("fiaviaavbschcbwputwtporxrmhjovdmlgtsnkkcbagphtorhnaavpfneysubwnntptterrkgkfcvtyeuorkbkdg"), false, 46675, 28449, false, 4900, 405, 7981, string("dejllhghrpqqirmlhuqqyejtawolqvgrecyqwjkuh"), string("navxwvhmpqnasuibnqir"));
	this->rhqcpsulshlicyvxogfhtizt(42050, false, string("ogylecvmbcppcqzkreswwgiprdjmbdejqlvqknvrybwh"), false, false);
	this->mpyybtlgwvgmknzctbsyakaj(15605);
	this->ffmdeidtyuvmlfy(46908, 773);
	this->chzfzpchbfnyljn(26235, 233, true, string("qxrkogtjqeimd"), string("sumjlniifxgktxnanrqbbywyzxqffqevlqkixibpq"), false, false);
	this->iqqkgporkgievzv(string("govcmwriwcaxr"), 1066, string("rhsqkrdvbdpvgplssvafkcwyqlmithigeoxjyqewymitklaavqdqwcvbpbyxtjaluumdwkupw"), string("fufhtalepufvpcoboqnhzgikodqlcjicuggwabaxtckdyenagoftwyvmgfpt"), false, false, 40625, 1206, string("snshilpzjvqcdzmkmqnmvchdaefwtgxdjyxqhfektflxlnlavvbygejhjniytcihnoujlejjplyadlbxcqlfmlbamncacbnrnjdh"));
	this->tltbkjsmsyr(19956, string("iokfoflrhafkrkwdmjkbdbetfmoqtbhhgprhrfdfczmlanshdllydylfvhkpbtofrfjwnfalvjqgc"), 32205, 19808);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jaawdkb {
public:
	double ifghbcyngatedd;
	string vgkpmzpfohqpjh;
	double bceas;
	jaawdkb();
	string ffvthmroghpb(double ehhvdshqkaf, int dntqaxnz, string xsecdhxx, bool tcsmcbjxu, int eobbn, int qqucmvi, int xnrqlj, string sawpunkqjxbxu, int gfhcaossen, int cxzgppka);
	int sumigqjfmbyzhhyknlxthdhrz(bool rrrqtapowywlt, int xuuorwjp, string mreoqbkcrztvz, string dlzrhhezs, double lcwnwlfxldvj, double twlaok, bool twdmnkaplost, bool ftatyxjcsz, double bdtogps, bool rslapvtbrhc);
	double iqrlcymgsvkqkbuilbd(bool psvuicmitt, bool ljvzfqeibmkqdw, int fmvbptqemcqw, bool kvvjtfme, bool ufkwbsmyajnkyt, double ozgffrqmbfkz, int orgkhmdvhjm);
	bool dwqhfuiuiblntuhdjzawuj(string xvbcy, int yccqx, bool wuhagees, string rbiia, string xhqgdhb);
	int ahmhmnjdluompuztoutkja(int qevuznordjt, string vpshbqy, bool wfltriylae, int ufytvgjcm, string vagajsahvh);
	double uecfexlnvyruvyostsnml(int pcuuvkrttxyd, bool otxwgqwj, bool hrqitpem, bool jkzwrc, string yvikkl, double ishmulykzvs, int waxvojai, bool bxestdxtnbssf, double ecsvlojtptrzuzt);
	double lqmztowzpjnnhzca(int dlhlfwqdjeynix, double pubjt, double tmswqcvznlxuk, double vsndfncocxbyt);
	void yurldcyvmjg(double qhpawzjmsb, double dadpnzejuglez, string imhaaqffxsh, int jbsodmmmetxkrcj, bool htadhhkqrhkcy, string ykxgq, string xxstzhszjo, bool flarmljelegmmkg, string hxomklmla, string atdqusnmmlm);
	double ijvxgiwdwiqafkrhhsjbbp();

protected:
	string foett;
	double qxqvktcq;
	double fsaupyfar;
	int hljgoeuo;
	string hhsugeyc;

	int aaitprplmvlmkfy(string bekdftqmloh, int xdehfxchq);
	void mjzgqshfsybvlsoefnv(string esnvwxpogojevuk, bool mwjhbmxwzivz, int pjugvsvmme, bool eagrilucbebmxbq, string oylrqgyibbzgye, double lbeudrllucgehn, bool deuvkaipncmlal, string wnrhrdot);
	string mmqlhnzcszzluptqt(double kvguzpfhtjbvz, double eegiu, string tnhndbm, bool mtvdwloghhb, string bjpfwmpxhvfovb, int swkeozjhfihy);

private:
	double uebab;
	int wudkpmkqhaqu;
	int ytzzpbtuyj;
	double zvbnirgtbrjg;
	double lvonyb;

	string rgvuucnxlfaspnfykbe(string ejbfi);
	void qhpgpuasqemvqmecpklorkw(string tuayxzxzeoxdr, double papxfntgz, string ugjwhblh, bool upbflvhxhhff, string purdaczrhu);
	int lpbmzqpbbhyxi(string inhyf, bool uxfwtjxegkffcpn, string esqzjyrdwekovrp, int dvibsrdsto, string ajiys, int crixewpbxifwb, int ajalocoeoqb, bool rtwppievczfs, double qaismxqxsxk, string wdqka);
	bool wxqyooigocvujcvn(string ckyqtistgyxksnq, bool sgoalyokmgjvbm, int vhxgarxquvr, bool gtptz, int antnwtyeoyf, bool dtjibf);
	string qgpxexnwhi(bool eemztchc, string nhwljtxsbbk, int aqoikx, bool fanbh, string fgcrbjjppovmgcw, int ncsanfrodbpvcd, double rxoitkjn, double nsaajdwtsiyhck);

};


string jaawdkb::rgvuucnxlfaspnfykbe(string ejbfi) {
	string ffhzv = "jusjlrdeposcmhodighdnrvhfpxnnszvrx";
	int sunvinogcc = 4606;
	double mfojabrqgb = 75977;
	if (string("jusjlrdeposcmhodighdnrvhfpxnnszvrx") == string("jusjlrdeposcmhodighdnrvhfpxnnszvrx")) {
		int fhewftgwjg;
		for (fhewftgwjg = 86; fhewftgwjg > 0; fhewftgwjg--) {
			continue;
		}
	}
	if (75977 == 75977) {
		int owdaidv;
		for (owdaidv = 94; owdaidv > 0; owdaidv--) {
			continue;
		}
	}
	return string("qfukjtgtkfaejtma");
}

void jaawdkb::qhpgpuasqemvqmecpklorkw(string tuayxzxzeoxdr, double papxfntgz, string ugjwhblh, bool upbflvhxhhff, string purdaczrhu) {
	double ovhtolpkn = 45328;
	string ytuyro = "vyjixlazyjpeiwcvcxxbqgurfhwmxgtmsknnmgahfwpppadivdaprbjufrfyzjgbrgojkxtrugofkzsmebfrsypybr";
	int dxskerdfhfbclfw = 6096;
	int uwhmcuamzp = 192;
	double xjlqtnjnwyoqmw = 30605;
	bool zubnfemg = true;
	string tzrzly = "onszrsvtfxbbcdkulpxtdjs";
	double yfzqygkibw = 24693;
	bool imjovvcnq = true;
	if (6096 != 6096) {
		int nowo;
		for (nowo = 29; nowo > 0; nowo--) {
			continue;
		}
	}
	if (true != true) {
		int tpjinee;
		for (tpjinee = 47; tpjinee > 0; tpjinee--) {
			continue;
		}
	}

}

int jaawdkb::lpbmzqpbbhyxi(string inhyf, bool uxfwtjxegkffcpn, string esqzjyrdwekovrp, int dvibsrdsto, string ajiys, int crixewpbxifwb, int ajalocoeoqb, bool rtwppievczfs, double qaismxqxsxk, string wdqka) {
	double hiqcdfwkzrmvp = 25550;
	if (25550 != 25550) {
		int nx;
		for (nx = 88; nx > 0; nx--) {
			continue;
		}
	}
	if (25550 == 25550) {
		int dauo;
		for (dauo = 69; dauo > 0; dauo--) {
			continue;
		}
	}
	if (25550 == 25550) {
		int gqfxggugnw;
		for (gqfxggugnw = 16; gqfxggugnw > 0; gqfxggugnw--) {
			continue;
		}
	}
	if (25550 != 25550) {
		int wjytyhx;
		for (wjytyhx = 93; wjytyhx > 0; wjytyhx--) {
			continue;
		}
	}
	return 16623;
}

bool jaawdkb::wxqyooigocvujcvn(string ckyqtistgyxksnq, bool sgoalyokmgjvbm, int vhxgarxquvr, bool gtptz, int antnwtyeoyf, bool dtjibf) {
	int ieoiwtv = 7573;
	int fipkjpwegt = 4785;
	int prrpgkafnqqe = 3503;
	int ctwulghz = 1395;
	string fimviburcdtbg = "ipdndzlewoitienwnyzzsfsf";
	double eihbtjqkny = 3972;
	int neehdqwdzfqlqs = 6057;
	bool ktuozsjtbgh = false;
	bool cjebm = true;
	if (4785 != 4785) {
		int rbcc;
		for (rbcc = 25; rbcc > 0; rbcc--) {
			continue;
		}
	}
	if (string("ipdndzlewoitienwnyzzsfsf") != string("ipdndzlewoitienwnyzzsfsf")) {
		int mshcbj;
		for (mshcbj = 46; mshcbj > 0; mshcbj--) {
			continue;
		}
	}
	if (1395 == 1395) {
		int puhi;
		for (puhi = 72; puhi > 0; puhi--) {
			continue;
		}
	}
	return true;
}

string jaawdkb::qgpxexnwhi(bool eemztchc, string nhwljtxsbbk, int aqoikx, bool fanbh, string fgcrbjjppovmgcw, int ncsanfrodbpvcd, double rxoitkjn, double nsaajdwtsiyhck) {
	int viugmn = 4466;
	double ifbuwcuywud = 19013;
	double wvzmf = 26124;
	double zldekoy = 84599;
	int mqmbbfmsachs = 1500;
	return string("nvhoemkm");
}

int jaawdkb::aaitprplmvlmkfy(string bekdftqmloh, int xdehfxchq) {
	bool amgbn = false;
	int sckcohwxbmg = 2917;
	double zqsxkyzxcfuwegu = 16741;
	double yisagjkuimdjy = 8558;
	double qqpjqpdlzkxtya = 2813;
	double wwlve = 13034;
	double canzu = 83852;
	string chjjfj = "iejicgmiahijzbobpprzauntlvxxlryxyxrtkjniretqvvrkvoh";
	double ndjhhnznojnbur = 15798;
	bool tqtzrwoxlobuq = false;
	return 43307;
}

void jaawdkb::mjzgqshfsybvlsoefnv(string esnvwxpogojevuk, bool mwjhbmxwzivz, int pjugvsvmme, bool eagrilucbebmxbq, string oylrqgyibbzgye, double lbeudrllucgehn, bool deuvkaipncmlal, string wnrhrdot) {
	string tafvd = "dguvbgfunivxwhaysdewharthgmpumrdbsnbuhjycgrbnqbgsbtwcmaygmuordjj";
	string bntglvdlsnjyaj = "mxpiqtfudzwiqnjvdgdegjbdymtwulrscxszogpsbqbybthfqkczbripaxphe";
	int wddymkgsynb = 2539;
	string pieha = "jjcsqslpvuhfukkjhjgvwlssxhb";
	double hbsme = 31794;
	double zjvnhn = 42300;
	bool qkogvi = false;
	if (2539 == 2539) {
		int nrx;
		for (nrx = 80; nrx > 0; nrx--) {
			continue;
		}
	}
	if (42300 == 42300) {
		int bszegge;
		for (bszegge = 33; bszegge > 0; bszegge--) {
			continue;
		}
	}
	if (string("jjcsqslpvuhfukkjhjgvwlssxhb") != string("jjcsqslpvuhfukkjhjgvwlssxhb")) {
		int qiwcrvaa;
		for (qiwcrvaa = 16; qiwcrvaa > 0; qiwcrvaa--) {
			continue;
		}
	}
	if (2539 == 2539) {
		int xrplw;
		for (xrplw = 75; xrplw > 0; xrplw--) {
			continue;
		}
	}
	if (string("jjcsqslpvuhfukkjhjgvwlssxhb") == string("jjcsqslpvuhfukkjhjgvwlssxhb")) {
		int ottfazihre;
		for (ottfazihre = 12; ottfazihre > 0; ottfazihre--) {
			continue;
		}
	}

}

string jaawdkb::mmqlhnzcszzluptqt(double kvguzpfhtjbvz, double eegiu, string tnhndbm, bool mtvdwloghhb, string bjpfwmpxhvfovb, int swkeozjhfihy) {
	return string("trkrxlosdsdmgye");
}

string jaawdkb::ffvthmroghpb(double ehhvdshqkaf, int dntqaxnz, string xsecdhxx, bool tcsmcbjxu, int eobbn, int qqucmvi, int xnrqlj, string sawpunkqjxbxu, int gfhcaossen, int cxzgppka) {
	string loqnrdmkyl = "jqehllhmgpnddrxuqzacxlzdxekfypmzdzleimcectprtntkenwrdyxkgpo";
	bool pafcpfxmjcqxbf = true;
	double fegjr = 36004;
	int sungley = 5718;
	double rcpkarmitnprvm = 26317;
	if (26317 == 26317) {
		int epmlqho;
		for (epmlqho = 0; epmlqho > 0; epmlqho--) {
			continue;
		}
	}
	if (26317 != 26317) {
		int xwkpvve;
		for (xwkpvve = 64; xwkpvve > 0; xwkpvve--) {
			continue;
		}
	}
	return string("xtvw");
}

int jaawdkb::sumigqjfmbyzhhyknlxthdhrz(bool rrrqtapowywlt, int xuuorwjp, string mreoqbkcrztvz, string dlzrhhezs, double lcwnwlfxldvj, double twlaok, bool twdmnkaplost, bool ftatyxjcsz, double bdtogps, bool rslapvtbrhc) {
	string kqpzvyfhm = "cvbaqbjsoabwgcezg";
	bool afbalnphuhjpsvy = false;
	double vnubr = 13154;
	bool gpiodnr = true;
	bool eacpyykffhatkoc = false;
	bool bqwfsmrjcwqg = true;
	double ewoxhscivd = 39707;
	int cpeyflmiebr = 8789;
	if (39707 == 39707) {
		int pszvlbyqx;
		for (pszvlbyqx = 11; pszvlbyqx > 0; pszvlbyqx--) {
			continue;
		}
	}
	if (13154 != 13154) {
		int hfv;
		for (hfv = 89; hfv > 0; hfv--) {
			continue;
		}
	}
	if (8789 == 8789) {
		int flkfpongqc;
		for (flkfpongqc = 39; flkfpongqc > 0; flkfpongqc--) {
			continue;
		}
	}
	if (string("cvbaqbjsoabwgcezg") != string("cvbaqbjsoabwgcezg")) {
		int sjetnadyp;
		for (sjetnadyp = 64; sjetnadyp > 0; sjetnadyp--) {
			continue;
		}
	}
	return 19611;
}

double jaawdkb::iqrlcymgsvkqkbuilbd(bool psvuicmitt, bool ljvzfqeibmkqdw, int fmvbptqemcqw, bool kvvjtfme, bool ufkwbsmyajnkyt, double ozgffrqmbfkz, int orgkhmdvhjm) {
	double esfbujyxymsyhxj = 28372;
	string hchydty = "nkypzmyebwcxzga";
	if (28372 != 28372) {
		int toufmed;
		for (toufmed = 55; toufmed > 0; toufmed--) {
			continue;
		}
	}
	return 92291;
}

bool jaawdkb::dwqhfuiuiblntuhdjzawuj(string xvbcy, int yccqx, bool wuhagees, string rbiia, string xhqgdhb) {
	double obaynmcqvklqjb = 5286;
	bool pzebf = true;
	string wqmyssezwgm = "gfxslivmvucbmkucfxvfekh";
	bool cjvisuoahgqz = false;
	bool wpecgkhhux = false;
	int jnsrsh = 3268;
	if (false == false) {
		int aaaekrir;
		for (aaaekrir = 80; aaaekrir > 0; aaaekrir--) {
			continue;
		}
	}
	return true;
}

int jaawdkb::ahmhmnjdluompuztoutkja(int qevuznordjt, string vpshbqy, bool wfltriylae, int ufytvgjcm, string vagajsahvh) {
	int jqxodwqnc = 1986;
	if (1986 != 1986) {
		int otul;
		for (otul = 11; otul > 0; otul--) {
			continue;
		}
	}
	if (1986 != 1986) {
		int hoq;
		for (hoq = 12; hoq > 0; hoq--) {
			continue;
		}
	}
	return 44439;
}

double jaawdkb::uecfexlnvyruvyostsnml(int pcuuvkrttxyd, bool otxwgqwj, bool hrqitpem, bool jkzwrc, string yvikkl, double ishmulykzvs, int waxvojai, bool bxestdxtnbssf, double ecsvlojtptrzuzt) {
	bool fatwoygdtceju = true;
	bool nxgwseetrsaeele = false;
	double qwnnbkjesnt = 17570;
	if (false != false) {
		int rdeq;
		for (rdeq = 64; rdeq > 0; rdeq--) {
			continue;
		}
	}
	if (17570 != 17570) {
		int dvtlzl;
		for (dvtlzl = 7; dvtlzl > 0; dvtlzl--) {
			continue;
		}
	}
	if (17570 == 17570) {
		int gi;
		for (gi = 25; gi > 0; gi--) {
			continue;
		}
	}
	return 8912;
}

double jaawdkb::lqmztowzpjnnhzca(int dlhlfwqdjeynix, double pubjt, double tmswqcvznlxuk, double vsndfncocxbyt) {
	string lbbztywoo = "emvpolbblyrqonpwvwkdrnlnkiqeebveenqtjvhkzmlifmkeueacnvbeqvitkdhjkudioolixfeeihdjucalu";
	if (string("emvpolbblyrqonpwvwkdrnlnkiqeebveenqtjvhkzmlifmkeueacnvbeqvitkdhjkudioolixfeeihdjucalu") != string("emvpolbblyrqonpwvwkdrnlnkiqeebveenqtjvhkzmlifmkeueacnvbeqvitkdhjkudioolixfeeihdjucalu")) {
		int hltm;
		for (hltm = 92; hltm > 0; hltm--) {
			continue;
		}
	}
	if (string("emvpolbblyrqonpwvwkdrnlnkiqeebveenqtjvhkzmlifmkeueacnvbeqvitkdhjkudioolixfeeihdjucalu") != string("emvpolbblyrqonpwvwkdrnlnkiqeebveenqtjvhkzmlifmkeueacnvbeqvitkdhjkudioolixfeeihdjucalu")) {
		int oxgjlmdcxn;
		for (oxgjlmdcxn = 72; oxgjlmdcxn > 0; oxgjlmdcxn--) {
			continue;
		}
	}
	return 21681;
}

void jaawdkb::yurldcyvmjg(double qhpawzjmsb, double dadpnzejuglez, string imhaaqffxsh, int jbsodmmmetxkrcj, bool htadhhkqrhkcy, string ykxgq, string xxstzhszjo, bool flarmljelegmmkg, string hxomklmla, string atdqusnmmlm) {
	string skjbhl = "zzpbxinmnwescqujsttocvcqxgzgpltthxssbbwnpzzgyzetvasvhxaqbtequoqwjaonvkbew";
	bool eiydwernilbx = true;
	double ahsgigwbxs = 1815;
	int jhngaxsszeq = 1092;
	double qazjanche = 16116;
	bool demvomeyhsk = false;
	bool cbhwqflsby = true;
	bool jpjpctdbqqq = false;

}

double jaawdkb::ijvxgiwdwiqafkrhhsjbbp() {
	int ptsxqkqqqyso = 1463;
	bool lvhnankw = true;
	bool rewilmdcwiamvof = true;
	bool fhoiieqqf = true;
	if (true != true) {
		int bclp;
		for (bclp = 13; bclp > 0; bclp--) {
			continue;
		}
	}
	if (1463 != 1463) {
		int ydoduejq;
		for (ydoduejq = 89; ydoduejq > 0; ydoduejq--) {
			continue;
		}
	}
	return 36696;
}

jaawdkb::jaawdkb() {
	this->ffvthmroghpb(54742, 2252, string("udrnrchgdqkxdvnqfglufgpjawonqhvgpohgymtthnzytzupnallyhkjtkvfzqaavhhbthyea"), true, 3316, 488, 1362, string("dxsirftkutorplnxaxzirjhlarlmbhbqsddjgvkpbcsdrfzedteqqwswxwlhzjaixpbxcwlqbimkuojdejaxnp"), 412, 1550);
	this->sumigqjfmbyzhhyknlxthdhrz(false, 2565, string("dupsctgffgsahxiksebxlzvcqcfklkckdrvhhyuknnjbwphxemwbqqybfqkxgykwldmsnjpahaunvdlkfnxxzxrpricaeplca"), string("rgyzoazjcrbtpxdzk"), 38330, 14361, true, false, 1443, true);
	this->iqrlcymgsvkqkbuilbd(true, false, 4836, true, false, 40835, 3577);
	this->dwqhfuiuiblntuhdjzawuj(string("tuotaomoxmppjevrbynwwabkrztwdbtzshelodzuz"), 2663, false, string("sduupaujyzanfswfxbibltkydvqzaeuqsil"), string("mewhhxncbavapbdbkhjaszuwuejduhbnblqdcgpjgivceqompzsplxw"));
	this->ahmhmnjdluompuztoutkja(1984, string("fifugyxujqvmzzixrfsszfrmohlfdkdcueajihvdanewmkmmowzbafqteifdjasrtmpuagpvdkv"), true, 5891, string("lyvishpelkdeczbiikklmusbjyjqhqhsdavsaifqevgmbitqunoqxsqhbsktngngpqjolgxwffzg"));
	this->uecfexlnvyruvyostsnml(313, true, true, false, string("effvunoxpoaonhofxxnlnzvwozzi"), 5160, 5150, true, 8623);
	this->lqmztowzpjnnhzca(373, 10044, 64011, 22511);
	this->yurldcyvmjg(6796, 14092, string("oljv"), 6686, true, string("zpllxopghiyduxigqlszxajkjpq"), string("heisxvxxdukkchq"), false, string("hnkxheytevpyfnnrtcptdjpgaodqcxdwpsszjmcbjpkxw"), string("ewsgbdklbhwesudfkceiaylhvrucjcuthkjbptrqnrjvacblzrypibkrq"));
	this->ijvxgiwdwiqafkrhhsjbbp();
	this->aaitprplmvlmkfy(string("mzvejylmqwlzgqmagfeansyzcmfogrgpiki"), 1189);
	this->mjzgqshfsybvlsoefnv(string("fqjolinhagzfebtwmuwnuxcj"), false, 548, true, string("wouabbiscszelgcfzluosidkgtdotllvfzuvwdoghiloetupkgiqvlck"), 91487, false, string("yksgwhvytd"));
	this->mmqlhnzcszzluptqt(15719, 85652, string("sjaiftesumnxsvgwddphjiupsytevbaejqdk"), true, string("mwaqivthcx"), 535);
	this->rgvuucnxlfaspnfykbe(string("jigpgoxeshrcbdhpwgevoto"));
	this->qhpgpuasqemvqmecpklorkw(string("xsfbystyd"), 44536, string("cipj"), false, string("lyubslicdjguwzksdttxwmhieevnctnhtnviqidxgeinrbikkatszuqvq"));
	this->lpbmzqpbbhyxi(string("ktrfdosovvo"), true, string("rtohxgj"), 631, string("twvoffzeklzlxfjsfofvvrdnfkdpryhiskb"), 56, 3357, true, 10878, string("mwfaeslof"));
	this->wxqyooigocvujcvn(string("coogdkiajelnrdyezpjfewdiwwykvphmxdsmwpqwbhptnklfywxoatneqfrdikafwqhl"), false, 735, true, 4126, false);
	this->qgpxexnwhi(false, string("qhglnfrpnfveyq"), 6952, false, string("nyovmaaxjek"), 5651, 20080, 26431);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class lzvvuxo {
public:
	string yslrrtpsxhpspsi;
	int fktawsxo;
	int apgmcazwaf;
	int edftxgtqzxc;
	int uzvpfwd;
	lzvvuxo();
	bool tkzwnazhifrhvnmacisdasox();
	double lrehtqvwuouq(double vnegjthpxihopt, bool svozadp, string zxfqbbsfkyygmiv, string jnnaviomshhy, int jvjoychljwxbpaj, string phjpvlucgbibk, string lzosj, int ecabxqbttgsl);
	int bkuahqyjffpbpg();
	double nkaaqgwwfansycpjpb(string fuyjiifpyrlp, string tcroebyyojhg, int fvcvkwxcfwsmiuu, bool cqwefdh, double qammdzqsh, string iluisjwxzblplkn);

protected:
	double neexoka;
	bool wbswfbwh;

	void qlmbgdbixjroja();
	bool kietxqescjfdkutsvyokskkgu(string novbabiogbpchbu, int xdlnbtabg, bool ufrszao, int kgpjtmtt, int nxfylcviexsda, int rswfuunnqnbu);
	int ruxllgxaptdytqoiwkh(int gzboge, string gxaswlaleer, bool iaibxgkpn, string humqctboqmapd, bool cnbbwrurkxnuf, int huiezetqjclstiu, double mjekogumfqw);
	double jnmaormiouy(double gboelbgjtrr, string hycevsvyemjn, string jbsvtjdhcqpfcil, string mmxdnzpabq, double hwpwgqxdnwcyv);
	bool qwhayfschxdvuycjsjebctc(string dffpwyiufhxhabo, bool euqsxyxjqzgati, double uydjoqvvebsmzcc, bool kwarhdpuxphveef, double grdirjgsk, double jafkgrzpcf, string lvfuvokxyiuna, int xlvfqbxfmhmch);
	void riwturswvgy(double zxkwvotocliv, bool ptsgewtvatnwbk, double rxlnzvmtlmk);
	double wluickwfdeyoqgfoew(string gieix, bool oloouwnkhhmj, double kpbqbfq, int vprmgrpggwdfp);
	int etxlkrttekvuskxibrmdmcf(double akcgke, double fultjhvztlg);
	void watodcpqcef(double jsqefzdumsryu, string qpynnlwuxxgii);

private:
	int vkpahrlt;
	double asyvmicfeejxzt;
	string vsgshel;
	int ghdtntrainwgefp;
	double hqzmyv;

	bool ewzybfqffhqpiisnugzwwi(int bwvku, string womzcynwnsgpvh, bool oavqidlmw, double otlheyo, int eyarjoctbldl, double wyndns, bool dzquhmgcxof, int kzffjfpiusda);
	void envdilpdcgtwjyk(bool vscpoxywi, string bdpejain, bool sjepntvw);
	void kvrjlsewtn(bool fubwztyip);
	string xewjfhexpcgowfjpb(bool xvjpjpqugvhqa, double qiqmyfucwkxs);
	void pkhgmelzxh(int illexlkiegxvg, double radsdcnexcf, bool fwkxd, bool jkaqsvlkgv, double akgrimpgejuci, bool oiqnq);
	bool abyucwzkndmwvgbpsct();
	int ocwouwpbkfam(bool naxzeqqxlrihyv, bool wrmrtcmuephm, double hbwzrqukeso, string iaios, bool hyfgpgrdrcqc, string aquxbf, double kfbur);
	int atxdupcicteyqgiowsi(double pszvbgepl, double tpuhd, double xsfppncs, double hyetwxu, bool fxykpt);
	bool siqwazixnqlg(bool zlrlxzguk, string biqziq, int dnwkznn, int udromvaxul, bool mxdtroumygmy, string plnzdjaesy);

};


bool lzvvuxo::ewzybfqffhqpiisnugzwwi(int bwvku, string womzcynwnsgpvh, bool oavqidlmw, double otlheyo, int eyarjoctbldl, double wyndns, bool dzquhmgcxof, int kzffjfpiusda) {
	double vohsxrgehyru = 16492;
	bool uiuqo = true;
	bool fxqbbl = false;
	string zphvgaypvlzmgi = "lnmbzjrnthrvxbuklhgoaiinrxsnfksvuwzgxftzzudsmaogbenibkcupeybctyuivhbtfgcfpntrvdojjvxnbibmcerxk";
	int ykfjcummhfumwak = 7023;
	double kvahbowonj = 30856;
	string fjkohlimjotknpn = "crarpwbiialzatltwfhgvanuggzdbfxpwowolhnxrkvvmtfsdtusqwmaplsaxtighhnpzdmqnrqqecshllzlqgovgftu";
	if (true != true) {
		int bq;
		for (bq = 92; bq > 0; bq--) {
			continue;
		}
	}
	if (true == true) {
		int rjofmh;
		for (rjofmh = 21; rjofmh > 0; rjofmh--) {
			continue;
		}
	}
	if (true != true) {
		int jdrmauqnsz;
		for (jdrmauqnsz = 15; jdrmauqnsz > 0; jdrmauqnsz--) {
			continue;
		}
	}
	return true;
}

void lzvvuxo::envdilpdcgtwjyk(bool vscpoxywi, string bdpejain, bool sjepntvw) {
	string xkdolfaiva = "ukvrymmjlyzfhyakggyzlsfsfnasvhxkpzldmnvhfglgnoiiibtsjdbr";
	int yzrvealbtehyh = 2293;
	if (2293 == 2293) {
		int yjwetxkexh;
		for (yjwetxkexh = 55; yjwetxkexh > 0; yjwetxkexh--) {
			continue;
		}
	}
	if (2293 == 2293) {
		int hqqjanvtra;
		for (hqqjanvtra = 76; hqqjanvtra > 0; hqqjanvtra--) {
			continue;
		}
	}
	if (string("ukvrymmjlyzfhyakggyzlsfsfnasvhxkpzldmnvhfglgnoiiibtsjdbr") != string("ukvrymmjlyzfhyakggyzlsfsfnasvhxkpzldmnvhfglgnoiiibtsjdbr")) {
		int jdiplm;
		for (jdiplm = 76; jdiplm > 0; jdiplm--) {
			continue;
		}
	}
	if (2293 != 2293) {
		int ryen;
		for (ryen = 1; ryen > 0; ryen--) {
			continue;
		}
	}

}

void lzvvuxo::kvrjlsewtn(bool fubwztyip) {
	double wpvdv = 68775;
	if (68775 == 68775) {
		int rqvlr;
		for (rqvlr = 15; rqvlr > 0; rqvlr--) {
			continue;
		}
	}
	if (68775 != 68775) {
		int qkxcv;
		for (qkxcv = 77; qkxcv > 0; qkxcv--) {
			continue;
		}
	}

}

string lzvvuxo::xewjfhexpcgowfjpb(bool xvjpjpqugvhqa, double qiqmyfucwkxs) {
	bool iyokyufetbvuwu = false;
	double kiqxcboioxfa = 37058;
	string psfeznkmqzujcl = "txvfzeneasjdurwicb";
	bool pgcywzhqzsigex = true;
	double vhwcrocghhdd = 84356;
	bool wehegzzvqnfwznp = false;
	int hlzgnzwrkvp = 5543;
	bool uhgvduqidgtn = true;
	string wrnacwhkfae = "txortlowtufyhfarmlditccptippvmfnkauvhmovkpchxaddalgqionzsouuefwhghqtidoxnwbqgetfbcmr";
	int fyuzz = 5508;
	return string("kwwxcprluzenkdenzi");
}

void lzvvuxo::pkhgmelzxh(int illexlkiegxvg, double radsdcnexcf, bool fwkxd, bool jkaqsvlkgv, double akgrimpgejuci, bool oiqnq) {
	bool dsagii = true;
	string abahta = "simtlrlxolfwjpbzbvwpbd";
	string xziptu = "lwabwfpcbamdisnalnocnvbbzarvngffepwxjerrnobfaqfsdlsxzpevvtjlzafsphcrtbcoptzejqvz";
	int qsauhzdidz = 6162;
	double kmgkagwm = 42916;
	if (42916 == 42916) {
		int ddpn;
		for (ddpn = 1; ddpn > 0; ddpn--) {
			continue;
		}
	}
	if (true != true) {
		int xf;
		for (xf = 89; xf > 0; xf--) {
			continue;
		}
	}
	if (string("simtlrlxolfwjpbzbvwpbd") != string("simtlrlxolfwjpbzbvwpbd")) {
		int mpkzqhpj;
		for (mpkzqhpj = 38; mpkzqhpj > 0; mpkzqhpj--) {
			continue;
		}
	}
	if (string("lwabwfpcbamdisnalnocnvbbzarvngffepwxjerrnobfaqfsdlsxzpevvtjlzafsphcrtbcoptzejqvz") != string("lwabwfpcbamdisnalnocnvbbzarvngffepwxjerrnobfaqfsdlsxzpevvtjlzafsphcrtbcoptzejqvz")) {
		int cgvkik;
		for (cgvkik = 5; cgvkik > 0; cgvkik--) {
			continue;
		}
	}

}

bool lzvvuxo::abyucwzkndmwvgbpsct() {
	int eqtdquw = 4522;
	bool bzafl = true;
	double qpcgrnxiklcgif = 39223;
	double gcpetc = 88084;
	string javxz = "uxyloabykwmraedvxgubpeulrppvnuqsamrfmzuueqzdfvdxozgisklnkreskggiamjaixhyqmzaaddeaforokpwothyqdyjhmio";
	if (39223 != 39223) {
		int dqqqif;
		for (dqqqif = 38; dqqqif > 0; dqqqif--) {
			continue;
		}
	}
	return true;
}

int lzvvuxo::ocwouwpbkfam(bool naxzeqqxlrihyv, bool wrmrtcmuephm, double hbwzrqukeso, string iaios, bool hyfgpgrdrcqc, string aquxbf, double kfbur) {
	return 4187;
}

int lzvvuxo::atxdupcicteyqgiowsi(double pszvbgepl, double tpuhd, double xsfppncs, double hyetwxu, bool fxykpt) {
	double vvtqtezakcvo = 26180;
	string qjivkf = "wcobnwtqtiytankhxxsqhngdgpglhqdnlwxstayzxrvzshnvypqbcyustczyfyrjyo";
	string pawqyxrmhntdvgt = "guvrtvbkjympq";
	string yranwhkoj = "mkhlpgrrbdfwpkbbywjrmemcjqlaquznqetfsvseojjnpnwctgydeovymslrdrw";
	bool lkijqraakyyxec = true;
	int ycinvvt = 2976;
	bool oqjoj = true;
	double qobgq = 2031;
	int wmtvgjookobt = 2155;
	if (2155 == 2155) {
		int lz;
		for (lz = 19; lz > 0; lz--) {
			continue;
		}
	}
	if (string("wcobnwtqtiytankhxxsqhngdgpglhqdnlwxstayzxrvzshnvypqbcyustczyfyrjyo") == string("wcobnwtqtiytankhxxsqhngdgpglhqdnlwxstayzxrvzshnvypqbcyustczyfyrjyo")) {
		int sppbcprdk;
		for (sppbcprdk = 3; sppbcprdk > 0; sppbcprdk--) {
			continue;
		}
	}
	if (2976 == 2976) {
		int nltq;
		for (nltq = 23; nltq > 0; nltq--) {
			continue;
		}
	}
	if (2155 != 2155) {
		int uvcrij;
		for (uvcrij = 28; uvcrij > 0; uvcrij--) {
			continue;
		}
	}
	if (string("mkhlpgrrbdfwpkbbywjrmemcjqlaquznqetfsvseojjnpnwctgydeovymslrdrw") != string("mkhlpgrrbdfwpkbbywjrmemcjqlaquznqetfsvseojjnpnwctgydeovymslrdrw")) {
		int gzwghigt;
		for (gzwghigt = 16; gzwghigt > 0; gzwghigt--) {
			continue;
		}
	}
	return 44034;
}

bool lzvvuxo::siqwazixnqlg(bool zlrlxzguk, string biqziq, int dnwkznn, int udromvaxul, bool mxdtroumygmy, string plnzdjaesy) {
	string kjjmu = "lgntcowvqdlaibltjbynejgaaf";
	double zvgnp = 21331;
	return true;
}

void lzvvuxo::qlmbgdbixjroja() {
	string tjrqqjcnsh = "jswgzfsqegwlgkylvhlwyqtacxthfnzmozylgzjgl";
	if (string("jswgzfsqegwlgkylvhlwyqtacxthfnzmozylgzjgl") != string("jswgzfsqegwlgkylvhlwyqtacxthfnzmozylgzjgl")) {
		int pbsfxsqr;
		for (pbsfxsqr = 48; pbsfxsqr > 0; pbsfxsqr--) {
			continue;
		}
	}
	if (string("jswgzfsqegwlgkylvhlwyqtacxthfnzmozylgzjgl") != string("jswgzfsqegwlgkylvhlwyqtacxthfnzmozylgzjgl")) {
		int odktmceib;
		for (odktmceib = 94; odktmceib > 0; odktmceib--) {
			continue;
		}
	}

}

bool lzvvuxo::kietxqescjfdkutsvyokskkgu(string novbabiogbpchbu, int xdlnbtabg, bool ufrszao, int kgpjtmtt, int nxfylcviexsda, int rswfuunnqnbu) {
	int rleejbubv = 1872;
	string ggqlpdqrehdqklt = "unlnbgtf";
	int imfreuuolhoyq = 3055;
	if (1872 == 1872) {
		int zfd;
		for (zfd = 38; zfd > 0; zfd--) {
			continue;
		}
	}
	if (string("unlnbgtf") != string("unlnbgtf")) {
		int ouxseqwj;
		for (ouxseqwj = 59; ouxseqwj > 0; ouxseqwj--) {
			continue;
		}
	}
	return false;
}

int lzvvuxo::ruxllgxaptdytqoiwkh(int gzboge, string gxaswlaleer, bool iaibxgkpn, string humqctboqmapd, bool cnbbwrurkxnuf, int huiezetqjclstiu, double mjekogumfqw) {
	double tpyjzlx = 24590;
	double yzoiwsntdqqv = 2101;
	string fnwyzm = "sdcdbvizmmgheomjnianaoyzhxfowkxpw";
	int llxvazhkkevx = 5977;
	int sceraahdbnbpxx = 2980;
	return 34364;
}

double lzvvuxo::jnmaormiouy(double gboelbgjtrr, string hycevsvyemjn, string jbsvtjdhcqpfcil, string mmxdnzpabq, double hwpwgqxdnwcyv) {
	int nlwypiqfwnvx = 4056;
	int qgofmisxjqxb = 1265;
	if (1265 != 1265) {
		int xnvrttha;
		for (xnvrttha = 10; xnvrttha > 0; xnvrttha--) {
			continue;
		}
	}
	if (4056 == 4056) {
		int wcebpmhs;
		for (wcebpmhs = 93; wcebpmhs > 0; wcebpmhs--) {
			continue;
		}
	}
	if (1265 != 1265) {
		int fgiuvif;
		for (fgiuvif = 22; fgiuvif > 0; fgiuvif--) {
			continue;
		}
	}
	if (1265 != 1265) {
		int pr;
		for (pr = 68; pr > 0; pr--) {
			continue;
		}
	}
	if (4056 != 4056) {
		int sswk;
		for (sswk = 75; sswk > 0; sswk--) {
			continue;
		}
	}
	return 46608;
}

bool lzvvuxo::qwhayfschxdvuycjsjebctc(string dffpwyiufhxhabo, bool euqsxyxjqzgati, double uydjoqvvebsmzcc, bool kwarhdpuxphveef, double grdirjgsk, double jafkgrzpcf, string lvfuvokxyiuna, int xlvfqbxfmhmch) {
	string qazkngijqocwx = "fmdyneijvxvifreipha";
	if (string("fmdyneijvxvifreipha") != string("fmdyneijvxvifreipha")) {
		int xdnjmvac;
		for (xdnjmvac = 48; xdnjmvac > 0; xdnjmvac--) {
			continue;
		}
	}
	if (string("fmdyneijvxvifreipha") != string("fmdyneijvxvifreipha")) {
		int ggi;
		for (ggi = 53; ggi > 0; ggi--) {
			continue;
		}
	}
	if (string("fmdyneijvxvifreipha") != string("fmdyneijvxvifreipha")) {
		int evh;
		for (evh = 33; evh > 0; evh--) {
			continue;
		}
	}
	if (string("fmdyneijvxvifreipha") != string("fmdyneijvxvifreipha")) {
		int lvkbpwb;
		for (lvkbpwb = 71; lvkbpwb > 0; lvkbpwb--) {
			continue;
		}
	}
	if (string("fmdyneijvxvifreipha") == string("fmdyneijvxvifreipha")) {
		int ptnisqekgg;
		for (ptnisqekgg = 24; ptnisqekgg > 0; ptnisqekgg--) {
			continue;
		}
	}
	return true;
}

void lzvvuxo::riwturswvgy(double zxkwvotocliv, bool ptsgewtvatnwbk, double rxlnzvmtlmk) {

}

double lzvvuxo::wluickwfdeyoqgfoew(string gieix, bool oloouwnkhhmj, double kpbqbfq, int vprmgrpggwdfp) {
	double dclysougjrup = 2166;
	double dtfwieno = 24802;
	int hdcnkuqeecz = 6103;
	double ekccjo = 43982;
	double nkkdip = 25391;
	int xzcniqlcfmfy = 1342;
	int gbxoc = 5520;
	double fwffltjapf = 70929;
	if (24802 != 24802) {
		int kvkhmm;
		for (kvkhmm = 55; kvkhmm > 0; kvkhmm--) {
			continue;
		}
	}
	return 10820;
}

int lzvvuxo::etxlkrttekvuskxibrmdmcf(double akcgke, double fultjhvztlg) {
	int srnqsclmim = 3936;
	int redwmgnczu = 2724;
	int vkbopxie = 5724;
	int mmkwdbiin = 3227;
	bool dpejrqv = false;
	string dsbptjt = "ugkrrrudpsiaphoccnvtgfotm";
	double fwuiankrerb = 55430;
	double bdksjtavxhfckv = 7290;
	if (3227 == 3227) {
		int pk;
		for (pk = 79; pk > 0; pk--) {
			continue;
		}
	}
	if (2724 != 2724) {
		int chyluskrgy;
		for (chyluskrgy = 28; chyluskrgy > 0; chyluskrgy--) {
			continue;
		}
	}
	if (7290 != 7290) {
		int amxs;
		for (amxs = 62; amxs > 0; amxs--) {
			continue;
		}
	}
	return 58135;
}

void lzvvuxo::watodcpqcef(double jsqefzdumsryu, string qpynnlwuxxgii) {
	double udiqolgeo = 23052;
	string kzkjheihvjnbvrk = "crfnwlppwraltummnnnxmpsvdrey";
	string oxyrftmtsdvsv = "ryeyazmyfxwjbmrtfxnyijefadcqnm";
	bool nfaie = false;
	double lrrdtplyz = 25437;
	if (25437 != 25437) {
		int yofhm;
		for (yofhm = 79; yofhm > 0; yofhm--) {
			continue;
		}
	}

}

bool lzvvuxo::tkzwnazhifrhvnmacisdasox() {
	bool fncijkoactb = false;
	int kdelpyqb = 5041;
	double mracu = 41381;
	bool nnyqzyhwsud = true;
	double zgcylqb = 33540;
	string subzzzxlrdw = "srodxoaqlcdjabwwqwayjawzxunkcaohzvejgwrqfcfhmjtlewvgkyxpgjtbwdqm";
	double huyfimxkj = 49326;
	string qcggyjjxgbyp = "oecrtzrovbujwcbefvsevlujvuvotvocgmraqconc";
	string mfvjcyyadjbxlh = "wybnpkvmwzlnrbeggwxhzubbmivavfbmfxawpbgysgtbnazlrnjhuyxoeunwwmjiornzyykuqezgnjvk";
	bool jdtjiwiskfqhyb = true;
	if (5041 != 5041) {
		int ykfrnytudr;
		for (ykfrnytudr = 90; ykfrnytudr > 0; ykfrnytudr--) {
			continue;
		}
	}
	if (false == false) {
		int rj;
		for (rj = 86; rj > 0; rj--) {
			continue;
		}
	}
	if (49326 != 49326) {
		int zjpqdvq;
		for (zjpqdvq = 39; zjpqdvq > 0; zjpqdvq--) {
			continue;
		}
	}
	return false;
}

double lzvvuxo::lrehtqvwuouq(double vnegjthpxihopt, bool svozadp, string zxfqbbsfkyygmiv, string jnnaviomshhy, int jvjoychljwxbpaj, string phjpvlucgbibk, string lzosj, int ecabxqbttgsl) {
	double bfhuosuhchv = 27820;
	if (27820 == 27820) {
		int xdooioxhb;
		for (xdooioxhb = 93; xdooioxhb > 0; xdooioxhb--) {
			continue;
		}
	}
	if (27820 != 27820) {
		int alvxv;
		for (alvxv = 8; alvxv > 0; alvxv--) {
			continue;
		}
	}
	return 69372;
}

int lzvvuxo::bkuahqyjffpbpg() {
	double imatwriiweoe = 47576;
	string hxfceb = "eolbfawspjpvowveynmdktlneqdogstmirnllxzhgljdtcjfdydjj";
	bool ceqdatbfkeka = false;
	int ndxjdeujebhfb = 2960;
	string fhjvudkcwt = "jxtlrcamcigyxppnivilcapsuqyhcbqofcgnwrontjbckbthquttvacvnidxvnxlmyzeltyizhrsrtgdpwglmovrprslhyj";
	bool rjywghpgbkuo = false;
	if (2960 == 2960) {
		int lhmcci;
		for (lhmcci = 94; lhmcci > 0; lhmcci--) {
			continue;
		}
	}
	if (string("jxtlrcamcigyxppnivilcapsuqyhcbqofcgnwrontjbckbthquttvacvnidxvnxlmyzeltyizhrsrtgdpwglmovrprslhyj") == string("jxtlrcamcigyxppnivilcapsuqyhcbqofcgnwrontjbckbthquttvacvnidxvnxlmyzeltyizhrsrtgdpwglmovrprslhyj")) {
		int qvomyg;
		for (qvomyg = 11; qvomyg > 0; qvomyg--) {
			continue;
		}
	}
	if (2960 != 2960) {
		int zqy;
		for (zqy = 58; zqy > 0; zqy--) {
			continue;
		}
	}
	if (false == false) {
		int vghzysq;
		for (vghzysq = 80; vghzysq > 0; vghzysq--) {
			continue;
		}
	}
	if (false == false) {
		int fjhbu;
		for (fjhbu = 75; fjhbu > 0; fjhbu--) {
			continue;
		}
	}
	return 67882;
}

double lzvvuxo::nkaaqgwwfansycpjpb(string fuyjiifpyrlp, string tcroebyyojhg, int fvcvkwxcfwsmiuu, bool cqwefdh, double qammdzqsh, string iluisjwxzblplkn) {
	bool sklumkfwxissuan = false;
	double rcmrqswhq = 10787;
	double vcfkvura = 60166;
	double neenvhysf = 4671;
	int byatbu = 504;
	double rspmwqmkyretk = 22751;
	if (false == false) {
		int jejblglqll;
		for (jejblglqll = 3; jejblglqll > 0; jejblglqll--) {
			continue;
		}
	}
	if (4671 != 4671) {
		int uckiga;
		for (uckiga = 10; uckiga > 0; uckiga--) {
			continue;
		}
	}
	return 60165;
}

lzvvuxo::lzvvuxo() {
	this->tkzwnazhifrhvnmacisdasox();
	this->lrehtqvwuouq(8897, false, string("ufwsgcbfvftfdrdoaupmae"), string("oggrbgpzmwwvcrefpbedqjgufqahuuatggfhleygwygtdspxguznxpypolwognxtapdrnuzgynhivzhefzdeuxlbvjtbxyepg"), 798, string("fgv"), string("ssuzxrqogfhgypjzgeapeyolqilxkbeixscatrrdzijbdhdrvifjrxdbxipdpnjxaaptitfegdbgrnemjfa"), 503);
	this->bkuahqyjffpbpg();
	this->nkaaqgwwfansycpjpb(string("zcgnhccgmpantquazamzpjhxbytibymjijyaynbexgkfgcttqpcgtvifqnatowbqm"), string("ofmybbiaztshkhsilkrmsxomhbtidzcltmkriyxjmdjdfowypsatccfyrviq"), 3277, false, 8087, string("ynaheoocufnrocnpeukahnpknikxwaynbtbibfgrchdenoaxfawpkq"));
	this->qlmbgdbixjroja();
	this->kietxqescjfdkutsvyokskkgu(string("tphekyzgzyakrjwawinptolkvpalqhiamjtbbfwdrhchufxfkgvrqeajqideuwmnympalxozathvhhhnkwuitwksfujhgxhj"), 2172, true, 4254, 6291, 526);
	this->ruxllgxaptdytqoiwkh(3187, string("mvmqtiuzztovvsygufxmjrsiyibtaiaaabtterhpfwpfjcojcxuuoavqydqzjcqfulfrfkaqcuuuwrjygk"), true, string("yvutnwefcwrnnsxilcpzpoexqipopbkkgjnara"), true, 381, 77230);
	this->jnmaormiouy(44407, string("lxbhsbdoznxveetawvtdjazljxojsvbnbpqszkfvsriezdfugcikdghsyvogkdooiaxbvxtx"), string("sxllasxgvrkemhxapnhwqylbyhtvbaetqbeudfuovpzdktcag"), string("pmcuczqpcbjbshxdqozauljxojregmybbntihmpnldsyeixdzx"), 54949);
	this->qwhayfschxdvuycjsjebctc(string("luzwmckbkukwjgazpgrawul"), false, 60163, true, 7686, 6396, string("qduyjilorkryopndkausszticpjmrboulhuytmqfqvjxzpsbputxlrxdzdeosxhbgzghjssubhzffewsnuuognrrylqkbgxja"), 765);
	this->riwturswvgy(23018, false, 5140);
	this->wluickwfdeyoqgfoew(string("eyqkfennhgqhodqrktclmxvqggnkedghrzektidclfczqdjpxlzvpkpt"), true, 63531, 1502);
	this->etxlkrttekvuskxibrmdmcf(42352, 12639);
	this->watodcpqcef(10360, string("tfcex"));
	this->ewzybfqffhqpiisnugzwwi(759, string("lgorwhqqiwutzttzaqvwolnvzzohzxpwuoatamlhnamf"), true, 11843, 39, 81558, false, 1415);
	this->envdilpdcgtwjyk(false, string("fpiowyewxbkkdjasbgnqisncriqkngnmnrj"), true);
	this->kvrjlsewtn(true);
	this->xewjfhexpcgowfjpb(true, 25719);
	this->pkhgmelzxh(8293, 10138, false, false, 4659, false);
	this->abyucwzkndmwvgbpsct();
	this->ocwouwpbkfam(true, false, 32194, string("xhpozprdyhgnjdspqdpljarhgmfleermamqvpshehxffxlxpviir"), true, string("wzhxlqirnwqw"), 3704);
	this->atxdupcicteyqgiowsi(31506, 27720, 8136, 30377, false);
	this->siqwazixnqlg(false, string(""), 1763, 3499, false, string("zrlnquadpcmxszsgftsccksalmd"));
}
