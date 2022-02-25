#define _CRT_SECURE_NO_WARNINGS

#include "myPng.h"

int main(int argc, char* argv[]) {

	BITMAPDATA_t bitmap;
	int i, j, c;
	int ave, sum;
	char outname[256];

	FILE* fo;

	if (argc != 2) {
		printf("ファイル名が指定されていません\n");
		return -1;
	}
	//pngファイルの読み込み
	if (pngFileReadDecode(&bitmap,argv[1]) == -1) {
		printf("pngFileReadDecode error\n");
		return -1;
	}

	printf("bitmap->data = %p\n", bitmap.data);
	printf("bitmap->width = %d\n", bitmap.width);
	printf("bitmap->height = %d\n", bitmap.height);
	printf("bitmap->ch = %d\n", bitmap.ch);

	/* グレースケールに変換 */
	for (j = 0; j < bitmap.height; j++) {
		for (i = 0; i < bitmap.width; i++) {
			sum = 0;
			for (c = 0; c < bitmap.ch; c++) {
				//RGBの輝度値の合計
				sum += bitmap.data[bitmap.ch * (i + j * bitmap.width) + c];
			}
			ave = sum / bitmap.ch;//輝度値の合計÷画像の色数
			for (c = 0; c < bitmap.ch; c++) {
				bitmap.data[bitmap.ch * (i + j * bitmap.width) + c] = ave;
			}
		}
	}

	sprintf(outname, "%s", "output.PNG");

	if (pngFileEncodeWrite(&bitmap, outname) == -1) {
		printf("pngFileEncodeWrite error\n");
		freeBitmapData(&bitmap);
		return -1;
	}
	//メモリ開放して終了
	freeBitmapData(&bitmap);

	return 0;
}