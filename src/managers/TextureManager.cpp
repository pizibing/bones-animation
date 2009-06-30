#include "TextureManager.h"
#include "Texture.h"

TextureManager* TextureManager::singleton = NULL;

TextureManager* TextureManager::getInstance(){
	if(singleton == NULL){
		singleton = new TextureManager();
	}
	return singleton;
}

TextureManager::TextureManager(void){
	textures = new TextureMap();
}

TextureManager::~TextureManager(void){
	delete textures;
}

GLuint TextureManager::getTextureId(char* path){
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
