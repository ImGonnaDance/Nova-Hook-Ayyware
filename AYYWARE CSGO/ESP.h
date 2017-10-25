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

#pragma once

#include "Hacks.h"
extern int hittedLogHits[65];
extern int missedLogHits[65];
extern int ResolverMode;
extern float enemyLBYTimer[65];
extern float enemysLastProxyTimer[65];
extern float enemyLBYDelta[65];
extern float lineLBY;
extern float lineRealAngle;
extern float lineFakeAngle;
extern int ResolverStage[65];
extern float autowalldmgtest[65];
extern float pEntityPredLbyTime[65];
extern bool antiAimSide;
extern int dynamicAntiAimSide;
extern bool canBacktrack;
extern float aaDistanceNew;
extern float aaDistanceOld;
extern int enemysCurrentAmmo[65];

extern bool enemysLbyUpdatedIndicator[65];

namespace Globals
{
	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;
	extern int Shots;
	extern bool change;
	extern int TargetID;
}

class CEsp : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	std::vector<IMaterial*> SkyTextures;
	std::vector<IMaterial*> WorldTextures;
private:

	// Other shit
	IClientEntity *BombCarrier;

	struct ESPBox
	{
		int x, y, w, h;
	};

	// Draw a player
	void NoSky();
	void DoFOV();
	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo);

	// Get player info
	Color GetPlayerColor(IClientEntity* pEntity);
	void DrawBullets();
	
	void drawBacktrackedSkelet(IClientEntity * base);

	void DrawLinesAA(Color color);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);
	

	// Draw shit about player
	void SpecList();
	void Test(CEsp::ESPBox size, Color color, IClientEntity * pEntity);
	void DrawBox(ESPBox size, Color color);
	void DrawBrackets(ESPBox size, Color color);
	void DrawCorners(ESPBox size, Color color);
	void DrawFilled(ESPBox size, Color color);
	void DrawCustom(ESPBox size, Color color, IClientEntity* pEntity);
	void DrawTopCustom(ESPBox size, Color color, IClientEntity* pEntity);
	void DrawName(player_info_t pinfo, ESPBox size, IClientEntity* pEntity);
	void DrawHealthText(player_info_t pinfo, ESPBox size, IClientEntity* pEntity);
	void Ammo(IClientEntity * pEntity, player_info_t pinfo, CEsp::ESPBox size);
	void Ammo1(IClientEntity * pEntity, player_info_t pinfo, CEsp::ESPBox size);
	void DrawGrenades(IClientEntity * pEntity);
	void DrawHealth(IClientEntity* pEntity, ESPBox size);
	void DrawArmor(IClientEntity* pEntity, CEsp::ESPBox size);
	void Tracers(CEsp::ESPBox size, Color color, IClientEntity * pEntity);
	void DrawGlow();
	void DrawHealth2(IClientEntity* pEntity, ESPBox size);
	void DrawInfo(IClientEntity* pEntity, ESPBox size);
	void DrawCross(IClientEntity* pEntity);
	void DrawSkeleton(IClientEntity* pEntity);

	//void DrawChicken(IClientEntity* pEntity, ClientClass* cClass);
	void DrawDrop(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
};

void DiLight();