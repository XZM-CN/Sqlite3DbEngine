#pragma once

#include "blockingsocket.h"

class CTelnetSocket :
	public CBlockingSocket
{
public:
	enum {nSizeRecv = 1024}; // max receive buffer size (> hdr line length)
	enum {FLAG_VALIDATED=1,FLAG_PLAYING=2};
	CTelnetSocket();
	~CTelnetSocket();
	int ReadLine(char* pch, const int nSize, const int nSecs=DEFAULT_TIMEOUT);
	int ReadResponse(char* pch, const int nSize, const int nSecs=DEFAULT_TIMEOUT);
	int Print(const char* msg);
	bool SetLoginName(const char* name);

public:
	//login information
	char* m_pLoginName;
	WORD m_wFlags;

private:
	char* m_pReadBuf; // read buffer
	int m_nReadBuf; // number of bytes in the read buffer

};
