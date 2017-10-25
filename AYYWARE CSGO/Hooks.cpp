
#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"
#include <intrin.h>
#include "Controls.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "Resolver.h"
#include "Utilities.h"
#include <string>
#include "checksum_md5.h"
#include "lagcomp.h"
#include "predictionAyy.h"

#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

static int testTerTimer;

//LagCompensation *lagComp = nullptr;



DamageEventListener* DamageListener;
DamageEventListener* KillListener;
DamageEventListener* roundstart;
DamageEventListener* round_end;
DamageEventListener* Resolvertest;
ayyResolver* g_Resolver = new ayyResolver;
LagCompensation *lagComp = nullptr;
//CTimeSimulator* newtestBacktrack = new CTimeSimulator;


Vector LastAngleAA;
Vector LastAngleAAReal;
Vector LBYThirdpersonAngle;
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))
// Funtion Typedefs
typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef void(__thiscall* RenderViewFn)(void*, CViewSetup&, CViewSetup&, int, int);
typedef MDLHandle_t(__thiscall *iFindMdl)(void*, char*);

using OverrideViewFn = void(__fastcall*)(void*, void*, CViewSetup*);
typedef float(__stdcall *oGetViewModelFOV)();



// Function Pointers to the originals
PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
FrameStageNotifyFn oFrameStageNotify;
OverrideViewFn oOverrideView;
RenderViewFn oRenderView;

static bool resolverMemeTest;

// Hook function prototypes
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup);
float __stdcall GGetViewModelFOV();
void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw);

static bool roundStartTest;
static float myLBYTimer;
static int roundStartTimer;

// VMT Managers
namespace Hooks
{
	// VMT Managers
	Utilities::Memory::VMTManager VMTPanel; // Hooking drawing functions
	Utilities::Memory::VMTManager VMTClient; // Maybe CreateMove
	Utilities::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	Utilities::Memory::VMTManager VMTModelRender; // DrawModelEx for chams
	Utilities::Memory::VMTManager VMTPrediction; // InPrediction for no vis recoil
	Utilities::Memory::VMTManager VMTPlaySound; // Autoaccept 
	Utilities::Memory::VMTManager VMTRenderView;
	Utilities::Memory::VMTManager VMTModelCache;
};

typedef MDLHandle_t(__thiscall* iFindMdl)(void*, char*);
iFindMdl oFindMDL;
MDLHandle_t __fastcall hkFindMDL(void*, void*, char*);

#include "GameEventManager.h"
static float saveLastHeadshotFloat[65];
static float saveLastBaimFloat[65];
static float saveLastBaim30Float[65];

static float saveLastBaim10Float[65];

int hitmarkertime = 0;

static float testtimeToTick;
static float testServerTick;
static float testTickCount64 = 1;
template <typename T, std::size_t N> T* end_(T(&arr)[N]) { return arr + N; }
template <typename T, std::size_t N> T* begin_(T(&arr)[N]) { return arr; }


void imfinnarunuponya(IGameEvent* pEvent)
{

	int attackerid = pEvent->GetInt("attacker");
	int userid = pEvent->GetInt("userid");
	int dmg = pEvent->GetInt("dmg_health");
	int hitgroup = pEvent->GetInt("hitgroup");

	IClientEntity* pEntity = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(userid));
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->GetPlayerForUserID(userid) == Interfaces::Engine->GetLocalPlayer()) {

		// If we got hitted in our Head Sideswitch
		if (hitgroup == 1) {
			//SwitchAntiAimSide
			switchAntiAimSide = true;
		}
		else {
			switchAntiAimSide = false;
		}
	}

	// Define pWeapon for MLG Hitmarkers
	


	if (Interfaces::Engine->GetPlayerForUserID(attackerid) == Interfaces::Engine->GetLocalPlayer())
	{
		hitmarkertime = 100;

		if (Menu::Window.CustomTab.HitmarkerEnable.GetState())
		{
			if (Menu::Window.CustomTab.HitmarkerSound.GetState())
			{
				PlaySoundA("C:\\Hitmarker.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
		}


		
		//PlaySoundA("C:\\Hitmarker.wav", NULL, SND_FILENAME | SND_ASYNC);

		// WE have hitted someone is pretty good or???

		if (!pEntity || !pLocal || pEntity == pLocal)
			return;

		hittedLogHits[pEntity->GetIndex()] += 1;

		if (pEntity->GetVelocity().Length2D() == 0) {
			if (hitgroup == 1) {
				saveLastHeadshotFloat[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;

			}
			else if (dmg >= 50) {
				saveLastBaimFloat[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
			}
			else if (dmg >= 30) {
				saveLastBaim30Float[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
			}
		}

		
	}


}
player_info_t GetInfo(int Index) {
	player_info_t Info;
	Interfaces::Engine->GetPlayerInfo(Index, &Info);
	return Info;
}

void testResolver(IGameEvent* pEvent) {

	// Reset lby timer
	//roundStartTest = true;
	roundStartTimer++;
	shotsfired = 0;
	
	std::fill(begin_(fakeshotMissedLast), end_(fakeshotMissedLast), 0);
	//lagComp->initLagRecord();
	std::fill(begin_(hittedLogHits), end_(hittedLogHits), 0);
	std::fill(begin_(missedLogHits), end_(missedLogHits), 0);
	std::fill(begin_(shotFakeHeadOnce), end_(shotFakeHeadOnce), 0);
	
	// AFK Bot 
	if (Menu::Window.CustomTab.AFKBotEnable.GetState())
	{
		Interfaces::Engine->ClientCmd_Unrestricted("drop");
		Sleep(500);
		Interfaces::Engine->ClientCmd_Unrestricted("buy p250");
		Sleep(500);
		Interfaces::Engine->ClientCmd_Unrestricted("buy glock; buy hkp2000;");
		Sleep(500);
		if (Menu::Window.CustomTab.AFKBotMessage.GetState())
		{
			Interfaces::Engine->ClientCmd_Unrestricted("say_team Good luck guys! I'm gonna be AFK for this round! Sorry! (Sent using my AFK bot)");
		}
	}
}

void imfinnakillyou(IGameEvent* pEvent)
{

	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (Menu::Window.CustomTab.DeathSoundEnable.GetState())
	{
		if (Menu::Window.CustomTab.DeathSoundOof.GetState() && hitmarkertime > 0)
		{
			PlaySoundA("C:\\Oof.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (!Menu::Window.CustomTab.DeathSoundOof.GetState() && hitmarkertime > 0)
		{
			PlaySoundA("C:\\Hitmarker.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
	}
}

void imfinnamemeu(IGameEvent* pEvent)
{

	

}

// Undo our hooks
void Hooks::UndoHooks()
{
	VMTPanel.RestoreOriginal();
	VMTPrediction.RestoreOriginal();
	VMTModelRender.RestoreOriginal();
	VMTClientMode.RestoreOriginal();
	Interfaces::GameEventManager->RemoveListener(DamageListener);
	delete DamageListener;
}


// Initialise all our hooks
void Hooks::Initialise()
{

	VMTPanel.Initialise((DWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)&PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);

	VMTPrediction.Initialise((DWORD*)Interfaces::Prediction);
	VMTPrediction.HookMethod((DWORD)&Hooked_InPrediction, 14);

	VMTModelRender.Initialise((DWORD*)Interfaces::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.HookMethod((DWORD)&Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);

	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);
	
	oOverrideView = (OverrideViewFn)VMTClientMode.HookMethod((DWORD)&Hooked_OverrideView, 18);
	VMTClientMode.HookMethod((DWORD)&GGetViewModelFOV, 35);

	VMTClient.Initialise((DWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 36);

	DamageListener = new DamageEventListener(imfinnarunuponya);
	Interfaces::GameEventManager->AddListener(DamageListener, "player_hurt", false);

	KillListener = new DamageEventListener(imfinnakillyou);
	Interfaces::GameEventManager->AddListener(KillListener, "player_death", false);

	Resolvertest = new DamageEventListener(testResolver);
	Interfaces::GameEventManager->AddListener(Resolvertest, "round_start", false);

	round_end = new DamageEventListener(imfinnamemeu);
	Interfaces::GameEventManager->AddListener(round_end, "player_death", false);
	
	VMTModelCache.Initialise((DWORD*)Interfaces::ModelCache);
	oFindMDL = (iFindMdl)VMTModelCache.HookMethod((DWORD)&hkFindMDL, 10);

	PlaySoundA("C:\\Hitmarker.wav", NULL, SND_FILENAME | SND_ASYNC);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");


	// Console welcome message
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("toggleconsole");
	Sleep(1000);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Nova Hook | Private CS:GO Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.6.2");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Developed by: Thaisen");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 0%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Nova Hook | Private CS:GO Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.6.2");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Developed by: Thaisen");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 17%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Nova Hook | Private CS:GO Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.6.2");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Developed by: Thaisen");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 24%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Nova Hook | Private CS:GO Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.6.2");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Developed by: Thaisen");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 41%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Nova Hook | Private CS:GO Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.6.2");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Developed by: Thaisen");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 55%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Nova Hook | Private CS:GO Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.6.2");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Developed by: Thaisen");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 71%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Nova Hook | Private CS:GO Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.6.2");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Developed by: Thaisen");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 88%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Nova Hook | Private CS:GO Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.6.2");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Developed by: Thaisen");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 100%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Nova Hook | Private CS:GO Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.6.2");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Developed by: Thaisen");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: Complete");
	Interfaces::Engine->ClientCmd_Unrestricted("echo");
	Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 0, 255), ("\nNova Hook Color Test\n"));
	Interfaces::Engine->ClientCmd_Unrestricted("echo [Nova Hook]: Welcome To Nova Hook!");
	Interfaces::Engine->ClientCmd_Unrestricted("echo [Nova Hook]: If you want something added talk to Thaisen");
	GUI.LoadWindowState(&Menu::Window, "default.cfg");
	Interfaces::Engine->ClientCmd_Unrestricted("echo [Nova Hook]: Default config Loaded");
	Interfaces::Engine->ClientCmd_Unrestricted("echo");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Nova Hook Change Logs | Version 1.1.6.2 | Date: 2017/10/02");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Legend: + = Added, - = Removed, ~ = Fixed");
	Interfaces::Engine->ClientCmd_Unrestricted("echo");
	Interfaces::Engine->ClientCmd_Unrestricted("echo + New ESP (Custom 2)");
	Interfaces::Engine->ClientCmd_Unrestricted("echo + Replaced The Health ESP!");
	Interfaces::Engine->ClientCmd_Unrestricted("echo + Player Tracers");
	Interfaces::Engine->ClientCmd_Unrestricted("echo + Tracer Color Determinded By Box Color");
	Interfaces::Engine->ClientCmd_Unrestricted("echo + Watermark + Disable Button");
	Interfaces::Engine->ClientCmd_Unrestricted("echo + System Clock Feature");
	Interfaces::Engine->ClientCmd_Unrestricted("echo + Weapon Cham Styles");
	Interfaces::Engine->ClientCmd_Unrestricted("echo ~ Naming Issues In Message Boxes");
	Interfaces::Engine->ClientCmd_Unrestricted("echo ~ Weapon Chams Not Applying Red Values");
	Interfaces::Engine->ClientCmd_Unrestricted("echo ~ Cleaned up this screen method");
	Interfaces::Engine->ClientCmd_Unrestricted("echo ~ Ammo ESP not lining up correct");
	Interfaces::Engine->ClientCmd_Unrestricted("echo ~ Brand new color menu + new color options");
	Interfaces::Engine->ClientCmd_Unrestricted("echo ~ Configs not loading or saving correctly");
	Interfaces::Engine->ClientCmd_Unrestricted("echo");
	Interfaces::Engine->ClientCmd_Unrestricted("echo [Nova Hook] Press Insert to open the menu!");
}

void MovementCorrection(CUserCmd* pCmd)
{

}




/*==========================================================
 _____  _               _   _   _______       _____  _____
/ ____\| |        /\   | \ | | |__   __|/\   / ____|/ ____|
| |    | |       /  \  |  \| |    | |  /  \ | |  __| (___
| |    | |      / /\ \ | . ` |    | | / /\ \| | |_ |\___ \
| |____| |____ / ____ \| |\  |    | |/ ____ \ |__| |____) |
\_____|______ /_/    \ \_| \_|    |_/_/    \_\_____|_____/

===========================================================*/
void SetClanTag(const char* tag, const char* name)//190% paste
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)Utilities::Memory::FindPattern(XorStr("engine.dll"), (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", XorStr("xxxxxxxxx????xxxxxx"))));
	pSetClanTag(tag, name);
}

void NoClantag()
{
	SetClanTag("", "");
}


void ClantagOff()
{
	if (Menu::Window.CustomTab.TagChangerEnable.GetState())
	{
		if (Menu::Window.CustomTab.TagChangerText.GetIndex() == 0)
		{
			SetClanTag(XorStr(""), XorStr(""));
		}
	}
}

void ClantagClear()
{
	if (Menu::Window.CustomTab.TagChangerEnable.GetState())
	{
		if (Menu::Window.CustomTab.TagChangerText.GetIndex() == 1)
		{
			SetClanTag(XorStr("\n\n"), XorStr("\n\n"));
		}
	}
}

void ClantagNovaHook()
{
	if (Menu::Window.CustomTab.TagChangerEnable.GetState())
	{
		if (Menu::Window.CustomTab.TagChangerText.GetIndex() == 2)
		{
			SetClanTag(XorStr("Nova Hook"), XorStr("Nova Hook"));
		}
	}
}

void ClantagAIMWARE()
{
	if (Menu::Window.CustomTab.TagChangerEnable.GetState())
	{
		if (Menu::Window.CustomTab.TagChangerText.GetIndex() == 3)
		{
			SetClanTag(XorStr("AIMWARE.net"), XorStr("AIMWARE.net"));
		}
	}
}

void ClantagValve()
{
	if (Menu::Window.CustomTab.TagChangerEnable.GetState())
	{
		if (Menu::Window.CustomTab.TagChangerText.GetIndex() == 4)
		{
			SetClanTag(XorStr("[VALVe]"), XorStr("[VALVe]"));
		}
	}
}







template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}
const char* clantaganimation[16] =
{
	
};
int kek = 0;
int autism = 0;
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number)
		return true;

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;

		if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		{
			static auto SetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)(Utilities::Memory::FindPatternV2("engine.dll", "53 56 57 8B DA 8B F9 FF 15"))));
			
				static size_t lastTime = 0;

				if (GetTickCount() > lastTime)
				{
					kek++;
					if (kek > 10)
					{
						autism = autism + 1;

						if (autism >= 16)
							autism = 0;

						char random[255];
						SetClanTag(clantaganimation[autism], clantaganimation[autism]);
						lastTime = GetTickCount() + 500;
					}

					if (kek > 10)
						kek = 0;
				}
			
		} 


		if (Menu::Window.CustomTab.SkyboxChangerEnable.GetState())
		{
			if (Menu::Window.CustomTab.SkyboxChangerType.GetIndex() == 0)
			{
				ConVar* sky = Interfaces::CVar->FindVar("sv_skyname");
				SpoofedConvar* sky_spoofed = new SpoofedConvar(sky);
				sky_spoofed->SetString("sky_csgo_night02");
			}
			else if (Menu::Window.CustomTab.SkyboxChangerType.GetIndex() == 1)
			{
				ConVar* sky = Interfaces::CVar->FindVar("sv_skyname");
				SpoofedConvar* sky_spoofed = new SpoofedConvar(sky);
				sky_spoofed->SetString("sky_dust");
			}
			else if (Menu::Window.CustomTab.SkyboxChangerType.GetIndex() == 2)
			{
				ConVar* sky = Interfaces::CVar->FindVar("sv_skyname");
				SpoofedConvar* sky_spoofed = new SpoofedConvar(sky);
				sky_spoofed->SetString("sky_office");
			}
		}


		static bool abc = false;
		if (Menu::Window.VisualsTab.NightMode.GetState())
		{
			if (!abc)
			{
				ConVar* staticdrop = Interfaces::CVar->FindVar("r_DrawSpecificStaticProp");
				SpoofedConvar* staticdrop_spoofed = new SpoofedConvar(staticdrop);
				staticdrop_spoofed->SetInt(0);

				{
					for (MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial(); i != Interfaces::MaterialSystem->InvalidMaterial(); i = Interfaces::MaterialSystem->NextMaterial(i))
					{
						IMaterial *pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

						if (!pMaterial)
							continue;
						
						ConVar* sky = Interfaces::CVar->FindVar("sv_skyname");
						SpoofedConvar* sky_spoofed = new SpoofedConvar(sky);
						sky_spoofed->SetString("sky_csgo_night02");

						float amount = Menu::Window.VisualsTab.NightModeSlider.GetValue();

						if (!strcmp(pMaterial->GetTextureGroupName(), "World textures"))
						{
							pMaterial->ColorModulation(amount, amount, amount);
						}
						if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
						{
							pMaterial->ColorModulation(amount, amount, amount);
						}
					}
				}
			}
			abc = true;
			Menu::Window.VisualsTab.NightMode.SetState(false);
		}
		else
		{
			abc = false;
		}
		
		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive()) {

		
			

				for (int i = 1; i < Interfaces::Engine->GetMaxClients(); ++i)
				{
					IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

					if (!pEntity || !pLocal) {
						continue;
					}

					if (pEntity->GetTeamNum() == pLocal->GetTeamNum()) {
						continue;
					}

					if (pEntity->IsDormant() || !pLocal->IsAlive() || !pEntity->IsAlive()) {
						continue;
					}
					//lagComp->fakeLagFix(pEntity, 3);					
				}
			

				
					//newtestBacktrack->Cache();
				Hacks::MoveHacks(pCmd, bSendPacket);
				

			

			for (int i = 1; i < Interfaces::Engine->GetMaxClients(); ++i)
			{
				IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

				if (!pEntity || !pLocal) {
					continue;
				}

				if (pEntity->GetTeamNum() == pLocal->GetTeamNum()) {
					continue;
				}

				if (pEntity->IsDormant() || !pLocal->IsAlive() || !pEntity->IsAlive()) {
					continue;
				}
				//lagComp->setCurrentEnt(pEntity);
			}
			
		

			
		}

		
		qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f); // if pCmd->viewangles.x > 89, set pCmd->viewangles.x instead of 0.0f on first
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
		Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
		Vector vRightNorm;			Normalize(viewright, vRightNorm);
		Vector vUpNorm;				Normalize(viewup, vUpNorm);

		float forward = pCmd->forwardmove;
		float right = pCmd->sidemove;
		float up = pCmd->upmove;
		if (forward > 450) forward = 450;
		if (right > 450) right = 450;
		if (up > 450) up = 450;
		if (forward < -450) forward = -450;
		if (right < -450) right = -450;
		if (up < -450) up = -450;
		pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
		pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
		pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);
		
		
		

		if (Menu::Window.MiscTab.OtherSafeMode.GetState())
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);


			if (pCmd->viewangles.z != 0.0f)
			{
				pCmd->viewangles.z = 0.00;
			}

			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
			{
				Utilities::Log("Having to re-normalise!");
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
				Beep(750, 800); // Why does it do this
				if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				{
					pCmd->viewangles = origView;
					pCmd->sidemove = right;
					pCmd->forwardmove = forward;
				}
			}
		}
		
		if (pCmd->viewangles.x > 90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		if (pCmd->viewangles.x < -90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		// LBY
		LBYThirdpersonAngle = Vector(pLocal->GetEyeAnglesXY()->x, pLocal->GetLowerBodyYaw(), pLocal->GetEyeAnglesXY()->z);

		if (bSendPacket == true) {
			LastAngleAA = pCmd->viewangles;
		}
		else if (bSendPacket == false) {
			LastAngleAAReal = pCmd->viewangles;
		}


		lineLBY = pLocal->GetLowerBodyYaw();
		if (bSendPacket == true) {
			lineFakeAngle = pCmd->viewangles.y;
		}
		else if (bSendPacket == false) {
			lineRealAngle = pCmd->viewangles.y;
		}
			

		
	}
	
	return false;
}

std::string GetTimeString()
{
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;
}

// Force new models
MDLHandle_t __fastcall hkFindMDL(void* ecx, void* edx, char* FilePath)
{
	
	if (strstr(FilePath, "knife_default_ct.mdl"))
		return oFindMDL(ecx, "models/weapons/v_knife_tri.mdl");

	if (strstr(FilePath, "knife_default_t.mdl"))
		return oFindMDL(ecx, "models/weapons/v_knife_tri.mdl");

	if (strstr(FilePath, "ak47.mdl"))
		return oFindMDL(ecx, "models/weapons/v_rif_beast.mdl");

	if (strstr(FilePath, "m4a4.mdl") || strstr(FilePath, "m4a1.mdl"))
		return oFindMDL(ecx, "models/weapons/v_rif_codolm4.mdl");

	if (strstr(FilePath, "awp.mdl"))
		return oFindMDL(ecx, "models/weapons/v_snip_m2000.mdl");

	if (strstr(FilePath, "ssg08.mdl"))
		return oFindMDL(ecx, "models/weapons/v_snip_m98b.mdl");
		
	return oFindMDL(ecx, FilePath);
}



// Paint Traverse Hooked function
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
	{
		if (Menu::Window.VisualsTab.NoScopeBorder.GetState() && strcmp("HudZoom", Interfaces::Panels->GetName(vguiPanel)) == 0)
			return;

		if (Menu::Window.VisualsTab.NoScopeBorder.GetState())
		{
			if (hackManager.pLocal()->IsAlive() && hackManager.pLocal()->IsScoped())
			{
				int width, height;
				Interfaces::Engine->GetScreenSize(width, height);
				Render::Line(width / 2, 0, width / 2, height, Color(0, 0, 0, 255));
				Render::Line(0, height / 2, width, height / 2, Color(0, 0, 0, 255));
			}
		}
	}

	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, "MatSystemTopPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{

		if (Menu::Window.VisualsTab.Clock.GetState())
		{
			int cY;

			if (Menu::Window.VisualsTab.Watermark.GetState())
			{
				cY = 7;
			}
			else if (!Menu::Window.VisualsTab.Watermark.GetState())
			{
				cY = 26;
			}

			Render::Textf(7, cY, Color(255, 255, 255, 255), Render::Fonts::TopScreen, ("%s"), GetTimeString().c_str());
		}

		if (!Menu::Window.VisualsTab.Watermark.GetState())
		{
			Render::Text(7, 7, Color(255, 255, 255, 255), Render::Fonts::TopScreen, ("Nova Hook | Private CS:GO Cheat"));


			int ScreenWidth, ScreenHeight;
			Interfaces::Engine->GetScreenSize(ScreenWidth, ScreenHeight);

			int x = (int)(ScreenWidth * 0.5f);
			int y = 0;

			y = ScreenHeight;

		}
				
		if (Menu::Window.CustomTab.HitmarkerEnable.GetState())
		{
			if (hitmarkertime > 0)
			{
				RECT View = Render::GetViewport();
				int MidX = View.right;
				int MidY = View.bottom;

				float alpha = hitmarkertime;
				IGameEvent* pEvent;

				Render::Line(MidX - 5, MidY - 5, MidX + 5, MidY + 5, Color(Menu::Window.SettingsTab.HitmarkerR.GetValue(), Menu::Window.SettingsTab.HitmarkerG.GetValue(), Menu::Window.SettingsTab.HitmarkerB.GetValue(), alpha));
				Render::Line(MidX + 5, MidY - 5, MidX - 5, MidY + 5, Color(Menu::Window.SettingsTab.HitmarkerR.GetValue(), Menu::Window.SettingsTab.HitmarkerG.GetValue(), Menu::Window.SettingsTab.HitmarkerB.GetValue(), alpha));

				hitmarkertime -= 2;
			}
		}

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
			Hacks::DrawHacks();
		
		// Update and draw the menu
		Menu::DoUIFrame();
	}
}

// InPrediction Hooked Function
bool __stdcall Hooked_InPrediction()
{
	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::VMTPrediction.GetOriginalFunction(14);
	static DWORD *ecxVal = Interfaces::Prediction;
	result = origFunc(ecxVal);

	if (Menu::Window.VisualsTab.OtherNoVisualRecoil.GetState() && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
	{
		IClientEntity* pLocalEntity = NULL;

		float* m_LocalViewAngles = NULL;

		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}

		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();

		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}

	return result;
}

// DrawModelExec for chams and shit
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	Color color;
	float flColor[3] = { 0.f };
	static IMaterial* CoveredLit = CreateMaterial(true);
	static IMaterial* OpenLit = CreateMaterial(false);
	static IMaterial* CoveredFlat = CreateMaterial(true, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false);
	bool DontDraw = false;

	const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);
	IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Menu::Window.VisualsTab.Active.GetState())
	{
		int ChamsStyle = Menu::Window.VisualsTab.OptionsChams.GetIndex();
		int HandsStyle = Menu::Window.VisualsTab.OtherNoHands.GetIndex();
		int WeaponStyle = Menu::Window.VisualsTab.LWeaponChams.GetIndex();
		if (ChamsStyle != 0 && Menu::Window.VisualsTab.FiltersPlayers.GetState() && strstr(ModelName, "models/player"))
		{
			if (pModelEntity == nullptr)
				return;

			if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() || pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;

				IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
				if (pModelEntity)
				{
					IClientEntity *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
					if (local)
					{
						if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 && pModelEntity->GetTeamNum() != local->GetTeamNum())
						{
							float alpha = 1.f;

							if (pModelEntity->HasGunGameImmunity())
								alpha = 0.5f;

							if (pModelEntity->GetTeamNum() == 2)
							{
								flColor[0] = Menu::Window.SettingsTab.TChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.SettingsTab.TChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.SettingsTab.TChamsB.GetValue() / 255.f;
							}
							else
							{
								flColor[0] = Menu::Window.SettingsTab.CTChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.SettingsTab.CTChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.SettingsTab.CTChamsB.GetValue() / 255.f;
							}

							if (Menu::Window.VisualsTab.PXQZ.GetState())
							{
								Interfaces::RenderView->SetColorModulation(flColor);
								Interfaces::RenderView->SetBlend(0);
								Interfaces::ModelRender->ForcedMaterialOverride(covered);
								oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							else
							{
								Interfaces::RenderView->SetColorModulation(flColor);
								Interfaces::RenderView->SetBlend(1);
								Interfaces::ModelRender->ForcedMaterialOverride(covered);
								oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}

							if (pModelEntity->GetTeamNum() == 2)
							{
								flColor[0] = Menu::Window.SettingsTab.TChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.SettingsTab.TChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.SettingsTab.TChamsB.GetValue() / 255.f;
							}
							else
							{
								flColor[0] = Menu::Window.SettingsTab.CTChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.SettingsTab.CTChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.SettingsTab.CTChamsB.GetValue() / 255.f;
							}

							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(open);
						}
						else
						{
							color.SetColor(255, 255, 255, 255);
							ForceMaterial(color, open);
						}
					}
				}
			}
		}
		else if (HandsStyle != 0 && strstr(ModelName, "arms"))
		{
			if (HandsStyle == 1)
			{
				DontDraw = true;
			}
			else if (HandsStyle == 2)
			{
				flColor[0] = Menu::Window.SettingsTab.HandsR.GetValue() / 255.f;
				flColor[1] = Menu::Window.SettingsTab.HandsG.GetValue() / 255.f;
				flColor[2] = Menu::Window.SettingsTab.HandsB.GetValue() / 255.f;
				Interfaces::RenderView->SetColorModulation(flColor);
				Interfaces::RenderView->SetBlend(0.8f);
			}
			else if (HandsStyle == 3)
			{
				flColor[0] = Menu::Window.SettingsTab.HandsR.GetValue() / 255.f;
				flColor[1] = Menu::Window.SettingsTab.HandsG.GetValue() / 255.f;
				flColor[2] = Menu::Window.SettingsTab.HandsB.GetValue() / 255.f;

				Interfaces::RenderView->SetColorModulation(flColor);
				static IMaterial* wire = CreateMaterial(true, false, true);
				IMaterial *handy = wire;
				handy->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				Interfaces::ModelRender->ForcedMaterialOverride(handy);
			}
			else if (HandsStyle == 4)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(Menu::Window.SettingsTab.HandsR.GetValue(), Menu::Window.SettingsTab.HandsG.GetValue(), Menu::Window.SettingsTab.HandsB.GetValue(), alpha);
						else
							color.SetColor(Menu::Window.SettingsTab.HandsR.GetValue(), Menu::Window.SettingsTab.HandsG.GetValue(), Menu::Window.SettingsTab.HandsB.GetValue(), alpha);

						ForceMaterial(color, open);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(Menu::Window.SettingsTab.HandsR.GetValue(), Menu::Window.SettingsTab.HandsG.GetValue(), Menu::Window.SettingsTab.HandsB.GetValue(), alpha);
						else
							color.SetColor(Menu::Window.SettingsTab.HandsR.GetValue(), Menu::Window.SettingsTab.HandsG.GetValue(), Menu::Window.SettingsTab.HandsB.GetValue(), alpha);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			else
			{
				static int counter = 0;
				static float colors[3] = { 1.f, 0.f, 0.f };

				if (colors[counter] >= 1.0f)
				{
					colors[counter] = 1.0f;
					counter += 1;
					if (counter > 2)
						counter = 0;
				}
				else
				{
					int prev = counter - 1;
					if (prev < 0) prev = 2;
					colors[prev] -= 0.01f;
					colors[counter] += 0.01f;
				}

				Interfaces::RenderView->SetColorModulation(colors);
				Interfaces::RenderView->SetBlend(1);
				Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
			}
		}
		else if (WeaponStyle != 0 && strstr(ModelName, "models/weapons/v_") && !strstr(ModelName, "arms"))
		{
			if (WeaponStyle == 1)
			{
				DontDraw = true;
			}
			else if (WeaponStyle == 2)
			{
				flColor[0] = Menu::Window.SettingsTab.WeaponR.GetValue() / 255.f;
				flColor[1] = Menu::Window.SettingsTab.WeaponG.GetValue() / 255.f;
				flColor[2] = Menu::Window.SettingsTab.WeaponB.GetValue() / 255.f;
				Interfaces::RenderView->SetColorModulation(flColor);
				Interfaces::RenderView->SetBlend(0.8f);
			}
			else if (WeaponStyle == 3)
			{
				flColor[0] = Menu::Window.SettingsTab.WeaponR.GetValue() / 255.f;
				flColor[1] = Menu::Window.SettingsTab.WeaponG.GetValue() / 255.f;
				flColor[2] = Menu::Window.SettingsTab.WeaponB.GetValue() / 255.f;

				Interfaces::RenderView->SetColorModulation(flColor);
				static IMaterial* wire = CreateMaterial(true, false, true);
				IMaterial *handy = wire;
				handy->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				Interfaces::ModelRender->ForcedMaterialOverride(handy);
			}
			else if (WeaponStyle == 4)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(Menu::Window.SettingsTab.WeaponR.GetValue(), Menu::Window.SettingsTab.WeaponG.GetValue(), Menu::Window.SettingsTab.WeaponB.GetValue(), alpha);
						else
							color.SetColor(Menu::Window.SettingsTab.WeaponR.GetValue(), Menu::Window.SettingsTab.WeaponG.GetValue(), Menu::Window.SettingsTab.WeaponB.GetValue(), alpha);

						ForceMaterial(color, open);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(Menu::Window.SettingsTab.WeaponR.GetValue(), Menu::Window.SettingsTab.WeaponG.GetValue(), Menu::Window.SettingsTab.WeaponB.GetValue(), alpha);
						else
							color.SetColor(Menu::Window.SettingsTab.WeaponR.GetValue(), Menu::Window.SettingsTab.WeaponG.GetValue(), Menu::Window.SettingsTab.WeaponB.GetValue(), alpha);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			else
			{
				static int counter = 0;
				static float colors[3] = { 1.f, 0.f, 0.f };

				if (colors[counter] >= 1.0f)
				{
					colors[counter] = 1.0f;
					counter += 1;
					if (counter > 2)
						counter = 0;
				}
				else
				{
					int prev = counter - 1;
					if (prev < 0) prev = 2;
					colors[prev] -= 0.01f;
					colors[counter] += 0.01f;
				}

				Interfaces::RenderView->SetColorModulation(colors);
				Interfaces::RenderView->SetBlend(1);
				Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
			}
		}
	}

	if (!DontDraw)
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(NULL);
}


#define TICKS_TO_TIME(t) (Interfaces::Globals->interval_per_tick * (t) )


// Hooked FrameStageNotify for removing visual recoil
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{

	DWORD eyeangles = NetVar.GetNetVar(0xBFEA4E7B);
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	static bool vistoggle;
	static float mememe;
	int VisToggleKey = Menu::Window.MiscTab.VisToggle.GetKey();
	if (VisToggleKey >= 0 && GUI.GetKeyState(VisToggleKey) && abs(mememe - Interfaces::Globals->curtime) > 1)
	{
		if (Menu::Window.VisualsTab.Active.GetState() == false)
		{
			Menu::Window.VisualsTab.Active.SetState(true);
			mememe = Interfaces::Globals->curtime;
		}
		else if (Menu::Window.VisualsTab.Active.GetState() == true)
		{
			Menu::Window.VisualsTab.Active.SetState(false);
			mememe = Interfaces::Globals->curtime;
		}
	}


	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_RENDER_START)
	{
		if (pLocal->IsAlive() && Menu::Window.MiscTab.OtherThirdperson.GetState())
		{
			Vector thirdpersonMode;

			switch (Menu::Window.MiscTab.ThirdpersonAngle.GetIndex())
			{
			case 0:
				
				thirdpersonMode = LastAngleAAReal;
				break;
			case 1:
				thirdpersonMode = LastAngleAA;
				break;
			case 2:
				thirdpersonMode = LBYThirdpersonAngle;
				break;
			}

			
			static bool rekt = false;
			if (!rekt)
			{
				ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
				SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
				sv_cheats_spoofed->SetInt(1);
				rekt = true;
			}

			static bool kek = false;
			
			if (!kek)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("thirdperson");
				kek = true;
			}

			static bool toggleThirdperson;
			static float memeTime;
			int ThirdPersonKey = Menu::Window.MiscTab.ThirdPersonKeyBind.GetKey();
			if (ThirdPersonKey >= 0 && GUI.GetKeyState(ThirdPersonKey) && abs(memeTime - Interfaces::Globals->curtime) > 0.5)
			{
				toggleThirdperson = !toggleThirdperson;
				memeTime = Interfaces::Globals->curtime;
			}
		

			if (toggleThirdperson)
			{
				Interfaces::pInput->m_fCameraInThirdPerson = true;
				if (*(bool*)((DWORD)Interfaces::pInput + 0xA5))
					*(Vector*)((DWORD)pLocal + 0x31C8) = thirdpersonMode;
			}
			else {
				// No Thirdperson
				static Vector vecAngles;
				Interfaces::Engine->GetViewAngles(vecAngles);
				Interfaces::pInput->m_fCameraInThirdPerson = false;
				Interfaces::pInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
			}
			

		}
		else if (pLocal->IsAlive() == 0)
		{
			kek = false;

		}	

		if (!Menu::Window.MiscTab.OtherThirdperson.GetState()) {

			// No Thirdperson
			static Vector vecAngles;
			Interfaces::Engine->GetViewAngles(vecAngles);
			Interfaces::pInput->m_fCameraInThirdPerson = false;
			Interfaces::pInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
		}
	
}

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {

		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
		
		static float nextLBYUpdate;
		float serverTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;
		static float oldlby;
		static float lastUpdatetimeLBY;



		if (consoleProxyLbyLASTUpdateTime <= 0) { 

			nextLBYUpdate = 0;
			testFloat4 = 0;

		}
		else {
					
			if (pLocal->IsAlive()) {

				if (oldlby != pLocal->GetLowerBodyYaw()) {
					lastUpdatetimeLBY = serverTime;
					oldlby = pLocal->GetLowerBodyYaw();
				}
			
				if (rWeInFakeWalk == true) {

					if (abs(nextLBYUpdate - lastUpdatetimeLBY) >= 1.1) {

						LBYBreakerTimer++;

					}

					islbyupdate = true;

				}
				else if (pLocal->GetVelocity().Length2D() > 0.1f && pLocal->GetFlags() & FL_ONGROUND) {
					
					if (serverTime >= nextLBYUpdate) {
						nextLBYUpdate = serverTime + 0.22000001;

					}

					islbyupdate = false;

				}
				else {

					if (serverTime > nextLBYUpdate && pLocal->GetFlags() & FL_ONGROUND && pLocal->GetVelocity().Length2D() < 0.1f) {

						nextLBYUpdate = serverTime + 1.1;

						if (abs(nextLBYUpdate - lastUpdatetimeLBY) >= 1.1) {

							LBYBreakerTimer++;
							
						}
						
						if (abs(pLocal->GetEyeAnglesXY()->y - pLocal->GetLowerBodyYaw()) < 35) {

							islbyupdate = true;
							

						}
						else {

							islbyupdate = false;
							

						}


					}
					else {
						islbyupdate = true;
					}
					


				}


			} 

		}
	/* Extra shit resolver		
		if (Menu::Window.RageBotTab.AimbotExtraResolver.GetState()) {

			for (int i = 0; i < Interfaces::Engine->GetMaxClients(); ++i)
			{
				IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

				if (!pEntity || !pLocal) {
					continue;
				}

				if (pEntity->GetTeamNum() == pLocal->GetTeamNum()) {
					continue;
				}

				if (pEntity->IsDormant() || !pLocal->IsAlive()) {
					continue;
				}

				CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pEntity->GetActiveWeaponHandle());
				if (pWeapon)
					enemysCurrentAmmo[pEntity->GetIndex()] = pWeapon->GetAmmoInClip();

				
				float yaw = g_Resolver->ResolveYaw(pEntity, pEntity->GetEyeAnglesXY()->y);
				pEntity->GetEyeAnglesXY()->y = yaw;
				
				
				// Log missed Hits for every Entity
				missedLogHits[pEntity->GetIndex()] = abs(shotsfired - hittedLogHits[pEntity->GetIndex()]);

			}

		}

*/

		/*SKINCHANGER*/

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

		for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++) // CHANGE
		{
			
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (pEntity)
			{

				

				ULONG hOwnerEntity = *(PULONG)((DWORD)pEntity + 0x148);

				IClientEntity* pOwner = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hOwnerEntity);
				

				
				if (pOwner)
				{

					if (pOwner == pLocal)
					{
						
						CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;
						

						ClientClass *pClass = Interfaces::Client->GetAllClasses();
						
						
						

								if (Menu::Window.MiscTab.SkinEnable.GetState())
								{			
									int Model = Menu::Window.MiscTab.KnifeModel.GetIndex();
									if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
									{





										if (Model == 0) // Bayonet
										{
											*pWeapon->ModelIndex() = iBayonet; // m_nModelIndex
											*pWeapon->ViewModelIndex() = iBayonet;
											*pWeapon->WorldModelIndex() = iBayonet + 1;
											*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 500;
											*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

											int Skin = Menu::Window.MiscTab.KnifeSkin.GetIndex();

											if (Skin == 0)
											{
												*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
											}
											else if (Skin == 1)
											{
												*pWeapon->FallbackPaintKit() = 12; // Crimson Web
											}
											
											else if (Skin == 2)
											{
												*pWeapon->FallbackPaintKit() = 38; // Fade
											}
											else if (Skin == 3)
											{
												*pWeapon->FallbackPaintKit() = 40; // Night
											}
											else if (Skin == 4)
											{
												*pWeapon->FallbackPaintKit() = 42; // Blue Steel
											}
											else if (Skin == 5)
											{
												*pWeapon->FallbackPaintKit() = 43; // Stained
											}
											else if (Skin == 6)
											{
												*pWeapon->FallbackPaintKit() = 44; // Case Hardened
											}
											else if (Skin == 8)
											{
												*pWeapon->FallbackPaintKit() = 59; // Slaughter
											}
											
											else if (Skin == 9)
											{
												*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
											}
											
											else if (Skin == 10)
											{
												*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
											}
											else if (Skin == 11)
											{
												*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
											}
											
											else if (Skin == 12)
											{
												*pWeapon->FallbackPaintKit() = 413; // Marble Fade
											}
											else if (Skin == 13)
											{
												*pWeapon->FallbackPaintKit() = 414; // Rust Coat
											}
											else if (Skin == 14)
											{
												*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
											}
											else if (Skin == 15)
											{
												*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
											}
											else if (Skin == 16)
											{
												*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
											}
											else if (Skin == 17)
											{
												*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
											}
											else if (Skin == 18)
											{
												*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
											}
											else if (Skin == 19)
											{
												*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
											}
											else if (Skin == 20)
											{
												*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
											}
											else if (Skin == 21)
											{
												*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
											}
											else if (Skin == 22)
											{
												*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
											}
											else if (Skin == 23)
											{
												*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
											}
											else if (Skin == 24)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
											}
											else if (Skin == 25)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
											}
											else if (Skin == 26)
											{
												*pWeapon->FallbackPaintKit() = 558; // Lore
											}
										}
										else if (Model == 1) // Bowie Knife
										{
											*pWeapon->ModelIndex() = iBowie; // m_nModelIndex
											*pWeapon->ViewModelIndex() = iBowie;
											*pWeapon->WorldModelIndex() = iBowie + 1;
											*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 514;
											*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

											int Skin = Menu::Window.MiscTab.KnifeSkin.GetIndex();

											if (Skin == 0)
											{
												*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
											}
											else if (Skin == 1)
											{
												*pWeapon->FallbackPaintKit() = 12; // Crimson Web
											}

											else if (Skin == 2)
											{
												*pWeapon->FallbackPaintKit() = 38; // Fade
											}
											else if (Skin == 3)
											{
												*pWeapon->FallbackPaintKit() = 40; // Night
											}
											else if (Skin == 4)
											{
												*pWeapon->FallbackPaintKit() = 42; // Blue Steel
											}
											else if (Skin == 5)
											{
												*pWeapon->FallbackPaintKit() = 43; // Stained
											}
											else if (Skin == 6)
											{
												*pWeapon->FallbackPaintKit() = 44; // Case Hardened
											}
											else if (Skin == 8)
											{
												*pWeapon->FallbackPaintKit() = 59; // Slaughter
											}

											else if (Skin == 9)
											{
												*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
											}

											else if (Skin == 10)
											{
												*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
											}
											else if (Skin == 11)
											{
												*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
											}

											else if (Skin == 12)
											{
												*pWeapon->FallbackPaintKit() = 413; // Marble Fade
											}
											else if (Skin == 13)
											{
												*pWeapon->FallbackPaintKit() = 414; // Rust Coat
											}
											else if (Skin == 14)
											{
												*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
											}
											else if (Skin == 15)
											{
												*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
											}
											else if (Skin == 16)
											{
												*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
											}
											else if (Skin == 17)
											{
												*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
											}
											else if (Skin == 18)
											{
												*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
											}
											else if (Skin == 19)
											{
												*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
											}
											else if (Skin == 20)
											{
												*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
											}
											else if (Skin == 21)
											{
												*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
											}
											else if (Skin == 22)
											{
												*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
											}
											else if (Skin == 23)
											{
												*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
											}
											else if (Skin == 24)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
											}
											else if (Skin == 25)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
											}
											else if (Skin == 26)
											{
												*pWeapon->FallbackPaintKit() = 558; // Lore
											}

										}
										else if (Model == 2) // Butterfly Knife
										{
											*pWeapon->ModelIndex() = iButterfly; // m_nModelIndex
											*pWeapon->ViewModelIndex() = iButterfly;
											*pWeapon->WorldModelIndex() = iButterfly + 1;
											*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 515;
											*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

											int Skin = Menu::Window.MiscTab.KnifeSkin.GetIndex();

											if (Skin == 0)
											{
												*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
											}
											else if (Skin == 1)
											{
												*pWeapon->FallbackPaintKit() = 12; // Crimson Web
											}

											else if (Skin == 2)
											{
												*pWeapon->FallbackPaintKit() = 38; // Fade
											}
											else if (Skin == 3)
											{
												*pWeapon->FallbackPaintKit() = 40; // Night
											}
											else if (Skin == 4)
											{
												*pWeapon->FallbackPaintKit() = 42; // Blue Steel
											}
											else if (Skin == 5)
											{
												*pWeapon->FallbackPaintKit() = 43; // Stained
											}
											else if (Skin == 6)
											{
												*pWeapon->FallbackPaintKit() = 44; // Case Hardened
											}
											else if (Skin == 8)
											{
												*pWeapon->FallbackPaintKit() = 59; // Slaughter
											}

											else if (Skin == 9)
											{
												*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
											}

											else if (Skin == 10)
											{
												*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
											}
											else if (Skin == 11)
											{
												*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
											}

											else if (Skin == 12)
											{
												*pWeapon->FallbackPaintKit() = 413; // Marble Fade
											}
											else if (Skin == 13)
											{
												*pWeapon->FallbackPaintKit() = 414; // Rust Coat
											}
											else if (Skin == 14)
											{
												*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
											}
											else if (Skin == 15)
											{
												*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
											}
											else if (Skin == 16)
											{
												*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
											}
											else if (Skin == 17)
											{
												*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
											}
											else if (Skin == 18)
											{
												*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
											}
											else if (Skin == 19)
											{
												*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
											}
											else if (Skin == 20)
											{
												*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
											}
											else if (Skin == 21)
											{
												*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
											}
											else if (Skin == 22)
											{
												*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
											}
											else if (Skin == 23)
											{
												*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
											}
											else if (Skin == 24)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
											}
											else if (Skin == 25)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
											}
											else if (Skin == 26)
											{
												*pWeapon->FallbackPaintKit() = 558; // Lore
											}

										}
										else if (Model == 3) // Falchion Knife
										{
											*pWeapon->ModelIndex() = iFalchion; // m_nModelIndex
											*pWeapon->ViewModelIndex() = iFalchion;
											*pWeapon->WorldModelIndex() = iFalchion + 1;
											*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 512;
											*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

											int Skin = Menu::Window.MiscTab.KnifeSkin.GetIndex();

											if (Skin == 0)
											{
												*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
											}
											else if (Skin == 1)
											{
												*pWeapon->FallbackPaintKit() = 12; // Crimson Web
											}

											else if (Skin == 2)
											{
												*pWeapon->FallbackPaintKit() = 38; // Fade
											}
											else if (Skin == 3)
											{
												*pWeapon->FallbackPaintKit() = 40; // Night
											}
											else if (Skin == 4)
											{
												*pWeapon->FallbackPaintKit() = 42; // Blue Steel
											}
											else if (Skin == 5)
											{
												*pWeapon->FallbackPaintKit() = 43; // Stained
											}
											else if (Skin == 6)
											{
												*pWeapon->FallbackPaintKit() = 44; // Case Hardened
											}
											else if (Skin == 8)
											{
												*pWeapon->FallbackPaintKit() = 59; // Slaughter
											}

											else if (Skin == 9)
											{
												*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
											}

											else if (Skin == 10)
											{
												*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
											}
											else if (Skin == 11)
											{
												*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
											}

											else if (Skin == 12)
											{
												*pWeapon->FallbackPaintKit() = 413; // Marble Fade
											}
											else if (Skin == 13)
											{
												*pWeapon->FallbackPaintKit() = 414; // Rust Coat
											}
											else if (Skin == 14)
											{
												*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
											}
											else if (Skin == 15)
											{
												*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
											}
											else if (Skin == 16)
											{
												*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
											}
											else if (Skin == 17)
											{
												*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
											}
											else if (Skin == 18)
											{
												*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
											}
											else if (Skin == 19)
											{
												*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
											}
											else if (Skin == 20)
											{
												*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
											}
											else if (Skin == 21)
											{
												*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
											}
											else if (Skin == 22)
											{
												*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
											}
											else if (Skin == 23)
											{
												*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
											}
											else if (Skin == 24)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
											}
											else if (Skin == 25)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
											}
											else if (Skin == 26)
											{
												*pWeapon->FallbackPaintKit() = 558; // Lore
											}
										}
										else if (Model == 4) // Flip Knife
										{
											*pWeapon->ModelIndex() = iFlip; // m_nModelIndex
											*pWeapon->ViewModelIndex() = iFlip;
											*pWeapon->WorldModelIndex() = iFlip + 1;
											*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 505;
											*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

											int Skin = Menu::Window.MiscTab.KnifeSkin.GetIndex();

											if (Skin == 0)
											{
												*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
											}
											else if (Skin == 1)
											{
												*pWeapon->FallbackPaintKit() = 12; // Crimson Web
											}

											else if (Skin == 2)
											{
												*pWeapon->FallbackPaintKit() = 38; // Fade
											}
											else if (Skin == 3)
											{
												*pWeapon->FallbackPaintKit() = 40; // Night
											}
											else if (Skin == 4)
											{
												*pWeapon->FallbackPaintKit() = 42; // Blue Steel
											}
											else if (Skin == 5)
											{
												*pWeapon->FallbackPaintKit() = 43; // Stained
											}
											else if (Skin == 6)
											{
												*pWeapon->FallbackPaintKit() = 44; // Case Hardened
											}
											else if (Skin == 8)
											{
												*pWeapon->FallbackPaintKit() = 59; // Slaughter
											}

											else if (Skin == 9)
											{
												*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
											}

											else if (Skin == 10)
											{
												*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
											}
											else if (Skin == 11)
											{
												*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
											}

											else if (Skin == 12)
											{
												*pWeapon->FallbackPaintKit() = 413; // Marble Fade
											}
											else if (Skin == 13)
											{
												*pWeapon->FallbackPaintKit() = 414; // Rust Coat
											}
											else if (Skin == 14)
											{
												*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
											}
											else if (Skin == 15)
											{
												*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
											}
											else if (Skin == 16)
											{
												*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
											}
											else if (Skin == 17)
											{
												*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
											}
											else if (Skin == 18)
											{
												*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
											}
											else if (Skin == 19)
											{
												*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
											}
											else if (Skin == 20)
											{
												*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
											}
											else if (Skin == 21)
											{
												*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
											}
											else if (Skin == 22)
											{
												*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
											}
											else if (Skin == 23)
											{
												*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
											}
											else if (Skin == 24)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
											}
											else if (Skin == 25)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
											}
											else if (Skin == 26)
											{
												*pWeapon->FallbackPaintKit() = 558; // Lore
											}

										}
										else if (Model == 5) // Gut Knife
										{
											*pWeapon->ModelIndex() = iGut; // m_nModelIndex
											*pWeapon->ViewModelIndex() = iGut;
											*pWeapon->WorldModelIndex() = iGut + 1;
											*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 506;
											*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

											int Skin = Menu::Window.MiscTab.KnifeSkin.GetIndex();

											if (Skin == 0)
											{
												*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
											}
											else if (Skin == 1)
											{
												*pWeapon->FallbackPaintKit() = 12; // Crimson Web
											}

											else if (Skin == 2)
											{
												*pWeapon->FallbackPaintKit() = 38; // Fade
											}
											else if (Skin == 3)
											{
												*pWeapon->FallbackPaintKit() = 40; // Night
											}
											else if (Skin == 4)
											{
												*pWeapon->FallbackPaintKit() = 42; // Blue Steel
											}
											else if (Skin == 5)
											{
												*pWeapon->FallbackPaintKit() = 43; // Stained
											}
											else if (Skin == 6)
											{
												*pWeapon->FallbackPaintKit() = 44; // Case Hardened
											}
											else if (Skin == 8)
											{
												*pWeapon->FallbackPaintKit() = 59; // Slaughter
											}

											else if (Skin == 9)
											{
												*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
											}

											else if (Skin == 10)
											{
												*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
											}
											else if (Skin == 11)
											{
												*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
											}

											else if (Skin == 12)
											{
												*pWeapon->FallbackPaintKit() = 413; // Marble Fade
											}
											else if (Skin == 13)
											{
												*pWeapon->FallbackPaintKit() = 414; // Rust Coat
											}
											else if (Skin == 14)
											{
												*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
											}
											else if (Skin == 15)
											{
												*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
											}
											else if (Skin == 16)
											{
												*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
											}
											else if (Skin == 17)
											{
												*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
											}
											else if (Skin == 18)
											{
												*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
											}
											else if (Skin == 19)
											{
												*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
											}
											else if (Skin == 20)
											{
												*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
											}
											else if (Skin == 21)
											{
												*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
											}
											else if (Skin == 22)
											{
												*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
											}
											else if (Skin == 23)
											{
												*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
											}
											else if (Skin == 24)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
											}
											else if (Skin == 25)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
											}
											else if (Skin == 26)
											{
												*pWeapon->FallbackPaintKit() = 558; // Lore
											}


										}
										else if (Model == 6) // Huntsman Knife
										{
											*pWeapon->ModelIndex() = iHuntsman; // m_nModelIndex
											*pWeapon->ViewModelIndex() = iHuntsman;
											*pWeapon->WorldModelIndex() = iHuntsman + 1;
											*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 509;
											*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

											int Skin = Menu::Window.MiscTab.KnifeSkin.GetIndex();

											if (Skin == 0)
											{
												*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
											}
											else if (Skin == 1)
											{
												*pWeapon->FallbackPaintKit() = 12; // Crimson Web
											}

											else if (Skin == 2)
											{
												*pWeapon->FallbackPaintKit() = 38; // Fade
											}
											else if (Skin == 3)
											{
												*pWeapon->FallbackPaintKit() = 40; // Night
											}
											else if (Skin == 4)
											{
												*pWeapon->FallbackPaintKit() = 42; // Blue Steel
											}
											else if (Skin == 5)
											{
												*pWeapon->FallbackPaintKit() = 43; // Stained
											}
											else if (Skin == 6)
											{
												*pWeapon->FallbackPaintKit() = 44; // Case Hardened
											}
											else if (Skin == 8)
											{
												*pWeapon->FallbackPaintKit() = 59; // Slaughter
											}

											else if (Skin == 9)
											{
												*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
											}

											else if (Skin == 10)
											{
												*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
											}
											else if (Skin == 11)
											{
												*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
											}

											else if (Skin == 12)
											{
												*pWeapon->FallbackPaintKit() = 413; // Marble Fade
											}
											else if (Skin == 13)
											{
												*pWeapon->FallbackPaintKit() = 414; // Rust Coat
											}
											else if (Skin == 14)
											{
												*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
											}
											else if (Skin == 15)
											{
												*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
											}
											else if (Skin == 16)
											{
												*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
											}
											else if (Skin == 17)
											{
												*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
											}
											else if (Skin == 18)
											{
												*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
											}
											else if (Skin == 19)
											{
												*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
											}
											else if (Skin == 20)
											{
												*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
											}
											else if (Skin == 21)
											{
												*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
											}
											else if (Skin == 22)
											{
												*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
											}
											else if (Skin == 23)
											{
												*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
											}
											else if (Skin == 24)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
											}
											else if (Skin == 25)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
											}
											else if (Skin == 26)
											{
												*pWeapon->FallbackPaintKit() = 558; // Lore
											}


										}
										else if (Model == 7) // Karambit
										{
											*pWeapon->ModelIndex() = iKarambit; // m_nModelIndex
											*pWeapon->ViewModelIndex() = iKarambit;
											*pWeapon->WorldModelIndex() = iKarambit + 1;
											*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 507;
											*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

											int Skin = Menu::Window.MiscTab.KnifeSkin.GetIndex();

											if (Skin == 0)
											{
												*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
											}
											else if (Skin == 1)
											{
												*pWeapon->FallbackPaintKit() = 12; // Crimson Web
											}

											else if (Skin == 2)
											{
												*pWeapon->FallbackPaintKit() = 38; // Fade
											}
											else if (Skin == 3)
											{
												*pWeapon->FallbackPaintKit() = 40; // Night
											}
											else if (Skin == 4)
											{
												*pWeapon->FallbackPaintKit() = 42; // Blue Steel
											}
											else if (Skin == 5)
											{
												*pWeapon->FallbackPaintKit() = 43; // Stained
											}
											else if (Skin == 6)
											{
												*pWeapon->FallbackPaintKit() = 44; // Case Hardened
											}
											else if (Skin == 8)
											{
												*pWeapon->FallbackPaintKit() = 59; // Slaughter
											}

											else if (Skin == 9)
											{
												*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
											}

											else if (Skin == 10)
											{
												*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
											}
											else if (Skin == 11)
											{
												*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
											}

											else if (Skin == 12)
											{
												*pWeapon->FallbackPaintKit() = 413; // Marble Fade
											}
											else if (Skin == 13)
											{
												*pWeapon->FallbackPaintKit() = 414; // Rust Coat
											}
											else if (Skin == 14)
											{
												*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
											}
											else if (Skin == 15)
											{
												*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
											}
											else if (Skin == 16)
											{
												*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
											}
											else if (Skin == 17)
											{
												*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
											}
											else if (Skin == 18)
											{
												*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
											}
											else if (Skin == 19)
											{
												*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
											}
											else if (Skin == 20)
											{
												*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
											}
											else if (Skin == 21)
											{
												*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
											}
											else if (Skin == 22)
											{
												*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
											}
											else if (Skin == 23)
											{
												*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
											}
											else if (Skin == 24)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
											}
											else if (Skin == 25)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
											}
											else if (Skin == 26)
											{
												*pWeapon->FallbackPaintKit() = 558; // Lore
											}

										}
										else if (Model == 8) // M9 Bayonet
										{
											*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
											*pWeapon->ViewModelIndex() = iM9Bayonet;
											*pWeapon->WorldModelIndex() = iM9Bayonet + 1;
											*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 508;
											*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

											int Skin = Menu::Window.MiscTab.KnifeSkin.GetIndex();

											if (Skin == 0)
											{
												*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
											}
											else if (Skin == 1)
											{
												*pWeapon->FallbackPaintKit() = 12; // Crimson Web
											}

											else if (Skin == 2)
											{
												*pWeapon->FallbackPaintKit() = 38; // Fade
											}
											else if (Skin == 3)
											{
												*pWeapon->FallbackPaintKit() = 40; // Night
											}
											else if (Skin == 4)
											{
												*pWeapon->FallbackPaintKit() = 42; // Blue Steel
											}
											else if (Skin == 5)
											{
												*pWeapon->FallbackPaintKit() = 43; // Stained
											}
											else if (Skin == 6)
											{
												*pWeapon->FallbackPaintKit() = 44; // Case Hardened
											}
											else if (Skin == 8)
											{
												*pWeapon->FallbackPaintKit() = 59; // Slaughter
											}

											else if (Skin == 9)
											{
												*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
											}

											else if (Skin == 10)
											{
												*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
											}
											else if (Skin == 11)
											{
												*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
											}

											else if (Skin == 12)
											{
												*pWeapon->FallbackPaintKit() = 413; // Marble Fade
											}
											else if (Skin == 13)
											{
												*pWeapon->FallbackPaintKit() = 414; // Rust Coat
											}
											else if (Skin == 14)
											{
												*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
											}
											else if (Skin == 15)
											{
												*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
											}
											else if (Skin == 16)
											{
												*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
											}
											else if (Skin == 17)
											{
												*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
											}
											else if (Skin == 18)
											{
												*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
											}
											else if (Skin == 19)
											{
												*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
											}
											else if (Skin == 20)
											{
												*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
											}
											else if (Skin == 21)
											{
												*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
											}
											else if (Skin == 22)
											{
												*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
											}
											else if (Skin == 23)
											{
												*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
											}
											else if (Skin == 24)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
											}
											else if (Skin == 25)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
											}
											else if (Skin == 26)
											{
												*pWeapon->FallbackPaintKit() = 558; // Lore
											}

										}


										else if (Model == 9) // Shadow Daggers
										{
											*pWeapon->ModelIndex() = iDagger; // m_nModelIndex
											*pWeapon->ViewModelIndex() = iDagger;
											*pWeapon->WorldModelIndex() = iDagger + 1;
											*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 516;
											*pWeapon->m_AttributeManager()->m_Item()->EntityQuality() = 3;

											int Skin = Menu::Window.SkinchangerTab.KnifeSkin.GetIndex();

											if (Skin == 0)
											{
												*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
											}
											else if (Skin == 1)
											{
												*pWeapon->FallbackPaintKit() = 12; // Crimson Web
											}

											else if (Skin == 2)
											{
												*pWeapon->FallbackPaintKit() = 38; // Fade
											}
											else if (Skin == 3)
											{
												*pWeapon->FallbackPaintKit() = 40; // Night
											}
											else if (Skin == 4)
											{
												*pWeapon->FallbackPaintKit() = 42; // Blue Steel
											}
											else if (Skin == 5)
											{
												*pWeapon->FallbackPaintKit() = 43; // Stained
											}
											else if (Skin == 6)
											{
												*pWeapon->FallbackPaintKit() = 44; // Case Hardened
											}
											else if (Skin == 8)
											{
												*pWeapon->FallbackPaintKit() = 59; // Slaughter
											}

											else if (Skin == 9)
											{
												*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
											}

											else if (Skin == 10)
											{
												*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
											}
											else if (Skin == 11)
											{
												*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
											}

											else if (Skin == 12)
											{
												*pWeapon->FallbackPaintKit() = 413; // Marble Fade
											}
											else if (Skin == 13)
											{
												*pWeapon->FallbackPaintKit() = 414; // Rust Coat
											}
											else if (Skin == 14)
											{
												*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
											}
											else if (Skin == 15)
											{
												*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
											}
											else if (Skin == 16)
											{
												*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
											}
											else if (Skin == 17)
											{
												*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
											}
											else if (Skin == 18)
											{
												*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
											}
											else if (Skin == 19)
											{
												*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
											}
											else if (Skin == 20)
											{
												*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
											}
											else if (Skin == 21)
											{
												*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
											}
											else if (Skin == 22)
											{
												*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
											}
											else if (Skin == 23)
											{
												*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
											}
											else if (Skin == 24)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
											}
											else if (Skin == 25)
											{
												*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
											}
											else if (Skin == 26)
											{
												*pWeapon->FallbackPaintKit() = 558; // Lore
											}


										}


									}

								}

							*pWeapon->OwnerXuidLow() = 0;
							*pWeapon->OwnerXuidHigh() = 0;
							*pWeapon->FallbackWear() = 0.001f;
							*pWeapon->m_AttributeManager()->m_Item()->ItemIDHigh() = 1;

						
						
						// For Loop weps

							/*ENN*/

						} // if (pOwner == pLocal)

					} // pOwner
				
			} // pEntity

		} // for GetHighestEntityIndex()

	}


	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_NET_UPDATE_END) {
		
		

		for (int i = 1; i < Interfaces::Engine->GetMaxClients(); ++i)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (!pEntity || !pLocal) {
				continue;
			}

			if (pEntity->GetTeamNum() == pLocal->GetTeamNum()) {
				continue;
			}

			if (pEntity->IsDormant() || !pLocal->IsAlive() || !pEntity->IsAlive()) {
				continue;
			}
			//lagComp->logEntity(pEntity);
			g_Resolver->StoreVars(pEntity);
			

		}

	}

	oFrameStageNotify(curStage);
}

void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup)
{
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		if (Menu::Window.VisualsTab.Active.GetState() && pLocal->IsAlive() && !pLocal->IsScoped())
		{
			if (pSetup->fov = 90)
				pSetup->fov = Menu::Window.VisualsTab.OtherFOV.GetValue();
		}

		oOverrideView(ecx, edx, pSetup);
	}

}

void GetViewModelFOV(float& fov)
{
	IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		if (!localplayer)
			return;


		if (Menu::Window.VisualsTab.Active.GetState())
			fov += Menu::Window.VisualsTab.OtherViewmodelFOV.GetValue();
	}
}

float __stdcall GGetViewModelFOV()
{
	float fov = Hooks::VMTClientMode.GetMethod<oGetViewModelFOV>(35)();

	GetViewModelFOV(fov);

	return fov;
}

void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
{
	static DWORD oRenderView = Hooks::VMTRenderView.GetOriginalFunction(6);

	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	__asm
	{
		PUSH whatToDraw
		PUSH nClearFlags
		PUSH hudViewSetup
		PUSH setup
		MOV ECX, ecx
		CALL oRenderView
	}
} 

























































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class hysbkcb {
public:
	double rrufp;
	int nqdyxnw;
	string nchuuykkxb;
	hysbkcb();
	void xbsxhfldjfdu(double jcjklozyxnvyjjf, double aaiscdgmujxrp, string spcftxentloub, string krfoxemalhei, double bfndvoz);
	int zpvauxcypgntk(double ganqgoi, bool ismmbj, int vzyzxbvmgabh, string qeefticgyej);
	int fgghncuqgngtkukvhsta(string jqmqkpme, double jkznkbzhg, string aaiovi, double ypbyacqx, double ooaeaidz, string zyhkqolthbocya, bool tlurcarb);
	int dmjoauudrtfxopexvxgzz(bool azsslmbfcbjfk);
	string ocydjwieybbrqvbpqtg(double swyuxb);

protected:
	bool weoysutrz;
	bool bajgolskuuknxy;
	int reixuadmncpow;

	void nptodcafmnrbs(double jukuc, int zgtlikylsv, string ofsaqpdwihalj, bool mfdligk, bool bdecqbmwmetsp, double rrubolq, double pyromevyovan, double zpqmr);
	string qngxlmkmbnhtd(string fgqpzbpkkbjpl, bool adyqofycdjbv, int xmvcyfgbzweg, double zoqtyzkepapsoyh, double ylrqc, bool iwvlxh, string gxsnlissbj);
	void xyxlhnhphzusggykqktxfo(double tedcpzdpnnmzcq);
	int csebygabbpk(bool uzsslbwja, bool nnfvyfs, double szpvbai, int hoyzavifr, bool xuvlruyzcvdp);
	void diqwxoeonuokdqy(double solelffkt, bool ovubmmepjozbfwl, int ucutjullrzyd, int rqfqgdwwirwiu, string okairoypuegpsc, bool uuzvltr, int onsgukrobd, double pezanbojzfafhh, bool wslkbg, int hmozlhc);
	bool ddrtxaoehfufuzvmhsvh(string lfljuu, double xoxtorhrzvi, double txyiirnw, double hgzlbbbrgfyqpd);
	string pnxsmzdbgpd(double qquhir, int nflmrjiwgxmjo, string sjedrcooggl, string akqhpgmo, double xwfhevdpgvlwq, string thhvussxow, string sndoqzrq, bool tgmrhk, double rhywozg, string qiahfizlh);
	string xtlhpolaxxud(double sqfnydypjypxhyr, string fnzjjqe);
	int fmkuqciygtjcrckixxqvvd(bool vvqrdhvlbdmot, bool reitxgjqy, bool brkcryhffqk, int vdkcthp, string enlxka);
	void xmnmypcmbetkonit(bool hqxlbbiye, double tmglryxwek, string bqrdyxtnomqkw, bool uwubjgbrliehmz, bool sjzpdogcxq, string gqnefm, bool yytez);

private:
	string cgmnlynticg;
	string qukxjxetbm;
	double sivbvashkquzmz;
	string grpkorn;

	string ovsweeqdjlkfbesjwpth(bool yukavvipra);
	double cobvkwcvcblav(double jpecwy);
	string tuamquzljeksx(bool aspcpmnoil, bool zcxtqqiezxwkfk, double qygydcb);

};


string hysbkcb::ovsweeqdjlkfbesjwpth(bool yukavvipra) {
	string alwpqpiefuk = "cvabqcorbgqveqstimobrceuoaqd";
	double ckyeocnyjekeuw = 11738;
	bool nplgmfebjmtibm = false;
	double behakbol = 25621;
	int jofjsppt = 5968;
	bool eorgjovraivnfa = false;
	int bcmjqgwjmefgsrg = 2109;
	string bgstkgokfmwzop = "gdcwgzweegjvzgcalgwrmjhqgulujyuzfcnfykunjqopxnnawiflhbq";
	int xckizvtisr = 270;
	if (string("cvabqcorbgqveqstimobrceuoaqd") == string("cvabqcorbgqveqstimobrceuoaqd")) {
		int ovj;
		for (ovj = 15; ovj > 0; ovj--) {
			continue;
		}
	}
	if (string("gdcwgzweegjvzgcalgwrmjhqgulujyuzfcnfykunjqopxnnawiflhbq") != string("gdcwgzweegjvzgcalgwrmjhqgulujyuzfcnfykunjqopxnnawiflhbq")) {
		int ltwnhabod;
		for (ltwnhabod = 39; ltwnhabod > 0; ltwnhabod--) {
			continue;
		}
	}
	if (false == false) {
		int vawh;
		for (vawh = 75; vawh > 0; vawh--) {
			continue;
		}
	}
	if (false == false) {
		int ytwa;
		for (ytwa = 69; ytwa > 0; ytwa--) {
			continue;
		}
	}
	if (5968 != 5968) {
		int awcgv;
		for (awcgv = 45; awcgv > 0; awcgv--) {
			continue;
		}
	}
	return string("gigeqpllbgusqlsodjs");
}

double hysbkcb::cobvkwcvcblav(double jpecwy) {
	double xwbapv = 83796;
	string azaumnkywytizmc = "fqqjwjmjbby";
	if (83796 != 83796) {
		int xfgwwgs;
		for (xfgwwgs = 3; xfgwwgs > 0; xfgwwgs--) {
			continue;
		}
	}
	if (string("fqqjwjmjbby") != string("fqqjwjmjbby")) {
		int bwbnsc;
		for (bwbnsc = 10; bwbnsc > 0; bwbnsc--) {
			continue;
		}
	}
	if (string("fqqjwjmjbby") == string("fqqjwjmjbby")) {
		int asc;
		for (asc = 55; asc > 0; asc--) {
			continue;
		}
	}
	if (string("fqqjwjmjbby") != string("fqqjwjmjbby")) {
		int stgekmwcsh;
		for (stgekmwcsh = 11; stgekmwcsh > 0; stgekmwcsh--) {
			continue;
		}
	}
	if (string("fqqjwjmjbby") != string("fqqjwjmjbby")) {
		int hqngdb;
		for (hqngdb = 21; hqngdb > 0; hqngdb--) {
			continue;
		}
	}
	return 68929;
}

string hysbkcb::tuamquzljeksx(bool aspcpmnoil, bool zcxtqqiezxwkfk, double qygydcb) {
	bool fslufgewvte = true;
	double rvrneckxkvzej = 17642;
	double xksamya = 7091;
	return string("xnmeuhoa");
}

void hysbkcb::nptodcafmnrbs(double jukuc, int zgtlikylsv, string ofsaqpdwihalj, bool mfdligk, bool bdecqbmwmetsp, double rrubolq, double pyromevyovan, double zpqmr) {
	bool hwdviubqjezbo = true;
	bool omszskgsqnnhop = false;
	string sjidlq = "nggvsfjxyzymoevksfvbktmpqmpomkbuovlhmrviftplpyzqcqabnthabpftkmmheupeqrkawjnlgbseldwksjkqrez";
	double cxzbufmwxra = 133;
	string vndqwsykkrgup = "aqig";
	int vhxzbyf = 2235;
	double olsgryirqkwndyv = 7902;
	if (133 != 133) {
		int ghpx;
		for (ghpx = 42; ghpx > 0; ghpx--) {
			continue;
		}
	}
	if (7902 != 7902) {
		int heejueund;
		for (heejueund = 40; heejueund > 0; heejueund--) {
			continue;
		}
	}
	if (2235 == 2235) {
		int xtxfl;
		for (xtxfl = 1; xtxfl > 0; xtxfl--) {
			continue;
		}
	}

}

string hysbkcb::qngxlmkmbnhtd(string fgqpzbpkkbjpl, bool adyqofycdjbv, int xmvcyfgbzweg, double zoqtyzkepapsoyh, double ylrqc, bool iwvlxh, string gxsnlissbj) {
	string urozjeuwrubtpvb = "ibinrvbbeswhhqvboqqczsbtj";
	if (string("ibinrvbbeswhhqvboqqczsbtj") == string("ibinrvbbeswhhqvboqqczsbtj")) {
		int evmbkh;
		for (evmbkh = 7; evmbkh > 0; evmbkh--) {
			continue;
		}
	}
	if (string("ibinrvbbeswhhqvboqqczsbtj") != string("ibinrvbbeswhhqvboqqczsbtj")) {
		int ei;
		for (ei = 78; ei > 0; ei--) {
			continue;
		}
	}
	return string("bygjkaxzqrtdfsn");
}

void hysbkcb::xyxlhnhphzusggykqktxfo(double tedcpzdpnnmzcq) {
	int avpmtqyhonvrrx = 1066;
	bool lwvruxjxkbmrol = false;
	bool tqkihfbtkac = false;
	double evggyyigbefvy = 23080;
	double vcqwgy = 38614;
	string yypzvxqxj = "uj";
	int wwlpnl = 2260;
	if (false != false) {
		int kzfyjsssq;
		for (kzfyjsssq = 16; kzfyjsssq > 0; kzfyjsssq--) {
			continue;
		}
	}
	if (2260 == 2260) {
		int egkslfidx;
		for (egkslfidx = 93; egkslfidx > 0; egkslfidx--) {
			continue;
		}
	}

}

int hysbkcb::csebygabbpk(bool uzsslbwja, bool nnfvyfs, double szpvbai, int hoyzavifr, bool xuvlruyzcvdp) {
	return 49435;
}

void hysbkcb::diqwxoeonuokdqy(double solelffkt, bool ovubmmepjozbfwl, int ucutjullrzyd, int rqfqgdwwirwiu, string okairoypuegpsc, bool uuzvltr, int onsgukrobd, double pezanbojzfafhh, bool wslkbg, int hmozlhc) {
	int mpqvufiprsvco = 1796;
	int mqqazi = 3219;
	string dpkltvnwejb = "eocjoxu";
	int wexyekfjibsoxre = 8290;
	bool nqognner = false;
	string lodoei = "hxmndfjwwdzxjvgqcvucgmqgzulrbysniaxtcamogutibraufzh";
	string gkvmxdjyvbo = "cefxdkwamwnkhtaygtqtummgbwiqua";
	double lvyeaal = 28464;
	int dpuvkcycgalhot = 3434;

}

bool hysbkcb::ddrtxaoehfufuzvmhsvh(string lfljuu, double xoxtorhrzvi, double txyiirnw, double hgzlbbbrgfyqpd) {
	int jmfokozm = 604;
	double dprpniukxqldidp = 18026;
	int bsjklzoiorkp = 1322;
	string apuuueijsyb = "bwfsgymxcswbkylconjmwfeyrzsijzvibbueypilqeiiueirowcaqytgpxgscxkdesngbltyoadobbkto";
	double scfrtpo = 7318;
	double dcebax = 7812;
	int bicnavphupqj = 3329;
	bool rargnvbyzzpwut = false;
	int lhqwwiijx = 620;
	if (604 == 604) {
		int fcje;
		for (fcje = 83; fcje > 0; fcje--) {
			continue;
		}
	}
	if (string("bwfsgymxcswbkylconjmwfeyrzsijzvibbueypilqeiiueirowcaqytgpxgscxkdesngbltyoadobbkto") == string("bwfsgymxcswbkylconjmwfeyrzsijzvibbueypilqeiiueirowcaqytgpxgscxkdesngbltyoadobbkto")) {
		int jn;
		for (jn = 59; jn > 0; jn--) {
			continue;
		}
	}
	if (18026 != 18026) {
		int icemnms;
		for (icemnms = 86; icemnms > 0; icemnms--) {
			continue;
		}
	}
	if (1322 != 1322) {
		int bghp;
		for (bghp = 45; bghp > 0; bghp--) {
			continue;
		}
	}
	if (7318 == 7318) {
		int uroj;
		for (uroj = 88; uroj > 0; uroj--) {
			continue;
		}
	}
	return false;
}

string hysbkcb::pnxsmzdbgpd(double qquhir, int nflmrjiwgxmjo, string sjedrcooggl, string akqhpgmo, double xwfhevdpgvlwq, string thhvussxow, string sndoqzrq, bool tgmrhk, double rhywozg, string qiahfizlh) {
	int oqyhxubatoklk = 2825;
	if (2825 != 2825) {
		int ctqhjss;
		for (ctqhjss = 26; ctqhjss > 0; ctqhjss--) {
			continue;
		}
	}
	if (2825 == 2825) {
		int gmwif;
		for (gmwif = 49; gmwif > 0; gmwif--) {
			continue;
		}
	}
	if (2825 == 2825) {
		int tm;
		for (tm = 84; tm > 0; tm--) {
			continue;
		}
	}
	if (2825 != 2825) {
		int lfombcx;
		for (lfombcx = 21; lfombcx > 0; lfombcx--) {
			continue;
		}
	}
	return string("cqvuneumqmbeviycpt");
}

string hysbkcb::xtlhpolaxxud(double sqfnydypjypxhyr, string fnzjjqe) {
	bool bvpylplzfjbuli = true;
	bool kestaclejuify = true;
	string omcrkwrnyla = "koqavpvxmaxdyznytrxkpaly";
	int jfeyneedl = 4213;
	double kbmiacfrxgoreo = 11959;
	double wxqxrkoscxi = 4758;
	bool vtjzfmq = false;
	if (string("koqavpvxmaxdyznytrxkpaly") != string("koqavpvxmaxdyznytrxkpaly")) {
		int gbd;
		for (gbd = 0; gbd > 0; gbd--) {
			continue;
		}
	}
	return string("cqidhpfggrcrkinlznpy");
}

int hysbkcb::fmkuqciygtjcrckixxqvvd(bool vvqrdhvlbdmot, bool reitxgjqy, bool brkcryhffqk, int vdkcthp, string enlxka) {
	string aumsgbily = "nyesgaidulryfnrnymnxjeadttseclvjfivvzmtpyuhewmrjtzcheesgjimrauhosckbfzez";
	string ngszdijmdmywa = "wsiqvtdapzwhjhrkbkmmqdamjlksrhyqukmxfvahkjzgswvttpdgvinnfrcdimbnvsgecgshgnjozrlezggyxvkxatmgnkhllhki";
	double tbyemzgcsypcnt = 13750;
	string kcfdxetnxrg = "rukgznclixdxcdwrzfhppwzcqkuwanrliyilbrnzoodhjkzajhdowf";
	double nrvgkpdemzsyt = 15846;
	if (string("rukgznclixdxcdwrzfhppwzcqkuwanrliyilbrnzoodhjkzajhdowf") != string("rukgznclixdxcdwrzfhppwzcqkuwanrliyilbrnzoodhjkzajhdowf")) {
		int evqvcra;
		for (evqvcra = 45; evqvcra > 0; evqvcra--) {
			continue;
		}
	}
	if (13750 == 13750) {
		int nm;
		for (nm = 78; nm > 0; nm--) {
			continue;
		}
	}
	if (15846 == 15846) {
		int mmxypyxme;
		for (mmxypyxme = 58; mmxypyxme > 0; mmxypyxme--) {
			continue;
		}
	}
	if (15846 != 15846) {
		int uwy;
		for (uwy = 50; uwy > 0; uwy--) {
			continue;
		}
	}
	if (string("nyesgaidulryfnrnymnxjeadttseclvjfivvzmtpyuhewmrjtzcheesgjimrauhosckbfzez") == string("nyesgaidulryfnrnymnxjeadttseclvjfivvzmtpyuhewmrjtzcheesgjimrauhosckbfzez")) {
		int lzgbhgj;
		for (lzgbhgj = 2; lzgbhgj > 0; lzgbhgj--) {
			continue;
		}
	}
	return 24729;
}

void hysbkcb::xmnmypcmbetkonit(bool hqxlbbiye, double tmglryxwek, string bqrdyxtnomqkw, bool uwubjgbrliehmz, bool sjzpdogcxq, string gqnefm, bool yytez) {
	bool bqtflrrcrk = false;
	double xwnrh = 10346;
	double axlvhyshvvtqdb = 33603;
	bool mpzub = false;
	double jlkutphulexcif = 48170;
	double srmbfudumlzcjr = 2237;
	if (false == false) {
		int iieljyye;
		for (iieljyye = 75; iieljyye > 0; iieljyye--) {
			continue;
		}
	}
	if (false == false) {
		int yimzpizws;
		for (yimzpizws = 29; yimzpizws > 0; yimzpizws--) {
			continue;
		}
	}
	if (48170 != 48170) {
		int phszte;
		for (phszte = 35; phszte > 0; phszte--) {
			continue;
		}
	}
	if (48170 == 48170) {
		int gbnugmtnsi;
		for (gbnugmtnsi = 63; gbnugmtnsi > 0; gbnugmtnsi--) {
			continue;
		}
	}

}

void hysbkcb::xbsxhfldjfdu(double jcjklozyxnvyjjf, double aaiscdgmujxrp, string spcftxentloub, string krfoxemalhei, double bfndvoz) {
	string lrusvw = "fskvgzabplmpfpepanwafviqnfjjgncbdtnrplzjavcna";
	double knleeunddji = 21865;
	double mowiqa = 6547;
	bool tjhjhh = false;
	string wuhcz = "rxvurtgbgpxiuaejrjujykcemoyvuvvwldkrvywkhfaeco";
	double woaytdripuw = 8124;

}

int hysbkcb::zpvauxcypgntk(double ganqgoi, bool ismmbj, int vzyzxbvmgabh, string qeefticgyej) {
	string vhatrwfads = "m";
	bool dfjwachabfjr = false;
	int cuiwkgxzp = 1222;
	string iqhqd = "hierqaa";
	double gxllb = 13496;
	string ylxtw = "cvvtdbpneibytoydsvxehwwoxqznpcfraxcsmppntysngemslbqovrwqyhgkdxbrsebtimkbmmbwgzqvrzste";
	int zagwaxqoi = 4449;
	if (false != false) {
		int yzhs;
		for (yzhs = 79; yzhs > 0; yzhs--) {
			continue;
		}
	}
	if (13496 != 13496) {
		int kmsxitrfnx;
		for (kmsxitrfnx = 9; kmsxitrfnx > 0; kmsxitrfnx--) {
			continue;
		}
	}
	if (13496 != 13496) {
		int oeund;
		for (oeund = 63; oeund > 0; oeund--) {
			continue;
		}
	}
	if (13496 != 13496) {
		int ant;
		for (ant = 16; ant > 0; ant--) {
			continue;
		}
	}
	return 97012;
}

int hysbkcb::fgghncuqgngtkukvhsta(string jqmqkpme, double jkznkbzhg, string aaiovi, double ypbyacqx, double ooaeaidz, string zyhkqolthbocya, bool tlurcarb) {
	return 76948;
}

int hysbkcb::dmjoauudrtfxopexvxgzz(bool azsslmbfcbjfk) {
	bool xosuvihyg = true;
	int howapzerhnanzdx = 653;
	if (653 == 653) {
		int him;
		for (him = 14; him > 0; him--) {
			continue;
		}
	}
	if (true == true) {
		int fqk;
		for (fqk = 37; fqk > 0; fqk--) {
			continue;
		}
	}
	if (true == true) {
		int eykdvw;
		for (eykdvw = 64; eykdvw > 0; eykdvw--) {
			continue;
		}
	}
	return 30562;
}

string hysbkcb::ocydjwieybbrqvbpqtg(double swyuxb) {
	string acmhjzecfrc = "ijjymekwtonskgioxcbtrviukylucjvuyujvrzfy";
	double kzmhymbvn = 8944;
	int djwjviiuncpzd = 4515;
	string sfnbajjjj = "ardgwjpzftascegyfecwiggjntzftxhqwbmjpldrikrthefozoszbnqfvhmdiharsalweued";
	int elajobclr = 1690;
	if (string("ardgwjpzftascegyfecwiggjntzftxhqwbmjpldrikrthefozoszbnqfvhmdiharsalweued") == string("ardgwjpzftascegyfecwiggjntzftxhqwbmjpldrikrthefozoszbnqfvhmdiharsalweued")) {
		int wnfydw;
		for (wnfydw = 22; wnfydw > 0; wnfydw--) {
			continue;
		}
	}
	if (8944 == 8944) {
		int mukurc;
		for (mukurc = 41; mukurc > 0; mukurc--) {
			continue;
		}
	}
	if (8944 != 8944) {
		int xyhshada;
		for (xyhshada = 27; xyhshada > 0; xyhshada--) {
			continue;
		}
	}
	if (string("ardgwjpzftascegyfecwiggjntzftxhqwbmjpldrikrthefozoszbnqfvhmdiharsalweued") == string("ardgwjpzftascegyfecwiggjntzftxhqwbmjpldrikrthefozoszbnqfvhmdiharsalweued")) {
		int ihj;
		for (ihj = 67; ihj > 0; ihj--) {
			continue;
		}
	}
	if (4515 == 4515) {
		int ocp;
		for (ocp = 34; ocp > 0; ocp--) {
			continue;
		}
	}
	return string("");
}

hysbkcb::hysbkcb() {
	this->xbsxhfldjfdu(19750, 562, string("walriiychgngryimgyzocfgm"), string("ysgebuzppfwkjf"), 53391);
	this->zpvauxcypgntk(33752, false, 3749, string("mivdqecedouwewymexxgerajxqvowrkexgxbvofwfni"));
	this->fgghncuqgngtkukvhsta(string("yndizctmecjqkqbgprzgwlorexhxhwmqdtpevbluntnfqqpmnmwkwfbepbbnvtdahfdnivpwzrlnpgntddxvxiqukhodc"), 54361, string("xuaphxbwqtpfxjjxwsztxcpsidyswienclvudecivguevbdyridhxogzdtlticysd"), 84473, 39998, string("bxuxssaj"), false);
	this->dmjoauudrtfxopexvxgzz(true);
	this->ocydjwieybbrqvbpqtg(8669);
	this->nptodcafmnrbs(14956, 3738, string("pvkfaktijdqmbquedsqomhocnkhjgnhxzonqnshdwxbjfyolxdytlmzfpjhzwvfraeahstzoteyvuj"), false, true, 36265, 13313, 36801);
	this->qngxlmkmbnhtd(string("rvwjpjvofrgqvextqbaohgszpzhkobhvjwfmmvvhrdxfxexzyudckyydbyladewzaevyi"), false, 1817, 35134, 54216, false, string("mfanfgjligmaibten"));
	this->xyxlhnhphzusggykqktxfo(21545);
	this->csebygabbpk(true, true, 68519, 2242, false);
	this->diqwxoeonuokdqy(39789, true, 1829, 2897, string("gwfwxtqvzglzejmthpwzkvofoutpwokddtggwmezbhdsuhqxt"), true, 7307, 76271, false, 3871);
	this->ddrtxaoehfufuzvmhsvh(string("nzruympbpewuavfgdgqffmgbeaslujmhlorvzlojtgbhctkvmqehndxyiczfyjsrjgxisrvicnvujxoaszcvpxjwmvoxamp"), 7775, 13656, 37701);
	this->pnxsmzdbgpd(2637, 485, string("ikhlkmtwprldpvebjzdpiwkcbqcouscfbmyszahsurzmurbfgamslnxhtzsa"), string("zxophkdublamvragwolvcmjzrrdoutdrlwjjeukkxkzvjrvqmpylrqkymjysusmarklnjblcbrcoryqrfzwbizier"), 61869, string(""), string(""), true, 45946, string("ufnxnetrxtnhyypszggxdkvodyyjxzxnafzmauwupwhqtgnhudmadcmcxeqeytigauinwroyeppwzknzpclufmpbbektnrsrote"));
	this->xtlhpolaxxud(12858, string("yryiyhzbknncfrhvvuyzjhfeygwauqe"));
	this->fmkuqciygtjcrckixxqvvd(false, false, true, 1566, string("sgmewszaubdaywizdvdbkklhxbcymgljpmhapcawljpjgnktsxqyakwnphcxuchqaooayspgwjtdcair"));
	this->xmnmypcmbetkonit(true, 9821, string("g"), true, false, string("rcfkhvbukwbtumfd"), true);
	this->ovsweeqdjlkfbesjwpth(false);
	this->cobvkwcvcblav(45894);
	this->tuamquzljeksx(true, true, 20795);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class rqrdgcj {
public:
	double kuyehpvdijijg;
	bool ajnmv;
	int khgligzdkfl;
	rqrdgcj();
	bool bgtqtdfrnhudzakwtchxhi(string deoyac, double lpblixxoxkt, string zdgusqjay, bool gwhoogjbdd);
	string ocqauwmkxgw(int bkelayxpsc, bool bcahvlyysni, string vhwhbei);
	void qhnxjvdazarjh(double cgxuelz, int liabwo, string ywijpqgnjh);
	string vjwmasgkaqnvtlxtvzkyozyu(string auzolnyds);
	bool yltdkbxzykfusc(bool pxnlbg, double nwozzurg, string wgtzpryetkyucz, string kihgdsiekh, bool ujnxtlha, string dbvldxsfsw, double ircwlgsivlgv, int gokomwnvya, double hpmczyxuxrgkvz);
	int sjiphcgqlpzxxrowrf(int pitorbqm, double bskpmcns, int tdlwidumge, int yvbsw);
	double fdxmyzpaiveiujwr(string cokolgtpumo, string eguacbimtt, double wmlgowzfzrtgh, int sbhfiepdit, double okzqggdrpcud, string hcwlqfhjoeuc);
	int mutllnddqdxg(double zpxvtahpgawwl, string hauebthkbzlq, double barirpq, string rebminhpzfvgo);

protected:
	string jsptviayu;
	bool kjxzkggotelgovj;

	double ycrzsmiglbinwdsqv(double yovgcqlpk, int uynivmjxue, int ihhxdbphtwrbr, double bdulitsuokufgqs, int fgjcg, double ajqvmogexaw, int qkthf, bool tfvlquosmlxg, bool ekfgbqx, int qkhocrkkix);
	string lifhcfqkibrbdepxwrqjech(string tqqoo, bool aoqvpydgepd, bool tsabvnuljfgcbck);
	double mlwbkitwtviezutdkpgbmf(double lrdrxbocetd, string ouvaawvfk, double njhttjlshj, string dmplqvkczyudkcw);
	int vxxuqqjlshprjicv(int rzzqtipascllpgd, double nicigfa, bool losru, string sjcbhwjys);
	string aafvhnacxqrjhh(int murpppqzffpeaqy, bool iavck, string rzdgjjittnwr, bool aoavgrxszc, string htrmzgeu, string nbcnvmkcvwutl);
	double yjeiqrsklkvzmyizyzgvqwgst(bool scesrauxflvld, string eyipxssdvvhlyu, bool zuvkbnpufr, int mmuzhodhrlcviuk, bool tydegdvdzmcyhkr);

private:
	double nlofbjrbxvculm;

	double eplgqjneyqiuhocn(bool yuvxhxmvrnuzesh, int tyhaxputbzsxzi, int nqaeqnzty, double wyltnk, string rsjetowqewngye, string ojdhxjng, int xkkhfumiqtj, string tatoj);
	string hsdynpzcrgtrserovszdl(string tuwdtq, string xcksndkcjwmypd, string ctguyn, string ioordottb, string gqaiqgm, double ufycjptrwxgmj, int jhnkxwvaiqvahts, bool vfcnnf);

};


double rqrdgcj::eplgqjneyqiuhocn(bool yuvxhxmvrnuzesh, int tyhaxputbzsxzi, int nqaeqnzty, double wyltnk, string rsjetowqewngye, string ojdhxjng, int xkkhfumiqtj, string tatoj) {
	double mrzcdzjte = 38566;
	double rvupvtcq = 27946;
	if (27946 == 27946) {
		int gycie;
		for (gycie = 68; gycie > 0; gycie--) {
			continue;
		}
	}
	return 95226;
}

string rqrdgcj::hsdynpzcrgtrserovszdl(string tuwdtq, string xcksndkcjwmypd, string ctguyn, string ioordottb, string gqaiqgm, double ufycjptrwxgmj, int jhnkxwvaiqvahts, bool vfcnnf) {
	int wxmchtyllcdhar = 56;
	bool svyjuvqs = false;
	string rooaborcq = "npsuhysfhkggbzfsigfnbkfrvslwnbywpltcmyqqpfttmfczoejrhrevczmowmxdatdwkdbmybjzyxyqintqhnmwbql";
	int cqvqvdan = 2447;
	string ivvqiecagvzdrn = "fomgdcwxxkvpfal";
	double ouepmejfe = 2304;
	double ehqtaiwn = 10565;
	double femqxrexgmjmty = 41347;
	double jndoxs = 35999;
	if (string("fomgdcwxxkvpfal") == string("fomgdcwxxkvpfal")) {
		int nvexpckm;
		for (nvexpckm = 40; nvexpckm > 0; nvexpckm--) {
			continue;
		}
	}
	if (2304 != 2304) {
		int moizpbfnn;
		for (moizpbfnn = 63; moizpbfnn > 0; moizpbfnn--) {
			continue;
		}
	}
	if (false == false) {
		int gniqjn;
		for (gniqjn = 18; gniqjn > 0; gniqjn--) {
			continue;
		}
	}
	if (35999 == 35999) {
		int qjzscfca;
		for (qjzscfca = 97; qjzscfca > 0; qjzscfca--) {
			continue;
		}
	}
	return string("rek");
}

double rqrdgcj::ycrzsmiglbinwdsqv(double yovgcqlpk, int uynivmjxue, int ihhxdbphtwrbr, double bdulitsuokufgqs, int fgjcg, double ajqvmogexaw, int qkthf, bool tfvlquosmlxg, bool ekfgbqx, int qkhocrkkix) {
	string qzhmhijoq = "yhjoclfrfdjnbbzsmpzzodvfmcnokvlkjkqdyzyjflabpbbfaczzelkvhqeqhpcniapxabxnpnbtcgjdov";
	int iwqqtqeypyowfh = 3984;
	if (string("yhjoclfrfdjnbbzsmpzzodvfmcnokvlkjkqdyzyjflabpbbfaczzelkvhqeqhpcniapxabxnpnbtcgjdov") == string("yhjoclfrfdjnbbzsmpzzodvfmcnokvlkjkqdyzyjflabpbbfaczzelkvhqeqhpcniapxabxnpnbtcgjdov")) {
		int fwsloxl;
		for (fwsloxl = 8; fwsloxl > 0; fwsloxl--) {
			continue;
		}
	}
	if (string("yhjoclfrfdjnbbzsmpzzodvfmcnokvlkjkqdyzyjflabpbbfaczzelkvhqeqhpcniapxabxnpnbtcgjdov") != string("yhjoclfrfdjnbbzsmpzzodvfmcnokvlkjkqdyzyjflabpbbfaczzelkvhqeqhpcniapxabxnpnbtcgjdov")) {
		int mifgwvm;
		for (mifgwvm = 58; mifgwvm > 0; mifgwvm--) {
			continue;
		}
	}
	return 15809;
}

string rqrdgcj::lifhcfqkibrbdepxwrqjech(string tqqoo, bool aoqvpydgepd, bool tsabvnuljfgcbck) {
	int ijpovh = 3478;
	double afuzlrrfqtekf = 30060;
	string jusiefps = "xqbsgmwflonpitalkwxqgsqrmaxgxsddnykzju";
	int vgeylfeuafr = 4191;
	bool cdscrmgmmu = false;
	double zwytnmg = 4358;
	int pnhmopqfzglf = 2704;
	double oxkiptuukjia = 4838;
	string vxgogngyyxvcho = "egvwnldkyrmuqocppbzoo";
	int vwnvmdv = 12;
	if (string("egvwnldkyrmuqocppbzoo") == string("egvwnldkyrmuqocppbzoo")) {
		int sfh;
		for (sfh = 15; sfh > 0; sfh--) {
			continue;
		}
	}
	if (4358 != 4358) {
		int fjcaew;
		for (fjcaew = 0; fjcaew > 0; fjcaew--) {
			continue;
		}
	}
	if (3478 != 3478) {
		int ot;
		for (ot = 88; ot > 0; ot--) {
			continue;
		}
	}
	return string("xqcwotg");
}

double rqrdgcj::mlwbkitwtviezutdkpgbmf(double lrdrxbocetd, string ouvaawvfk, double njhttjlshj, string dmplqvkczyudkcw) {
	string pjzjobmdgombot = "ufefikaohkpbgwygbxjurdetenlgkripwicgcdgehgkekxlvjzogolxmqoqjhplrsrbtppnsuzcmitybxqyawbzambxpit";
	double kgmxh = 19456;
	int mssmthdkgaaukx = 2480;
	bool mscnegk = true;
	bool xzegrybj = true;
	return 13240;
}

int rqrdgcj::vxxuqqjlshprjicv(int rzzqtipascllpgd, double nicigfa, bool losru, string sjcbhwjys) {
	bool zivpbrzanoy = false;
	bool dybslyt = false;
	double ymbajspodltgj = 1396;
	int qnfkghsso = 4175;
	string bqrrlr = "iwzzjclojfkqmqlskbfzubbyhohyrdcmimvzgtwlmkhxdevvlvkyehdogrbdysdoqscbrnfijpneofrtfwheldcnpi";
	bool lwckyw = true;
	bool kulsxuohxz = true;
	if (true != true) {
		int iolz;
		for (iolz = 37; iolz > 0; iolz--) {
			continue;
		}
	}
	if (1396 == 1396) {
		int osx;
		for (osx = 38; osx > 0; osx--) {
			continue;
		}
	}
	return 68900;
}

string rqrdgcj::aafvhnacxqrjhh(int murpppqzffpeaqy, bool iavck, string rzdgjjittnwr, bool aoavgrxszc, string htrmzgeu, string nbcnvmkcvwutl) {
	int umrdztvaj = 7947;
	bool rjagov = true;
	bool kmrickvuaoduui = false;
	string tfhgnbbgdi = "bwfqxuvdmxqbctoyzyhzhqphjordihkuiluwbjkjcfcwptfjtyydexziulxjiwkawsvlhgjktoabaixaxpepawpczd";
	int aqgtjblmwpy = 2377;
	double ogfjo = 23607;
	int ylmrisekyeblkrv = 2321;
	int ioaubwjj = 3461;
	return string("xrbrprzqhkh");
}

double rqrdgcj::yjeiqrsklkvzmyizyzgvqwgst(bool scesrauxflvld, string eyipxssdvvhlyu, bool zuvkbnpufr, int mmuzhodhrlcviuk, bool tydegdvdzmcyhkr) {
	return 59231;
}

bool rqrdgcj::bgtqtdfrnhudzakwtchxhi(string deoyac, double lpblixxoxkt, string zdgusqjay, bool gwhoogjbdd) {
	double ayhbqnnm = 2683;
	double jhdohpvthpouug = 53879;
	bool pzmsoqimntmifhh = true;
	string opqtlgzqdyq = "tygzjqjiyqtxmvktpihqxctdspykwhpdwdvmgffdjcpxlzmbwvrsvdynvgqejeo";
	bool tqcpjki = false;
	int lnqwovikfp = 2220;
	bool nuljmvjuaa = true;
	int ygzizgqbrrxpuse = 1081;
	if (1081 != 1081) {
		int taq;
		for (taq = 15; taq > 0; taq--) {
			continue;
		}
	}
	if (2683 == 2683) {
		int leyriti;
		for (leyriti = 4; leyriti > 0; leyriti--) {
			continue;
		}
	}
	return false;
}

string rqrdgcj::ocqauwmkxgw(int bkelayxpsc, bool bcahvlyysni, string vhwhbei) {
	bool vbsoxbq = false;
	bool nmtsiiprt = true;
	int wnoefalwcr = 3164;
	double gqhmlhmrridrk = 8213;
	int fcdcngbbwlt = 4814;
	if (true != true) {
		int orqulj;
		for (orqulj = 78; orqulj > 0; orqulj--) {
			continue;
		}
	}
	if (true != true) {
		int apuzfam;
		for (apuzfam = 96; apuzfam > 0; apuzfam--) {
			continue;
		}
	}
	if (4814 != 4814) {
		int peizigqp;
		for (peizigqp = 84; peizigqp > 0; peizigqp--) {
			continue;
		}
	}
	if (4814 != 4814) {
		int qrxhaum;
		for (qrxhaum = 93; qrxhaum > 0; qrxhaum--) {
			continue;
		}
	}
	if (3164 != 3164) {
		int aptkyzt;
		for (aptkyzt = 34; aptkyzt > 0; aptkyzt--) {
			continue;
		}
	}
	return string("zeybpgyhsxijsjwqm");
}

void rqrdgcj::qhnxjvdazarjh(double cgxuelz, int liabwo, string ywijpqgnjh) {
	string jgcjq = "ll";
	int zgaaujnryuynih = 6199;
	bool xaiuyfj = true;
	int lxnkj = 812;
	double zteaykb = 76881;
	double wkspx = 381;
	bool xlnrrqhtypxx = false;
	string chbvzaevfuv = "ebhpeqjibghcnqhxotqnynheqbikhqclvnxvopfjlyrhlfj";
	string pubkvvvwpagox = "jdczu";
	if (812 != 812) {
		int qedr;
		for (qedr = 56; qedr > 0; qedr--) {
			continue;
		}
	}
	if (true != true) {
		int gv;
		for (gv = 52; gv > 0; gv--) {
			continue;
		}
	}
	if (string("ebhpeqjibghcnqhxotqnynheqbikhqclvnxvopfjlyrhlfj") != string("ebhpeqjibghcnqhxotqnynheqbikhqclvnxvopfjlyrhlfj")) {
		int ugjsntznvm;
		for (ugjsntznvm = 10; ugjsntznvm > 0; ugjsntznvm--) {
			continue;
		}
	}
	if (6199 != 6199) {
		int fjasiwdl;
		for (fjasiwdl = 66; fjasiwdl > 0; fjasiwdl--) {
			continue;
		}
	}
	if (string("jdczu") == string("jdczu")) {
		int sc;
		for (sc = 28; sc > 0; sc--) {
			continue;
		}
	}

}

string rqrdgcj::vjwmasgkaqnvtlxtvzkyozyu(string auzolnyds) {
	double mwvtpprjvttv = 9804;
	bool vzlixovwysazrt = false;
	double tsrlw = 35435;
	string udccecrwiwypu = "juxalufsanisqxifdifbkfwqpoefwtqvxsfettaolrocoudffoaduuvbehxhieulddopnkkvifcplcntus";
	return string("fkaqllofrt");
}

bool rqrdgcj::yltdkbxzykfusc(bool pxnlbg, double nwozzurg, string wgtzpryetkyucz, string kihgdsiekh, bool ujnxtlha, string dbvldxsfsw, double ircwlgsivlgv, int gokomwnvya, double hpmczyxuxrgkvz) {
	bool tdvpbhhtniovfqb = false;
	double tdknqlbuyaqpq = 18927;
	string sgppaymraiuradu = "fiukqssjipjvtbuhlkxcbuybqorkqdwtmqagprzdustsldygppx";
	if (string("fiukqssjipjvtbuhlkxcbuybqorkqdwtmqagprzdustsldygppx") != string("fiukqssjipjvtbuhlkxcbuybqorkqdwtmqagprzdustsldygppx")) {
		int xrsm;
		for (xrsm = 79; xrsm > 0; xrsm--) {
			continue;
		}
	}
	if (18927 == 18927) {
		int noymtgjr;
		for (noymtgjr = 51; noymtgjr > 0; noymtgjr--) {
			continue;
		}
	}
	if (string("fiukqssjipjvtbuhlkxcbuybqorkqdwtmqagprzdustsldygppx") == string("fiukqssjipjvtbuhlkxcbuybqorkqdwtmqagprzdustsldygppx")) {
		int hpbywfhk;
		for (hpbywfhk = 0; hpbywfhk > 0; hpbywfhk--) {
			continue;
		}
	}
	if (18927 == 18927) {
		int ruadabpz;
		for (ruadabpz = 8; ruadabpz > 0; ruadabpz--) {
			continue;
		}
	}
	return false;
}

int rqrdgcj::sjiphcgqlpzxxrowrf(int pitorbqm, double bskpmcns, int tdlwidumge, int yvbsw) {
	string vwgpykdot = "sfskjalwcivcjk";
	double snxywwk = 2561;
	bool wmnpfqbnv = true;
	string qubfaelqr = "pemnxhlwmxserhqpvrkejrctwtehvazhkobmiredqsgdkjhpuwlwmbggphhalwagtwgtltwsuxtdzitsmyhxbufwrifazyk";
	string sphdfrzhcli = "beuunlxbjrfchkmxfwiamdbaqfycammscw";
	double zzhtvsychfqm = 12315;
	int eowkegrx = 623;
	int izpqje = 1186;
	int gulobcskjmmbhz = 2074;
	double ptkntjhrypelhd = 30809;
	if (30809 == 30809) {
		int auvxwjpg;
		for (auvxwjpg = 49; auvxwjpg > 0; auvxwjpg--) {
			continue;
		}
	}
	if (623 != 623) {
		int surgvchykl;
		for (surgvchykl = 72; surgvchykl > 0; surgvchykl--) {
			continue;
		}
	}
	if (623 != 623) {
		int kavajslwf;
		for (kavajslwf = 21; kavajslwf > 0; kavajslwf--) {
			continue;
		}
	}
	return 89859;
}

double rqrdgcj::fdxmyzpaiveiujwr(string cokolgtpumo, string eguacbimtt, double wmlgowzfzrtgh, int sbhfiepdit, double okzqggdrpcud, string hcwlqfhjoeuc) {
	return 65755;
}

int rqrdgcj::mutllnddqdxg(double zpxvtahpgawwl, string hauebthkbzlq, double barirpq, string rebminhpzfvgo) {
	return 4419;
}

rqrdgcj::rqrdgcj() {
	this->bgtqtdfrnhudzakwtchxhi(string("bhiahn"), 17659, string("bx"), true);
	this->ocqauwmkxgw(2732, false, string("uwsmnkpvkrfsherwkyzpabaozxgreudoehqflaemgjdgibyylvacqttukglqxhpnlnvcgpdgfcx"));
	this->qhnxjvdazarjh(7927, 5248, string("ipjoupnxlmmvcizcdgubpdjmcgyotahjfesvvaxmvexlxrlr"));
	this->vjwmasgkaqnvtlxtvzkyozyu(string("bhlwsshezzleozzlarwyjfokecwbmzxulpwcnulzrepbxwezywpuqijoikaspocipbjpwdzknetixbbqozbdcos"));
	this->yltdkbxzykfusc(false, 6809, string("lhignugswspbtcguqlihyvqgdhrjngjonazeql"), string("jpytnqfibjnwayebehjfadediasywtbhlmclqnticaedyopxrnxhltdvcddbbjzjwnhnyobhhgszcthxdllkdonqbmk"), false, string("srhbvxtznaktlkgfqnqamfciytnboptszkqgjuwacqmlizplyyfniytgzrfbllbhtomcssenfsiriykiwnkkssvtfgrywyoulzf"), 19826, 3473, 83949);
	this->sjiphcgqlpzxxrowrf(404, 83945, 1155, 6015);
	this->fdxmyzpaiveiujwr(string("xxpqttfpuopjntjzzwjwzblwomwjhbmiixjhouutkymibzvlvfsnbyovgmldffwkbrtcwiucyjrypksvtzkwwpqdkytmq"), string("acogtgsgnhnkzirullrwauutvcvyixaticfycud"), 49065, 6361, 20775, string("awdvdkmmphibdctlhyjbecohjfyqccsewkcqxstgjkoxkebgfgilstxcvq"));
	this->mutllnddqdxg(19187, string("tbngpxn"), 2155, string("bwcfehmsnvliivvkokjlyxwpdfalwhquyzooymrcrxkecvqngywiyxkfdrkayintnkqfplgcozugduaobvjkfmpjnpeujdtr"));
	this->ycrzsmiglbinwdsqv(45103, 354, 2761, 8964, 2087, 53678, 6147, false, true, 3449);
	this->lifhcfqkibrbdepxwrqjech(string("fxjguhcwcktlvj"), false, true);
	this->mlwbkitwtviezutdkpgbmf(60021, string("sr"), 6372, string("qckctgwyizgabsnxtvbljedxshcbiqyncotuxwkqbvnsofcdwozirwqwdyixsssetlybdbokxdaz"));
	this->vxxuqqjlshprjicv(6327, 146, false, string("xggms"));
	this->aafvhnacxqrjhh(2971, true, string("lowoqvzasimiu"), false, string("lsinkuccoxypncse"), string("fkikporzsnskcxuqeaxuqqskoiocjwlfgpynxotkfwlilsvdpevpywxulessxilhkbxopwcmlffytnlkzplaqqjqomjomfrsxvp"));
	this->yjeiqrsklkvzmyizyzgvqwgst(true, string("wbxpqmoepzfhviumekaqgntcqtdoimvfzffbudhphqsibo"), true, 370, false);
	this->eplgqjneyqiuhocn(false, 8500, 8202, 48, string("tgxqnhjjtweuoaaysclccsbupiiv"), string("lyxjvkrswzzwnbobdhlikemixjogxquatmdrwxhlvcdonyiaezmtfxsaqkhpuoafsnfmzmsoegthnzwrogjapstfeifv"), 304, string("pcupaejrikkrhwmlxqftuvbwdxcelpka"));
	this->hsdynpzcrgtrserovszdl(string("ghgghnmmbhoecbfvytbzvoesdttfirnfnb"), string("hebtdeywataguypocuauxhmcjopwiovqgaefhbcfghxshjtyeuxciqi"), string("ytltoinlsdjebjlffurkgjeiiiesguirsjjugatv"), string("crhfftdwcegwktubkdaccqqrnjxgmmncienlih"), string("lzxjdfmwukyhumsnvohcxvizdsbmmckyjnzkvwllfinklrhcagekrnjxpabvnbzmuhauanduwdysr"), 15554, 7216, false);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class xxwhfnn {
public:
	bool jhlsoahbj;
	string hkyzwihjrcirxy;
	xxwhfnn();
	int fjukhpzsbrfhta(bool zwfdctnhwrx, bool ufyboyqd, double fvvkazvnibzsta);
	double lbzzuvjctpbupxuajls(int uckfpvidbbkafkv, string prngsizxydz);
	double upukrzkczoiwdbvx(string uosogt, int vrdsw, int gwicamtf, int vauqss, int oaxlqomfeyk, double bcxrcccgjfrxvh, double zsksp, string kahupyxrebijhhx);

protected:
	bool urddmhlixgjh;
	bool utrawr;
	int cdrluedd;

	bool vkqeulbszjbxjufweprn(bool bttmhai);
	double samponcgpipuuyrzfo(int htdhmcqksek, string mojzdullkgljdpb, bool qsrpn, bool cxronsf);

private:
	int gusljd;
	int fmjomskhpo;
	string pgkzzhsmglqezxx;
	string sovxgwv;
	string kichcaqkqab;

	bool iknnhhebebrbroqkldz();

};


bool xxwhfnn::iknnhhebebrbroqkldz() {
	string gqrksag = "eszhknouujwwjcrxtzeehqtupmkrik";
	int dcuqnjsi = 1077;
	if (string("eszhknouujwwjcrxtzeehqtupmkrik") != string("eszhknouujwwjcrxtzeehqtupmkrik")) {
		int xfdgsti;
		for (xfdgsti = 11; xfdgsti > 0; xfdgsti--) {
			continue;
		}
	}
	return true;
}

bool xxwhfnn::vkqeulbszjbxjufweprn(bool bttmhai) {
	string kmavrmzvavnfadj = "kaplpqyllctlqkchgkpghioeuqhcapnpjirfsidgxmzgnqtvejwkyvhtwuedlexfivtsn";
	int trfpk = 925;
	bool qkqbdbmxr = false;
	string toyxzjrksbbsd = "jpgbapjuprzcaxqxlivdzgdcyavoiseswyzccgkbsovzofadrbuqdtowjmawjkxdasxzypegfiugxsxzzrmxptvhqmhhazmgt";
	string flbqpvneimdkhql = "n";
	double copqi = 15381;
	string wtpyaeowbeyzlf = "pksonnqrlwqapwymaywrrafbhbwpanwieayiwezmncyuhehabbtwckbgrttsvtdhmogiutrqemeiwpdggiryjthxkireezygxh";
	if (925 == 925) {
		int baqrncjeqi;
		for (baqrncjeqi = 66; baqrncjeqi > 0; baqrncjeqi--) {
			continue;
		}
	}
	if (15381 != 15381) {
		int xgy;
		for (xgy = 50; xgy > 0; xgy--) {
			continue;
		}
	}
	if (false == false) {
		int unwcvuep;
		for (unwcvuep = 37; unwcvuep > 0; unwcvuep--) {
			continue;
		}
	}
	if (string("pksonnqrlwqapwymaywrrafbhbwpanwieayiwezmncyuhehabbtwckbgrttsvtdhmogiutrqemeiwpdggiryjthxkireezygxh") != string("pksonnqrlwqapwymaywrrafbhbwpanwieayiwezmncyuhehabbtwckbgrttsvtdhmogiutrqemeiwpdggiryjthxkireezygxh")) {
		int zm;
		for (zm = 73; zm > 0; zm--) {
			continue;
		}
	}
	if (string("jpgbapjuprzcaxqxlivdzgdcyavoiseswyzccgkbsovzofadrbuqdtowjmawjkxdasxzypegfiugxsxzzrmxptvhqmhhazmgt") == string("jpgbapjuprzcaxqxlivdzgdcyavoiseswyzccgkbsovzofadrbuqdtowjmawjkxdasxzypegfiugxsxzzrmxptvhqmhhazmgt")) {
		int sxywd;
		for (sxywd = 76; sxywd > 0; sxywd--) {
			continue;
		}
	}
	return true;
}

double xxwhfnn::samponcgpipuuyrzfo(int htdhmcqksek, string mojzdullkgljdpb, bool qsrpn, bool cxronsf) {
	int rfjytrtsrlqcjo = 5686;
	double putgeesurxhkpfq = 11178;
	int uhuthzlirr = 463;
	int tsvtvaihxlwabcq = 3690;
	int exghxgnow = 1224;
	double hzvhdpr = 29122;
	int kbspgeo = 1380;
	return 43805;
}

int xxwhfnn::fjukhpzsbrfhta(bool zwfdctnhwrx, bool ufyboyqd, double fvvkazvnibzsta) {
	string rpezjmnbvyre = "mvprhbwqxysarjbfugjtpfnghmeqmzbhfrwrmdzayktezxvqbfchyiyqefff";
	bool whkpzffczsfzpdp = true;
	bool ipqbtbplej = true;
	double ltvlmapjjjrgifc = 5603;
	bool zsnnhqwqrwvb = false;
	int asxzbrarlpwspr = 1027;
	double fvukqdnzs = 3391;
	int zjzwjbngo = 1318;
	int xbdpmtwrljcpth = 1167;
	if (3391 != 3391) {
		int uursakj;
		for (uursakj = 41; uursakj > 0; uursakj--) {
			continue;
		}
	}
	if (3391 == 3391) {
		int geeqpfkmea;
		for (geeqpfkmea = 4; geeqpfkmea > 0; geeqpfkmea--) {
			continue;
		}
	}
	if (5603 == 5603) {
		int ham;
		for (ham = 31; ham > 0; ham--) {
			continue;
		}
	}
	if (false != false) {
		int srflkqz;
		for (srflkqz = 22; srflkqz > 0; srflkqz--) {
			continue;
		}
	}
	return 91990;
}

double xxwhfnn::lbzzuvjctpbupxuajls(int uckfpvidbbkafkv, string prngsizxydz) {
	int hnbcyqm = 786;
	string drkiktd = "phyyioiszkwvaxxakpbwrhhxiauhxhuylylgadtevigmsytcwtxxpsshwhvjpeaiithjgpcdletqstnmuzlfzuvuorfq";
	bool pmnxyvpdap = false;
	int oubegoydpwjsbdk = 1287;
	double tqneadyfm = 8102;
	double gzzxhoaudt = 4249;
	int nxyxoluyw = 1394;
	int rwvwxdgafspo = 3475;
	double pbosdzhsyyqhkl = 28650;
	if (4249 == 4249) {
		int tdgl;
		for (tdgl = 84; tdgl > 0; tdgl--) {
			continue;
		}
	}
	if (1394 == 1394) {
		int umglzkogk;
		for (umglzkogk = 1; umglzkogk > 0; umglzkogk--) {
			continue;
		}
	}
	return 15422;
}

double xxwhfnn::upukrzkczoiwdbvx(string uosogt, int vrdsw, int gwicamtf, int vauqss, int oaxlqomfeyk, double bcxrcccgjfrxvh, double zsksp, string kahupyxrebijhhx) {
	string prvpbrxbmrkhuf = "egkivtmywwbsuucskilxcuzpgvbejszyhvcxpbissznxjzgckkifihh";
	double jpxjzwvvdhgsrk = 57784;
	double ntbbdzyxxcsgh = 13336;
	bool kibkxpkplyfdbfu = false;
	int ifnsxqdm = 1338;
	string jeqoaabtara = "dacldbfstticnnazllxzsldswesrcdxzofghorugbanhdqpgmyshzhelzqfrdaexbcagvzrrewcxhbmykqbyscmrbnxilvacq";
	int xhydshuiaypldbn = 810;
	bool kdgkbsrweejz = true;
	bool nkxref = false;
	if (13336 != 13336) {
		int cxhumsk;
		for (cxhumsk = 8; cxhumsk > 0; cxhumsk--) {
			continue;
		}
	}
	return 85817;
}

xxwhfnn::xxwhfnn() {
	this->fjukhpzsbrfhta(true, false, 65161);
	this->lbzzuvjctpbupxuajls(4459, string("dexznqvksyksjoaiymvkbvkpvnpblokmbytmfsqdsoujbdrmeweglhdgnbldskndapchppsyfwkrafzxdprrbvyyr"));
	this->upukrzkczoiwdbvx(string("aueouvvqgtqzwheasbxjwkwbmqxypoyqogxouegsaxxnbrlccbftbfufcioiatludkosqjp"), 3518, 4938, 2723, 1845, 22875, 33795, string("husybqagifmnzli"));
	this->vkqeulbszjbxjufweprn(false);
	this->samponcgpipuuyrzfo(426, string("dhtayowkhnfbheujbpcbxiqzdlkmjmytijjuetifebliifdlagmdkhqgxpf"), false, true);
	this->iknnhhebebrbroqkldz();
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class tmaekbv {
public:
	double snytoytrqus;
	tmaekbv();
	bool hxtbvblvzothgm(int bfbpdyah, bool zjcckeiyl, double juinqgnpi, double frzxi, bool izwfl, int eoevwasx, int yktgxsngyodnrw, int jlnzmvez, int mdrwunjbrlwelwn);
	void eatjqxhxwjdvrtvhbh();
	bool jvnovbreyc(string lbmyuok, double wetdxoverijdc, bool ayocxkztmfdnuzf);
	bool ieyyniwajtvmtt(double wnzhue, string gdpvzj, int mcajrnoses, string lcqthifkdiojkm, double qsgwgxc, int zsqdui, double mdskoaawwfvdq, string dvgnyznfm, int gkphpypluivlorn);
	int thmpmgksjtvkbpdc(double mgjlmb, string xyehzfvqtlrj);
	void hbixoyxctfdzkrktiydcl(bool hpklgbwbmlytfto, string cbkjojxmdkjf, int krkzkgcacurcm, string uwhuxabcpfyj, bool rmixppv, bool njukgzclmavy, double iajmlnuhpdnscp, bool bmtledbdxejme);
	double cjodgbwkgyqqpomqngpqmkihw();

protected:
	int chxfuarprmapg;
	string iyokeugcqemmpk;
	double aqbrynkmqae;
	double wycujgbkqvwjji;

	void xryquwzowyf(double gddfrlnxwvgxzs, bool jyvcyzjccjap, bool biiqnlgph, string uffhaccdn, bool dvjstoykb, int kpxevi, bool ountgcmz, double iwhkksrg);
	double puqoebozlhal();
	void fckkywicbumeukbfulxhtnbqz(bool cxupvw, int cphvqtqh, int vjcqctcsvrwj, bool ezmqytiqhawr, double zbmroptmgqlcu, int xspaxhwc, bool oeuee, int inykdh);
	string prqtybrire(bool wngkmhxbj, bool qbtswpcak);

private:
	string bsgej;
	bool nirzetdnk;
	string peduimxly;
	double wsderzy;

	double fhflsqjlsijhwprcvqpm(double qztvvd, double atqmtvhmghkuxv);
	bool qjcnrnbissk(string suqqmqtu, bool arizofkzgzqkwd, double ljtau);
	string mziamohpealppxu();
	void fbwbnaajgcwahfd(int dtwwzpzdsj, int ifjksjuedhzyfa, bool iobaj, int wfjxglmtqlmmnvo, bool vvklytixjv, int rpoeguah, string wrmydvttnng, string fjkeyrs, string mdpbquhqlp);

};


double tmaekbv::fhflsqjlsijhwprcvqpm(double qztvvd, double atqmtvhmghkuxv) {
	return 54074;
}

bool tmaekbv::qjcnrnbissk(string suqqmqtu, bool arizofkzgzqkwd, double ljtau) {
	return true;
}

string tmaekbv::mziamohpealppxu() {
	string qrdmekgrfmydki = "qocxeqffnjvktbblhhkdzhzsylzhibhxeroot";
	double jmmimpaapa = 9768;
	string mpkhnnxngct = "vdnpyyeashmw";
	bool qedlyomegprzhm = true;
	double nqfensnhu = 39788;
	string ljsfn = "bqwvmkrzr";
	double irvmlfx = 4259;
	double oahcwfwqeehy = 16120;
	string uvqzfysqugmdmvd = "pqlqmitwmveqjenrvsmxvpiemnjqrdwfyrxiln";
	if (16120 != 16120) {
		int iexdi;
		for (iexdi = 55; iexdi > 0; iexdi--) {
			continue;
		}
	}
	return string("vqphiqtfcqi");
}

void tmaekbv::fbwbnaajgcwahfd(int dtwwzpzdsj, int ifjksjuedhzyfa, bool iobaj, int wfjxglmtqlmmnvo, bool vvklytixjv, int rpoeguah, string wrmydvttnng, string fjkeyrs, string mdpbquhqlp) {
	int aazcojcepxta = 3525;
	int rxbberrwyqz = 5731;
	if (5731 != 5731) {
		int ajsovvpgde;
		for (ajsovvpgde = 77; ajsovvpgde > 0; ajsovvpgde--) {
			continue;
		}
	}
	if (5731 != 5731) {
		int oyue;
		for (oyue = 97; oyue > 0; oyue--) {
			continue;
		}
	}

}

void tmaekbv::xryquwzowyf(double gddfrlnxwvgxzs, bool jyvcyzjccjap, bool biiqnlgph, string uffhaccdn, bool dvjstoykb, int kpxevi, bool ountgcmz, double iwhkksrg) {
	double adhsreoofw = 18630;
	bool tzmbfiuqyezyjkf = false;
	string ukwttiheeg = "ghjbguwvszlinzvpdvawbobikkykcmtmrtvoecraotxdclbdeqnucmdnwrfkekhrcacthkj";
	bool bbodnbmeh = false;
	string auozlvz = "xnzfudfkvhzensfaxsdbfwvgvrncibvdyztrhwfcyrgtcczhwjxcqgekllkqelqy";
	string irdpdxpycm = "lwtvgqwuwkrmsdljuidszfuefpzwozxrnfricgpxjodwxnybbppakkormfsqpszg";
	if (false != false) {
		int jjlou;
		for (jjlou = 16; jjlou > 0; jjlou--) {
			continue;
		}
	}
	if (string("ghjbguwvszlinzvpdvawbobikkykcmtmrtvoecraotxdclbdeqnucmdnwrfkekhrcacthkj") != string("ghjbguwvszlinzvpdvawbobikkykcmtmrtvoecraotxdclbdeqnucmdnwrfkekhrcacthkj")) {
		int hrapfa;
		for (hrapfa = 77; hrapfa > 0; hrapfa--) {
			continue;
		}
	}

}

double tmaekbv::puqoebozlhal() {
	bool unchywmz = true;
	int zjvfvqzoutht = 1582;
	int rqprxfpljgspoxf = 633;
	int ngemousew = 769;
	if (true == true) {
		int wzm;
		for (wzm = 26; wzm > 0; wzm--) {
			continue;
		}
	}
	if (633 != 633) {
		int qupdznhw;
		for (qupdznhw = 95; qupdznhw > 0; qupdznhw--) {
			continue;
		}
	}
	if (1582 != 1582) {
		int nbn;
		for (nbn = 99; nbn > 0; nbn--) {
			continue;
		}
	}
	return 96266;
}

void tmaekbv::fckkywicbumeukbfulxhtnbqz(bool cxupvw, int cphvqtqh, int vjcqctcsvrwj, bool ezmqytiqhawr, double zbmroptmgqlcu, int xspaxhwc, bool oeuee, int inykdh) {

}

string tmaekbv::prqtybrire(bool wngkmhxbj, bool qbtswpcak) {
	string tjrbpbvaf = "efqrruqioontendathmtmzlkorsraxcghebsqlfzooiffbzafaorqvsuswcrpwopgdlrifrbyvqnfwreofphdruh";
	bool psdbdj = true;
	string loyiggjdszrny = "ytyddigswuqwwdjxmyow";
	bool xepvrnxvp = false;
	double vbgvwlyi = 53278;
	if (string("ytyddigswuqwwdjxmyow") == string("ytyddigswuqwwdjxmyow")) {
		int luh;
		for (luh = 25; luh > 0; luh--) {
			continue;
		}
	}
	if (string("efqrruqioontendathmtmzlkorsraxcghebsqlfzooiffbzafaorqvsuswcrpwopgdlrifrbyvqnfwreofphdruh") == string("efqrruqioontendathmtmzlkorsraxcghebsqlfzooiffbzafaorqvsuswcrpwopgdlrifrbyvqnfwreofphdruh")) {
		int rjhzhg;
		for (rjhzhg = 87; rjhzhg > 0; rjhzhg--) {
			continue;
		}
	}
	if (string("efqrruqioontendathmtmzlkorsraxcghebsqlfzooiffbzafaorqvsuswcrpwopgdlrifrbyvqnfwreofphdruh") == string("efqrruqioontendathmtmzlkorsraxcghebsqlfzooiffbzafaorqvsuswcrpwopgdlrifrbyvqnfwreofphdruh")) {
		int fjbgcaxpib;
		for (fjbgcaxpib = 100; fjbgcaxpib > 0; fjbgcaxpib--) {
			continue;
		}
	}
	return string("termnizxsa");
}

bool tmaekbv::hxtbvblvzothgm(int bfbpdyah, bool zjcckeiyl, double juinqgnpi, double frzxi, bool izwfl, int eoevwasx, int yktgxsngyodnrw, int jlnzmvez, int mdrwunjbrlwelwn) {
	string xvxggyrlfpcgwxw = "ndfszpamnnpgtueyhxbzysrljfnmqgpphbbzbjkoxysgpgpuconvtwtuxeoakmesttznwaickpfqippzkwcnjs";
	bool itgtnqfquiaqbbb = true;
	double lgori = 61657;
	int ipooayyw = 2165;
	if (61657 != 61657) {
		int axwqbd;
		for (axwqbd = 12; axwqbd > 0; axwqbd--) {
			continue;
		}
	}
	if (2165 != 2165) {
		int qyc;
		for (qyc = 81; qyc > 0; qyc--) {
			continue;
		}
	}
	if (61657 != 61657) {
		int xs;
		for (xs = 25; xs > 0; xs--) {
			continue;
		}
	}
	return false;
}

void tmaekbv::eatjqxhxwjdvrtvhbh() {
	int brhuvzhjg = 725;
	bool mwlicdeg = true;
	string xnipokwzbenqu = "egpmkvwodmueliratmnanekwzuihlzggubsctb";
	int kxnexxyxgb = 1974;

}

bool tmaekbv::jvnovbreyc(string lbmyuok, double wetdxoverijdc, bool ayocxkztmfdnuzf) {
	bool iqphschmygxhrmr = true;
	bool kservm = true;
	double hoyddxoe = 30007;
	int zgjctbfmgxmqnw = 1486;
	double uuukpjeww = 32800;
	string hmhiuceciatxuv = "tjqgipgzakjkhoogbvuioojfyxwxyiysdpejnssksjvvsukfmalqczlhqmmrifkfmj";
	string iraxoz = "irzdpqhnlzmqyvzbfzhpjulihshxkqiaegzwghnyypultgtuzfhtxdslxkogbjxfaaacarrkd";
	double wriflgirysvhd = 11821;
	return true;
}

bool tmaekbv::ieyyniwajtvmtt(double wnzhue, string gdpvzj, int mcajrnoses, string lcqthifkdiojkm, double qsgwgxc, int zsqdui, double mdskoaawwfvdq, string dvgnyznfm, int gkphpypluivlorn) {
	double cyefupbi = 48099;
	string gxqhelsfhrtlis = "yhjkgnlkrgujnaqghfvobwrnmjorbabqeuzchsskqhgviwnoumylsfjoylraefjtliexruygznrqwbkx";
	bool zbmlgygqvkjg = true;
	double fxgjnsjhcgnt = 16393;
	int rqvbomdso = 1058;
	string tukjx = "gpcblcvorsuglnohefwxtmohmdpufrqbkjrhrxapjvyhqnjahmizdokxtsooqxdvuflzaxunyvs";
	bool kwocfhbvhs = true;
	bool lnorkim = true;
	if (48099 == 48099) {
		int on;
		for (on = 42; on > 0; on--) {
			continue;
		}
	}
	if (48099 == 48099) {
		int pbdtgjh;
		for (pbdtgjh = 38; pbdtgjh > 0; pbdtgjh--) {
			continue;
		}
	}
	if (16393 == 16393) {
		int rxf;
		for (rxf = 80; rxf > 0; rxf--) {
			continue;
		}
	}
	if (16393 == 16393) {
		int taxjc;
		for (taxjc = 51; taxjc > 0; taxjc--) {
			continue;
		}
	}
	if (true != true) {
		int onupvvga;
		for (onupvvga = 42; onupvvga > 0; onupvvga--) {
			continue;
		}
	}
	return false;
}

int tmaekbv::thmpmgksjtvkbpdc(double mgjlmb, string xyehzfvqtlrj) {
	return 81316;
}

void tmaekbv::hbixoyxctfdzkrktiydcl(bool hpklgbwbmlytfto, string cbkjojxmdkjf, int krkzkgcacurcm, string uwhuxabcpfyj, bool rmixppv, bool njukgzclmavy, double iajmlnuhpdnscp, bool bmtledbdxejme) {
	double cmscojofhaj = 52787;
	bool azwnkzwcchjcr = false;
	string ynzhro = "ymh";
	int jyvkyqkwaydi = 6972;
	double wgflybxfuxpzplu = 54457;

}

double tmaekbv::cjodgbwkgyqqpomqngpqmkihw() {
	int vkcuhwua = 2929;
	double behgsnqfm = 84312;
	string hbhwofep = "yuacerzpybwgrpqywkgawuswuqjkauwdxqokawipdhccopvvgdz";
	int uefzimjd = 390;
	int pajjwo = 2339;
	string oeknhsm = "nmtjwnmdmzutfbramlxggqsolckzdxrfnkcqrjfwdtwxvzojswkqehweo";
	string khmyaurfzf = "unzdqqvgiosbzulmnsaheinoimmscblplkvxpkuahrnoayafmmimfkvkko";
	bool nnuwlmudeqo = false;
	bool ifdajymfhwlr = true;
	double onytcvbbrllivh = 17588;
	if (string("unzdqqvgiosbzulmnsaheinoimmscblplkvxpkuahrnoayafmmimfkvkko") == string("unzdqqvgiosbzulmnsaheinoimmscblplkvxpkuahrnoayafmmimfkvkko")) {
		int cqnprzftn;
		for (cqnprzftn = 94; cqnprzftn > 0; cqnprzftn--) {
			continue;
		}
	}
	if (17588 != 17588) {
		int oqn;
		for (oqn = 57; oqn > 0; oqn--) {
			continue;
		}
	}
	return 21784;
}

tmaekbv::tmaekbv() {
	this->hxtbvblvzothgm(1620, true, 15308, 80094, true, 9351, 3723, 1463, 4040);
	this->eatjqxhxwjdvrtvhbh();
	this->jvnovbreyc(string("bavhypnmunqijejrpdhbbsk"), 62609, true);
	this->ieyyniwajtvmtt(83373, string("drdhyawwtyvlyvlnqxxpbogulbnvmoquklbpzi"), 252, string("oowyunfksrkjmkvbatqvbrvzhdtoauevayjkwocowwveesauqeguylvvqufhcclvejafmfukgikwcuesfuudvnlipwfpoalaffd"), 7175, 2280, 13070, string("hxyhmketanawgglvxuwsfixsofdrmtpdcurcozwdzajbjuzxulayfhndalfcu"), 1406);
	this->thmpmgksjtvkbpdc(22889, string("ztfcmfpcibiemaxqiadntpukpcokwcgliggzlvnqttylnxkrlxuiwnmykkcrtudzplmggewxnvjplxchwieejbuarffhox"));
	this->hbixoyxctfdzkrktiydcl(false, string("albsndjtxskiroqszbyaggadomoalbatkuftwmkljuaggfgvhquwqpaqyqomesxgmqqptkrcuvexfcjxqchjhtzeusrtfyqxbc"), 1276, string("kvnclgjhqnthjlbbpahckkmdcwfbofhuw"), false, true, 9827, true);
	this->cjodgbwkgyqqpomqngpqmkihw();
	this->xryquwzowyf(7107, true, true, string("cykjbfgzcnmf"), false, 2801, false, 24244);
	this->puqoebozlhal();
	this->fckkywicbumeukbfulxhtnbqz(true, 1899, 236, false, 86450, 1874, true, 1262);
	this->prqtybrire(true, true);
	this->fhflsqjlsijhwprcvqpm(42214, 13352);
	this->qjcnrnbissk(string("haadqnppgbedqovdlsqtql"), true, 13854);
	this->mziamohpealppxu();
	this->fbwbnaajgcwahfd(1360, 3023, true, 4770, true, 1981, string("otvsdklrmzgbaynzjwpfkaqgembefrnhxpitede"), string("oxhkrdzodshaexnxfndeukuspbxqjwwjxnhlmhpvxksaxgxxgenbonjoyxovbsazcrbkybhxkflysixhzy"), string("hrcpyguelvcdocdjjdigyajcwf"));
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class agcuurt {
public:
	int nryuz;
	agcuurt();
	bool nbxhvvnldljkcpyiirxvfo(string hzopv, bool hrpervfhxserc, string zureh);
	string wpwowrofdcavnvoacy(string hmjfdu, double imgtkcyavl, bool fcxibxqmvxgcon, double wzrpuv, string khrvjsb);
	bool gauxtixmlfxjzduuzducqgp(double fqeuspubfsi);
	string cqgbbrpjmbxwsinfaxjao();
	double itrklutmutwhcqcx(double ciueglgqmgtt, double apstdrrnwhx, int cvaghn, bool yrgalwk, bool nbmoshmyvfdnc, string ccxugcdvzwmikpi, double gdqskocas, double lqpnmxj, int oqfuyfanq, string khavybu);
	double pijdxcgvuyq(bool svwrgohrc, string nhiwxvlknlml);
	string hdvbhbouxamcbuahcnxwvn(string lonmrilizyldw, int uvxvqhwezr, string iiooaxpfg, int nuucbembdicmt, double ysbqia);
	void xwlgkqjysh(int njiqhhx, string cxxbl, int wieuvg, double hugakvrcvpnhyvt, double nioemluwv, double rrgupduucvhyi);
	void kadusaxhdoth(bool hpfewxhq, string wnvix, double bwrlbymesfvpn, bool ajrsytkfmw, int cbyvcqsnoxt, double pulrqgtwqx, int llvnl);

protected:
	double ptkasllm;
	double mycfsntjueri;
	bool pqexne;
	bool usbnuwnwekfbrs;

	int pkgttxcdzknhpbvj(string scsrkwsjpgz, string gwttrcvpei, string igmzpei, double lyyfzwjsacofsks, string bifccoax, string pvcncq, string ckiycgg, bool qvnbbwhdheawum, double wwflzm, string bunpqywymmyycvf);
	void knaxbrgoboulwuryp(double cwqpvpojb, int vfhwddaclmne, bool byskmbk, bool fjmldchc, bool btlfcvjgq, bool lixuyaq);
	int fwdhqgtdmbktsfqtntqmn(bool ztwcqvbiqrlov, string yixigjnoe, double pdmegpcgoozmvxt, string aiquxzxvain, double qipgwjqibwgry, bool upetg, double mhitzi);
	string siejoygezesanuowadnwgtp(int vplym, bool jionvdf, double owlmuttwy);
	void fnwubaizfnopocrvjbfcyzs(bool ohmkqyflfdjud);
	string ocdqxaatpdqthghxbqkrbxfc();
	double isgrubsrtdyzmcmfjzr(string kjwhhdvqfqs, int qaqstdz);
	int aujueatqymhmoolypqwz(string ogwzdzkaydhep, int thyntwvi, bool dnhhbl, int oprrdn, double xhpbvhuiohgmew, bool vfrmbzbxbqsr, bool tpqpmgjecjcmjq, int wcuasovvr, string erifdgbifi);
	double jppwivqvdpxfdidl(bool eqxdcv, double ctfpbmy, bool xxjynhqhnsctx);

private:
	bool brgowxwxhbq;
	int smralchxtqcuily;
	bool yowttgmsk;
	string dvlrl;
	bool xooyfxchaatumzj;

	bool qwfkxdyoezbvukpvjy(int oojmndgeewa, double kyclrxwunpt, bool ohzmn, bool eopoqebeszbowqw, string fcwycaexd, double wqgkorbrdzdruf, double wvlkw, int hnoxxatyyp, double qwhyc);
	double ssseopxsgraxbogqwvh(double xzezlnxyae);
	int ebcqepxjhsqaxsaepdkjetqk(string icfvisyubnz, double mcsagluypcomuot, bool quorryzgifdngbs, bool toajgopatgey, int slsmtytcatwou, double tsenquaku, double majdrctuvqzo, string bxriigqprspcto, string vrddya);
	void hmkaqfwiaeiv(string pkiilgqotal, bool kbolvbslrbpc, int ruqoojdxab, int ytyrcsnhbd, int jkhzrzdzhznwmn, string vktheaupb, int ryqduddnh, string bdrlnefsxrsrbc, double kmhhehb);
	double gyfekcejddocjpnbudheze(double owtvxiozdoqcum, bool rnropssfpsyxy);
	bool unesxvwgrmjx(int zshfixbtunb, string lnbaeu, string mvprvp, double mbcfzo, int dqryehtqng, double yvnwumovinq, bool fgkvm);
	void anucwpntactftzpxrilkaqah(int dewor, bool gjnmxmwndo, int uvgpolndk, double fywltcaat, bool toroqpimalstrl, bool rudrmbqycyou, double kclmqzykrn, bool avgnrre);

};


bool agcuurt::qwfkxdyoezbvukpvjy(int oojmndgeewa, double kyclrxwunpt, bool ohzmn, bool eopoqebeszbowqw, string fcwycaexd, double wqgkorbrdzdruf, double wvlkw, int hnoxxatyyp, double qwhyc) {
	int jhexhow = 8384;
	double anguq = 21341;
	int krvoe = 2426;
	double tgthle = 68806;
	if (68806 == 68806) {
		int jjrjci;
		for (jjrjci = 36; jjrjci > 0; jjrjci--) {
			continue;
		}
	}
	if (2426 == 2426) {
		int rlijg;
		for (rlijg = 36; rlijg > 0; rlijg--) {
			continue;
		}
	}
	if (68806 == 68806) {
		int fdfawkyff;
		for (fdfawkyff = 74; fdfawkyff > 0; fdfawkyff--) {
			continue;
		}
	}
	return false;
}

double agcuurt::ssseopxsgraxbogqwvh(double xzezlnxyae) {
	double ufufecdshqz = 52666;
	string ljjuznq = "puyhwjgmtvjhlrkljyjrsqccbahyzumpjmejlaoyrhwagnaevvejeoqzrbmy";
	string etwtzhpxkgvd = "ghcygqzmrzfxkvanqolwalqypytewehwwfjrwkdcrkqwbfmvtqdwr";
	bool nnhqlreglp = true;
	string gxavhydaivaeowz = "ubklfbxftphofimipuosekfozdocdrxrflsflxdawckn";
	string hpniyk = "gsenyjznnokqevrkaeugnlfwzzaozkmanexqmpepelnkdslbkzmeskqtbzrbawdtyofl";
	bool wokiofvbfm = true;
	string sudgbdfoc = "dhqwnrrfnfgnxoopyefhxjeqqf";
	int cyebuakbo = 1437;
	string vvmvfn = "knqeadgkslyutuqcdhjmeolawclicplwmboyedveb";
	return 67721;
}

int agcuurt::ebcqepxjhsqaxsaepdkjetqk(string icfvisyubnz, double mcsagluypcomuot, bool quorryzgifdngbs, bool toajgopatgey, int slsmtytcatwou, double tsenquaku, double majdrctuvqzo, string bxriigqprspcto, string vrddya) {
	int xnidimp = 5742;
	int rysempkmwcvam = 6378;
	int alihkt = 373;
	int opeosopjfw = 1188;
	double dmmheyff = 23663;
	bool yosxel = true;
	string shhckyqe = "uknlrgnestbzcylxltkpktlstaevbkclarniuvqwfnavnbxkwue";
	if (5742 != 5742) {
		int avhd;
		for (avhd = 81; avhd > 0; avhd--) {
			continue;
		}
	}
	return 54568;
}

void agcuurt::hmkaqfwiaeiv(string pkiilgqotal, bool kbolvbslrbpc, int ruqoojdxab, int ytyrcsnhbd, int jkhzrzdzhznwmn, string vktheaupb, int ryqduddnh, string bdrlnefsxrsrbc, double kmhhehb) {
	bool dakqzj = false;
	bool gfxxbcwvbrf = true;
	double qydrfpktta = 14685;
	int oitpsqsnsr = 1268;
	if (false == false) {
		int tnbn;
		for (tnbn = 31; tnbn > 0; tnbn--) {
			continue;
		}
	}

}

double agcuurt::gyfekcejddocjpnbudheze(double owtvxiozdoqcum, bool rnropssfpsyxy) {
	double vovkuwbbgbkus = 17460;
	string rmrqjnwqxhjtyxj = "xyohvpzwgcjtdnhvvjjxntruobijvfxomgwstxonwkdsgrinrnyhqzschaephvj";
	int ewlqyo = 521;
	bool kpvxfedpihb = true;
	string yiwdeqi = "rxsgirznnzws";
	double ecfuzbmiaafujq = 22457;
	double gtngccmftixmn = 13694;
	bool mpfuscmccyq = false;
	string wuhzggvfindzveb = "ahyxjqlqbujrriaxkicwfrxljfrfujoltwzamvfvqnrkrhcrhssjwpmvvbtcctombexbicdughordwtqwqkuhcmgt";
	int ajgryju = 1092;
	if (13694 == 13694) {
		int ybmxy;
		for (ybmxy = 16; ybmxy > 0; ybmxy--) {
			continue;
		}
	}
	if (true == true) {
		int nuonncgjpm;
		for (nuonncgjpm = 3; nuonncgjpm > 0; nuonncgjpm--) {
			continue;
		}
	}
	if (22457 != 22457) {
		int nmcpkfuazf;
		for (nmcpkfuazf = 29; nmcpkfuazf > 0; nmcpkfuazf--) {
			continue;
		}
	}
	if (13694 != 13694) {
		int tws;
		for (tws = 83; tws > 0; tws--) {
			continue;
		}
	}
	return 6597;
}

bool agcuurt::unesxvwgrmjx(int zshfixbtunb, string lnbaeu, string mvprvp, double mbcfzo, int dqryehtqng, double yvnwumovinq, bool fgkvm) {
	int uhfswmirdcd = 1323;
	int ljkalyu = 6923;
	string qinzxopep = "hbbn";
	double gfnhctxc = 17335;
	bool oandqrhokqblrhq = true;
	bool ewqmxouvkxwulw = true;
	bool lbyilc = false;
	string aysdfbrmxomak = "yptmdqwnvarpylgfocoajszecahjrqybjfvqhesubalraufgynq";
	string ybwwyxlp = "kxqauyglnitfhnhlabgmnxuhicbaqihcsrqqtjbyvwlaldpwibbe";
	return false;
}

void agcuurt::anucwpntactftzpxrilkaqah(int dewor, bool gjnmxmwndo, int uvgpolndk, double fywltcaat, bool toroqpimalstrl, bool rudrmbqycyou, double kclmqzykrn, bool avgnrre) {
	int odqgsuu = 6743;
	string rvwjwtdxiog = "whiupwylfemlsjidyjiudgwcsxdnagturanqpdlirvnzabxxzstbzteqzfhgtzrzyxyvqkivvljmvnyktnaw";
	bool zedlq = true;
	double ustnfaeqbnzxh = 1240;
	double dlujefkazhpvq = 14311;
	string scsemborvsyzma = "gmzasxfspyhvppyauooeefklhkbcjffafv";
	if (string("whiupwylfemlsjidyjiudgwcsxdnagturanqpdlirvnzabxxzstbzteqzfhgtzrzyxyvqkivvljmvnyktnaw") != string("whiupwylfemlsjidyjiudgwcsxdnagturanqpdlirvnzabxxzstbzteqzfhgtzrzyxyvqkivvljmvnyktnaw")) {
		int wkgzjfw;
		for (wkgzjfw = 22; wkgzjfw > 0; wkgzjfw--) {
			continue;
		}
	}
	if (string("whiupwylfemlsjidyjiudgwcsxdnagturanqpdlirvnzabxxzstbzteqzfhgtzrzyxyvqkivvljmvnyktnaw") != string("whiupwylfemlsjidyjiudgwcsxdnagturanqpdlirvnzabxxzstbzteqzfhgtzrzyxyvqkivvljmvnyktnaw")) {
		int dkboynvowg;
		for (dkboynvowg = 46; dkboynvowg > 0; dkboynvowg--) {
			continue;
		}
	}

}

int agcuurt::pkgttxcdzknhpbvj(string scsrkwsjpgz, string gwttrcvpei, string igmzpei, double lyyfzwjsacofsks, string bifccoax, string pvcncq, string ckiycgg, bool qvnbbwhdheawum, double wwflzm, string bunpqywymmyycvf) {
	bool glrxnhbcamql = true;
	int yduhdiuthdlfo = 1616;
	int jbbdgqsblvsvhn = 1120;
	if (1616 == 1616) {
		int fzcjk;
		for (fzcjk = 32; fzcjk > 0; fzcjk--) {
			continue;
		}
	}
	if (1120 != 1120) {
		int ravkxrakr;
		for (ravkxrakr = 43; ravkxrakr > 0; ravkxrakr--) {
			continue;
		}
	}
	if (true == true) {
		int fng;
		for (fng = 20; fng > 0; fng--) {
			continue;
		}
	}
	if (1120 != 1120) {
		int tv;
		for (tv = 69; tv > 0; tv--) {
			continue;
		}
	}
	return 58041;
}

void agcuurt::knaxbrgoboulwuryp(double cwqpvpojb, int vfhwddaclmne, bool byskmbk, bool fjmldchc, bool btlfcvjgq, bool lixuyaq) {
	double joetvgre = 17637;
	bool ixoemtpjbp = false;
	string gbqilm = "pzfvffenjewnvfjkwcxmvyknkfhbpqdrjunxifdngwqhnka";
	int plmvfbhbbxoqdom = 1110;
	if (string("pzfvffenjewnvfjkwcxmvyknkfhbpqdrjunxifdngwqhnka") != string("pzfvffenjewnvfjkwcxmvyknkfhbpqdrjunxifdngwqhnka")) {
		int plmf;
		for (plmf = 78; plmf > 0; plmf--) {
			continue;
		}
	}
	if (false == false) {
		int pilzb;
		for (pilzb = 39; pilzb > 0; pilzb--) {
			continue;
		}
	}
	if (string("pzfvffenjewnvfjkwcxmvyknkfhbpqdrjunxifdngwqhnka") != string("pzfvffenjewnvfjkwcxmvyknkfhbpqdrjunxifdngwqhnka")) {
		int qayr;
		for (qayr = 25; qayr > 0; qayr--) {
			continue;
		}
	}
	if (string("pzfvffenjewnvfjkwcxmvyknkfhbpqdrjunxifdngwqhnka") != string("pzfvffenjewnvfjkwcxmvyknkfhbpqdrjunxifdngwqhnka")) {
		int wyhbttqm;
		for (wyhbttqm = 96; wyhbttqm > 0; wyhbttqm--) {
			continue;
		}
	}
	if (false == false) {
		int gpamigdmp;
		for (gpamigdmp = 24; gpamigdmp > 0; gpamigdmp--) {
			continue;
		}
	}

}

int agcuurt::fwdhqgtdmbktsfqtntqmn(bool ztwcqvbiqrlov, string yixigjnoe, double pdmegpcgoozmvxt, string aiquxzxvain, double qipgwjqibwgry, bool upetg, double mhitzi) {
	bool gxzdbmbbb = false;
	int zapspyzxn = 1108;
	int rzdovjbtmqccrtq = 3853;
	string dkobimbmpz = "ttaogwuenadgxioflphauzckpofqrvwqcxmmyyrwvxndqyoietntrlttsdxfijhzbmbyhuudkzizogippyq";
	if (string("ttaogwuenadgxioflphauzckpofqrvwqcxmmyyrwvxndqyoietntrlttsdxfijhzbmbyhuudkzizogippyq") != string("ttaogwuenadgxioflphauzckpofqrvwqcxmmyyrwvxndqyoietntrlttsdxfijhzbmbyhuudkzizogippyq")) {
		int sfapew;
		for (sfapew = 75; sfapew > 0; sfapew--) {
			continue;
		}
	}
	if (false != false) {
		int hkzxlca;
		for (hkzxlca = 31; hkzxlca > 0; hkzxlca--) {
			continue;
		}
	}
	return 42515;
}

string agcuurt::siejoygezesanuowadnwgtp(int vplym, bool jionvdf, double owlmuttwy) {
	double vbgvzgfi = 17699;
	int pcmcwajoxtpkgwv = 1451;
	if (1451 == 1451) {
		int whojzlhy;
		for (whojzlhy = 37; whojzlhy > 0; whojzlhy--) {
			continue;
		}
	}
	return string("kcaovhmychmadokwtd");
}

void agcuurt::fnwubaizfnopocrvjbfcyzs(bool ohmkqyflfdjud) {
	bool ngnykykmvimaa = true;
	int hlbikhuguouc = 666;
	double igdcclvteru = 8556;
	double jnnosvzaodfvzk = 9028;
	string hyutjlpmrx = "pypnstguvtvpywpjuvunbuedtljpaknvazjncoghdbxtngvcfvtfyuafmnmamrtqufutb";
	int lnlxnwq = 717;
	if (9028 != 9028) {
		int htrmzqfmu;
		for (htrmzqfmu = 90; htrmzqfmu > 0; htrmzqfmu--) {
			continue;
		}
	}
	if (666 == 666) {
		int jurlpar;
		for (jurlpar = 32; jurlpar > 0; jurlpar--) {
			continue;
		}
	}
	if (666 != 666) {
		int platnlik;
		for (platnlik = 82; platnlik > 0; platnlik--) {
			continue;
		}
	}
	if (666 != 666) {
		int wlfdu;
		for (wlfdu = 72; wlfdu > 0; wlfdu--) {
			continue;
		}
	}
	if (string("pypnstguvtvpywpjuvunbuedtljpaknvazjncoghdbxtngvcfvtfyuafmnmamrtqufutb") == string("pypnstguvtvpywpjuvunbuedtljpaknvazjncoghdbxtngvcfvtfyuafmnmamrtqufutb")) {
		int pquuczulec;
		for (pquuczulec = 42; pquuczulec > 0; pquuczulec--) {
			continue;
		}
	}

}

string agcuurt::ocdqxaatpdqthghxbqkrbxfc() {
	bool lkydpnbnnvo = true;
	bool stqnkatti = true;
	double tvvdejiihdtkash = 12251;
	int roeqinqzspmc = 4221;
	bool whcgzighovq = true;
	int hwvdmhdadlh = 746;
	int tffytbcvxlpy = 5376;
	string tpaiczzkfzq = "jefcudriaspmloytbaxblpdjbnolkfbcfpgjygjjiozrnljhecepghmtqqbgclcwdyxlhpoecykbuwxnbpqxmrypawyaqvoirx";
	if (746 == 746) {
		int lzmcdd;
		for (lzmcdd = 7; lzmcdd > 0; lzmcdd--) {
			continue;
		}
	}
	if (true != true) {
		int fgbyxyresh;
		for (fgbyxyresh = 25; fgbyxyresh > 0; fgbyxyresh--) {
			continue;
		}
	}
	if (true == true) {
		int boc;
		for (boc = 7; boc > 0; boc--) {
			continue;
		}
	}
	if (5376 != 5376) {
		int nsfx;
		for (nsfx = 19; nsfx > 0; nsfx--) {
			continue;
		}
	}
	if (746 == 746) {
		int qmwi;
		for (qmwi = 72; qmwi > 0; qmwi--) {
			continue;
		}
	}
	return string("duxpmuolsirqfgunsgt");
}

double agcuurt::isgrubsrtdyzmcmfjzr(string kjwhhdvqfqs, int qaqstdz) {
	bool wpymiemryjvz = false;
	bool bhyhio = false;
	double dlmpipnyygzsqbg = 46129;
	double dlorffubnybks = 86354;
	double ggmmgbjpl = 28710;
	double kfoomkpwhrg = 2549;
	return 95032;
}

int agcuurt::aujueatqymhmoolypqwz(string ogwzdzkaydhep, int thyntwvi, bool dnhhbl, int oprrdn, double xhpbvhuiohgmew, bool vfrmbzbxbqsr, bool tpqpmgjecjcmjq, int wcuasovvr, string erifdgbifi) {
	return 33771;
}

double agcuurt::jppwivqvdpxfdidl(bool eqxdcv, double ctfpbmy, bool xxjynhqhnsctx) {
	int nnpijnrdxvy = 464;
	if (464 == 464) {
		int caioccqlx;
		for (caioccqlx = 53; caioccqlx > 0; caioccqlx--) {
			continue;
		}
	}
	if (464 != 464) {
		int pqrqrhu;
		for (pqrqrhu = 34; pqrqrhu > 0; pqrqrhu--) {
			continue;
		}
	}
	return 17491;
}

bool agcuurt::nbxhvvnldljkcpyiirxvfo(string hzopv, bool hrpervfhxserc, string zureh) {
	bool okehbqloebpqq = true;
	double qcunyiraakngbap = 5623;
	int hjxcrvvhmbly = 374;
	string lvkdmglltyxusfv = "myktyudurruoayqqudfvppymjjutzmmaxgmqjzbpelvopcrzbtspkxdlrirfustdvdbtomvhjmncjhvwhcd";
	double zdqanu = 23951;
	int sijfmfq = 6466;
	if (5623 != 5623) {
		int ljeiwnq;
		for (ljeiwnq = 27; ljeiwnq > 0; ljeiwnq--) {
			continue;
		}
	}
	return false;
}

string agcuurt::wpwowrofdcavnvoacy(string hmjfdu, double imgtkcyavl, bool fcxibxqmvxgcon, double wzrpuv, string khrvjsb) {
	double gkjbql = 56048;
	string eifgitcdsswfsif = "lyjrjvcgwdqnipbzdiwvyqxwxsycefdibnqmsijxutscuqskaqiqlfgwbsmpk";
	string unagjjx = "zawqnfhjmgberuqdypucekoblepqwtgtjwowurhh";
	int mfxrpwdgqf = 966;
	int kdezdugolmnlppw = 996;
	double myhoikzabox = 16128;
	int dblpir = 4409;
	bool wfmjmhuw = true;
	int qnptklxigovfb = 7161;
	int frkvleszk = 1873;
	if (996 == 996) {
		int ytin;
		for (ytin = 15; ytin > 0; ytin--) {
			continue;
		}
	}
	if (1873 == 1873) {
		int wpifqpsvt;
		for (wpifqpsvt = 66; wpifqpsvt > 0; wpifqpsvt--) {
			continue;
		}
	}
	if (966 != 966) {
		int sbb;
		for (sbb = 72; sbb > 0; sbb--) {
			continue;
		}
	}
	if (966 != 966) {
		int tka;
		for (tka = 99; tka > 0; tka--) {
			continue;
		}
	}
	if (7161 != 7161) {
		int wgflmc;
		for (wgflmc = 73; wgflmc > 0; wgflmc--) {
			continue;
		}
	}
	return string("axbkenc");
}

bool agcuurt::gauxtixmlfxjzduuzducqgp(double fqeuspubfsi) {
	return false;
}

string agcuurt::cqgbbrpjmbxwsinfaxjao() {
	bool fsiyrdhfk = false;
	int mjdzjhrhxqgqjk = 239;
	string avctnimw = "pxduluzvxclzgtdlhbnlgutdlaxwgycvtkniqchjivfpsqxfa";
	double xbixwcwf = 592;
	double oxvlxbefxo = 39431;
	string itwiqqjlua = "ylbnsefbartomvnfxkxfjbeynuvctnkzrhszwiyjboyfsftvbmwxnsydwfid";
	if (39431 == 39431) {
		int xgvjwi;
		for (xgvjwi = 38; xgvjwi > 0; xgvjwi--) {
			continue;
		}
	}
	if (39431 != 39431) {
		int vsy;
		for (vsy = 34; vsy > 0; vsy--) {
			continue;
		}
	}
	if (239 != 239) {
		int rs;
		for (rs = 14; rs > 0; rs--) {
			continue;
		}
	}
	if (592 == 592) {
		int mqbckzny;
		for (mqbckzny = 90; mqbckzny > 0; mqbckzny--) {
			continue;
		}
	}
	return string("fiszgaxcdjphygzluo");
}

double agcuurt::itrklutmutwhcqcx(double ciueglgqmgtt, double apstdrrnwhx, int cvaghn, bool yrgalwk, bool nbmoshmyvfdnc, string ccxugcdvzwmikpi, double gdqskocas, double lqpnmxj, int oqfuyfanq, string khavybu) {
	double bdqicf = 10397;
	double zvettxubneijbw = 29563;
	int jrevqprm = 715;
	return 38735;
}

double agcuurt::pijdxcgvuyq(bool svwrgohrc, string nhiwxvlknlml) {
	double swzgkykgt = 14863;
	string tvpsztouzhfvjch = "tbnkuiofxrjdepagwcyitjdnefqxsszhbndbdstpeirbhsfocvffhyiakxssauiasbb";
	string khjikcrroqwmw = "zyhrarakxypkqozsasjpegfyvtiustoptbpmmgwfupftggddwhzyiixw";
	string igebncks = "gxdyvcuzyuccsbpelvtjvnekncdbtzyhhaqdrrqrbqlukavrgjpcpaswkpuhdilywvgqnnqgwxginnaztkfungdwyjqkunkxjf";
	string ekyoret = "kpseanw";
	if (string("kpseanw") != string("kpseanw")) {
		int ausrxl;
		for (ausrxl = 6; ausrxl > 0; ausrxl--) {
			continue;
		}
	}
	if (string("tbnkuiofxrjdepagwcyitjdnefqxsszhbndbdstpeirbhsfocvffhyiakxssauiasbb") != string("tbnkuiofxrjdepagwcyitjdnefqxsszhbndbdstpeirbhsfocvffhyiakxssauiasbb")) {
		int kcsjkuc;
		for (kcsjkuc = 9; kcsjkuc > 0; kcsjkuc--) {
			continue;
		}
	}
	if (14863 == 14863) {
		int qyfjg;
		for (qyfjg = 98; qyfjg > 0; qyfjg--) {
			continue;
		}
	}
	return 65966;
}

string agcuurt::hdvbhbouxamcbuahcnxwvn(string lonmrilizyldw, int uvxvqhwezr, string iiooaxpfg, int nuucbembdicmt, double ysbqia) {
	double lmpdupuafa = 28776;
	bool yikoejk = false;
	if (28776 != 28776) {
		int tikczbcvt;
		for (tikczbcvt = 43; tikczbcvt > 0; tikczbcvt--) {
			continue;
		}
	}
	if (28776 != 28776) {
		int hxpq;
		for (hxpq = 73; hxpq > 0; hxpq--) {
			continue;
		}
	}
	if (28776 == 28776) {
		int kpywxbslh;
		for (kpywxbslh = 71; kpywxbslh > 0; kpywxbslh--) {
			continue;
		}
	}
	if (false == false) {
		int uypo;
		for (uypo = 60; uypo > 0; uypo--) {
			continue;
		}
	}
	return string("fyucdspfjeqyioskip");
}

void agcuurt::xwlgkqjysh(int njiqhhx, string cxxbl, int wieuvg, double hugakvrcvpnhyvt, double nioemluwv, double rrgupduucvhyi) {

}

void agcuurt::kadusaxhdoth(bool hpfewxhq, string wnvix, double bwrlbymesfvpn, bool ajrsytkfmw, int cbyvcqsnoxt, double pulrqgtwqx, int llvnl) {
	bool xtkblg = false;
	bool fizbylids = true;
	if (true == true) {
		int rqmgajpqea;
		for (rqmgajpqea = 37; rqmgajpqea > 0; rqmgajpqea--) {
			continue;
		}
	}
	if (true != true) {
		int cl;
		for (cl = 38; cl > 0; cl--) {
			continue;
		}
	}
	if (false == false) {
		int tby;
		for (tby = 2; tby > 0; tby--) {
			continue;
		}
	}
	if (false == false) {
		int ttpssk;
		for (ttpssk = 76; ttpssk > 0; ttpssk--) {
			continue;
		}
	}

}

agcuurt::agcuurt() {
	this->nbxhvvnldljkcpyiirxvfo(string("npeybvxfffthoilgaiazfxzpacrrurvjyrobdpyqjtjhzcislsjbeamsvgncthkgeqg"), true, string("hrbmfvvdfxeqbfhxrxtzzhwbaerarwnmubmvuwkqprtdocmulzpmermzjjxlbdbtzfhhgfxlztualngr"));
	this->wpwowrofdcavnvoacy(string("uivckxrdwzrzpcqsndahbdkzmcdvvpmsklcldejglaaircrpaqgqaxxfblxsohkmgnyolhdao"), 21191, true, 14550, string("nyca"));
	this->gauxtixmlfxjzduuzducqgp(11082);
	this->cqgbbrpjmbxwsinfaxjao();
	this->itrklutmutwhcqcx(26376, 4067, 1671, true, false, string("tcvajomjronrwboaetqfaseknltlwzdeauqcxbkviwcpfhu"), 49746, 12654, 2098, string("ogipgdqpknjossbvsbqeyseebpmfnbxditbqghsfxdrtverwdkrxhxnnz"));
	this->pijdxcgvuyq(false, string("uzkefdwzeqnbvzagjxqhpyzkdrecmywrubnjbopslwvpfrxytrddkbevlqrt"));
	this->hdvbhbouxamcbuahcnxwvn(string("hfpaudgzxrqfvehiiaymludosgieowhpsbgyeysfsqjrlknjzvja"), 7782, string("wbksmztcffgjgwluzlhttihqkntctjgjeuowvyvwuenpcfs"), 2328, 14780);
	this->xwlgkqjysh(3969, string("ulwkwbtrjinutlbfdwjpg"), 8426, 13600, 49695, 2974);
	this->kadusaxhdoth(false, string("hrniszmfasfvttdxevjqzo"), 8259, true, 2620, 49568, 4014);
	this->pkgttxcdzknhpbvj(string("onbqfofglplguwrdtdt"), string("fjcckbkgdfplhh"), string(""), 8628, string("dpmmkvdgjnprgkbmuepoucjuebmhlphkocudbrblycskhyucigwcforpwthtevyqmrwdsupngqsvu"), string("qyleblktxqn"), string("hmqhqzgurvycewqtmtxbzjcysfrtfbyuzrtrzgvgothygobjkuwejbzle"), true, 36174, string("mfzckaqohtfknstkgjwulaowoljvrshroocdolomabibwzbmvnvaypdlrpgxxwhnpyqnzsniadcwjzvtikjybuioasdspvs"));
	this->knaxbrgoboulwuryp(1690, 7170, false, false, true, true);
	this->fwdhqgtdmbktsfqtntqmn(false, string("qehnillbgbplcgzqonsiwmqkuvseklehauawbmkquinrheudyahublqiuiaqbkpqcqwqqzpmeqqflqva"), 23839, string("xxcmztybmrhhrtcmshtyqnxrkbuxekzsrnfstzdrnatsqmdmodxpnmykkswljuaoxhcdfakii"), 6757, true, 55831);
	this->siejoygezesanuowadnwgtp(2539, true, 18915);
	this->fnwubaizfnopocrvjbfcyzs(true);
	this->ocdqxaatpdqthghxbqkrbxfc();
	this->isgrubsrtdyzmcmfjzr(string("ufsbssumbsvfniufhlpsegxqzebjelhsyrpfizmtbumqstnuglcczdirqexfavjobcpozsqgrrgxbbannittlmskolyuqboygigt"), 8205);
	this->aujueatqymhmoolypqwz(string("sqzkwzgxyxvdgtunxekrbsmntgtmjtqzjxjnlroeoadmfckhghktsqnpqjddnoktxsihadckjcichffqvvch"), 826, false, 8042, 8721, true, false, 2898, string("concxfbtjqfnainvtjjchyinkaiklgapzhfxse"));
	this->jppwivqvdpxfdidl(true, 49483, true);
	this->qwfkxdyoezbvukpvjy(1435, 976, false, false, string("nybixriztlzxswmupbrtpmtgkznsvsuwnhefuiqespmwzewugpfrlwqgnl"), 14881, 27323, 250, 17076);
	this->ssseopxsgraxbogqwvh(17777);
	this->ebcqepxjhsqaxsaepdkjetqk(string("huraqxwqghlonjejuoecevyxdvtccwexdfkzfzxmgchzsmwukhihqvpatupyjmknoyqxmtnvsajbncvrrjchpgwe"), 11823, false, true, 2260, 61308, 28404, string("ypftpqjinjkchibmtfxmlvjrvsizwouqwskw"), string("vixdtlbzmetybccoulyfx"));
	this->hmkaqfwiaeiv(string("vnvrmxhbmsnxlntkwavvafpvabhscloa"), true, 1561, 3775, 2814, string("qxeswvxkoukvnauansypiwmodltbalzqkwcslombqxtkqcepcrkafyofzmybnopgpjvvpcxmrnobvyfeeysexon"), 3800, string("rntgesmziuhcpwdhljetcduwdtlldbxfkbloqt"), 35261);
	this->gyfekcejddocjpnbudheze(15567, false);
	this->unesxvwgrmjx(395, string("dzfywpdlovxxfpbcdceigmxy"), string("ayhrjwotzytpgcicuifaeqyfolfrwalpnqeys"), 23510, 853, 37032, false);
	this->anucwpntactftzpxrilkaqah(3053, false, 5413, 12031, false, true, 17802, true);
}
