#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

//BITMAP�`���̏��i�[�p�\���� 
typedef struct {
	unsigned char* data;	//png���ŃR�[�h����BITMAP�`���摜�f�[�^�̐擪�A�h���X
	unsigned int width;		//�摜�̉���
	unsigned int height;	//�摜�̍���
	unsigned int ch;		//�摜�̐F��
} BITMAPDATA_t;

int pngFileReadDecode(BITMAPDATA_t*, const char*);
int pngFileEncodeWrite(BITMAPDATA_t*, const char*);
int freeBitmapData(BITMAPDATA_t*);

#define SIGNATURE_NUM 8