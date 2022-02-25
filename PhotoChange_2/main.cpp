#define _CRT_SECURE_NO_WARNINGS

#include "myPng.h"

int main(int argc, char* argv[]) {

	BITMAPDATA_t bitmap;
	int i, j, c;
	int ave, sum;
	char outname[256];

	FILE* fo;

	if (argc != 2) {
		printf("�t�@�C�������w�肳��Ă��܂���\n");
		return -1;
	}
	//png�t�@�C���̓ǂݍ���
	if (pngFileReadDecode(&bitmap,argv[1]) == -1) {
		printf("pngFileReadDecode error\n");
		return -1;
	}

	printf("bitmap->data = %p\n", bitmap.data);
	printf("bitmap->width = %d\n", bitmap.width);
	printf("bitmap->height = %d\n", bitmap.height);
	printf("bitmap->ch = %d\n", bitmap.ch);

	/* �O���[�X�P�[���ɕϊ� */
	for (j = 0; j < bitmap.height; j++) {
		for (i = 0; i < bitmap.width; i++) {
			sum = 0;
			for (c = 0; c < bitmap.ch; c++) {
				//RGB�̋P�x�l�̍��v
				sum += bitmap.data[bitmap.ch * (i + j * bitmap.width) + c];
			}
			ave = sum / bitmap.ch;//�P�x�l�̍��v���摜�̐F��
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
	//�������J�����ďI��
	freeBitmapData(&bitmap);

	return 0;
}