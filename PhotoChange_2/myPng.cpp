#define _CRT_SECURE_NO_WARNINGS

#include "myPng.h"

//pngファイルの読み込みを行う関数
int pngFileReadDecode(BITMAPDATA_t* bitmapData, const char* filename) {

	FILE* fi;
	int j;
	unsigned int width, height;
	unsigned int readSize;

	png_structp png;
	png_infop info;
	png_bytepp datap;
	png_byte type;
	png_byte signature[8];

	fi = fopen(filename, "rb");
	if (fi == NULL) {
		printf("%sは開けません\n", filename);
		return -1;
	}

	//先頭8ビットを調べて、pngファイルか調べる
	//				↓格納先　↓項目のサイズ　　　↓ファイルポインタ
	readSize = fread(signature, 1, SIGNATURE_NUM, fi);
	//								↑読み取る項目の最大数(8)

	//pngファイルじゃないとき、関数を終了
	if (png_sig_cmp(signature, 0, SIGNATURE_NUM)) {
		printf("png_sig_cmp error!\n");
		fclose(fi);
		return -1;
	}

	/*　構造体の生成　*/
	//"png_create_read_struct"はpng_read構造体を生成する関数
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	//作れなかった時
	if (png == NULL) {
		printf("png_create_read_struct error!\n");
		fclose(fi);
		return -1;
	}

	//png_info構造体を生成
	info = png_create_info_struct(png);
	//作れなかった時
	if (info == NULL) {
		printf("png_crete_info_struct error!\n");
		png_destroy_read_struct(&png, NULL, NULL);
		fclose(fi);
		return -1;
	}

	png_init_io(png, fi);//読み込み先ファイルの設定
	png_set_sig_bytes(png, readSize);//シグネチャサイズの設定
	//pngファイルの読込
	//各画素の輝度値を8ビットで扱うための指定
	png_read_png(png, info, PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16, NULL);

	//読み込んだpngから画像データの情報を取得
	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	datap = png_get_rows(png, info);		//各行のデータのアドレスへのポインタ
	type = png_get_color_type(png, info);	//pngの色の形式


	/* RGB or RGBAだけ対応 */
	if (type != PNG_COLOR_TYPE_RGB && type != PNG_COLOR_TYPE_RGB_ALPHA) {
		printf("color type is not RGB or RGBA\n");
		png_destroy_read_struct(&png, &info, NULL);
		fclose(fi);
		return -1;
	}

	bitmapData->width = width;
	bitmapData->height = height;
	if (type == PNG_COLOR_TYPE_RGB) {
		bitmapData->ch = 3;
	}
	else if (type == PNG_COLOR_TYPE_RGBA) {
		bitmapData->ch = 4;
	}
	printf("width = %d, height = %d, ch = %d\n", bitmapData->width, bitmapData->height, bitmapData->ch);

	bitmapData->data =
		(unsigned char*)malloc(sizeof(unsigned char) * bitmapData->width * bitmapData->height * bitmapData->ch);
	if (bitmapData->data == NULL) {
		printf("data malloc error\n");
		png_destroy_read_struct(&png, &info, NULL);
		fclose(fi);
		return -1;
	}
	//BITMAPデータをメモリにmemcpyでコピー
	for (j = 0; j < bitmapData->height; j++) {
		memcpy(bitmapData->data + j * bitmapData->width * bitmapData->ch, datap[j], bitmapData->width * bitmapData->ch);
	}
	//生成した構造体の削除
	png_destroy_read_struct(&png, &info, NULL);
	fclose(fi);

	return 0;
}

//pngファイルの書き込みを行う関数
int pngFileEncodeWrite(BITMAPDATA_t* bitmapData, const char* filename) {
	FILE* fo;
	int j;

	png_structp png;
	png_infop info;
	png_bytepp datap;
	png_byte type;

	fo = fopen(filename, "wb");
	if (fo == NULL) {
		printf("%sは開けません\n", filename);
		return -1;
	}
	//png_write構造体
	png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	//png_info構造体
	info = png_create_info_struct(png);

	//pngファイルの色情報を設定
	//bitmapdata.chの値を参照して、色数を判定
	if (bitmapData->ch == 3) {
		type = PNG_COLOR_TYPE_RGB;
	}
	else if (bitmapData->ch == 4) {
		type = PNG_COLOR_TYPE_RGB_ALPHA;
	}
	else {
		printf("ch num is invalid!\n");
		png_destroy_write_struct(&png, &info);
		fclose(fo);
		return -1;
	}
	//書き込み先ファイルの設定
	png_init_io(png, fo);
	//pngのヘッダ設定　png画像がどのようなものであるかを表すデータ
	png_set_IHDR(png, info, bitmapData->width, bitmapData->height, 8, type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	/* libpngにPNGにエンコードしてもらうBITMAPのデータ設定 */

	//エンコードしてもらいたいBITMAPデータを格納するためのメモリを確保
	datap = (png_bytepp)png_malloc(png, sizeof(png_bytep) * bitmapData->height);

	//↑のメモリのアドレスを設定
	png_set_rows(png, info, datap);

	for (j = 0; j < bitmapData->height; j++) {
		//BITMAPデータのコピー
		datap[j] = (png_byte*)png_malloc(png, bitmapData->width * bitmapData->ch);
		memcpy(datap[j], bitmapData->data + j * bitmapData->width * bitmapData->ch, bitmapData->width * bitmapData->ch);
	}
	//ファイル書き込み
	png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);

	for (j = 0; j < bitmapData->height; j++) {
		png_free(png, datap[j]);
	}
	png_free(png, datap);

	//生成した構造体の削除
	png_destroy_write_struct(&png, &info);
	fclose(fo);
	return 0;
}


int freeBitmapData(BITMAPDATA_t* bitmap) {
	if (bitmap->data != NULL) {
		free(bitmap->data);
		bitmap->data = NULL;
	}
	return 0;
}