#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

//BITMAP形式の情報格納用構造体 
typedef struct {
	unsigned char* data;	//pngをでコードしたBITMAP形式画像データの先頭アドレス
	unsigned int width;		//画像の横幅
	unsigned int height;	//画像の高さ
	unsigned int ch;		//画像の色数
} BITMAPDATA_t;

int pngFileReadDecode(BITMAPDATA_t*, const char*);
int pngFileEncodeWrite(BITMAPDATA_t*, const char*);
int freeBitmapData(BITMAPDATA_t*);

#define SIGNATURE_NUM 8