#include "TextureManager.h"
#include "Texture.h"

// initialize the singleton to NULL
TextureManager* TextureManager::singleton = NULL;

// get the single instance of TextureManager
// if TextureManager is never used before, create a new instance
// and return it.
TextureManager* TextureManager::getInstance(){
	if(singleton == NULL){
		singleton = new TextureManager();
	}
	return singleton;
}

// constructor
TextureManager::TextureManager(void){
	textures = new TextureMap();
}

// destructor
TextureManager::~TextureManager(void){
	delete textures;
}

// if path is in the TextureMap:textures, then return the
// value it maps.(this texture has been loaded before)
// else load the texture and add the path and texture id
// as a pair to the TextureMap:textures.
GLuint TextureManager::getTextureId(const char* path){
    GLuint id;
	TextureMap::iterator it = textures->find(path);
	//haven't built this texture , build a new one
	if(it == textures->end()){
		BuildTexture(path,id);
		std::pair<std::string,GLuint> p;
		p.first = path;
		p.second = id;
		textures->insert(p);
	}
	//already have built one
	else
		id = (*it).second;
	return id;
}
