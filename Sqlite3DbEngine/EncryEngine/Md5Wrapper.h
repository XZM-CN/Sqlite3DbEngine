#pragma once
class CMd5Wrapper
{
public:
	CMd5Wrapper(void);
	~CMd5Wrapper(void);


public:
	/// 获得字符串的哈希值
	CString GetStringSha1(const CString& strString);
	CString GetStringMD5(const CString& strString);
};

