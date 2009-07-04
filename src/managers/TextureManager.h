#pragma once

#include <map>
#include <string>
#include <gl/glut.h>

// define type TextureMap
typedef std::map<std::string,GLuint,std::less<std::string>> TextureMap;

// TextureManager is a class that manages texture
// it is a singleton class.
// it maintains a map of path to texture id, so every texture
// will be loaded only once.
class TextureManager
{
public:
	// get the single instance of TextureManager
	static TextureManager* getInstance();

	// if path is in the TextureMap:textures, then return the
	// value it maps.(this texture has been loaded before)
	// else load the texture and add the path and texture id
	// as a pair to the TextureMap:textures.
	GLuint getTextureId(const char* path);

private:
	// singleton
	static TextureManager* singleton;

	// constructor
	TextureManager(void);
	// destructor
	~TextureManager(void);

	// a map stores the texture path as the key and texture id
	// as the value
	TextureMap* textures;
};
