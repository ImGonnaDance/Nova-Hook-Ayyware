/*
Rest In Peace ApocalypseCheats
*/

#pragma once

#include "GUI.h"
#include "Controls.h"



class CRageBotTab : public CTab
{
public:
	void Setup();
	
	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CSlider2  AimbotFov;
	CCheckBox AimbotSilentAim;
	CCheckBox PVSFix;
	CCheckBox AimbotResolver;
	CCheckBox AdvancedResolver;
	CCheckBox AutoRevolver;
	CSlider2 AddFakeYaw;
	CSlider2 CustomPitch;
	CCheckBox LBY;
	CComboBox FakeYaw;
	CCheckBox LowerbodyFix;
	CCheckBox PreferBodyAim;
	CCheckBox AWPAtBody;
	CSlider2 BaimIfUnderXHealth;
	CSlider2 XShots;
	CCheckBox EnginePrediction;
	CCheckBox AimbotAimStep;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CKeyBind  AimbotStopKey;

	CGroupBox TargetGroup;
	CComboBox TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBox TargetHitbox;
	CComboBox TargetHitscan;
	CCheckBox TargetMultipoint;
	CSlider   TargetPointscale;

	CGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;
	CCheckBox AccuracyAutoWall;
	CSlider2 AccuracyMinimumDamage;
	CCheckBox AccuracyAutoStop;
	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CSlider2   AccuracyHitchance;
	CComboBox AccuracyResolver;

	CGroupBox AntiAimGroup;
	CCheckBox AntiAimEnable;
	CComboBox AntiAimPitch;
	CComboBox AntiAimYaw;
	CCheckBox FlipAA;
	CCheckBox AntiAimEdge;
	CCheckBox EdgeAntiAim;
	CSlider2 AntiAimOffset;
	CCheckBox AntiAimKnife;
	CCheckBox AntiAimTarget;
	CCheckBox AccuracyResolver2;
};

class CLegitBotTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	CGroupBox AimbotGroup;

	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotSmokeCheck;

	CGroupBox TriggerGroup;
	CCheckBox TriggerEnable;
	CCheckBox TriggerKeyPress;
	CKeyBind  TriggerKeyBind;
	CCheckBox TriggerHitChance;
	CSlider   TriggerHitChanceAmmount;
	CSlider   TriggerDelay;
	CSlider   TriggerBurst;
	CSlider   TriggerBreak;
	CCheckBox TriggerRecoil;

	CGroupBox TriggerFilterGroup;
	CCheckBox TriggerHead;
	CCheckBox TriggerChest;
	CCheckBox TriggerStomach;
	CCheckBox TriggerArms;
	CCheckBox TriggerLegs;
	CCheckBox TriggerTeammates;
	CCheckBox TriggerFlashCheck;
	CCheckBox TriggerSmokeCheck;

	CGroupBox WeaponMainGroup;
	CSlider   WeaponMainSpeed;
	CSlider   WeaponMainFoV;
	CSlider WeaponMainRecoil;
	CComboBox WeaponMainHitbox;
	CSlider WeaponMainAimtime;
	CSlider WeaoponMainStartAimtime;

	CGroupBox WeaponPistGroup;
	CSlider   WeaponPistSpeed;
	CSlider   WeaponPistFoV;
	CSlider WeaponPistRecoil;
	CComboBox WeaponPistHitbox;
	CSlider WeaponPistAimtime;
	CSlider WeaoponPistStartAimtime;

	CGroupBox WeaponSnipGroup;
	CSlider   WeaponSnipSpeed;
	CSlider   WeaponSnipFoV;
	CSlider WeaponSnipRecoil;
	CComboBox WeaponSnipHitbox;
	CSlider WeaponSnipAimtime;
	CSlider WeaoponSnipStartAimtime;

	CGroupBox WeaponMpGroup;
	CSlider   WeaponMpSpeed;
	CSlider   WeaponMpFoV;
	CSlider WeaponMpRecoil;
	CComboBox WeaponMpHitbox;
	CSlider WeaponMpAimtime;
	CSlider WeaoponMpStartAimtime;

	CGroupBox WeaponShotgunGroup;
	CSlider   WeaponShotgunSpeed;
	CSlider   WeaponShotgunFoV;
	CSlider WeaponShotgunRecoil;
	CComboBox WeaponShotgunHitbox;
	CSlider WeaponShotgunAimtime;
	CSlider WeaoponShotgunStartAimtime;

	CGroupBox WeaponMGGroup;
	CSlider   WeaponMGSpeed;
	CSlider   WeaponMGFoV;
	CSlider WeaponMGRecoil;
	CComboBox WeaponMGHitbox;
	CSlider WeaponMGAimtime;
	CSlider WeaoponMGStartAimtime;
};

class CCustomTab : public CTab
{
public:
	void Setup();

	CGroupBox AFKBotGroup;
	CCheckBox AFKBotEnable;
	CCheckBox AFKBotMessage;

	CGroupBox HitmarkerGroup;
	CCheckBox HitmarkerEnable;
	CCheckBox ShowHitmarker;
	CCheckBox HitmarkerOof;
	CCheckBox HitmarkerSound;
	CCheckBox HitmarkerMLG;

	CGroupBox DeathSoundGroup;
	CCheckBox DeathSoundEnable;
	CCheckBox DeathSoundOof;

	CGroupBox NameChangerGroup;
	CCheckBox NameChangerEnable;
	CComboBox NameChangerText;

	CGroupBox TagChangerGroup;
	CCheckBox TagChangerEnable;
	CComboBox TagChangerText;

	CGroupBox ChatSpammerGroup;
	CComboBox ChatSpammerText;

	CGroupBox CrosshairGroup;
	CCheckBox CrosshairEnable;
	CComboBox CrosshairStyle;

	CGroupBox SkyboxChangerGroup;
	CCheckBox SkyboxChangerEnable;
	CComboBox SkyboxChangerType;

	CGroupBox KnifeModelG;
	CCheckBox KnifeModelE;
	CComboBox KnifeModelT;
	CButton   KnifeModelA;
};

class CVisualsTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Options Settings
	CGroupBox OptionsGroup;
	CComboBox OptionsBox;
	CCheckBox OptionsBoxFill;
	CCheckBox OptionsBoxFillColor;
	CCheckBox OptionsName;
	CCheckBox OptionsHealth;
	CCheckBox OptionsArmor;
	CCheckBox OptionsDistance;
	CCheckBox OptionsWeapon;
	CCheckBox OptionsInfo;
	CCheckBox OptionsHelmet;
	CCheckBox OptionsKit;
	CCheckBox OptionsDefuse;
	CCheckBox OtherWireframe;
	//CCheckBox OptionsGlow;
	CComboBox OptionsChams;
	CCheckBox OptionsSkeleton;
	CCheckBox OptionsVisibleOnly;
	CCheckBox OptionsAimSpot;
	CCheckBox OptionsCompRank;
	CCheckBox OtherAsus;
	CCheckBox DisablePostProcess;
	CCheckBox HealthText;
	CCheckBox AmmoText;
	CCheckBox Tracers;
	CCheckBox Glow;
	CSlider2 GlowZ;

	// Filters Settings
	CGroupBox FiltersGroup;
	CCheckBox FiltersAll;
	CCheckBox FiltersPlayers;
	CCheckBox FiltersEnemiesOnly;
	CCheckBox FiltersWeapons;
	CCheckBox FiltersChickens;
	CCheckBox FiltersC4;
	CCheckBox FiltersDead;

	// Other Settings
	CGroupBox OtherGroup;
	CCheckBox OtherCrosshair;
	CComboBox OtherRecoilCrosshair;
	CCheckBox OtherHitmarker;
	CCheckBox OtherRadar;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox OtherNoSky;
	CCheckBox OtherNoFlash;
	CSlider   OtherNoFlashVal;
	CCheckBox GrenadePrediction;
	CCheckBox Watermark;
	CCheckBox Clock;
	CCheckBox Radar;

	CCheckBox OtherNoSmoke;

	CCheckBox NightMode;
	CSlider NightModeSlider;
	CCheckBox NoScopeBorder;
	CButton ResetXD;
	//CCheckBox OtherNoSmoke;
	CCheckBox OtherAsusWalls;
	CComboBox OtherNoHands;
	CSlider OtherViewmodelFOV;
	CSlider OtherFOV;
	CCheckBox OtherNoScope;

	CGroupBox PlayerChams;
	CComboBox PChamStyle;
	CCheckBox PEnemyOnly;
	CCheckBox PXQZ;
	CCheckBox PEnable;

	CGroupBox LocalChams;
	CComboBox LWeaponChams;
	CComboBox LHandChamStyle;
	CCheckBox LHXQZ;
	CCheckBox LWXQZ;
	CCheckBox LEnable;
};

class CMiscTab : public CTab
{
public:
	void Setup();
	CKeyBind  ThirdPersonKeyBind;
	CComboBox ThirdpersonAngle;
	CGroupBox ButtonGroup;
	CButton SaveButton;
	CButton LoadButton;
	//Options
	CComboBox SetNr;
	CButton Unload;
	CComboBox ConfigBox;
	CGroupBox ConfigGroup;
	CGroupBox Settings;
	CButton SettingsSave;
	CButton SettingsLoad;

	// Main Misc
	CGroupBox Main;
	CCheckBox Slowmo;
	CKeyBind VisToggle;

	// Other Settings
	CGroupBox OtherGroup;
	CCheckBox OtherAutoJump;
	CCheckBox OtherHAutoJump;
	CCheckBox OtherEdgeJump;
	CComboBox OtherAutoStrafe;
	CCheckBox OtherSafeMode;
	CComboBox OtherChatSpam;
	CCheckBox OtherTeamChat;
	CSlider	  OtherChatDelay;
	CKeyBind  OtherAirStuck;
	CKeyBind  OtherLagSwitch;
	CCheckBox OtherSpectators;
	CCheckBox OtherThirdperson;
	CCheckBox OtherThirdpersonFake;
	CCheckBox OtherSkinChanger;
	CCheckBox OtherWatermark;
	CCheckBox OtherAutoCounterStafe;
	CKeyBind  OtherAutoCounterStafeKey;
	//CCheckBox OtherAutoAccept;
	CCheckBox OtherWalkbot;
	CComboBox OtherClantag;
	CCheckBox DisableAll;
	CGroupBox MPGroup;
	CComboBox MAC10Skin;
	CComboBox P90Skin;
	CComboBox UMP45Skin;
	CComboBox BIZONSkin;
	CComboBox MP7Skin;
	CComboBox MP9Skin;

	CButton NApply;
	// Fake Lag Settings
	CGroupBox FakeLagGroup;
	CCheckBox FakeLagEnable;
	CSlider  FakeLagChoke;
	CSlider  FakeLagSend;
	CCheckBox ChokeRandomize;
	CCheckBox SendRandomize;
	//CCheckBox FakeLagWhileShooting;

	// Teleport shit cause we're cool
	CGroupBox TeleportGroup;
	CCheckBox TeleportEnable;
	CKeyBind  TeleportKey;





	/* LEGITBOT IN MISCTAB CONFIG AND SOME OTHER SHIT*/
	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Aimbot Settings
	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;

	CCheckBox AimbotBacktrack;

	CCheckBox AimbotAutoFire;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotAutoPistol;
	CSlider   AimbotInaccuracy;
	CKeyBind  AimbotDisableVis;

	// Main
	CGroupBox TriggerGroup;
	CCheckBox TriggerEnable;
	CCheckBox TriggerKeyPress;
	CKeyBind  TriggerKeyBind;
	CSlider  TriggerDelay;

	// Main
	CGroupBox WeaponMainGroup;
	CSlider   WeaponMainSpeed;
	CSlider   WeaponMainFoV;
	CCheckBox WeaponMainRecoil;
	CCheckBox WeaponMainPSilent;
	CSlider   WeaponMainInacc;
	CComboBox WeaponMainHitbox;
	CComboBox WeaponMainSecHitbox;

	// Pistol
	CGroupBox WeaponPistGroup;
	CSlider   WeaponPistSpeed;
	CSlider   WeaponPistFoV;
	CCheckBox WeaponPistRecoil;
	CCheckBox WeaponPistPSilent;
	CSlider   WeaponPistInacc;
	CComboBox WeaponPistHitbox;
	CComboBox WeaponPistSecHitbox;
	// Sniper
	CCheckBox WeaponSnipEnable;
	CGroupBox WeaponSnipGroup;
	CSlider   WeaponSnipSpeed;
	CSlider   WeaponSnipFoV;
	CCheckBox WeaponSnipRecoil;
	CCheckBox WeaponSnipPSilent;
	CSlider   WeaponSnipInacc;
	CComboBox WeaponSnipHitbox;
	CComboBox WeaponSnipSecHitbox;

	CGroupBox TriggerFilterGroup;

	CCheckBox TriggerHead;
	CCheckBox TriggerChest;
	CCheckBox TriggerStomach;
	CCheckBox TriggerArms;
	CCheckBox TriggerLegs;
	CCheckBox TriggerTeammates;
	CCheckBox TriggerRecoil;
	//More
	CGroupBox AimMoreGroup;
	CCheckBox FoVRadius;
	CCheckBox WeaponConfig;
	CComboBox WeaponConf;
	/* END OF LEGIT BOT IN MISCTAB */


	// Knife Changer/Skin Changer
	CLabel SkinActive;
	CCheckBox SkinEnable;
	CButton   SkinApply;

	// Knife
	CGroupBox KnifeGroup;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;




};

class CSkinchangerTab : public CTab
{
public:
	void Setup();

	// Knife Changer/Skin Changer
	CLabel SkinActive;
	CCheckBox SkinEnable;
	CButton   SkinApply;

	// Knife
	CGroupBox KnifeGroup;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;

	// Pistols
	CGroupBox PistolGroup;
	CComboBox GLOCKSkin;
	CComboBox USPSSkin;
	CComboBox DEAGLESkin;
	CComboBox MAGNUMSkin;
	CComboBox DUALSSkin;
	CComboBox FIVESEVENSkin;
	CComboBox TECNINESkin;
	CComboBox P2000Skin;
	CComboBox P250Skin;

	// MPs


	// Rifles
	CGroupBox Riflegroup;
	CComboBox M41SSkin;
	CComboBox M4A4Skin;
	CComboBox AK47Skin;
	CComboBox AUGSkin;
	CComboBox FAMASSkin;
	CComboBox GALILSkin;
	CComboBox SG553Skin;


	// Machineguns
	CGroupBox MachinegunsGroup;
	CComboBox NEGEVSkin;
	CComboBox M249Skin;

	// Snipers
	CGroupBox Snipergroup;
	CComboBox SCAR20Skin;
	CComboBox G3SG1Skin;
	CComboBox SSG08Skin;
	CComboBox AWPSkin;

	// Shotguns
	CGroupBox Shotgungroup;
	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;
	CComboBox NOVASkin;

	// Skinsettings
	CGroupBox SkinsettingsGroup;
	CCheckBox StatTrakEnable;
	/*
	*/

};

class CColorsTab : public CTab
{
public:
	void Setup();

};

class CSettingsTab : public CTab
{
public:
	void Setup();
	CGroupBox ButtonGroup;
	CButton SaveButton;
	CButton LoadButton;
	//Options
	CComboBox SetNr;
	CButton Unload;
	CComboBox ConfigBox;
	CGroupBox ConfigGroup;
	CGroupBox Settings;
	CButton SettingsSave;
	CButton SettingsLoad;

	CGroupBox ColorsGroup;

	CGroupBox MenuColor;
	CSlider2 MenuC;
	CSlider2 MenuR;
	CSlider2 MenuG;
	CSlider2 MenuB;

	CGroupBox SkeletonGroup;
	CSlider2 Skeleton;
	CSlider2 SkeletonR;
	CSlider2 SkeletonG;
	CSlider2 SkeletonB;

	CGroupBox TracerGroup;
	CSlider2 Tracer;
	CSlider2 TracerR;
	CSlider2 TracerG;
	CSlider2 TracerB;

	CGroupBox HitmarkerGroup;
	CSlider2 Hitmarker;
	CSlider2 HitmarkerR;
	CSlider2 HitmarkerG;
	CSlider2 HitmarkerB;

	CGroupBox CTChamsGroup;
	CSlider2 CTChams;
	CSlider2 CTChamsR;
	CSlider2 CTChamsG;
	CSlider2 CTChamsB;

	CGroupBox TChamsGroup;
	CSlider2 TChams;
	CSlider2 TChamsR;
	CSlider2 TChamsB;
	CSlider2 TChamsG;

	CGroupBox HandsGroup;
	CSlider2 Hands;
	CSlider2 HandsR;
	CSlider2 HandsG;
	CSlider2 HandsB;

	CGroupBox WeaponGroup;
	CSlider2 Weapon;
	CSlider2 WeaponR;
	CSlider2 WeaponG;
	CSlider2 WeaponB;

	CGroupBox BoxGroup;
	CSlider2 Box;
	CSlider2 BoxR;
	CSlider2 BoxG;
	CSlider2 BoxB;
};

class ApocalypseWindow : public CWindow
{
public:
	void Setup();
	CRageBotTab RageBotTab;
	CLegitBotTab LegitBotTab;
	CCustomTab CustomTab;
	CVisualsTab VisualsTab;
	CMiscTab MiscTab;
	CSettingsTab SettingsTab;
	CSkinchangerTab SkinchangerTab;


	CGroupBox ConfigGroup;
	CGroupBox Settings;
	CButton SettingsSave;
	CButton SettingsLoad;
	

	CButton SaveButton1;
	CButton LoadButton1;
	CButton SaveButton;
	CButton LoadButton;
	CButton UnloadButton;
	CButton PanicButton;
	CButton ThirdButton;
	CButton ByPass;
	CButton FirstButton;
	CComboBox ConfigBox;
};

namespace Menu
{
	void SetupMenu();
	void DoUIFrame();

	extern ApocalypseWindow Window;
};
