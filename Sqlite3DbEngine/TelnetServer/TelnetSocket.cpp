#include "TelnetSocket.h"
#include <assert.h>
#include <stdio.h>


CTelnetSocket::CTelnetSocket()
{
	m_pReadBuf   = new char[nSizeRecv];
	m_nReadBuf   = 0;
	m_wFlags     = 0;
	m_pLoginName = NULL;
}

CTelnetSocket::~CTelnetSocket()
{
	delete [] m_pReadBuf;
	if(m_pLoginName) delete [] m_pLoginName;
}

bool CTelnetSocket::SetLoginName(const char* pname)
{
	m_pLoginName=(char*)malloc(strlen(pname)+1);
	if(!m_pLoginName)
		return false;

	strcpy(m_pLoginName,pname);
	m_pLoginName[strlen(pname)]='\0';
	return true;
}

int CTelnetSocket::ReadLine(char* pch, const int nSize, const int nSecs)
	// reads an entire header line through CRLF (or socket close)
	// inserts zero string terminator, object maintains a buffer
{
	int nBytesThisTime = m_nReadBuf;
	int nLineLength = 0;
	char* pch1 = m_pReadBuf;
	char* pch2;
	do {
		// look for lf (assume preceded by cr)
		if((pch2 = (char*) memchr(pch1 , '\n', nBytesThisTime)) != NULL) {
			ASSERT((pch2) > m_pReadBuf);
			ASSERT(*(pch2 - 1) == '\r');
			nLineLength = (pch2 - m_pReadBuf) + 1;
			if(nLineLength >= nSize) nLineLength = nSize - 1;
			memcpy(pch, m_pReadBuf, nLineLength); // copy the line to caller
			m_nReadBuf -= nLineLength;
			memmove(m_pReadBuf, pch2 + 1, m_nReadBuf); // shift remaining characters left
			break;
		}
		pch1 += nBytesThisTime;
		nBytesThisTime = Receive(m_pReadBuf + m_nReadBuf, nSizeRecv - m_nReadBuf, nSecs);
		if(nBytesThisTime <= 0) { // sender closed socket or line longer than buffer
			throw "ReadHeaderLine";
		}

		//update telnet session
		try 
		{
			Write(m_pReadBuf + m_nReadBuf,nBytesThisTime);
		}
		catch(const char* e) 
		{
			e=e;
			throw "ReadHeaderLine::write";
		}

		m_nReadBuf += nBytesThisTime;
	}
	while(TRUE);
	*(pch + nLineLength-2) = '\0';	//add rest of buffer, and remove lfcr
	return nLineLength;
}

int CTelnetSocket::ReadResponse(char* pch, const int nSize, const int nSecs)
	// reads remainder of a transmission through buffer full or socket close
	// (assume headers have been read already)
{
	int nBytesToRead, nBytesThisTime, nBytesRead = 0;
	if(m_nReadBuf > 0) { // copy anything already in the recv buffer
		memcpy(pch, m_pReadBuf, m_nReadBuf);
		pch += m_nReadBuf;
		nBytesRead = m_nReadBuf;
		m_nReadBuf = 0;
	}
	do { // now pass the rest of the data directly to the caller
		nBytesToRead = min(nSizeRecv, nSize - nBytesRead);
		nBytesThisTime = Receive(pch, nBytesToRead, nSecs);
		if(nBytesThisTime <= 0) break; // sender closed the socket
		pch += nBytesThisTime;
		nBytesRead += nBytesThisTime;
	}
	while(nBytesRead <= nSize);
	return nBytesRead;
}

int CTelnetSocket::Print(const char* msg)
{
	int iBytes;
	char* buf=(char*)malloc(strlen(msg)+3);
	sprintf(buf,"%s\r\n",msg);
	iBytes=Write(buf,strlen(buf));
	free(buf);
	return iBytes;
}
