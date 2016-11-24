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
#define NUMTEST (12)

typedef unsigned char uchar;

uchar** MemAlloc_2D(int width, int height);
double** MemAlloc_2D_double(int width, int height);

void MemFree_2D(uchar** arr, int height);
void MemFree_2D_double(double** arr, int height);

void Training_FileRead(uchar** img_in, int width, int height, char* path, int sub);
void Test_FileRead(uchar** img_in, double** exp, double** effect, int width, int height, char* path, int sub);

void BMPtoRAW_Gray(uchar* in_bmp, uchar *out_raw, int width, int height);
void subSampling(uchar* img_raw, uchar* img_out, int width, int height, int sub);
void GetAverageMatrix(uchar** img_ori, uchar** ave_ori, int width, int height, int sub);
double** GetCovarianceMat(uchar** img_ori, uchar** img_ave, double** img_cov, double** exp, int width, int height, int sub);
void Transpose(double** img_ave_removed, double** img_ave_removed_T, int width, int height);
void Transpose_uchar(uchar** img, uchar** img_T, int width, int height);
double** JacobiRotation(double** img_cov, int width, int height, int sub);
void sorting(double* arr, double** arr2, int size);
double getEuclideanDistance(double** in1, double** in2, int width, int height, int sub);
//void mySorting(double** arr2, double* arr, int n);;

#endif
