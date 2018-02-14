#include "stdafx.h"
#include "xbsocket.h"
 

XBUDPClient::XBUDPClient()
{
	m_bInitSuccess = FALSE;
}
BOOL XBUDPClient::init( char *pcdestAddr,unsigned short usDestPort)
{

	if(NO_ERROR != WSAStartup(MAKEWORD(2, 2), &m_wsaData))
	{
		return FALSE;
	}

    // Create a socket for sending data
    m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == m_sock) 
	{
        WSACleanup();
        return FALSE;
    }
    //---------------------------------------------
    // Set up the RecvAddr structure with the IP address of
    // the receiver (in this example case "192.168.1.1")
    // and the specified port number.
    m_destAddress.sin_family = AF_INET;
    m_destAddress.sin_port = htons(usDestPort);
    m_destAddress.sin_addr.s_addr = inet_addr(pcdestAddr);

	m_bInitSuccess = TRUE;
	return TRUE;
}
int XBUDPClient::sendData(const char *buf, const int ilen)
{

	return sendto(m_sock,buf, ilen, 0, \
		(SOCKADDR *) & m_destAddress, sizeof (m_destAddress));
}
XBUDPClient::~XBUDPClient()
{
	if(m_bInitSuccess)
	{
		closesocket(m_sock);
		WSACleanup();
	}
}









