#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>

struct Variables
{
	Variables()
	{

	}

	struct Ragebot_s
	{
		bool 	Enabled;
		bool 	AutoFire;
		bool AutoPistol;
		bool 	Hold;
		bool AntiSpawnProtect;
		int 	HoldKey;
		int 	Filter; // will hold flags for team/enemy/etc.
		bool 	Silent;
		int Selections;
		bool	RCS;
		bool 	VisCheck;
		bool 	AutoWall;
		float	AutoWallDmg;
		float  BestDamageSlow;
		int HitScaneMode;
		int 	TargetMethod;
		int		Hitbox;
		int 	FOV;
		float pointscale;
		bool 	HitChance;
		float	HitChanceAmt;
		bool 	AutoStop;
		bool 	AutoCrouch;
		bool	AutoScope;
		bool	AutoSlow;
		bool 	FriendlyFire;
		bool	Aimstep;
		float	AimstepAmount;
		bool   Fakelagg;
		float		Fakelaggamount;
		bool	HitScanAll;
		bool	BodyAim;
		bool	HitScanHitbox;
		bool	UntrustedCheck;
		struct
		{
			bool	Enabled;
			bool	FakeYaw;
			int		Pitch;
			int		Yaw;
			int		Fyaw;
			bool	AimAtPlayer;
			bool	DetectEdge;
			bool	PSilent;
			bool FixAngle;
			int Choke;
		} Antiaim;
	} Ragebot;

	struct
	{
		struct
		{
			bool 	Enabled;
			int WType;
			int 	Key;
			bool	OnKey;
			int 	Filter; // will hold flags for team/enemy/etc.
			bool 	RCS;
			float 	RCSAmount;
			float	Speed;
			bool 	VisCheck;
			int 	Hitbox;
			float 	FOV;
			float PsilentBullet;
			float StartBullet;
			float StopBullet;
			bool 	AlwaysOn;
			bool	FriendlyFire;
			bool Psilent;
			bool pPsilent;
		} Aimbot;

		struct
		{
			bool	Enabled;
			bool	AutoFire;
			int		Key;
			bool 	HitChance;
			bool Autowall;
			float	HitChanceAmt;

			struct
			{
				bool Head;
				bool Chest;
				bool Stomach;
				bool Arms;
				bool Legs;
				bool Friendly;
			} Filter;

		} Triggerbot;

	} Legitbot;

	struct
	{
		bool 	Enabled;
		bool 	Box;
		bool    ShowName;
		bool    ShowHealth;
		bool    ShowWeapon;
		bool    ShowInfo;
		bool    ShowRank;
		bool    ChamsEnable;
		bool CrossEnable;
		bool NoScopeBorder;
		bool NoVisualRecoil;
		bool NoFlash;
		bool EnableHandChams;
		bool Thirdperson;
		bool SkeletonEsp;
		int ThirdpersonType;
		int SkeletonEspType;
		int HandChamsStyle;
		int CrossType;
		int     ChamsStyle;
		int     HealthType;
		int 	BoxType;
		bool	Skeleton;
		bool	Glow;
		bool	BulletTrace;
		float   BulletTraceLength;
		float	Cspeed;
		bool	CrosshairOn;
		bool DrawFov;
		int		CrosshairType;
		int		CrosshairStyle;
		float 	TeamColor[3];
		float 	TeamColorWall[3];
		float 	EnemyColor[3];
		float 	EnemyColorWall[3];
		int HandsMode;
		int WeaponMode;
		int Knife;
		struct
		{
			bool Enemies;
			bool Friendlies;
			bool Weapons;
			bool Decoy;
			bool C4;
		} Filter;

		struct
		{
			bool	Enabled;
			int 	Mode;
			bool	Wireframe;
			bool	XQZ;
			float 	TeamColor[3];
			float 	TeamColorWall[3];
			float  	EnemyColor[3];
			float 	EnemyColorWall[3];
			bool HandsOn;
			bool WeaponOn;
			float Transparent;
			float OverrideView;
		} Chams;

		struct
		{
			bool fltrPlayer;
			bool fltrShowTeam;
			bool fltrWeapon;
			bool fltrC4;
			bool fltrShowName;
			//Old
			bool 	Name;
			bool 	Health;
			bool Armor;
			bool 	HealthString;
			bool ArmorString;
			bool 	Weapon;
			bool	Flashed;
		} Info;

		struct
		{
			bool	Hands;
			bool	Weapon;
			bool	VisualRecoil;
			bool	Flash;
			bool	NoSmoke;
			bool    Smoke;
			bool	Sky;
		} Removals;
	} Visuals;

	struct
	{
		bool 	Bhop;
		bool namesteal;
		bool Locspam;
		int locchat;
		int 	AutoStrafe;
		bool   Cstrafe;
		bool Thirdperson;
		bool	ChatSpam;
		bool	ChatSpam2;
		int		ChatSpamMode;
		int ChatSpamMode2;
		int NameStealMode;
		bool	Watermark;
		bool Skinchanger1;
		bool	Ranks;
		bool	TP;
		bool	AirStuck;
		bool    Teleport;
		int		AirStuckKey;
		int		TeleportKey;
		int KNIFE;
		int AKSKIN;
		int DEAGLE;
		int SSG08;
		bool	AutoAccept;
	} Misc;

	struct
	{
		bool	Opened = false;
		int 	Key;
		bool	Ragebot = true;
		bool	Legitbot = false;
		bool	Visual = false;
		bool	Misc = false;
		bool    SkinChanger = false;
		bool    Config = false;
		int ConfigBox;
		const char* config_path = "";
	} Menu;

	struct
	{
		bool EnabledSkin = true;
		bool EnabledKnife = true;
		bool EnabledGlove = true;
		int Knife = 4; // 0 - Bayonet, 1 - Flip, 2 - Gut, 3 - Karambit, 4 - M9Bayonet, 5 - Huntsman, 6 - Falchion, 7 - Bowie, 8 - Butterfly, 9 - Daggers, 10 - Golden
		int Glove = 0; // 0 - Bloodhound; 1 - Sport; 2 - Driver; 3 - Wraps; 4 - Moto; 5 - Specialist
		int GloveSkin = 0;
	}SkinChanger;
};

extern Variables Vars;

