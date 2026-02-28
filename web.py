#!/usr/bin/env python3
"""
Router Access Tool - EDUKASI ONLY
Program untuk mengakses dan mencoba login ke router WiFi
Gunakan hanya pada jaringan milik sendiri!
Fitur: deteksi gateway, coba kredensial default, buka antarmuka router
"""

import os
import sys
import platform
import subprocess
import requests
import socket
import urllib3
from urllib.parse import urlparse
import time
import threading
from datetime import datetime
import json
import re

# Nonaktifkan peringatan SSL untuk koneksi lokal (aman karena hanya ke router sendiri)
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

class RouterAccessTool:
    def __init__(self):
        self.os_type = platform.system().lower()
        self.gateway_ip = None
        self.router_url = None
        self.router_brand = None
        self.session = requests.Session()
        self.session.verify = False  # Untuk koneksi HTTPS lokal
        self.session.timeout = 5
        
        # Default credentials dari berbagai merek router [citation:7][citation:9][citation:4]
        self.default_credentials = [
            # Format: (username, password, brand)
            ("admin", "admin", "Generic/Linksys/TP-Link/ASUS"),
            ("admin", "", "D-Link/Generic"),  # Password kosong
            ("admin", "password", "Netgear"),
            ("admin", "1234", "Generic"),
            ("root", "admin", "Linux-based"),
            ("root", "", "Linux-based"),
            ("Administrator", "admin", "Windows-based"),
            ("user", "user", "Generic"),
            ("cisco", "cisco", "Cisco") [citation:6],
            ("webui", "cisco", "Cisco WebUI") [citation:6],
            ("admin", "default", "Generic"),
            ("admin", "123456", "Generic"),
            ("admin", "password123", "Generic"),
            ("support", "support", "Older routers"),
            ("", "admin", "Some Linksys models") [citation:1],
        ]
        
        # Brand-specific credentials dari berbagai sumber [citation:2][citation:10]
        self.brand_credentials = {
            "tp-link": [
                ("admin", "admin"),
                ("admin", "1234"),
                ("admin", "password"),
            ],
            "d-link": [
                ("admin", ""),
                ("admin", "admin"),
                ("user", "user"),
            ],
            "linksys": [
                ("", "admin"),
                ("admin", "admin"),
                ("admin", "password"),
            ],
            "netgear": [
                ("admin", "password"),
                ("admin", "1234"),
                ("admin", "admin"),
            ],
            "asus": [
                ("admin", "admin") [citation:9],
                ("admin", "password"),
            ],
            "cisco": [
                ("cisco", "cisco") [citation:6],
                ("admin", "admin"),
                ("root", "root"),
            ],
            "huawei": [
                ("admin", "admin"),
                ("root", "admin"),
                ("telecomadmin", "admintelecom"),
            ],
            "3com": [
                ("admin", "admin") [citation:7],
                ("root", "root"),
            ],
            "2wire": [
                ("", "admin"),  # 2Wire sering tanpa username [citation:7]
            ],
            "mikrotik": [
                ("admin", ""),
                ("admin", "admin"),
            ],
            "ubiquiti": [
                ("ubnt", "ubnt"),
                ("admin", "ubnt"),
            ],
            "zyxel": [
                ("admin", "1234"),
                ("admin", "admin"),
                ("", "1234"),
            ],
            "sagemcom": [
                ("admin", "admin"),
                ("admin", "password"),
            ],
            "technicolor": [
                ("admin", "admin"),
                ("admin", "password"),
                ("administrator", "administrator"),
            ],
        }
        
        # Tambahkan kredensial dari ISP tertentu [citation:2]
        self.isp_credentials = {
            "australia": [
                ("admin", "act@123"),  # ACT
                ("admin", "support180"),  # AGL
                ("admin1", "admin1"),  # Amaysim
                ("admin", "apbroadband$"),  # Australia Post
                ("admin", "mate12345"),  # Mate
            ],
            "new_zealand": [
                ("admin", "password1"),
                ("admin", "admin"),
            ],
            "singapore": [
                ("admin", "StarHub@1234"),  # StarHub [citation:2]
                ("admin", "starhub@1234"),
                ("admin", "StarHub@123"),
            ],
        }
    
    def print_banner(self):
        """Tampilkan banner program"""
        banner = """
╔══════════════════════════════════════════════════════════╗
║              ROUTER ACCESS TOOL - EDUKASI               ║
║                    Personal Use Only                     ║
║         Gunakan hanya pada router milik sendiri!         ║
╚══════════════════════════════════════════════════════════╝
        """
        print(banner)
    
    def get_gateway_windows(self):
        """Dapatkan IP gateway di Windows menggunakan ipconfig [citation:4]"""
        try:
            result = subprocess.run(['ipconfig'], capture_output=True, text=True)
            lines = result.stdout.split('\n')
            
            for i, line in enumerate(lines):
                if 'Default Gateway' in line or 'Gateway' in line:
                    # Format: "Default Gateway . . . . . . . . . : 192.168.1.1"
                    parts = line.split(':')
                    if len(parts) > 1:
                        gateway = parts[1].strip()
                        if gateway and gateway != '0.0.0.0':
                            return gateway
                
                # Cek juga di baris berikutnya
                if i > 0 and ('Default Gateway' in lines[i-1] or 'Gateway' in lines[i-1]):
                    gateway = line.strip()
                    if gateway and gateway != '0.0.0.0':
                        return gateway
            
            # Alternatif: cek route print
            result = subprocess.run(['route', 'print', '0.0.0.0'], capture_output=True, text=True)
            for line in result.stdout.split('\n'):
                if '0.0.0.0' in line and 'On-link' not in line:
                    parts = line.split()
                    if len(parts) >= 3:
                        return parts[2]  # Gateway IP
            
            return None
        except Exception as e:
            print(f"Error: {e}")
            return None
    
    def get_gateway_linux(self):
        """Dapatkan IP gateway di Linux"""
        try:
            result = subprocess.run(['ip', 'route'], capture_output=True, text=True)
            for line in result.stdout.split('\n'):
                if 'default' in line:
                    parts = line.split()
                    if len(parts) >= 3:
                        return parts[2]  # Gateway IP
            
            # Alternatif: route -n
            result = subprocess.run(['route', '-n'], capture_output=True, text=True)
            for line in result.stdout.split('\n'):
                if line.startswith('0.0.0.0'):
                    parts = line.split()
                    if len(parts) >= 2:
                        return parts[1]  # Gateway IP
            
            return None
        except Exception as e:
            print(f"Error: {e}")
            return None
    
    def get_gateway(self):
        """Deteksi IP gateway berdasarkan OS"""
        if self.os_type == 'windows':
            return self.get_gateway_windows()
        else:
            return self.get_gateway_linux()
    
    def detect_router_brand(self, ip):
        """Coba deteksi merek router dari halaman login atau response headers"""
        try:
            # Coba akses halaman utama
            urls = [
                f"http://{ip}",
                f"https://{ip}",
                f"http://{ip}/status",
                f"http://{ip}/login",
                f"http://{ip}/index.html",
            ]
            
            for url in urls:
                try:
                    response = self.session.get(url, timeout=3)
                    if response.status_code == 200:
                        # Cek dari title
                        title_match = re.search(r'<title>(.*?)</title>', response.text, re.IGNORECASE)
                        if title_match:
                            title = title_match.group(1).lower()
                            for brand in self.brand_credentials.keys():
                                if brand in title:
                                    return brand
                        
                        # Cek dari konten
                        for brand in self.brand_credentials.keys():
                            if brand in response.text.lower():
                                return brand
                        
                        # Cek header server
                        server = response.headers.get('Server', '').lower()
                        for brand in self.brand_credentials.keys():
                            if brand in server:
                                return brand
                    
                except:
                    continue
            
            return "generic"
            
        except Exception as e:
            return "generic"
    
    def test_connection(self, url):
        """Test koneksi ke URL router"""
        try:
            response = self.session.get(url, timeout=3)
            return response.status_code == 200
        except:
            return False
    
    def try_login_url(self, url, username, password):
        """Coba login dengan kombinasi username/password"""
        try:
            # Beberapa format umum form login
            login_data = {
                'username': username,
                'user': username,
                'login': username,
                'name': username,
                'auth_user': username,
                'email': username,
                
                'password': password,
                'pass': password,
                'pwd': password,
                'passwd': password,
                'auth_pass': password,
            }
            
            # Coba POST ke berbagai endpoint
            login_urls = [
                url,
                f"{url}/login",
                f"{url}/cgi-bin/luci",
                f"{url}/cgi-bin/login",
                f"{url}/api/login",
                f"{url}/goform/login",
                f"{url}/auth",
                f"{url}/authenticate",
            ]
            
            for login_url in login_urls:
                try:
                    # Coba POST
                    response = self.session.post(login_url, data=login_data, timeout=3)
                    
                    # Cek apakah login berhasil (redirect atau konten tertentu)
                    if response.status_code == 200:
                        # Cek apakah ada indikator login gagal
                        fail_indicators = ['invalid', 'failed', 'error', 'wrong', 'incorrect']
                        if not any(ind in response.text.lower() for ind in fail_indicators):
                            return True, login_url
                    
                    # Coba basic auth
                    response = self.session.get(login_url, auth=(username, password), timeout=3)
                    if response.status_code == 200:
                        return True, login_url
                        
                except:
                    continue
            
            return False, None
            
        except Exception as e:
            return False, None
    
    def try_default_credentials(self, ip):
        """Coba semua kombinasi username/password default"""
        print("\n[*] Mencoba login dengan kredensial default...")
        
        # Format URL yang mungkin
        url_formats = [
            f"http://{ip}",
            f"https://{ip}",
            f"http://{ip}:8080",
            f"http://{ip}:8443",
        ]
        
        # Test koneksi ke URL yang mungkin
        working_url = None
        for url in url_formats:
            if self.test_connection(url):
                working_url = url
                print(f"[✓] Router terdeteksi di: {working_url}")
                break
        
        if not working_url:
            print("[!] Tidak dapat terhubung ke router. Cek IP dan koneksi.")
            return None, None
        
        self.router_url = working_url
        
        # Deteksi merek router
        brand = self.detect_router_brand(ip)
        print(f"[*] Merek terdeteksi: {brand.upper()}")
        
        # Kumpulkan semua kredensial yang akan dicoba
        credentials_to_try = []
        
        # Tambahkan kredensial default umum
        credentials_to_try.extend(self.default_credentials)
        
        # Tambahkan kredensial spesifik brand
        if brand in self.brand_credentials:
            for user, pwd in self.brand_credentials[brand]:
                credentials_to_try.append((user, pwd, brand.upper()))
        
        # Tambahkan kredensial ISP (jika relevan)
        credentials_to_try.extend([(u, p, "ISP") for u, p in self.isp_credentials.get("singapore", [])])
        credentials_to_try.extend([(u, p, "ISP") for u, p in self.isp_credentials.get("australia", [])])
        
        # Hapus duplikat
        seen = set()
        unique_creds = []
        for cred in credentials_to_try:
            key = (cred[0], cred[1])
            if key not in seen:
                seen.add(key)
                unique_creds.append(cred)
        
        print(f"[*] Mencoba {len(unique_creds)} kombinasi...")
        
        # Coba login
        found = False
        for i, (username, password, source) in enumerate(unique_creds, 1):
            # Tampilkan progress
            if i % 10 == 0:
                print(f"    Progress: {i}/{len(unique_creds)} kombinasi...")
            
            success, login_url = self.try_login_url(working_url, username, password)
            if success:
                print(f"\n[✓] LOGIN BERHASIL!")
                print(f"    URL: {login_url or working_url}")
                print(f"    Username: {username}")
                print(f"    Password: {password}")
                print(f"    Sumber: {source}")
                
                # Simpan hasil
                self.save_result(ip, username, password, login_url or working_url)
                
                # Buka browser
                self.open_browser(login_url or working_url, username, password)
                
                return username, password
            
            # Beri jeda agar tidak terlalu cepat
            time.sleep(0.1)
        
        print("\n[!] Tidak ada kombinasi yang berhasil.")
        return None, None
    
    def save_result(self, ip, username, password, url):
        """Simpan hasil login ke file"""
        try:
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            filename = f"router_access_{timestamp}.txt"
            
            with open(filename, 'w') as f:
                f.write("="*50 + "\n")
                f.write("ROUTER ACCESS RESULT\n")
                f.write("="*50 + "\n")
                f.write(f"Waktu: {datetime.now()}\n")
                f.write(f"IP Router: {ip}\n")
                f.write(f"URL: {url}\n")
                f.write(f"Username: {username}\n")
                f.write(f"Password: {password}\n")
                f.write("="*50 + "\n")
                f.write("CATATAN: Simpan kredensial ini di tempat aman!\n")
            
            print(f"[✓] Hasil disimpan ke: {filename}")
        except:
            pass
    
    def open_browser(self, url, username, password):
        """Buka browser ke halaman login (opsional)"""
        try:
            import webbrowser
            
            print("\n[*] Membuka browser...")
            
            # Untuk beberapa router, kita perlu memasukkan kredensial di URL
            # Format: http://username:password@ip/
            parsed = urlparse(url)
            auth_url = f"{parsed.scheme}://{username}:{password}@{parsed.netloc}{parsed.path}"
            
            # Tanya user
            response = input("\nBuka browser sekarang? (y/n): ").strip().lower()
            if response == 'y':
                webbrowser.open(auth_url)
                print("[✓] Browser dibuka. Jika login otomatis gagal, masukkan manual.")
        except:
            pass
    
    def scan_common_ips(self):
        """Scan IP umum untuk router (jika gateway tidak terdeteksi)"""
        common_ips = [
            "192.168.1.1", "192.168.0.1", "192.168.2.1", "192.168.100.1",
            "10.0.0.1", "10.0.0.138", "172.16.0.1", "192.168.10.1",
            "192.168.1.254", "192.168.0.254", "192.168.254.254" [citation:7]
        ]
        
        print("\n[*] Mencari router di IP umum...")
        
        for ip in common_ips:
            print(f"    Mencoba {ip}...", end=" ")
            try:
                response = self.session.get(f"http://{ip}", timeout=1)
                if response.status_code < 500:  # Ada respons
                    print("BERHASIL")
                    return ip
                else:
                    print("Gagal")
            except:
                print("Timeout")
        
        return None
    
    def interactive_menu(self):
        """Menu interaktif"""
        while True:
            print("\n" + "="*60)
            print("                       MENU ROUTER ACCESS")
            print("="*60)
            print("1.  Deteksi otomatis gateway & coba login")
            print("2.  Masukkan IP router manual")
            print("3.  Scan IP umum router")
            print("4.  Lihat daftar kredensial default")
            print("5.  Cek koneksi ke router")
            print("6.  Bantuan & informasi")
            print("0.  Keluar")
            
            choice = input("\nPilih menu: ").strip()
            
            if choice == '1':
                print("\n[*] Mendeteksi gateway...")
                gateway = self.get_gateway()
                
                if gateway:
                    print(f"[✓] Gateway terdeteksi: {gateway}")
                    self.gateway_ip = gateway
                    self.try_default_credentials(gateway)
                else:
                    print("[!] Gateway tidak terdeteksi.")
                    print("    Coba menu 2 atau 3 untuk mencari router.")
            
            elif choice == '2':
                ip = input("\nMasukkan IP router (contoh: 192.168.1.1): ").strip()
                if ip:
                    self.gateway_ip = ip
                    self.try_default_credentials(ip)
                else:
                    print("[!] IP tidak valid")
            
            elif choice == '3':
                ip = self.scan_common_ips()
                if ip:
                    print(f"\n[✓] Router ditemukan di {ip}")
                    response = input("Coba login sekarang? (y/n): ").strip().lower()
                    if response == 'y':
                        self.gateway_ip = ip
                        self.try_default_credentials(ip)
                else:
                    print("[!] Tidak menemukan router. Pastikan terhubung ke WiFi.")
            
            elif choice == '4':
                print("\n[*] Daftar kredensial default umum [citation:7][citation:9]:")
                print("    " + "="*50)
                
                # Tampilkan 20 kredensial pertama
                for i, (user, pwd, brand) in enumerate(self.default_credentials[:20], 1):
                    print(f"    {i}. {brand}: {user}/{pwd if pwd else '(kosong)'}")
                
                print("\n    [*] Merek spesifik:")
                for brand, creds in list(self.brand_credentials.items())[:5]:
                    print(f"    - {brand.upper()}: {creds[0][0]}/{creds[0][1]}")
            
            elif choice == '5':
                if not self.gateway_ip:
                    ip = input("Masukkan IP router: ").strip()
                    if not ip:
                        continue
                    self.gateway_ip = ip
                
                print(f"\n[*] Cek koneksi ke {self.gateway_ip}...")
                
                # Ping
                if self.os_type == 'windows':
                    cmd = ['ping', '-n', '2', self.gateway_ip]
                else:
                    cmd = ['ping', '-c', '2', self.gateway_ip]
                
                try:
                    result = subprocess.run(cmd, capture_output=True, text=True)
                    if result.returncode == 0:
                        print("[✓] Router merespons ping")
                    else:
                        print("[!] Router tidak merespons ping")
                except:
                    print("[!] Gagal ping")
                
                # Cek HTTP
                try:
                    response = self.session.get(f"http://{self.gateway_ip}", timeout=2)
                    print(f"[✓] HTTP port 80 terbuka (status: {response.status_code})")
                except:
                    print("[!] HTTP port 80 tidak dapat diakses")
                
                try:
                    response = self.session.get(f"https://{self.gateway_ip}", timeout=2)
                    print(f"[✓] HTTPS port 443 terbuka (status: {response.status_code})")
                except:
                    print("[!] HTTPS port 443 tidak dapat diakses")
            
            elif choice == '6':
                print("\n[*] BANTUAN & INFORMASI")
                print("="*50)
                print("Program ini untuk mengakses router milik sendiri.")
                print("\nIP umum router [citation:7][citation:4]:")
                print("- 192.168.1.1 (Linksys, TP-Link, ASUS)")
                print("- 192.168.0.1 (D-Link, Netgear)")
                print("- 192.168.1.254 (2Wire, beberapa ISP)")
                print("- 192.168.2.1 (Broadband)")
                print("\nKredensial default umum [citation:9][citation:1][citation:7]:")
                print("- admin/admin (Linksys, TP-Link, ASUS, 3Com)")
                print("- admin/(kosong) (D-Link, beberapa model)")
                print("- admin/password (Netgear)")
                print("- cisco/cisco (Cisco)")
                print("\nJika tidak bisa login, reset router dengan tombol fisik.")
                print("Peringatan: Reset akan menghapus semua pengaturan! [citation:4]")
            
            elif choice == '0':
                print("\nTerima kasih! Ingat: Gunakan hanya untuk router sendiri!")
                break

def main():
    tool = RouterAccessTool()
    tool.print_banner()
    
    # Cek koneksi internet minimal
    try:
        socket.gethostbyname("google.com")
    except:
        print("[!] Peringatan: Koneksi internet mungkin bermasalah.")
        print("    Pastikan terhubung ke WiFi untuk mengakses router.")
    
    # Jalankan menu interaktif
    tool.interactive_menu()

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\n[!] Program dihentikan user")
        sys.exit(0)
    except Exception as e:
        print(f"\n[!] Error: {e}")
        sys.exit(1)
