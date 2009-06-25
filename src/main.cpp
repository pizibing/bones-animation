#include <string.h>
#include <GL/glui.h>
#include "para.h"

float xy_aspect;
int main_window;

/** These are the live variables passed into GLUI ***/


/** Pointers to the windows and some of the controls we'll create **/


/********** Miscellaneous global variables **********/

GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};

GLfloat light1_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light1_diffuse[] =  {.9f, .6f, 0.0f, 1.0f};
GLfloat light1_position[] = {-1.0f, -1.0f, 1.0f, 0.0f};

/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void control_cb( int control ){
}

/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y)
{
	switch(Key)
	{
	case 27: 
	case 'q':
		exit(0);
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

void myGlutIdle( void )
{
	/* According to the GLUT specification, the current window is 
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if ( glutGetWindow() != main_window ) 
		glutSetWindow(main_window);  

	/*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
	application  */

	glutPostRedisplay();
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


/************************************************** draw_axes() **********/
/* Disables lighting, then draws RGB axes                                */


/***************************************** myGlutDisplay() *****************/

void myGlutDisplay( void )
{
	glClearColor( .9f, .9f, .9f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glutSwapBuffers(); 
}


/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	/*glutInitWindowPosition( 50, 50 );*/
	glutInitWindowSize( WIN_WIDTH, WIN_HEIGHT );

	main_window = glutCreateWindow( WIN_TITLE );
	glutDisplayFunc( myGlutDisplay );
	GLUI_Master.set_glutReshapeFunc( myGlutReshape );  
	GLUI_Master.set_glutKeyboardFunc( myGlutKeyboard );
	GLUI_Master.set_glutSpecialFunc( NULL );
	GLUI_Master.set_glutMouseFunc( myGlutMouse );
	glutMotionFunc( myGlutMotion );

	glEnable(GL_DEPTH_TEST);

#if 0
	/**** We register the idle callback with GLUI, *not* with GLUT ****/
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
#endif

	/**** Regular GLUT main loop ****/

	glutMainLoop();

	return EXIT_SUCCESS;
}

