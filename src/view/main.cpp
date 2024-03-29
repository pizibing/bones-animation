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

// function declare
void myGlutIdle(int value);

//////////////////////////////////////////////////////////////////////
float xy_aspect;//screen width/height
Command* command;//command that handles all events

/* these are variables that record the states of the view */
// arrow key down state of every frame,default state is ARROW_NONE
// other states are ARROW_UP,ARROW_DOWN,ARROW_LEFT,ARROW_RIGHT
int arrowState = 0;
// button state
bool updown = false;
bool downdown = false;
bool leftdown = false;
bool rightdown = false;
// when one key is down its effect should last for several frames
// this value record the frame it lasts
int arrowLast = 0;
// fps information
static float fps = 0;
char printFloat[32];

/** These are the live variables passed into GLUI ***/
bool displayInLineDown = false;

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
		// push down display in lines button
		case DISPLAY_IN_LINE_DOWN:
			displayInLineDown = true;
			break;
		// push down display in meshes button
		case DISPLAY_IN_MESH_DOWN:
			displayInLineDown = false;
			break;
		default:
			break;
	}
}

// draw 2D statistics information about the view on the screen
void drawInformation(){
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();   
	glLoadIdentity();   
	gluLookAt (0.0, 0.0, 5.0,   // vecEyePos    
		0.0, 0.0, 0.0,  // vecLookat    
		0.0, 1.0, 0.0); // vecUp  
	glColor3f(1.0f,1.0f,1.0f);
	//fps : **.**
	static char* print = "fps :";
	for(int i = 0; print[i] != '\0';i++){
		glRasterPos2f(-2.5+0.05*i,-1.75);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,print[i]);
	}
	
	
	for(int i = 0; i < 5; i++){
		glRasterPos2f(-2.1+0.05*i,-1.75);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,printFloat[i]);
	}

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
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

	//draw 3D scene
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	// display in lines
	if(displayInLineDown)
		command->drawLineCharacter();
	// display in meshes
	else
		command->drawScene();

	// draw 2D information
	drawInformation();

	glutSwapBuffers(); 
}

/**************************************** myGlutKeyboard() **********/
// deal with regular keys
void myGlutKeyboard(unsigned char Key, int x, int y)
{
	switch(Key)
	{
	case 'q':
		exit(0);
		break;
	// rotate camera clockwise
	case 'z':
		command->rotateCamera(3);
		break;
	// rotate camera anticlockwise
	case 'x':
		command->rotateCamera(-3);
		break;
	// move camera closer to the character
	case 'a':
		command->moveCloser();
		break;
	// move camera far from the character
	case 's':
		command->moveAway();
		break;
	//default
	default:
		break;
	};
}

// deal with special keys up events
void myGlutSpecialUp(int a_keys, int x, int y){
	switch(a_keys){
		// set corresponding key down false
	case GLUT_KEY_UP:
		updown = false;
		break;
	case GLUT_KEY_DOWN:
		downdown = false;
		break;
	case GLUT_KEY_LEFT:
		leftdown = false;
		break;
	case GLUT_KEY_RIGHT:
		rightdown = false;
		break;
	default:
		break;
	}
}

// deal with special keys include arrow keys and F1-F10
void myGlutSpecial(int a_keys, int x, int y){
	switch(a_keys){
		// character move forward
	case GLUT_KEY_UP:
		updown = true;
		arrowLast = 0;
		break;
		// character move backward
	case GLUT_KEY_DOWN:
		downdown = true;
		arrowLast = 0;
		break;
		// character move left
	case GLUT_KEY_LEFT:
		leftdown = true;
		arrowLast = 0;
		break;
		// character move right
	case GLUT_KEY_RIGHT:
		rightdown = true;
		arrowLast = 0;
		break;
	default:
		break;
	}
}

// according to the arrow key states to decide the arrowState
void arrowControl(){
	// if up and left arrow key are pushed down
	if(updown && leftdown) arrowState = ARROW_LEFT_UP;
	// if down and left arrow key are pushed down
	else if(downdown && leftdown) arrowState = ARROW_LEFT_DOWN;
	// if up and right arrow key are pushed down
	else if(updown && rightdown) arrowState = ARROW_RIGHT_UP;
	// if down and right arrow key are pushed down
	else if(downdown && rightdown) arrowState = ARROW_RIGHT_DOWN;
	// only up arrow key is pushed down
	else if(updown) arrowState = ARROW_UP;
	// only down arrow key is pushed down
	else if(downdown) arrowState = ARROW_DOWN;
	// only left arrow key is pushed down
	else if(leftdown) arrowState = ARROW_LEFT;
	// only right arrow key is pushed down
	else if(rightdown) arrowState = arrowState = ARROW_RIGHT;
}

// control the frame update time to MSPF
// frameBegin is the start time of every frame
// frameEnd is the end calculation time of every frame
void frameControl(SYSTEMTIME frameBegin,SYSTEMTIME frameEnd){
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

/***************************************** myGlutIdle() ***********/

void myGlutIdle(int value)
{
	// record begin time of this frame
	SYSTEMTIME frameBegin;
	GetLocalTime(&frameBegin);

	// calculate fps
	static SYSTEMTIME latestFrameTime;
	// add fps every time
	fps++;
	int change_time = frameBegin.wMilliseconds - latestFrameTime.wMilliseconds;
	if(change_time < 0){ 
		// time pass 1 sec update real fps
		sprintf(printFloat,"%.0f",fps);
		// recount fps
		fps = 0;
	}

	// record latest frame time
	latestFrameTime = frameBegin;

	/* According to the GLUT specification, the current window is 
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if ( glutGetWindow() != main_window ) 
		glutSetWindow(main_window);  

	// decide arrowState
	arrowControl();

	// move the character
	command->moveCharacter(arrowState);

	// display
	myGlutDisplay();

	// set arrow key state to default, if it is out of effect
	if(arrowLast > ARROW_LAST_FRAME)
		arrowState = ARROW_NONE;
	// increase arrowLast
	arrowLast++;

	//record end time of this frame
	SYSTEMTIME frameEnd;
	GetLocalTime(&frameEnd);

	// control the frame time
	frameControl(frameBegin,frameEnd);
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
	GLUI_Master.set_glutSpecialFunc( myGlutSpecial );
	GLUI_Master.set_glutMouseFunc( myGlutMouse );
	glutSpecialUpFunc(myGlutSpecialUp);
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
	// display in lines 
	GLUI_Button* displayLine = 
		control->add_button("Display In Lines",DISPLAY_IN_LINE_DOWN,control_cb);
	// display in meshes
	GLUI_Button* displayMesh = 
		control->add_button("Display In Meshes",DISPLAY_IN_MESH_DOWN,control_cb);
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

