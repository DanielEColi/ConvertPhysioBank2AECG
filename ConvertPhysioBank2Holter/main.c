#ifndef CONVERTPHYSIOBANK2HOLTER_C
#define CONVERTPHYSIOBANK2HOLTER_C



// #define LEAD_NUM	3
// typedef struct tagOrginData
// {
// 	short data[LEAD_NUM];
// };

// extern void WriteInfo(MLLHHPATIENINFO * pInfo);
// extern void WriteBBKPData(char * pFileName, unsigned int nDataLen);
// #include "convertdms2bbkp.c"
// 
// int main(int argc, char **argv)
// {
// 	char strDataFileName[100];
// 	FILE *pFile;
// 	FILE *pHltFile;
// 	char *pReturn = NULL;
// 	int pos;
// 
// 
// 	if (argc == 1)
// 	{
// 		pFile = fopen(argv[0],'r');
// 		do 
// 		{
// 			pReturn = fgets(strDataFileName,sizeof(strDataFileName)-1,pFile);
// 			pos = strlen(strDataFileName);
// 			if (pos<sizeof(strDataFileName)-6)
// 			{
// 				strcpy(strDataFileName+pos,".BBKP");
// 				pHltFile = fopen(strDataFileName,"wb");
// 
// 				fclose(pHltFile);
// 			}
// 			else
// 			{
// 				printf("%s\n",strDataFileName);
// 			}
// 
// 
// 
// 
// 		} while (pReturn != NULL);
// 
// 
// 		fclose( pFile);
// 
// 	}
// 	else
// 	{
// 		printf("Usage:ConvertPhysioBank2Holter hltList.txt\n");
// 	}
// 
// 	return 0;
// }
// 








#endif