//PCA_2011706153_¿ÃªÛ¡ÿ

#include "func.h"

void main() {
	char Training_path[256] = "Training/set_";
	char Test_path[256] = "Test";
	int n;
	printf("Subsampling : ");
	scanf("%d", &n);
	/*uchar** img_ori = MemAlloc_2D(NUMFOLDERS * NUMFILES, WIDTH * HEIGHT);
	uchar** img_ave = MemAlloc_2D(NUMFOLDERS * NUMFILES, WIDTH * HEIGHT);*/
	uchar** img_ori = MemAlloc_2D(WIDTH * HEIGHT/(n*n), NUMFOLDERS * NUMFILES);
	uchar** img_ori_T = MemAlloc_2D(NUMFOLDERS * NUMFILES, WIDTH * HEIGHT/(n*n));
	uchar** img_test = MemAlloc_2D(WIDTH * HEIGHT / (n*n), NUMTEST);
	uchar** img_ave = MemAlloc_2D(NUMFOLDERS, WIDTH * HEIGHT/(n*n));
	double** img_cov = MemAlloc_2D_double(WIDTH * HEIGHT/(n*n), WIDTH * HEIGHT/(n*n));
	double** eigvec = MemAlloc_2D_double(WIDTH * HEIGHT /(n*n), WIDTH * HEIGHT /(n*n));
	double** expectation = MemAlloc_2D_double(NUMFOLDERS,10);
	double** effective_eig = MemAlloc_2D_double(WIDTH * HEIGHT / (n*n), 10);

	Training_FileRead(img_ori, WIDTH, HEIGHT, Training_path, n);

	Transpose_uchar(img_ori, img_ori_T, WIDTH*HEIGHT / (n*n), NUMFOLDERS*NUMFILES);

	MemFree_2D(img_ori, NUMFOLDERS * NUMFILES);

	GetAverageMatrix(img_ori_T, img_ave, WIDTH, HEIGHT, n);
	
	effective_eig = GetCovarianceMat(img_ori_T, img_ave, img_cov, expectation, WIDTH, HEIGHT, n);

	Test_FileRead(img_test, expectation, effective_eig, WIDTH, HEIGHT, Test_path, n);

	MemFree_2D(img_ori_T, WIDTH * HEIGHT / (n*n));
	MemFree_2D(img_ave, WIDTH * HEIGHT / (n*n));
	MemFree_2D_double(img_cov, WIDTH * HEIGHT / (n*n));
}

