//PCA_2011706153_¿ÃªÛ¡ÿ

#ifndef PCA
#define PCA

#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Windows.h>

#define WIDTH (92)
#define HEIGHT (112)
#define NUMFILES (8)
#define NUMFOLDERS (6)

typedef unsigned char uchar;

uchar** MemAlloc_2D(int width, int height);
int** MemAlloc_2D_int(int width, int height);
void MemFree_2D(uchar** arr, int height);

void FileRead(uchar** img_in, int width, int height, char* path);
void FileWrite(char* filename, uchar** img_out, int height, int width);

void BMPtoRAW_Gray(uchar* in_bmp, uchar *out_raw, int width, int height);

void GetAverageMatrix(uchar** img_ori, uchar** ave_ori, int width, int height);
void GetCovarianceMat(uchar** img_ori, uchar** img_ave, uchar** img_cov, int width, int height);
void Transpose(int** img_ave_removed, int** img_ave_removed_T, int width, int height);

#endif
