#include <gl/glut.h>
#include <gl/glext.h>
#include <assert.h>

#include "../data/SimpleLine.h"
#include "../matrixlib/Vector3D.h"
#include "../data/VBOMesh.h"
#include "Display.h"
#include "ConcreteDisplay.h"

// declare some VBO function which is defined in main.cpp
extern PFNGLBINDBUFFERARBPROC pglBindBufferARB;
#define glBindBufferARB    pglBindBufferARB

ConcreteDisplay::ConcreteDisplay(void){
}

ConcreteDisplay::~ConcreteDisplay(void){
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

// display a group of lines
// toDisplay specifies the array of lines to display
// num is the length of this array
void ConcreteDisplay::display(const SimpleLine* toDisplay, int num){
	
	// toDisplay should not be null
	assert(toDisplay);
	// num should not be smaller than 0
	assert(num >= 0);

	// draw lines
	// set light, texture, color
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	// for each line
	for(int i = 0; i < num; i++){
		SimpleLine line = toDisplay[i];
		// draw line
		glBegin(GL_LINE);
		glVertex3fv(line.getDot1());
		glVertex3fv(line.getDot2());
		glEnd();
	}
}

