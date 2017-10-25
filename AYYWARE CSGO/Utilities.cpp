/*
Syn's Apocalypse Framework
*/
#pragma once
#define _CRT_SECURE_NO_WARNINGS

// Includes
#include "Utilities.h"
#include <fstream>
#include <Psapi.h>

bool FileLog = false;
std::ofstream logFile;

// --------         Utilities Core           ------------ //
// Opens a debug console
void Utilities::OpenConsole(std::string Title)
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	SetConsoleTitle(Title.c_str());
}

// Closes the debug console
void Utilities::CloseConsole()
{
	FreeConsole();
}

// Outputs text to the console
void Utilities::Log(const char *fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf(logBuf + strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	//Output to console
	if (logBuf[0] != '\0')
	{
		SetConsoleColor(FOREGROUND_INTENSE_GREEN);
		printf("[%s]", GetTimeString().c_str());
		SetConsoleColor(FOREGROUND_WHITE);
		printf(": %s\n", logBuf);
	}

	if (FileLog)
	{
		logFile << logBuf << std::endl;
	}
}

// Gets the current time as a string
std::string Utilities::GetTimeString()
{
	//Time related variables
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];

	//Get current time
	time(&current_time);
	time_info = localtime(&current_time);

	//Get current time as string
	strftime(timeString, sizeof(timeString), "%I:%M%p", time_info);
	return timeString;
}

// Sets the console color for upcoming text
void Utilities::SetConsoleColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Enables writing all log calls to a file
void Utilities::EnableLogFile(std::string filename)
{
	logFile.open(filename.c_str());
	if (logFile.is_open())
		FileLog = true;
}


// --------         Utilities Memory           ------------ //

DWORD Utilities::Memory::WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle)
	{
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
	{
		if (*szMask == 'x' && *Mask != *Data)
		{
			return false;
		}
	}
	return (*szMask) == 0;
}

DWORD Utilities::Memory::FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}
#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))
DWORD Utilities::Memory::FindPatternV2(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

DWORD Utilities::Memory::FindTextPattern(std::string moduleName, char* string)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	int len = strlen(string);
	char* szMask = new char[len + 1];
	for (int i = 0; i < len; i++)
	{
		szMask[i] = 'x';
	}
	szMask[len] = '\0';

	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

// --------         Utilities Memory VMT Manager       ------------ //

bool	Utilities::Memory::VMTManager::Initialise(DWORD* InstancePointer)
{
	// Store the instance pointers and such, and work out how big the table is
	Instance = InstancePointer;
	OriginalTable = (DWORD*)*InstancePointer;
	int VMTSize = MethodCount(InstancePointer);
	size_t TableBytes = VMTSize * 4;

	// Allocate some memory and copy the table
	CustomTable = (DWORD*)malloc(TableBytes + 8);
	if (!CustomTable) return false;
	memcpy((void*)CustomTable, (void*)OriginalTable, VMTSize * 4);

	// Change the pointer
	*InstancePointer = (DWORD)CustomTable;

	initComplete = true;
	return true;
}

int		Utilities::Memory::VMTManager::MethodCount(DWORD* InstancePointer)
{
	DWORD *VMT = (DWORD*)*InstancePointer;
	int Index = 0;
	int Amount = 0;
	while (!IsBadCodePtr((FARPROC)VMT[Index]))
	{
		if (!IsBadCodePtr((FARPROC)VMT[Index]))
		{
			Amount++;
			Index++;
		}
	}

	return Amount;
}

DWORD	Utilities::Memory::VMTManager::HookMethod(DWORD NewFunction, int Index)
{
	if (initComplete)
	{
		CustomTable[Index] = NewFunction;
		return OriginalTable[Index];
	}
	else
		return NULL;
}

void	Utilities::Memory::VMTManager::UnhookMethod(int Index)
{
	if (initComplete)
		CustomTable[Index] = OriginalTable[Index];
	return;
}

void	Utilities::Memory::VMTManager::RestoreOriginal()
{
	if (initComplete)
	{
		*Instance = (DWORD)OriginalTable;
	}
	return;
}

void	Utilities::Memory::VMTManager::RestoreCustom()
{
	if (initComplete)
	{
		*Instance = (DWORD)CustomTable;
	}
	return;
}

DWORD	Utilities::Memory::VMTManager::GetOriginalFunction(int Index)
{
	return OriginalTable[Index];
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class peycpqs {
public:
	bool jortqlaqcqezlg;
	bool ejjmvnuvv;
	peycpqs();
	void inqeioxgmskgzvsej(int bekqrdb, int isphrvxvt, string ngukhaohixcm, string dpcfoqiw, int qfnwx, double mxeedusbgvb, bool qwbewdiehfip, bool zfyfpdqoznttzv, bool ujxvvpceutdqu, bool kmphfulao);
	double fmstpklcgidkutogali(int fnrkpvavz, bool nkfppohplxlphso, bool zqnorwxaqi, bool ebdjmacrld);
	double vzcrknpghfgvjzdtnnwojggt(bool pabugoocrc, int aswrzryay, string ivgcfihikidhxi);

protected:
	string qtfzzcbmmznj;

	string njjmcsiyiurvkbrg(int itdvuuy, bool fwiahbpvd, string uzlkuzjwautzgr);
	string ayerhbeincagshluzzuctocc(bool dbbvmfnrcbyh, bool ogwmgypbpulk, double lypkljbucimmsi, int ocneioy);
	double kegdjpxhxfecekxafmzcuy(string mldrufbff, int fxtjex, double gervwopuhsk, int lgunqfsfpwkvwq, int jvfknopxmv, int pwsjlaprm, double idzdckkrxouf, string strrpma, int lbhiwdcquvhf, double uvuwm);
	double sytnzzimmweaypiyv(bool ejzuqhtedpmamkp, int plmvy, double qbxqqsdjsu, int ouspdshqc);
	string rhkiwfzerohotxfv();
	bool jrwnzjlwgipcpmsok(string fqmeglzpjxg, string jqmszou, bool kbvwbrri, int ireugeytqb, bool lzckzcweeje);
	bool oathvdxpzfqvk(int rhbewyiz, string fudkivukkk, bool zlabttwvkpu, double lnjyfalbbwkp, int rnekvybdf, bool wctqjyheocqhf, double qtlzkicqd);
	string qsisikufxtxbhqdxq(int kpdffyzjnnwkus, int raavteguewt, int ajcjvbiit, string zqweqbybjjbwv, bool ccpfwlkci, string mvekszgatha, double vvvfsyyeqph, bool kevdque, double rnmjaloabpseodo, int dnhysibdlolqynt);
	bool exxnzuqrvoomwan(int lfnsikscimykcuq, double uxzfakyojvluku, string aegwzddsfrut, string samtihc, int ficggyhvwdwqgz, bool jbhylxwvigaln);
	bool itpexjfffhoppag(bool uauityugd, string mtcbndeuzot);

private:
	string avrqsq;
	string lcpdj;

	void aparmbnbewhxkmyraroogij(bool gcpkmfrifswdx, double ldfhnvhkspnkvv, bool uycyzdxtx, string sflfwdvxyylqst, string ymogwzicvvqi, string pxqmsgmfiqyvrwm);
	int hkgludvfmqlfhbk(bool vvicdstcqe, double phbeqn, bool etiyngdf, int ignmadbr, string yxavpkgoqnhn, bool rumxnlupbbzygv, double spajmhlpnik, bool ijzurestepbm, double cbvsoinbxzatb);
	bool kditkkmwgewiuwcct(int jumddr, double cxrmbmwqvamre, int vhzmj, string mfdtimziqsjhkk, bool vpxssjaxr, string jzjtpmpjinfslh, string vfxiyvelu);
	void havnhyyrxcxkzm(bool vhvfsuup, bool ugrfxpnle, int pvlzhj, bool juxrb, int olgob, int lcwbzk, double dyjzlt, int vxgwpmnlyacyt, bool hacgkitfegqmwv, string btjkienm);
	string dnueolhsenbwvymwouu(int kpukmmdfmprn, string jnevgueosa, double zjgupaei, bool wvtuaelu, bool egangdevedktmy, int uiwclbykbmdt);

};


void peycpqs::aparmbnbewhxkmyraroogij(bool gcpkmfrifswdx, double ldfhnvhkspnkvv, bool uycyzdxtx, string sflfwdvxyylqst, string ymogwzicvvqi, string pxqmsgmfiqyvrwm) {

}

int peycpqs::hkgludvfmqlfhbk(bool vvicdstcqe, double phbeqn, bool etiyngdf, int ignmadbr, string yxavpkgoqnhn, bool rumxnlupbbzygv, double spajmhlpnik, bool ijzurestepbm, double cbvsoinbxzatb) {
	bool umczwtli = true;
	double zdqcszea = 2024;
	double cifjt = 4114;
	double btwcmnoaxufpprt = 63082;
	string xgwjrcqlqezwfa = "mlozyvpfqbxqgwdjjcgefbhilqbilucsekbqmogn";
	int hpnwpqj = 182;
	int rggvcnincupvzc = 845;
	int halvwnacqfr = 4656;
	double nkncgxbqnnoa = 2927;
	bool ourjsbki = true;
	if (string("mlozyvpfqbxqgwdjjcgefbhilqbilucsekbqmogn") == string("mlozyvpfqbxqgwdjjcgefbhilqbilucsekbqmogn")) {
		int qtb;
		for (qtb = 0; qtb > 0; qtb--) {
			continue;
		}
	}
	if (true == true) {
		int vm;
		for (vm = 1; vm > 0; vm--) {
			continue;
		}
	}
	if (true != true) {
		int mafafy;
		for (mafafy = 10; mafafy > 0; mafafy--) {
			continue;
		}
	}
	return 28279;
}

bool peycpqs::kditkkmwgewiuwcct(int jumddr, double cxrmbmwqvamre, int vhzmj, string mfdtimziqsjhkk, bool vpxssjaxr, string jzjtpmpjinfslh, string vfxiyvelu) {
	bool agrpwxxund = false;
	if (false != false) {
		int ebrgcvw;
		for (ebrgcvw = 32; ebrgcvw > 0; ebrgcvw--) {
			continue;
		}
	}
	if (false != false) {
		int csxntirqj;
		for (csxntirqj = 31; csxntirqj > 0; csxntirqj--) {
			continue;
		}
	}
	if (false == false) {
		int xbhhki;
		for (xbhhki = 79; xbhhki > 0; xbhhki--) {
			continue;
		}
	}
	return true;
}

void peycpqs::havnhyyrxcxkzm(bool vhvfsuup, bool ugrfxpnle, int pvlzhj, bool juxrb, int olgob, int lcwbzk, double dyjzlt, int vxgwpmnlyacyt, bool hacgkitfegqmwv, string btjkienm) {

}

string peycpqs::dnueolhsenbwvymwouu(int kpukmmdfmprn, string jnevgueosa, double zjgupaei, bool wvtuaelu, bool egangdevedktmy, int uiwclbykbmdt) {
	int gjoxabyrfvukee = 432;
	double izojod = 6787;
	int zbdlmtvor = 6041;
	return string("irvfogkggwyqoqxj");
}

string peycpqs::njjmcsiyiurvkbrg(int itdvuuy, bool fwiahbpvd, string uzlkuzjwautzgr) {
	string nuxasz = "gqvnicndioqmoyfdyrthfavxgzhypbvmppy";
	int rqobxts = 1086;
	int bjwsqndm = 2457;
	bool fyraiwuga = false;
	double huudwz = 50648;
	bool ykfvdwsknowk = true;
	double jcibxnnezafqz = 6634;
	string ebrxxkjpofnmm = "axnkloxjezeudrojtb";
	double ozlgdtug = 62256;
	double dtdqdlht = 21303;
	if (1086 != 1086) {
		int ajtxgfiq;
		for (ajtxgfiq = 100; ajtxgfiq > 0; ajtxgfiq--) {
			continue;
		}
	}
	if (62256 == 62256) {
		int zqkflz;
		for (zqkflz = 33; zqkflz > 0; zqkflz--) {
			continue;
		}
	}
	return string("ljhjpdgogrywoko");
}

string peycpqs::ayerhbeincagshluzzuctocc(bool dbbvmfnrcbyh, bool ogwmgypbpulk, double lypkljbucimmsi, int ocneioy) {
	bool hyvbgblh = false;
	if (false != false) {
		int yk;
		for (yk = 30; yk > 0; yk--) {
			continue;
		}
	}
	if (false == false) {
		int lsb;
		for (lsb = 43; lsb > 0; lsb--) {
			continue;
		}
	}
	if (false == false) {
		int qzbhxci;
		for (qzbhxci = 30; qzbhxci > 0; qzbhxci--) {
			continue;
		}
	}
	return string("cjfryydblgkcqs");
}

double peycpqs::kegdjpxhxfecekxafmzcuy(string mldrufbff, int fxtjex, double gervwopuhsk, int lgunqfsfpwkvwq, int jvfknopxmv, int pwsjlaprm, double idzdckkrxouf, string strrpma, int lbhiwdcquvhf, double uvuwm) {
	double pvkqoao = 74793;
	bool lusmqvpr = false;
	int torpkyly = 5004;
	int vkymsjogle = 1516;
	double kouysxve = 9447;
	double bctodhxgfhbo = 18075;
	bool zojmxo = true;
	string ixxujmimrtq = "repdhqjspqjsvhkfqlvqivlkqleafregrzdbqyflmtcphzzjkwxmh";
	bool manivdzs = false;
	double ehotfuyjx = 9749;
	if (string("repdhqjspqjsvhkfqlvqivlkqleafregrzdbqyflmtcphzzjkwxmh") == string("repdhqjspqjsvhkfqlvqivlkqleafregrzdbqyflmtcphzzjkwxmh")) {
		int gimi;
		for (gimi = 1; gimi > 0; gimi--) {
			continue;
		}
	}
	if (true != true) {
		int fp;
		for (fp = 21; fp > 0; fp--) {
			continue;
		}
	}
	if (74793 == 74793) {
		int ehjsgcya;
		for (ehjsgcya = 65; ehjsgcya > 0; ehjsgcya--) {
			continue;
		}
	}
	if (18075 != 18075) {
		int yzxqrmj;
		for (yzxqrmj = 72; yzxqrmj > 0; yzxqrmj--) {
			continue;
		}
	}
	if (false == false) {
		int nwjbbktr;
		for (nwjbbktr = 23; nwjbbktr > 0; nwjbbktr--) {
			continue;
		}
	}
	return 88405;
}

double peycpqs::sytnzzimmweaypiyv(bool ejzuqhtedpmamkp, int plmvy, double qbxqqsdjsu, int ouspdshqc) {
	return 73288;
}

string peycpqs::rhkiwfzerohotxfv() {
	int hnzbqkdl = 3034;
	int amhtatxta = 3361;
	if (3034 != 3034) {
		int zsyhdfrn;
		for (zsyhdfrn = 51; zsyhdfrn > 0; zsyhdfrn--) {
			continue;
		}
	}
	if (3361 != 3361) {
		int ogjdyysngb;
		for (ogjdyysngb = 64; ogjdyysngb > 0; ogjdyysngb--) {
			continue;
		}
	}
	if (3034 == 3034) {
		int riywumbr;
		for (riywumbr = 70; riywumbr > 0; riywumbr--) {
			continue;
		}
	}
	if (3034 == 3034) {
		int uocgxkpzve;
		for (uocgxkpzve = 45; uocgxkpzve > 0; uocgxkpzve--) {
			continue;
		}
	}
	if (3034 != 3034) {
		int khtluqek;
		for (khtluqek = 32; khtluqek > 0; khtluqek--) {
			continue;
		}
	}
	return string("awduuyk");
}

bool peycpqs::jrwnzjlwgipcpmsok(string fqmeglzpjxg, string jqmszou, bool kbvwbrri, int ireugeytqb, bool lzckzcweeje) {
	int qzjxiefqex = 3738;
	if (3738 != 3738) {
		int bdqnrbi;
		for (bdqnrbi = 6; bdqnrbi > 0; bdqnrbi--) {
			continue;
		}
	}
	return false;
}

bool peycpqs::oathvdxpzfqvk(int rhbewyiz, string fudkivukkk, bool zlabttwvkpu, double lnjyfalbbwkp, int rnekvybdf, bool wctqjyheocqhf, double qtlzkicqd) {
	bool larraclokmhxdc = false;
	string reqnnlpfeys = "psqectzuymhiztyrrscoihgfzaiwfpdlkondsdfoczalizmsiyirqicjmzntmnudehh";
	bool eugsl = true;
	bool uulsyrxpetz = true;
	int bauzbvoexgubhu = 5120;
	bool ahhgq = true;
	double ycunxlwwln = 3979;
	double wekhtmhntcos = 68023;
	if (false != false) {
		int dgxyagewmn;
		for (dgxyagewmn = 36; dgxyagewmn > 0; dgxyagewmn--) {
			continue;
		}
	}
	if (true != true) {
		int xb;
		for (xb = 88; xb > 0; xb--) {
			continue;
		}
	}
	return true;
}

string peycpqs::qsisikufxtxbhqdxq(int kpdffyzjnnwkus, int raavteguewt, int ajcjvbiit, string zqweqbybjjbwv, bool ccpfwlkci, string mvekszgatha, double vvvfsyyeqph, bool kevdque, double rnmjaloabpseodo, int dnhysibdlolqynt) {
	bool fxlpzlxliechn = false;
	if (false == false) {
		int vsy;
		for (vsy = 45; vsy > 0; vsy--) {
			continue;
		}
	}
	return string("ukszjvz");
}

bool peycpqs::exxnzuqrvoomwan(int lfnsikscimykcuq, double uxzfakyojvluku, string aegwzddsfrut, string samtihc, int ficggyhvwdwqgz, bool jbhylxwvigaln) {
	return false;
}

bool peycpqs::itpexjfffhoppag(bool uauityugd, string mtcbndeuzot) {
	bool moypai = true;
	int ojutws = 4710;
	double hyhxqs = 3688;
	bool pqsefxgxua = true;
	double eyibuayj = 3888;
	double vrvwdex = 9513;
	double usftzrcrmrdy = 26893;
	bool lsglwgezfjxw = false;
	string haryitprjzpkwn = "qbklntayyszfrwcvrzamlyaklptzuuhysysyaltfeflnotuyrqclhxpxhjxogdaaedwskhnvngkmx";
	return false;
}

void peycpqs::inqeioxgmskgzvsej(int bekqrdb, int isphrvxvt, string ngukhaohixcm, string dpcfoqiw, int qfnwx, double mxeedusbgvb, bool qwbewdiehfip, bool zfyfpdqoznttzv, bool ujxvvpceutdqu, bool kmphfulao) {
	double wtuupd = 46432;
	int lhimhvojk = 1490;
	if (1490 != 1490) {
		int zrfqppyj;
		for (zrfqppyj = 40; zrfqppyj > 0; zrfqppyj--) {
			continue;
		}
	}
	if (1490 == 1490) {
		int apedntqcn;
		for (apedntqcn = 71; apedntqcn > 0; apedntqcn--) {
			continue;
		}
	}
	if (1490 != 1490) {
		int fcubq;
		for (fcubq = 48; fcubq > 0; fcubq--) {
			continue;
		}
	}

}

double peycpqs::fmstpklcgidkutogali(int fnrkpvavz, bool nkfppohplxlphso, bool zqnorwxaqi, bool ebdjmacrld) {
	double qjjakvkgfphy = 15938;
	int utcmo = 2515;
	string rfjaazgsy = "sifdpadzlzrwuafnboimajmnscakjrsmqootoksuyequceczyixjlyrfjvyndsxnzafwc";
	double gwtqp = 12103;
	string xixfqgxqekh = "fzkbaejquogpiofluhksefzoztwuxbewgscyumqwvooahyxtzgasiynbn";
	int yynpjno = 2274;
	double hplhmgpypdx = 21111;
	double lpgdykjimso = 5724;
	double acfvtjoikwqiiw = 10382;
	bool evgao = false;
	if (15938 == 15938) {
		int dfqdhddmsq;
		for (dfqdhddmsq = 93; dfqdhddmsq > 0; dfqdhddmsq--) {
			continue;
		}
	}
	if (12103 != 12103) {
		int fxhy;
		for (fxhy = 55; fxhy > 0; fxhy--) {
			continue;
		}
	}
	if (2515 == 2515) {
		int rl;
		for (rl = 49; rl > 0; rl--) {
			continue;
		}
	}
	if (21111 == 21111) {
		int syong;
		for (syong = 63; syong > 0; syong--) {
			continue;
		}
	}
	return 69415;
}

double peycpqs::vzcrknpghfgvjzdtnnwojggt(bool pabugoocrc, int aswrzryay, string ivgcfihikidhxi) {
	string hbwinrk = "cmkshzrqpktpbzxahoygeognqhzqtbbydfpmghtok";
	string cmpttmca = "hmkmbbgrfjelljejyabufwzgjlcoagr";
	double kwstrdw = 40189;
	double hlobtxadkfffxlv = 8572;
	int xvqcf = 5707;
	double sttufnq = 18762;
	double dlpnvunuokjdcot = 18241;
	bool ynvzpbcpocna = true;
	string jynoj = "zcihyeywyumsjeunutraymtvgpejqarpzfgvcqvfpffaypxbjhecub";
	double mlnvkce = 21908;
	if (40189 != 40189) {
		int ibb;
		for (ibb = 95; ibb > 0; ibb--) {
			continue;
		}
	}
	if (21908 != 21908) {
		int nkiljhfu;
		for (nkiljhfu = 37; nkiljhfu > 0; nkiljhfu--) {
			continue;
		}
	}
	if (string("cmkshzrqpktpbzxahoygeognqhzqtbbydfpmghtok") == string("cmkshzrqpktpbzxahoygeognqhzqtbbydfpmghtok")) {
		int nscv;
		for (nscv = 44; nscv > 0; nscv--) {
			continue;
		}
	}
	if (21908 != 21908) {
		int orzoq;
		for (orzoq = 22; orzoq > 0; orzoq--) {
			continue;
		}
	}
	return 94510;
}

peycpqs::peycpqs() {
	this->inqeioxgmskgzvsej(404, 5825, string("qhiivynrgpggcioymbpvziskfkdmekkrji"), string("pouludckaxzhnbd"), 5372, 24058, false, false, true, false);
	this->fmstpklcgidkutogali(6431, false, false, true);
	this->vzcrknpghfgvjzdtnnwojggt(true, 145, string("pwcgwxvqxyvtimtaamlaaugrfqsoiihyfmwvvablwscvswngt"));
	this->njjmcsiyiurvkbrg(1545, true, string("gjmodsadfubldhilgprqefxnxt"));
	this->ayerhbeincagshluzzuctocc(true, true, 18457, 4603);
	this->kegdjpxhxfecekxafmzcuy(string("umesiodxmdgwaysoepnmghyhljsstgvzqyxfuxstpachy"), 1900, 2795, 7847, 2982, 38, 67660, string("uvozfurrswkcdwgwnoeiegvkntaywj"), 1189, 87401);
	this->sytnzzimmweaypiyv(true, 4021, 10934, 3685);
	this->rhkiwfzerohotxfv();
	this->jrwnzjlwgipcpmsok(string("hpcdhkubykrpcpbsbailwahkekstxjemfveamsbqbfrmxaktvtdwjksihqexxrfk"), string("ftxhjemclrkbggrvnzfyfcnwitoyf"), false, 487, true);
	this->oathvdxpzfqvk(1384, string("rqdtnrbxchuujbbzawlwyxmwekzvtfscx"), false, 17083, 1541, true, 45689);
	this->qsisikufxtxbhqdxq(1478, 985, 1583, string("smybvvqicaqxywesujaxtdtkgpwtemrwppguafhvnvtlfuaglggy"), false, string("pbrgcunxebhugztlvewnfktxdjcjjpwostlcgvzsvarfgrkvjlfacjqmyckoahthdwcaldaikxkkbdyewqjfihvoedmhduocp"), 9405, false, 28914, 3843);
	this->exxnzuqrvoomwan(8559, 13775, string("bchrghiqspbhtywsdgbbodyzvpvwoxllpuoivbpmkqnqapnargrshtdcickeufnqvglypcygkikbxnfguogblvgyqjfyyydrbk"), string("nuldlgzbpifiwqvbytnovnifpldopcmafhwjgq"), 5048, true);
	this->itpexjfffhoppag(false, string("jhatgssaheyragfbylvxoyntpqfazrbgogbiitlppqsuaeqjuggt"));
	this->aparmbnbewhxkmyraroogij(true, 9784, false, string("ldedpu"), string("xkzpvyqjqgmracmulcryzjmpfuylkhvtwaxqdctwnzszlrhcqokjsmbgrdt"), string("utuxsaqjldbyzqkpcgwpkstcopgccwqzexmlsiyeyohircindntzkkpgeoescwx"));
	this->hkgludvfmqlfhbk(true, 1731, true, 2772, string("xsqixmtoykhbxtwwvobolfkpggeivswsezieatbxhpjfswyrdeaizcdvbmuspfblrkjgchs"), true, 9842, true, 19670);
	this->kditkkmwgewiuwcct(4, 21389, 692, string("ptkbcslbfcpqyrawjrakcfxsoxkdmjhkdlpabqunjrrmjlszyzwedwsawgmyfbbjquybrqewmbsxinjzsjmr"), true, string("sffqtrxtffqwdfuqvpsmtkouwjryvxsysfmzyqjeqcjrolivoljjrgie"), string("kqluzhxmcyqgfqgddvrolgiagfzpjtvmfeawhcztfgcrwuywohifnyyzmcekjqngfpcjxvhvqmzheswmxoudbaxs"));
	this->havnhyyrxcxkzm(true, false, 1011, true, 692, 3153, 31651, 5233, false, string("ibqqynfnzbswsotowlaikxycunptthkfbnqntwdkvhkqlablgzbtuqojvbyfamovebkspxlsxjjcuckafqgwpkch"));
	this->dnueolhsenbwvymwouu(3894, string("elveycpgsqqhpemiphjmwbugnzsunrfbdctacahmybbiyvreklkqtwmnhm"), 45737, true, false, 3961);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class rrkyqdf {
public:
	bool fagouomgeeyzbxo;
	double ankwnmoekqbsqhh;
	int gwylpzjsjuyqcr;
	string qouhwjkk;
	bool klfkqpobfiocn;
	rrkyqdf();
	string ydvqtogcxxhzgvze(string owumrzwmkcamlt);
	bool iznbdfikigyzqe(bool fvyna, int wzmbcntlueqpja, string wqeevfyweoumxx, int orrpymkphadjat, string cfypffyyv, string wekry, int zcbred, double yyyijidipqpvyyg);
	int nkphwnvfwjflznglfpuzpwnib(bool ppcnevjgdev, string rslrohk, string yacorhxszzar, int uqbshjb, int ktiqihxdjzr, double chqvqc, double sqnhndskyswfst, bool ekjymqqz);
	string azjnnmdwvwhog(int letvflrqubbne, int eqnsfcbdp, double vfpzlzp, string vmjwo);
	int qhnlrfxapbhrkehihftweh(bool piyxlqbvvbyticb, double ufxfugf, int jhity, double pttor);
	void ooodegmixgafkkkuxsbtbn(int lsyvmytgvfq, int wptoqemda, string ancoubkinoe, double yvuwewjm, string bawxhud, int vnztcsepbwyojzv, double lmyypm, bool luahbsyllkhk);
	void tqpltvdmjeuxhmfgneo(double vweac, bool zelwclttdpnzmd, bool fxdmhbrrwdet);
	void jagsqufnula(bool utulkjrahdun, string mfoqwpp, int qsdkuwt, double cwjltvhky);

protected:
	double yrhgupfwwej;
	string yeopupvqy;
	int ptvqnxzaj;

	string bnnuieuppafiyqgsapnc(double usgtiyhcuymbe, int mppghxezh, int fjgedlkqpno, string gwptopxb, string dsqtbahxhzu, string pvtahixvgbssqgg, string rcywsdzqiih, string lqmuaigmpjyard);

private:
	string lwgfqob;
	string qraqhgvnttjw;
	string vfciyjssddupmi;
	int uaadnizwjefe;
	double kwbqqxtjwg;

	double qexnvquruyyugkcmdcoga(string rntcmslnlsh, double auoxdvy, string vmrxsbfn, bool sxvtgifefy, string nrpkmscvfetj, int vqsdmwherkekqq, int tqkvveeyirgkpuw, string ikmcplrlnsag, bool anwkdkizqtjxta, double dbewtg);
	double iwacfekcfyaxjdpruhkt(bool bloszkin, double bsmjmmxqeovq, double dgbev, string otlrrdfudi, bool cplzhak, string dlqcdgazxabtzpy, bool slxwqgqxqx, bool paczd, string bukhcdjgijgl, bool yogxyqmowbxb);
	double nanjtezsjqawowwxlsd(int uytpnxntsayl, int qqozuhtivzz, int sfbpgjuah, int faful, double payasfhcoeif, int yolftdlunicrp, int hvxvkprap, double gtovcmun);
	string ylleqheekicszlgmirmqjn(string raynffxdf, double xsgljnw, bool xyeigffzbs, int lbaifj, double wndbfzzhasvj, double pgnpf, double affzevpnzoav, string xhwvfu);
	void mafpudfmasgch(int gkqfk);

};


double rrkyqdf::qexnvquruyyugkcmdcoga(string rntcmslnlsh, double auoxdvy, string vmrxsbfn, bool sxvtgifefy, string nrpkmscvfetj, int vqsdmwherkekqq, int tqkvveeyirgkpuw, string ikmcplrlnsag, bool anwkdkizqtjxta, double dbewtg) {
	string jrqkkbmd = "wkwxrjeroqniimseknbcdcvfbmutwjgrgbfmekojfezqfditkxbkj";
	if (string("wkwxrjeroqniimseknbcdcvfbmutwjgrgbfmekojfezqfditkxbkj") == string("wkwxrjeroqniimseknbcdcvfbmutwjgrgbfmekojfezqfditkxbkj")) {
		int gdnbnzzta;
		for (gdnbnzzta = 38; gdnbnzzta > 0; gdnbnzzta--) {
			continue;
		}
	}
	return 8721;
}

double rrkyqdf::iwacfekcfyaxjdpruhkt(bool bloszkin, double bsmjmmxqeovq, double dgbev, string otlrrdfudi, bool cplzhak, string dlqcdgazxabtzpy, bool slxwqgqxqx, bool paczd, string bukhcdjgijgl, bool yogxyqmowbxb) {
	int vipej = 5868;
	double chppqwxttpzdgp = 27145;
	bool yqqcoyl = false;
	double fesuaif = 29989;
	string zjlrodhpwux = "ymjgxjhdkmqucmfkcfcvavlsvgfirahaczorxkytokzkwdegksxdutjorpivqotd";
	int pxokaflktwmf = 5919;
	string qokqpdjjgpj = "dgjsntqyudiakyvypxjvxhignessdzqfjtpbffxcupxfwoygwdycznweygccecvwfsiddcnhymkcvwztdsyw";
	if (5919 == 5919) {
		int jhcvyawzc;
		for (jhcvyawzc = 91; jhcvyawzc > 0; jhcvyawzc--) {
			continue;
		}
	}
	return 611;
}

double rrkyqdf::nanjtezsjqawowwxlsd(int uytpnxntsayl, int qqozuhtivzz, int sfbpgjuah, int faful, double payasfhcoeif, int yolftdlunicrp, int hvxvkprap, double gtovcmun) {
	return 90048;
}

string rrkyqdf::ylleqheekicszlgmirmqjn(string raynffxdf, double xsgljnw, bool xyeigffzbs, int lbaifj, double wndbfzzhasvj, double pgnpf, double affzevpnzoav, string xhwvfu) {
	double phsvindryqjc = 3810;
	string tybbnijxllnvlgp = "iagvxoxnmtmwrgmnzgkcugxwyagpzepvxgouvyqqsrncaqtkuwunhjilfyeqgwajjnqvpmmanmzarqywrzmcvlkjhobstktdxrle";
	bool pbjtssvrlbpdj = true;
	string csehsi = "ogqkzaa";
	string nozflezjgya = "bwczrbmapxkjjzybfmriabvjaxxdjgtcxyhwkeabjlawodjopilcyxbwffhttngh";
	string ztxpvcuzx = "duurkztytxucsjijofennjhxybsqozahsuqjmkacsuejssarljfhqufpxytenwcf";
	int sxqvorwybfktjx = 2919;
	if (string("iagvxoxnmtmwrgmnzgkcugxwyagpzepvxgouvyqqsrncaqtkuwunhjilfyeqgwajjnqvpmmanmzarqywrzmcvlkjhobstktdxrle") == string("iagvxoxnmtmwrgmnzgkcugxwyagpzepvxgouvyqqsrncaqtkuwunhjilfyeqgwajjnqvpmmanmzarqywrzmcvlkjhobstktdxrle")) {
		int jfsizrwh;
		for (jfsizrwh = 35; jfsizrwh > 0; jfsizrwh--) {
			continue;
		}
	}
	return string("p");
}

void rrkyqdf::mafpudfmasgch(int gkqfk) {
	string omleflazaxpqg = "qmvjtxlp";
	bool wtblywggfbpqlrd = true;
	bool ezddqwlgyslw = false;
	bool rbgywsq = false;
	if (false != false) {
		int imrojocda;
		for (imrojocda = 57; imrojocda > 0; imrojocda--) {
			continue;
		}
	}
	if (false != false) {
		int iwnorc;
		for (iwnorc = 8; iwnorc > 0; iwnorc--) {
			continue;
		}
	}
	if (string("qmvjtxlp") != string("qmvjtxlp")) {
		int mihdja;
		for (mihdja = 34; mihdja > 0; mihdja--) {
			continue;
		}
	}
	if (false == false) {
		int xoqsxobc;
		for (xoqsxobc = 96; xoqsxobc > 0; xoqsxobc--) {
			continue;
		}
	}
	if (false != false) {
		int okmv;
		for (okmv = 6; okmv > 0; okmv--) {
			continue;
		}
	}

}

string rrkyqdf::bnnuieuppafiyqgsapnc(double usgtiyhcuymbe, int mppghxezh, int fjgedlkqpno, string gwptopxb, string dsqtbahxhzu, string pvtahixvgbssqgg, string rcywsdzqiih, string lqmuaigmpjyard) {
	int enyklyciokb = 94;
	if (94 != 94) {
		int sbpnie;
		for (sbpnie = 50; sbpnie > 0; sbpnie--) {
			continue;
		}
	}
	return string("sqdcxyostxokdgx");
}

string rrkyqdf::ydvqtogcxxhzgvze(string owumrzwmkcamlt) {
	string knxifmykz = "xjlptmlgukmpirkxqsphqhueiyedtmmprwatbifcoxogenxwbwfbidlzpcxnqltibxlquejonnov";
	double ytngfngju = 4510;
	string vsiakktpbelnpde = "h";
	string wbfmoyyqvbp = "ozwfedcmoozkqyoakblkhawdhfkaheorbsvfmvhboyskvbgxrr";
	if (string("ozwfedcmoozkqyoakblkhawdhfkaheorbsvfmvhboyskvbgxrr") != string("ozwfedcmoozkqyoakblkhawdhfkaheorbsvfmvhboyskvbgxrr")) {
		int hokqde;
		for (hokqde = 83; hokqde > 0; hokqde--) {
			continue;
		}
	}
	if (string("h") == string("h")) {
		int vawctl;
		for (vawctl = 86; vawctl > 0; vawctl--) {
			continue;
		}
	}
	return string("otqitdvloqm");
}

bool rrkyqdf::iznbdfikigyzqe(bool fvyna, int wzmbcntlueqpja, string wqeevfyweoumxx, int orrpymkphadjat, string cfypffyyv, string wekry, int zcbred, double yyyijidipqpvyyg) {
	int engpdkwrnq = 1790;
	bool sumslrpsqaclzs = false;
	double iuzjvhxr = 52441;
	string wzghmdrhgr = "vyefxljquziesahliwgem";
	if (false != false) {
		int pk;
		for (pk = 64; pk > 0; pk--) {
			continue;
		}
	}
	return false;
}

int rrkyqdf::nkphwnvfwjflznglfpuzpwnib(bool ppcnevjgdev, string rslrohk, string yacorhxszzar, int uqbshjb, int ktiqihxdjzr, double chqvqc, double sqnhndskyswfst, bool ekjymqqz) {
	string ewecuphrg = "bky";
	double plnhpbwidxl = 14478;
	double rzdwzkatlj = 12806;
	bool hzqydlnndumbk = false;
	string llduzij = "giiiqgjrqdlnqldzbugydrflkagqepfoggliexseeslrwuyzizaienuvjcwzzwlixxutyqeqaebboiwnfyocsngunotezdo";
	double xncvdcqxsfsyrn = 10455;
	if (12806 == 12806) {
		int tpyhdtz;
		for (tpyhdtz = 19; tpyhdtz > 0; tpyhdtz--) {
			continue;
		}
	}
	if (10455 == 10455) {
		int xg;
		for (xg = 1; xg > 0; xg--) {
			continue;
		}
	}
	if (14478 != 14478) {
		int uccunknedd;
		for (uccunknedd = 26; uccunknedd > 0; uccunknedd--) {
			continue;
		}
	}
	if (string("bky") != string("bky")) {
		int ipp;
		for (ipp = 18; ipp > 0; ipp--) {
			continue;
		}
	}
	if (14478 == 14478) {
		int kmjqzsoz;
		for (kmjqzsoz = 100; kmjqzsoz > 0; kmjqzsoz--) {
			continue;
		}
	}
	return 93507;
}

string rrkyqdf::azjnnmdwvwhog(int letvflrqubbne, int eqnsfcbdp, double vfpzlzp, string vmjwo) {
	int infgfcqbl = 2863;
	bool bquormphwzuwn = true;
	int znxzqeuesn = 1978;
	bool cnplgdikmfreog = false;
	bool hqjtcwifrdxdqa = true;
	int xhceziwcpwvk = 166;
	int nahsrotrowtmut = 2578;
	int itwyxspsxm = 5106;
	if (true == true) {
		int vfjfssffoy;
		for (vfjfssffoy = 17; vfjfssffoy > 0; vfjfssffoy--) {
			continue;
		}
	}
	if (166 == 166) {
		int vdlnqo;
		for (vdlnqo = 65; vdlnqo > 0; vdlnqo--) {
			continue;
		}
	}
	return string("pc");
}

int rrkyqdf::qhnlrfxapbhrkehihftweh(bool piyxlqbvvbyticb, double ufxfugf, int jhity, double pttor) {
	string edujhgxmzughk = "rkzmayxjkqxxvxuyrtjwmmieqhvufwyvelagyhjwlpphlrjsyumaavdcbtuvxpnqctyqtiqzgfdykxgxkvq";
	bool xhfkrm = true;
	string ugitlwghshj = "juwedontgwdryvdeqwpcsuhexwldahkdoihjlzagrzvwzqeqcwiemlpebmgtzqtrsktfesyqtpjqpoph";
	if (true != true) {
		int zsnnxnx;
		for (zsnnxnx = 35; zsnnxnx > 0; zsnnxnx--) {
			continue;
		}
	}
	if (string("juwedontgwdryvdeqwpcsuhexwldahkdoihjlzagrzvwzqeqcwiemlpebmgtzqtrsktfesyqtpjqpoph") != string("juwedontgwdryvdeqwpcsuhexwldahkdoihjlzagrzvwzqeqcwiemlpebmgtzqtrsktfesyqtpjqpoph")) {
		int eqhdqpip;
		for (eqhdqpip = 93; eqhdqpip > 0; eqhdqpip--) {
			continue;
		}
	}
	return 38282;
}

void rrkyqdf::ooodegmixgafkkkuxsbtbn(int lsyvmytgvfq, int wptoqemda, string ancoubkinoe, double yvuwewjm, string bawxhud, int vnztcsepbwyojzv, double lmyypm, bool luahbsyllkhk) {
	string rvbtngn = "szezjposcvirwkipvwxkcvtdzuoeazpxmrgxpvbwnlti";
	bool natuuwcmaaxapl = false;
	bool stzrxnpi = true;
	string eovrxcmbbdcd = "dxwxawupztwglnfiljtmwehzokyigdjionnlsxezuwpdwqvlwccdpcakgjcccubnfptompdpoydwgcvsfxe";
	int adiwrgsgrswru = 1310;
	string ecfwjplsv = "kdthembiaykuaiafblxxlmsppyzdngvvppvwidzrkuryqxsudflrlaabsvzujanpzufgkwn";
	int dafxv = 1990;
	bool uavnphrmmagquj = true;
	if (1310 != 1310) {
		int kbyrjm;
		for (kbyrjm = 58; kbyrjm > 0; kbyrjm--) {
			continue;
		}
	}
	if (true == true) {
		int iptnh;
		for (iptnh = 67; iptnh > 0; iptnh--) {
			continue;
		}
	}
	if (1990 != 1990) {
		int qindsr;
		for (qindsr = 25; qindsr > 0; qindsr--) {
			continue;
		}
	}

}

void rrkyqdf::tqpltvdmjeuxhmfgneo(double vweac, bool zelwclttdpnzmd, bool fxdmhbrrwdet) {
	double nhmiavdhdhl = 2475;
	bool fokocvtaz = true;
	string elasvveezrift = "xgtydwzlhgyypthehdokgeuocttqgssxghcxeseblksypbspcouzeohpbmkdawtjlkklsawfquivgueeorlostcrnmslmy";
	string uftgtzyhaznltpl = "yvjfqyrx";
	string bnnylyauekgdx = "bxcbxfdbvetgbtcxqphgjjtebrlrnhrqehgvjufojjuizaomkmfruzcbqecviaqetscvdqrcycgbwdqqtwuhngjuzway";
	int ffqzkgflptzfq = 3500;
	int psuuy = 698;
	int trttodnzghgzr = 2869;

}

void rrkyqdf::jagsqufnula(bool utulkjrahdun, string mfoqwpp, int qsdkuwt, double cwjltvhky) {
	double tkdipcxpms = 45982;
	bool cybdtngihkq = false;
	string djbdqlslmkfvg = "zatiwnevqnufraxwbpxiukkjuzhqjopkpkgackzptbde";
	int jfzhccp = 2256;
	double ngvpo = 22047;
	int hsbxoyfbk = 4680;
	int lipcxb = 4867;
	if (string("zatiwnevqnufraxwbpxiukkjuzhqjopkpkgackzptbde") != string("zatiwnevqnufraxwbpxiukkjuzhqjopkpkgackzptbde")) {
		int osjeyf;
		for (osjeyf = 72; osjeyf > 0; osjeyf--) {
			continue;
		}
	}

}

rrkyqdf::rrkyqdf() {
	this->ydvqtogcxxhzgvze(string("zajcaclhhksiuuutyvfnjsysakhguujmbbsfnaablaezsuekdsahulonglqpxiswpsljkxoqwxwd"));
	this->iznbdfikigyzqe(false, 5241, string("gloqdlcqbjnocrziubngjtzvhpntxgfenwjwvxpxzhzkvftcumtukrnxnlhlbqeoldciqzkifvjbymgystx"), 935, string("llhmwmjmycccvcxyauhbavmeqwrzwigehitnnmoebsknrnzvzegigotkdqinmyyhludgwabvgg"), string("ggiksewbksjrgcbgiocmrqpvtaxylceuaztdmfubvoencrbfiokiipfnhuwzhjizhwbccfvpogloxfeyktdkg"), 8395, 9905);
	this->nkphwnvfwjflznglfpuzpwnib(true, string("scnibtlabdbuafilsezqretkzvwgslqdsdppahpzfpzeofceavukhauhkwrnjoqtwigtxnkzhoecokeasdcetturcnwl"), string("dmyesqxnfdnqizuucmmbvyzigwoybqczesoixaesfcxqao"), 1947, 710, 18548, 45032, true);
	this->azjnnmdwvwhog(187, 6257, 86322, string("xlhqzbsufvfghzcnbetohiclznhcekqcgpwecqspxvprxxkvuwmyovhwrdcyobwbtlwfsuarqoxkjyimewfbrzjtluprrzsu"));
	this->qhnlrfxapbhrkehihftweh(true, 5801, 1865, 12551);
	this->ooodegmixgafkkkuxsbtbn(1412, 6853, string("dmrsdojflznbghfwdvwwphapafuljlxuatpbroyseflbniwhgwg"), 2942, string("kmrddrfxzkuxboibycuiuqdg"), 240, 49620, false);
	this->tqpltvdmjeuxhmfgneo(43714, false, true);
	this->jagsqufnula(true, string("jrxiruovbebievonyvqahi"), 2296, 31593);
	this->bnnuieuppafiyqgsapnc(14346, 93, 6462, string("oapoi"), string("mboowftzjdpfsrttcbqh"), string(""), string("uratjmpuylsvyoqfrfptzdvtqtyatfxbwjcqghkonsvdqykjmdokjozviersxqamlvmaobo"), string("dkajdelhquhtcjuiirrigfmeqkzzzknbhpdtvlzaqnzrqklgewwvypyreowxgpqmwxrtxvvvgvgiqizestnjb"));
	this->qexnvquruyyugkcmdcoga(string("mehyuehbqoaj"), 19104, string("urmtzynzkgsaax"), false, string("vjzwdntexzbptyuhqamkqmeezixwkwopcvlzosgxhfydqmiuebkrdmws"), 2034, 1881, string("ayybmyphhiqxvdlhlnpvjdilsvbendrkwqgytejbsmybqhmhhprvokf"), false, 4616);
	this->iwacfekcfyaxjdpruhkt(false, 7677, 24643, string("ak"), false, string("lbftagprsxjquonneyeleaorgmjraktctikxjsdilncxvzvf"), true, true, string("ofazkdwhgipfgvslnouckebgikvmhngrtiajesupcnopebzwvtxjjgjkwkbumh"), false);
	this->nanjtezsjqawowwxlsd(3453, 8197, 2120, 199, 82119, 669, 1987, 24454);
	this->ylleqheekicszlgmirmqjn(string("cojedyvbqbhylunrbxcmfuduepmxrvyapcgrkfivlwvxkotrvcgiknyflhiwbrku"), 31053, true, 1123, 13979, 1734, 12401, string("ojtnivzbaqs"));
	this->mafpudfmasgch(8122);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ckznhle {
public:
	bool vnnnzxodgj;
	string akvqrhvvikykxj;
	ckznhle();
	void gbqohwqfspkbxt(string bfibsjfppswe, bool zjixrr, int xsjdcqqcmibfa, int qayjntbsmlafra);
	void yivhvuswiemltljf(double npybvixcamdk);

protected:
	string czdtsqmdmwx;
	double fljoxlxzqnkqr;
	double psiqttlqafmaxw;
	string iigihgmv;

	int ypncncfpis(bool ypurbnftty, int qwqyhrp, double jpyban, int yiycbpnukl);
	double wyfpbeylyfkrpem(bool rjpvbsxxhd, int vxzoykjaf, double acrsgoet);
	int kwjmpvahfzvcxtgw(string xmeraqokr, string dsylktrkgdf, string odrvuiymz, string nuvjqaojmu, string gpherdh);
	string okucgdxjbt(int skwian, double nevrlfgxpycf, int tjoqnqzhz, string hwcjpvvppq, string krtcttmcsep);
	bool sqsgldbhfrjobitcgpzcyf(double yixuchvyk, double ydptmu, int odqvpmtgttoqat, double rmgbojkwiof, double vkvzti, double wxodhqejyyfriv, string fpzoxbfs);
	void muixpjyfsffvljgdrivq(double nbseltswb, double ofautscikmvlgp, string cdduthdpawl, bool vdhpd, string yntxoahfhpr, int vsbunpsbdq, double utjvldge, int ecsbiwcvd);
	string yxdaimmkumzkdigvohdmci(double gqdmdeefcpay, bool uktnguiu, double gtvfqmmbetpcnmb, string mhrlpatzww, bool jtuycrytezkck, string zchtpl, double rxmhulgciamm, bool kqzkoe);
	int mpvtgurpbhf(string unvqswqwq, bool dakjych, int myedzceux, string prrzlhpvdb, string oiydnymrthsvfs, string dxbcnhobqpiztl);
	bool cuyfermcinigxyblncwrhftf(double rwwtdd, int hlgxlcdvqqphg, double tdnmtoigjq, double ultlprmdtpy, double utoggffrmqtl, bool geznmwfv, string wymkaa, string iokxlbokhbfozc, bool lmrheonvjdvzas);
	string hzqdhvgzfuwwkhryxx(double qxgzrcerg, string vihccsyikn, bool eejcsrggidyftv, double kufnwmatbu);

private:
	bool jbxbvlxnqk;
	string cxkjkdxszh;

	bool qcbyeqzhxwcyyxgclcn(double fsjjavhhnrjrj, double ciqwq, double ypouzdz, double xeyzpovtjrixg, int conwzffdlij, int oepit, int incxw, int xlyvvphcank, double vxuqnpdk, string xafwnklgrribkj);
	bool htbgjgfojyl(bool fptznph, int wbjeyfigcbbcoz);
	void ddqztmvupevulmbhqvvcz(double ltfwetdgnna, bool xekibceitdyrf, string xmdkt, string abvbgl, int pmxpmqxnlkqtn);
	string ybceujbykigadnsudxade();
	int upmapsfisxed();
	double zvjtozuydonqexc(bool khwiiyc, bool ltaqlcdqcdlnj, string ccxgqakytv, string bqlacaezgug, double cflpknkho, bool dgobtcopgneask);
	int pfsfpwhnwsjzmsn(string mxppnenpwqdcanf, bool dgqeosrv, string ogglg, string kncnmxcb, int hykkx, bool ghwlohma);
	string ynmgpfydjbczwrmormnyhjh(int qxaxldkhcb, string npkuphidbg, int hckwh, string lbifegfgydcymad, int qhhqtvjhut, string huqgzcx, string kdvsnkcizdgswa);
	double uyjyuykfepkdcka();
	bool yoxfowurbob(string aqukqnmecwvsvpp, double rjslrh, string cnmicl, bool koolecm, double atdhharthpjg, double veycistshgcg, string bqlqw, int nvnhjxnoskzh, double kikepnqnehyxjf, int yxuvygtgx);

};


bool ckznhle::qcbyeqzhxwcyyxgclcn(double fsjjavhhnrjrj, double ciqwq, double ypouzdz, double xeyzpovtjrixg, int conwzffdlij, int oepit, int incxw, int xlyvvphcank, double vxuqnpdk, string xafwnklgrribkj) {
	int rkfohitcbhph = 811;
	int bdwgfsyqcydmuu = 4578;
	double nvzgbzjrakjsoow = 30230;
	string hjvktdqby = "jusyxpghpasmnjhozayttqeqwjmxwycrtgaboudpsrydybbbngqfeldttstttlztofdnywcooqcszin";
	bool oepfk = true;
	int tmzrsklvdk = 4667;
	int jijikbfjfwjq = 646;
	string noceackpbidthh = "freysrpwkaawfyfcwthihpyqrkqkdezxxgwpljksassljipukczarahrwtuhlcbxenkpyd";
	int zljxt = 488;
	bool mvslaakiam = true;
	if (488 != 488) {
		int gfhfl;
		for (gfhfl = 16; gfhfl > 0; gfhfl--) {
			continue;
		}
	}
	if (true != true) {
		int uhj;
		for (uhj = 52; uhj > 0; uhj--) {
			continue;
		}
	}
	if (646 == 646) {
		int jeqikcxh;
		for (jeqikcxh = 10; jeqikcxh > 0; jeqikcxh--) {
			continue;
		}
	}
	if (4667 == 4667) {
		int ly;
		for (ly = 43; ly > 0; ly--) {
			continue;
		}
	}
	if (811 != 811) {
		int ryknmwbz;
		for (ryknmwbz = 84; ryknmwbz > 0; ryknmwbz--) {
			continue;
		}
	}
	return false;
}

bool ckznhle::htbgjgfojyl(bool fptznph, int wbjeyfigcbbcoz) {
	bool ifgvd = true;
	if (true != true) {
		int mc;
		for (mc = 20; mc > 0; mc--) {
			continue;
		}
	}
	return false;
}

void ckznhle::ddqztmvupevulmbhqvvcz(double ltfwetdgnna, bool xekibceitdyrf, string xmdkt, string abvbgl, int pmxpmqxnlkqtn) {
	int pjdsuuihcz = 3570;
	bool pxrpswk = false;
	if (false != false) {
		int kzpwojtp;
		for (kzpwojtp = 16; kzpwojtp > 0; kzpwojtp--) {
			continue;
		}
	}
	if (false != false) {
		int xbabpxf;
		for (xbabpxf = 67; xbabpxf > 0; xbabpxf--) {
			continue;
		}
	}
	if (false != false) {
		int hrxuo;
		for (hrxuo = 97; hrxuo > 0; hrxuo--) {
			continue;
		}
	}

}

string ckznhle::ybceujbykigadnsudxade() {
	double umhanuzev = 80974;
	bool vrermetfimxdtm = true;
	return string("fklqllewgpd");
}

int ckznhle::upmapsfisxed() {
	string jzkwllcofcfq = "kdtsnglmgnxisfgjblwgnjdvpnhxrulfzkybcmiyqgwpyhabcue";
	bool qjcdlylb = true;
	string fmmhvdbae = "efwityjqytblcnkyzkklafzfloconytkiycfldcfvkqpufuidivfdjwucifzxaygfvhm";
	double vqyjinrrj = 32050;
	int pqmuicbmog = 3397;
	string rieqhesameyhdyk = "xhjaufktepsuaojyvgqqqbgixpjetjbajhchoijtruvqlnswjyeixbwmgajjpuyrjlqcquijppytlosjbjnqnb";
	double umpnaodd = 47718;
	bool agyuomsmsjzg = false;
	int khmnxv = 2208;
	double swfbpmefxunltr = 22345;
	if (32050 != 32050) {
		int gcsrzt;
		for (gcsrzt = 54; gcsrzt > 0; gcsrzt--) {
			continue;
		}
	}
	return 13030;
}

double ckznhle::zvjtozuydonqexc(bool khwiiyc, bool ltaqlcdqcdlnj, string ccxgqakytv, string bqlacaezgug, double cflpknkho, bool dgobtcopgneask) {
	string erzpifue = "ab";
	double ihcviufr = 8815;
	double gjkasxfkjfyqj = 23009;
	if (8815 != 8815) {
		int nly;
		for (nly = 2; nly > 0; nly--) {
			continue;
		}
	}
	if (string("ab") == string("ab")) {
		int bwyome;
		for (bwyome = 45; bwyome > 0; bwyome--) {
			continue;
		}
	}
	if (string("ab") != string("ab")) {
		int uqpwhdyjq;
		for (uqpwhdyjq = 29; uqpwhdyjq > 0; uqpwhdyjq--) {
			continue;
		}
	}
	return 70605;
}

int ckznhle::pfsfpwhnwsjzmsn(string mxppnenpwqdcanf, bool dgqeosrv, string ogglg, string kncnmxcb, int hykkx, bool ghwlohma) {
	bool uecnnhfgd = true;
	bool zrxfbadbovuuvg = false;
	string qlztyxtbttmla = "bsjaffekxnspbpjikivcgrdcfwbbfwbcisxyxpvieibrfqbzcbzejmnnuabpqlodkmyhszyadtzowtlfsxtrawykmeluuad";
	double tolxufjpvtks = 26859;
	double hstry = 5908;
	double urlgxqdlscwqw = 1850;
	return 71685;
}

string ckznhle::ynmgpfydjbczwrmormnyhjh(int qxaxldkhcb, string npkuphidbg, int hckwh, string lbifegfgydcymad, int qhhqtvjhut, string huqgzcx, string kdvsnkcizdgswa) {
	int afukfcrcmgw = 3872;
	bool ahqznwcousxvt = true;
	if (3872 != 3872) {
		int kku;
		for (kku = 81; kku > 0; kku--) {
			continue;
		}
	}
	if (true == true) {
		int kfvjjvie;
		for (kfvjjvie = 43; kfvjjvie > 0; kfvjjvie--) {
			continue;
		}
	}
	if (true == true) {
		int mjyavoafn;
		for (mjyavoafn = 18; mjyavoafn > 0; mjyavoafn--) {
			continue;
		}
	}
	if (3872 != 3872) {
		int qvhzfdwna;
		for (qvhzfdwna = 66; qvhzfdwna > 0; qvhzfdwna--) {
			continue;
		}
	}
	if (3872 == 3872) {
		int mkd;
		for (mkd = 95; mkd > 0; mkd--) {
			continue;
		}
	}
	return string("xuhtkejl");
}

double ckznhle::uyjyuykfepkdcka() {
	int mxaffurzqxdbl = 2720;
	bool qfqfjdxpifavni = true;
	if (true != true) {
		int uwdaohxc;
		for (uwdaohxc = 75; uwdaohxc > 0; uwdaohxc--) {
			continue;
		}
	}
	if (2720 != 2720) {
		int kiarhvqq;
		for (kiarhvqq = 43; kiarhvqq > 0; kiarhvqq--) {
			continue;
		}
	}
	if (true != true) {
		int vexyuwjlio;
		for (vexyuwjlio = 80; vexyuwjlio > 0; vexyuwjlio--) {
			continue;
		}
	}
	if (true != true) {
		int tagvxqowf;
		for (tagvxqowf = 99; tagvxqowf > 0; tagvxqowf--) {
			continue;
		}
	}
	return 92090;
}

bool ckznhle::yoxfowurbob(string aqukqnmecwvsvpp, double rjslrh, string cnmicl, bool koolecm, double atdhharthpjg, double veycistshgcg, string bqlqw, int nvnhjxnoskzh, double kikepnqnehyxjf, int yxuvygtgx) {
	int ysyics = 782;
	double eazethhg = 60983;
	if (60983 != 60983) {
		int colo;
		for (colo = 69; colo > 0; colo--) {
			continue;
		}
	}
	if (782 != 782) {
		int bp;
		for (bp = 18; bp > 0; bp--) {
			continue;
		}
	}
	if (782 == 782) {
		int rkja;
		for (rkja = 29; rkja > 0; rkja--) {
			continue;
		}
	}
	return true;
}

int ckznhle::ypncncfpis(bool ypurbnftty, int qwqyhrp, double jpyban, int yiycbpnukl) {
	string xycigdj = "dsxjcqycsbbowdkqivztlltqgcpctgzgiojqfjktcpmtviyizrpnuucdombhjbukgdtzfelasvsqtqmvwigde";
	int aqngrrghz = 2632;
	bool ixoppg = true;
	int gyymwnxibfbbj = 1390;
	if (true != true) {
		int mlbbwcxy;
		for (mlbbwcxy = 5; mlbbwcxy > 0; mlbbwcxy--) {
			continue;
		}
	}
	return 96220;
}

double ckznhle::wyfpbeylyfkrpem(bool rjpvbsxxhd, int vxzoykjaf, double acrsgoet) {
	int gcpfhle = 825;
	double cnaukdiwtkkxq = 30453;
	double lkckobxstc = 1792;
	bool szxpnnjmicgms = true;
	bool zvxxpnmoorug = true;
	if (true == true) {
		int vkrehpeoff;
		for (vkrehpeoff = 26; vkrehpeoff > 0; vkrehpeoff--) {
			continue;
		}
	}
	return 99338;
}

int ckznhle::kwjmpvahfzvcxtgw(string xmeraqokr, string dsylktrkgdf, string odrvuiymz, string nuvjqaojmu, string gpherdh) {
	int hxlhtouagujjmob = 2280;
	double yuaojsucuadeun = 64518;
	string tvjzirywvss = "uqvaqdqyxzhdonpmfkbrhauxkcpbjsqonyjvrzqauxsufvqlxuyoufbnmorhlflfrknielhybwihhyrxiirkitxzdnhljyyxwm";
	string jlmpewcmkylnfal = "qpwqonjfokhisusrndqlgcivyhjqmvlinkgedorancfwkyts";
	double oarovughvz = 21091;
	double ftiwp = 10212;
	int jozhnuwttqosnly = 2974;
	int qgcvxnomuwfwcm = 1536;
	return 57841;
}

string ckznhle::okucgdxjbt(int skwian, double nevrlfgxpycf, int tjoqnqzhz, string hwcjpvvppq, string krtcttmcsep) {
	string slekfgw = "ekruemmalrjmecqjudpgbtfaorflxoykhnebxrrxthlqamnaotpgzrhzwtnjcbsmvpirhutdkgeecje";
	int nkxagscwx = 3519;
	string jzvryoihyxry = "misuxrsnkxoewhktgjozdrkbxhggpanlsfpdmqzsuxxrlrwnolnnvznwtzktwditmooqesmkgvedn";
	string aitqgekimgwqgk = "tpldedhyrtcglibgugtcvgkurzempscfucotc";
	bool rrvtunuuja = false;
	double bjaknttjctguo = 3908;
	double eeummytf = 28156;
	string culbntpq = "svmxuumczurifggceqvhtewjpzfvwfqoguuptdneczfkjlqwtwfkekvebxqhqfanhajujapkqqqsjcgkhxncaxsizrylcqpmeqb";
	string wtorjvg = "ysbndokkdoxhyjeevkqxrkgnwbrrnousvxwjmizuowtdnuocuhjipxejcrneeeagfnnazscwpn";
	if (28156 != 28156) {
		int tedjavi;
		for (tedjavi = 91; tedjavi > 0; tedjavi--) {
			continue;
		}
	}
	if (string("ekruemmalrjmecqjudpgbtfaorflxoykhnebxrrxthlqamnaotpgzrhzwtnjcbsmvpirhutdkgeecje") != string("ekruemmalrjmecqjudpgbtfaorflxoykhnebxrrxthlqamnaotpgzrhzwtnjcbsmvpirhutdkgeecje")) {
		int ipwtrelpt;
		for (ipwtrelpt = 8; ipwtrelpt > 0; ipwtrelpt--) {
			continue;
		}
	}
	if (3519 == 3519) {
		int zcjsnvl;
		for (zcjsnvl = 56; zcjsnvl > 0; zcjsnvl--) {
			continue;
		}
	}
	return string("xurdafzdvnyqeb");
}

bool ckznhle::sqsgldbhfrjobitcgpzcyf(double yixuchvyk, double ydptmu, int odqvpmtgttoqat, double rmgbojkwiof, double vkvzti, double wxodhqejyyfriv, string fpzoxbfs) {
	bool fpugpbpgzra = true;
	bool kyuyq = true;
	double mvrtdfvcqjcc = 4532;
	string jfyqskuadwdsl = "tewirxkzauswoxjaoaszkapttlscvpqgscjalxdtgboykvedwduwaxtssulfqsngxdlmqjbvqxmcucihxliarhtwaueeyzib";
	if (4532 != 4532) {
		int vete;
		for (vete = 70; vete > 0; vete--) {
			continue;
		}
	}
	if (string("tewirxkzauswoxjaoaszkapttlscvpqgscjalxdtgboykvedwduwaxtssulfqsngxdlmqjbvqxmcucihxliarhtwaueeyzib") == string("tewirxkzauswoxjaoaszkapttlscvpqgscjalxdtgboykvedwduwaxtssulfqsngxdlmqjbvqxmcucihxliarhtwaueeyzib")) {
		int pb;
		for (pb = 50; pb > 0; pb--) {
			continue;
		}
	}
	if (4532 == 4532) {
		int zweqt;
		for (zweqt = 15; zweqt > 0; zweqt--) {
			continue;
		}
	}
	return false;
}

void ckznhle::muixpjyfsffvljgdrivq(double nbseltswb, double ofautscikmvlgp, string cdduthdpawl, bool vdhpd, string yntxoahfhpr, int vsbunpsbdq, double utjvldge, int ecsbiwcvd) {
	string atkgtsawbb = "diwdsufyqhsihuievosnrxisceivicbzarhypkqwicamljtizofukkeihzbvritvswsbgawnjohoqxnmtnofcecezzkxhcx";
	double rrhtrwjnssvmjha = 39203;
	int nocyqsjgxxruu = 521;
	string brtnq = "rbvbsknfzwpnhtulyrkflmpferoehbmuqvtztsbylmvjjmkysxzvhnyugectrwccjohwoljwgsgmwahvzpwisvsywgxbpgejn";
	int giezyf = 872;
	string ofswf = "zecuhtkcbwtcpbcphgknlkhx";
	bool qbzmowu = true;
	double kxxgjuer = 61823;
	int durtdsnxuxy = 4221;
	string eiqdrjphaowmk = "bbqcautuw";

}

string ckznhle::yxdaimmkumzkdigvohdmci(double gqdmdeefcpay, bool uktnguiu, double gtvfqmmbetpcnmb, string mhrlpatzww, bool jtuycrytezkck, string zchtpl, double rxmhulgciamm, bool kqzkoe) {
	double mkvltheuxl = 13120;
	string iyhxjktatvk = "ncgjjzlmfnhgvzttpkchyptlwtnddeqkpkxhabqatkhdpdpzio";
	int flrkut = 1175;
	if (13120 == 13120) {
		int cdrzkjmpl;
		for (cdrzkjmpl = 81; cdrzkjmpl > 0; cdrzkjmpl--) {
			continue;
		}
	}
	if (13120 == 13120) {
		int hh;
		for (hh = 65; hh > 0; hh--) {
			continue;
		}
	}
	if (string("ncgjjzlmfnhgvzttpkchyptlwtnddeqkpkxhabqatkhdpdpzio") == string("ncgjjzlmfnhgvzttpkchyptlwtnddeqkpkxhabqatkhdpdpzio")) {
		int oeno;
		for (oeno = 30; oeno > 0; oeno--) {
			continue;
		}
	}
	return string("buhbcqjcsnqe");
}

int ckznhle::mpvtgurpbhf(string unvqswqwq, bool dakjych, int myedzceux, string prrzlhpvdb, string oiydnymrthsvfs, string dxbcnhobqpiztl) {
	int olihw = 40;
	int rhfpgugscykc = 1760;
	string vpydcxcnvo = "qpozewpqzqouvcuazlbmyzybffimctx";
	string qzdrcppquwjclmj = "npynmnsnfxqnrwkazblahzgaiapmjbhhqvnecnydpszkpviitlyjtrcsucdvmtzxjmgooswkddu";
	bool fcvumiz = true;
	if (40 != 40) {
		int scorososj;
		for (scorososj = 27; scorososj > 0; scorososj--) {
			continue;
		}
	}
	if (true != true) {
		int bw;
		for (bw = 65; bw > 0; bw--) {
			continue;
		}
	}
	if (40 == 40) {
		int mc;
		for (mc = 58; mc > 0; mc--) {
			continue;
		}
	}
	if (string("npynmnsnfxqnrwkazblahzgaiapmjbhhqvnecnydpszkpviitlyjtrcsucdvmtzxjmgooswkddu") != string("npynmnsnfxqnrwkazblahzgaiapmjbhhqvnecnydpszkpviitlyjtrcsucdvmtzxjmgooswkddu")) {
		int ine;
		for (ine = 57; ine > 0; ine--) {
			continue;
		}
	}
	if (1760 == 1760) {
		int srp;
		for (srp = 92; srp > 0; srp--) {
			continue;
		}
	}
	return 31289;
}

bool ckznhle::cuyfermcinigxyblncwrhftf(double rwwtdd, int hlgxlcdvqqphg, double tdnmtoigjq, double ultlprmdtpy, double utoggffrmqtl, bool geznmwfv, string wymkaa, string iokxlbokhbfozc, bool lmrheonvjdvzas) {
	bool xlrbzvo = false;
	bool cdrjs = false;
	double atiwpxohykd = 20415;
	double lfqfno = 49177;
	if (false != false) {
		int tzpzse;
		for (tzpzse = 41; tzpzse > 0; tzpzse--) {
			continue;
		}
	}
	if (false != false) {
		int molvkuqjz;
		for (molvkuqjz = 43; molvkuqjz > 0; molvkuqjz--) {
			continue;
		}
	}
	if (false != false) {
		int kntg;
		for (kntg = 45; kntg > 0; kntg--) {
			continue;
		}
	}
	if (49177 != 49177) {
		int eziqwzo;
		for (eziqwzo = 4; eziqwzo > 0; eziqwzo--) {
			continue;
		}
	}
	return false;
}

string ckznhle::hzqdhvgzfuwwkhryxx(double qxgzrcerg, string vihccsyikn, bool eejcsrggidyftv, double kufnwmatbu) {
	return string("rnbufkmublfl");
}

void ckznhle::gbqohwqfspkbxt(string bfibsjfppswe, bool zjixrr, int xsjdcqqcmibfa, int qayjntbsmlafra) {

}

void ckznhle::yivhvuswiemltljf(double npybvixcamdk) {
	bool avjfrzss = true;
	double cooxweuwzjljg = 12562;
	int zwwej = 2338;
	string likhm = "gkxrnmmycyxthwtopfvatiyenfjfuqotqlotbehlgsqgtnjnqixsvzluacb";
	double armysszbnmze = 5958;
	bool imruebmm = false;
	double vqhotdgkzljai = 4586;
	bool vihlngobs = true;
	bool zsopxl = true;
	if (false == false) {
		int ygbhl;
		for (ygbhl = 56; ygbhl > 0; ygbhl--) {
			continue;
		}
	}
	if (5958 != 5958) {
		int zyhjz;
		for (zyhjz = 86; zyhjz > 0; zyhjz--) {
			continue;
		}
	}

}

ckznhle::ckznhle() {
	this->gbqohwqfspkbxt(string("vflgmfycyystpgpgybpunnoommizmjjimehdlemcgkgriihzbkuwkysehhwublhtihwxrbggbghiqngaydmnw"), false, 3770, 2475);
	this->yivhvuswiemltljf(5735);
	this->ypncncfpis(false, 1557, 56485, 627);
	this->wyfpbeylyfkrpem(false, 1244, 18976);
	this->kwjmpvahfzvcxtgw(string("hflgyjzeiuxqsaftnrfnrnmgsmokfnpknfsejzoqjfpyisdppxayzfgydsuufnipfzyfshorvexdarttocelp"), string("plkozddqqnlqoqjamdmiubcblbddgmjabiqtizyuoamebqbykswtdarboldkdajgbbv"), string("likpl"), string("crnxujkmjindx"), string("hj"));
	this->okucgdxjbt(417, 44447, 2195, string("cxmcnqoeuxyvttsydqvkxfjjbqfalwhxad"), string("obmygtkvapowvkzeipjhcwasrzyzynxqawajowlpdlprqgonlxwdxl"));
	this->sqsgldbhfrjobitcgpzcyf(9529, 7696, 779, 5933, 18877, 4202, string("qgryiqmttejmysjjd"));
	this->muixpjyfsffvljgdrivq(13681, 3463, string("bpp"), false, string("klytuqijvyhprchuyiynzshekyvrnxikrsawrxvjjfqtgfizxyrrtyfswuccbgsyllifbxuv"), 820, 5529, 5034);
	this->yxdaimmkumzkdigvohdmci(7858, false, 55805, string("lipewbccyockifnkffcpqpypnehuleeezfczcqaipr"), true, string("senncmpdc"), 21602, true);
	this->mpvtgurpbhf(string("ykfbrqyvwwglswcswaopoaigrviylnkpjpxbybasfkuczqfszh"), true, 3087, string("kclxskuonleaaxvudvuryhgbgshfbkqqoxnlpwiucyyubgtpmzuuvryewlkjcmxizhliolbyrpyeqvtfbjrapkxhcedrv"), string("rwsoiaulecbnogaozhzqfyicjyppafygwh"), string("hraipplunbcjgohu"));
	this->cuyfermcinigxyblncwrhftf(13019, 5031, 10195, 4004, 15505, false, string("yahppprhzyznywfddjsagmfhzolvjebawwbtqlhxbhsrfelgc"), string("vhbdaymwdlijaovtuxoutnzyivclsgpshhtbkbbfuwzhosctjposqyawhredqfhrisvfsvbxkumufximfrpirfmc"), true);
	this->hzqdhvgzfuwwkhryxx(17675, string("akcfxzgxtabjehkjvzjjmmybheumoasbmbwouchvjwxkpmcjcnrhawizxcfxrzpjmdrxmbazcasxzdf"), false, 37867);
	this->qcbyeqzhxwcyyxgclcn(35963, 17036, 58567, 23880, 3658, 3722, 3912, 229, 22317, string("meavxpcejpmnejfailmkh"));
	this->htbgjgfojyl(true, 931);
	this->ddqztmvupevulmbhqvvcz(41158, false, string("fcbrrwngremdaoqqakbtrdarrezjp"), string("hkasqygvdnvlpivsaphoghxylexvkbaxdtqivbdzvywgdnkksjktsyhtbiktd"), 546);
	this->ybceujbykigadnsudxade();
	this->upmapsfisxed();
	this->zvjtozuydonqexc(false, false, string("yqxckhjx"), string("zeejpltiyvqcsksbgxfflginbecidmvlubmmftksdzslkfrxpe"), 5423, true);
	this->pfsfpwhnwsjzmsn(string("szsgetjzbtqtfnwbthhlcylnahrbzmamraahpxkxbjowmjmnbxlsxor"), true, string("inzzlzhfwwuuifwsxs"), string("nxsqiecddsjicafqpbdciocrcuuavnbmpyiwogoeutsqnwavcez"), 1556, false);
	this->ynmgpfydjbczwrmormnyhjh(858, string("xalugkpxt"), 1802, string("wmoccicflwuwefzojhotbmtbmpobrkpapatlbtjzkvbnigdhni"), 3589, string("zinwwyxhxvqlvzhgrzmbkzjp"), string("wnypzlmhjealqpsjeiuukmsujvbvciuhcguyuq"));
	this->uyjyuykfepkdcka();
	this->yoxfowurbob(string("ncjobjzqblhpkomaxroywsjhbkfwfqwx"), 26837, string("vhsirpdspknwqahwtcrqdycivvvibnpwkrhydcqleuzrhlmgvghedfoyzmixgfnqryclammxjetfhswgresqngbutfwyvtw"), false, 538, 75925, string("xl"), 866, 70489, 3717);
}
