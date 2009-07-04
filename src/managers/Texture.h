#ifndef GL_TEXTURE_LOADER
#define GL_TEXTURE_LOADER

#include <vector>

typedef struct													// ����һ���ṹ��
{
	GLubyte	*imageData;											// ͼ������ (���32bit)
	GLuint	bpp;												// ÿһ���ص�ͼ����ɫ���
	GLuint	width;												// ͼ����
	GLuint	height;												// ͼ��߶�
	GLuint	texID;												// ����ID
} TextureTga;

// load bmp, jpg, files
bool BuildTexture(const char *szPathName, GLuint &texid);

// load tga file
bool BuildTexture(const char *filename, TextureTga *texture);

#endif