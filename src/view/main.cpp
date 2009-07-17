#include <string.h>
#include <GL/glui.h>
#include "para.h"
#include "../commands/Command.h"
#include <windows.h>	
#include "glInfo.h"
#include "gl/glext.h"
#include <iostream>

// this main.cpp is the main class of the whole system
// it contains all the UIs
// it contains the basic flow of glut
// custormized parameters such as window's height and width etc are stored in para.h

// function pointers for VBO Extension
// Windows needs to get function pointers from ICD OpenGL drivers,
// because opengl32.dll does not support extensions higher than v1.1.
#ifdef _WIN32
PFNGLGENBUFFERSARBPROC pglGenBuffersARB = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC pglBindBufferARB = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC pglBufferDataARB = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC pglMapBufferARB = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB = 0;                   // unmap VBO procedure

#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB
#endif

//////////////////////////////////////////////////////////////////////
float xy_aspect;//screen width/height
Command* command;//command that handles all events

/** These are the live variables passed into GLUI ***/


/** Pointers to the windows and some of the controls we'll create **/
int main_window;//id of main window
//GLUI_FileBrowser *fb; // file browser
GLUI *control; // control panel that contains all controller


/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */
// this function is the callback of the UI
// control tells which event has been sprung
// all control's possible values are defined in para.h
void control_cb( int control ){
	switch(control){
		//// choose a file to open
		//case OPEN_FILE:
		//	command->loadModel(fb->get_file());
		//	break;
		//// push down the choose model button
		//case OPEN_FILE_DOWN:
		//	GLUI* openfile = GLUI_Master.create_glui( "open file..",0,950,100);
		//	fb = new GLUI_FileBrowser(openfile,"", false,OPEN_FILE,control_cb);
		//	break;
	}
}

/***************************************** myGlutDisplay() *****************/
// display function of glut
void myGlutDisplay( void )
{
	glClearColor( 0,0,0,1 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, EYE_SIGHT );

	//draw scene
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	command->drawScene();

	glutSwapBuffers(); 
}

/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y)
{
	switch(Key)
	{
	case 'q':
		exit(0);
		break;
	// rotate camera clockwise
	case 'z':
		command->rotateCamera(0.1);
		break;
	// rotate camera anticlockwise
	case 'x':
		command->rotateCamera(-0.1);
		break;
	// character move forward
	case GLUT_KEY_UP:
		break;
	// character move backward
	case GLUT_KEY_DOWN:
		break;
	// character move left
	case GLUT_KEY_LEFT:
		break;
	// character move right
	case GLUT_KEY_RIGHT:
		break;
	//default
	default:
		break;
	};

	glutPostRedisplay();
}


/***************************************** myGlutMenu() ***********/

void myGlutMenu( int value )
{
	myGlutKeyboard( value, 0, 0 );
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle(int value)
{
	//record begin time of this frame
	SYSTEMTIME frameBegin;
	GetLocalTime(&frameBegin);

	/* According to the GLUT specification, the current window is 
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if ( glutGetWindow() != main_window ) 
		glutSetWindow(main_window);  

	/*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
	application  */

	myGlutDisplay();

	//record end time of this frame
	SYSTEMTIME frameEnd;
	GetLocalTime(&frameEnd);

	//calculate milliseconds this frame took
	int frameBetween;
	if(frameEnd.wSecond == frameBegin.wSecond)
		frameBetween = frameEnd.wMilliseconds - frameBegin.wMilliseconds;
	else
		frameBetween = 1000 + frameEnd.wMilliseconds - frameBegin.wMilliseconds;

	//control fps(mspf), and display next frame
	if(frameBetween >= MSPF) 
		glutTimerFunc(0,myGlutIdle,0);
	else
		glutTimerFunc(MSPF - frameBetween,myGlutIdle,0);
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y ){
}


/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y ){
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );

	xy_aspect = (float)tw / (float)th;

	glutPostRedisplay();
}

//init gl
void initGL(){
	glClearDepth(1.0f);
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
}

//init glut
void initGlut(int argc, char* argv[]){
	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	/*glutInitWindowPosition( 50, 50 );*/
	glutInitWindowSize( WIN_WIDTH, WIN_HEIGHT );

	main_window = glutCreateWindow( WIN_TITLE );
	glutDisplayFunc( myGlutDisplay );
	//GLUI_Master.set_glutIdleFunc(myGlutIdle);
	GLUI_Master.set_glutReshapeFunc( myGlutReshape );  
	GLUI_Master.set_glutKeyboardFunc( myGlutKeyboard );
	GLUI_Master.set_glutSpecialFunc( NULL );
	GLUI_Master.set_glutMouseFunc( myGlutMouse );
	glutMotionFunc( myGlutMotion );
}

// init the scene
void initScene(){
	command->loadModel();
}

// check whether user computer support VBO
// if not , terminate the system
void checkVBO(){

	// get OpenGL info
	glInfo glInfo;
	glInfo.getInfo();
	//glInfo.printSelf();

#ifdef _WIN32
	// check VBO is supported by your video card
	if(glInfo.isExtensionSupported("GL_ARB_vertex_buffer_object"))
	{
		// get pointers to GL functions
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
		glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
		glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
		glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
		glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

		// check once again VBO extension
		if(glGenBuffersARB && glBindBufferARB && glBufferDataARB && glBufferSubDataARB &&
			glMapBufferARB && glUnmapBufferARB && glDeleteBuffersARB && glGetBufferParameterivARB)
		{
			std::cout << "Video card supports GL_ARB_vertex_buffer_object." << std::endl;
		}
		else
		{
			std::cout << "Video card does NOT support GL_ARB_vertex_buffer_object." << std::endl;
			exit(0);
		}
	}
#endif
}


// create UI using glui
void createUI(){
	// create a outer panel, set its position
	control = GLUI_Master.create_glui( "demo control",0,850,50);
	//// open file button
	//GLUI_Button* openFile = 
	//	control->add_button("choose model",OPEN_FILE_DOWN,control_cb);
}

/**************************************** main() ********************/
int main(int argc, char* argv[])
{
	//init glut
	initGlut(argc, argv);

	//init gl
	initGL();

	//check whether user computer support VBO
	// if not , terminate the system
	checkVBO();

	//initialize command
	command = new Command();

	//init scene (load model)
	initScene();

	// create UI
	createUI();

#if 0
	/**** We register the idle callback with GLUI, *not* with GLUT ****/
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
#endif

	glutTimerFunc(MSPF,myGlutIdle,0);
	/**** Regular GLUT main loop ****/

	glutMainLoop();

	return EXIT_SUCCESS;
}

