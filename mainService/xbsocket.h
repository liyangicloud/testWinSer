#pragma once

#include<Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define XB_UDP_BUFFER_LEN 1024

class XBUDPServer{
	private:
		WSADATA m_wsaData ;
		SOCKET m_sock ;
		SOCKADDR m_localAdd;
		int m_iLocalPort;
		BOOL m_bInitSuccess;
    public:
		XBUDPServer();
		~XBUDPServer();

		BOOL init( int ilocalPort);
		BOOL listen();
		BOOL receive(int cbBufferLen,BYTE *pBuffer);
};

class XBUDPClient{
	private:
		WSADATA m_wsaData ;
		SOCKET m_sock ;
		sockaddr_in  m_destAddress;
		unsigned short m_destPort;
		BOOL m_bInitSuccess;
    public:
		XBUDPClient();
		~XBUDPClient();

		BOOL init( char *destAddr,unsigned short usDestPort);
		int sendData(const char *buf, const int ilen);
};
 