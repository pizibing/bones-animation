#pragma once

#define WIN_TITLE "Bone Animation"
const int WIN_WIDTH = 800;// width of the initial window
const int WIN_HEIGHT = 600;// height of the initial window

const float EYE_SIGHT = 1000; //the range can be seen

// MSPF is the millisecond per frame
// it tells the glut how much time it should change a frame
const int MSPF = 30;

/* here defines possible values of control*/
const int DISPLAY_IN_LINE_DOWN = 0; // push choose model button
const int DISPLAY_IN_MESH_DOWN = 1; // choose a file to open

// the frame number arrow key's effect should last
const int ARROW_LAST_FRAME = 5;