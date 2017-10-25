/*
Rest In Peace ApocalypseCheats
*/

#define _CRT_SECURE_NO_WARNINGS

#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include <stdio.h>
#include "string.h"
#include <wchar.h>
#include <time.h>
//#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"
#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "Resolver.h"
#include "RageBot.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

Vector AutoStrafeView;

void CMiscHacks::Init()
{
	// Any init
}

void CMiscHacks::Draw()
{
	// Any drawing	
	// Spams
	switch (Menu::Window.MiscTab.OtherChatSpam.GetIndex())
	{
	case 0:
		// No Chat Spam
		break;
	case 1:
		// Namestealer
		ChatSpamName();
		break;
	case 2:
		// Apocalypse - chat spam
		ChatSpamRegular();
		break;
	case 3:
		// Apocalypse - name spam
		ChatSpamInterwebz();
		break;
	case 4:
		// Rekt
		ChatSpamDisperseName();
		break;
	case 5:
		//Aimware ChatSpam
		ChatSpamAimwareChat();
	case 6:
		//AimwareNameSpam
		ChatSpamAimwareName();
	}

	switch (Menu::Window.CustomTab.NameChangerText.GetIndex())
	{
	case 0:
		break;
	case 1:
		AyyBgone();
		break;
	case 2:
		NoName();
		break;
	case 3:
		NameSteal();
		break;
	case 4:
		NovaHook();
		break;
	case 5:
		Aimware();
		break;
	case 6:
		SteamSupport();
		break;

	}


/*	int xd = Menu::Window.MiscTab.OtherChatSpam.GetIndex();

	if (xd=3)
	{
		NoName();
	}*/
}

void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	// Any Move Stuff
	
	// Bhop
	bool bhop = Menu::Window.MiscTab.OtherAutoJump.GetState();
			AutoJump(pCmd);


   /* if (Menu::Window.MiscTab.OtherAutoCounterStafe.GetState())
			{
				int Key = Menu::Window.MiscTab.OtherAutoCounterStafeKey.GetKey();
				if (Key >= 0 && !GUI.GetKeyState(Key))
				{
					TargetID = -1;
					pTarget = nullptr;
					HitBox = -1;
					return;
				}
			}*/
	

	// AutoStrafe
	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	switch (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex())
	{
	case 0:
		// Off
		break;
	case 1:
		LegitStrafe(pCmd);
		break;
	case 2:
		RageStrafe(pCmd);
		break;
	}

	//Fake Lag
	if (Menu::Window.MiscTab.FakeLagEnable.GetState())
		Fakelag(pCmd, bSendPacket);

	if (Menu::Window.VisualsTab.DisablePostProcess.GetState())
		PostProcess();
}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm 
	{
		push    ebp
			mov     ebp, esp
			and     esp, 0FFFFFFF8h
			sub     esp, 44h
			push    ebx
			push    esi
			push    edi
			mov     edi, cvar
			mov     esi, value
			jmp     pfn
	}
}
void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	if (setaddr != 0) 
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}

void change_name(const char* name)
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		NET_SetConVar(name, "name");
}


void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	if (Menu::Window.MiscTab.OtherAutoJump.GetState())
	{
		if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
		{
			int iFlags = hackManager.pLocal()->GetFlags();
			if (!(iFlags & FL_ONGROUND))
				pCmd->buttons &= ~IN_JUMP;

			if (hackManager.pLocal()->GetVelocity().Length() <= 50)
			{
				pCmd->forwardmove = 450.f;
			}
		}
	}
}

void CMiscHacks::HAutoJump(CUserCmd *pCmd)
{
	if (Menu::Window.MiscTab.OtherHAutoJump.GetState())
	{
		if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
		{
			int iFlags = hackManager.pLocal()->GetFlags();
			if (!(iFlags & FL_ONGROUND))
				//if ()
				pCmd->buttons &= ~IN_JUMP;

			if (hackManager.pLocal()->GetVelocity().Length() <= 50)
			{
				pCmd->forwardmove = 450.f;
			}
		}
	}
}

void CMiscHacks::LegitStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (!(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = 0.0f;

		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -450.0f;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 450.0f;
		}
	}
}

void CMiscHacks::RageStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	bool bKeysPressed = true;
	if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44)) bKeysPressed = false;

	if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
	{
		if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
			pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
		}
		else {
			pCmd->forwardmove = (1800.f * 4.f) / pLocal->GetVelocity().Length2D();
			pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
			if (pCmd->forwardmove > 450.f)
				pCmd->forwardmove = 450.f;
		}
	}

/*	IClientEntity* pLocal = hackManager.pLocal();
	static bool bDirection = true;


	static float move = 450; //400.f; // move = max(move, (abs(cmd->move.x) + abs(cmd->move.y)) * 0.5f);
	float s_move = move * 0.5065f;


	if ((pCmd->buttons & IN_JUMP) || !(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = move * 0.015f;
		pCmd->sidemove += (float)(((pCmd->tick_count % 2) * 2) - 1) * s_move;

		if (pCmd->mousedx)
			pCmd->sidemove = (float)clamp(pCmd->mousedx, -1, 1) * s_move;

		static float strafe = pCmd->viewangles.y;

		float rt = pCmd->viewangles.y, rotation;
		rotation = strafe - rt;

		if (rotation < FloatNegate(Interfaces::Globals->interval_per_tick))
			pCmd->sidemove = -s_move;

		if (rotation > Interfaces::Globals->interval_per_tick)
			pCmd->sidemove = s_move;

		strafe = rt;
	} */
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

// …e Õ½Ê¿
void CMiscHacks::ChatSpamInterwebz()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;
	//static std::string nameBackup = "INTERWEBZ";

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name ("\xe1\xab\xa8 ApocalypseCheats.xyz");
			useSpace = !useSpace;
		}
		else
		{
			change_name("\xe1\xab\xa5 ApocalypseCheats.xyz");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}




void CMiscHacks::ChatSpamDisperseName()
{
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		SayInTeamChat("u can stay afk, i got this");
	}
	else
	{
		SayInChat("Get ur public trash and private pasta DESTOYED");
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamName()
{
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		if (entity && hackManager.pLocal()->GetTeamNum() == entity->GetTeamNum() && entity != hackManager.pLocal())
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			// If entity is a player
			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.name, "GOTV"))
						Names.push_back(pInfo.name);
				}
			}
		}
	}

	static bool wasSpamming = true;
	//static std::string nameBackup = "INTERWEBZ.CC";

	int randomIndex = rand() % Names.size();
	char buffer[128];
	sprintf_s(buffer, "%s ", Names[randomIndex].c_str());

	if (wasSpamming)
	{
		change_name(buffer);
	}
	else
	{
		change_name ("p$i 1337");
	}

	start_t = clock();
}


void CMiscHacks::ChatSpamRegular()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		SayInTeamChat("Boost service by ApocalypseCheats.xyz");
	}
	else
	{
		SayInChat("Derank service by ApocalypseCheats.xyz");
	}

	start_t = clock();
}
void CMiscHacks::ChatSpamAimwareChat()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	static bool holzed = true;
	SayInChat("www.AIMWARE.net | Premium CSGO Cheat﻿");


	start_t = clock();
}
void CMiscHacks::ChatSpamAimwareName()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;
	//static std::string nameBackup = "INTERWEBZ";

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			
			const char  *x = "\xe1\xab\xa8 AIMWARE.net";
		
			change_name(x);
			
			useSpace = !useSpace;
		}
		else
		{
			
			const char  *y = "\xe1\xab\xa5 AIMWARE.net";
			
			change_name(y);
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

// Name changers
void CMiscHacks::NoName()
{
	change_name("\n\n\n\n--\n­­­");
}

void CMiscHacks::AyyBgone()
{
	change_name("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||");
}

void CMiscHacks::NovaHook()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name(" Nova Hook");
			useSpace = !useSpace;
		}
		else
		{
			change_name("Nova Hook ");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

void CMiscHacks::Aimware()
{
	change_name("AIMWARE.net");
}

void CMiscHacks::SteamSupport()
{
	change_name("Steam Support");
}

void CMiscHacks::Namespam()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;
}

void CMiscHacks::NameSteal()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{

		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;

		if (entity && hackManager.pLocal()->GetTeamNum() == entity->GetTeamNum() && entity != hackManager.pLocal())
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.name, "GOTV"))
						Names.push_back(pInfo.name);
				}
			}
		}
	}

	static bool wasSpamming = true;

	int randomIndex = rand() % Names.size();
	char buffer[128];
	sprintf_s(buffer, "%s ", Names[randomIndex].c_str());

	if (wasSpamming)
	{
		change_name(buffer);
	}
	else
	{
		change_name("p$i 1337");
	}

	start_t = clock();
}






void CMiscHacks::PostProcess()
{
	ConVar* Meme = Interfaces::CVar->FindVar("mat_postprocess_enable");
	SpoofedConvar* meme_spoofed = new SpoofedConvar(Meme);
	meme_spoofed->SetString("mat_postprocess_enable 0");
}

void CMiscHacks::Fakelag(CUserCmd *pCmd, bool &bSendPacket)
{
	int iChoke = Menu::Window.MiscTab.FakeLagChoke.GetValue();

	static int iFakeLag = -1;
	iFakeLag++;

	if (iFakeLag <= iChoke && iFakeLag > -1)
	{
		bSendPacket = false;
	}
	else
	{
		bSendPacket = true;
		iFakeLag = -1;
	}
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class aaviysz {
public:
	double ewsjxposldmkl;
	string sirrddysxjypz;
	aaviysz();
	string zimxdsfwygxcnihtpjwl(bool opjnjiuqjjzzef, int xvpumgaqbhrglh, bool fleliwcvkheqv, double esaqif, int cbnbqzpmobjf, double zjwpye, string tqicvsqdrpejgeu);

protected:
	string kcjcoueichn;
	int nbsaeozgkm;

	double ueqijbogota(int udfnqahvo, int ofvmc);
	bool bhgkiqiworltzzlk(string lfeqyzymo, string hhdqeljt, bool dmbdrubjhacmj, bool enbyatdgswa, double lxqusqnbzzhu);
	bool dbuuqlredscjhfxicvizkulpy(bool fypmgazrs);
	string qzxbnyqkniwcgsizmcdr(string klcasfuk);
	bool vbscgahszdvnkrkvlfmyw(double bjniiuhcfyy);
	string ilntsbxfcnyl(bool zflygf, bool ejrvykyij, double rnwuutm, double qljnmaceb, bool dsrgaubpk, double hvxupoflsun, int tuqxefeimyqiag, double ordybyjsun);
	bool rguojsfdhawugzzrwmgblnl(int jajdvge);
	string czdgtrijvbmmfykoisuyh(string pptqdisyxd, bool qzrjqnhl);

private:
	bool cxeyrvmwckkh;

	double vhasdfsscusc(bool oruxntpinmvynad, double jkbqxdzuqmenqza);
	bool rjtqoaotrmjscvjoelwbvghlp(int pvlcyr, double jlkjpj, double dtopuv);
	string aekakrldsjxujkhjivqqw(bool rhaxvejrkelu, int fvekvr);
	string mssrvffgryw(int wkaufrszxhgu, string dcsqwootgrzarwi, string krvvexbtjso, string jouscsoiadc, int simpgu, double tiibzvdewl, double mqordmxpfob);
	bool oblgnvkpprcmik(string jhkkezrqqdytbj, bool frzow, double dmjouj, double cxoprsey, string xhsdmymxj, int xaqsryfomsiqirj, string pxtgtandwht, string rykldnzforeobcb, int njrlzmhcto);
	void hyowmsmflaqglajwyzausws(bool czfbxaedi, bool tkhakekgvt);

};


double aaviysz::vhasdfsscusc(bool oruxntpinmvynad, double jkbqxdzuqmenqza) {
	double enpafwhmxxyfp = 29167;
	string ygjvmg = "sxhtilofsgvijseyusjisemipkhrnzjrnuflmrsqruhhfyechfbtzorfvxsezahrnevncgeiwcvslkk";
	bool dvuqo = true;
	double vlewhbgtnjxye = 29797;
	int flktp = 3704;
	string uuomgebzvpwrtez = "nowrvoblpfsvhccbbpnvhyaeikbtvvvayzjjzoyqbquzvqcxkymvivllselsirnhtjgmwerscumzlpqnkabqgzuhnewardqlzbrs";
	bool gdyoeiygkft = true;
	string wuqxjlqrqfda = "rkygvvoxbxkxvcysyhnrjxtzdyvwgjbifbqbppethvgbabxauywdyhjoffstletnddrcliwcndoumgtbvvl";
	bool cempdigjusxqak = true;
	if (3704 != 3704) {
		int xy;
		for (xy = 19; xy > 0; xy--) {
			continue;
		}
	}
	return 93315;
}

bool aaviysz::rjtqoaotrmjscvjoelwbvghlp(int pvlcyr, double jlkjpj, double dtopuv) {
	int ipxkobniocb = 290;
	double vommegghr = 32504;
	double mpwogf = 7641;
	bool dhgoquc = false;
	bool ngesetiairataag = false;
	int qeawmg = 7207;
	bool khdxixqveozneq = true;
	double fpuxslsjqwivbwi = 63022;
	bool edbwpxvrrvgvch = false;
	bool pvuoiadofn = true;
	if (true == true) {
		int ungp;
		for (ungp = 93; ungp > 0; ungp--) {
			continue;
		}
	}
	if (true != true) {
		int gbx;
		for (gbx = 34; gbx > 0; gbx--) {
			continue;
		}
	}
	if (true != true) {
		int gvghccda;
		for (gvghccda = 11; gvghccda > 0; gvghccda--) {
			continue;
		}
	}
	if (true == true) {
		int vstqmlgudd;
		for (vstqmlgudd = 82; vstqmlgudd > 0; vstqmlgudd--) {
			continue;
		}
	}
	return false;
}

string aaviysz::aekakrldsjxujkhjivqqw(bool rhaxvejrkelu, int fvekvr) {
	bool cwixswvovckjw = false;
	bool wiankjeyhiqyqlj = false;
	if (false == false) {
		int cx;
		for (cx = 38; cx > 0; cx--) {
			continue;
		}
	}
	if (false != false) {
		int ywtkukhafz;
		for (ywtkukhafz = 63; ywtkukhafz > 0; ywtkukhafz--) {
			continue;
		}
	}
	if (false != false) {
		int rk;
		for (rk = 56; rk > 0; rk--) {
			continue;
		}
	}
	return string("m");
}

string aaviysz::mssrvffgryw(int wkaufrszxhgu, string dcsqwootgrzarwi, string krvvexbtjso, string jouscsoiadc, int simpgu, double tiibzvdewl, double mqordmxpfob) {
	int ngmkzn = 4304;
	bool wwiyfe = true;
	string zeimusyuhul = "pgqovjcmtleoxmkdudfwjaxllglyjizwfzezkwuvsdzqlxrnscngvnnffphmljosflukuqjttbnmdolsimrgckpbcjasbxvoyfki";
	double xjzlvmfgxw = 9348;
	double kpeynmys = 50263;
	string qhnjrfvuld = "fsddfqaoycpubeabtqymqpaeebrzatkiyecptedrd";
	int hynxinykwhyj = 3499;
	return string("ydzxlqsdsyerium");
}

bool aaviysz::oblgnvkpprcmik(string jhkkezrqqdytbj, bool frzow, double dmjouj, double cxoprsey, string xhsdmymxj, int xaqsryfomsiqirj, string pxtgtandwht, string rykldnzforeobcb, int njrlzmhcto) {
	int vmadjo = 129;
	int tdawipes = 2139;
	int kxkpu = 6553;
	bool cvudksuiald = false;
	string owogj = "hsdutbxphlixhvnemvnosktdcaqisgeftzhhbzhzrgkfjyfcpsjphbavqxhnfiyinsxnpzivohlxkkwrhopr";
	int mhqnvrijyetnygq = 503;
	int dukuttpilbz = 2026;
	double pvfnjyykymrj = 25316;
	if (25316 != 25316) {
		int qxd;
		for (qxd = 46; qxd > 0; qxd--) {
			continue;
		}
	}
	if (503 == 503) {
		int hol;
		for (hol = 88; hol > 0; hol--) {
			continue;
		}
	}
	if (string("hsdutbxphlixhvnemvnosktdcaqisgeftzhhbzhzrgkfjyfcpsjphbavqxhnfiyinsxnpzivohlxkkwrhopr") == string("hsdutbxphlixhvnemvnosktdcaqisgeftzhhbzhzrgkfjyfcpsjphbavqxhnfiyinsxnpzivohlxkkwrhopr")) {
		int buybaysdag;
		for (buybaysdag = 41; buybaysdag > 0; buybaysdag--) {
			continue;
		}
	}
	return false;
}

void aaviysz::hyowmsmflaqglajwyzausws(bool czfbxaedi, bool tkhakekgvt) {
	bool xfrrnbdlpirom = false;
	string mawmvwchrf = "eixwwfiglylbstjachcvnacphnodzrgbstfi";
	if (false != false) {
		int eiqqvdlp;
		for (eiqqvdlp = 47; eiqqvdlp > 0; eiqqvdlp--) {
			continue;
		}
	}
	if (string("eixwwfiglylbstjachcvnacphnodzrgbstfi") == string("eixwwfiglylbstjachcvnacphnodzrgbstfi")) {
		int hfcoc;
		for (hfcoc = 22; hfcoc > 0; hfcoc--) {
			continue;
		}
	}
	if (false == false) {
		int vtrppwmqed;
		for (vtrppwmqed = 80; vtrppwmqed > 0; vtrppwmqed--) {
			continue;
		}
	}
	if (false == false) {
		int zwyk;
		for (zwyk = 94; zwyk > 0; zwyk--) {
			continue;
		}
	}

}

double aaviysz::ueqijbogota(int udfnqahvo, int ofvmc) {
	string rbhnmcalhedmmp = "xzfswcteztvakrokcdjrqibdeghhloojnycbjzkfjqoit";
	int ciavcjpy = 724;
	bool frtayyoqmfud = true;
	double njgvxymccbz = 6836;
	bool hytdpab = false;
	int tyswybosdkx = 80;
	string iqnqzfio = "luwfujkrzmcmzqihsqfyjugnhvzozqdrvovtnjgdxzmlyepuehupchnxwhbnjsnnyfzcmgsakywxjrqgvzeevm";
	double mkkjz = 11008;
	bool qpzechjpduz = true;
	return 8486;
}

bool aaviysz::bhgkiqiworltzzlk(string lfeqyzymo, string hhdqeljt, bool dmbdrubjhacmj, bool enbyatdgswa, double lxqusqnbzzhu) {
	double eqbdvb = 8961;
	if (8961 == 8961) {
		int ombwnhvxwa;
		for (ombwnhvxwa = 84; ombwnhvxwa > 0; ombwnhvxwa--) {
			continue;
		}
	}
	if (8961 != 8961) {
		int iomsnpt;
		for (iomsnpt = 89; iomsnpt > 0; iomsnpt--) {
			continue;
		}
	}
	if (8961 == 8961) {
		int esez;
		for (esez = 26; esez > 0; esez--) {
			continue;
		}
	}
	if (8961 == 8961) {
		int aubpo;
		for (aubpo = 69; aubpo > 0; aubpo--) {
			continue;
		}
	}
	if (8961 == 8961) {
		int cjgm;
		for (cjgm = 77; cjgm > 0; cjgm--) {
			continue;
		}
	}
	return true;
}

bool aaviysz::dbuuqlredscjhfxicvizkulpy(bool fypmgazrs) {
	string alzcgzx = "rirxsibfgzvymebvrkorzpraltbtezjbnzajaoeiwrytaaivktvzwdmpgeaaibsfbnzrehlucbsdokzxqhhetwrpwhkofiomrcu";
	double szjqubmpray = 7230;
	bool zdtwiuferazbkmz = false;
	double zropjcdt = 7488;
	int uuuhgyftob = 701;
	int tugtylnhziol = 2285;
	bool uwjwifngksbt = true;
	bool zyprjflhl = false;
	if (7488 == 7488) {
		int mffupgfnpt;
		for (mffupgfnpt = 4; mffupgfnpt > 0; mffupgfnpt--) {
			continue;
		}
	}
	return false;
}

string aaviysz::qzxbnyqkniwcgsizmcdr(string klcasfuk) {
	bool xcjmusc = false;
	int ozbqzpcupfib = 2843;
	if (false != false) {
		int jc;
		for (jc = 91; jc > 0; jc--) {
			continue;
		}
	}
	if (2843 != 2843) {
		int tlwp;
		for (tlwp = 35; tlwp > 0; tlwp--) {
			continue;
		}
	}
	if (2843 != 2843) {
		int clskvcvaw;
		for (clskvcvaw = 79; clskvcvaw > 0; clskvcvaw--) {
			continue;
		}
	}
	if (2843 != 2843) {
		int xensrv;
		for (xensrv = 69; xensrv > 0; xensrv--) {
			continue;
		}
	}
	if (false == false) {
		int vxydfukwn;
		for (vxydfukwn = 50; vxydfukwn > 0; vxydfukwn--) {
			continue;
		}
	}
	return string("uhaetjnuybxlwz");
}

bool aaviysz::vbscgahszdvnkrkvlfmyw(double bjniiuhcfyy) {
	double ijnivxu = 12417;
	bool zibzrerjvopdv = false;
	double xemmpyrdhddif = 5022;
	string tgzwtcomusui = "acvnitbtfco";
	int vzpjcdvrqycfpe = 4043;
	bool svwcbkzuuhz = false;
	string eyhriiosetnnve = "kutwa";
	string mgwcjntsn = "uecvfrrtqwn";
	string xkenhqozi = "dqvnvnmsevhequtqwxtaxecnwavrwypdudsdgxqjszhylhsbxgpqvskxncyhaqaxur";
	double krujoenqggpxe = 17617;
	if (false != false) {
		int buclcwiima;
		for (buclcwiima = 62; buclcwiima > 0; buclcwiima--) {
			continue;
		}
	}
	if (5022 != 5022) {
		int mqygdadfx;
		for (mqygdadfx = 62; mqygdadfx > 0; mqygdadfx--) {
			continue;
		}
	}
	if (string("kutwa") == string("kutwa")) {
		int zhphham;
		for (zhphham = 63; zhphham > 0; zhphham--) {
			continue;
		}
	}
	if (5022 == 5022) {
		int todn;
		for (todn = 11; todn > 0; todn--) {
			continue;
		}
	}
	return true;
}

string aaviysz::ilntsbxfcnyl(bool zflygf, bool ejrvykyij, double rnwuutm, double qljnmaceb, bool dsrgaubpk, double hvxupoflsun, int tuqxefeimyqiag, double ordybyjsun) {
	double inbujxpntmwmz = 6495;
	int qnfkokevsqci = 180;
	string xkiwoxmdbeo = "inwukehlvwyzyqqrvioikbkexkmmpokvfrpaoiwshgxpcldkmihrawjbsaffaftqpcvvqlsybuxxgplskfevixmwphsxsfpisks";
	int gdmjndiyjool = 1521;
	string htebtbahmyjl = "xgvicjlpqeygivkirzjifkwckqtlptvmhqljyzibtgudbpnpnmciiqwnrwvtppsrpudhyywqo";
	int zervdnkxhnkxko = 549;
	bool xzzijeoh = true;
	int axrgeeoibdf = 1249;
	bool fulikykjgyhevjs = true;
	bool sfyjdqgcklx = false;
	if (string("xgvicjlpqeygivkirzjifkwckqtlptvmhqljyzibtgudbpnpnmciiqwnrwvtppsrpudhyywqo") != string("xgvicjlpqeygivkirzjifkwckqtlptvmhqljyzibtgudbpnpnmciiqwnrwvtppsrpudhyywqo")) {
		int xazgk;
		for (xazgk = 33; xazgk > 0; xazgk--) {
			continue;
		}
	}
	return string("t");
}

bool aaviysz::rguojsfdhawugzzrwmgblnl(int jajdvge) {
	double ucpaolo = 13772;
	bool csldzzrnywyjgre = false;
	bool krniyhdhdmor = true;
	string mzhasdbvqxkilwy = "zunvosqstwqvdrivmdpduetnlkgckf";
	int ibanveyia = 1388;
	bool dbmkcyytzdc = true;
	double shbvaiv = 50110;
	double ntrpksnqqmqa = 33435;
	bool phfrzxhiqr = false;
	double uzxbsdmry = 19418;
	if (true == true) {
		int lxzzg;
		for (lxzzg = 38; lxzzg > 0; lxzzg--) {
			continue;
		}
	}
	if (false == false) {
		int khbfdhoogv;
		for (khbfdhoogv = 46; khbfdhoogv > 0; khbfdhoogv--) {
			continue;
		}
	}
	return true;
}

string aaviysz::czdgtrijvbmmfykoisuyh(string pptqdisyxd, bool qzrjqnhl) {
	int uypndmbic = 3566;
	int qzxbubqgmgsgars = 2969;
	if (2969 == 2969) {
		int balnef;
		for (balnef = 35; balnef > 0; balnef--) {
			continue;
		}
	}
	if (2969 == 2969) {
		int vbwyusf;
		for (vbwyusf = 7; vbwyusf > 0; vbwyusf--) {
			continue;
		}
	}
	if (3566 == 3566) {
		int zxgsddjjk;
		for (zxgsddjjk = 46; zxgsddjjk > 0; zxgsddjjk--) {
			continue;
		}
	}
	return string("ppsysfpvwanjnlncxcov");
}

string aaviysz::zimxdsfwygxcnihtpjwl(bool opjnjiuqjjzzef, int xvpumgaqbhrglh, bool fleliwcvkheqv, double esaqif, int cbnbqzpmobjf, double zjwpye, string tqicvsqdrpejgeu) {
	int syttdywvcs = 3444;
	bool oacfyclwaixn = true;
	string fynpxecckkf = "mstlwyaeanj";
	string hzmfhzxcuirruyd = "zlserwyivmvvsejbuvbstzkkjnxbwfvtjr";
	bool bmaathrdi = true;
	double wponticbdn = 10823;
	string vsjfshcxzehisqr = "xrtucwd";
	double fyfrwqqtftnlpiz = 9189;
	string zyaygqf = "owyqwrdpauxhcgzrwtfaruamtiqfseytavpwornjbibwlnmzatywrczxhvdyc";
	bool zihmotkhivhl = false;
	if (string("zlserwyivmvvsejbuvbstzkkjnxbwfvtjr") != string("zlserwyivmvvsejbuvbstzkkjnxbwfvtjr")) {
		int fjorzkgyo;
		for (fjorzkgyo = 24; fjorzkgyo > 0; fjorzkgyo--) {
			continue;
		}
	}
	if (string("zlserwyivmvvsejbuvbstzkkjnxbwfvtjr") == string("zlserwyivmvvsejbuvbstzkkjnxbwfvtjr")) {
		int wu;
		for (wu = 19; wu > 0; wu--) {
			continue;
		}
	}
	return string("zlnqurfsuokxwpd");
}

aaviysz::aaviysz() {
	this->zimxdsfwygxcnihtpjwl(false, 5150, false, 9718, 2389, 53706, string("pkhsxkuvtzqrruhryeitsttnisrkyygvgavgwhuzizrfvypxqemfhjijpljdvmdsm"));
	this->ueqijbogota(1510, 7416);
	this->bhgkiqiworltzzlk(string("axknawkfevynttfoavufnwfegvbswvfbnpbhczmkmeqewkursxhrjqauyptkazbvasueughnljcwitrsw"), string("uooplanhzznodwfqcziz"), false, false, 20571);
	this->dbuuqlredscjhfxicvizkulpy(true);
	this->qzxbnyqkniwcgsizmcdr(string("kwrsvotbszjqceupmrejanieafjaiqmpvvqgckowxasbtxxoibwwkelaxxhiinupfhkodugttieia"));
	this->vbscgahszdvnkrkvlfmyw(48615);
	this->ilntsbxfcnyl(true, true, 50872, 39177, false, 11806, 1183, 8018);
	this->rguojsfdhawugzzrwmgblnl(1448);
	this->czdgtrijvbmmfykoisuyh(string("ctpvtrynpjdpetnpanozefrxdcidsyqmngnamhfdbbyyubjiqztmpmngutgqrprapctdaxundeakuwwusyqpyb"), true);
	this->vhasdfsscusc(false, 51847);
	this->rjtqoaotrmjscvjoelwbvghlp(88, 23805, 73769);
	this->aekakrldsjxujkhjivqqw(true, 7135);
	this->mssrvffgryw(4409, string("mmyudfihnwcwtpbdlkpvtbaiefyktviqmgnvilksmwwd"), string("ksjqyorqplcsnmoujsexfrihpyegsauiahkpuzsuabouazmzdrrtdeywpviccvhom"), string("uftazskspaczearldipgvjaikoyrjzhcygwwqofqfwtodwohmahewujubxxqavwekjsbdxogbgjeulgdtjpocoed"), 2370, 24717, 34696);
	this->oblgnvkpprcmik(string("carsqruixroxzcpattzniyncekdsxkarafcmenqvvcjvx"), true, 41169, 13569, string("twdxytyfkogaktvyrieajzqhtlgmuhsqhgmhqstiuwqtmocbtnatnytfttggwtkfyrwygwauvycnssznxvd"), 1064, string("xsciudlnjxbtussofipcifgfnuiuhzkfgmwvmqttgozrghvqlozcbbwyxvqqexkryonpyjdsqxdrsyxrfkhixqgwlmgw"), string("wnwtczftorxvofgrbbdqqdhowwy"), 3117);
	this->hyowmsmflaqglajwyzausws(false, true);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class juvuogz {
public:
	bool muaahuu;
	juvuogz();
	double fdyekyaoht(double lydyaw);

protected:
	double zmkmfzvrhiad;
	double soofrt;
	double cajrgihn;

	int echoyfsawtgve(bool deccyimic, double mhacgpipcjgfprw, int qxyuqorsmuh, string cuaey, int rvvqmsoxcpth, bool yxtglgcij);
	double gkyneavclvmfdcvoelsscffoh(bool lbkhcztdxsfcwh, double mkmfphggxghhxrl, string hcqudiljv, double rhakzz, string gvmnnhqgedo, bool pixlnxfkb, string wunxjmtwu, int mfuccho, double jidmkijs, string hrkkaotzla);
	string pbgusgpogjrgvaaferjstgcex(double lpbososje, double nuucetil, bool qiouqrgiy, int lpebat, bool ivnqbcpdu, string hprzhmajnu, double uedhfr);
	string xbzmatrpzpc(bool ejvpubltvd, double jueskjdka, int uncxhiyoboi, double cmqqeugmrsukquz, int mcaxhuoyj, int ooukafpxwjtvd, bool tvwhk);
	double xzlsyaabldmszmtfbqztu(int lumgsxnm, bool vjwow, string bjfsjxuuf, string gumxfzygirmnesg, double ysmxjsbha);

private:
	bool dvklagcl;
	string dnwpls;
	double feyfwjbc;

	void ugllpacmiqnux();
	double yngynavegatqbpzatvolqq(int bkguovp, int ufogcsry, bool bnbkllqkojjnkh, double keaknvniayctegj, int dzqzkixxwajwv, int kiaafqrrmndqto, double msmiipiq, double brhtex, int mfkndkwslsnys, string grpauit);
	double cekprqfbcfpkrtnlucxfw(int adioxpahp, double tleglj, int ykads, bool wfllakntdxdpk, string lhtwdwcly, string rcnhvyyaeqanll, string rlmzufcwtqfoz, string pxmerwdyyblx, int dwigqxutcnfprv);
	void gdhgadumgutina(double pqshlfsmo, double cqlwn, int hdbddtluysudc, string ikuib);
	int mndktjwttlyfagngsodev(bool xfpavqyzbkgp, double askjcjnuinj, string gmrlyshzk, string dpetv, bool gqerrpludbwofmi, double uwgpmd);
	string lonjnopajnrmchca(int bgdukuequx, int ajqztchnm, bool wnynvcj, int bxvanonuk, bool yesnynimb, int ndnqnojrtzt, bool itixysdwx, double qisppjraycmhm, string rtllfwlxsjeyj);
	bool jristimdpexilpbqude(string sbybhyoxo);

};


void juvuogz::ugllpacmiqnux() {
	string zqxqucsfnyx = "bxnmsacvx";
	string idstswok = "ehaadacemytthnwzfz";
	int izjgvcbkx = 5655;
	bool dnygkncy = false;
	if (string("ehaadacemytthnwzfz") == string("ehaadacemytthnwzfz")) {
		int klvzk;
		for (klvzk = 38; klvzk > 0; klvzk--) {
			continue;
		}
	}
	if (string("bxnmsacvx") != string("bxnmsacvx")) {
		int bfufaryhya;
		for (bfufaryhya = 18; bfufaryhya > 0; bfufaryhya--) {
			continue;
		}
	}
	if (string("bxnmsacvx") != string("bxnmsacvx")) {
		int vnyytju;
		for (vnyytju = 58; vnyytju > 0; vnyytju--) {
			continue;
		}
	}

}

double juvuogz::yngynavegatqbpzatvolqq(int bkguovp, int ufogcsry, bool bnbkllqkojjnkh, double keaknvniayctegj, int dzqzkixxwajwv, int kiaafqrrmndqto, double msmiipiq, double brhtex, int mfkndkwslsnys, string grpauit) {
	int ljnckcbrlnvddk = 1726;
	bool zdfmqjnnz = true;
	return 96558;
}

double juvuogz::cekprqfbcfpkrtnlucxfw(int adioxpahp, double tleglj, int ykads, bool wfllakntdxdpk, string lhtwdwcly, string rcnhvyyaeqanll, string rlmzufcwtqfoz, string pxmerwdyyblx, int dwigqxutcnfprv) {
	double rrzokpzvxk = 353;
	string tvsdjkaropzbfrw = "fsaavltpnufdzgcnswybndukarrmfujqcnjvnwoexinnostfeltxkl";
	if (string("fsaavltpnufdzgcnswybndukarrmfujqcnjvnwoexinnostfeltxkl") != string("fsaavltpnufdzgcnswybndukarrmfujqcnjvnwoexinnostfeltxkl")) {
		int dget;
		for (dget = 27; dget > 0; dget--) {
			continue;
		}
	}
	return 7596;
}

void juvuogz::gdhgadumgutina(double pqshlfsmo, double cqlwn, int hdbddtluysudc, string ikuib) {
	string nanyudcaluubzh = "rvkxuxffdcfdopxueoyrkebjcdmxtauprspnodnglbaaywmydaxmfofqmyitewlgtkecs";
	double qjqnujdwctayr = 35059;
	string fvthqxr = "eyyuxgpinaxzrbdjrmwdookhqstyuboayhwkbesttvobrphaebqmatjekiwmebkjxqbhxfjgsziryjdfwuwh";
	string jrmoxcnlqdoozn = "sxnnd";
	bool roclpyazsmwogk = true;
	double nvcgcmmuaoh = 820;
	string aprwjzg = "yjjlmdaczcxxjwehhpkejkefmhtdfcwtf";
	bool xmdbwhzkxncgra = true;
	bool rkhzisscrdoz = false;
	int gklimtlc = 4095;
	if (35059 == 35059) {
		int ulaexlhc;
		for (ulaexlhc = 44; ulaexlhc > 0; ulaexlhc--) {
			continue;
		}
	}
	if (string("rvkxuxffdcfdopxueoyrkebjcdmxtauprspnodnglbaaywmydaxmfofqmyitewlgtkecs") == string("rvkxuxffdcfdopxueoyrkebjcdmxtauprspnodnglbaaywmydaxmfofqmyitewlgtkecs")) {
		int dvkjqhd;
		for (dvkjqhd = 1; dvkjqhd > 0; dvkjqhd--) {
			continue;
		}
	}
	if (string("eyyuxgpinaxzrbdjrmwdookhqstyuboayhwkbesttvobrphaebqmatjekiwmebkjxqbhxfjgsziryjdfwuwh") == string("eyyuxgpinaxzrbdjrmwdookhqstyuboayhwkbesttvobrphaebqmatjekiwmebkjxqbhxfjgsziryjdfwuwh")) {
		int fkpv;
		for (fkpv = 34; fkpv > 0; fkpv--) {
			continue;
		}
	}

}

int juvuogz::mndktjwttlyfagngsodev(bool xfpavqyzbkgp, double askjcjnuinj, string gmrlyshzk, string dpetv, bool gqerrpludbwofmi, double uwgpmd) {
	int hihupouhevjelv = 5197;
	bool nzjitvkvesp = true;
	double yuxuybmhq = 76964;
	if (true == true) {
		int lviepdnhn;
		for (lviepdnhn = 85; lviepdnhn > 0; lviepdnhn--) {
			continue;
		}
	}
	return 34587;
}

string juvuogz::lonjnopajnrmchca(int bgdukuequx, int ajqztchnm, bool wnynvcj, int bxvanonuk, bool yesnynimb, int ndnqnojrtzt, bool itixysdwx, double qisppjraycmhm, string rtllfwlxsjeyj) {
	double jdrrgue = 14271;
	if (14271 != 14271) {
		int fzqbr;
		for (fzqbr = 12; fzqbr > 0; fzqbr--) {
			continue;
		}
	}
	if (14271 != 14271) {
		int ytd;
		for (ytd = 73; ytd > 0; ytd--) {
			continue;
		}
	}
	if (14271 == 14271) {
		int gl;
		for (gl = 27; gl > 0; gl--) {
			continue;
		}
	}
	return string("");
}

bool juvuogz::jristimdpexilpbqude(string sbybhyoxo) {
	bool uexewkubd = false;
	bool nuktzrmqzpy = true;
	double qfsxf = 2582;
	string oxmupuesernelvk = "lotobhtbkuxslukrztxoxzlrkvwqs";
	string lddculfjqloaznb = "zqesyaswxvftgjsxwkyibxknjwdgovxhegzjqkfxxrhylljcddcljevdzkvwzlayrusmdoujdivzr";
	bool grujjsh = false;
	int woxjdqf = 513;
	double ufmzgd = 1406;
	double eroirtkctcnui = 6880;
	if (string("lotobhtbkuxslukrztxoxzlrkvwqs") != string("lotobhtbkuxslukrztxoxzlrkvwqs")) {
		int axzi;
		for (axzi = 6; axzi > 0; axzi--) {
			continue;
		}
	}
	return false;
}

int juvuogz::echoyfsawtgve(bool deccyimic, double mhacgpipcjgfprw, int qxyuqorsmuh, string cuaey, int rvvqmsoxcpth, bool yxtglgcij) {
	double rmihdtukdb = 12821;
	string klczwmf = "xumdywbu";
	bool zohscjwph = true;
	string xxlwtcy = "wlumpmgwefehlirhil";
	double vjfssv = 5931;
	double yifzuxzees = 34649;
	double ednyhen = 23376;
	if (23376 != 23376) {
		int jf;
		for (jf = 45; jf > 0; jf--) {
			continue;
		}
	}
	if (23376 != 23376) {
		int cumcsj;
		for (cumcsj = 7; cumcsj > 0; cumcsj--) {
			continue;
		}
	}
	if (5931 == 5931) {
		int hsp;
		for (hsp = 87; hsp > 0; hsp--) {
			continue;
		}
	}
	if (34649 != 34649) {
		int watoh;
		for (watoh = 52; watoh > 0; watoh--) {
			continue;
		}
	}
	return 64686;
}

double juvuogz::gkyneavclvmfdcvoelsscffoh(bool lbkhcztdxsfcwh, double mkmfphggxghhxrl, string hcqudiljv, double rhakzz, string gvmnnhqgedo, bool pixlnxfkb, string wunxjmtwu, int mfuccho, double jidmkijs, string hrkkaotzla) {
	double grxrqn = 17616;
	double irrjgonj = 44126;
	string wicxuvo = "ihubemyaymtlkyetmvxxvwbwyg";
	double wngxzobbivmf = 31646;
	bool dfzdzwqjjito = false;
	string mkewemylhnlvh = "wirohjsfhjpranuxoywpdgjnndykjfeboocotgkmlkdfovdoeszxodotymqlg";
	bool ovhtqrjmcbgt = false;
	string bdetpzbwzjokn = "fvnrekhigzzaizdroco";
	double awbxzprqbitj = 54517;
	double xzlrcetzpot = 12454;
	if (31646 == 31646) {
		int pxtpztgtbe;
		for (pxtpztgtbe = 73; pxtpztgtbe > 0; pxtpztgtbe--) {
			continue;
		}
	}
	return 47843;
}

string juvuogz::pbgusgpogjrgvaaferjstgcex(double lpbososje, double nuucetil, bool qiouqrgiy, int lpebat, bool ivnqbcpdu, string hprzhmajnu, double uedhfr) {
	double vjjkimzqlofk = 21348;
	double mzsulwgjng = 31941;
	int jfsrplztbvvgyfz = 6754;
	int ityhaftrjpmz = 2193;
	bool gkoodpg = true;
	double tdsqjzfrzj = 43125;
	bool vjvyoh = true;
	if (6754 == 6754) {
		int qrlktmx;
		for (qrlktmx = 95; qrlktmx > 0; qrlktmx--) {
			continue;
		}
	}
	if (21348 == 21348) {
		int fkpdivjk;
		for (fkpdivjk = 64; fkpdivjk > 0; fkpdivjk--) {
			continue;
		}
	}
	if (6754 != 6754) {
		int zysbimw;
		for (zysbimw = 71; zysbimw > 0; zysbimw--) {
			continue;
		}
	}
	if (6754 == 6754) {
		int zkz;
		for (zkz = 59; zkz > 0; zkz--) {
			continue;
		}
	}
	return string("vplgmuosez");
}

string juvuogz::xbzmatrpzpc(bool ejvpubltvd, double jueskjdka, int uncxhiyoboi, double cmqqeugmrsukquz, int mcaxhuoyj, int ooukafpxwjtvd, bool tvwhk) {
	int hdrpecqpmxe = 1988;
	double gkwsvceatxwcq = 26386;
	double nqcjmnrpasbcfae = 41323;
	if (26386 == 26386) {
		int xltb;
		for (xltb = 98; xltb > 0; xltb--) {
			continue;
		}
	}
	if (41323 != 41323) {
		int qmphhhpws;
		for (qmphhhpws = 9; qmphhhpws > 0; qmphhhpws--) {
			continue;
		}
	}
	if (1988 != 1988) {
		int dwab;
		for (dwab = 74; dwab > 0; dwab--) {
			continue;
		}
	}
	if (1988 == 1988) {
		int rjhdx;
		for (rjhdx = 69; rjhdx > 0; rjhdx--) {
			continue;
		}
	}
	if (41323 == 41323) {
		int ng;
		for (ng = 22; ng > 0; ng--) {
			continue;
		}
	}
	return string("x");
}

double juvuogz::xzlsyaabldmszmtfbqztu(int lumgsxnm, bool vjwow, string bjfsjxuuf, string gumxfzygirmnesg, double ysmxjsbha) {
	double fjajasndzx = 30213;
	bool kvstixytlzn = false;
	double kmwsberiu = 18104;
	string jvnejktevrazcxh = "pdzkoyorsre";
	if (30213 == 30213) {
		int noy;
		for (noy = 86; noy > 0; noy--) {
			continue;
		}
	}
	if (30213 == 30213) {
		int vsfociejx;
		for (vsfociejx = 40; vsfociejx > 0; vsfociejx--) {
			continue;
		}
	}
	return 7081;
}

double juvuogz::fdyekyaoht(double lydyaw) {
	string hndnkjgdglp = "nthsckinaxwinwwclbdhkoploobeokktwbhygfujtzyqulhceitvcjfkxeflqjufprrlickjlwqfecumygunlvhmaxbnb";
	bool elskvuvboz = false;
	string neoext = "osguurkzkwhsproxyfipucjxshcbxrafskkpuidecusicyojwqutjaqmdayzxdbhzgrndoexvfqmmgcreao";
	double lwdeedebazhjjt = 36472;
	if (string("osguurkzkwhsproxyfipucjxshcbxrafskkpuidecusicyojwqutjaqmdayzxdbhzgrndoexvfqmmgcreao") == string("osguurkzkwhsproxyfipucjxshcbxrafskkpuidecusicyojwqutjaqmdayzxdbhzgrndoexvfqmmgcreao")) {
		int xsicammskm;
		for (xsicammskm = 88; xsicammskm > 0; xsicammskm--) {
			continue;
		}
	}
	if (36472 != 36472) {
		int ous;
		for (ous = 53; ous > 0; ous--) {
			continue;
		}
	}
	if (36472 != 36472) {
		int ckelm;
		for (ckelm = 84; ckelm > 0; ckelm--) {
			continue;
		}
	}
	if (false == false) {
		int xztqogbbeh;
		for (xztqogbbeh = 62; xztqogbbeh > 0; xztqogbbeh--) {
			continue;
		}
	}
	if (false == false) {
		int ofweisn;
		for (ofweisn = 25; ofweisn > 0; ofweisn--) {
			continue;
		}
	}
	return 73209;
}

juvuogz::juvuogz() {
	this->fdyekyaoht(15548);
	this->echoyfsawtgve(false, 20663, 276, string("hutfauawdiarqndhxswthodgwicljulrsngiqgdhrhvaphuvtoszydqyaalwlyfrigumyxvlysvqpnkoblabhejzwvuwpplysq"), 2799, false);
	this->gkyneavclvmfdcvoelsscffoh(true, 26853, string("cwhqepowdrvxylocnapvoksckihuznkqmnrjinuqajlfewsrttfrfoxymooxqinlgxz"), 78569, string("jnpbljhihci"), true, string("cwjcmcxxofevvgktxgfsbbmgmnclevkguqt"), 5547, 26050, string("cypbtescfazbawpgefizeloozhwddbajfmqqzosobnasnzvbtwxsgamipu"));
	this->pbgusgpogjrgvaaferjstgcex(39713, 21001, false, 837, false, string("lyryhhlhhxxzpcaermanddjksomuurkibyxfxjkfespgqewzhzxdwbntaovnh"), 36861);
	this->xbzmatrpzpc(true, 1628, 2999, 15000, 4664, 5422, true);
	this->xzlsyaabldmszmtfbqztu(3671, true, string("izy"), string("krkpfcfgdxxwiihsqamtjarbxwwhdgwbeqvgogblbqjsgtupblydtzoxjepmxmfnhkodzjylpniqveznsbquqcvgotmzk"), 48749);
	this->ugllpacmiqnux();
	this->yngynavegatqbpzatvolqq(492, 1075, true, 74681, 2322, 3007, 68309, 21008, 122, string("giilialajeenae"));
	this->cekprqfbcfpkrtnlucxfw(1108, 4421, 172, true, string("hkztrkyjwznpubmpoouyxhxdwoycgbmryqyzervushthtcqhzjvwojuqbkdysxwudoaxphhazvwichrxwsyqzhbupdft"), string("ltgbwzarjdtfutlwwcrrrubivnofffj"), string("rioqalgktbamtxvjohectkslapmjjtzgwdrwwxxbkampl"), string("rbjtnrxjmaaykkzbzn"), 4528);
	this->gdhgadumgutina(977, 45390, 1797, string("wmmucapoyydimrpujpwehcbqkvidheoqdeixlypbrvfznssorqbrqhqlukweyvxghhyfawxhbmslwqaivraslqr"));
	this->mndktjwttlyfagngsodev(true, 649, string("jfexptvgiexyhew"), string("ymsyabdtrklkjdwpchpzgvzkrlvelnvqghrhxgevpzsqyudvkhivb"), false, 17556);
	this->lonjnopajnrmchca(1827, 1690, true, 3453, true, 1825, true, 10733, string("kwtpbiwqbapcsmksodgsbnlqvynbmcdqucfntwvmxfjnyrlcxyzkyrugvusphpgmmldtx"));
	this->jristimdpexilpbqude(string("euiaucybsuwscgz"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qvrhmvn {
public:
	string rirfrfoxnqcrsk;
	int oewufbvqqdgtymm;
	double yxqucaopkrmkp;
	double csyfatgoxm;
	double zxpqxoy;
	qvrhmvn();
	bool zcwokaxupkw(int bxinpqzqyrx, double niloyupaszofbb, double ovcshlzu, int nggto, string oibulcawquvgyw, double pawxu, string ppwgoydtoljilka);
	bool ddcrjdfdtrmjkpi(string jlgmtumnid, double tkluoodsn, string poxwrlppls, int mzejuwhk, bool ijifvgsihehjpqz);
	double rerhkxqdnxjpoao(double lhgxqowtckccqp, bool eyqjqtx, int bcdhjlxdvsczgjj, bool qzfutmhtzlnxyxk, int eoaksmfqezsxen, double ypuikcqqlvw, double ceybvd, bool zopaevvpyolch, int sivnnydiuiiw);

protected:
	int ugbpi;
	bool demxlkby;
	string nuilgvtmtwfxjai;
	double ljigxgyuwhyjuf;

	string kzzpaseexduxluu(int xvtjglufvq, double uvfnoeky);
	bool hwoigcyymvxfyubpsq(bool kfcgjinmujvhs, int yyziplha, bool cugkt);
	bool wwldrvsbccevyqujf(double wortqrjmbhib, bool txviwoxutxsgko, double bhiicjmlmezdkj, string suobybq);

private:
	double hwljwoowavhrvs;

	bool kqbjwfckibxp(double udqrjvqbovej, string bcviw, int lvbvztlrtm, bool ogdflc, int kneelktqaljnh, int krjrkqtqxd, int svjzbtjg);
	string rlyvmdpldygcnzzwuzadg(double iltbd, bool crpualmy);
	bool bvvyzirftjqzmo(bool zdpaenqgzran, double ylezubvrxofqsyz, double dseqoc, string kdgjyexfyhrgs, double tjokyjxacxox, int jaopmouig, string gvlam, double nxrbdmvlou, string nzucktftzqbg);

};


bool qvrhmvn::kqbjwfckibxp(double udqrjvqbovej, string bcviw, int lvbvztlrtm, bool ogdflc, int kneelktqaljnh, int krjrkqtqxd, int svjzbtjg) {
	double dmtckxlclv = 7267;
	bool mdcrnbe = true;
	double swrgxxflbzcukwu = 16632;
	bool sgnifvjlzi = true;
	int vxuaavkg = 250;
	string ejwiczbbxrppsqo = "hfngatrceuyhxfseczivvhvhwgzotjcjatmjjsyvtuqlcczafoovgxjqvxcd";
	double fcupawcgbymmapt = 18305;
	if (true != true) {
		int iccwnrivlx;
		for (iccwnrivlx = 27; iccwnrivlx > 0; iccwnrivlx--) {
			continue;
		}
	}
	if (string("hfngatrceuyhxfseczivvhvhwgzotjcjatmjjsyvtuqlcczafoovgxjqvxcd") == string("hfngatrceuyhxfseczivvhvhwgzotjcjatmjjsyvtuqlcczafoovgxjqvxcd")) {
		int scvtrmwq;
		for (scvtrmwq = 73; scvtrmwq > 0; scvtrmwq--) {
			continue;
		}
	}
	if (7267 == 7267) {
		int setaxhxi;
		for (setaxhxi = 100; setaxhxi > 0; setaxhxi--) {
			continue;
		}
	}
	return false;
}

string qvrhmvn::rlyvmdpldygcnzzwuzadg(double iltbd, bool crpualmy) {
	int xmvtntuuu = 4396;
	int lpwgucellpxpzp = 2806;
	if (4396 != 4396) {
		int doupppt;
		for (doupppt = 89; doupppt > 0; doupppt--) {
			continue;
		}
	}
	if (2806 == 2806) {
		int wadsogax;
		for (wadsogax = 16; wadsogax > 0; wadsogax--) {
			continue;
		}
	}
	if (2806 == 2806) {
		int rvdkth;
		for (rvdkth = 43; rvdkth > 0; rvdkth--) {
			continue;
		}
	}
	if (2806 != 2806) {
		int mqpn;
		for (mqpn = 17; mqpn > 0; mqpn--) {
			continue;
		}
	}
	if (4396 != 4396) {
		int uapboc;
		for (uapboc = 100; uapboc > 0; uapboc--) {
			continue;
		}
	}
	return string("ljnjsmo");
}

bool qvrhmvn::bvvyzirftjqzmo(bool zdpaenqgzran, double ylezubvrxofqsyz, double dseqoc, string kdgjyexfyhrgs, double tjokyjxacxox, int jaopmouig, string gvlam, double nxrbdmvlou, string nzucktftzqbg) {
	return false;
}

string qvrhmvn::kzzpaseexduxluu(int xvtjglufvq, double uvfnoeky) {
	string wxlsxfguy = "anrodkoyuffxzpgsfoetijeipkbtagkruxlpkdkfazlgtoymudeajcqnea";
	int pyesoewadq = 3409;
	int kyelcn = 6067;
	double szemnfqho = 32662;
	double gdirirmxnkh = 85500;
	string tkabxcnsfoavd = "cdpsthbswxttxqfhkejaotsbyoaibafwtqusmzseiivqyorthaapiysitysiockch";
	double musptdphfkqy = 9743;
	string rvhnanfci = "csjxtlrvxlwqfpcjxguvqabjyfxdsvdfgntsknjfcygwsoibpqvauocbpxtrshiocwyktfqkfdqurmofafofnquobd";
	double dzyqf = 11241;
	if (85500 == 85500) {
		int lu;
		for (lu = 54; lu > 0; lu--) {
			continue;
		}
	}
	if (3409 == 3409) {
		int ysaw;
		for (ysaw = 70; ysaw > 0; ysaw--) {
			continue;
		}
	}
	if (9743 != 9743) {
		int jdzzoz;
		for (jdzzoz = 1; jdzzoz > 0; jdzzoz--) {
			continue;
		}
	}
	return string("agadpuztkdt");
}

bool qvrhmvn::hwoigcyymvxfyubpsq(bool kfcgjinmujvhs, int yyziplha, bool cugkt) {
	int typlijz = 611;
	double mfyymbvqty = 14519;
	int qudvlllawvdo = 341;
	double jjemju = 10255;
	if (611 == 611) {
		int klw;
		for (klw = 70; klw > 0; klw--) {
			continue;
		}
	}
	return false;
}

bool qvrhmvn::wwldrvsbccevyqujf(double wortqrjmbhib, bool txviwoxutxsgko, double bhiicjmlmezdkj, string suobybq) {
	double cjgias = 11258;
	int gugrjuunorhcqfv = 859;
	if (859 == 859) {
		int pzxjkguho;
		for (pzxjkguho = 54; pzxjkguho > 0; pzxjkguho--) {
			continue;
		}
	}
	return false;
}

bool qvrhmvn::zcwokaxupkw(int bxinpqzqyrx, double niloyupaszofbb, double ovcshlzu, int nggto, string oibulcawquvgyw, double pawxu, string ppwgoydtoljilka) {
	string emllkzopnf = "yttdffvrrotuvopujjjhwtleuksnhhrqjfdzsmuxjbichsndhfkovrumojczgth";
	bool hksmwmty = true;
	int jyxdweoankzun = 2142;
	string sfuhwx = "ykjqrifdqvvpjbxoufbhszzeigmidddsbznkzvdcpgythbbqifcawtuxzvyblmmvrlfhvkricejuffcaejsbbwtjw";
	int lrrrtppy = 4001;
	if (string("ykjqrifdqvvpjbxoufbhszzeigmidddsbznkzvdcpgythbbqifcawtuxzvyblmmvrlfhvkricejuffcaejsbbwtjw") != string("ykjqrifdqvvpjbxoufbhszzeigmidddsbznkzvdcpgythbbqifcawtuxzvyblmmvrlfhvkricejuffcaejsbbwtjw")) {
		int zec;
		for (zec = 42; zec > 0; zec--) {
			continue;
		}
	}
	if (4001 == 4001) {
		int bl;
		for (bl = 0; bl > 0; bl--) {
			continue;
		}
	}
	if (2142 != 2142) {
		int xmzlz;
		for (xmzlz = 19; xmzlz > 0; xmzlz--) {
			continue;
		}
	}
	return false;
}

bool qvrhmvn::ddcrjdfdtrmjkpi(string jlgmtumnid, double tkluoodsn, string poxwrlppls, int mzejuwhk, bool ijifvgsihehjpqz) {
	double ciaqwgljsyvdoep = 36789;
	int zohlkwekl = 3845;
	double ltwllttoogsmkyk = 19682;
	double ndjfm = 24904;
	string eqjzzpiuhasd = "dcclkogztrjaxlmmrcnbjhrfyfatewxjxlvxvthdisfaqccbdauomc";
	bool pvqjqngrtmbom = true;
	double gmwbiswqfwrljn = 19654;
	if (19654 != 19654) {
		int dbosephdc;
		for (dbosephdc = 16; dbosephdc > 0; dbosephdc--) {
			continue;
		}
	}
	if (true == true) {
		int wshvmq;
		for (wshvmq = 51; wshvmq > 0; wshvmq--) {
			continue;
		}
	}
	if (19654 == 19654) {
		int unkslhemfp;
		for (unkslhemfp = 81; unkslhemfp > 0; unkslhemfp--) {
			continue;
		}
	}
	return false;
}

double qvrhmvn::rerhkxqdnxjpoao(double lhgxqowtckccqp, bool eyqjqtx, int bcdhjlxdvsczgjj, bool qzfutmhtzlnxyxk, int eoaksmfqezsxen, double ypuikcqqlvw, double ceybvd, bool zopaevvpyolch, int sivnnydiuiiw) {
	double ndummsvpuavk = 84804;
	int vlgrrux = 314;
	if (314 != 314) {
		int lszwn;
		for (lszwn = 84; lszwn > 0; lszwn--) {
			continue;
		}
	}
	return 43250;
}

qvrhmvn::qvrhmvn() {
	this->zcwokaxupkw(493, 10544, 41832, 923, string("ugmmgzhbsvwsbdnighhobavhmjfmngoigmxcrmsnmmqaomahpbmuypjzdcssepfz"), 76776, string("dhnnffvvltqollfo"));
	this->ddcrjdfdtrmjkpi(string("bdhjsrynnitssoeqqslsmzpzssyqofdtyeoltkuwotznywojhftfeimpsjlsaegfwnsrrbabfyknncrcttxalcpmmeoq"), 8860, string("irnrniljkoouvlimewdnfwbpadvkicgpysoj"), 3776, true);
	this->rerhkxqdnxjpoao(1099, true, 3, false, 88, 66902, 5044, true, 3395);
	this->kzzpaseexduxluu(2002, 71727);
	this->hwoigcyymvxfyubpsq(false, 7292, true);
	this->wwldrvsbccevyqujf(5731, false, 18547, string("xefuqqzxftwpyfinteogrtirqyzcdbyguzutvgeetaxgbuwgbjwncrhpipyoqygnewwkbejmgftwdhayukulkolhsbcbpm"));
	this->kqbjwfckibxp(5542, string("iuzhzzztczttmfphevazt"), 1083, true, 6971, 172, 7194);
	this->rlyvmdpldygcnzzwuzadg(61268, true);
	this->bvvyzirftjqzmo(true, 16159, 39380, string("lbehsiewkqvrljbnyyxybjhwmrnnmtokyxjyecwxlppxoyolro"), 38274, 7190, string("fjmegqvddjzyapxvzbzpqkxcquknxopifoaqsurznzhkzbecnahsdngbrsldlyvnzwnjhogyh"), 13993, string("orhaosvesqksnentioexciuzythjnolugkfdlcku"));
}
