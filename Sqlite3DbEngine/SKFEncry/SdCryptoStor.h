#ifndef __SD_CRYPTO_STOR_DLL_H__
#define __SD_CRYPTO_STOR_DLL_H__

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
	typedef HANDLE			HDEV;	
#else
	typedef int				HDEV;
	typedef void*			HANDLE;
	typedef unsigned long	ULONG;
	typedef unsigned char	UCHAR;
#endif

#define INTERFACEFILE	"SECOM.SCT"
	//-----------------return value--------------------//
#define SDR_OK					0
#define SDR_READ_ERROR			0x01	/* �����ݴ��� */
#define SDR_WRITE_ERROR			0x02	/* д���ݴ��� */
#define SDR_PARAM_ERROR			0x03	/* �������� */
#define SDR_RESPONSE_ERROR		0x04	/* ��ȡ�������ݴ��� */
#define SDR_NO_DEVICE			0x05	/* û���ҵ��豸 */

/* ���ܣ�����ͨѶ���ݰ��İ�ͷ 
 * ������pucSendHeader
 *			�������ݰ��İ�ͷ�����7Bytes
 * ������pucSendHeader
 *			�������ݰ��İ�ͷ�����7Bytes
 * ���أ��ɹ�����SDR_OK, ��������
 */
void sd_CommInit(PUCHAR pucSendHeader, PUCHAR pucRecvHeader, PCHAR pszFileName);

/* ���ܣ�ö�����п����豸 
 * ������szDevNames
 *			ö�ٵ��������豸���ƣ�����֮����NULL�ָ�����2��NULL����������"J\0N\0\0"
 *			ʹ��sd_Free�ͷ�szDevNames
 *		  pulLen
 *			���ص��ַ�����
 * ���أ��ɹ�����SDR_OK, ��������
 */
int sd_EnumDevice(char** szDevNames, ULONG *pulLen);

/* ���ܣ���ָ���豸 
 * ������szDevName
 *			��NULL��β���豸���ƣ�����"j\0"
 *		 phDevice
 *			���ص��豸�������sd_CloseDevice�ر�
 * ���أ��ɹ�����SDR_OK, ��������
 */
int sd_OpenDevice(const char* szDevName, HDEV* phDevice);

/* ���ܣ��ر��豸 
 * ������hDevice
 *			�豸�������sd_OpenDevice��ȡ
 * ���أ��ɹ�����SDR_OK, ��������
 */
void sd_CloseDevice(HDEV hDevice);

/* ���ܣ���ȡ�豸˽�пռ��С
 * ������hDevice
 *			�豸�������sd_OpenDevice��ȡ
 *		 puCapcity
 *			�豸�ܿռ��С����������
 * ���أ��ɹ�����SDR_OK, ��������
 */
int sd_GetCapcity(HDEV hDevice, ULONG *puCapcity);

/* ���ܣ���ȡָ��LBA������
 * ������hDevice
 *			�豸�������sd_OpenDevice��ȡ
 *		uLba
 *			�����ʵ�LBA��ַ
 *		wSectorCount
 *			����ȡ�����ݳ��ȣ���λ���ֽڣ��ó��ȱ�����512��������
 *		pBuf
 *			���ն�ȡ���ݵĻ���������ȷ������
 * ���أ��ɹ�����SDR_OK, ��������
 */
int sd_ReadData(HDEV hDevice, ULONG uLba, DWORD dwBytesLength, UCHAR *pBuf);

/* ���ܣ�������д��ָ����LBA
 * ������hDevice
 *			�豸�������sd_OpenDevice��ȡ
 *		uLba
 *			�����ʵ�LBA��ַ
 *		wBytesLength
 *			��д������ݳ��ȣ���λ���ֽڣ��ó��ȱ�����512��������
 *		pBuf
 *			��д�����ݵĻ�����
 * ���أ��ɹ�����SDR_OK, ��������
 */
int sd_WriteData(HDEV hDevice, ULONG uLba, DWORD dwBytesLength, UCHAR *pBuf);

/* ���ܣ��ͷ��ڴ���Դ
 * ������pMem
 *			Ҫ�ͷŵ��ڴ�ָ��
 * ���أ���
 */
void sd_Free(void *pMem);

#ifdef  __cplusplus
}
#endif

#endif //__SD_CRYPTO_STOR_DLL_H__
