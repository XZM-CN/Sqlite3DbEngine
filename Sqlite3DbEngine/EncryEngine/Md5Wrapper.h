#pragma once
class CMd5Wrapper
{
public:
	CMd5Wrapper(void);
	~CMd5Wrapper(void);


public:
	/// ����ַ����Ĺ�ϣֵ
	CString GetStringSha1(const CString& strString);
	CString GetStringMD5(const CString& strString);
};

