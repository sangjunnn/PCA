//PCA_2011706153_이상준

#include "func.h"
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

double** MemAlloc_2D_double(int width, int height) {
	double** arr;
	int i;

	arr = (double**)malloc(sizeof(double*)*height);	//2차원 동적 메모리 할당
	for (i = 0; i < height; i++) {
		arr[i] = (double*)malloc(sizeof(double)*width);
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

void MemFree_2D_double(double** arr, int height) {
	int i;
	for (i = 0; i < height; i++) {//2차원 동적 메모리 해제
		free(arr[i]);
	}
	free(arr);
}

void Training_FileRead(uchar** img_in, int width, int height, char* path, int sub) {
	uchar* img_BMP = (uchar*)malloc(width * height * 3 * sizeof(uchar));
	uchar* img_RAW = (uchar*)malloc(width * height * sizeof(uchar));
	uchar* img_RAW_sub = (uchar*)malloc(width * height * sizeof(uchar) / (sub*sub));
	int k = 0;

	for (int i = 1; i < 7; i++) {
		for (int j = 1; j < 9; j++) {
			char buffer[256];
			sprintf(buffer, "%s%d/%d.bmp", path, i, j);
			FILE* fp = fopen(buffer, "rb");

			if (!fp) {
				assert(false);
			}

			fseek(fp, 54, SEEK_SET);
			fread(img_BMP, sizeof(uchar), width*height * 3, fp);
			fclose(fp);

			BMPtoRAW_Gray(img_BMP, img_RAW, width, height);
			subSampling(img_RAW, img_RAW_sub, width, height,sub);
			memcpy(&img_in[k][0], img_RAW_sub, width*height / (sub*sub));
			k++;
		}
	}
	free(img_RAW_sub);
	free(img_RAW);
	free(img_BMP);
}

void Test_FileRead(uchar** img_in, double** exp, double** effect, int width, int height, char* path,int sub) {
	uchar* img_BMP = (uchar*)malloc(width * height * 3 * sizeof(uchar));
	uchar* img_RAW = (uchar*)malloc(width * height * sizeof(uchar));
	uchar* img_RAW_sub = (uchar*)malloc(width * height * sizeof(uchar) / (sub*sub));
	double** expect = MemAlloc_2D_double(NUMTEST,10);
	int k = 0;
	double distance = 0;
	//double* dist = (double*)malloc(sizeof(double)*NUMFOLDERS);

	for (int i = 1; i <= NUMTEST; i++) {
		char buffer[256];
		sprintf(buffer, "%s/%d.bmp", path, i);
		FILE* fp = fopen(buffer, "rb");

		if (!fp) {
			assert(false);
		}

		fseek(fp, 54, SEEK_SET);
		fread(img_BMP, sizeof(uchar), width*height * 3, fp);
		fclose(fp);

		BMPtoRAW_Gray(img_BMP, img_RAW, width, height);
		subSampling(img_RAW, img_RAW_sub, width, height, sub);
		for (int k = 0; k < NUMFOLDERS; k++){
			for (int m = 0; m < 10; m++){
				for (int j = 0; j < width*height / (sub*sub); j++){
					expect[m][i] += effect[m][j] * img_RAW_sub[j];
				}
			}
			distance = getEuclideanDistance(expect, exp, NUMTEST, 10, sub);
			printf("%d 번째 test 이미지와 %d 번째 training class의 유클리디안 거리 : %f\n",i, k+1, distance);
			//dist[k] = distance;
			//printf("%lf\n", dist[k]);

		}
		printf("\n");
	}

	//for (int i = 0; i < 10; i++){
	//	for (int j = 0; j < NUMTEST; j++){
	//		
	//	}
	//}

	//for (int i = 0; i < width*height / (sub*sub); i++){
	//	img_RAW_sub[i]
	//}

	free(img_RAW_sub);
	free(img_RAW);
	free(img_BMP);
}

double getEuclideanDistance(double** in1, double** in2, int width, int height, int sub){
	double distance;
	double temp=0;

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			temp += pow((in1[i][j] - in2[i][j]), 2);
		}
	}
	distance = sqrt(temp);

	return distance;
}


void subSampling(uchar* img_raw, uchar* img_out, int width, int height,int sub) {
	int sum = 0;

	for (int h = 0; h < height; h += sub) {
		for (int w = 0; w < width; w += sub) {
			sum = 0;
			for (int i = 0; i < sub; i++) {
				for (int j = 0; j < sub; j++) {
					sum += img_raw[(h + i)*width + w + j];
				}
			}
			img_out[h/sub*width/sub + w/sub] = (uchar)sum / (sub*sub);
		}
	}
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

void GetAverageMatrix(uchar** img_ori, uchar** ave_ori, int width, int height,int sub) {
	int ave = 0;

	for (int i = 0; i < width*height / (sub*sub); i++) {
		for (int j = 0; j < NUMFOLDERS; j++) {
			for (int k = 0; k < NUMFILES; k++) {
				ave += img_ori[i][j*NUMFOLDERS + k];
			}
			ave_ori[i][j] = ave / NUMFILES;
		}
	}
}

void Transpose_uchar(uchar** img, uchar** img_T, int width, int height) {
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			img_T[w][h] = img[h][w];
		}
	}
}

void Transpose(double** img_ave_removed, double** img_ave_removed_T, int width, int height) {
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			img_ave_removed_T[w][h] = img_ave_removed[h][w];
		}
	}
}//h=NUMFOLDERS*NUMFILES//w=WIDTH*HEIGHT

double** GetCovarianceMat(uchar** img_ori, uchar** img_ave, double** img_cov, double** exp, int width, int height,int sub) {
	double** img_ave_removed_T = MemAlloc_2D_double(WIDTH * HEIGHT / (sub*sub), NUMFOLDERS);
	double** img_ave_removed = MemAlloc_2D_double(NUMFOLDERS, WIDTH * HEIGHT / (sub*sub));
	double** effective_eigen = MemAlloc_2D_double(10, WIDTH * HEIGHT / (sub*sub));
	double** effective_eigen_T = MemAlloc_2D_double(WIDTH * HEIGHT / (sub*sub), 10);
	
	int sum;

	for (int w = 0; w < width*height/(sub*sub); w++) {
		for (int f = 0; f < NUMFOLDERS; f++) {
			for (int h = 0; h < NUMFILES; h++) {
				//img_ave_removed[w][f*NUMFILES + h] = (double)(img_ori[w][f*NUMFILES + h] - img_ave[w][f]);
				img_ave_removed[w][f] = (double)(img_ori[w][f*NUMFILES + h] - img_ave[w][f]);
			}
		}
	}//각 영상에서 평균값 제거
	
	Transpose(img_ave_removed, img_ave_removed_T, NUMFOLDERS, WIDTH * HEIGHT / (sub*sub));

	for (int i = 0; i < WIDTH * HEIGHT / (sub*sub); i++) {
		for (int j = 0; j < WIDTH * HEIGHT / (sub*sub); j++) {
			sum = 0;
			for (int k = 0; k < NUMFOLDERS; k++) {
				sum += img_ave_removed[i][k] * img_ave_removed_T[k][j];
			}
			img_cov[i][j] = sum;//cov=>((92x112)x(92x112))Mat
		}
	}
	
	effective_eigen = JacobiRotation(img_cov, width, height, sub);

	Transpose(effective_eigen, effective_eigen_T, 10, WIDTH * HEIGHT / (sub*sub));

	for (int j = 0; j < 10; j++){
		for (int k = 0; k < NUMFOLDERS; k++){
			for (int i = 0; i < WIDTH * HEIGHT / (sub*sub); i++){
				exp[j][k] = effective_eigen_T[j][i] * img_ave[i][k];
			}
		}
	}

	MemFree_2D_double(img_ave_removed_T,NUMFOLDERS);
	MemFree_2D_double(img_ave_removed, WIDTH * HEIGHT / (sub*sub));
	MemFree_2D_double(effective_eigen, WIDTH * HEIGHT / (sub*sub));

	return effective_eigen_T;
}

double** JacobiRotation(double** img_cov, int width, int height, int sub) {
	double thresh = 0.0;
	int n = width*height / (sub*sub);
	double theta;
	double temp;
	double ori1, ori2;
	int numif = 0;

	double** effectiv_eigen = MemAlloc_2D_double(10, n);
	double* Diogonal = (double*)malloc(sizeof(double)*n);
	double** EigenVector = MemAlloc_2D_double(n, n);

	for (int i = 0; i < n; i++){
		EigenVector[i][i] = 1.0;
		for (int j = 0; j < n; j++){
			if (j != i){
				EigenVector[i][j] = 0.0;
			}
		}
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			theta = 0.0;
			if (fabs(img_cov[i][j]) > thresh) {
				theta = atan((2 * img_cov[i][j]) / (img_cov[i][i] - img_cov[j][j])) / 2;

				temp = img_cov[i][i] * cos(theta)*cos(theta) + 2 * img_cov[i][j] * sin(theta)*cos(theta) + img_cov[j][j] * sin(theta)*sin(theta);
			
				for (int m = 0; m < i; m++){
					ori1 = img_cov[m][i];
					ori2 = img_cov[m][j];
					img_cov[m][i] = ori1*cos(theta) + ori2*sin(theta);
					img_cov[i][m] = img_cov[m][i];
				}

				for (int m = i+1; m < n; m++){
					ori1 = img_cov[m][i];
					ori2 = img_cov[m][j];
					img_cov[i][m] = ori1*cos(theta) + ori2*sin(theta);
					img_cov[m][i] = img_cov[i][m];
				}

				for (int m = 0; m < j; m++){
					ori1 = img_cov[m][i];
					ori2 = img_cov[m][j];
					img_cov[m][j] = ori2*cos(theta) - ori1*sin(theta);
					img_cov[j][m] = img_cov[m][j];
				}

				for (int m = j+1; m < n; m++){
					ori1 = img_cov[m][i];
					ori2 = img_cov[m][j];
					img_cov[j][m] = ori2*cos(theta) - ori1*sin(theta);
					img_cov[m][j] = img_cov[j][m];
				}

				for (int m = 0; m < n; m++){
					ori1 = EigenVector[m][i];
					ori2 = EigenVector[m][j];
					EigenVector[i][m] = ori1*cos(theta) + ori2*sin(theta);
					EigenVector[j][m] = ori2*cos(theta) - ori1*sin(theta);
				}

				img_cov[i][i] = temp;
				img_cov[j][j] = temp;

				thresh = thresh + img_cov[i][j] * img_cov[i][j];

				img_cov[i][j] = 0;
			}
			/*if (numif % 5000 == 0){
				printf("%d\n", numif);*/
			//}
			//numif++;
			thresh = sqrt(thresh) / (double)(4 * n);
		}
	}
	
	for (int i = 0; i < n; i++) {
		Diogonal[i] = img_cov[i][i];
	}

	sorting(Diogonal,EigenVector, n);

	for (int i = 0; i < n; i++){
		for (int j = 0; j < 10; j++){
			effectiv_eigen[i][j] = EigenVector[i][n-j];
		}
	}
	
	free(Diogonal);
	MemFree_2D_double(EigenVector, n);

	return effectiv_eigen;
}

void sorting(double* arr, double** arr2, int size) {	//버블정렬(작은->큰)
	bool flag = 0;
	double temp = 0;

	for (int i = 0; i < size - 1; i++) {
		flag = false;
		for (int j = 0; j < size - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				flag = true;
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				for (int m = 0; m < size; m++){
					temp = arr2[j][m];
					arr2[j][m] = arr2[j + 1][m];
					arr2[j + 1][m] = temp;
				}
			}
		}
		if (flag == false) {
			break;
		}
	}
}


