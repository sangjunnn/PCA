//PCA_2011706153_ภฬป๓มุ

#include "PCA.h"

void main() {
	char Training_path[256] = "Training/set_";
	uchar** img_ori = MemAlloc_2D(NUMFOLDERS * NUMFILES,WIDTH * HEIGHT);
	uchar** img_ave = MemAlloc_2D(NUMFOLDERS * NUMFILES, WIDTH * HEIGHT);
	int** img_cov = MemAlloc_2D_int(WIDTH * HEIGHT, WIDTH * HEIGHT);

	FileRead(img_ori, WIDTH, HEIGHT, Training_path);
	
	GetAverageMatrix(img_ori, img_ave, WIDTH, HEIGHT);
	
	GetCovarianceMat(img_ori, img_ave, img_cov, WIDTH, HEIGHT);
	
}
