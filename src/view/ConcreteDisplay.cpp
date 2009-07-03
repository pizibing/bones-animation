#include <gl/glut.h>
#include <gl/glext.h>

#include "../matrixlib/Vector3D.h"
#include "../data/MyPolygon.h"
#include "../data/Mesh.h"
//#include "../data/VBO.h"
#include "../data/VBOMesh.h"
#include "Display.h"
#include "ConcreteDisplay.h"

extern PFNGLBINDBUFFERARBPROC pglBindBufferARB;
#define glBindBufferARB    pglBindBufferARB

ConcreteDisplay::ConcreteDisplay(void){
}

ConcreteDisplay::~ConcreteDisplay(void){
}

/*
* para: toDisplay: the point to a list of Mesh* to be displayed
*       isWire: true:wire, false:solid
* func: draw all the meshes in the openGL world space in wire/solid model
* return: void
*/
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

// used by void display(bool isWire,const std::vector<Mesh*>* toDisplay);
// display Mesh in wire model
void ConcreteDisplay::displayWire(const std::vector<Mesh*>* toDisplay){
	//each mesh
	glDisable(GL_TEXTURE);
	glColor3f(1,1,1);
	for(int i = 0; i < (signed int)toDisplay->size(); i++){
		Mesh* mesh = (*toDisplay)[i];
		glPushMatrix();
		glMultMatrixf(mesh->matrix);
		//each polygon
		for(int j = 0; j < (signed int)mesh->polygons->size(); j++){
			MyPolygon* polygon = (*mesh->polygons)[j];
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

// used by void display(bool isWire,const std::vector<Mesh*>* toDisplay);
// display Mesh in solid model
void ConcreteDisplay::displaySolid(const std::vector<Mesh*>* toDisplay){
	//each mesh
	for(int i = 0; i < (signed int)toDisplay->size(); i++){
		Mesh* mesh = (*toDisplay)[i];
		glPushMatrix();
		glMultMatrixf(mesh->matrix);
		//each polygon
		for(int j = 0; j < (signed int)mesh->polygons->size(); j++){
			MyPolygon* polygon = (*mesh->polygons)[j];
			//texture
			if(polygon->hasTexture){
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,polygon->texID);
			}
			//normal
			if(polygon->hasNormals) glEnable(GL_LIGHTING);
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
			//back to default
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
		}
		glPopMatrix();
	}
}


// display a set of VBOMesh
// toDisplay specify the VBOMeshes to display
// num is the number of VBOMesh to display
// isWire specify whether to display in wire model or solid model
// vertices in VBOMesh will be treated as triangles
void ConcreteDisplay::display(bool isWire,const VBOMesh* toDisplay,int num){
	
	//wire model
	if(isWire){

	}
	//solid model
	else{
		//for each VBOMesh
		for(int i = 0; i < num; i++){
			glPushMatrix();

			VBOMesh vbomesh= toDisplay[i];
			//set matrix
			glMultMatrixf(vbomesh.getMatrix());
			//if has material, set material
			if(vbomesh.getHasMaterial()){
				//set ambient
				glMaterialfv(GL_FRONT,GL_AMBIENT,vbomesh.getAmbient());
				// set diffuse
				glMaterialfv(GL_FRONT,GL_DIFFUSE,vbomesh.getDiffuse());
				// set specular
				glMaterialfv(GL_FRONT,GL_SPECULAR,vbomesh.getSpecular());
				// set emission
				glMaterialfv(GL_FRONT,GL_EMISSION,vbomesh.getEmission());
				// set shininess
				glMaterialf(GL_FRONT,GL_SHININESS,vbomesh.getShininess());
			}
			//set vertices
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,vbomesh.getVertices());
			glVertexPointer(3,GL_FLOAT,0,0);
			glEnableClientState(GL_VERTEX_ARRAY);
			//if has texture, set texture
			if(vbomesh.getHasTexture()){
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,vbomesh.getTexID());
				glBindBufferARB(GL_ARRAY_BUFFER_ARB,vbomesh.getTextures());
				glTexCoordPointer(2,GL_FLOAT,0,0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			//if has normal, set normal
			if(vbomesh.getHasNormal()){
				glEnable(GL_LIGHTING);
				glBindBufferARB(GL_ARRAY_BUFFER_ARB,vbomesh.getNormals());
				glNormalPointer(GL_FLOAT,0,0);
				glEnableClientState(GL_NORMAL_ARRAY);
			}
			// draw element
			glDrawArrays(GL_TRIANGLES, 0, vbomesh.getSize());
			// release bind buffer
			glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
			// set back client state
			glDisableClientState( GL_VERTEX_ARRAY );	
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
			glDisable(GL_NORMAL_ARRAY);
			//back to default
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);

			glPopMatrix();
		}
	}
}
