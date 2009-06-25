#include "ConcreteDisplay.h"

ConcreteDisplay::ConcreteDisplay(void){
}

ConcreteDisplay::~ConcreteDisplay(void){
}

void ConcreteDisplay::display(bool isWire,const std::vector<Mesh*>* toDisplay){
	
	//draw wire model
	if(isWire){
		displayWire(toDisplay);
	}
	//draw solid model
	else{
		displaySolid(toDisplay);
	}
}

void ConcreteDisplay::displayWire(const std::vector<Mesh*>* toDisplay){
	//each mesh
	glDisable(GL_TEXTURE);
	for(int i = 0; i < (signed int)toDisplay->size(); i++){
		Mesh* mesh = (*toDisplay)[i];
		glPushMatrix();
		glMultMatrixf(mesh->matrix);
		//each polygon
		for(int j = 0; j < (signed int)mesh->polygons->size(); j++){
			Polygon* polygon = (*mesh->polygons)[j];
			glBegin(GL_LINE_LOOP);
			//each vertex
			for(int k = 0; k < (signed int)polygon->vertices->size(); k++){
				Vector3D* dot = (*polygon->vertices)[k];
				glVertex3f(dot->x,dot->y,dot->z);
			}
			glEnd();
		}
		glPopMatrix();
	}
}

void ConcreteDisplay::displaySolid(const std::vector<Mesh*>* toDisplay){
	//each mesh
	for(int i = 0; i < (signed int)toDisplay->size(); i++){
		Mesh* mesh = (*toDisplay)[i];
		glPushMatrix();
		glMultMatrixf(mesh->matrix);
		//each polygon
		for(int j = 0; j < (signed int)mesh->polygons->size(); j++){
			Polygon* polygon = (*mesh->polygons)[j];
			//texture
			if(polygon->hasTexture){
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,polygon->texID);
			}
			//normal
			if(polygon->hasNormals) glEnable(GL_NORMALIZE);
			//material
			if(polygon->hasMaterial){
				glMaterialfv(GL_FRONT,GL_AMBIENT,polygon->ambient);
				glMaterialfv(GL_FRONT,GL_DIFFUSE,polygon->diffuse);
				glMaterialfv(GL_FRONT,GL_SPECULAR,polygon->specular);
				glMaterialfv(GL_FRONT,GL_EMISSION,polygon->emission);
				glMaterialf(GL_FRONT,GL_SHININESS,polygon->shininess);
			}
			glBegin(GL_POLYGON);
			//each vertex
			for(int k = 0; k < (signed int)polygon->vertices->size(); k++){
				Vector3D* dot = (*polygon->vertices)[k];
				//texture
				if(polygon->hasTexture){
					Vector3D* texCoord = (*polygon->verticeTexCoords)[k];
					glTexCoord2f(texCoord->x,texCoord->y);
				}
				if(polygon->hasNormals){
					Vector3D* normals = (*polygon->verticeNormals)[k];
					glNormal3f(normals->x,normals->y,normals->z);
				}
				glVertex3f(dot->x,dot->y,dot->z);
			}
			glEnd();
		}
		glPopMatrix();
	}
}
