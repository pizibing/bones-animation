#include <string.h>
#include <GL/glui.h>
#include "para.h"
#include "../commands/Command.h"

float xy_aspect;//screen width/height
int main_window;//main window
Command* command;//command

/** These are the live variables passed into GLUI ***/


/** Pointers to the windows and some of the controls we'll create **/


/********** Miscellaneous global variables **********/

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
	glClearColor( 0,0,0,1 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0 );

	//draw scene
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	command->drawScene();

	glutSwapBuffers(); 
}

//init glut
void initGL(){
	glClearDepth(1.0f);
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
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

	initGL();

	//initialize command
	command = new Command();

#if 0
	/**** We register the idle callback with GLUI, *not* with GLUT ****/
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
#endif

	/**** Regular GLUT main loop ****/

	glutMainLoop();

	return EXIT_SUCCESS;
}

