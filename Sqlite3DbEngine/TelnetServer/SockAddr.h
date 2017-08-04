#pragma once

#include <winsock.h>

// needs winsock.h in the precompiled headers

typedef const struct sockaddr* LPCSOCKADDR;

class CSockAddr :
	public sockaddr_in
{
public:
	// constructors
	CSockAddr() // Default
	{
		sin_family = AF_INET;
		sin_port = 0;
		sin_addr.s_addr = 0;
	}
	CSockAddr(const SOCKADDR& sa) {
		memcpy(this, &sa, sizeof(SOCKADDR));
	}

	CSockAddr(const SOCKADDR_IN& sin) {
		memcpy(this, &sin, sizeof(SOCKADDR_IN));
	}

	CSockAddr(const ULONG ulAddr, const USHORT ushPort = 0) {
		sin_family = AF_INET;
		sin_port = htons(ushPort);
		sin_addr.s_addr = htonl(ulAddr); // parms are host byte ordered
	}

	CSockAddr(const char* pchIP, const USHORT ushPort = 0) // dotted IP addr string
	{
		sin_family = AF_INET;
		sin_port = htons(ushPort);
		sin_addr.s_addr = inet_addr(pchIP); // already network byte ordered
	}

	// Return the address in dotted-decimal format
	char* DottedDecimal()
	{
		return inet_ntoa(sin_addr); // constructs a new CString object
	}

	// Get port and address (even though they're public)
	USHORT Port() const
	{
		return ntohs(sin_port);
	}

	ULONG IPAddr() const
	{
		return ntohl(sin_addr.s_addr);
	}

	// operators added for efficiency
	const CSockAddr& operator=(const SOCKADDR& sa)
	{
		memcpy(this, &sa, sizeof(SOCKADDR));
		return *this;
	}

	const CSockAddr& operator=(const SOCKADDR_IN& sin)
	{
		memcpy(this, &sin, sizeof(SOCKADDR_IN));
		return *this;
	}

	operator SOCKADDR()
	{
		return *((LPSOCKADDR) this);
	}

	operator LPSOCKADDR()
	{
		return (LPSOCKADDR) this; 
	}

	operator LPSOCKADDR_IN()
	{
		return (LPSOCKADDR_IN) this;
	}
};