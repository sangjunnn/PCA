//PCA_2011706153_이상준

#include "PCA.h"
#include <assert.h>

uchar** MemAlloc_2D(int width, int height) {
	uchar** arr;
	int i;

	arr = (uchar**)malloc(sizeof(uchar*)*height);	//2차원 동적 메모리 할당
	for (i = 0; i < height; i++) {
		arr[i] = (uchar*)malloc(sizeof(uchar)*width);
	}
	return arr;
}

int** MemAlloc_2D_int(int width, int height) {
	int** arr;
	int i;

	arr = (int**)malloc(sizeof(int*)*height);	//2차원 동적 메모리 할당
	for (i = 0; i < height; i++) {
		arr[i] = (int*)malloc(sizeof(int)*width);
	}
	return arr;
}

void MemFree_2D(uchar** arr, int height) {
	int i;
	for (i = 0; i < height; i++) {//2차원 동적 메모리 해제
		free(arr[i]);
	}
	free(arr);
}

void MemFree_2D_int(int** arr, int height) {
	int i;
	for (i = 0; i < height; i++) {//2차원 동적 메모리 해제
		free(arr[i]);
	}
	free(arr);
}

void FileRead(uchar** img_in, int width, int height, char* path) {
	uchar* img_BMP = (uchar*)malloc(width * height * 3 * sizeof(uchar));
	uchar* img_RAW = (uchar*)malloc(width * height * sizeof(uchar));

	int k = 0;

	for (int i = 1; i < 7; i++) {
		for (int j = 1; j < 9; j++) {
			char buffer[256];
			sprintf(buffer, "%s%d/%d.bmp", path, i, j);
			FILE* fp = fopen(buffer, "rb");

//			printf("%s\n", buffer);
			if (!fp) {
				assert(false);
			}

			fseek(fp, 54, SEEK_SET);
			fread(img_BMP, sizeof(uchar), width*height * 3, fp);
			fclose(fp);

			BMPtoRAW_Gray(img_BMP, img_RAW, width, height);

			memcpy(&img_in[k][0], img_RAW, width*height);
			k++;
		}
	}

	free(img_RAW);
	free(img_BMP);
}

void BMPtoRAW_Gray(uchar* in_bmp, uchar *out_raw, int width, int height) {
	int sum = 0;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			sum = in_bmp[(height - i)*width * 3 + 3 * j] + in_bmp[(height - i)*width * 3 + 3 * j + 1] + in_bmp[(height - i)*width * 3 + 3 * j + 2];
			out_raw[i*width + j] = sum / 3;
		}
	}
}

void GetAverageMatrix(uchar** img_ori, uchar** ave_ori, int width, int height) {
	int ave = 0;
	int cnt = 0;

	for (int i = 0; i < width*height; i++) {
		for (int j = 0; j < NUMFOLDERS; j++) {
			for (int k = 0; k < NUMFILES; k++) {
				ave += img_ori[j*NUMFOLDERS + k][i];
			}
			ave_ori[j][i] = ave / NUMFILES;
			
			cnt++;
		}
	}
}

void Transpose(int** img_ave_removed, int** img_ave_removed_T, int width, int height) {
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			img_ave_removed_T[w][h] = img_ave_removed[h][w];
		}
	}
}//h=NUMFOLDERS*NUMFILES//w=WIDTH*HEIGHT

void GetCovarianceMat(uchar** img_ori, uchar** img_ave, uchar** img_cov, int width, int height) {
	int** img_ave_removed_T = MemAlloc_2D_int(WIDTH * HEIGHT, NUMFOLDERS * NUMFILES);
	int** img_ave_removed = MemAlloc_2D_int(NUMFOLDERS * NUMFILES, WIDTH * HEIGHT);
	int sum;
	for (int w = 0; w < width*height; w++) {
		for (int f = 0; f < NUMFOLDERS; f++) {
			for (int h = 0; h < NUMFILES; h++) {
				img_ave_removed[w][f*NUMFOLDERS + h] = img_ori[w][f*NUMFOLDERS + h] - img_ave[w][f];
			}
		}
	}//각 영상에서 평균값 제거
	
	Transpose(img_ave_removed, img_ave_removed_T, NUMFOLDERS * NUMFILES, WIDTH * HEIGHT);
	
	for (int i = 0; i < WIDTH * HEIGHT;i++) {
		for (int j = 0; j < WIDTH * HEIGHT; j++) {
			sum = 0;
			for (int k = 0; k < NUMFILES*NUMFOLDERS; k++) {
				sum += img_ave_removed_T[i][k] * img_ave_removed[k][j];
			}
			img_cov[i][j] = sum;
		}
	}
	
	free(img_ave_removed);
	free(img_ave_removed_T);
}


