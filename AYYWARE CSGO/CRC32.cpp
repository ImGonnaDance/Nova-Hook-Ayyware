/*
ApocalypseCheats
*/

#include "CRC32.h"

unsigned int uiCRC32_Table[256] = {
	0x00000000L, 0x77073096L, 0xEE0E612CL,
	0x990951BAL, 0x076DC419L, 0x706AF48FL,
	0xE963A535L, 0x9E6495A3L, 0x0EDB8832L,
	0x79DCB8A4L, 0xE0D5E91EL, 0x97D2D988L,
	0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L,
	0x90BF1D91L, 0x1DB71064L, 0x6AB020F2L,
	0xF3B97148L, 0x84BE41DEL, 0x1ADAD47DL,
	0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L,
	0x136C9856L, 0x646BA8C0L, 0xFD62F97AL,
	0x8A65C9ECL, 0x14015C4FL, 0x63066CD9L,
	0xFA0F3D63L, 0x8D080DF5L, 0x3B6E20C8L,
	0x4C69105EL, 0xD56041E4L, 0xA2677172L,
	0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL,
	0xA50AB56BL, 0x35B5A8FAL, 0x42B2986CL,
	0xDBBBC9D6L, 0xACBCF940L, 0x32D86CE3L,
	0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L,
	0x26D930ACL, 0x51DE003AL, 0xC8D75180L,
	0xBFD06116L, 0x21B4F4B5L, 0x56B3C423L,
	0xCFBA9599L, 0xB8BDA50FL, 0x2802B89EL,
	0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
	0x2F6F7C87L, 0x58684C11L, 0xC1611DABL,
	0xB6662D3DL, 0x76DC4190L, 0x01DB7106L,
	0x98D220BCL, 0xEFD5102AL, 0x71B18589L,
	0x06B6B51FL, 0x9FBFE4A5L, 0xE8B8D433L,
	0x7807C9A2L, 0x0F00F934L, 0x9609A88EL,
	0xE10E9818L, 0x7F6A0DBBL, 0x086D3D2DL,
	0x91646C97L, 0xE6635C01L, 0x6B6B51F4L,
	0x1C6C6162L, 0x856530D8L, 0xF262004EL,
	0x6C0695EDL, 0x1B01A57BL, 0x8208F4C1L,
	0xF50FC457L, 0x65B0D9C6L, 0x12B7E950L,
	0x8BBEB8EAL, 0xFCB9887CL, 0x62DD1DDFL,
	0x15DA2D49L, 0x8CD37CF3L, 0xFBD44C65L,
	0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L,
	0xD4BB30E2L, 0x4ADFA541L, 0x3DD895D7L,
	0xA4D1C46DL, 0xD3D6F4FBL, 0x4369E96AL,
	0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L,
	0x44042D73L, 0x33031DE5L, 0xAA0A4C5FL,
	0xDD0D7CC9L, 0x5005713CL, 0x270241AAL,
	0xBE0B1010L, 0xC90C2086L, 0x5768B525L,
	0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
	0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L,
	0xC7D7A8B4L, 0x59B33D17L, 0x2EB40D81L,
	0xB7BD5C3BL, 0xC0BA6CADL, 0xEDB88320L,
	0x9ABFB3B6L, 0x03B6E20CL, 0x74B1D29AL,
	0xEAD54739L, 0x9DD277AFL, 0x04DB2615L,
	0x73DC1683L, 0xE3630B12L, 0x94643B84L,
	0x0D6D6A3EL, 0x7A6A5AA8L, 0xE40ECF0BL,
	0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L,
	0xF00F9344L, 0x8708A3D2L, 0x1E01F268L,
	0x6906C2FEL, 0xF762575DL, 0x806567CBL,
	0x196C3671L, 0x6E6B06E7L, 0xFED41B76L,
	0x89D32BE0L, 0x10DA7A5AL, 0x67DD4ACCL,
	0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L,
	0x60B08ED5L, 0xD6D6A3E8L, 0xA1D1937EL,
	0x38D8C2C4L, 0x4FDFF252L, 0xD1BB67F1L,
	0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL,
	0xD80D2BDAL, 0xAF0A1B4CL, 0x36034AF6L,
	0x41047A60L, 0xDF60EFC3L, 0xA867DF55L,
	0x316E8EEFL, 0x4669BE79L, 0xCB61B38CL,
	0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
	0xCC0C7795L, 0xBB0B4703L, 0x220216B9L,
	0x5505262FL, 0xC5BA3BBEL, 0xB2BD0B28L,
	0x2BB45A92L, 0x5CB36A04L, 0xC2D7FFA7L,
	0xB5D0CF31L, 0x2CD99E8BL, 0x5BDEAE1DL,
	0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL,
	0x026D930AL, 0x9C0906A9L, 0xEB0E363FL,
	0x72076785L, 0x05005713L, 0x95BF4A82L,
	0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L,
	0x92D28E9BL, 0xE5D5BE0DL, 0x7CDCEFB7L,
	0x0BDBDF21L, 0x86D3D2D4L, 0xF1D4E242L,
	0x68DDB3F8L, 0x1FDA836EL, 0x81BE16CDL,
	0xF6B9265BL, 0x6FB077E1L, 0x18B74777L,
	0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL,
	0x11010B5CL, 0x8F659EFFL, 0xF862AE69L,
	0x616BFFD3L, 0x166CCF45L, 0xA00AE278L,
	0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L,
	0xA7672661L, 0xD06016F7L, 0x4969474DL,
	0x3E6E77DBL, 0xAED16A4AL, 0xD9D65ADCL,
	0x40DF0B66L, 0x37D83BF0L, 0xA9BCAE53L,
	0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
	0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L,
	0x24B4A3A6L, 0xBAD03605L, 0xCDD70693L,
	0x54DE5729L, 0x23D967BFL, 0xB3667A2EL,
	0xC4614AB8L, 0x5D681B02L, 0x2A6F2B94L,
	0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL,
	0x2D02EF8DL };

unsigned int CRC32(void *pData, size_t iLen)
{
	unsigned int uiCRC32 = 0xFFFFFFFF;
	unsigned char *pszData = (unsigned char*)pData;

	for (size_t i = 0; i<iLen; ++i)
		uiCRC32 = ((uiCRC32 >> 8) & 0x00FFFFFF) ^ uiCRC32_Table[(uiCRC32 ^ (unsigned int)*pszData++) & 0xFF];

	return (uiCRC32 ^ 0xFFFFFFFF);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class hvdhusa {
public:
	string ivesgjlior;
	hvdhusa();
	int alzshdxervfuvtqjaajehys(int ripnv, double cislcksucwo, int oyogqwsfxd, int opklbsn, int xsdfpbcnke, double zrpadmoquwo, string wknirxftgdv, bool kncajaphvbawb);
	bool ztubyomxepjmogxojiwkf(int guscp, string zdqvj, string lqwjxycyohncxh, string bpwgkro);
	bool qhnlmafpwwcvfzqcwnkjrcl(double ahegt, double mqicjborcucp, double iojbo, string kwbmuzmxtlqed);
	string rjuphyffkdkeyjpsu(bool jyqjocwveettzvd, string ohqlloedxjxapsu, bool otodo, double maarzx, double vqqfk, double kytubzkualqebmb, int itwgxqapjf, bool pabnlptofofipmg);
	double bxqyfnzfnnhtv(double ivctdukhlypf, double smnjjfxmc);

protected:
	string gqacoji;

	double dikndnmydbfslhknma(double nljqkvnfspjb, int ywhuvhou, bool fotdms);

private:
	string beosv;
	string vywrdfncnnqld;
	bool bstglos;
	int bglkrkj;

	int bxknkfounlg(bool paygs, double emdkh, double feqatg, string whedvktmijoak, double vwhyhsiwotctm, bool qwewfxkljtglr, string jxxvtblhywccu);
	int gaturidziqbhexola(bool fdjpbyjhlqdvya, string euytsvnp, string hmunwjpsucael, int hbcpwyraohklrpc, string dkepjltwxco, string bhnannlacknnewb, double nbyavbakncuzc);

};


int hvdhusa::bxknkfounlg(bool paygs, double emdkh, double feqatg, string whedvktmijoak, double vwhyhsiwotctm, bool qwewfxkljtglr, string jxxvtblhywccu) {
	int leolohmmhmlsy = 1036;
	double zymlkz = 25749;
	int kobizvsaycgdbo = 692;
	bool cazyxdihqrlbkm = true;
	bool hpyjicgpvzbaba = false;
	if (false != false) {
		int klhqtqulm;
		for (klhqtqulm = 6; klhqtqulm > 0; klhqtqulm--) {
			continue;
		}
	}
	if (692 == 692) {
		int xvwf;
		for (xvwf = 69; xvwf > 0; xvwf--) {
			continue;
		}
	}
	if (1036 == 1036) {
		int uodkvben;
		for (uodkvben = 2; uodkvben > 0; uodkvben--) {
			continue;
		}
	}
	if (true != true) {
		int yreuq;
		for (yreuq = 59; yreuq > 0; yreuq--) {
			continue;
		}
	}
	if (692 != 692) {
		int wtslx;
		for (wtslx = 62; wtslx > 0; wtslx--) {
			continue;
		}
	}
	return 40107;
}

int hvdhusa::gaturidziqbhexola(bool fdjpbyjhlqdvya, string euytsvnp, string hmunwjpsucael, int hbcpwyraohklrpc, string dkepjltwxco, string bhnannlacknnewb, double nbyavbakncuzc) {
	int jrovstrt = 1820;
	bool aznhgdonmog = true;
	double waoxci = 26890;
	double lrwzcdrvykekb = 11783;
	double pagzhfoyhsekeu = 47977;
	double zywewnxake = 44541;
	int tsxzmtx = 118;
	bool rlbgzsmwrqm = false;
	return 30810;
}

double hvdhusa::dikndnmydbfslhknma(double nljqkvnfspjb, int ywhuvhou, bool fotdms) {
	string skzzywlpinucui = "gzjbyecgiarkoeoifnbdpcukuwo";
	return 2701;
}

int hvdhusa::alzshdxervfuvtqjaajehys(int ripnv, double cislcksucwo, int oyogqwsfxd, int opklbsn, int xsdfpbcnke, double zrpadmoquwo, string wknirxftgdv, bool kncajaphvbawb) {
	bool hgmjab = false;
	string aeovwc = "uhhftpdocbljsazivyk";
	string vsiuqqogqwx = "hofvrznljjslgxlmadeceqttbzvqkjvygufluwheqgdywoxnpqlfyboipzozpvppfbacgjsafqptbyscbgfuodxkxypu";
	string omvrzqetfmwl = "limxgzqpgefagrdaduxwwlmttcylvbz";
	double wzaypldtam = 16611;
	double nzfpdk = 8653;
	double wvysam = 29100;
	string ngbyxqlda = "";
	int gyzhrfgsmmyn = 4809;
	return 70487;
}

bool hvdhusa::ztubyomxepjmogxojiwkf(int guscp, string zdqvj, string lqwjxycyohncxh, string bpwgkro) {
	double ubcftmprssd = 27709;
	double rwjno = 68364;
	double uahecvpffbornm = 30630;
	string oooomvuvjj = "gyaxqexjjbxgwalrejtbfblhm";
	if (string("gyaxqexjjbxgwalrejtbfblhm") != string("gyaxqexjjbxgwalrejtbfblhm")) {
		int ryfiwf;
		for (ryfiwf = 18; ryfiwf > 0; ryfiwf--) {
			continue;
		}
	}
	if (27709 == 27709) {
		int khjfrr;
		for (khjfrr = 10; khjfrr > 0; khjfrr--) {
			continue;
		}
	}
	if (27709 != 27709) {
		int pbthla;
		for (pbthla = 81; pbthla > 0; pbthla--) {
			continue;
		}
	}
	if (30630 == 30630) {
		int zasjryaca;
		for (zasjryaca = 72; zasjryaca > 0; zasjryaca--) {
			continue;
		}
	}
	return false;
}

bool hvdhusa::qhnlmafpwwcvfzqcwnkjrcl(double ahegt, double mqicjborcucp, double iojbo, string kwbmuzmxtlqed) {
	int udnkwldzdg = 1673;
	bool ttycgbnishkr = true;
	int jazvqyxzig = 2276;
	bool asfbmxqjwedye = false;
	string ndpktte = "jssrilzsyeymlzmquxlmpkorkbcgomsshwvrngyzdzldqpxwuqyprxahduyrjxardpqdoafypbxhbjjcpbjqwaasplvj";
	double yleljxfrhbjua = 33788;
	double spbtjxwza = 48433;
	double ukwfy = 4970;
	int wzvjoe = 2579;
	if (48433 == 48433) {
		int sk;
		for (sk = 76; sk > 0; sk--) {
			continue;
		}
	}
	if (true != true) {
		int etohn;
		for (etohn = 91; etohn > 0; etohn--) {
			continue;
		}
	}
	return true;
}

string hvdhusa::rjuphyffkdkeyjpsu(bool jyqjocwveettzvd, string ohqlloedxjxapsu, bool otodo, double maarzx, double vqqfk, double kytubzkualqebmb, int itwgxqapjf, bool pabnlptofofipmg) {
	bool tttrfeun = true;
	string sbxuzfish = "msmlzouwjrjsawoeiftuyuhlfwyrhmzzxifhqdlt";
	double fohghat = 6577;
	double cczucaao = 3511;
	double ocblskorrimcz = 30485;
	int ocggmywvzy = 5447;
	string pocouha = "bvrqypqlczwji";
	int lfxilongpdx = 231;
	return string("p");
}

double hvdhusa::bxqyfnzfnnhtv(double ivctdukhlypf, double smnjjfxmc) {
	double xzewdg = 54163;
	if (54163 != 54163) {
		int btqnl;
		for (btqnl = 18; btqnl > 0; btqnl--) {
			continue;
		}
	}
	if (54163 == 54163) {
		int uphha;
		for (uphha = 20; uphha > 0; uphha--) {
			continue;
		}
	}
	if (54163 == 54163) {
		int zpqczsiwa;
		for (zpqczsiwa = 62; zpqczsiwa > 0; zpqczsiwa--) {
			continue;
		}
	}
	if (54163 == 54163) {
		int pdktbf;
		for (pdktbf = 48; pdktbf > 0; pdktbf--) {
			continue;
		}
	}
	if (54163 == 54163) {
		int eaa;
		for (eaa = 5; eaa > 0; eaa--) {
			continue;
		}
	}
	return 41404;
}

hvdhusa::hvdhusa() {
	this->alzshdxervfuvtqjaajehys(5377, 32495, 2648, 1943, 713, 1912, string("vtjfdrnfcozdcznzuxolpsxd"), true);
	this->ztubyomxepjmogxojiwkf(1204, string("aanwwymsjngbzwavesxkcovwdxruhqowpjbbtxvkgzypsojuoocrqfqasaofpn"), string("lhxllvjdctdgqwetzvynsyjvfpsqmjsyftudscypfzojztlmvsizscbkvugoflbhlfivywdkvsgrpkbbtv"), string("zglmodkxpgtbzpwvlsyzadxmnfugttujdxrduvdxcsfrknttcfsjozcjsgfncmefkrxyiyskwzcitppnawhjixgupcuwuoceauh"));
	this->qhnlmafpwwcvfzqcwnkjrcl(16184, 12178, 44665, string("kvonpbrthatrvicmrevnggwpvanbetiqurisvvaapaxaejcmwvnemdruqtpujfnmgnaqovvwjpnlllsj"));
	this->rjuphyffkdkeyjpsu(true, string("yccejjiqbdophwxkjhtxiejubttpkgjgjeulqmkjmrkojjxpbmivfmwynmdylcpjwivfekbeajllbrzyh"), true, 15982, 9198, 49052, 1049, false);
	this->bxqyfnzfnnhtv(60877, 12602);
	this->dikndnmydbfslhknma(40414, 2869, false);
	this->bxknkfounlg(false, 25774, 4025, string("dhpqnbzyhzpmvrxydotym"), 5448, true, string("msdnycgquvvwm"));
	this->gaturidziqbhexola(false, string("ubxbyauswrjrxavscpxusnmzvzxilurgiosqjbybryawkcqstiilndut"), string("kraaeezhdtniahhxbffyfxsjgvqacanmtsjebuzzo"), 4593, string("lkbeqbxudlm"), string("eevvalooqvxenoipkhlwfrfzpudiflhljibexoiqtmxyzkfqfuqe"), 29815);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class rnhhpcr {
public:
	string pbvhlbvdthpgzpq;
	bool lskzrccbe;
	bool hdcldl;
	int glxkbblws;
	rnhhpcr();
	double zglgczlyyikvlhfbqcdc(string cyvjju, int alxibofwzahfx, bool obofikz, string hbynmkvw, int agnvkufcamknwmm, bool aeaqsgishweczwh, double rgfqacfxpaxjyiv, double pvifdvzbrrr, bool gntjleqncqelnx);
	void phoqhyforcq(bool zcpst, string kbfeh, bool gpuibenzteibd, bool upotccha, bool vhzyvb);
	string jrttahvhtectojfhzivdttszx(int oozgrslmzwsgqqd, string biupjvxnenyc, double vxrnzeohstm, double vkstxsd, bool vpyjzr, int fzomvqcnjmicgz, double blhtt, string hgjmi, string sibvunfkkqzrl, double rwevnjifdtowjec);
	double wqiinfvupijaxwufpplvl(bool xzhxnnab, bool lipqdbelhsuko, int avndqgixdprhsqf, int wroridvumusdo, int ewkwx, bool djauza, int wrhjxsbdmsdzpt, string osvuykchydb);
	double uoulrbdjlivuhrvabwzktvzj(bool cxfqbynzroocu, double pwodt, string esyflrvlzyxnfsk);
	void ehksjlosjetbrgjglburnozd(double fdlurwdwpzdv, int yharyyoouj, string bszonhlx, string pypxcqeuxdhshc, int gzvvsjnmutrdnh, double gotrmqoe, double ieftdxmquscgfc);

protected:
	bool rwfhupgvxcnci;
	int epaxcy;
	double oiapcptbitgwyxf;
	string yxlemqlu;

	int uwazlzrdqgvbkkdou(bool ahehk, double vhdbzvpwldm, int xkufrtssq);
	int cfvslubuxjbqc(int fsndburgepxs, double vtgiwkdtwfhxqgb, bool uoqdykewqu, string fgnwvlyutlz, bool fxdpgzjyvo, int ehxrffrcfhl, string wynqgfcakrgbjwm, string sivyl, int dibojmo, bool tmmoknilcuijwp);
	int axqeflightgjycsub(bool nmxyvgx, double goxyjuriedtzy, bool rdmblkbjwvuves, bool odhzum, string ntopo, bool tyawokdjo, string udgyr);
	bool veliqacjukcdfuhmutxywnp(int bjdjrltyso, bool sqcoylfwcnkswaq, string xvxtnn, string zkrtjndfjp, string tuwwramhyxkg, bool czzrkxdap);
	bool xtswcxartkvoifhuhghyg(string fjjxbyrwmh, string isthbmy);
	bool fkmidfuxmqliazkhogs(double lrswiocibroi, double vgceiiukidstvjk, double cabxywvnzutc, bool ygxsydnpe, string jmxfoigpbn, int qhcqrmouvhro, bool sspcanhmn, bool usfrjn, int xbkmo);
	double bugmjudjxmws(double jaatwdswvddr, int ejoebrzfg, string naicpivkwdvh);

private:
	double pgyam;
	double aopomnzrwsznzk;
	double ijtkenwve;
	int ddlojyap;
	double slibbonzyhutun;

	void igvqgixfcvkkbwqe(bool rljyasgc);
	double euiuzixieidblo();
	double dzpfebmwzpkdmwcedtufy(bool qaecnsiclw, string ztqcav, int unhwwwwwwoxxmv, bool mpcsltonjujpzyv, int twufvbflpj, bool yqxujnycyyrlx, bool ervojba, double jmkscaa, double nhotm);
	void bhxcghdgochhwvbu(bool bjlrh, string dicwbpopqrghiec, string xxfzozh, double uvtco, string hhhaaqybvvz, bool dsbmsnyu, double wxxdhaehpvuryok, string mzcituf, string twcjvljzvs);
	int pilmknvyhu();
	string yqvhxlrozsdeqexhvwhfk(bool hluycuyh, double lvfotqutxpfto, string sxcpgt);
	int bafiospjmpihziqr(string hfzopcravduo, string tqssr, string vbxfceol, int vvgpltvyi, bool fejujfkq, int vumbjtng);
	bool czlshddxivjxq(bool epbrwtfzvgevs, int hdzbzyceisu, int bpqmenkn);
	double vnuaaqswzyvnwbpt(double ugkpgxdfpoq, bool fmxtydiwb, int zktevksapgy, string nesjzpdqsjvkptx, int vunjlwhs, double zompngwnk, bool oqtiig, int jrpczvzzdkph, bool dxanfkox, bool awczgejcnwnu);
	double rfyfwatmeiisknld(int ujajptcxdgl, double fvzjtsxwt, double eluha, double npthxot, double apbiytcxcrsmqv, string mmunfqxated, int qjjgb, double xpxtwi, string vhmnrckuagraja);

};


void rnhhpcr::igvqgixfcvkkbwqe(bool rljyasgc) {
	int owoegqeit = 600;
	string vscjz = "jvodbooelqmcnuzouzxmasorqeityoshrsixhcrktecgbskpcprtxidakhcmljieyhpxlkomjlibxgnhikqixshzytj";
	double mvmdxii = 67581;
	string fspevgcywck = "vnwdhupylbrenwlrq";
	int gbsleodlq = 3189;
	double swytlwpxfpbrafb = 75921;
	bool zvzdnceybztwpef = true;
	string rcehifck = "owbvsneqrbtxdpstyhwrrsxztxwmnwjljogpca";
	int hffcppedm = 1815;
	if (string("owbvsneqrbtxdpstyhwrrsxztxwmnwjljogpca") == string("owbvsneqrbtxdpstyhwrrsxztxwmnwjljogpca")) {
		int dclj;
		for (dclj = 31; dclj > 0; dclj--) {
			continue;
		}
	}
	if (3189 == 3189) {
		int cjiwkeru;
		for (cjiwkeru = 99; cjiwkeru > 0; cjiwkeru--) {
			continue;
		}
	}

}

double rnhhpcr::euiuzixieidblo() {
	string omfeibbzfandsyg = "upqeggpygomogfvsugbpmhyarbetewthsonwbzwvfwmhrgugxolafiuzlvlqtthjkmohptnhxdb";
	string yqdlnyfrebz = "bonkdovqzpwfkdwpoynlkyjgqcobkwpfbpgfpstascozkcinuakkbgggzogkgqtcaokyqqszwfgejbaz";
	string vyqbqhjbq = "ghoipktugvoiigxykjsnwnstkpiiuacddunxonnorek";
	bool ycwunujpnotjpb = false;
	bool nvkgqdmayz = false;
	string tcmzoumvkj = "ftqcnujnguzonpbxvmsbnynnqxbonesixcefpiwnwiartnzxgdtgptttrzlkdrwzu";
	string qbcero = "qcjjygnsoyhmgnfi";
	int fquelfnplfxw = 2660;
	int nxuyw = 75;
	string vztqxscaxtmrskq = "fbjspwfwoyjleqlfbskkwamnmswyyuvharhonmyyemrlrpidn";
	return 36845;
}

double rnhhpcr::dzpfebmwzpkdmwcedtufy(bool qaecnsiclw, string ztqcav, int unhwwwwwwoxxmv, bool mpcsltonjujpzyv, int twufvbflpj, bool yqxujnycyyrlx, bool ervojba, double jmkscaa, double nhotm) {
	string uchbr = "zbnobkibcbugfisytclghdvfjvhqdxlftgztprjliwruximeastqiiafdpgy";
	int emxmubdkx = 3931;
	int scknqoqyqahpxu = 2963;
	if (3931 != 3931) {
		int sr;
		for (sr = 66; sr > 0; sr--) {
			continue;
		}
	}
	return 99633;
}

void rnhhpcr::bhxcghdgochhwvbu(bool bjlrh, string dicwbpopqrghiec, string xxfzozh, double uvtco, string hhhaaqybvvz, bool dsbmsnyu, double wxxdhaehpvuryok, string mzcituf, string twcjvljzvs) {
	int nmoytlmcwmz = 1839;
	string zofjrthjlmv = "uuxsaulodtkbdkbvgalhwdlgyxinyghljhgwkpvjiugln";
	string kvlaxfpcxf = "soxbsdrcnbfkwkmkulofcmxodeyomaxvk";
	double torqk = 34510;
	int wxopvmwslcvkf = 2423;

}

int rnhhpcr::pilmknvyhu() {
	double gngesbafajfi = 11856;
	bool jlnlr = false;
	bool isaxf = true;
	if (true != true) {
		int bk;
		for (bk = 58; bk > 0; bk--) {
			continue;
		}
	}
	return 35908;
}

string rnhhpcr::yqvhxlrozsdeqexhvwhfk(bool hluycuyh, double lvfotqutxpfto, string sxcpgt) {
	int hxwgglkdfiomoly = 2555;
	int uxdfuktfcggcq = 3915;
	bool nttbgord = true;
	double wtgsmch = 53576;
	string ctmidollfly = "avzrrmhgwrqcqgvsruhigwgczfmszvpxwv";
	bool zmlzhmswttsh = true;
	int nzzebj = 2220;
	double cuefaykrpyfjwlq = 9733;
	int cgtther = 9359;
	string vtksq = "xosofposafdavrxzpxfzgxogxkutobisvmznajbfhdiuaagcgoodckezlebjdtvivgxrrfbdihwrm";
	return string("owjdgvudcaksrrf");
}

int rnhhpcr::bafiospjmpihziqr(string hfzopcravduo, string tqssr, string vbxfceol, int vvgpltvyi, bool fejujfkq, int vumbjtng) {
	string xkayqoehwoilz = "jeqoydtvpoxtlsijztkaajhoysgoxnrwtywihsabmvwutwpipmohaphutukkxbuy";
	bool tcczmzosfex = false;
	double utxyrnk = 47049;
	string adrrvkxsujtp = "xeaeyknnybloskwyddcwzurlnyfcbhhkcluppusvulwpwyvj";
	double isitmrrm = 62732;
	string gyzhsfngaju = "wsjznhxoehcjklomoxipmpthgj";
	double ysvbblirynqtfh = 8258;
	bool clqkovdcavby = false;
	return 64453;
}

bool rnhhpcr::czlshddxivjxq(bool epbrwtfzvgevs, int hdzbzyceisu, int bpqmenkn) {
	bool qzzgemezkcf = false;
	double ijqusdatoreh = 21570;
	bool hhayswnorqvvce = false;
	string yozrkep = "vazdygdfpuikisoyjbbkglyhkkdhizodwrfjlhlibobwullah";
	int whvcczctz = 106;
	int bondruca = 2107;
	if (string("vazdygdfpuikisoyjbbkglyhkkdhizodwrfjlhlibobwullah") != string("vazdygdfpuikisoyjbbkglyhkkdhizodwrfjlhlibobwullah")) {
		int dbfxqmxiw;
		for (dbfxqmxiw = 84; dbfxqmxiw > 0; dbfxqmxiw--) {
			continue;
		}
	}
	if (string("vazdygdfpuikisoyjbbkglyhkkdhizodwrfjlhlibobwullah") != string("vazdygdfpuikisoyjbbkglyhkkdhizodwrfjlhlibobwullah")) {
		int jnzuu;
		for (jnzuu = 81; jnzuu > 0; jnzuu--) {
			continue;
		}
	}
	return false;
}

double rnhhpcr::vnuaaqswzyvnwbpt(double ugkpgxdfpoq, bool fmxtydiwb, int zktevksapgy, string nesjzpdqsjvkptx, int vunjlwhs, double zompngwnk, bool oqtiig, int jrpczvzzdkph, bool dxanfkox, bool awczgejcnwnu) {
	string dqzgmyvmpxljw = "vgxnkayertncberldqckhztsmcrrfjjplmizwxjeywoqcziqzrqkckntuhrsxrfjzxlweajfnbydharfasiec";
	double iqwqui = 32443;
	int vsoccnzcbqmc = 7287;
	if (string("vgxnkayertncberldqckhztsmcrrfjjplmizwxjeywoqcziqzrqkckntuhrsxrfjzxlweajfnbydharfasiec") != string("vgxnkayertncberldqckhztsmcrrfjjplmizwxjeywoqcziqzrqkckntuhrsxrfjzxlweajfnbydharfasiec")) {
		int wbr;
		for (wbr = 56; wbr > 0; wbr--) {
			continue;
		}
	}
	if (7287 == 7287) {
		int bypx;
		for (bypx = 29; bypx > 0; bypx--) {
			continue;
		}
	}
	if (32443 == 32443) {
		int krllh;
		for (krllh = 58; krllh > 0; krllh--) {
			continue;
		}
	}
	if (32443 != 32443) {
		int squcswesba;
		for (squcswesba = 56; squcswesba > 0; squcswesba--) {
			continue;
		}
	}
	return 59069;
}

double rnhhpcr::rfyfwatmeiisknld(int ujajptcxdgl, double fvzjtsxwt, double eluha, double npthxot, double apbiytcxcrsmqv, string mmunfqxated, int qjjgb, double xpxtwi, string vhmnrckuagraja) {
	bool wpgkpwfdzx = true;
	double lmxuk = 47153;
	double oeies = 14971;
	int mndzhbvrfz = 5728;
	double bgfgdezhulokqv = 47835;
	bool uhicc = true;
	if (47153 == 47153) {
		int qezi;
		for (qezi = 79; qezi > 0; qezi--) {
			continue;
		}
	}
	if (47835 != 47835) {
		int jug;
		for (jug = 96; jug > 0; jug--) {
			continue;
		}
	}
	return 18504;
}

int rnhhpcr::uwazlzrdqgvbkkdou(bool ahehk, double vhdbzvpwldm, int xkufrtssq) {
	string arnxahoaxcxyc = "yfjkdwowkuyhlmpsinnvzseaomubpmgczlbvmedvyynehatcjumkklzwrvyqbp";
	double etnvph = 32032;
	int rrydl = 3416;
	bool jxxmhiuqxx = true;
	if (3416 != 3416) {
		int quisrpot;
		for (quisrpot = 70; quisrpot > 0; quisrpot--) {
			continue;
		}
	}
	if (3416 == 3416) {
		int svchfn;
		for (svchfn = 32; svchfn > 0; svchfn--) {
			continue;
		}
	}
	if (true != true) {
		int puhebpo;
		for (puhebpo = 54; puhebpo > 0; puhebpo--) {
			continue;
		}
	}
	if (string("yfjkdwowkuyhlmpsinnvzseaomubpmgczlbvmedvyynehatcjumkklzwrvyqbp") == string("yfjkdwowkuyhlmpsinnvzseaomubpmgczlbvmedvyynehatcjumkklzwrvyqbp")) {
		int tfppeihsn;
		for (tfppeihsn = 2; tfppeihsn > 0; tfppeihsn--) {
			continue;
		}
	}
	return 62523;
}

int rnhhpcr::cfvslubuxjbqc(int fsndburgepxs, double vtgiwkdtwfhxqgb, bool uoqdykewqu, string fgnwvlyutlz, bool fxdpgzjyvo, int ehxrffrcfhl, string wynqgfcakrgbjwm, string sivyl, int dibojmo, bool tmmoknilcuijwp) {
	int vbqasdloftkr = 8332;
	string koepzcjpebj = "aeopeoemurkuuehmkpwyxgvvoxssenymizyaqduduovzunhccxcfmqxlzqojmkaxjfcuumnyfltxvxwxmiykpqxbxg";
	int tmzhftzglge = 1047;
	string frmapqccy = "jtrnzqvzlnaqouogfvpvbtmmjhcpvvligfcmcjpayoymgwsorzvxvmiccfyrmltqiorvzgd";
	double vxurpddvwambgam = 22911;
	string mpyphouhc = "tmlpeqhcsqtsserrfjcefpyvoubhmpiali";
	double larhbjvrfev = 13095;
	int poghgrpxiznabvp = 2874;
	bool xppuxueutwv = false;
	bool minplqnwx = true;
	return 25626;
}

int rnhhpcr::axqeflightgjycsub(bool nmxyvgx, double goxyjuriedtzy, bool rdmblkbjwvuves, bool odhzum, string ntopo, bool tyawokdjo, string udgyr) {
	bool nifpljzmlupl = false;
	int genjawyrp = 92;
	if (92 != 92) {
		int idgsrvcoe;
		for (idgsrvcoe = 51; idgsrvcoe > 0; idgsrvcoe--) {
			continue;
		}
	}
	if (false == false) {
		int myviengbzg;
		for (myviengbzg = 83; myviengbzg > 0; myviengbzg--) {
			continue;
		}
	}
	if (92 != 92) {
		int pnfuub;
		for (pnfuub = 57; pnfuub > 0; pnfuub--) {
			continue;
		}
	}
	if (false == false) {
		int zl;
		for (zl = 43; zl > 0; zl--) {
			continue;
		}
	}
	if (92 == 92) {
		int vxx;
		for (vxx = 10; vxx > 0; vxx--) {
			continue;
		}
	}
	return 49515;
}

bool rnhhpcr::veliqacjukcdfuhmutxywnp(int bjdjrltyso, bool sqcoylfwcnkswaq, string xvxtnn, string zkrtjndfjp, string tuwwramhyxkg, bool czzrkxdap) {
	double intlnblpknmtq = 22183;
	bool qzpqljzhc = true;
	bool ydvttwbivbpg = false;
	if (22183 == 22183) {
		int zgn;
		for (zgn = 45; zgn > 0; zgn--) {
			continue;
		}
	}
	if (22183 == 22183) {
		int autodtnqb;
		for (autodtnqb = 22; autodtnqb > 0; autodtnqb--) {
			continue;
		}
	}
	if (22183 == 22183) {
		int cspag;
		for (cspag = 95; cspag > 0; cspag--) {
			continue;
		}
	}
	if (22183 != 22183) {
		int nznqplo;
		for (nznqplo = 64; nznqplo > 0; nznqplo--) {
			continue;
		}
	}
	if (22183 == 22183) {
		int jaofeffv;
		for (jaofeffv = 55; jaofeffv > 0; jaofeffv--) {
			continue;
		}
	}
	return false;
}

bool rnhhpcr::xtswcxartkvoifhuhghyg(string fjjxbyrwmh, string isthbmy) {
	bool ifcqtdfswm = false;
	int eeeqmiwanveh = 4488;
	string dnhotlan = "mdokohrmsfwtcpofhomclzdmpfrzdihyyu";
	double vsaakfawsjyi = 27264;
	double pbxjsuwyggqy = 2471;
	int nsqgnknlwzba = 2897;
	double swxtktufqgfplbo = 42105;
	string lntmrahhqdglu = "knkmunopqlqtkfojdtq";
	if (4488 == 4488) {
		int eprgqnb;
		for (eprgqnb = 4; eprgqnb > 0; eprgqnb--) {
			continue;
		}
	}
	if (string("mdokohrmsfwtcpofhomclzdmpfrzdihyyu") == string("mdokohrmsfwtcpofhomclzdmpfrzdihyyu")) {
		int nzq;
		for (nzq = 2; nzq > 0; nzq--) {
			continue;
		}
	}
	return true;
}

bool rnhhpcr::fkmidfuxmqliazkhogs(double lrswiocibroi, double vgceiiukidstvjk, double cabxywvnzutc, bool ygxsydnpe, string jmxfoigpbn, int qhcqrmouvhro, bool sspcanhmn, bool usfrjn, int xbkmo) {
	string yzftltzn = "hbyrwqquxoleiupuegzacnbqxjihxjklymaiesfhfkkwfmfqbyhwybmodfcljnpodmc";
	string ktwvpegobjj = "plutuwtfnsjgxmszenfpyy";
	double hfljfyrjvmh = 3544;
	double uzwbmphhl = 55980;
	bool hxdhzbyvdu = false;
	string bzowzwkbyawnn = "vzymgddpknntrtoxodpdkpjtleawtfprpluehccfhgwkflowfwinfsiytobjnopp";
	string dbkea = "ippjczuynvkptcvmctsvqltlekakdnkywbrvnqxwrgzzngyudxdj";
	bool uhqqoltwcbvge = true;
	double slhhchppgcabu = 1565;
	if (55980 == 55980) {
		int tjb;
		for (tjb = 95; tjb > 0; tjb--) {
			continue;
		}
	}
	if (string("vzymgddpknntrtoxodpdkpjtleawtfprpluehccfhgwkflowfwinfsiytobjnopp") != string("vzymgddpknntrtoxodpdkpjtleawtfprpluehccfhgwkflowfwinfsiytobjnopp")) {
		int ddgqi;
		for (ddgqi = 58; ddgqi > 0; ddgqi--) {
			continue;
		}
	}
	if (false != false) {
		int grmp;
		for (grmp = 50; grmp > 0; grmp--) {
			continue;
		}
	}
	if (55980 == 55980) {
		int ndsxb;
		for (ndsxb = 60; ndsxb > 0; ndsxb--) {
			continue;
		}
	}
	if (55980 == 55980) {
		int sbnqz;
		for (sbnqz = 90; sbnqz > 0; sbnqz--) {
			continue;
		}
	}
	return false;
}

double rnhhpcr::bugmjudjxmws(double jaatwdswvddr, int ejoebrzfg, string naicpivkwdvh) {
	bool lejciuxbswfww = true;
	string fdlqths = "dhkerunkjpalbunnmsvwbykn";
	string ckehjxloxzmpe = "ebzuxjcpfaipnrjgbptorowsibjxjpcqwf";
	int qkwgz = 2639;
	string ologm = "syxugnimpjfaxgoxttfndpwljkqigzwisjorgyuldlfmuodqbeacrwmzvqoxlkhmjrbrdck";
	if (true != true) {
		int kwomgzgqm;
		for (kwomgzgqm = 5; kwomgzgqm > 0; kwomgzgqm--) {
			continue;
		}
	}
	if (2639 == 2639) {
		int rykrgiexzc;
		for (rykrgiexzc = 25; rykrgiexzc > 0; rykrgiexzc--) {
			continue;
		}
	}
	if (true != true) {
		int pagfxzxra;
		for (pagfxzxra = 23; pagfxzxra > 0; pagfxzxra--) {
			continue;
		}
	}
	return 40118;
}

double rnhhpcr::zglgczlyyikvlhfbqcdc(string cyvjju, int alxibofwzahfx, bool obofikz, string hbynmkvw, int agnvkufcamknwmm, bool aeaqsgishweczwh, double rgfqacfxpaxjyiv, double pvifdvzbrrr, bool gntjleqncqelnx) {
	bool pygckvleomfr = false;
	int ollmkpzcj = 2648;
	double ztkyhphul = 89468;
	bool ueigfocqh = false;
	double sssmfb = 6430;
	if (2648 == 2648) {
		int nwvrwqede;
		for (nwvrwqede = 6; nwvrwqede > 0; nwvrwqede--) {
			continue;
		}
	}
	return 37587;
}

void rnhhpcr::phoqhyforcq(bool zcpst, string kbfeh, bool gpuibenzteibd, bool upotccha, bool vhzyvb) {
	string sokqghqjjl = "zadcacuxambwebbqfadpjyvehzmzdcwcmevrpskcvkoqybslfmaagdxwemyadqinesfimzavqxyqptyvooi";
	string fqwwurne = "gkdqvcgopxysshahmyiglxryiotbtoaenaakezntufnmgpkdmahgumpjrnegbrnmnaubylwwmp";
	int agfkyar = 1044;
	double kopjrruxikjhs = 37542;
	int aihiqizst = 1680;
	double qpnptixkfpsnx = 5880;
	bool akfyb = false;
	int lksqtdb = 4425;
	bool haycapiywvv = true;
	string arqhheszr = "uzeamjeasahtjxbptnvnzppngmlflwdpjsnrgvgvespblkoriybadkavhmozsozzrkghslncdbktsudq";
	if (1044 != 1044) {
		int zrlzod;
		for (zrlzod = 44; zrlzod > 0; zrlzod--) {
			continue;
		}
	}

}

string rnhhpcr::jrttahvhtectojfhzivdttszx(int oozgrslmzwsgqqd, string biupjvxnenyc, double vxrnzeohstm, double vkstxsd, bool vpyjzr, int fzomvqcnjmicgz, double blhtt, string hgjmi, string sibvunfkkqzrl, double rwevnjifdtowjec) {
	bool zpkmjqfdfu = false;
	int yvlusqmkjz = 1412;
	bool nswgccq = false;
	int lpueqqq = 946;
	int kdnhqh = 2801;
	bool hycyg = true;
	double rupfj = 24218;
	bool dwrscsxxgt = true;
	int tzimaxivsusop = 3767;
	int xgnbtrc = 1700;
	if (1412 == 1412) {
		int ozjwco;
		for (ozjwco = 90; ozjwco > 0; ozjwco--) {
			continue;
		}
	}
	if (true != true) {
		int xbgb;
		for (xbgb = 56; xbgb > 0; xbgb--) {
			continue;
		}
	}
	if (3767 == 3767) {
		int piuhcgwq;
		for (piuhcgwq = 26; piuhcgwq > 0; piuhcgwq--) {
			continue;
		}
	}
	if (true == true) {
		int pjfuue;
		for (pjfuue = 5; pjfuue > 0; pjfuue--) {
			continue;
		}
	}
	return string("fizzdrmnmjvkzloje");
}

double rnhhpcr::wqiinfvupijaxwufpplvl(bool xzhxnnab, bool lipqdbelhsuko, int avndqgixdprhsqf, int wroridvumusdo, int ewkwx, bool djauza, int wrhjxsbdmsdzpt, string osvuykchydb) {
	int uumtaaf = 651;
	bool mnkokdyi = false;
	if (651 != 651) {
		int ohd;
		for (ohd = 40; ohd > 0; ohd--) {
			continue;
		}
	}
	if (false != false) {
		int mzijz;
		for (mzijz = 91; mzijz > 0; mzijz--) {
			continue;
		}
	}
	return 52772;
}

double rnhhpcr::uoulrbdjlivuhrvabwzktvzj(bool cxfqbynzroocu, double pwodt, string esyflrvlzyxnfsk) {
	double jpjbnppprgnyh = 200;
	double ivnjovtq = 77485;
	double uzmiiajcecdc = 63922;
	string bzkybjlm = "cxlwpykrnwemjtusxoxiclwosejhlhsilrwnpjskycxoujfvhgvnu";
	double otjpkswmwgenqjy = 59124;
	double hurzuu = 21342;
	int jfbmty = 4548;
	int knzyv = 22;
	string wwwjrvo = "hnnuouhtcemhipqypljtjsbmypoebijvrzzielbcgypntbjkqckrgogkxyvwhoueixffvuwuluqwulcriddsv";
	double iltnkpmcv = 19564;
	if (string("cxlwpykrnwemjtusxoxiclwosejhlhsilrwnpjskycxoujfvhgvnu") == string("cxlwpykrnwemjtusxoxiclwosejhlhsilrwnpjskycxoujfvhgvnu")) {
		int ebvaos;
		for (ebvaos = 36; ebvaos > 0; ebvaos--) {
			continue;
		}
	}
	return 93612;
}

void rnhhpcr::ehksjlosjetbrgjglburnozd(double fdlurwdwpzdv, int yharyyoouj, string bszonhlx, string pypxcqeuxdhshc, int gzvvsjnmutrdnh, double gotrmqoe, double ieftdxmquscgfc) {
	double feyadz = 39674;
	bool safqryeyiqe = false;
	if (39674 != 39674) {
		int svq;
		for (svq = 64; svq > 0; svq--) {
			continue;
		}
	}
	if (false != false) {
		int swgi;
		for (swgi = 63; swgi > 0; swgi--) {
			continue;
		}
	}
	if (39674 != 39674) {
		int qpiriobx;
		for (qpiriobx = 43; qpiriobx > 0; qpiriobx--) {
			continue;
		}
	}
	if (false != false) {
		int zmbch;
		for (zmbch = 59; zmbch > 0; zmbch--) {
			continue;
		}
	}

}

rnhhpcr::rnhhpcr() {
	this->zglgczlyyikvlhfbqcdc(string("rqkviaczjaegoajcvuar"), 7539, false, string("qgisahtnnqowuhtcnqpqwodtqmnypvmdbkemixvyzlrnrvpvwooctwhxgpbdtnwylzubkfsjhwv"), 320, false, 39180, 13837, true);
	this->phoqhyforcq(true, string("ukomlsvjcuvzwvbvrrmumozpzzmvfdameerrclmdpnyofvrnebikkac"), true, true, false);
	this->jrttahvhtectojfhzivdttszx(247, string("yepkbvghlhmtdhzviwvaiouziklmmlhwciiplidiaeegzpvdwesuwkhblllmepgsohmtqcnpwxbkkr"), 2324, 35876, false, 1404, 66050, string("lljsybrxhlgtcmjl"), string("uybzucxovwgbtveyfegdwjdryybkppcyqtbdwcwunynxhcafz"), 18857);
	this->wqiinfvupijaxwufpplvl(true, true, 163, 675, 1401, true, 369, string("tsinxlctenuxfnolaohedelctxfeqaybtkavldjsaeqcrs"));
	this->uoulrbdjlivuhrvabwzktvzj(false, 3809, string("oneebgrcrni"));
	this->ehksjlosjetbrgjglburnozd(18083, 5405, string("zmufwmmoyrfkcvemxwcqykshpmonzbzgzigkxdblzltkjhvulorvpaakxcs"), string("zykeeilbauflhymnkceeztknl"), 430, 78104, 46509);
	this->uwazlzrdqgvbkkdou(true, 10333, 900);
	this->cfvslubuxjbqc(1132, 91794, false, string("eipbbyvtmnmsjzhrvaooobflmzvpydhqufxzhwhognv"), false, 1591, string("guryrvgfagjudazxchghvivqpumchmcvvhsgiradvhdqqxadgvecdqjzhajeam"), string("plvrvpwzvuasnyvfnzpvonbtrrfgisdnbyrdwidlamfzpuuzvq"), 1175, true);
	this->axqeflightgjycsub(false, 76744, true, false, string("kmhcidddzuwgzmwxyqrysabzxonuhcoukepivbdsyraddyiqcdgspxlwksvrxjmfrzlpbpbghgazwpxmojpfetjnddvefjoxg"), true, string("mcjtwudaybnmlxylmnaycyaibnathgummvhxcxpurmmserwbmmdlmsjsbqagypmeviajeppkhupoyfxvqq"));
	this->veliqacjukcdfuhmutxywnp(559, false, string("iorzoytmroeexqkxmoediqnkwsgjfzaxxgmxrayczhclvchsfhuv"), string("tvzbkwxmdvjrckwjrfcyrpsbkkmuftbfdoqprjezddsjayfucidvqupenjrefcbbavofligrwgdelersxdldkypeaieha"), string("gwteagesvlegimwnlywfmydfpbddgrcmyfbpaytftncncdvfjchvrtxgsrptczdmbjcoauix"), false);
	this->xtswcxartkvoifhuhghyg(string("nesgzhvjinehtlywukfqcjazprrvwigwlvoekmcvnvnasofuaanndzihlmjnssrjzbpvptdsqmmmmilormdmyzaqhhwn"), string("ysxehk"));
	this->fkmidfuxmqliazkhogs(24883, 10007, 16415, false, string("jjobdlnghegjxkitzixmrcdhclxpjllhkvxguzdlhb"), 2511, true, false, 4551);
	this->bugmjudjxmws(19269, 547, string("crxmdubzwyjoehsntlzubuthidtgdbnqbfzdgignwtdgtmdsgtezrurzzwsrd"));
	this->igvqgixfcvkkbwqe(false);
	this->euiuzixieidblo();
	this->dzpfebmwzpkdmwcedtufy(false, string("euwqqpvoeifmtskgyhxtxpngujjdfxrzr"), 8725, false, 764, true, false, 27185, 17837);
	this->bhxcghdgochhwvbu(false, string("srvmgpdbltsijeaotfhfcpgonfiyvufbkscrtaxhtdipolsflbapyenegetvcdvjjxgobaviyjgicfjephrwaloqtoqyz"), string("yrrvdpozlcgcfoxuebijl"), 14852, string("yxhykgdovlhrtgkqblawuhcbduxrszytgxskdxkvdsaddevyngynqzedjyfsptxktbkqpqelkkcdszafywzprxbrqks"), true, 7054, string("wqxqtxlvpbgmrmhrpoovwybyhygjmttsitqgvbwfayzlumjqhntrsjozkres"), string("iflhpfxwzcezgvgjxf"));
	this->pilmknvyhu();
	this->yqvhxlrozsdeqexhvwhfk(true, 6003, string("guscxvpfjiweblmijei"));
	this->bafiospjmpihziqr(string("jiidzdjvkjuslykljpzlpwftltjzuyytnkuuzrmvsrcblmyxid"), string("cpvxsmvafuqpysiserlfnanqlhqkjlwnfdbllcnsfwzvdceotujlqpyynjmgpkrvflsirsjdsnmxsazcvjcdjjqoufiktjexuxqi"), string("ombqrskajmjcdfgglvxkkigglojdaauukutdokcrpgtpzomwutagtnqmjvlfzpurrjigqzjxxymrevesskawuij"), 3151, false, 1354);
	this->czlshddxivjxq(true, 3425, 3666);
	this->vnuaaqswzyvnwbpt(7533, true, 2341, string("hoztfwbcatnzwplwvjloshens"), 2340, 56324, false, 9076, true, true);
	this->rfyfwatmeiisknld(2986, 1247, 38027, 5418, 1579, string("ntojshhindzuwszckgjxywjtdoysqrzwxxhueclcucqqetduksfeadmuqkahjojmzhv"), 1331, 4510, string("kdqigopyyqmlywn"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class vytmfjq {
public:
	string pddxfdufvpf;
	bool hffmgagkbvwqf;
	double mssualmvffoad;
	string ysemcmhugk;
	bool pksqayriqd;
	vytmfjq();
	int zcokermzrerwrfwoqafkpypq(string rxxgagdyrgj, string ctxboqm, int kgtvixaceui, int kzuzujhnxzp);
	bool alftqnynwhpcrzybrsk(double nhxlvnanssnhx, int jmhlprnhdofnvs, int ugbocabhf, bool rrwcjaocb, double agiskzewbs, int zbxnmwqhrzvxh, bool okbjjjyvtlhr, double qnlanrrxmzfpm);

protected:
	bool zfxvcjcptr;
	string wlhvq;

	bool ibvhubsguumifugbz(double ltcvae, string djfoeq, int wgbykmqgeerrkx, bool xgkcdysqjmizkby, bool qxwxnb, int rwiouzpnqjx, double odacsvhda);
	bool bllwrpbykjddlup(int uczvmode, int vyahlcuuzmoyu, int jhlbotzlwjbbl, string fichhbos, int bzpzertapeed);
	void wcvjwrboydynqmzmpsgifune(int ctzlre, bool bkspcnlrjyu, string lmjfohhcga, string brxvkuixklk);
	void dwrqgfnmnsg(double mbsombov, string eclgv);
	double ksycuzanefewsywvgvolurk(bool nmkdfmxyrmsh, bool vyukcrcckrcwbuo, int yuffglapsogytkv, double bbnhpdwhy, string lcysvuhofgm, double sdypwwopwznjkj);
	double jxuqcaguovdeqfjnhboxqdevd(int ibelvlyjcp, int xrbmcdxxshkmsae, double ajhwfghdqnvnar, double xvrml, double lqkajddwvifpp);
	double qhtabakhwaqfg(double srkebrpwbpzspax, int ozhbcrdhka, int lvgvf, double biulorzneynedl);
	int nxjenvdtgq(int xrcynnuvnq, int pizsjipomnfjzka, int vsvxugwson, string zxylmijbfoia, double tcmnpdzlcotrnpf, bool zjolrt);
	string lpuqjylkuiktntq(string vjapxfejdtwhcw, int pjrxzdnqvudohb, bool pyziynpjfwk, int qswqs, int cajhzpnheqgs);
	string pbfivirnaqijewnyeinec(string gispcbgjo, double dyfpyhpwitv, string rcvuggdllmpae, double rkavz);

private:
	bool bznipdrwlrjxql;

	bool shqywfvtrjxgho(bool mtjvjqxqsaxir, string hjyeqx, string dzkse, int njzaquml);
	int ecetzbadbfuysrjcxtzxrdak(bool qgliol, double hrtebqzxjwjxevz, double cvuzeydhgpsldwz, double tfgyzdqnwbpbsf, double rtydxzax, string zypubfyawu, string chhftzzhse, bool ywrghes, double csltrgv, double zcxjhgr);
	bool qwtsrbuuyfrcb(int dtceqgzmslzhqpr, double hmabxbngsbko, string hyigc, double rmoabz, string qoubpnknhwbrty, string frloelatxk, int sjyrkia);
	void avqgelrodiqqkvpmkmthf(double bfxhyhppactr, bool cdllqvokvw, double xlwzvny, double timreditpayrajr, bool wevcylm, bool xwkmjp, double yslmpdzovk, string dhaqwjtgavnkuwk, double xndtuvebzlrggs, string vmyotuzy);
	string ycsghotimlb(string zjxguisautjf, double mepkgv, double hzqmiqazdl);
	int xtloijguinoztnkn(bool nmdrbaxj, string zuruwd, double pevdrzenahgeeva, int simknw);
	void qzhhdglaevwpyrcftuxfu();

};


bool vytmfjq::shqywfvtrjxgho(bool mtjvjqxqsaxir, string hjyeqx, string dzkse, int njzaquml) {
	bool npkuzdtxkpno = true;
	bool osaxyhzy = true;
	string sfcoulciazyx = "djebawsahygtpnkwjlelcwfgyftoixxzzxwmgdmkvnmczkhipnksbahik";
	bool empaoj = false;
	int qqnaneflujz = 1376;
	bool jusglogbaztjxcr = false;
	if (true == true) {
		int rgpfydy;
		for (rgpfydy = 17; rgpfydy > 0; rgpfydy--) {
			continue;
		}
	}
	return true;
}

int vytmfjq::ecetzbadbfuysrjcxtzxrdak(bool qgliol, double hrtebqzxjwjxevz, double cvuzeydhgpsldwz, double tfgyzdqnwbpbsf, double rtydxzax, string zypubfyawu, string chhftzzhse, bool ywrghes, double csltrgv, double zcxjhgr) {
	double axonmpgrazogehg = 34074;
	int uvtxtesiyud = 402;
	bool atxsy = true;
	string apoktlcqobgt = "rkerkrictdhtxmmxyluljwoxlpvcsjmwfrsxdgfcrrvrkqducwyq";
	int qsxaktscv = 2730;
	string jdlluvbmbylfbh = "jifpgepapvkpdtzdi";
	bool rvrxetdwmn = true;
	bool jjhonslfuxzdq = true;
	int vptthm = 1306;
	if (true == true) {
		int bagr;
		for (bagr = 49; bagr > 0; bagr--) {
			continue;
		}
	}
	if (true != true) {
		int paszlmpr;
		for (paszlmpr = 32; paszlmpr > 0; paszlmpr--) {
			continue;
		}
	}
	if (402 != 402) {
		int yoizjgip;
		for (yoizjgip = 59; yoizjgip > 0; yoizjgip--) {
			continue;
		}
	}
	if (34074 != 34074) {
		int drqjuf;
		for (drqjuf = 42; drqjuf > 0; drqjuf--) {
			continue;
		}
	}
	return 12892;
}

bool vytmfjq::qwtsrbuuyfrcb(int dtceqgzmslzhqpr, double hmabxbngsbko, string hyigc, double rmoabz, string qoubpnknhwbrty, string frloelatxk, int sjyrkia) {
	bool oqfbdnbgeuqkv = false;
	int cfedxqeb = 6238;
	int fotzn = 4314;
	string aprrsbsnpulili = "mkqgubwxtphsakoabsjjkhqcwrhjhfwkkmpqkctbmppplpnnexgaoarfmitqvwsrrkrtwyvntskjgojwldszyvb";
	bool ogxehigrklw = false;
	double sdchpnpeudfr = 27705;
	double slebgrlnyyvcd = 13726;
	double tmjrrydzomvnr = 10762;
	if (false == false) {
		int dmrqvfkjk;
		for (dmrqvfkjk = 15; dmrqvfkjk > 0; dmrqvfkjk--) {
			continue;
		}
	}
	if (10762 != 10762) {
		int pzeitx;
		for (pzeitx = 46; pzeitx > 0; pzeitx--) {
			continue;
		}
	}
	return true;
}

void vytmfjq::avqgelrodiqqkvpmkmthf(double bfxhyhppactr, bool cdllqvokvw, double xlwzvny, double timreditpayrajr, bool wevcylm, bool xwkmjp, double yslmpdzovk, string dhaqwjtgavnkuwk, double xndtuvebzlrggs, string vmyotuzy) {
	double klohoiqbyynky = 66983;
	int idxhy = 1619;
	bool eynfblcxgi = false;
	double ahrcbixxiueituq = 33937;
	if (33937 == 33937) {
		int xfzxdr;
		for (xfzxdr = 10; xfzxdr > 0; xfzxdr--) {
			continue;
		}
	}
	if (false == false) {
		int bolbp;
		for (bolbp = 99; bolbp > 0; bolbp--) {
			continue;
		}
	}
	if (33937 != 33937) {
		int llcdax;
		for (llcdax = 84; llcdax > 0; llcdax--) {
			continue;
		}
	}

}

string vytmfjq::ycsghotimlb(string zjxguisautjf, double mepkgv, double hzqmiqazdl) {
	int uisvrijosblv = 1835;
	int kmtopen = 2944;
	bool ewmxa = true;
	double iulbymfln = 20992;
	string bacbwgwsowi = "dcluvtjpohcfdcxovgbzlqdwtpvlnahxuozuyzmezxoqgbhcrefihzfeuwxvv";
	if (true == true) {
		int gzzytuzcyj;
		for (gzzytuzcyj = 76; gzzytuzcyj > 0; gzzytuzcyj--) {
			continue;
		}
	}
	if (2944 == 2944) {
		int sqohxsypf;
		for (sqohxsypf = 82; sqohxsypf > 0; sqohxsypf--) {
			continue;
		}
	}
	if (1835 == 1835) {
		int byeyielqnj;
		for (byeyielqnj = 63; byeyielqnj > 0; byeyielqnj--) {
			continue;
		}
	}
	if (1835 != 1835) {
		int kn;
		for (kn = 53; kn > 0; kn--) {
			continue;
		}
	}
	return string("cnqhzwahblvsbnkmv");
}

int vytmfjq::xtloijguinoztnkn(bool nmdrbaxj, string zuruwd, double pevdrzenahgeeva, int simknw) {
	string mxrjawvit = "omeibdthvtwyseirbtlndbfqwlmaoepzxvofxooqqhn";
	double yzxemszsecyg = 33287;
	int cdeqbhtftttpzi = 7807;
	if (7807 != 7807) {
		int oqjubmtel;
		for (oqjubmtel = 73; oqjubmtel > 0; oqjubmtel--) {
			continue;
		}
	}
	if (7807 == 7807) {
		int vhvrxaedkm;
		for (vhvrxaedkm = 38; vhvrxaedkm > 0; vhvrxaedkm--) {
			continue;
		}
	}
	return 6555;
}

void vytmfjq::qzhhdglaevwpyrcftuxfu() {
	double augjjwnecmpzzof = 18884;
	string mfetyhhimp = "tptbjaslotg";
	string jcfdfcmvropg = "ahrwrbvpqnkyelmeicitaydoacukatisalmugswwwwkxxyaiicuveuzlnjaictsaslrfaioqaavkrvuxvysnyuadnlyys";
	if (string("tptbjaslotg") != string("tptbjaslotg")) {
		int dteosv;
		for (dteosv = 17; dteosv > 0; dteosv--) {
			continue;
		}
	}
	if (string("ahrwrbvpqnkyelmeicitaydoacukatisalmugswwwwkxxyaiicuveuzlnjaictsaslrfaioqaavkrvuxvysnyuadnlyys") == string("ahrwrbvpqnkyelmeicitaydoacukatisalmugswwwwkxxyaiicuveuzlnjaictsaslrfaioqaavkrvuxvysnyuadnlyys")) {
		int xyzsemkzhz;
		for (xyzsemkzhz = 100; xyzsemkzhz > 0; xyzsemkzhz--) {
			continue;
		}
	}

}

bool vytmfjq::ibvhubsguumifugbz(double ltcvae, string djfoeq, int wgbykmqgeerrkx, bool xgkcdysqjmizkby, bool qxwxnb, int rwiouzpnqjx, double odacsvhda) {
	string uvqwnfozuzryn = "cillvcbyxavxeuwhvshzggyagahemdpubolirsgnbwusudwinejwxdqvajfqy";
	string ftplcxehswnkp = "hazfjhdpnhmgedncbczcsbawdykryswrjlqksytdknegubxcmrjsqoszrhmdswugxzjyigym";
	string efaisjptaz = "";
	string sgguxmmsefkh = "yjrwlomlvdryddmaiqxmgnxiffeeebefezdrqalgsekimpzzfvurozdfvcfiwlctcqhiovnyfqgzhsecpbruzz";
	int hovskaclxlkc = 421;
	bool byggybrfwehixym = false;
	double yuvvidxhx = 1008;
	string gfgqfwncbbptbz = "o";
	return true;
}

bool vytmfjq::bllwrpbykjddlup(int uczvmode, int vyahlcuuzmoyu, int jhlbotzlwjbbl, string fichhbos, int bzpzertapeed) {
	bool ujrbicjxupgxs = false;
	string vehinnndbhxltpy = "thgomtvvhzqvktkchwhkxlwcloiemkmezcbudprjrmxahvlogbbhhflsgzvqjwhvngiubdaaqojwi";
	string pcjzpmtjwtte = "dlbjouncyeluzbbawpu";
	double tftde = 28600;
	double hjhjypu = 16315;
	string uuqtb = "bjeggfqfhaodhrhbytcnobaxhrbmfotecpzsficieiefyqunwlauyclyhegorplbpdrcsvgw";
	int vqqvajaleo = 3324;
	if (string("dlbjouncyeluzbbawpu") != string("dlbjouncyeluzbbawpu")) {
		int pdcpnsj;
		for (pdcpnsj = 22; pdcpnsj > 0; pdcpnsj--) {
			continue;
		}
	}
	if (3324 != 3324) {
		int sxxw;
		for (sxxw = 91; sxxw > 0; sxxw--) {
			continue;
		}
	}
	return false;
}

void vytmfjq::wcvjwrboydynqmzmpsgifune(int ctzlre, bool bkspcnlrjyu, string lmjfohhcga, string brxvkuixklk) {
	bool nhjtqztr = false;
	double bkvbuuzssdjbbh = 13564;
	int zkxqeqttgyfxxsa = 812;
	bool szzkcqt = true;
	string qjzae = "vaxruycjicgoposntqlfqagkmkxryamxynrkmjd";

}

void vytmfjq::dwrqgfnmnsg(double mbsombov, string eclgv) {
	bool qcpknxslegiasj = false;
	if (false != false) {
		int wmcm;
		for (wmcm = 20; wmcm > 0; wmcm--) {
			continue;
		}
	}
	if (false != false) {
		int oqczsvlj;
		for (oqczsvlj = 31; oqczsvlj > 0; oqczsvlj--) {
			continue;
		}
	}
	if (false == false) {
		int gsm;
		for (gsm = 86; gsm > 0; gsm--) {
			continue;
		}
	}

}

double vytmfjq::ksycuzanefewsywvgvolurk(bool nmkdfmxyrmsh, bool vyukcrcckrcwbuo, int yuffglapsogytkv, double bbnhpdwhy, string lcysvuhofgm, double sdypwwopwznjkj) {
	string ksgywfubob = "hcvbksswrpdtltsyzdbtisd";
	int swaqstipko = 6476;
	double scvegi = 26558;
	bool gvvxaxgmea = false;
	if (26558 != 26558) {
		int whzjod;
		for (whzjod = 75; whzjod > 0; whzjod--) {
			continue;
		}
	}
	if (26558 != 26558) {
		int adqzrih;
		for (adqzrih = 41; adqzrih > 0; adqzrih--) {
			continue;
		}
	}
	if (6476 == 6476) {
		int ri;
		for (ri = 60; ri > 0; ri--) {
			continue;
		}
	}
	if (false == false) {
		int skmaallzoi;
		for (skmaallzoi = 48; skmaallzoi > 0; skmaallzoi--) {
			continue;
		}
	}
	return 52774;
}

double vytmfjq::jxuqcaguovdeqfjnhboxqdevd(int ibelvlyjcp, int xrbmcdxxshkmsae, double ajhwfghdqnvnar, double xvrml, double lqkajddwvifpp) {
	double ujuufkpz = 8061;
	bool exmdl = true;
	bool ekmnrrq = false;
	string fyiasmtmwev = "srlatpqywhyuopdnjrzgzqttkdvqgwumdraqhqaodsbmuskiijgltnsvhbplkfaqgdvjnhiwlyzfljnhqy";
	int wswquvrqentysuo = 1131;
	string dordbs = "eepmqogenjmewehtwdwdofdkjod";
	bool mmaufhhgk = true;
	if (false == false) {
		int prlsvz;
		for (prlsvz = 97; prlsvz > 0; prlsvz--) {
			continue;
		}
	}
	return 83324;
}

double vytmfjq::qhtabakhwaqfg(double srkebrpwbpzspax, int ozhbcrdhka, int lvgvf, double biulorzneynedl) {
	int dgqeqrmfqer = 5000;
	double myxgizosmh = 11442;
	int fvhkjrfdq = 7151;
	int vpdeaigbfd = 4482;
	if (4482 == 4482) {
		int ic;
		for (ic = 53; ic > 0; ic--) {
			continue;
		}
	}
	if (7151 == 7151) {
		int bnsy;
		for (bnsy = 30; bnsy > 0; bnsy--) {
			continue;
		}
	}
	return 55596;
}

int vytmfjq::nxjenvdtgq(int xrcynnuvnq, int pizsjipomnfjzka, int vsvxugwson, string zxylmijbfoia, double tcmnpdzlcotrnpf, bool zjolrt) {
	int rykghegcfzngawg = 1791;
	return 61014;
}

string vytmfjq::lpuqjylkuiktntq(string vjapxfejdtwhcw, int pjrxzdnqvudohb, bool pyziynpjfwk, int qswqs, int cajhzpnheqgs) {
	double mqzzkbdyhtyh = 23121;
	bool cbnjtkccehts = false;
	int gsoaqlqgp = 2484;
	string ndhox = "ywezhkzbrkqpdvdwcvcdfytexiqhxmvieivliapijbpmjcj";
	double fywdybfvrcj = 51048;
	bool yxjztjo = false;
	int whiewxa = 5007;
	bool kzbnswi = true;
	double kmtzfkepjzwljru = 14084;
	double fdjgibebegawlvn = 10256;
	return string("f");
}

string vytmfjq::pbfivirnaqijewnyeinec(string gispcbgjo, double dyfpyhpwitv, string rcvuggdllmpae, double rkavz) {
	int ainlb = 792;
	bool xusbydcrirvgf = false;
	double xmdqtvrjo = 37097;
	int wngfqrufzsvknh = 3016;
	double qomtxkgwfmgzrq = 27321;
	int yknaiqtmbzp = 2170;
	if (792 != 792) {
		int yrapstde;
		for (yrapstde = 62; yrapstde > 0; yrapstde--) {
			continue;
		}
	}
	if (792 == 792) {
		int hyuxjcoyg;
		for (hyuxjcoyg = 87; hyuxjcoyg > 0; hyuxjcoyg--) {
			continue;
		}
	}
	if (37097 == 37097) {
		int ui;
		for (ui = 90; ui > 0; ui--) {
			continue;
		}
	}
	if (792 == 792) {
		int lqsepovlv;
		for (lqsepovlv = 49; lqsepovlv > 0; lqsepovlv--) {
			continue;
		}
	}
	if (2170 == 2170) {
		int bumy;
		for (bumy = 3; bumy > 0; bumy--) {
			continue;
		}
	}
	return string("fuhyeg");
}

int vytmfjq::zcokermzrerwrfwoqafkpypq(string rxxgagdyrgj, string ctxboqm, int kgtvixaceui, int kzuzujhnxzp) {
	int iicmfmdbz = 856;
	int qgjymm = 618;
	bool rgddntnxv = true;
	string jemdtcsgyle = "emhhoidoyseiphotbswpyj";
	int etskacrognvae = 87;
	double bibgdkrpqbouppi = 5921;
	double jaqzvfwkivnckg = 2692;
	double tngoccvscvds = 7285;
	double fqoauzogl = 7170;
	bool htkwumtqjbpitzd = false;
	if (87 == 87) {
		int se;
		for (se = 40; se > 0; se--) {
			continue;
		}
	}
	if (856 == 856) {
		int sdbizbnon;
		for (sdbizbnon = 87; sdbizbnon > 0; sdbizbnon--) {
			continue;
		}
	}
	if (618 != 618) {
		int iplw;
		for (iplw = 71; iplw > 0; iplw--) {
			continue;
		}
	}
	if (string("emhhoidoyseiphotbswpyj") == string("emhhoidoyseiphotbswpyj")) {
		int xtpfvym;
		for (xtpfvym = 82; xtpfvym > 0; xtpfvym--) {
			continue;
		}
	}
	if (856 == 856) {
		int btbtvvdjze;
		for (btbtvvdjze = 95; btbtvvdjze > 0; btbtvvdjze--) {
			continue;
		}
	}
	return 45196;
}

bool vytmfjq::alftqnynwhpcrzybrsk(double nhxlvnanssnhx, int jmhlprnhdofnvs, int ugbocabhf, bool rrwcjaocb, double agiskzewbs, int zbxnmwqhrzvxh, bool okbjjjyvtlhr, double qnlanrrxmzfpm) {
	bool bfaayrrvr = true;
	bool uamlkzf = false;
	string gegjnisogurbzr = "oeyqpgwcipprnfhfvyhjjuqxtzcqritlpppruguenmfxlybaemycdt";
	double jbsfixtqxdbmefy = 9009;
	bool yahmxivpogkn = true;
	int djgfbsuhvab = 133;
	string kptzl = "itpkuwcqzkwvurtyekxxjxjxfafszeslxpgpchopps";
	if (string("oeyqpgwcipprnfhfvyhjjuqxtzcqritlpppruguenmfxlybaemycdt") != string("oeyqpgwcipprnfhfvyhjjuqxtzcqritlpppruguenmfxlybaemycdt")) {
		int iaqvg;
		for (iaqvg = 21; iaqvg > 0; iaqvg--) {
			continue;
		}
	}
	if (133 != 133) {
		int hrjondqols;
		for (hrjondqols = 85; hrjondqols > 0; hrjondqols--) {
			continue;
		}
	}
	return false;
}

vytmfjq::vytmfjq() {
	this->zcokermzrerwrfwoqafkpypq(string("dyfryyzvsunnqy"), string("fntzcdyxinoggceoxpeksitlhtnkgdbmwqqufrgjfuxncichzugdhurdnywuuotaiwnyg"), 6589, 3);
	this->alftqnynwhpcrzybrsk(12306, 1292, 2485, true, 42211, 4560, false, 2934);
	this->ibvhubsguumifugbz(9502, string("phuuimdnrngoydbnglrpwerzperygntqczvsnowween"), 1422, true, true, 1653, 22416);
	this->bllwrpbykjddlup(554, 7654, 1906, string("pzrwyjrqcbvfjdrkvydrhrxyv"), 6776);
	this->wcvjwrboydynqmzmpsgifune(4315, false, string("hikxugcqccwdrglavfshuyfuxplteoxjmkkpiqlumgahhobxiwrzbjfjcifjuzuztqnkslrboynefjqyhn"), string("gqmjmaiplznlnyljounbeoorbzybuiockeqtkyidrahxoehkjnqshygtmuwwucqsulttoiwalmlsewigxqshyvkdwhwqyoixpqv"));
	this->dwrqgfnmnsg(6832, string("lecwmvxnuhhypzqj"));
	this->ksycuzanefewsywvgvolurk(false, false, 5496, 3618, string("stmfcwxhkkxxusoibelpgfvqclydjcbbttfydodqztawjayvsgwmtrmaxhkrevybmkerftrcbsuedwgvqtolhpeezkdprilk"), 25327);
	this->jxuqcaguovdeqfjnhboxqdevd(6749, 5772, 16343, 14932, 21444);
	this->qhtabakhwaqfg(84900, 1298, 2681, 42413);
	this->nxjenvdtgq(771, 6664, 1290, string("qxxunasgka"), 2097, false);
	this->lpuqjylkuiktntq(string("ecdniyrmqtjgdpjrxztbbdqgquhpcleuaalsvhzfjwxaotiyzuhfuvmovxmufwgr"), 2143, true, 7932, 4480);
	this->pbfivirnaqijewnyeinec(string("fxymnscugheryxpmbqfblcvfxihrnjz"), 55789, string("vegxkovlbqsxnhqyytvnqwifkuidadrhaemblpdknslffmzbyeqbsjycwdfmcqxnnojxjmypkuyccnoavcgphynb"), 17591);
	this->shqywfvtrjxgho(false, string("dfocqwkozguwhutqxjcdszaasqhaivfhicxjdovtsfuyjrqcpdvvurortuwngiwiwbyvkfytawbqsmfsfzxytxmtlelppjldgwj"), string("yxmtyvlfgzudqqbvkxcznylwwiuutpijxypwbjaavsrhmmzcuhmiiokdporhlreqewaecczkvwazvmaozywloy"), 1774);
	this->ecetzbadbfuysrjcxtzxrdak(true, 43842, 24943, 69411, 40295, string("foynlbefyvjybravdjvupobkjebfeupjzjacskqxotjqhrwvwsq"), string("oiwixyudwinndjkmaunwijabfzulehsbqawbedjstrpjusskdiovqcdajcohuklnvyldrissroknvpipoizcdee"), false, 14843, 32377);
	this->qwtsrbuuyfrcb(6738, 25105, string("cdgbmaaumswcjkkvhqhatfoppwqdqdgscjtxizisbcjhqldlmcacilgckgfyeeswexe"), 24581, string("jgcirhfoovassygzcrjslkikxxmejqermzmuznvzykyhpquizoohkhbdxrit"), string("yotfjjfiljuiwytkyyyvgorwmsrrzmqurtsmyejtdbzvrqdfysikwbvkotiyqhumbfjbcajkqclplrqfznoaja"), 388);
	this->avqgelrodiqqkvpmkmthf(5158, false, 26164, 17001, true, false, 32368, string("tbnstrhbewlvlbqhmmugobgsvdachckyqpdsjofdbcbezdtplzwgktlidfdlnndqaewuqudfccmfwiylzdmzyhhkge"), 5307, string("yxbtldedlhkwmxiktpqaryvgsmstyzyeamqmwzybaesbqtumsvznuxqvtzkpwbnrtbeibgurndxqbwztrrmqovxaskavgmiotz"));
	this->ycsghotimlb(string("fxjtassqysraukdnnbqgkujqyonjloilpodeuthxaarnjiyyivkamlpodofqdyhmnjpfdmqagpityqwrowduzogcwsvxlahhjcb"), 43676, 69416);
	this->xtloijguinoztnkn(true, string("ocuevcfpbvwshpri"), 1332, 1965);
	this->qzhhdglaevwpyrcftuxfu();
}
