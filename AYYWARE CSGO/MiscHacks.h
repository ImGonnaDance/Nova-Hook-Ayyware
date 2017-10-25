/*
Rest In Peace ApocalypseCheats
*/

#pragma once

#include "Hacks.h"

Vector GetAutostrafeView();

class CMiscHacks : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	void CustomName();
private:
	void HAutoJump(CUserCmd *pCmd);
	void AutoJump(CUserCmd *pCmd);
	void AutoJumpSMAC(CUserCmd *pCmd);
	void EdgeJump(CUserCmd *pCmd);
	void Walkbot(CUserCmd *pCmd);
	void PerfectStrafe(CUserCmd *pCmd);
	void LegitStrafe(CUserCmd *pCmd);
	void RageStrafe(CUserCmd *pCmd);
	void Namespam();
	void PostProcess();

	// Name Changers
	void AyyBgone();
	void NoName();
	void NameSteal();
	void NovaHook();
	void Aimware();
	void SteamSupport();

	// Chat Spammers
	void ChatSpamInterwebz();
	void ChatSpamName();
	void ChatSpamDisperseName();
	void ChatSpamRegular();
	void ChatSpamAimwareChat();
	void ChatSpamAimwareName();
	void ChatSpamReports();
	void ChatSpamCallout();
	void Fakelag(CUserCmd *pCmd, bool &bSendPacket);
};



