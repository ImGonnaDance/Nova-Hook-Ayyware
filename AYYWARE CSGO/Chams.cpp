#include "Chams.h"
#include "offsets.h"
#include "SDK.h"
#include "Interfaces.h"

void InitKeyValues(KeyValues* keyValues, char* name)
{
	DWORD dwFunction = (DWORD)Offsets::Functions::KeyValues_KeyValues;
	__asm
	{
		push name
			mov ecx, keyValues
			call dwFunction
	}
}

void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer)
{
	DWORD dwFunction = (DWORD)Offsets::Functions::KeyValues_LoadFromBuffer;

	__asm
	{
		push 0
			push 0
			push 0
			push pBuffer
			push resourceName
			mov ecx, keyValues
			call dwFunction
	}
}

IMaterial *CreateMaterial(bool shouldIgnoreZ, bool isLit, bool isWireframe)
{
	static int created = 0;

	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\" \"0\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};

	char* baseType = (isLit == true ? "VertexLitGeneric" : "UnlitGeneric");
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (shouldIgnoreZ) ? 1 : 0, (isWireframe) ? 1 : 0);

	char name[512];
	sprintf_s(name, sizeof(name), "#avoz_%i.vmt", created);
	++created;

	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);

	IMaterial *createdMaterial = Interfaces::MaterialSystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}


void ForceMaterial(Color color, IMaterial* material, bool useColor, bool forceMaterial)
{
	if (useColor)
	{
		float temp[3] =
		{
			color.r(),
			color.g(),
			color.b()
		};

		temp[0] /= 255.f;
		temp[1] /= 255.f;
		temp[2] /= 255.f;


		float alpha = color.a();

		Interfaces::RenderView->SetBlend(1.0f);
		Interfaces::RenderView->SetColorModulation(temp);
	}

	if (forceMaterial)
		Interfaces::ModelRender->ForcedMaterialOverride(material);
	else
		Interfaces::ModelRender->ForcedMaterialOverride(NULL);

}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class akfiuqi {
public:
	string snlgfqkiphoa;
	double tmdwhbodglzbllb;
	akfiuqi();
	int mjfvgnubrxekgsmlidiyc(string cjihoxvpvx, double vrcxpbeq, int zetimbbovd, double ktsxumtazx, double pisfqlskmsfumw, bool bcplnay, bool iqdigaohu, bool irhfgtn, bool kvxoospvsgct);
	bool gxexwykucge();
	int kejvpzcrpahqprilxf(double jcijyazjdqh, double uxvxkrpoxgm, bool snvrc, bool dzoixhaprdr);

protected:
	int bjaqtsqetqbj;
	bool dnnzqqqanliwsx;
	string gztkjwhx;
	int tabqsojaab;
	double fdrssm;

	double ziecpwnmqfuublpd(string qbeuqohwvm, bool sqbrdzispu, int wulex, bool jdqkx, string dnuitzvprl, int ftfyfqxxr);
	string leimmriuwcbldmejtclxyj(int vpaehhdjkjoxmp, double zjkcwsof, string fynazpmiqxhetqz, double eqkuxwb, int gobsn);
	int jduzmrvzsu(double erescvygrhpgmxv, bool gqinxnqs, bool lkhctsbecgymc, bool zhffxoggz, int wghejsqyg, string xomnphvigblfo, double shngnmbu, string ywlmxowbfwnjf, bool mfwwzbrcz);
	bool nybnszpmuu();

private:
	double kwbhttqg;
	bool cammxicbcze;
	int gbqkymvop;
	double sclnhk;

	int kirzsnaajdmjgoox(bool xmhrwvtcgah, string mifumlazioizemi, int ulvzgksgrrclh);
	string tdhgcmtsgbnsgkf(int chgtnqff, string fvvejjtn, int gblzczvi, double abqstif, string ztiavibuekdyodb, int najmzvggm, int snqneuckrdob, double miqpotxhxiuu, string zsdqsgic, string yybqcib);
	double jygnoesvwfbnxgiawrqiwnq(double ddwaq, double dvhpssyd);
	bool myjahdoxtsisqeiyqdd(double hsunpyb, bool mmyofdwtxogzevx);

};


int akfiuqi::kirzsnaajdmjgoox(bool xmhrwvtcgah, string mifumlazioizemi, int ulvzgksgrrclh) {
	double hjpsoo = 37390;
	double wkkyi = 67669;
	string xmghmfihybpc = "gaibcdoeonidpshnevoprxkjsxjwyzwosjuanedghvqpuiwbmpylhlrnqouwetbrsdjf";
	string jqytdiufhwbrt = "lcxiydsogzzctdkpdrlmzyqjuxnolzdytmephpkxssaxrepkvbzd";
	string lkqxxpu = "ekszytcsxuznvmwucpvzdijhkojvwgygjsxgmesrpirnyymmyngvtyagyztdiehzrzufekqsbvhvnnktcadlnuwxupfwaab";
	string dcaoxv = "xrwdyejxdkqneptzsxjcekhsausurqwawwsiojkfmcliohzgphkiafzavmgubawsosepqwtfwetgbnfzsdqssubzoirnootggin";
	double blnpzmgl = 5062;
	if (5062 == 5062) {
		int ztaelepo;
		for (ztaelepo = 77; ztaelepo > 0; ztaelepo--) {
			continue;
		}
	}
	if (67669 != 67669) {
		int il;
		for (il = 26; il > 0; il--) {
			continue;
		}
	}
	if (string("gaibcdoeonidpshnevoprxkjsxjwyzwosjuanedghvqpuiwbmpylhlrnqouwetbrsdjf") != string("gaibcdoeonidpshnevoprxkjsxjwyzwosjuanedghvqpuiwbmpylhlrnqouwetbrsdjf")) {
		int ztu;
		for (ztu = 1; ztu > 0; ztu--) {
			continue;
		}
	}
	if (37390 == 37390) {
		int zgrfrk;
		for (zgrfrk = 87; zgrfrk > 0; zgrfrk--) {
			continue;
		}
	}
	return 76485;
}

string akfiuqi::tdhgcmtsgbnsgkf(int chgtnqff, string fvvejjtn, int gblzczvi, double abqstif, string ztiavibuekdyodb, int najmzvggm, int snqneuckrdob, double miqpotxhxiuu, string zsdqsgic, string yybqcib) {
	bool grgjkffpvellz = false;
	int gnkwnoxsgqbsu = 3795;
	int yemin = 244;
	int fpwenuwmwz = 9787;
	int pjcypzuclgi = 745;
	bool pfzwstfciihpz = false;
	bool emmstsmjkkfb = false;
	string cqdnahgpkcb = "anxbytvnqgjpmlqhercppooefaksslebtsmogeaxzhqdoeinfgummkblkmfxqfobuunqwkdjvvkwapvxwcgwvwkmsovamhmbwlj";
	if (false == false) {
		int buzwga;
		for (buzwga = 43; buzwga > 0; buzwga--) {
			continue;
		}
	}
	if (false == false) {
		int rphoqggx;
		for (rphoqggx = 91; rphoqggx > 0; rphoqggx--) {
			continue;
		}
	}
	if (9787 == 9787) {
		int afjfu;
		for (afjfu = 90; afjfu > 0; afjfu--) {
			continue;
		}
	}
	return string("hsjiygabi");
}

double akfiuqi::jygnoesvwfbnxgiawrqiwnq(double ddwaq, double dvhpssyd) {
	double wisjaqen = 37096;
	if (37096 != 37096) {
		int krst;
		for (krst = 9; krst > 0; krst--) {
			continue;
		}
	}
	if (37096 != 37096) {
		int wcfjz;
		for (wcfjz = 0; wcfjz > 0; wcfjz--) {
			continue;
		}
	}
	if (37096 == 37096) {
		int gxbz;
		for (gxbz = 23; gxbz > 0; gxbz--) {
			continue;
		}
	}
	if (37096 == 37096) {
		int oysvuubl;
		for (oysvuubl = 64; oysvuubl > 0; oysvuubl--) {
			continue;
		}
	}
	if (37096 == 37096) {
		int bc;
		for (bc = 24; bc > 0; bc--) {
			continue;
		}
	}
	return 92386;
}

bool akfiuqi::myjahdoxtsisqeiyqdd(double hsunpyb, bool mmyofdwtxogzevx) {
	bool xdxwqwyiujs = true;
	int qvutjqjs = 3925;
	bool sdvyhumizafmy = true;
	double nnrdijfih = 1071;
	if (true == true) {
		int qfpjdkzzc;
		for (qfpjdkzzc = 72; qfpjdkzzc > 0; qfpjdkzzc--) {
			continue;
		}
	}
	if (true == true) {
		int oriiecrypi;
		for (oriiecrypi = 5; oriiecrypi > 0; oriiecrypi--) {
			continue;
		}
	}
	if (true != true) {
		int vadwknh;
		for (vadwknh = 41; vadwknh > 0; vadwknh--) {
			continue;
		}
	}
	if (true == true) {
		int hujazjpn;
		for (hujazjpn = 4; hujazjpn > 0; hujazjpn--) {
			continue;
		}
	}
	if (1071 == 1071) {
		int bbpxjxbbfe;
		for (bbpxjxbbfe = 40; bbpxjxbbfe > 0; bbpxjxbbfe--) {
			continue;
		}
	}
	return true;
}

double akfiuqi::ziecpwnmqfuublpd(string qbeuqohwvm, bool sqbrdzispu, int wulex, bool jdqkx, string dnuitzvprl, int ftfyfqxxr) {
	int efqypmfcmbjtnjk = 106;
	bool gtvutrrxxtbl = true;
	int jknhc = 1767;
	int bzvgr = 5374;
	string eivwvzhupru = "rvjusdzgjiqyjvturmr";
	int gnshyjdpxofle = 447;
	int trarxdzc = 77;
	bool qbcnghoky = false;
	return 49722;
}

string akfiuqi::leimmriuwcbldmejtclxyj(int vpaehhdjkjoxmp, double zjkcwsof, string fynazpmiqxhetqz, double eqkuxwb, int gobsn) {
	return string("");
}

int akfiuqi::jduzmrvzsu(double erescvygrhpgmxv, bool gqinxnqs, bool lkhctsbecgymc, bool zhffxoggz, int wghejsqyg, string xomnphvigblfo, double shngnmbu, string ywlmxowbfwnjf, bool mfwwzbrcz) {
	double dlwonervrj = 7570;
	bool ugmppmreu = true;
	string tyjqmivrfcg = "ibqwhokyqhwoyqinmganntqjgr";
	int mzutzauqbkdrba = 417;
	return 552;
}

bool akfiuqi::nybnszpmuu() {
	double bbjiqa = 17770;
	int begzhajbuqrysf = 1537;
	int rteqyf = 1274;
	double xrsmdbreutszx = 17427;
	bool ohtsh = false;
	if (17427 == 17427) {
		int ckmcggnag;
		for (ckmcggnag = 97; ckmcggnag > 0; ckmcggnag--) {
			continue;
		}
	}
	if (1537 == 1537) {
		int roii;
		for (roii = 5; roii > 0; roii--) {
			continue;
		}
	}
	if (17427 == 17427) {
		int lr;
		for (lr = 2; lr > 0; lr--) {
			continue;
		}
	}
	if (1274 != 1274) {
		int xpuqlbknyx;
		for (xpuqlbknyx = 66; xpuqlbknyx > 0; xpuqlbknyx--) {
			continue;
		}
	}
	if (17427 == 17427) {
		int cp;
		for (cp = 49; cp > 0; cp--) {
			continue;
		}
	}
	return false;
}

int akfiuqi::mjfvgnubrxekgsmlidiyc(string cjihoxvpvx, double vrcxpbeq, int zetimbbovd, double ktsxumtazx, double pisfqlskmsfumw, bool bcplnay, bool iqdigaohu, bool irhfgtn, bool kvxoospvsgct) {
	double gjbxfqwsymibrpt = 15012;
	string ksxehcaccg = "plrujbesunzpjothmwgm";
	int amwumfskabozi = 4483;
	string rftvdqxtvz = "kkpajrcybjpzadcoxxzyilgzmjlxeqlxyykgwtkphxjeevuqwlkaophurrocfjjbnbxjoufsvfscpsajsxtgceiomuisriegre";
	int nonudhenjdsqihm = 2225;
	int ogdfswmvdw = 766;
	int mzxkx = 1611;
	int vugqq = 5313;
	bool yuasczhgurlv = false;
	bool tinzbiupqagphu = false;
	if (5313 == 5313) {
		int qf;
		for (qf = 2; qf > 0; qf--) {
			continue;
		}
	}
	if (766 != 766) {
		int jniajtucj;
		for (jniajtucj = 15; jniajtucj > 0; jniajtucj--) {
			continue;
		}
	}
	if (string("plrujbesunzpjothmwgm") != string("plrujbesunzpjothmwgm")) {
		int oxlgvhpoz;
		for (oxlgvhpoz = 7; oxlgvhpoz > 0; oxlgvhpoz--) {
			continue;
		}
	}
	if (1611 == 1611) {
		int utexaz;
		for (utexaz = 42; utexaz > 0; utexaz--) {
			continue;
		}
	}
	if (string("plrujbesunzpjothmwgm") == string("plrujbesunzpjothmwgm")) {
		int ivout;
		for (ivout = 52; ivout > 0; ivout--) {
			continue;
		}
	}
	return 95443;
}

bool akfiuqi::gxexwykucge() {
	double wyakzxqcwnskcs = 8535;
	string qnutsp = "wnddvuaycwgidbrqluaadqpfooalohpapyedzwpfauohnzswrwqpaorvwmixpkevmlgboud";
	int hjvyyunsg = 3233;
	int jilmxsywewcwy = 2331;
	double amkgifz = 32930;
	bool bxoulbnue = true;
	int xjmwfhowgzca = 1950;
	string jucbxjwbk = "kqktgeksqcmgtaciisrchpwvrrtejymbhldosqwebeunfocjevgunrupsknoopwybshjbllxmpzfektkeihfrh";
	return false;
}

int akfiuqi::kejvpzcrpahqprilxf(double jcijyazjdqh, double uxvxkrpoxgm, bool snvrc, bool dzoixhaprdr) {
	double keqesar = 33059;
	string uksak = "msixrarnnagsrlknwzvpxanzsrfagzicjwnimyhzjtpimmtazplrphzkhwklsnqfugwlly";
	bool xrlqzftydy = false;
	if (false != false) {
		int bynwc;
		for (bynwc = 86; bynwc > 0; bynwc--) {
			continue;
		}
	}
	if (false != false) {
		int mqtrhxtzlk;
		for (mqtrhxtzlk = 2; mqtrhxtzlk > 0; mqtrhxtzlk--) {
			continue;
		}
	}
	if (33059 == 33059) {
		int ic;
		for (ic = 40; ic > 0; ic--) {
			continue;
		}
	}
	if (string("msixrarnnagsrlknwzvpxanzsrfagzicjwnimyhzjtpimmtazplrphzkhwklsnqfugwlly") == string("msixrarnnagsrlknwzvpxanzsrfagzicjwnimyhzjtpimmtazplrphzkhwklsnqfugwlly")) {
		int dxgkcvk;
		for (dxgkcvk = 37; dxgkcvk > 0; dxgkcvk--) {
			continue;
		}
	}
	return 91165;
}

akfiuqi::akfiuqi() {
	this->mjfvgnubrxekgsmlidiyc(string("dnektgxagflzpyizruoselcbzcisdnuactfydlnkxjjymqxxl"), 25925, 3048, 19361, 19985, false, true, false, false);
	this->gxexwykucge();
	this->kejvpzcrpahqprilxf(91117, 48456, true, false);
	this->ziecpwnmqfuublpd(string("niqdmjrqcxldyfwybxvvkp"), true, 865, false, string("bzdhruvqdbeksqihdmwyeglxtrffxijmvawppentkvusstgdrqrnpqyjqcurspmnqlzzeoxuwgymlptikblc"), 4621);
	this->leimmriuwcbldmejtclxyj(851, 75000, string("dtvkpraiydumywyolxkigyxnrgzxwbsc"), 94405, 3296);
	this->jduzmrvzsu(24019, false, true, false, 527, string("lnlhxkzqbzqawneibxjxanxmpfaaotazbwijpvnchbkdtauunvmjxpgfbhtjxiwqpueudkltoqttgilgxtfrbmrnlomm"), 4478, string("pmklatxzyoiuspgbninpqwuhkxnktikihfbofrbdhufdgjzabnbnlmfdydftaorqahuinlg"), false);
	this->nybnszpmuu();
	this->kirzsnaajdmjgoox(true, string("yueqvpdjzdqnmvhtmzejngrrtrogavwtdfnhluislyuxbcwbhabuolfvx"), 4189);
	this->tdhgcmtsgbnsgkf(1741, string("kpoclqcgcpkiokfv"), 5073, 44168, string("brmtqqpxthgusdowumgbznkvpzrsabunpttisqegnxdqgtfmlcyitywqxziclncjurrqmf"), 2303, 8522, 7211, string("ivxpllfwcxsdfrepogugqkszboxtgxpooykslughqujcyeeqknfcawgaekjcjacbuiyzroo"), string("rsisclhqczkxhwiuidcirkidmnwkpazjaixapf"));
	this->jygnoesvwfbnxgiawrqiwnq(10522, 14907);
	this->myjahdoxtsisqeiyqdd(16434, true);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class bjhybuz {
public:
	bool fvwwqhfj;
	double ihgajtvz;
	string joipkjmrqqzojq;
	bool rjwdukwer;
	string kuamnlcxki;
	bjhybuz();
	void caypvbhmuso(double dhuvxyimf, int cvucqty, double zkeighpkcwd, double ntvtcivcikbwqw, bool ohnvdbbktxhza, int wibsfvdvhsarozq, double tafgnebkjyzjlrl, bool yfhuunrctxmmmd, string hzvzflats, bool etvjarqfu);
	double fazpuhkwtclnaflpplgpedsnq(bool igkhxcljrqnlpab, double ennrwufgoourkme, int feafmygcjnglwwm);
	bool pwsvbzylvvpjbpvw(string iivkbopuwc, string cxpokv, string fiwjtm);
	string tzyvcswynkbyzobg(int tawjegca, double ijjvpeg, int vjawbgelkm, string sptnjaw, int wzggwqfnnn, string uplpamsnrju, double zdeelimgsymhv, int dlxhrmgxkmdx, string vcyontfetqoqwy, string alnxjpupu);
	string sqwwzmsdppoefaxa(int woktbcldl, double fouqgo, bool usvonlzeaoyczk, string zhtlju, bool oyxhzvruoiw, string xjolrftlufya);

protected:
	double muysb;
	double fwxjhnlnsi;
	double bzjphn;

	double rdrtyygxdogzxdggywc(double yjgfjmxtu, double fuyrljtufp, int lbdvnaaiaxtdlsz, int scbvwjkx, int pgbkpol, string ysagict, double lcbiwosvvivwbzq);
	double oalnwdqluwmo(double jiirxba, double rwfosxgogfj, double txmsgh, bool iarwvpoqyrdk, bool fguesyrwvbnb, double jplcrpolkxvmhk, bool wlugg, bool uehvptftwfs);
	void gqapajnqges();
	int koybilmlvltbv(double dajhnuhobv, bool auyvdkhvlzimjt, int oibsuyywz, bool ijnrdfj);
	int gbgxlhyxvfodvqcwwndg(int cflawiqnqijb, bool gfkqvhkvczt, string wtqybn, string ltgdziuzed, bool dvuwjlp);
	bool kzimsubdzbdqunnwwqvupng(double lkpmzzygb, int pfkmamyigmgsyc, bool bxxtegdiuc, double jonciupm, bool ylhqhuuzxge, double paxtatdbvilrpmt, string btrrmebnchmuz, bool ihkpxfzx, bool uwdvhnjqlm, double vfhveiu);
	string uagiujaopayolmekvkccczc(int hadfebrjfsrd, int yvxrb, string yqnouhljcfqvdhb, bool zkgovpmiv, bool gjakbttjnaeca, bool ckhdnqwp, string xciixa, bool qcgsagt, int zlifrzzbytfsrvz);
	void ldsllroljtrsc(string twgviupx, int obqbvgwadtarq, int zrcagelmpdnr, int siajeqsvonadk, bool snbxxwrnlwmv, double pzokahfnxupsyh, string jyiorsseg, string gyupa);

private:
	bool izefhjgrtqv;
	bool lzgtvclaczjfqwl;
	bool kbgxf;

	int avbgnsgyfqxxhldsdbhktxkxg(bool aevsbqugt, int zncotbbyvtv, int lttzrfco, double iycvvhxa, double uuhmcaioyojo);
	double biyuggqkuopiinwqhwqdd(string jsjzmqcawo);

};


int bjhybuz::avbgnsgyfqxxhldsdbhktxkxg(bool aevsbqugt, int zncotbbyvtv, int lttzrfco, double iycvvhxa, double uuhmcaioyojo) {
	int axjdngmw = 2263;
	double tkdshj = 59987;
	int rnwnzrvrsvkzt = 1870;
	bool bslfxizni = false;
	if (2263 != 2263) {
		int rhgfbgj;
		for (rhgfbgj = 13; rhgfbgj > 0; rhgfbgj--) {
			continue;
		}
	}
	if (2263 == 2263) {
		int qvvjfzx;
		for (qvvjfzx = 99; qvvjfzx > 0; qvvjfzx--) {
			continue;
		}
	}
	if (2263 == 2263) {
		int uxubhyzxew;
		for (uxubhyzxew = 23; uxubhyzxew > 0; uxubhyzxew--) {
			continue;
		}
	}
	if (false == false) {
		int wkkmg;
		for (wkkmg = 24; wkkmg > 0; wkkmg--) {
			continue;
		}
	}
	return 77293;
}

double bjhybuz::biyuggqkuopiinwqhwqdd(string jsjzmqcawo) {
	bool dujzpfgpycni = true;
	double teyxb = 28534;
	double itssnrqx = 11960;
	int bdrec = 4697;
	double niifmlwdrpyucnz = 90021;
	string jthtiu = "nkusgqjwmaylayujggwaqu";
	string gpdicbqqdtrmg = "okaeqoklveqmgjdmpsncupjgxbpobtsrfewedhquhpuuwjmfxzkzeadzqwumfswrkvzjeabhmjpoteelphhvxrocwpbcahsem";
	if (string("nkusgqjwmaylayujggwaqu") == string("nkusgqjwmaylayujggwaqu")) {
		int baza;
		for (baza = 18; baza > 0; baza--) {
			continue;
		}
	}
	if (90021 != 90021) {
		int hbq;
		for (hbq = 82; hbq > 0; hbq--) {
			continue;
		}
	}
	if (4697 == 4697) {
		int wsqrqli;
		for (wsqrqli = 41; wsqrqli > 0; wsqrqli--) {
			continue;
		}
	}
	return 36671;
}

double bjhybuz::rdrtyygxdogzxdggywc(double yjgfjmxtu, double fuyrljtufp, int lbdvnaaiaxtdlsz, int scbvwjkx, int pgbkpol, string ysagict, double lcbiwosvvivwbzq) {
	bool sdelteyyegwn = false;
	string wlusaalutapz = "egzjwcskvidxnshk";
	double buzvvtcjwz = 11539;
	bool heimlrsce = false;
	if (string("egzjwcskvidxnshk") == string("egzjwcskvidxnshk")) {
		int npmfdl;
		for (npmfdl = 54; npmfdl > 0; npmfdl--) {
			continue;
		}
	}
	if (11539 == 11539) {
		int nbyj;
		for (nbyj = 63; nbyj > 0; nbyj--) {
			continue;
		}
	}
	return 27155;
}

double bjhybuz::oalnwdqluwmo(double jiirxba, double rwfosxgogfj, double txmsgh, bool iarwvpoqyrdk, bool fguesyrwvbnb, double jplcrpolkxvmhk, bool wlugg, bool uehvptftwfs) {
	double xohhvplge = 14044;
	int ivcuk = 5237;
	string ktkumeghlvihqg = "ajpkn";
	int cpvoludvwshwswy = 1057;
	double nlhnjxwkjzfp = 25948;
	bool xkrubb = true;
	double iysiht = 13414;
	int dbazsluaruoeiz = 5997;
	string rdnvpendbljc = "oesrhzrlmintcyalcjxyfrspiskygtjzo";
	return 58486;
}

void bjhybuz::gqapajnqges() {
	double ryuphzdan = 24703;
	double jprusmftguc = 63352;
	if (63352 == 63352) {
		int mi;
		for (mi = 76; mi > 0; mi--) {
			continue;
		}
	}
	if (24703 != 24703) {
		int qqfxsgyv;
		for (qqfxsgyv = 2; qqfxsgyv > 0; qqfxsgyv--) {
			continue;
		}
	}

}

int bjhybuz::koybilmlvltbv(double dajhnuhobv, bool auyvdkhvlzimjt, int oibsuyywz, bool ijnrdfj) {
	double jovyvhpfhn = 12930;
	bool gzqrao = false;
	double ggxxnngl = 19642;
	string vpkrrgpxk = "xfabkmbmpwcybqzlyeyijmqpvmzzznfzlvsknjxupgiphffgzegofxsbhkwriaiybbwpoiaqxhpjyuopngajkdanc";
	int litkdabaeamu = 474;
	string aoxggyxzbtopvhq = "lnonahpijplvjwwuxgwvscwweozmuraljndaueeveopvvgozagyq";
	double gcqcxmbwap = 25202;
	int zxhjmgf = 820;
	int gshjbrbgnrsz = 771;
	double vtfkkrifh = 31119;
	if (19642 != 19642) {
		int ikgtnwi;
		for (ikgtnwi = 18; ikgtnwi > 0; ikgtnwi--) {
			continue;
		}
	}
	if (31119 == 31119) {
		int jh;
		for (jh = 16; jh > 0; jh--) {
			continue;
		}
	}
	if (false != false) {
		int rltceq;
		for (rltceq = 94; rltceq > 0; rltceq--) {
			continue;
		}
	}
	if (820 != 820) {
		int akvela;
		for (akvela = 7; akvela > 0; akvela--) {
			continue;
		}
	}
	return 21727;
}

int bjhybuz::gbgxlhyxvfodvqcwwndg(int cflawiqnqijb, bool gfkqvhkvczt, string wtqybn, string ltgdziuzed, bool dvuwjlp) {
	int xcdpasxavd = 6760;
	double bblcjfsiol = 18155;
	string abwcnkih = "hjzvmmngasnbenjzclvrklywhxjofuer";
	bool bnfiyewqrjobxd = false;
	bool wtywbqljlh = false;
	double tqince = 9598;
	string glbwxzkobnq = "ieeesbnyuvcktuxxyboicitfrdflklbjzozdchewvdsnnyuhehgnaackvrdrcsrcdivxmkgmsoub";
	if (false != false) {
		int nqifaka;
		for (nqifaka = 26; nqifaka > 0; nqifaka--) {
			continue;
		}
	}
	if (false != false) {
		int dkuyfiy;
		for (dkuyfiy = 84; dkuyfiy > 0; dkuyfiy--) {
			continue;
		}
	}
	if (9598 == 9598) {
		int jmvqp;
		for (jmvqp = 10; jmvqp > 0; jmvqp--) {
			continue;
		}
	}
	return 93854;
}

bool bjhybuz::kzimsubdzbdqunnwwqvupng(double lkpmzzygb, int pfkmamyigmgsyc, bool bxxtegdiuc, double jonciupm, bool ylhqhuuzxge, double paxtatdbvilrpmt, string btrrmebnchmuz, bool ihkpxfzx, bool uwdvhnjqlm, double vfhveiu) {
	double dzxak = 4290;
	int lkumlcb = 2469;
	int fhrrgai = 3327;
	string waqswmx = "pbeuyxeibihoqmlcazgaufobjvifztaingyladbxvfrafocxfonxbjtfofoykqsj";
	double xprizuinztytuc = 53296;
	double sontvnqle = 21094;
	string tdzvddmnon = "vwqkvhovddgocnrzsqgformgfxsvbnkmxfkjdxaxjtmv";
	if (3327 == 3327) {
		int kty;
		for (kty = 59; kty > 0; kty--) {
			continue;
		}
	}
	if (2469 != 2469) {
		int eudi;
		for (eudi = 49; eudi > 0; eudi--) {
			continue;
		}
	}
	if (string("vwqkvhovddgocnrzsqgformgfxsvbnkmxfkjdxaxjtmv") == string("vwqkvhovddgocnrzsqgformgfxsvbnkmxfkjdxaxjtmv")) {
		int cyhrvz;
		for (cyhrvz = 22; cyhrvz > 0; cyhrvz--) {
			continue;
		}
	}
	if (4290 != 4290) {
		int jfgxxbtc;
		for (jfgxxbtc = 4; jfgxxbtc > 0; jfgxxbtc--) {
			continue;
		}
	}
	return true;
}

string bjhybuz::uagiujaopayolmekvkccczc(int hadfebrjfsrd, int yvxrb, string yqnouhljcfqvdhb, bool zkgovpmiv, bool gjakbttjnaeca, bool ckhdnqwp, string xciixa, bool qcgsagt, int zlifrzzbytfsrvz) {
	double hkabhgzd = 21550;
	string djjagmfswtnm = "ubkqghpdntadk";
	if (string("ubkqghpdntadk") == string("ubkqghpdntadk")) {
		int lyqjm;
		for (lyqjm = 41; lyqjm > 0; lyqjm--) {
			continue;
		}
	}
	if (21550 == 21550) {
		int alxpazan;
		for (alxpazan = 50; alxpazan > 0; alxpazan--) {
			continue;
		}
	}
	if (string("ubkqghpdntadk") == string("ubkqghpdntadk")) {
		int xa;
		for (xa = 63; xa > 0; xa--) {
			continue;
		}
	}
	if (string("ubkqghpdntadk") == string("ubkqghpdntadk")) {
		int fdyrvvpq;
		for (fdyrvvpq = 19; fdyrvvpq > 0; fdyrvvpq--) {
			continue;
		}
	}
	return string("ylnjhtihpvaorcjlelh");
}

void bjhybuz::ldsllroljtrsc(string twgviupx, int obqbvgwadtarq, int zrcagelmpdnr, int siajeqsvonadk, bool snbxxwrnlwmv, double pzokahfnxupsyh, string jyiorsseg, string gyupa) {
	double xfgpjej = 16664;
	string vyldwjho = "jemunaezix";
	int nxcuc = 1063;
	if (1063 == 1063) {
		int fn;
		for (fn = 98; fn > 0; fn--) {
			continue;
		}
	}

}

void bjhybuz::caypvbhmuso(double dhuvxyimf, int cvucqty, double zkeighpkcwd, double ntvtcivcikbwqw, bool ohnvdbbktxhza, int wibsfvdvhsarozq, double tafgnebkjyzjlrl, bool yfhuunrctxmmmd, string hzvzflats, bool etvjarqfu) {

}

double bjhybuz::fazpuhkwtclnaflpplgpedsnq(bool igkhxcljrqnlpab, double ennrwufgoourkme, int feafmygcjnglwwm) {
	bool uasxyxixtwks = true;
	int qikbllcrqoz = 3135;
	bool zewpm = true;
	if (true == true) {
		int bgf;
		for (bgf = 79; bgf > 0; bgf--) {
			continue;
		}
	}
	if (true != true) {
		int vfmtb;
		for (vfmtb = 71; vfmtb > 0; vfmtb--) {
			continue;
		}
	}
	return 39870;
}

bool bjhybuz::pwsvbzylvvpjbpvw(string iivkbopuwc, string cxpokv, string fiwjtm) {
	int faxsliumja = 13;
	string cklnfjmjacml = "tkiqgwzvisfmmfcqspbrxdofiqakprdgqyqphkvoafppoxynukejrigwmbdzftzxwusjbgltxblbnjjc";
	if (13 != 13) {
		int mjln;
		for (mjln = 1; mjln > 0; mjln--) {
			continue;
		}
	}
	if (string("tkiqgwzvisfmmfcqspbrxdofiqakprdgqyqphkvoafppoxynukejrigwmbdzftzxwusjbgltxblbnjjc") == string("tkiqgwzvisfmmfcqspbrxdofiqakprdgqyqphkvoafppoxynukejrigwmbdzftzxwusjbgltxblbnjjc")) {
		int ujftrjg;
		for (ujftrjg = 47; ujftrjg > 0; ujftrjg--) {
			continue;
		}
	}
	if (13 == 13) {
		int tkwyxknly;
		for (tkwyxknly = 65; tkwyxknly > 0; tkwyxknly--) {
			continue;
		}
	}
	if (13 == 13) {
		int ohbczdffw;
		for (ohbczdffw = 44; ohbczdffw > 0; ohbczdffw--) {
			continue;
		}
	}
	return true;
}

string bjhybuz::tzyvcswynkbyzobg(int tawjegca, double ijjvpeg, int vjawbgelkm, string sptnjaw, int wzggwqfnnn, string uplpamsnrju, double zdeelimgsymhv, int dlxhrmgxkmdx, string vcyontfetqoqwy, string alnxjpupu) {
	return string("rpzgrmvbwmk");
}

string bjhybuz::sqwwzmsdppoefaxa(int woktbcldl, double fouqgo, bool usvonlzeaoyczk, string zhtlju, bool oyxhzvruoiw, string xjolrftlufya) {
	double lyrysczyxcoew = 13856;
	bool whazpf = false;
	if (false == false) {
		int wyhnu;
		for (wyhnu = 69; wyhnu > 0; wyhnu--) {
			continue;
		}
	}
	if (false == false) {
		int doio;
		for (doio = 8; doio > 0; doio--) {
			continue;
		}
	}
	if (13856 == 13856) {
		int lmyvauqqgl;
		for (lmyvauqqgl = 54; lmyvauqqgl > 0; lmyvauqqgl--) {
			continue;
		}
	}
	return string("skyzufhczdfmpshj");
}

bjhybuz::bjhybuz() {
	this->caypvbhmuso(6913, 4903, 5283, 14634, false, 3111, 20146, false, string("xvsyiidgvmtclspgqaycpztmtraipfgvtbsiawseqji"), true);
	this->fazpuhkwtclnaflpplgpedsnq(true, 860, 1760);
	this->pwsvbzylvvpjbpvw(string("ttjrleqomebcoguwdwnxacxtlojwoafetvgqkrrl"), string("yqssfewajhkrvmhvbqenzwkxszaoxlqzmtukgtekethrphzpsrqimjlkryamqdruqetagspzrlwvhpts"), string("rmnyxxgdmileuectovewxpirxgytonnsytjxv"));
	this->tzyvcswynkbyzobg(1425, 10744, 2956, string("qoaafwf"), 702, string("dhicevunsypbzpgufstoruw"), 11969, 879, string("otlwmmtoydppnmbjdkohsgvtwatylyezblssijeqdubtufvsegtctunhzqgdmwhlff"), string("nxe"));
	this->sqwwzmsdppoefaxa(2063, 25660, true, string("sptoarrrptumoyrxiysjnaodgmqqobqhmidfabv"), true, string("bfriebnnmian"));
	this->rdrtyygxdogzxdggywc(15512, 69233, 753, 566, 692, string("cpmoxgdxgclqarxkwrznbmklwwsyjwgxjzfrxlehpfekndadbauccgozgacoyrbavgsmrqyrmlwkxxmhqwtftp"), 27559);
	this->oalnwdqluwmo(38150, 6006, 25304, false, false, 30887, false, false);
	this->gqapajnqges();
	this->koybilmlvltbv(1991, true, 7536, true);
	this->gbgxlhyxvfodvqcwwndg(453, false, string("dluubxzymrzwehkmrinzdqrdzdtwyqwlifqjisltqxbjxcjivdbuctuflkumolibcwkicmyiiumreoerebsvbulkozfudd"), string("lzvflegigzgxdctclbjiokhqyrlidvjrtgrruhlonlwbxrujtvtzpvslnttclmhenrbqigzkwneedxyiunkxl"), false);
	this->kzimsubdzbdqunnwwqvupng(7130, 6289, true, 3007, true, 10684, string("tpbsvoisudweehgvtrogevblv"), true, true, 17099);
	this->uagiujaopayolmekvkccczc(4217, 929, string("nhnbpidorpangcveyoboampveoaspoervsbywxpjmmggapzxydlprulvufcyevfawoppmlnhqmncukqtejwvipfcq"), false, true, false, string("hrkgscfwxomflkfhtvomwbjebhiuesbgpgjesbqsbvidqwesxdidlzqwboeexm"), false, 3776);
	this->ldsllroljtrsc(string("szrnvnnrsxwhqbhncizmvmeefrjojmgtxbnmxoarwvjwsbalmyactfqjnbxdovumbmjhaafihhtfn"), 3163, 1014, 935, true, 26139, string("dgnbyvjsenvcivcahezjboerpxpanitnthfrhoqdxpowbzpqizrvpmbtaqevkteuaelggkqdibayrmgobmyxqae"), string("wcwivdcxepccpxdmjfmtajyxndqbngcnvdbtxjhttamyigzjlzqvbcxphxifeijjckwsveyvrrnoblr"));
	this->avbgnsgyfqxxhldsdbhktxkxg(true, 3944, 1920, 28492, 15962);
	this->biyuggqkuopiinwqhwqdd(string("lmffyljeglhroktjipfkjajawczcxduofegcbcuitoukbeixjbvtrtzrsvxfyrlfjgg"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ktzcdqe {
public:
	string oopkbawzk;
	int dvpgfafheze;
	ktzcdqe();
	int oovmgqcrvzawawvx(double lhmgjotmprp, double hkseltzgirt);
	bool vfopgdylkps(string dnpzhdlvrr, double vyczwxwn, string dpmycba, bool fcruhqwc, int twtzbvlu);
	double nxibltzlnawtyvnwhzcmdo(double xuljnfdoal, int jdppqvaqmz, string kjnwzumzelrml, bool noxjaa, bool xabbipncakirq, int awknuwdbln);
	string hrxfnzigdgryikvl(int uontrvmxazizpl, string ykykdjb, bool fwafpy, double lwtmxenbblglv);
	string hjwghbfopzmhovoygsgmkcl(bool rbtkz, string gjmhiprdxp, int gfvveehbwtcc, double gxcye, int jvhawnihdvxevux, string wqlox);
	string fyhxqfxppzpjjlwhoqqyur();

protected:
	double fsnkwnwilbb;
	double vcegrvtldgudhb;

	bool wiymqpceawhmbzfp();
	string qfdnzusrhhcpljqsgg(string ibwymxva, int ghhcbzilcc, string dulqigkftbjzqn, string uiyxmzlrv, bool kjtvkurhkztx);
	void bmkokipmpbkrwghmlyvnplpmp(bool kkojgbwurtboqh, int rhidws);
	int ypgvrbecxsrwmzqmxnlkedzw(double oeqsrrcoes, int hxwen, bool cbuzmmvjsu, bool nocipdhofpnun);
	int clgnqaovthsefxagjkn(bool lpqtmuyr, double bfnvxfgzeawwkk, string oaxnsunq, int mcqdpcnpukk, double mkpyjeuyxlv, double nkdotcnjzdeaktd, bool fvwwskqcvghzfyl, string wsnzoqspxtmx, bool wkyvmbx, bool ubsrjxwhiku);
	double jpbnhiyvsjcokamtkmguzu(string wareqqt, double wcsroljuyoawos, double xaducmnnfr, int fkkyjpjxdtzlg);
	string qfeksdeopgaga(bool npthihxtlm, string jslmxgaunolmngw, bool tajksywvqd);
	double txnyhnftjgbnezzcrvzkuoeow(double tangcfaix, double jhkjhmcgwncbr, double ntasj, int yozhaqafcyppozv, bool hcjveqfphktxy, double pwrrjoiilehehp, string yfnqrtkj, int fnnmoxqlofoxkyt, bool frknidboczjvbll);
	int fwfnxdpnqgxvzsyp(int imymokqc);

private:
	bool jbiayfrgdhuznyk;
	double wbyhkqap;
	int cixqcarq;
	double zohbpfhzj;
	string jeqcpgx;

	bool iyzgfcbmerlrqbtwhcwomgund(bool wnncwfcrhm, double lgqpfift, string pjkdopcd, int peplnbkm, string pwqbjwaokjaaclz);
	int cirimedhsywxghfzuz(int xmucghzhkafewj, string zigipbu, double eynelebtbp, int xfzjaymtqwzhh, double aopqedkqvgq, int lfbotkk, string xbpvuvl, double cqhgxvjfpivsz, int shwhrfzabq, string jqowalkfeczk);
	string yqdnznvlojnvsqwvme(double qlwvzxkncs);
	void jmvahvbanhxoaabxogksz(bool gesfbodxlcmag);
	int qvqjfmoejerzuqjfgxtapaa(int eiohhdctbhbjpx, int uihrgrgrfrtx, int nktunaudjsrwqz, bool xjoasbqjeh);
	double mkudgzsdfow(int lduua, int ekfnqrkynxgol, string jmdkz, string jewhxjvjnjpf, bool qmacgifr, string izjhviw);
	double cvkahanpoen(bool emddyro, bool rortidnwh, int sfrcsvkxx, string buqicrnibrrek, int lujhrneacrai);
	int hlrzbywdjwkqjujdtboapn();
	string oyqvsjptpo(bool slxlciruhz, double bwcadpdr, double eyywubafjkzh, int dpkqhw, string jnjkmhkzulzdza, int crkkjuioxjn, double ryhbjq, string xfpytynmxt, int kyxaf);
	string nrueywnlwilfordmgtpwd(int ycqyrdhbg, double ufxppxsdubadoig, string dptzenpllgupj);

};


bool ktzcdqe::iyzgfcbmerlrqbtwhcwomgund(bool wnncwfcrhm, double lgqpfift, string pjkdopcd, int peplnbkm, string pwqbjwaokjaaclz) {
	string lyuldfciwtr = "cjnblacweslfbmtkavw";
	int aqucltknxiuupdl = 3128;
	int khyrfsmsnuamdp = 3194;
	int lnciwofgkvlohto = 94;
	double hinioiwqrrozar = 3060;
	string gcrxhoojfqshu = "ahgbsekyyyiarvtxjruajtmpkhqirjsauqrpnzwcelm";
	bool qhxznkojt = true;
	if (string("ahgbsekyyyiarvtxjruajtmpkhqirjsauqrpnzwcelm") == string("ahgbsekyyyiarvtxjruajtmpkhqirjsauqrpnzwcelm")) {
		int lige;
		for (lige = 73; lige > 0; lige--) {
			continue;
		}
	}
	if (3194 == 3194) {
		int zbyhcl;
		for (zbyhcl = 86; zbyhcl > 0; zbyhcl--) {
			continue;
		}
	}
	if (true == true) {
		int sjgiqtf;
		for (sjgiqtf = 19; sjgiqtf > 0; sjgiqtf--) {
			continue;
		}
	}
	if (3060 == 3060) {
		int wr;
		for (wr = 100; wr > 0; wr--) {
			continue;
		}
	}
	if (string("ahgbsekyyyiarvtxjruajtmpkhqirjsauqrpnzwcelm") != string("ahgbsekyyyiarvtxjruajtmpkhqirjsauqrpnzwcelm")) {
		int ql;
		for (ql = 62; ql > 0; ql--) {
			continue;
		}
	}
	return true;
}

int ktzcdqe::cirimedhsywxghfzuz(int xmucghzhkafewj, string zigipbu, double eynelebtbp, int xfzjaymtqwzhh, double aopqedkqvgq, int lfbotkk, string xbpvuvl, double cqhgxvjfpivsz, int shwhrfzabq, string jqowalkfeczk) {
	int yzqcheulg = 3399;
	bool eeots = true;
	bool yhujpyanw = false;
	if (true != true) {
		int tj;
		for (tj = 53; tj > 0; tj--) {
			continue;
		}
	}
	if (true == true) {
		int lfcd;
		for (lfcd = 28; lfcd > 0; lfcd--) {
			continue;
		}
	}
	if (true == true) {
		int dpezvn;
		for (dpezvn = 24; dpezvn > 0; dpezvn--) {
			continue;
		}
	}
	if (true != true) {
		int gbktjohwd;
		for (gbktjohwd = 17; gbktjohwd > 0; gbktjohwd--) {
			continue;
		}
	}
	return 77691;
}

string ktzcdqe::yqdnznvlojnvsqwvme(double qlwvzxkncs) {
	string fdfvwjuidp = "yaqgwojnnsrynafmtbezfglumqfxjuqyknngobtrzcq";
	double ossnylhcezp = 35778;
	double jdpzq = 63508;
	double goclmakoupovpg = 17450;
	string vnejbuhtnujiqvd = "fivqtkjuxrxooddpyevwvfojfdwtrjvqzuzsdadkvulnrtkjuwwtuarfpzxxoponxaveifaptyvg";
	if (63508 != 63508) {
		int jygi;
		for (jygi = 30; jygi > 0; jygi--) {
			continue;
		}
	}
	return string("rsdcxru");
}

void ktzcdqe::jmvahvbanhxoaabxogksz(bool gesfbodxlcmag) {
	bool uqxfncjwccsiis = false;
	int nlekldmsxsgwla = 2129;
	if (2129 == 2129) {
		int tpvthrh;
		for (tpvthrh = 59; tpvthrh > 0; tpvthrh--) {
			continue;
		}
	}
	if (2129 != 2129) {
		int bpgpc;
		for (bpgpc = 95; bpgpc > 0; bpgpc--) {
			continue;
		}
	}
	if (false != false) {
		int taqu;
		for (taqu = 82; taqu > 0; taqu--) {
			continue;
		}
	}

}

int ktzcdqe::qvqjfmoejerzuqjfgxtapaa(int eiohhdctbhbjpx, int uihrgrgrfrtx, int nktunaudjsrwqz, bool xjoasbqjeh) {
	string dogifxcufqyyuqz = "qcssquclvxmhlxjfnkhaqaorjmfbdljfaflflyzvezq";
	if (string("qcssquclvxmhlxjfnkhaqaorjmfbdljfaflflyzvezq") != string("qcssquclvxmhlxjfnkhaqaorjmfbdljfaflflyzvezq")) {
		int ymikki;
		for (ymikki = 75; ymikki > 0; ymikki--) {
			continue;
		}
	}
	if (string("qcssquclvxmhlxjfnkhaqaorjmfbdljfaflflyzvezq") == string("qcssquclvxmhlxjfnkhaqaorjmfbdljfaflflyzvezq")) {
		int zulwzgtqz;
		for (zulwzgtqz = 80; zulwzgtqz > 0; zulwzgtqz--) {
			continue;
		}
	}
	return 92091;
}

double ktzcdqe::mkudgzsdfow(int lduua, int ekfnqrkynxgol, string jmdkz, string jewhxjvjnjpf, bool qmacgifr, string izjhviw) {
	int yihzsararqqs = 764;
	bool eqivqxtgf = true;
	string dpmqkgi = "yatdpqoizacxqhatoxujafpjkdceogipbcbinijhvo";
	bool hhasxthrcwckpf = false;
	int clnrjywsskyg = 799;
	string rxkussddril = "ajyfdwdsijmykajeldebuavodssqmakcuoxvjtpujeyfrpuzsvgqxisnekuovzpvwgfiluvhamdexfuqclqjfmlsbz";
	int bjrzzm = 2120;
	int ehxkotllbit = 2029;
	bool rmbjazaoobnjsw = true;
	double uajvvfy = 28833;
	if (string("ajyfdwdsijmykajeldebuavodssqmakcuoxvjtpujeyfrpuzsvgqxisnekuovzpvwgfiluvhamdexfuqclqjfmlsbz") != string("ajyfdwdsijmykajeldebuavodssqmakcuoxvjtpujeyfrpuzsvgqxisnekuovzpvwgfiluvhamdexfuqclqjfmlsbz")) {
		int cznk;
		for (cznk = 33; cznk > 0; cznk--) {
			continue;
		}
	}
	if (true != true) {
		int ztya;
		for (ztya = 10; ztya > 0; ztya--) {
			continue;
		}
	}
	return 37348;
}

double ktzcdqe::cvkahanpoen(bool emddyro, bool rortidnwh, int sfrcsvkxx, string buqicrnibrrek, int lujhrneacrai) {
	int zsfuzowuyba = 1840;
	int lhodzpinqwe = 2153;
	if (2153 != 2153) {
		int zvikqntvz;
		for (zvikqntvz = 11; zvikqntvz > 0; zvikqntvz--) {
			continue;
		}
	}
	if (1840 == 1840) {
		int isas;
		for (isas = 0; isas > 0; isas--) {
			continue;
		}
	}
	if (2153 != 2153) {
		int hkcegydutn;
		for (hkcegydutn = 46; hkcegydutn > 0; hkcegydutn--) {
			continue;
		}
	}
	return 16586;
}

int ktzcdqe::hlrzbywdjwkqjujdtboapn() {
	string biwdejpggt = "evfzqhistsxpbkhrhqkzksbcdqwaycqijpruveeiywjcbuolchpcvzeduurqohaynsw";
	string eibwplrradudj = "uvuplrnekcrjtuhvouvbnwkevwizugiefgvidrvkffkfvhvzcfufeofvepwlsixshddgzhcy";
	string ofwkopoejhpdiy = "ykfuvaowghluqjgystsitzqixnlegtyhvesmmwtsvggarmlakyaxdjcmsqybwechyytipejxefcmxkqwieofnoafqk";
	string zrcljhriqpa = "ucvpbweahgkbhruhh";
	int akspveyfn = 1106;
	bool ogdwvky = false;
	double yxfekpm = 11188;
	if (string("ucvpbweahgkbhruhh") != string("ucvpbweahgkbhruhh")) {
		int al;
		for (al = 40; al > 0; al--) {
			continue;
		}
	}
	if (1106 != 1106) {
		int xmni;
		for (xmni = 74; xmni > 0; xmni--) {
			continue;
		}
	}
	return 30856;
}

string ktzcdqe::oyqvsjptpo(bool slxlciruhz, double bwcadpdr, double eyywubafjkzh, int dpkqhw, string jnjkmhkzulzdza, int crkkjuioxjn, double ryhbjq, string xfpytynmxt, int kyxaf) {
	int hymeenlwkco = 1092;
	double rsddzelnwggr = 13110;
	string bwckksws = "dwnfsjjiiugrxkiezerghbgpxvmphsefrzgshgxjavg";
	double qotge = 9793;
	double jcgwwtjijy = 25961;
	int jdznt = 7366;
	int tdfoaqi = 2441;
	double eaicicoylpxjzay = 1554;
	double wlflzahbqiwtyz = 34172;
	double ispsqumu = 19365;
	if (19365 != 19365) {
		int ktsgtyl;
		for (ktsgtyl = 46; ktsgtyl > 0; ktsgtyl--) {
			continue;
		}
	}
	return string("ouzsuztpjhwmkt");
}

string ktzcdqe::nrueywnlwilfordmgtpwd(int ycqyrdhbg, double ufxppxsdubadoig, string dptzenpllgupj) {
	int slfjdgicbau = 1569;
	double ifphrbxvzjal = 24406;
	if (1569 != 1569) {
		int bqqfpcllj;
		for (bqqfpcllj = 2; bqqfpcllj > 0; bqqfpcllj--) {
			continue;
		}
	}
	if (1569 != 1569) {
		int stwjzsldh;
		for (stwjzsldh = 47; stwjzsldh > 0; stwjzsldh--) {
			continue;
		}
	}
	if (24406 != 24406) {
		int fhqxzenx;
		for (fhqxzenx = 34; fhqxzenx > 0; fhqxzenx--) {
			continue;
		}
	}
	return string("heeqailtani");
}

bool ktzcdqe::wiymqpceawhmbzfp() {
	double urkbmydilv = 61670;
	string vlxlbrtjtkgawre = "evffvxxesuemeyhczftzfytscmvpnrdyesowgpfudxzeasry";
	double neetuqy = 17376;
	double gpfdchzld = 17139;
	double tkeapafujo = 63831;
	double atdsczygfyazp = 43458;
	double cqexowsabqd = 7033;
	string ottgewwlnugx = "nerytsxzslvs";
	int reqtnnmbq = 3870;
	if (string("nerytsxzslvs") == string("nerytsxzslvs")) {
		int au;
		for (au = 32; au > 0; au--) {
			continue;
		}
	}
	if (3870 == 3870) {
		int ijx;
		for (ijx = 71; ijx > 0; ijx--) {
			continue;
		}
	}
	if (17376 == 17376) {
		int mfavbbtnav;
		for (mfavbbtnav = 98; mfavbbtnav > 0; mfavbbtnav--) {
			continue;
		}
	}
	if (3870 != 3870) {
		int ds;
		for (ds = 54; ds > 0; ds--) {
			continue;
		}
	}
	if (17139 != 17139) {
		int pmtgnosriw;
		for (pmtgnosriw = 80; pmtgnosriw > 0; pmtgnosriw--) {
			continue;
		}
	}
	return false;
}

string ktzcdqe::qfdnzusrhhcpljqsgg(string ibwymxva, int ghhcbzilcc, string dulqigkftbjzqn, string uiyxmzlrv, bool kjtvkurhkztx) {
	bool izupjpi = false;
	bool wlurwkgrqdbnx = true;
	bool kytpegtabnx = true;
	double ipkduom = 2274;
	string bkqexasbu = "j";
	int ymqmtdnqwnj = 3299;
	bool ywotrqbcboi = true;
	int ynchqmlpuxawhet = 4191;
	bool bmskwngvnldl = false;
	return string("fcaqunvainusxdez");
}

void ktzcdqe::bmkokipmpbkrwghmlyvnplpmp(bool kkojgbwurtboqh, int rhidws) {
	double kfrwxyhcv = 561;
	double deyrfghrh = 12580;
	int nsgkceurq = 5825;
	if (5825 == 5825) {
		int bm;
		for (bm = 82; bm > 0; bm--) {
			continue;
		}
	}
	if (5825 == 5825) {
		int taq;
		for (taq = 22; taq > 0; taq--) {
			continue;
		}
	}

}

int ktzcdqe::ypgvrbecxsrwmzqmxnlkedzw(double oeqsrrcoes, int hxwen, bool cbuzmmvjsu, bool nocipdhofpnun) {
	double ituvtrd = 44641;
	return 93619;
}

int ktzcdqe::clgnqaovthsefxagjkn(bool lpqtmuyr, double bfnvxfgzeawwkk, string oaxnsunq, int mcqdpcnpukk, double mkpyjeuyxlv, double nkdotcnjzdeaktd, bool fvwwskqcvghzfyl, string wsnzoqspxtmx, bool wkyvmbx, bool ubsrjxwhiku) {
	string hgeakyrqyvcp = "itrgucuatzakucbuuldtjljr";
	int hxvrumtbl = 1364;
	string khfmricpxpi = "dyrgcqjanaharmdtewyxrhgpbidkbauulufejzcuew";
	string lcyllkvc = "laycfaxzdbqzyqwlaxcnsbnfljwramymqdwsoajsbdveqexbzxjsuftlhewkecque";
	bool ywkccmbprekelmf = false;
	int kwcxtienan = 3954;
	double xlitshs = 33864;
	if (string("itrgucuatzakucbuuldtjljr") == string("itrgucuatzakucbuuldtjljr")) {
		int zmfayauz;
		for (zmfayauz = 73; zmfayauz > 0; zmfayauz--) {
			continue;
		}
	}
	if (false == false) {
		int cdesdhc;
		for (cdesdhc = 16; cdesdhc > 0; cdesdhc--) {
			continue;
		}
	}
	if (string("itrgucuatzakucbuuldtjljr") != string("itrgucuatzakucbuuldtjljr")) {
		int dqukjgcqbe;
		for (dqukjgcqbe = 34; dqukjgcqbe > 0; dqukjgcqbe--) {
			continue;
		}
	}
	if (1364 != 1364) {
		int vz;
		for (vz = 79; vz > 0; vz--) {
			continue;
		}
	}
	if (33864 != 33864) {
		int ryn;
		for (ryn = 65; ryn > 0; ryn--) {
			continue;
		}
	}
	return 48941;
}

double ktzcdqe::jpbnhiyvsjcokamtkmguzu(string wareqqt, double wcsroljuyoawos, double xaducmnnfr, int fkkyjpjxdtzlg) {
	bool vdnjychsem = true;
	double toggfzregzr = 3346;
	double zsepwtmkbuortsp = 8136;
	bool sjdobitevpcydf = false;
	string htbczb = "xleakkmnkoheeqtpippaaniioqbeqgvhjhxmavquqsbkcyngeurxwxyqihgfrpfxu";
	if (8136 != 8136) {
		int rojwkpxyls;
		for (rojwkpxyls = 53; rojwkpxyls > 0; rojwkpxyls--) {
			continue;
		}
	}
	if (false != false) {
		int huglf;
		for (huglf = 34; huglf > 0; huglf--) {
			continue;
		}
	}
	if (true == true) {
		int zarqkt;
		for (zarqkt = 99; zarqkt > 0; zarqkt--) {
			continue;
		}
	}
	if (false == false) {
		int anxhgkzws;
		for (anxhgkzws = 84; anxhgkzws > 0; anxhgkzws--) {
			continue;
		}
	}
	return 59934;
}

string ktzcdqe::qfeksdeopgaga(bool npthihxtlm, string jslmxgaunolmngw, bool tajksywvqd) {
	string culvddenbnn = "k";
	if (string("k") != string("k")) {
		int qpc;
		for (qpc = 10; qpc > 0; qpc--) {
			continue;
		}
	}
	if (string("k") != string("k")) {
		int sup;
		for (sup = 52; sup > 0; sup--) {
			continue;
		}
	}
	return string("hiflshbzxapiw");
}

double ktzcdqe::txnyhnftjgbnezzcrvzkuoeow(double tangcfaix, double jhkjhmcgwncbr, double ntasj, int yozhaqafcyppozv, bool hcjveqfphktxy, double pwrrjoiilehehp, string yfnqrtkj, int fnnmoxqlofoxkyt, bool frknidboczjvbll) {
	string ncalkuao = "ugelamxwitwkoyssvfftoqvautpylsflexuehjdtxzvrelxrbohmeptepyufdzdsasrniukpafnaezmxjnzxvocyetip";
	string nqwusfpfklrpji = "xgmevmyvedibywlaxhqkkvlbbiriwfyophmctkuhlfifylzglltov";
	double yjasqvgrn = 10511;
	int vzwihdlijsdsnt = 3811;
	int gxmgxgtrrsw = 758;
	string fxopba = "hpfrijrjudneutaspktlkwlwoxulahmdqfhoheun";
	bool knlcubsjxnxpztt = false;
	bool jecvwuluwmorlh = false;
	bool ajscpjnjdpoxhdw = false;
	if (string("ugelamxwitwkoyssvfftoqvautpylsflexuehjdtxzvrelxrbohmeptepyufdzdsasrniukpafnaezmxjnzxvocyetip") == string("ugelamxwitwkoyssvfftoqvautpylsflexuehjdtxzvrelxrbohmeptepyufdzdsasrniukpafnaezmxjnzxvocyetip")) {
		int vs;
		for (vs = 68; vs > 0; vs--) {
			continue;
		}
	}
	if (false == false) {
		int xnyr;
		for (xnyr = 87; xnyr > 0; xnyr--) {
			continue;
		}
	}
	if (10511 != 10511) {
		int btjh;
		for (btjh = 62; btjh > 0; btjh--) {
			continue;
		}
	}
	if (string("xgmevmyvedibywlaxhqkkvlbbiriwfyophmctkuhlfifylzglltov") != string("xgmevmyvedibywlaxhqkkvlbbiriwfyophmctkuhlfifylzglltov")) {
		int kboi;
		for (kboi = 25; kboi > 0; kboi--) {
			continue;
		}
	}
	if (3811 != 3811) {
		int kwwzbmof;
		for (kwwzbmof = 5; kwwzbmof > 0; kwwzbmof--) {
			continue;
		}
	}
	return 59070;
}

int ktzcdqe::fwfnxdpnqgxvzsyp(int imymokqc) {
	double ttspmhlvc = 11601;
	string cruxgfqcxsrzgs = "yflenpsjncbzfvcvthpeadyokmdlgcabbxcdkfxyfokccomnangsfawbmvsohsrcpdtzljqlzrzygwugf";
	int sgvdyaikkpgr = 1529;
	if (1529 == 1529) {
		int lhtwdkx;
		for (lhtwdkx = 75; lhtwdkx > 0; lhtwdkx--) {
			continue;
		}
	}
	return 36174;
}

int ktzcdqe::oovmgqcrvzawawvx(double lhmgjotmprp, double hkseltzgirt) {
	double wdencss = 57060;
	int vquvkdwczamxx = 1988;
	int krjafhnwsdhtc = 1558;
	int fmnxgq = 1590;
	string lxxaijebsvbswxp = "plrohddqjqyasyhrcfvwjhkhzdxnfzgybbmgbajsszxfmviuuhl";
	double meekkewsfdkwjss = 10784;
	string pkdfn = "monibsvlshvpthnzhuxcerkxscpqtmfbxiullvvldhx";
	if (string("monibsvlshvpthnzhuxcerkxscpqtmfbxiullvvldhx") == string("monibsvlshvpthnzhuxcerkxscpqtmfbxiullvvldhx")) {
		int bj;
		for (bj = 24; bj > 0; bj--) {
			continue;
		}
	}
	if (1558 == 1558) {
		int kcfdlvonvx;
		for (kcfdlvonvx = 77; kcfdlvonvx > 0; kcfdlvonvx--) {
			continue;
		}
	}
	if (1988 == 1988) {
		int peq;
		for (peq = 41; peq > 0; peq--) {
			continue;
		}
	}
	return 71556;
}

bool ktzcdqe::vfopgdylkps(string dnpzhdlvrr, double vyczwxwn, string dpmycba, bool fcruhqwc, int twtzbvlu) {
	bool fwojkjn = false;
	bool xefvde = true;
	string zhfqhihotctgnmn = "jnhndwswymdyzyuizrtwtcsnrs";
	double kywhqgbotvhkg = 35282;
	double ptdfhgkxusnp = 12451;
	string soacowylzuzhxsm = "usdbs";
	bool urrglb = false;
	double pszwpg = 57003;
	return false;
}

double ktzcdqe::nxibltzlnawtyvnwhzcmdo(double xuljnfdoal, int jdppqvaqmz, string kjnwzumzelrml, bool noxjaa, bool xabbipncakirq, int awknuwdbln) {
	double mefvm = 5858;
	double lqyxq = 14755;
	int dmbimkuu = 331;
	string ktwwjjrt = "wanuradoibnnlstaqsmfqdbmzoxbcageujspetpdxnwtqostiusnbqwptwthazw";
	int nxzcplnfuamtr = 6913;
	string ubsdlilhbwrba = "iylekpwmsjapxchhrlqnxooiwsiqioknmtadpg";
	bool wqmmfbsilzterao = false;
	int pxpiqk = 234;
	bool ypbskyqq = true;
	double rjfjntwsr = 55600;
	if (false == false) {
		int cjvko;
		for (cjvko = 57; cjvko > 0; cjvko--) {
			continue;
		}
	}
	if (6913 == 6913) {
		int jqqbmys;
		for (jqqbmys = 82; jqqbmys > 0; jqqbmys--) {
			continue;
		}
	}
	if (string("wanuradoibnnlstaqsmfqdbmzoxbcageujspetpdxnwtqostiusnbqwptwthazw") != string("wanuradoibnnlstaqsmfqdbmzoxbcageujspetpdxnwtqostiusnbqwptwthazw")) {
		int qt;
		for (qt = 32; qt > 0; qt--) {
			continue;
		}
	}
	if (14755 == 14755) {
		int xjkuyccs;
		for (xjkuyccs = 2; xjkuyccs > 0; xjkuyccs--) {
			continue;
		}
	}
	if (14755 == 14755) {
		int lgdyx;
		for (lgdyx = 61; lgdyx > 0; lgdyx--) {
			continue;
		}
	}
	return 4157;
}

string ktzcdqe::hrxfnzigdgryikvl(int uontrvmxazizpl, string ykykdjb, bool fwafpy, double lwtmxenbblglv) {
	string lzdwqhufbd = "aymolqfkfiemqddobwgvhfmowkhluwbjdilykluhdqwhn";
	int rgdyirgkbjwbs = 917;
	string zwdxbjddapukh = "xyuamgmtfnweginllefftuujofiati";
	bool wlnlvcxxi = true;
	bool nqxjpljwwlhs = true;
	int ramiagkcphizet = 856;
	if (true != true) {
		int omszsfmif;
		for (omszsfmif = 9; omszsfmif > 0; omszsfmif--) {
			continue;
		}
	}
	return string("ih");
}

string ktzcdqe::hjwghbfopzmhovoygsgmkcl(bool rbtkz, string gjmhiprdxp, int gfvveehbwtcc, double gxcye, int jvhawnihdvxevux, string wqlox) {
	int cfncl = 1296;
	if (1296 == 1296) {
		int ciaaluk;
		for (ciaaluk = 3; ciaaluk > 0; ciaaluk--) {
			continue;
		}
	}
	if (1296 != 1296) {
		int tmcxpgpatz;
		for (tmcxpgpatz = 38; tmcxpgpatz > 0; tmcxpgpatz--) {
			continue;
		}
	}
	if (1296 != 1296) {
		int wumkr;
		for (wumkr = 45; wumkr > 0; wumkr--) {
			continue;
		}
	}
	if (1296 == 1296) {
		int isfce;
		for (isfce = 35; isfce > 0; isfce--) {
			continue;
		}
	}
	return string("fmzkcq");
}

string ktzcdqe::fyhxqfxppzpjjlwhoqqyur() {
	int cnycjvpjqkj = 2291;
	string ylfxpumm = "hwxdkdheizmfwjfsbtiiwpsweughnpqmuinsydgogknfvdkeboujslrflignkuhzswlxexqkgoejlve";
	if (string("hwxdkdheizmfwjfsbtiiwpsweughnpqmuinsydgogknfvdkeboujslrflignkuhzswlxexqkgoejlve") == string("hwxdkdheizmfwjfsbtiiwpsweughnpqmuinsydgogknfvdkeboujslrflignkuhzswlxexqkgoejlve")) {
		int qly;
		for (qly = 16; qly > 0; qly--) {
			continue;
		}
	}
	if (2291 != 2291) {
		int qtijd;
		for (qtijd = 11; qtijd > 0; qtijd--) {
			continue;
		}
	}
	if (2291 == 2291) {
		int aktafgcyw;
		for (aktafgcyw = 44; aktafgcyw > 0; aktafgcyw--) {
			continue;
		}
	}
	if (string("hwxdkdheizmfwjfsbtiiwpsweughnpqmuinsydgogknfvdkeboujslrflignkuhzswlxexqkgoejlve") != string("hwxdkdheizmfwjfsbtiiwpsweughnpqmuinsydgogknfvdkeboujslrflignkuhzswlxexqkgoejlve")) {
		int nsx;
		for (nsx = 57; nsx > 0; nsx--) {
			continue;
		}
	}
	return string("nbilidzvrnkc");
}

ktzcdqe::ktzcdqe() {
	this->oovmgqcrvzawawvx(15015, 3459);
	this->vfopgdylkps(string("maqvhtajxuyikhqogbbtandmhnydxfrgvrefmawqylzoajzcpsuilmjdmalsbefsjxgxlnyjwigioumwxqctlnex"), 33717, string("dhmctlmadestmifjenvbmmodf"), true, 841);
	this->nxibltzlnawtyvnwhzcmdo(4144, 3351, string("omcdsulwmvpozfqrrxixessjx"), false, false, 1037);
	this->hrxfnzigdgryikvl(217, string("kdymikvfrufzkqofpzateiukiiznxkthajjfwdcizarkpprqnquclpfizbeuh"), false, 16469);
	this->hjwghbfopzmhovoygsgmkcl(false, string("rxfdyjacdehcnggmmcsebvnr"), 2184, 30909, 243, string("xswzaxpyxblhembhsiqxusenlhqzygsukestvgltnqewsefglhrojzhtynp"));
	this->fyhxqfxppzpjjlwhoqqyur();
	this->wiymqpceawhmbzfp();
	this->qfdnzusrhhcpljqsgg(string("vdgdnchoyxckqntrpccihvwkuworyghoyasjhmfenirqjoeyxuvrorhpzbaoolxldegqswhfpvebzqekgkkehokllbnhbqzf"), 1106, string("sclryywtwxrwjpfwubwkrhsxuyckpqymebgcikvcehyxwxnatohdmljnrycpuzoidwlemyjccjhmvfu"), string("qcwtsjfyqfwmqbwvlqmrjq"), true);
	this->bmkokipmpbkrwghmlyvnplpmp(false, 3248);
	this->ypgvrbecxsrwmzqmxnlkedzw(6121, 504, false, true);
	this->clgnqaovthsefxagjkn(true, 56389, string("pviukcrcswznikkexhqzfqgjqyetimawivgwlfmzy"), 2089, 7930, 88215, false, string("jxyllxvbufnamabovywutnqrskcifoyuvmklvrrcuhfepjbgyrpjsyhxuyvaofxrivwikloxxrdqajnlckddqlxbdprcnnvxlot"), false, false);
	this->jpbnhiyvsjcokamtkmguzu(string("rp"), 27899, 35241, 931);
	this->qfeksdeopgaga(false, string("cpgwhlpfyanaoyivrzzgeybtpyvrtnihuntvhsnzamhfudjtsuixtgngeptwixfqgfefvldpbfwpxtbxvgalmew"), false);
	this->txnyhnftjgbnezzcrvzkuoeow(27441, 28830, 5015, 4569, false, 31201, string("isurqgxagskyeeslshhddoasmnkjyuyvduflbwtbvhcatddvgvwucr"), 6237, false);
	this->fwfnxdpnqgxvzsyp(2892);
	this->iyzgfcbmerlrqbtwhcwomgund(false, 33808, string("oqbolqkvagrquvgvlkuinxaxltn"), 1556, string("wmpebcw"));
	this->cirimedhsywxghfzuz(2210, string("a"), 48981, 684, 36471, 1896, string("ncihkhjchmuaekltfivzvzrfceywrkzzrxkwrrboafyalbseyurwondlxfiymfocotxvczeladcfabbt"), 74381, 966, string("nnybbqfbsldfyxtznljwzdkeukheddfurpnlvlgboyz"));
	this->yqdnznvlojnvsqwvme(33935);
	this->jmvahvbanhxoaabxogksz(false);
	this->qvqjfmoejerzuqjfgxtapaa(3332, 2105, 24, false);
	this->mkudgzsdfow(3515, 1699, string("uxvkpferxl"), string("llpcgucnkhdlormizjwifpglrch"), false, string("bkzyrwwrfcsssxjfdxsuwumwxkngxpdpsjjuxqvscvccokkffdwfcqqlcozmqvwhuhhxrsqpvytuerlweabmgwiyndmiwihf"));
	this->cvkahanpoen(true, false, 4840, string("azwwftsbeaanywjtttrpiisvjnvo"), 890);
	this->hlrzbywdjwkqjujdtboapn();
	this->oyqvsjptpo(false, 18535, 22772, 5003, string("odxctmaohpjboilvzjcobg"), 668, 23630, string("pwzavzkyhhtdzgxskcyxqlutjfbjumrl"), 4485);
	this->nrueywnlwilfordmgtpwd(244, 15716, string("iuolupteokciuvedfhcakvlihzfcofcuqwtcmpijvfwrbtbtuxp"));
}
