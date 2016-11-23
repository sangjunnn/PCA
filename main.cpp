//PCA_2011706153_¿ÃªÛ¡ÿ

#include "PCA.h"

void main() {
	char Training_path[256] = "Training/set_";
	/*uchar** img_ori = MemAlloc_2D(NUMFOLDERS * NUMFILES, WIDTH * HEIGHT);
	uchar** img_ave = MemAlloc_2D(NUMFOLDERS * NUMFILES, WIDTH * HEIGHT);*/
	uchar** img_ori = MemAlloc_2D(WIDTH * HEIGHT, NUMFOLDERS * NUMFILES);
	uchar** img_ori_T = MemAlloc_2D(NUMFOLDERS * NUMFILES, WIDTH * HEIGHT);
	
	uchar** img_ave = MemAlloc_2D(NUMFOLDERS * NUMFILES, WIDTH * HEIGHT);
	double** img_cov = MemAlloc_2D_double(WIDTH * HEIGHT, WIDTH * HEIGHT);

	FileRead(img_ori, WIDTH, HEIGHT, Training_path);
	
	Transpose_uchar(img_ori, img_ori_T, WIDTH*HEIGHT, NUMFOLDERS*NUMFILES);
	
	GetAverageMatrix(img_ori_T, img_ave, WIDTH, HEIGHT);
	
	GetCovarianceMat(img_ori_T, img_ave, img_cov, WIDTH, HEIGHT);
	
}
