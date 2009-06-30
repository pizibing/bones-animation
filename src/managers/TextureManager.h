#pragma once

#include <map>
#include <string>
#include <gl/glut.h>

typedef std::map<std::string,GLuint,std::less<std::string>> TextureMap;

class TextureManager
{
public:
	static TextureManager* getInstance();

	GLuint getTextureId(char* path);

private:
	static TextureManager* singleton;
	TextureManager(void);
	~TextureManager(void);

	TextureMap* textures;
};
