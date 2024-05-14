#include "stdio.h"


#define GET_SHORT(x) (((*x)<<8)|((*(x+1))&0xff))

#define GET_INT(x) (((*x)<<24&0xff000000)|((*(x+1))<<16&0x00ff0000)|((*(x+2))<<8&0x0000ff00)|((*(x+3))&0x000000ff))

#define GET_ClASS(x) ((*(x+9))&0x000f)
#define GET_BRUNCH(x) (((*(x+9))>>4)&0x0007)

#define GET_FLAG(x)  (((*(x+9))>>7)&0x0001)
typedef struct Record
{
	unsigned short usSize;
	unsigned long unLinkID;
	unsigned short usRoadNameSize;
	unsigned int unNodeInfo;

	char disClass;
	char brunch;
	char flag;



	char* pStrRoadName;
}Record;

typedef struct ArrRecords
{
	Record** pArrRecord;
	unsigned int unSize;
	unsigned int unCount;
}ArrRecords;


void push_back(ArrRecords* pA, Record* pR)
{
	unsigned int i = 0;
	Record** pT;

	if (pA->unCount >= pA->unSize)
	{
		pA->unSize += 500;
		pT = (Record**)malloc(sizeof(Record*) * pA->unSize);

		for (; i < pA->unCount; ++i)
		{
			pT[i] = pA->pArrRecord[i];
		}

		free(pA->pArrRecord);

		pA->pArrRecord = pT;
	}

	pA->pArrRecord[pA->unCount] = pR;
	++pA->unCount;

}
void readFile(const char* pF, ArrRecords* arr)
{
	char cSize[2];
	char* pT;
	Record* pR;

	FILE* ptrF = fopen(pF, "r+b");
	if (ptrF != NULL)
	{
		while (!feof(ptrF))
		{
			fread(cSize, sizeof(char), 2, ptrF);
			for (int i = 0; i < 2; i++) {
				printf("%02X", cSize[i]);
			}
			printf("%d", sizeof(Record)*1);
			pR = (Record*)malloc(sizeof(Record) * 1);
			pR->usSize = GET_SHORT(cSize);


			pT = (char*)malloc(sizeof(char) * (pR->usSize - 2));
			memset(pT, '\0', pR->usSize - 2);
			fread(pT, sizeof(char), pR->usSize - 2, ptrF);

			pR->unLinkID = GET_INT(pT);
			pR->usRoadNameSize = GET_SHORT(pT + 4);

			pR->pStrRoadName = (char*)malloc(sizeof(char) * pR->usRoadNameSize);

			pR->flag = GET_FLAG(pT);

			if (pR->flag != 0)
				strcpy(pR->pStrRoadName, pT + 10);
			else
				strcpy(pR->pStrRoadName, "");

			pR->unNodeInfo = GET_INT(pT + 6);
			pR->brunch = GET_BRUNCH(pT);
			pR->disClass = GET_ClASS(pT);

			push_back(arr, pR);
			free(pT);

		}

	}
	else
	{
		perror("error\n");
	}

}

void initArrR(ArrRecords* pA)
{
	pA->unSize = 500;
	pA->unCount = 0;
	pA->pArrRecord = (Record**)malloc(sizeof(Record*) * 500);
}
void destroyArrR(ArrRecords* pA)
{
	unsigned int i = 0;
	for (; i < pA->unCount; ++i)
	{
		free((pA->pArrRecord)[i]->pStrRoadName);
		free((pA->pArrRecord)[i]);
	}
	free(pA->pArrRecord);
}

int main() {
	ArrRecords arr;
	initArrR(&arr);

	readFile("C:\\Users\\16630\\source\\repos\\ConsoleApplication1\\GTBL.dat", &arr);
	
}