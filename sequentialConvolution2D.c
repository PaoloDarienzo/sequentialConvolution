#include <stdio.h>

#include <sys/time.h> // for Linux

//#include <iostream>
//#include <windows.h> // for Windows APIs
//using namespace std;

/*!
* \brief Sequential convolution on a 2D array, with a 2D mask; float matrices, reading input and mask matrices from file.
* \author Paolo D'Arienzo
*/

#define MASK_WIDTH1 77 //kernel rows
#define MASK_WIDTH2 77 //kernel columns
#define WIDTH1 1000 //rows
#define WIDTH2 1000 //columns

void matrix_Print(float A[][WIDTH2]);

void matrix_Print_Mask(float M[][MASK_WIDTH2]);

int mask_Loading(float M[][MASK_WIDTH2]);

int input_Loading(float N[][WIDTH2]);

int output_Writing(float P[][WIDTH2]);

/*!
* Convolution function.
* @param N, M, P pointers to input and output matrices.
*/
void convolution_2D(float N[][WIDTH2], float M[][MASK_WIDTH2], float P[][WIDTH2]) {

	// find center position of kernel (half of kernel size)
	int kCenterX = MASK_WIDTH2 / 2;
	int kCenterY = MASK_WIDTH1 / 2;

	for (int i = 0; i < WIDTH1; i++)					// rows
	{
		for (int j = 0; j < WIDTH2; j++)				// columns
		{
			for (int m = 0; m < MASK_WIDTH1; m++)		// kernel rows
			{
				for (int n = 0; n < MASK_WIDTH2; n++)	// kernel columns
				{
					// index of input signal, used for checking boundary
					int ii = i + (m - kCenterY);
					int jj = j + (n - kCenterX);

					// ignore input samples which are out of bound
					if (ii >= 0 && ii < WIDTH1 && jj >= 0 && jj < WIDTH2)
						P[i][j] += N[ii][jj] * M[m][n];
				}
			}
		}
	}

}

int main(void) {

	/*
	//Time calculation on Windows

	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks

	//QueryPerformanceFrequency(&frequency);
	*/

	//Time calculation on Linux
	struct timeval tv1, tv2;

	double timeExec = 0; //contains time of execution

	FILE *fp;
	//Pointer to the file where will be stored the execution time
	fp = fopen("/storage/pdarienzo/tirocinio/tesi/timestat.txt", "a+");
	if (fp == NULL) {
		printf("Failed to open timestat.txt file.\n");
		return(-1);
	}

	float N[WIDTH1][WIDTH2] = { { 0 } };
	float P[WIDTH1][WIDTH2] = { { 0 } };
	float M[MASK_WIDTH1][MASK_WIDTH2] = { { 0 } };

	printf("Loading input matrix N... \n");
	input_Loading(N);
	printf("N loaded. \n");
	//matrix_Print(N);

	printf("Loading kernel matrix M... \n");
	mask_Loading(M);
	printf("M loaded. \n");
	//matrix_Print_Mask(M);

	printf("Convolution... \n");

	// start timer
	//Time calculation on Linux
	gettimeofday(&tv1, NULL);
	//Time calculation on Windows
	//QueryPerformanceCounter(&t1);

	//Convolution
	convolution_2D(N, M, P);

	// stop timer
	//Time calculation on Linux
	gettimeofday(&tv2, NULL);
	//Time calculation on Windows
	//QueryPerformanceCounter(&t2);

	printf("Writing resulting matrix P. \n");
	//matrix_Print(P);
	output_Writing(P);

	/*
	//Time calculation on Windows
	// compute and print the elapsed time in millisec
	timeExec = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
	cout << elapsedTime << " ms.\n";
	*/
	//Time calculation on Linux
	timeExec = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
	printf("Total time = %lf seconds\n", timeExec);

	fprintf(fp, "\n======================================================\n");
	fprintf(fp, "(float) P: %ix%i, (float) M: %ix%i\n\n", WIDTH1, WIDTH2, MASK_WIDTH1, MASK_WIDTH2);
	fprintf(fp, "Execution time of sequential convolution 2D: %lf seconds.", timeExec);
	fprintf(fp, "\n");


	fclose(fp);

	printf("\nPress any key to continue... ");
	//getc(stdin);

	return 0;

}

/*!
* Function that prints the input/output matrix.
* @param N pointer to input matrix.
*/
void matrix_Print(float A[][WIDTH2]) {

	for (int i = 0; i < WIDTH1; i++) {
		for (int j = 0; j < WIDTH2; j++)
			printf("%f\t", A[i][j]);
		printf("\n");
	}
	printf("\n");

}

/*!
* Function that prints the mask matrix.
* @param M pointer to mask matrix.
*/
void matrix_Print_Mask(float M[][MASK_WIDTH2]) {

	for (int i = 0; i < MASK_WIDTH1; i++) {
		for (int j = 0; j < MASK_WIDTH2; j++)
			printf("%f\t", M[i][j]);
		printf("\n");
	}
	printf("\n");

}

/*!
* Input loading that is read from a file.
* @param N pointer to input matrix.
*/
int input_Loading(float N[][WIDTH2]) {

	FILE *fp;
	//fp = fopen("/storage/pdarienzo/tirocinio/tesi/inputValues/bigIntInput.txt", "r");
	fp = fopen("/storage/pdarienzo/tirocinio/tesi/inputValues/bigFloatInput.txt", "r");

	if (fp == NULL) {
		printf("Failed to open input file.\n");
		return(-1);
	}
	else {
		for (int i = 0; i < WIDTH1; i++)
			for (int j = 0; j < WIDTH2; j++)
				fscanf(fp, "%f", &N[i][j]);
	}

	fclose(fp);

	return 0;

}

/*!
* Mask input loading that read from a file.
* @param M pointer to mask matrix.
*/
int mask_Loading(float M[][MASK_WIDTH2]) {

	FILE *fp;
	//Sharpen_Mask has MW1=3, MW2=3 (int)
	//fp = fopen("/storage/pdarienzo/tirocinio/tesi/inputValues/Sharpen_Mask.txt", "r");
	//Sobel_Mask has MW1=9, MW2=9 (int)
	//fp = fopen("/storage/pdarienzo/tirocinio/tesi/inputValues/Sobel_Mask.txt", "r");
	//Box_blur_Mask has MW1=3, MW2=3 (float)
	//fp = fopen("/storage/pdarienzo/tirocinio/tesi/inputValues/Box_blur_Mask.txt", "r");
	//Gaussian_blur_Mask has MW1=7, MW2=7 (float)
	//fp = fopen("/storage/pdarienzo/tirocinio/tesi/inputValues/Gaussian_blur_Mask.txt", "r");
	//Stress_Mask1 has MW1=32, MW2=32 (float)
	//fp = fopen("/storage/pdarienzo/tirocinio/tesi/inputValues/Stress_Mask1.txt", "r");
	//Stress_Mask2 has MW1=77, MW2=77 (float)
	fp = fopen("/storage/pdarienzo/tirocinio/tesi/inputValues/Stress_Mask2.txt", "r");

	if (fp == NULL) {
		printf("Failed to open mask input file.\n");
		return(-1);
	}
	else {
		for (int i = 0; i < MASK_WIDTH1; i++)
			for (int j = 0; j < MASK_WIDTH2; j++)
				fscanf(fp, "%f", &M[i][j]);
	}

	fclose(fp);

	return 0;

}

/*!
* Output printing on a file.
* @param P pointer to output matrix.
*/
int output_Writing(float P[][WIDTH2]) {

	FILE *fp;
	fp = fopen("/storage/pdarienzo/tirocinio/tesi/outputValues/sequentialConvolution2DOutput.txt", "w");

	if (fp == NULL) {
		printf("Failed to open output file.\n");
		return(-1);
	}
	else {
		for (int i = 0; i < WIDTH1; i++) {
			for (int j = 0; j < WIDTH2; j++)
				fprintf(fp, "%6.3f\t", P[i][j]);
			fprintf(fp, "\n");
		}
	}

	fclose(fp);

	return 0;

}
