#include "stdio.h"
#include "stdint.h"

typedef struct Road {
	char len;
	int roadSize;

	int linkid;
	int roadnameflag;
	int brunch;
	int dispclass;
	char* roadname;
}Road;

typedef struct ArrRoadss
{
	Road** pArrRoads;
	unsigned int size;
	unsigned int count;
}ArrRoads;

void initArrR(ArrRoads* pA)
{
	pA->size = 500;
	pA->count = 0;
	pA->pArrRoads = (Road*)malloc(sizeof(Road) * pA->size);
}

//void push_back(ArrRoads* pA, Road* pR)
//{
//	unsigned int i = 0;
//	Road** pT;
//
//	if (pA->count >= pA->size)
//	{
//		pA->size += 500;
//		pT = (Road**)malloc(sizeof(Road*) * pA->size);
//
//		for (; i < pA->count; ++i)
//		{
//			pT[i] = pA->pArrRoads[i];
//		}
//
//		free(pA->pArrRoads);
//
//		pA->pArrRoads = pT;
//	}
//
//	pA->pArrRoads[pA->count] = pR;
//	++pA->count;
//
//}
void push_back(ArrRoads* pA, Road* pR)
{
	if (pA->count >= pA->size) {
		pA->size += 500;
		pA->pArrRoads = (Road*)realloc(pA->pArrRoads, sizeof(Road) * pA->size);
	}
	pA->pArrRoads[pA->count] = pR;
	pA->count++;

}


int readFile(char* path,ArrRoads* arr) {
	FILE* file = fopen(path, "rb");  // "rb"表示以二进制模式读取文件
	if (file == NULL) {
		// 处理文件打开失败的情况
		printf("无法打开文件\n");
		return 1;  // 返回非零值表示程序异常终止
	}




	//数据长度
	unsigned char dataLen[2];
	//道路编号
	unsigned char roadNumber[4];
	//道路名称尺寸
	unsigned char roadNameLenFake[2];
	//道路信息
	unsigned char roadInfo[4];

	//道路名称字符串所占字节数
	unsigned short roadNameLength = 0;

	//读取的字节长度
	size_t bytesRead;
	size_t size = 10;

	



	//int count = 0;
	while (bytesRead = fread(dataLen, sizeof(unsigned char), sizeof(dataLen), file)) {
		
		//printf("数据长度:");
		Road* r = (Road*)malloc(sizeof(Road));
		
		if (bytesRead == 0) {
			break;
		}
		/*for (size_t i = 0; i < bytesRead; i++) {
			printf("%02X ", dataLen[i]);

		}*/
		
		r->len = dataLen;
		//printf("道路编号:");
		(bytesRead = fread(roadNumber, sizeof(unsigned char), sizeof(roadNumber), file));
	/*	for (size_t i = 0; i < bytesRead; i++) {
			printf("%02X ", roadNumber[i]);
		}*/

		int linkid = (roadNumber[0] << 24) & 0xff000000 | (roadNumber[1] << 16) & 0x00ff0000 | (roadNumber[2] << 8) & 0x0000ff00 | roadNumber[3] & 0x000000ff;

		r->linkid = linkid;
		//printf("linkid %d", linkid);

		//printf("道路名称尺寸:");
		(bytesRead = fread(roadNameLenFake, sizeof(unsigned char), sizeof(roadNameLenFake), file));
	/*	for (size_t i = 0; i < bytesRead; i++) {
			printf("%02X ", roadNameLenFake[i]);
		}*/
		//转换成10进制，并减去固定长度的字节数 2+4+2+4
		roadNameLength = ((dataLen[0] << 8) | dataLen[1]) - 12;

		//printf("道路信息:");
		(bytesRead = fread(roadInfo, sizeof(unsigned char), sizeof(roadInfo), file));
	/*	for (int i = 0; i < bytesRead; i++) {
			printf("%02X ", roadInfo[i]);
		}*/

		int roadnameflag = (roadInfo[3] & 128) >> 7;
		int brunch = (roadInfo[3] & 112) >> 4;
		int dispclass = (roadInfo[3] & 15);
		r->roadnameflag = roadnameflag;
		r->brunch = brunch;
		r->dispclass = dispclass;
		/*printf("flag, %d", roadnameflag);
		printf("brunch, %d", brunch);
		printf("dispclass,  %d", dispclass);*/

		//flag  与10000000后  右移7位
		//岔路数   与01110000后  右移4位
		//class番号 与00001111




		//printf("道路名称:");
		unsigned char roadname[100];

		(bytesRead = fread(roadname, sizeof(unsigned char), roadNameLength, file));
		//printf("%s", roadname);
	
		r->roadname = strdup(roadname);

		//printf("%s", r.roadname);
		//printf("\n");
		push_back(arr, r);



	}
	int cc = 0;

	for (int i = 0; i < 3000; i++) {
		printf("道路名称: %s\n", arr->pArrRoads[i]->roadname);
	}
	

	fclose(file);
	return 0;
}
int main() {
	/*ArrRoads *list=(ArrRoads*)malloc(sizeof(ArrRoads*));
	list->count = 0;
	list->size = 50;
	list->pArrRoads = (Road**)malloc(sizeof(Road*) * 50);*/


	ArrRoads arr;
	initArrR(&arr);
	//arr.size = 500;
	//arr.count = 0;
	//arr.pArrRoads= (Road**)malloc(sizeof(Road*) * 500);



	//list->count = 0;
	//list->size = 50;
	//list->pArrRoads = (Road**)malloc(sizeof(Road*) * list->size);
	//if (list->pArrRoads == NULL) {
	//	fprintf(stderr, "Memory allocation failed for pArrRoads\n");
	//	free(list);
	//	return 1;
	//}

	//readFile("C:\\Users\\16630\\Desktop\\GTBL.dat",&arr);
	int exit = 0;
	int op = -1;
	while (exit == 0) {
		printf("/**********************************  WELCOME *********************************/\n");
		printf("/*************  ##   电子地图信息管理系统  ##   ******************************/\n");
		printf("/******************************************************************************/\n");
		printf("\n");
		printf("请选择服务的种类\n");
		printf("		1.读取文件 \n");
		printf("		2.排序 \n");
		printf("		3.检索 \n");
		printf("		4.更新 \n");
		printf("		0.退出 \n");
		printf("		请选择 \n");
		scanf("%d", &op);
		switch (op)
		{
		case(0):
			exit = 1;
			break;
		case(1):
			if (readFile("C:\\Users\\16630\\Desktop\\GTBL.dat", &arr) == 0) {
				printf("读取文件完成。\n");
			}
			else {
				printf("读取失败!!\n");
			}
			printf("\n");

		
		default:
			break;
		}

	}
	

	return 0;
}




