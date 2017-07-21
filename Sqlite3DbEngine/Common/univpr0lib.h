/// �ں�����IO���������
#ifndef _UNIVPRLIB_H_
#define _UNIVPRLIB_H_

//����DeviceIoControl���������
#define UvProt_SetDIControl					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x01, METHOD_BUFFERED, FILE_WRITE_DATA )

//����������״̬�������س������е�
#define UvProt_SelfSafeGuard				(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x02, METHOD_BUFFERED, FILE_WRITE_DATA )

//�����Ƿ��ж��������ֹͣ����
#define UvProt_UnloadDriver					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x03, METHOD_BUFFERED, FILE_WRITE_DATA )

//����HOST����ID
#define UvProt_HostPID						(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x04, METHOD_BUFFERED, FILE_WRITE_DATA )

//�����Ƿ��ڰ�װ����ж����������ģʽ
#define UvProt_InsPID						(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x05, METHOD_BUFFERED, FILE_WRITE_DATA )

//���û��ȡ�ļ���ע������̵ȵı�����־
#define UvProt_OperateSafeFlag			    (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x06, METHOD_BUFFERED, FILE_WRITE_DATA )

//�����Ƿ���ɨ����������
#define UvProt_ScaningFile					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x07, METHOD_BUFFERED, FILE_WRITE_DATA )

//�����Ƿ���ɨ����̹�����
#define UvProt_ScaningDisk					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x08, METHOD_BUFFERED, FILE_WRITE_DATA )


//��Ӱ������ļ�
#define UvProt_AddWhiteFile			        (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x11, METHOD_BUFFERED, FILE_WRITE_DATA )

//ɾ���������ļ�
#define UvProt_DelWhiteFile			        (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x12, METHOD_BUFFERED, FILE_WRITE_DATA )

//������������
#define UvProt_SetExtFlag			        (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x13, METHOD_BUFFERED, FILE_WRITE_DATA )

//���÷ǰ������ļ��������ػ�������
#define UvProt_SetRunControl		        (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x14, METHOD_BUFFERED, FILE_WRITE_DATA )

//����ܱ�������
#define UvProt_AddProtectExe		        (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x15, METHOD_BUFFERED, FILE_WRITE_DATA )

//�����ܱ���Ŀ¼
#define UvProt_SetDirProtect				(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x16, METHOD_BUFFERED, FILE_WRITE_DATA )

//�����ܱ���ע���
#define UvProt_SetRegProtect				(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x17, METHOD_BUFFERED, FILE_WRITE_DATA )

//���÷ǰ�����U�̷���Ȩ��
#define UvProt_SetUDiskAccess			    (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x18, METHOD_BUFFERED, FILE_WRITE_DATA )

//����U�̰�����
#define UvProt_SetUdiskWhite				(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x19, METHOD_BUFFERED, FILE_WRITE_DATA )

//���ÿ�����Ŀ¼����ʱ������
#define UvProt_SetRunDir					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x31, METHOD_BUFFERED, FILE_WRITE_DATA )

//���ñ��ش��̷��������ƶ�U�̵�ɨ��
#define UvProt_SetLocalDisk					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x32, METHOD_BUFFERED, FILE_WRITE_DATA )

/// U�̰�������󳤶�
#define MAX_UDISK_WHITE_LEN					( 26*sizeof(WCHAR) )

/*
/// ���������ṹ
typedef struct ServiceDescriptorEntry {
    unsigned int *ServiceTableBase;
    unsigned int *ServiceCounterTableBase; 
    unsigned int NumberOfServices;
    unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;

__declspec(dllimport) ServiceDescriptorTableEntry_t ServiceDescriptorTable;

#define SYSTEMSERVICE(_function) ServiceDescriptorTable.ServiceTableBase[ *(PULONG)((PUCHAR)_function+1)] 
*/

#ifndef DEVICE_NAME_SZ
#define DEVICE_NAME_SZ	256
#endif

typedef struct _VOLUMN_RIGHT
{
	UCHAR	uRight;
	WCHAR	szVolumeName[DEVICE_NAME_SZ];
}VOLUMN_RIGHT,*PVOLUMN_RIGHT;

#endif