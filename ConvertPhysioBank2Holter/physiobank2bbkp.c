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
		unsigned int m_nHeaderSize; //头结构大小
		unsigned int m_nRstFileSize; //分析结果文件大小
		unsigned int m_nDataFileSize; //数据文件大小
		unsigned int m_nPaceFileSize; //起搏文件大小
		unsigned int m_nPatientEventFileSize; //病人事件文件大小
		unsigned int m_nVLPFileSize; //心室晚电位文件长度（仅适用于中健数据
		byte m_nVersion; //1字节版本号
		byte m_nCaseIDLen; //病历号长度
		char m_lpszCaseID[50]; //病历号
		byte m_nPatientNameLen;//病人姓名长度
		char m_lpszPatientName[50];//病人姓名
		byte m_nPatientType; //病人类型0成人1儿童3新生儿
		byte m_nGender; //性别
		byte m_nAge; //年龄
		__int64 m_nBirthDate; //出生日期
		byte m_nHeight; //身高
		byte m_nHeightUnit; //身高单位0 cm 1英尺
		byte m_nWeight; //体重
		byte m_nWeightUnit; //体重单位0kg 1磅
		byte m_nAttendDocIDLen; //主治医生编码长度
		char m_lpszAttendDocID[32]; //主治医生编码
		byte m_nAttendDocNameLen; //主治医生姓名长度
		char m_lpszAttendDocName[50]; //主治医生姓名
		byte m_nOperateDocIDLen; //操作医生编码长度
		char m_lpszOperateDocID[32]; //操作医生编码
		byte m_nOperateDocNameLen; //操作医生姓名长度
		char m_lpszOperateDocName[50]; //操作医生姓名
		byte m_nRecorderType; //记录仪类型
		__int64 m_nStartTime; //开始记录时间
		__int64 m_nAnalysisTime; //分析时间
		__int64 m_nPrintTime; //打印时间
		unsigned int m_nRecordedLen; //记录时间长度，单位为秒
		char m_lpszDataPath[260]; //心电数据文件路径
		char m_lpszRstFilePath[260]; //分析结果文件路径
		char m_lpszRstOrgFilePath[260]; //分析结果备份文件路径
		unsigned short m_nLeadType; //3三导，十二导
		unsigned int m_nSampleRate; //采样率
		unsigned int m_nBatteryInfo; //电池欠压时间，点数
		unsigned short m_nAD; //AD增益

		byte m_nPaceSwitch; //起搏开关十二导时：-关，-V2/I,2-V3/I,3-V2/II,4-V3/II；三导时：-关，-I
		byte m_nVLPSwitch; //心室晚电位开关0-关，-开
		byte m_nDays; //记录了的天数从开始
		byte m_nLock; //加锁标志0 未锁定1锁定
		unsigned short m_nDiagnosisTipLen; // 诊断提示的长度
		char m_lpszDiagnosisTip[1000]; //诊断提示
		unsigned short m_nDiagnosisDay1Len; //第一天诊断结论的长度
		char m_lpszDiagnosisDay1[2000]; //第一天诊断结论
		unsigned short m_nDiagnosisDay2Len; //第二天诊断结论的长度
		char m_lpszDiagnosisDay2[2000]; //第二天诊断结论
		unsigned short m_nDiagnosisDay3Len; //第三天诊断结论的长度
		char m_lpszDiagnosisDay3[2000]; //第三天诊断结论
		char m_lpszReserve[13110]; //预留字节
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

	//==== 初始化设置 ====
	void WriteInfo(MLLHHPATIENINFO * pInfo)
	{
		bbkpBackUp->m_nHeaderSize = 21264;//0x5310
		bbkpBackUp->m_nRstFileSize = 0;
		bbkpBackUp->m_nDataFileSize = pInfo->nDataLen;//字节数
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
		bbkpBackUp-> m_nAttendDocNameLen = 3; //主治医生姓名长度
		memset(bbkpBackUp->m_lpszAttendDocName, 0, 50);
		bbkpBackUp-> m_nOperateDocIDLen = 3; //操作医生编码长度
		memset(bbkpBackUp->m_lpszOperateDocID, 0, 32);
		bbkpBackUp->m_nOperateDocNameLen = 3; //操作医生姓名长度
		memset(bbkpBackUp->m_lpszOperateDocName, 0, 50);
		bbkpBackUp->m_nRecorderType = 0; //记录仪类型
		bbkpBackUp->m_nStartTime = 0x0000a58200000000; //开始记录时间
		bbkpBackUp->m_nAnalysisTime = 0; //分析时间
		bbkpBackUp->m_nPrintTime = 0; //打印时间
		//bbkpBackUp->m_nRecordedLen = 3600; //记录时间长度，单位为秒
		//    memset(bbkpBackUp->m_lpszDataPath, 0, 260);
		//    memset(bbkpBackUp->m_lpszRstFilePath, 0, 260);
		//    memset(bbkpBackUp->m_lpszRstOrgFilePath, 0, 260);
		//    bbkpBackUp->m_lpszDataPath[260]; //心电数据文件路径
		//    bbkpBackUp->m_lpszRstFilePath[260]; //分析结果文件路径
		//    bbkpBackUp->m_lpszRstOrgFilePath[260]; //分析结果备份文件路径
		bbkpBackUp->m_nLeadType = 3; //3三导，十二导
		bbkpBackUp->m_nSampleRate = 256; //采样率
		bbkpBackUp->m_nRecordedLen = bbkpBackUp->m_nDataFileSize/sizeof(short)/(bbkpBackUp->m_nLeadType)/(bbkpBackUp->m_nSampleRate); //记录时间长度，单位为秒


		bbkpBackUp->m_nBatteryInfo = 0; //电池欠压时间，点数
		bbkpBackUp->m_nAD = 328; //AD增益

		bbkpBackUp->m_nPaceSwitch = 0; //起搏开关十二导时：-关，-V2/I,2-V3/I,3-V2/II,4-V3/II；三导时：-关，-I
		bbkpBackUp->m_nVLPSwitch = 0; //心室晚电位开关0-关，-开
		bbkpBackUp->m_nDays = 1; //记录了的天数从开始
		bbkpBackUp->m_nLock = 0; //加锁标志0 未锁定1锁定

		bbkpBackUp->m_nDiagnosisTipLen = 0; // 诊断提示的长度
		memset(bbkpBackUp->m_lpszDiagnosisTip, 0, 1000);
		//    bbkpBackUp->m_lpszDiagnosisTip[1000]; //诊断提示
		bbkpBackUp->m_nDiagnosisDay1Len = 0; //第一天诊断结论的长度
		memset(bbkpBackUp->m_lpszDiagnosisDay1, 0, 2000);
		//    bbkpBackUp->m_lpszDiagnosisDay1[2000]; //第一天诊断结论
		bbkpBackUp->m_nDiagnosisDay2Len = 0; //第二天诊断结论的长度
		memset(bbkpBackUp->m_lpszDiagnosisDay2, 0, 2000);
		//    bbkpBackUp->m_lpszDiagnosisDay2[2000]; //第二天诊断结论
		bbkpBackUp->m_nDiagnosisDay3Len = 0; //第三天诊断结论的长度
		memset(bbkpBackUp->m_lpszDiagnosisDay3, 0, 2000);
		//    bbkpBackUp->m_lpszDiagnosisDay3[2000]; //第三天诊断结论
		memset(bbkpBackUp->m_lpszReserve, 0, 13110);
		//    bbkpBackUp->m_lpszReserve[13110]; //预留字节
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
			//pBbkpData[i] = pDBData[i] * 328 / 53;//holter AD增益是328
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
		unsigned int *nHeaderSize; //头结构大小
		unsigned int *nRstFileSize; //分析结果文件大小
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