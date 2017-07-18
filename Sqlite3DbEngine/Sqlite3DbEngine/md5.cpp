#include "StdAfx.h"
#include "MD5.h"

using namespace ATL;

// Given a CFile object, compute the MD5_File digest 
MD5_File::MD5_File( FILE* InputFile) 
{ 
	// Initialize the digest. 
	a = A_INIT; 
	b = B_INIT; 
	c = C_INIT; 
	d = D_INIT; 

	// Now init the bit length of the input. 
	count[0] = 0; 
	count[1] = 0; 

	// Now perform transformation on every block of input. 
	do 
	{ 
		// Read a block from the input. 
		block_idx = (int)fread(manip_block,sizeof(unsigned char),MD5_BLOCK_SIZE,InputFile);
		//block_idx = InputFile.Read( manip_block, MD5_BLOCK_SIZE); 

		if (block_idx == MD5_BLOCK_SIZE) 
		{ 
			// A full block is ready to be transformed.  Do so. 
			MD5_IncCount( MD5_BLOCK_SIZE ); 
			MD5_Xform(); 
		} 
		else 
		{ 
			MD5_IncCount( block_idx ); 

			// If the count of bytes read is not a full block, 
			// the end of the file has been found, and the ending should be prepared. 
			if (block_idx < (MD5_PAD_SIZE) ) 
			{ 
				// There is enough room in the block, to pad up to the correct size. 
				// Pad and then transform. 
				MD5_Pad( normal ); 
				MD5_Xform(); 
			} 
			else 
			{ 
				// The block is already up to or beyond the pad size. Pad to the 
				// end and of the current block, and then transform it. 
				MD5_Pad( to_end ); 
				MD5_Xform(); 


				// Now create another block that is empty and pad all the way. 
				block_idx = 0; 
				MD5_Pad( finish ); 
				MD5_Xform(); 
			} 
		} 
	} 
	while (block_idx == MD5_BLOCK_SIZE); 
} 

MD5_File::MD5_File(unsigned char *szMD5)
{
	a = A_INIT; 
	b = B_INIT; 
	c = C_INIT; 
	d = D_INIT; 

	// Now init the bit length of the input. 
	count[0] = 0; 
	count[1] = 0;
	CString strMD5(szMD5);
	::memset(manip_block,0,64);
	::memcpy_s(manip_block,64,szMD5,strMD5.GetLength());
	strMD5.Empty();
	strMD5=manip_block;
	block_idx = strMD5.GetLength();

	MD5_IncCount( block_idx );
	MD5_Pad( normal ); 
	MD5_Xform(); 
}

// Perform the padding functions. 
void MD5_File::MD5_Pad( MD5_PadMode PadMode ) 
{ 
	switch( PadMode ) 
	{ 
	case normal: 
		// Perform normal padding. 
		manip_block[block_idx++] = MD5_PAD_FIRST_BYTE; 
		while (block_idx < MD5_PAD_SIZE) 
			manip_block[block_idx++] = 0; 

		// Now store count in this last block. 
		MD5_StoreCnt(); 
		break; 

	case to_end: 
		// Pad until the end of the block. 
		manip_block[block_idx++] = MD5_PAD_FIRST_BYTE; 
		while (block_idx < MD5_BLOCK_SIZE) 
			manip_block[block_idx++] = 0; 
		break; 

	case finish: 
		// Finish padding. 
		while (block_idx < MD5_PAD_SIZE) 
			manip_block[block_idx++] = 0; 


		// Now store count in this last block. 
		MD5_StoreCnt(); 
		break; 
	} 
} 

// Examine the manip_block, and stir it into the digest under construction. 
void MD5_File::MD5_Xform() 
{ 
	// Save orig values of a,b,c,d. 
	UINT4 a_orig = a; 
	UINT4 b_orig = b; 
	UINT4 c_orig = c; 
	UINT4 d_orig = d; 

	// Now decode the manip_block into xform_buf; 
	MD5_Decode(); 

	// Round 1. 
	// Let [abcd k s i] denote the operation 
	// a = b + ((a + F(b,c,d) + xform_buf[k] + T[i]) <<< s). 
	// Do the following 16 operations. 
	//  [ABCD  0  7  1]  [DABC  1 12  2]  [CDAB  2 17  3]  [BCDA  3 22  4] 
	//  [ABCD  4  7  5]  [DABC  5 12  6]  [CDAB  6 17  7]  [BCDA  7 22  8] 
	//  [ABCD  8  7  9]  [DABC  9 12 10]  [CDAB 10 17 11]  [BCDA 11 22 12] 
	//  [ABCD 12  7 13]  [DABC 13 12 14]  [CDAB 14 17 15]  [BCDA 15 22 16] 

	FF (a, b, c, d, xform_buf[ 0], S11, T1); 
	FF (d, a, b, c, xform_buf[ 1], S12, T2); 
	FF (c, d, a, b, xform_buf[ 2], S13, T3); 
	FF (b, c, d, a, xform_buf[ 3], S14, T4); 
	FF (a, b, c, d, xform_buf[ 4], S11, T5); 
	FF (d, a, b, c, xform_buf[ 5], S12, T6); 
	FF (c, d, a, b, xform_buf[ 6], S13, T7); 
	FF (b, c, d, a, xform_buf[ 7], S14, T8); 
	FF (a, b, c, d, xform_buf[ 8], S11, T9); 
	FF (d, a, b, c, xform_buf[ 9], S12, T10); 
	FF (c, d, a, b, xform_buf[10], S13, T11); 
	FF (b, c, d, a, xform_buf[11], S14, T12); 
	FF (a, b, c, d, xform_buf[12], S11, T13); 
	FF (d, a, b, c, xform_buf[13], S12, T14); 
	FF (c, d, a, b, xform_buf[14], S13, T15); 
	FF (b, c, d, a, xform_buf[15], S14, T16); 

	// Round 2. 
	// Let [abcd k s i] denote the operation 
	// a = b + ((a + G(b,c,d) + xform_buf[k] + T[i]) <<< s). 
	// Do the following 16 operations. 
	//  [ABCD  1  5 17]  [DABC  6  9 18]  [CDAB 11 14 19]  [BCDA  0 20 20] 
	//  [ABCD  5  5 21]  [DABC 10  9 22]  [CDAB 15 14 23]  [BCDA  4 20 24] 
	//  [ABCD  9  5 25]  [DABC 14  9 26]  [CDAB  3 14 27]  [BCDA  8 20 28] 
	//  [ABCD 13  5 29]  [DABC  2  9 30]  [CDAB  7 14 31]  [BCDA 12 20 32] 

	GG (a, b, c, d, xform_buf[ 1], S21, T17); 
	GG (d, a, b, c, xform_buf[ 6], S22, T18); 
	GG (c, d, a, b, xform_buf[11], S23, T19); 
	GG (b, c, d, a, xform_buf[ 0], S24, T20); 
	GG (a, b, c, d, xform_buf[ 5], S21, T21); 
	GG (d, a, b, c, xform_buf[10], S22, T22); 
	GG (c, d, a, b, xform_buf[15], S23, T23); 
	GG (b, c, d, a, xform_buf[ 4], S24, T24); 
	GG (a, b, c, d, xform_buf[ 9], S21, T25); 
	GG (d, a, b, c, xform_buf[14], S22, T26); 
	GG (c, d, a, b, xform_buf[ 3], S23, T27); 
	GG (b, c, d, a, xform_buf[ 8], S24, T28); 
	GG (a, b, c, d, xform_buf[13], S21, T29); 
	GG (d, a, b, c, xform_buf[ 2], S22, T30); 
	GG (c, d, a, b, xform_buf[ 7], S23, T31); 
	GG (b, c, d, a, xform_buf[12], S24, T32); 

	// Round 3. 
	// Let [abcd k s t] denote the operation 
	// a = b + ((a + H(b,c,d) + xform_buf[k] + T[i]) <<< s). 
	// Do the following 16 operations. 
	//  [ABCD  5  4 33]  [DABC  8 11 34]  [CDAB 11 16 35]  [BCDA 14 23 36] 
	//  [ABCD  1  4 37]  [DABC  4 11 38]  [CDAB  7 16 39]  [BCDA 10 23 40] 
	//  [ABCD 13  4 41]  [DABC  0 11 42]  [CDAB  3 16 43]  [BCDA  6 23 44] 
	//  [ABCD  9  4 45]  [DABC 12 11 46]  [CDAB 15 16 47]  [BCDA  2 23 48] 

	HH (a, b, c, d, xform_buf[ 5], S31, T33); 
	HH (d, a, b, c, xform_buf[ 8], S32, T34); 
	HH (c, d, a, b, xform_buf[11], S33, T35); 
	HH (b, c, d, a, xform_buf[14], S34, T36); 
	HH (a, b, c, d, xform_buf[ 1], S31, T37); 
	HH (d, a, b, c, xform_buf[ 4], S32, T38); 
	HH (c, d, a, b, xform_buf[ 7], S33, T39); 
	HH (b, c, d, a, xform_buf[10], S34, T40); 
	HH (a, b, c, d, xform_buf[13], S31, T41); 
	HH (d, a, b, c, xform_buf[ 0], S32, T42); 
	HH (c, d, a, b, xform_buf[ 3], S33, T43); 
	HH (b, c, d, a, xform_buf[ 6], S34, T44); 
	HH (a, b, c, d, xform_buf[ 9], S31, T45); 
	HH (d, a, b, c, xform_buf[12], S32, T46); 
	HH (c, d, a, b, xform_buf[15], S33, T47); 
	HH (b, c, d, a, xform_buf[ 2], S34, T48); 

	// Round 4. 
	// Let [abcd k s t] denote the operation 
	// a = b + ((a + I(b,c,d) + xform_buf[k] + T[i]) <<< s). 
	// Do the following 16 operations. 
	//  [ABCD  0  6 49]  [DABC  7 10 50]  [CDAB 14 15 51]  [BCDA  5 21 52] 
	//  [ABCD 12  6 53]  [DABC  3 10 54]  [CDAB 10 15 55]  [BCDA  1 21 56] 
	//  [ABCD  8  6 57]  [DABC 15 10 58]  [CDAB  6 15 59]  [BCDA 13 21 60] 
	//  [ABCD  4  6 61]  [DABC 11 10 62]  [CDAB  2 15 63]  [BCDA  9 21 64] 

	II (a, b, c, d, xform_buf[ 0], S41, T49); 
	II (d, a, b, c, xform_buf[ 7], S42, T50); 
	II (c, d, a, b, xform_buf[14], S43, T51); 
	II (b, c, d, a, xform_buf[ 5], S44, T52); 
	II (a, b, c, d, xform_buf[12], S41, T53); 
	II (d, a, b, c, xform_buf[ 3], S42, T54); 
	II (c, d, a, b, xform_buf[10], S43, T55); 
	II (b, c, d, a, xform_buf[ 1], S44, T56); 
	II (a, b, c, d, xform_buf[ 8], S41, T57); 
	II (d, a, b, c, xform_buf[15], S42, T58); 
	II (c, d, a, b, xform_buf[ 6], S43, T59); 
	II (b, c, d, a, xform_buf[13], S44, T60); 
	II (a, b, c, d, xform_buf[ 4], S41, T61); 
	II (d, a, b, c, xform_buf[11], S42, T62); 
	II (c, d, a, b, xform_buf[ 2], S43, T63); 
	II (b, c, d, a, xform_buf[ 9], S44, T64); 


	// Now perform the following additions. (That is increment each 
	// of the four registers by the value it had before this block 
	// was started.) 
	a += a_orig; 
	b += b_orig; 
	c += c_orig; 
	d += d_orig; 
} 

// Given a length in bytes, increase the running bit count of the input total. 
void MD5_File::MD5_IncCount( int IncAmt) 
{ 
	UINT4 old_cnt0 = count[0]; 

	count[0] += IncAmt*8; 
	if (count[0] < old_cnt0) 
		count[1]++;      // Carry detection. 
} 

// Take the manip_block and turn it into an unsigned integer array 
// for further manipulation. 
void MD5_File::MD5_Decode() 
{ 
	unsigned int i, j; 

	for (i = 0, j = 0; j < MD5_BLOCK_SIZE; i++, j += sizeof(UINT4)) 
	{
		xform_buf[i] = ((UINT4)manip_block[j]) | (((UINT4)manip_block[j+1]) << 8) | 
			(((UINT4)manip_block[j+2]) << 16) | (((UINT4)manip_block[j+3]) << 24); 
	}
} 

// Put the bit length count into the current manip_block. 
void MD5_File::MD5_StoreCnt() 
{ 
	manip_block[MD5_PAD_SIZE+0]= unsigned int( count[0] & 0xff); 
	manip_block[MD5_PAD_SIZE+1]= unsigned int((count[0] >> 8)  & 0xff); 
	manip_block[MD5_PAD_SIZE+2]= unsigned int((count[0] >> 16) & 0xff); 
	manip_block[MD5_PAD_SIZE+3]= unsigned int((count[0] >> 24) & 0xff); 

	manip_block[MD5_PAD_SIZE+4]= unsigned int(count[1] & 0xff); 
	manip_block[MD5_PAD_SIZE+5]= unsigned int((count[1] >> 8)  & 0xff); 
	manip_block[MD5_PAD_SIZE+6]= unsigned int((count[1] >> 16) & 0xff); 
	manip_block[MD5_PAD_SIZE+7]= unsigned int((count[1] >> 24) & 0xff); 
} 

void MD5_File::GetDigest( unsigned char* Dest) 
{ 
	Dest[0] = unsigned int(a & 0xff); 
	Dest[1] = unsigned int((a >> 8) & 0xff); 
	Dest[2] = unsigned int((a >> 16) & 0xff); 
	Dest[3] = unsigned int((a >> 24) & 0xff); 

	Dest[4] = unsigned int(b & 0xff); 
	Dest[5] = unsigned int((b >> 8) & 0xff); 
	Dest[6] = unsigned int((b >> 16) & 0xff); 
	Dest[7] = unsigned int((b >> 24) & 0xff); 

	Dest[8] = unsigned int(c & 0xff); 
	Dest[9] = unsigned int((c >> 8) & 0xff); 
	Dest[10] = unsigned int((c >> 16) & 0xff); 
	Dest[11] = unsigned int((c >> 24) & 0xff); 

	Dest[12] = unsigned int(d & 0xff); 
	Dest[13] = unsigned int((d >> 8) & 0xff); 
	Dest[14] = unsigned int((d >> 16) & 0xff); 
	Dest[15] = unsigned int((d >> 24) & 0xff); 
} 