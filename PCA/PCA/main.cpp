//PCA_2011706153_ภฬป๓มุ

#include "PCA.h"

void main() {
	char Training_path[256] = "Training/set_";
	uchar** img_ori = MemAlloc_2D(WIDTH * HEIGHT, NUMFOLDERS * NUMFILES);
	uchar** img_ave = MemAlloc_2D(WIDTH * HEIGHT, NUMFOLDERS * NUMFILES);
	uchar** img_cov = MemAlloc_2D(WIDTH * HEIGHT, WIDTH * HEIGHT);

	FileRead(img_ori, WIDTH, HEIGHT, Training_path);
	printf("1\n");

	GetAverageMatrix(img_ori, img_ave, WIDTH, HEIGHT);
	printf("2\n");
	GetCovarianceMat(img_ori, img_ave, img_cov, WIDTH, HEIGHT);
	printf("3\n");
}
