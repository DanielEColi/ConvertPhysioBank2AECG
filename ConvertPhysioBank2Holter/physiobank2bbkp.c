/*
* @Author: malin(ma.lin@139.com)
* @Date:   2015-12-08 20:08:00
* @Last Modified:   Administrator
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>


#ifdef __cplusplus
extern "C" {
#endif

#define ML_ASSERT(expr) \
	if(!(expr)) { \
	fprintf(stderr,"ASSERTION FAILED!\n");\
	fprintf(stderr,"expr: " #expr "\n");\
	fprintf(stderr,"at: %s line %d\n", __FILE__, __LINE__);\
	system("pause");\
	exit(-1);\
	}
	typedef unsigned char byte;
	//#define WIN32
#ifdef WIN32
	typedef unsigned __int8 __uint8;
#endif
	typedef char * string;

#ifdef __unix__
#include <stdint.h>
#define __int64 int64_t
#define __uint8 uint8_t
#endif
	typedef struct MllhhSPatientInfoBackUp
	{
		unsigned int m_nHeaderSize; //ͷ�ṹ��С
		unsigned int m_nRstFileSize; //��������ļ���С
		unsigned int m_nDataFileSize; //�����ļ���С
		unsigned int m_nPaceFileSize; //���ļ���С
		unsigned int m_nPatientEventFileSize; //�����¼��ļ���С
		unsigned int m_nVLPFileSize; //�������λ�ļ����ȣ����������н�����
		byte m_nVersion; //1�ֽڰ汾��
		byte m_nCaseIDLen; //�����ų���
		char m_lpszCaseID[50]; //������
		byte m_nPatientNameLen;//������������
		char m_lpszPatientName[50];//��������
		byte m_nPatientType; //��������0����1��ͯ3������
		byte m_nGender; //�Ա�
		byte m_nAge; //����
		__int64 m_nBirthDate; //��������
		byte m_nHeight; //���
		byte m_nHeightUnit; //��ߵ�λ0 cm 1Ӣ��
		byte m_nWeight; //����
		byte m_nWeightUnit; //���ص�λ0kg 1��
		byte m_nAttendDocIDLen; //����ҽ�����볤��
		char m_lpszAttendDocID[32]; //����ҽ������
		byte m_nAttendDocNameLen; //����ҽ����������
		char m_lpszAttendDocName[50]; //����ҽ������
		byte m_nOperateDocIDLen; //����ҽ�����볤��
		char m_lpszOperateDocID[32]; //����ҽ������
		byte m_nOperateDocNameLen; //����ҽ����������
		char m_lpszOperateDocName[50]; //����ҽ������
		byte m_nRecorderType; //��¼������
		__int64 m_nStartTime; //��ʼ��¼ʱ��
		__int64 m_nAnalysisTime; //����ʱ��
		__int64 m_nPrintTime; //��ӡʱ��
		unsigned int m_nRecordedLen; //��¼ʱ�䳤�ȣ���λΪ��
		char m_lpszDataPath[260]; //�ĵ������ļ�·��
		char m_lpszRstFilePath[260]; //��������ļ�·��
		char m_lpszRstOrgFilePath[260]; //������������ļ�·��
		unsigned short m_nLeadType; //3������ʮ����
		unsigned int m_nSampleRate; //������
		unsigned int m_nBatteryInfo; //���Ƿѹʱ�䣬����
		unsigned short m_nAD; //AD����

		byte m_nPaceSwitch; //�𲫿���ʮ����ʱ��-�أ�-V2/I,2-V3/I,3-V2/II,4-V3/II������ʱ��-�أ�-I
		byte m_nVLPSwitch; //�������λ����0-�أ�-��
		byte m_nDays; //��¼�˵������ӿ�ʼ
		byte m_nLock; //������־0 δ����1����
		unsigned short m_nDiagnosisTipLen; // �����ʾ�ĳ���
		char m_lpszDiagnosisTip[1000]; //�����ʾ
		unsigned short m_nDiagnosisDay1Len; //��һ����Ͻ��۵ĳ���
		char m_lpszDiagnosisDay1[2000]; //��һ����Ͻ���
		unsigned short m_nDiagnosisDay2Len; //�ڶ�����Ͻ��۵ĳ���
		char m_lpszDiagnosisDay2[2000]; //�ڶ�����Ͻ���
		unsigned short m_nDiagnosisDay3Len; //��������Ͻ��۵ĳ���
		char m_lpszDiagnosisDay3[2000]; //��������Ͻ���
		char m_lpszReserve[13110]; //Ԥ���ֽ�
	}MLLHHSPATIENTINFOBACKUP;

	typedef struct MllhhPatienInfo{
		char patientName[50];
		char caseId[50];
		unsigned int nDataLen;
	}MLLHHPATIENINFO;

	static MLLHHSPATIENTINFOBACKUP * bbkpBackUp ;
	static __uint8 * pDmsData;
	static short * pBbkpData;
	static MLLHHPATIENINFO * pInfo;
	static short * pDBData;

	//==== ��ʼ������ ====
	void WriteInfo(MLLHHPATIENINFO * pInfo)
	{
		bbkpBackUp->m_nHeaderSize = 21264;//0x5310
		bbkpBackUp->m_nRstFileSize = 0;
		bbkpBackUp->m_nDataFileSize = pInfo->nDataLen;//�ֽ���
		bbkpBackUp->m_nPaceFileSize = 0;
		bbkpBackUp->m_nPatientEventFileSize = 5;
		bbkpBackUp->m_nVLPFileSize = 10;
		bbkpBackUp->m_nVersion = 1;
		bbkpBackUp->m_nCaseIDLen = 10;
		//    memset(bbkpBackUp->m_lpszCaseID,0,50);
		//    bbkpBackUp->m_lpszCaseID[0] = '7';
		strcpy(bbkpBackUp->m_lpszCaseID, pInfo->caseId);
		bbkpBackUp->m_nPatientNameLen = 4;
		//    memset(bbkpBackUp->m_lpszPatientName,0,50);
		//    bbkpBackUp->m_lpszPatientName[0] = 'a';
		strcpy(bbkpBackUp->m_lpszPatientName, pInfo->patientName);
		bbkpBackUp->m_nPatientType = 0;
		bbkpBackUp->m_nGender = 1;
		bbkpBackUp->m_nAge = 0;
		bbkpBackUp->m_nBirthDate = 0x0000a58200000000;
		bbkpBackUp->m_nHeight = 180;
		bbkpBackUp->m_nHeightUnit =0;
		bbkpBackUp->m_nWeight = 60;
		bbkpBackUp->m_nWeightUnit = 0;
		bbkpBackUp->m_nAttendDocIDLen = 0;
		memset(bbkpBackUp-> m_lpszAttendDocID, 0, 32);
		bbkpBackUp-> m_nAttendDocNameLen = 3; //����ҽ����������
		memset(bbkpBackUp->m_lpszAttendDocName, 0, 50);
		bbkpBackUp-> m_nOperateDocIDLen = 3; //����ҽ�����볤��
		memset(bbkpBackUp->m_lpszOperateDocID, 0, 32);
		bbkpBackUp->m_nOperateDocNameLen = 3; //����ҽ����������
		memset(bbkpBackUp->m_lpszOperateDocName, 0, 50);
		bbkpBackUp->m_nRecorderType = 0; //��¼������
		bbkpBackUp->m_nStartTime = 0x0000a58200000000; //��ʼ��¼ʱ��
		bbkpBackUp->m_nAnalysisTime = 0; //����ʱ��
		bbkpBackUp->m_nPrintTime = 0; //��ӡʱ��
		//bbkpBackUp->m_nRecordedLen = 3600; //��¼ʱ�䳤�ȣ���λΪ��
		//    memset(bbkpBackUp->m_lpszDataPath, 0, 260);
		//    memset(bbkpBackUp->m_lpszRstFilePath, 0, 260);
		//    memset(bbkpBackUp->m_lpszRstOrgFilePath, 0, 260);
		//    bbkpBackUp->m_lpszDataPath[260]; //�ĵ������ļ�·��
		//    bbkpBackUp->m_lpszRstFilePath[260]; //��������ļ�·��
		//    bbkpBackUp->m_lpszRstOrgFilePath[260]; //������������ļ�·��
		bbkpBackUp->m_nLeadType = 3; //3������ʮ����
		bbkpBackUp->m_nSampleRate = 256; //������
		bbkpBackUp->m_nRecordedLen = bbkpBackUp->m_nDataFileSize/sizeof(short)/(bbkpBackUp->m_nLeadType)/(bbkpBackUp->m_nSampleRate); //��¼ʱ�䳤�ȣ���λΪ��


		bbkpBackUp->m_nBatteryInfo = 0; //���Ƿѹʱ�䣬����
		bbkpBackUp->m_nAD = 328; //AD����

		bbkpBackUp->m_nPaceSwitch = 0; //�𲫿���ʮ����ʱ��-�أ�-V2/I,2-V3/I,3-V2/II,4-V3/II������ʱ��-�أ�-I
		bbkpBackUp->m_nVLPSwitch = 0; //�������λ����0-�أ�-��
		bbkpBackUp->m_nDays = 1; //��¼�˵������ӿ�ʼ
		bbkpBackUp->m_nLock = 0; //������־0 δ����1����

		bbkpBackUp->m_nDiagnosisTipLen = 0; // �����ʾ�ĳ���
		memset(bbkpBackUp->m_lpszDiagnosisTip, 0, 1000);
		//    bbkpBackUp->m_lpszDiagnosisTip[1000]; //�����ʾ
		bbkpBackUp->m_nDiagnosisDay1Len = 0; //��һ����Ͻ��۵ĳ���
		memset(bbkpBackUp->m_lpszDiagnosisDay1, 0, 2000);
		//    bbkpBackUp->m_lpszDiagnosisDay1[2000]; //��һ����Ͻ���
		bbkpBackUp->m_nDiagnosisDay2Len = 0; //�ڶ�����Ͻ��۵ĳ���
		memset(bbkpBackUp->m_lpszDiagnosisDay2, 0, 2000);
		//    bbkpBackUp->m_lpszDiagnosisDay2[2000]; //�ڶ�����Ͻ���
		bbkpBackUp->m_nDiagnosisDay3Len = 0; //��������Ͻ��۵ĳ���
		memset(bbkpBackUp->m_lpszDiagnosisDay3, 0, 2000);
		//    bbkpBackUp->m_lpszDiagnosisDay3[2000]; //��������Ͻ���
		memset(bbkpBackUp->m_lpszReserve, 0, 13110);
		//    bbkpBackUp->m_lpszReserve[13110]; //Ԥ���ֽ�
	}

	static unsigned int LoadDataBase(const char * strName)
	{
		int i;
		long lSizeBytes,lSize;
		FILE * fp;
		// #ifdef WIN32
		// 	fopen_s(&fp, strName, "rb");
		// #else
		fp = fopen(strName, "rb");
		/*#endif*/
		ML_ASSERT(fp);

		// load start to end
		fseek (fp , 0 , SEEK_END);
		lSizeBytes = ftell (fp);
		lSize = lSizeBytes/sizeof(short);
		rewind (fp);
		//=======================

		pDBData = (short *)calloc(lSize, sizeof(short));
		pBbkpData = (short *)calloc(lSize, sizeof(short));
		fread(pDBData, sizeof(short), lSize, fp);
		for (i = 0; i < lSize; i++)
		{
			//pBbkpData[i] = pDBData[i] * 328 / 53;//holter AD������328
			pBbkpData[i] = pDBData[i];
		}


		fclose(fp);
		return (unsigned int)lSize;
	}

	static unsigned int LoadDms(char * pFileName)
	{
		int i;
		int lSize;
		FILE * fp;
#ifdef WIN32
		fopen_s(&fp, pFileName, "rb");
#else
		fp = fopen(pFileName, "rb");
#endif
		ML_ASSERT(fp);

		// load start to end
		fseek (fp , 0 , SEEK_END);
		lSize = ftell (fp);
		rewind (fp);
		//=======================

		pDmsData = (__uint8 *)calloc(lSize, sizeof(__uint8));
		pBbkpData = (short *)calloc(lSize, sizeof(short));
		fread(pDmsData, sizeof(__uint8), lSize, fp);
		for (i = 0; i < lSize; i++)
		{
			pBbkpData[i] = (short)pDmsData[i] * 328 / 53;
		}

		fclose(fp);
		return lSize;
	}

	void WriteBBKPData(char * pFileName, unsigned int nDataLen)
	{
		FILE * fp;
		// #ifdef WIN32
		// 	fopen_s(&fp, pFileName, "wb");
		// #else
		fp = fopen(pFileName, "wb");
		/*#endif*/
		ML_ASSERT(fp);
		fwrite(bbkpBackUp, sizeof(MLLHHSPATIENTINFOBACKUP), 1, fp);
		fwrite(pBbkpData, sizeof(short), nDataLen, fp);
		fclose(fp);
	}

	static void GetInfo(MLLHHPATIENINFO * pInfo, char * pFileName)
	{
		FILE * fp;
		char buf[50];
		int line;
#ifdef WIN32
		fopen_s(&fp, pFileName, "r");
#else
		fp = fopen(pFileName, "r");
#endif
		ML_ASSERT(fp);
		line = 0;
		while(fgets(buf, 50, fp))
		{
			line++;
			if (line == 9)
			{
				strcpy(pInfo->patientName, buf);
			}
			if (line == 13)
			{
				strcpy(pInfo->caseId, buf);
				break;
			}
		}
	}
	static void FreeVar()
	{
		free(pDBData);
		pDBData =NULL;
		free(bbkpBackUp);
		bbkpBackUp =NULL;
		free(pDmsData);
		pDmsData = NULL;
		free(pBbkpData);
		pBbkpData = NULL;
		free(pInfo);
		pInfo = NULL;
	}

#if 0
	__declspec(dllexport) void MllhhConvertDms2Bbkp(char * pEcgFile, char * pInfoFile, char * pOutFile)
	{
		unsigned int nDataLen;
		bbkpBackUp = (MLLHHSPATIENTINFOBACKUP*)calloc(1,sizeof(MLLHHSPATIENTINFOBACKUP));
		pInfo = (MLLHHPATIENINFO *)calloc(1, sizeof(MLLHHPATIENINFO));
		GetInfo(pInfo, pInfoFile);
		nDataLen = LoadDms(pEcgFile);
		//pInfo->nDataLen = nDataLen;
		pInfo->nDataLen = 2*nDataLen + 21264;
		SetValue(pInfo);
		WriteBBKPData(pOutFile, nDataLen);
		FreeVar();
	}

	__declspec(dllexport) void MllhhConvertBbkp2Dms(char * pEcgFile, char * pOutFile)
	{
		FILE *fp;
		FILE *fpOut;
		long lSize;
		int nDataLen;
		int i;
		unsigned int *nHeaderSize; //ͷ�ṹ��С
		unsigned int *nRstFileSize; //��������ļ���С
		int temp;
#ifdef WIN32
		fopen_s(&fp, pEcgFile, "rb");
		//fp = fopen(pEcgFile, "rb");
#else
		fp = fopen(pEcgFile, "rb");
#endif
		ML_ASSERT(fp);

		nHeaderSize = (unsigned int *)calloc(1, sizeof(unsigned int));
		nRstFileSize = (unsigned int *)calloc(1, sizeof(unsigned int));
		fread(nHeaderSize, sizeof(unsigned int), 1, fp);
		fread(nRstFileSize, sizeof(unsigned int), 1, fp);
		rewind (fp);
		temp = (*nHeaderSize)+(*nRstFileSize);
		fseek(fp, 0, SEEK_END);
		lSize = ftell (fp);
		rewind (fp);
		nDataLen = (lSize-temp)>>1;
		fseek(fp, temp, SEEK_SET);
		pBbkpData = (short *)calloc(nDataLen, sizeof(short));
		pDmsData = (__uint8 *)calloc(nDataLen, sizeof(__uint8));
		fread(pBbkpData, sizeof(short), nDataLen, fp);
		for (i = 0; i < nDataLen; i++)
		{
			pDmsData[i] = pBbkpData[i] * 53 / 328;
		}

#ifdef WIN32
		fopen_s(&fpOut, pOutFile, "wb");
		//fpOut = fopen(pOutFile, "wb");
#else
		fpOut = fopen(pOutFile, "wb");
#endif
		ML_ASSERT(fpOut);
		fwrite(pDmsData, sizeof(__uint8), nDataLen, fpOut);
		fclose(fp);
		fclose(fpOut);
		free(nHeaderSize);
		nHeaderSize = NULL;
		free(nRstFileSize);
		nRstFileSize = NULL;
		FreeVar();
	}
#endif


	int main(int argc, char **argv)
	{
#define  MAX_PATH  200
		char strSrcFileName[MAX_PATH+1];
		char strDesFileName[MAX_PATH+1];
		char strDirect[MAX_PATH+1];
		char pBuf[MAX_PATH+1];
		char strPrefix[MAX_PATH+1];
		FILE *pFile;
		FILE *pHltFile;
		FILE *fp;
		char *pReturn = NULL;
		int pos;
		unsigned int nDataLen;
		short i;

		if (NULL == getcwd(strDirect,MAX_PATH))
		{
			perror("getcwd error!\n");  
			system("pause");
			return 0;
		}
		// 		strcpy(strDirect,"E:\\MIT_BIH\\nstdb");
		pos = strlen(strDirect);
		strDirect[pos]='\\';
		strDirect[pos+1]=0;


		bbkpBackUp = (MLLHHSPATIENTINFOBACKUP*)calloc(1,sizeof(MLLHHSPATIENTINFOBACKUP));
		pInfo = (MLLHHPATIENINFO *)calloc(1, sizeof(MLLHHPATIENINFO));
		if ( bbkpBackUp==NULL || pInfo==NULL)
		{
			printf("Memory calloc fail!\n");
			system("pause");
			return 0;
		}

		printf("argc=%d\n",argc);
		for (i=0;i<argc;i++)
		{
			printf("argv[%d]=%s\n",i,argv[i]);
		}


		// 		pFile = fopen("E:\\MIT_BIH\\nstdb\\20160127114823.BBKP","rb");
		// 		fread(bbkpBackUp,sizeof(MLLHHSPATIENTINFOBACKUP),1,pFile);
		// 		fclose(pFile);
		if (argc >= 2)
		{
			if (argc==3)
			{
				strcpy(strPrefix,argv[2]);
			}
			else
			{
				memset(strPrefix,0,sizeof(strPrefix));
			}


			pFile = fopen(argv[1],"r");
			// 		pFile = fopen("E:\\MIT_BIH\\nstdb\\hltList.txt","r");
			pReturn = fgets(pBuf,MAX_PATH,pFile);
			while (pFile!= NULL && pReturn != NULL)
			{
				strcpy(strSrcFileName,strDirect);
				pos = strlen(pBuf);
				if (pBuf[pos-1] == 10)
				{
					pBuf[pos-1] = 0;
				}
				strcat(strSrcFileName,pBuf);

				pos = strlen(strSrcFileName);
				strcpy(strDesFileName,strSrcFileName);
				if (pos<sizeof(strDesFileName)-6)
				{
					strcpy(strDesFileName+pos-4,".BBKP");
					pHltFile = fopen(strDesFileName,"wb");
					if (pHltFile)
					{
						nDataLen = LoadDataBase(strSrcFileName);
						strcpy(pInfo->caseId,"00000000001");
						pos = strlen(pBuf);
						pBuf[pos-4] = 0;
						strcpy(pInfo->patientName,strPrefix);
						strcat(pInfo->patientName,pBuf);
						pInfo->nDataLen = nDataLen*sizeof(short) ;
						WriteInfo(pInfo);
						WriteBBKPData(strDesFileName, nDataLen);
						printf("PASS:%s\n",strDesFileName);

						fclose(pHltFile);
					}
					else
					{
						printf("Create %s.BBKP fail!\n",strDesFileName);
					}
				}
				else
				{
					printf("FAIL:%s\n",strSrcFileName);
				}

				pReturn = fgets(pBuf,MAX_PATH,pFile);
			} 

			FreeVar();
			fclose( pFile);

		}
		else
		{
			printf("Usage:ConvertPhysioBank2Holter hltList.txt [prefix]\n");
		}

		//system("pause");
		return 0;
	}



#ifdef __cplusplus
}
#endif
//int main(int argc, char *argv[])
//{
//	//    int i;
//	//    FILE *fp;
//	//short * pTempData;
//	char inFileName[] = "D:\\Carsc12\\Save010\\crecg.dat";
//	char infoFile[] = "D:\\Carsc12\\Save010\\info.pat";
//	char outFileName[] =  "E:\\data_base\\bbkpdb\\convert_dms2bbkp.BBKP";
//	MllhhConvertDms2Bbkp(inFileName,infoFile,outFileName);
//	return 0;
//}