
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#pragma pack(push, 1)
typedef struct
{
	uint16_t bfType; 
	uint32_t  bfSize; 
	uint16_t bfReserved1; 
	uint16_t bfReserved2; 
	uint32_t  bfOffBits; 
	uint32_t  biSize; 
	int32_t  biWidth; 
	int32_t  biHeight; 
	int16_t biPlanes; 
	int16_t biBitCount; 
	uint32_t  biCompression; 
	uint32_t  biSizeImage; 
	int32_t biXPelsPerMeter; 
	int32_t biYPelsPerMeter; 
	uint32_t  biClrUsed; 
	uint32_t  biClrImportant;
	uint32_t  RGBQuad_0;
	uint32_t  RGBQuad_1;
} bmpHdr;
#pragma pack(pop)

typedef struct
{
	int rowByteSize;		// rozmiar wiersza w bajtach
	int width, height;		// szerokosc i wysokosc obrazu
	unsigned char* pImg;	// wskazanie na początek danych pikselowych
} imgInfo;

void* freeResources(FILE* pFile, void* pFirst, void* pSnd)
{
	if (pFile != 0)
		fclose(pFile);
	if (pFirst != 0)
		free(pFirst);
	if (pSnd !=0)
		free(pSnd);
	return 0;
}

imgInfo* readBMP(const char* fname)
{
	
	imgInfo* pInfo = 0;
	FILE* fbmp = 0;
	bmpHdr bmpHead;
	int lineBytes, y;
	unsigned long imageSize = 0;
	unsigned char* ptr;

	pInfo = 0;
	fbmp = fopen(fname, "rb");
	if (fbmp == 0)
		return 0;

	fread((void *) &bmpHead, sizeof(bmpHead), 1, fbmp);

	// some basic checks
	if ((pInfo = (imgInfo *) malloc(sizeof(imgInfo))) == 0)
		return (imgInfo*) freeResources(fbmp, pInfo->pImg, pInfo);

	pInfo->width = bmpHead.biWidth;
	pInfo->height = bmpHead.biHeight;
	imageSize = (((pInfo->width + 31) >> 5) << 2) * pInfo->height;

	if ((pInfo->pImg = (unsigned char*) malloc(imageSize)) == 0)
		return (imgInfo*) freeResources(fbmp, pInfo->pImg, pInfo);

	// process height (it can be negative)
	ptr = pInfo->pImg;
	lineBytes = ((pInfo->width + 31) >> 5) << 2; // line size in bytes
	pInfo->rowByteSize = lineBytes;

	if (pInfo->height > 0)
	{
		// "upside down", bottom of the image first
		ptr += lineBytes * (pInfo->height - 1);
		lineBytes = -lineBytes;
	}
	else
		pInfo->height = -pInfo->height;

	// reading image
	// moving to the proper position in the file
	if (fseek(fbmp, bmpHead.bfOffBits, SEEK_SET) != 0)
		return (imgInfo*) freeResources(fbmp, pInfo->pImg, pInfo);

	for (y=0; y<pInfo->height; ++y)
	{
		fread(ptr, 1, abs(lineBytes), fbmp);
		ptr += lineBytes;
	}

	fclose(fbmp);
	return pInfo;
}

int saveBMP(const imgInfo* pInfo, const char* fname)
{
	int lineBytes = ((pInfo->width + 31) >> 5)<<2;
	bmpHdr bmpHead = 
	{
	0x4D42,				// unsigned short bfType; 
	sizeof(bmpHdr),		// unsigned long  bfSize; 
	0, 0,				// unsigned short bfReserved1, bfReserved2; 
	sizeof(bmpHdr),		// unsigned long  bfOffBits; 
	40,					// unsigned long  biSize; 
	pInfo->width,		// long  biWidth; 
	pInfo->height,		// long  biHeight; 
	1,					// short biPlanes; 
	1,					// short biBitCount; 
	0,					// unsigned long  biCompression; 
	lineBytes * pInfo->height,	// unsigned long  biSizeImage; 
	11811,				// long biXPelsPerMeter; = 300 dpi
	11811,				// long biYPelsPerMeter; 
	2,					// unsigned long  biClrUsed; 
	0,					// unsigned long  biClrImportant;
	0x00000000,			// unsigned long  RGBQuad_0;
	0x00FFFFFF			// unsigned long  RGBQuad_1;
	};

	FILE * fbmp;
	unsigned char *ptr;
	int y;

	if ((fbmp = fopen(fname, "wb")) == 0)
		return -1;
	if (fwrite(&bmpHead, sizeof(bmpHdr), 1, fbmp) != 1)
	{
		fclose(fbmp);
		return -2;
	}

	ptr = pInfo->pImg + lineBytes * (pInfo->height - 1);
	for (y=pInfo->height; y > 0; --y, ptr -= lineBytes)
		if (fwrite(ptr, sizeof(unsigned char), lineBytes, fbmp) != lineBytes)
		{
			fclose(fbmp);
			return -3;
		}
	fclose(fbmp);
	return 0;
}

/****************************************************************************************/
imgInfo* InitScreen (int w, int h)
{
	imgInfo *pImg;
	if ( (pImg = (imgInfo *) malloc(sizeof(imgInfo))) == 0)
		return 0;
	pImg->height = h;
	pImg->width = w;
	pImg->rowByteSize = ((w + 31) >> 5) << 2;
	pImg->pImg = (unsigned char*) malloc((((w + 31) >> 5) << 2) * h);
	if (pImg->pImg == 0)
	{
		free(pImg);
		return 0;
	}
	memset(pImg->pImg, 0, (((w + 31) >> 5) << 2) * h);
	return pImg;
}



void FreeScreen(imgInfo* pInfo)
{
	if (pInfo && pInfo->pImg)
		free(pInfo->pImg);
	if (pInfo)
		free(pInfo);
}

/****************************************************************************************/

extern void RightEdgeErosion(unsigned char* pImg, unsigned char* pImgCopy, int byteNum1, int byteNum2);

extern void MiddleErosion(unsigned char* pImg, unsigned char* pImgCopy, int byteNum1, int byteNum2);

extern void LeftEdgeErosion(unsigned char* pImg, unsigned char* pImgCopy, int byteNum1, int byteNum2);

extern void FourBytesErosion(unsigned char* pImg, unsigned char* pImgCopy, int byteNum1, int byteNum2);


imgInfo* Erosion(imgInfo* pImg)
{
	imgInfo* pErodedImg;
	int columnDWORD;

	pErodedImg = InitScreen(pImg->width, pImg->height);

	// rozmiar obrazka w bajtach
	int byteSize = pImg->height * pImg->rowByteSize;
	// rozmiar obrazka w 4 * bajt
	int sizeDWORD = byteSize >> 2;
	// szerokosc w 4 * bajt
	int widthDWORD = pImg->rowByteSize >> 2;

	// przypadek gdy wielkosc wiersza w obrazku wynosi 4 bajty, czyli najmniej
	if (pImg->rowByteSize == 4)
	{
		int i;
		for (i = 0; i < byteSize; i+=4)
		{
			// upper edge
			if (i < pImg->rowByteSize)
			{
				FourBytesErosion(pImg->pImg, pErodedImg->pImg, i, i);
				FourBytesErosion(pImg->pImg, pErodedImg->pImg, i, i + pImg->rowByteSize);
			}
			// bottom edge
			else if (i > (byteSize - pImg->rowByteSize))
			{
				FourBytesErosion(pImg->pImg, pErodedImg->pImg, i, i);
				FourBytesErosion(pImg->pImg, pErodedImg->pImg, i, i - pImg->rowByteSize);
			}
			// middle
			else
			{
				FourBytesErosion(pImg->pImg, pErodedImg->pImg, i, i);
				FourBytesErosion(pImg->pImg, pErodedImg->pImg, i, i + pImg->rowByteSize);
				FourBytesErosion(pImg->pImg, pErodedImg->pImg, i, i - pImg->rowByteSize);
			}
		}
		return pErodedImg;
	}
	
	// gdy wielkosc wiersza obrazka jest wieksza niz 4 bajty
	int i;
	for (i = 0; i < byteSize; i+=4)
	{
		columnDWORD = (i >> 2) % widthDWORD;
		
		// upper edge
		if (i < pImg->rowByteSize)
		{
			// left edge
			if (columnDWORD == 0)
			{
				LeftEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i);
				LeftEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i + pImg->rowByteSize);
			}
			// right edge
			else if (columnDWORD == (widthDWORD - 1))
			{
				RightEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i);
				RightEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i + pImg->rowByteSize);
			}
			// middle
			else
			{
				MiddleErosion(pImg->pImg, pErodedImg->pImg, i, i);
				MiddleErosion(pImg->pImg, pErodedImg->pImg, i, i + pImg->rowByteSize);
			}
		}
		// bottom edge
		else if (i > (byteSize - pImg->rowByteSize))
		{
			// left edge
			if (columnDWORD == 0)
			{
				LeftEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i);
				LeftEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i - pImg->rowByteSize);
			}
			// right edge
			else if (columnDWORD == (widthDWORD - 1))
			{
				RightEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i);
				RightEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i - pImg->rowByteSize);
			}
			// middle
			else
			{
				MiddleErosion(pImg->pImg, pErodedImg->pImg, i, i);
				MiddleErosion(pImg->pImg, pErodedImg->pImg, i, i - pImg->rowByteSize);
			}
		}
		// middle
		else
		{
			// left edge
			if (columnDWORD == 0)
			{
				LeftEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i);
				LeftEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i + pImg->rowByteSize);
				LeftEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i - pImg->rowByteSize);
			}
			// right edge
			else if (columnDWORD == (widthDWORD - 1))
			{
				RightEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i);
				RightEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i + pImg->rowByteSize);
				RightEdgeErosion(pImg->pImg, pErodedImg->pImg, i, i - pImg->rowByteSize);
			}
			// middle
			else
			{
				MiddleErosion(pImg->pImg, pErodedImg->pImg, i, i);
				MiddleErosion(pImg->pImg, pErodedImg->pImg, i, i + pImg->rowByteSize);
				MiddleErosion(pImg->pImg, pErodedImg->pImg, i, i - pImg->rowByteSize);
			}
		}
	}
	
	return pErodedImg;
}

/****************************************************************************************/

int main(int argc, char* argv[])
{
	imgInfo* pInfo;
	imgInfo* pErodedImg;


	printf("Size of bmpHeader = %ld\n", sizeof(bmpHdr));
	
	if (sizeof(bmpHdr) != 62)
	{
		printf("Change compilation options so as bmpHdr struct size is 62 bytes.\n");
		return 1;
	}

	pInfo = readBMP("test.bmp");

	pErodedImg = Erosion(pInfo);

	saveBMP(pErodedImg, "result.bmp");

	FreeScreen(pErodedImg);
	FreeScreen(pInfo);
	
	return 0;
}
