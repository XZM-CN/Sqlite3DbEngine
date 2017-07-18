#pragma once

// 
//  A c++ implementation of the MD5_File algo as described by RFC1321. 
//    All rights and credits as specified therein. 
// 
// This software was written using the Microsoft Visual C++ 1.0 
// compiler, along with the Microsoft Foundation Class Library. 
// It implements a class called MD5_File that provides MD5_File functionality. 
// 
// Although the MD5_File algo operates on inputs of arbitrary bit lengths, 
// this class only operates on eight-bit bytes. 


// Define MD5_File constants as specified in RFC1321. 


// Define the F,G,H, & I functions. 
#define F(x, y, z) (((x) & (y)) | ((~x) & (z))) 
#define G(x, y, z) (((x) & (z)) | ((y) & (~z))) 
#define H(x, y, z) ((x) ^ (y) ^ (z)) 
#define I(x, y, z) ((y) ^ ((x) | (~z))) 


// Define shift constants for the transformation routine. 
#define S11 7 
#define S12 12 
#define S13 17 
#define S14 22 


#define S21 5 
#define S22 9 
#define S23 14 
#define S24 20 


#define S31 4 
#define S32 11 
#define S33 16 
#define S34 23 


#define S41 6 
#define S42 10 
#define S43 15 
#define S44 21 


// Now define the 64 magic constants. 
#define T1  0xd76aa478 
#define T2  0xe8c7b756 
#define T3  0x242070db 
#define T4  0xc1bdceee 
#define T5  0xf57c0faf 
#define T6  0x4787c62a 
#define T7  0xa8304613 
#define T8  0xfd469501 
#define T9  0x698098d8 
#define T10 0x8b44f7af 
#define T11 0xffff5bb1 
#define T12 0x895cd7be 
#define T13 0x6b901122 
#define T14 0xfd987193 
#define T15 0xa679438e 
#define T16 0x49b40821 


#define T17 0xf61e2562 
#define T18 0xc040b340 
#define T19 0x265e5a51 
#define T20 0xe9b6c7aa 
#define T21 0xd62f105d 
#define T22 0x02441453 
#define T23 0xd8a1e681 
#define T24 0xe7d3fbc8 
#define T25 0x21e1cde6 
#define T26 0xc33707d6 
#define T27 0xf4d50d87 
#define T28 0x455a14ed 
#define T29 0xa9e3e905 
#define T30 0xfcefa3f8 
#define T31 0x676f02d9 
#define T32 0x8d2a4c8a 


#define T33 0xfffa3942 
#define T34 0x8771f681 
#define T35 0x6d9d6122 
#define T36 0xfde5380c 
#define T37 0xa4beea44 
#define T38 0x4bdecfa9 
#define T39 0xf6bb4b60 
#define T40 0xbebfbc70 
#define T41 0x289b7ec6 
#define T42 0xeaa127fa 
#define T43 0xd4ef3085 
#define T44 0x04881d05 
#define T45 0xd9d4d039 
#define T46 0xe6db99e5 
#define T47 0x1fa27cf8 
#define T48 0xc4ac5665 


#define T49 0xf4292244 
#define T50 0x432aff97 
#define T51 0xab9423a7 
#define T52 0xfc93a039 
#define T53 0x655b59c3 
#define T54 0x8f0ccc92 
#define T55 0xffeff47d 
#define T56 0x85845dd1 
#define T57 0x6fa87e4f 
#define T58 0xfe2ce6e0 
#define T59 0xa3014314 
#define T60 0x4e0811a1 
#define T61 0xf7537e82 
#define T62 0xbd3af235 
#define T63 0x2ad7d2bb 
#define T64 0xeb86d391 


// Now define the digest initialization constants. 
#define A_INIT 0x67452301 
#define B_INIT 0xefcdab89 
#define C_INIT 0x98badcfe 
#define D_INIT 0x10325476 


// Now define the FF, GG, HH, and II transformations for rounds 
// 1, 2, 3, and 4 of the transformation routine. Also define the 
// helper rotate function. Rotation is separate from addition to 
// prevent recomputation. 
#define FF(a, b, c, d, x, s, ac) { \
				(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
                (a) = ROTATE_LEFT ((a), (s)); \
                (a) += (b); \
        } 


#define GG(a, b, c, d, x, s, ac) { \
                (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
                (a) = ROTATE_LEFT ((a), (s)); \
                (a) += (b); \
        } 


#define HH(a, b, c, d, x, s, ac) { \
                (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
                (a) = ROTATE_LEFT ((a), (s)); \
                (a) += (b); \
        } 


#define II(a, b, c, d, x, s, ac) { \
                (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
                (a) = ROTATE_LEFT ((a), (s)); \
                (a) += (b); \
        } 


#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n)))) 


// Now define an unsigned, 32 bit value. 
#define UINT4 unsigned long 


// Although the RFC1321 description speaks of bit lengths (ie. 448, 512), 
// we will deal with byte lengths in this software. 
#define MD5_BLOCK_SIZE  64      // Operate on the input in 64 byte chunks. 
#define MD5_COUNT_SIZE   2      // The bit cnt of the input is this big. 
#define MD5_PAD_FIRST_BYTE 0x80 // The first byte of padding. 
#define MD5_PAD_SIZE    56      // The padding function pads out to 56 bytes. 
#define MD5_XFORM_SIZE  16      // The xform buffer is this long. 


// The pad function operates in these modes: normal, to_end, finish. 
// 
// normal means pad out to MD5_PAD_SIZE bytes. 
// 
// to_end is called when the current len is >= MD5_PAD_SIZE bytes, 
// so pad until the end of the block. 
// 
// finish is called when the prior block was padded via to_end, and 
// therefore the current block needs the rest of the padding. 
enum MD5_PadMode{normal, to_end, finish}; 

/**
 * @ingroup 
 *
 * @brief 支持对文件进行MD5_File码的封装类
 *
 *
 *
 *
 * @Function List 
 *
 *
 *
 */
class MD5_File 
{ 
private: 
	// Operate on the input file one block at a time. 
	unsigned char manip_block[MD5_BLOCK_SIZE]; 


	// Index to next empty manip_block position. 
	// If block_idx == MD5_BLOCK_SIZE, manip_block is full. 
	int block_idx; 


	// The bit count of the input file, mod 2^64, lsb first. 
	UINT4 count[MD5_COUNT_SIZE]; 


	// The A,B,C,D, components of the digest in the making. 
	UINT4 a,b,c,d; 


	// The manip_block is converted into this array of unsigned integers 
	// to me used by the transformation routine. 
	UINT4 xform_buf[MD5_XFORM_SIZE]; 


public: 
	MD5_File( FILE* ); 
	MD5_File(unsigned char *szMD5);


	// Pointer to an array 16 bytes long.  Large enough to hold 128 bits 
	// of digest, ls byte first. 
	void GetDigest( unsigned char* ); 


	// Helper functions 
private: 
	// Convert a manip_block of data from the input to the xform_buf to 
	// be transformed. 
	void MD5_Decode(); 


	// Keep track of the bit length of the input, as successive blocks 
	// are read. 
	void MD5_IncCount( int ); 


	// Perform padding functions. 
	void MD5_Pad( MD5_PadMode ); 


	// Store the bit length of the input into the current manip_block. 
	void MD5_StoreCnt(); 


	// Do the transformation. 
	void MD5_Xform(); 
}; 