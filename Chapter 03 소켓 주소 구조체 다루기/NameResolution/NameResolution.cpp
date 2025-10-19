#include "../../Common.h" // 세미콜론 금지!

#define TESTNAME "www.google.com"

// 도메인 이름 -> IPv4 주소 (getaddrinfo 사용)
bool GetIpAddr(const char* name, in_addr* outAddr)
{
    addrinfo hints{};
    hints.ai_family = AF_INET;      // IPv4만
    hints.ai_socktype = SOCK_STREAM;  // 아무거나 OK, 관습적으로 설정
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* res = nullptr;
    int rc = getaddrinfo(name, nullptr, &hints, &res);
    if (rc != 0 || !res) {
        err_display("getaddrinfo()");
        return false;
    }

    // 첫 결과에서 IPv4 주소 추출
    const sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(res->ai_addr);
    *outAddr = sin->sin_addr;

    freeaddrinfo(res);
    return true;
}

// IPv4 주소 -> 도메인 이름 (getnameinfo 사용)
bool GetDomainName(in_addr addr, char* name, size_t nameLen)
{
    sockaddr_in sin{};
    sin.sin_family = AF_INET;
    sin.sin_addr = addr;
    sin.sin_port = 0;

    // NI_NAMEREQD: 역방향 조회 실패 시 에러 반환(없으면 리터럴 IP를 주기도 함)
    int rc = getnameinfo(
        reinterpret_cast<sockaddr*>(&sin), sizeof(sin),
        name, static_cast<DWORD>(nameLen),
        nullptr, 0, NI_NAMEREQD
    );
    if (rc != 0) {
        err_display("getnameinfo()");
        return false;
    }
    return true;
}

int main()
{
    // 윈속 초기화
    WSADATA wsa{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 0;

    std::printf("도메인 이름(변환 전) = %s\n", TESTNAME);

    // 도메인 이름 -> IP 주소
    in_addr addr{};
    if (GetIpAddr(TESTNAME, &addr)) {
        char ipStr[INET_ADDRSTRLEN]{};
        // Windows에서 inet_ntop은 <ws2tcpip.h>에 선언
        if (inet_ntop(AF_INET, &addr, ipStr, sizeof(ipStr)) == nullptr) {
            err_display("inet_ntop()");
        }
        else {
            std::printf("IP 주소(변환 후) = %s\n", ipStr);
        }

        // IP 주소 -> 도메인 이름
        char name[256]{};
        if (GetDomainName(addr, name, sizeof(name))) {
            std::printf("도메인 이름(다시 변환 후) = %s\n", name);
        }
        else {
            std::printf("역방향 조회 실패(해당 PTR 레코드가 없을 수 있음)\n");
        }
    }

    WSACleanup();
    return 0;
}