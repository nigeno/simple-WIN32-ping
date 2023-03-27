#pragma once

#include <string>
#include <windows.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")

HANDLE icmpFile;
unsigned long addr = INADDR_NONE;
DWORD dwRetVal = 0;
char SendData[32] = "Data buffer";
LPVOID ReplyBuffer = NULL;
DWORD ReplySize = 0;

// https://github.com/philekiert/ping/blob/main/Ping.cc
// https://learn.microsoft.com/en-us/windows/win32/api/icmpapi/nf-icmpapi-icmpsendecho

namespace Ping
{
	/* Return values
	----------------
	0 Successful
	1 Failure*/

    unsigned Ping(const std::string& ip, const unsigned& timeout) // timeout in ms
    {
        addr = inet_addr(ip.c_str()); // inet_addr converts a string into an IP address.
        if (addr == INADDR_NONE)
            return 1; // inet_addr() returns -1 on invalid IP.

        icmpFile = IcmpCreateFile();
        if (icmpFile == INVALID_HANDLE_VALUE) {} // On failure to create handle.

        ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
        ReplyBuffer = (VOID*)malloc(ReplySize); // Reserve memory for replyBuffer.
        if (ReplyBuffer == NULL)
            return 1; // On failure to allocate memory.

        dwRetVal = IcmpSendEcho(icmpFile, addr, SendData, sizeof(SendData),
            NULL, ReplyBuffer, ReplySize, timeout); // Timeout value is DWORD.

        if (dwRetVal != 0)
        {
            PICMP_ECHO_REPLY echo = (PICMP_ECHO_REPLY)ReplyBuffer;
            ULONG status = echo->Status;

            if (status != 0)
                return 1;
        }
        else
            return 1;

        IcmpCloseHandle(icmpFile);

        return 0;
    }

}