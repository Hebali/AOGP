//////////////////////////////////////////////////
/*          Art of Graphics Programming         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2014                   */
//////////////////////////////////////////////////

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GLLinesApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
};

void GLLinesApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
}

void GLLinesApp::mouseDown(MouseEvent event)
{
}

void GLLinesApp::keyUp(KeyEvent event)
{
}

void GLLinesApp::update()
{
}

void GLLinesApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set matrices window from unit size:
	gl::setMatricesWindow( Vec2i( 1, 1 ) );
	
	// Set line display width:
	glLineWidth( 2.0 );
	
	// Get current iterator:
	size_t tIter = getElapsedFrames() % 3;
	
	// Three approaches to drawing lines:
	
	// GL_LINES:
	if( tIter == 0 ) {
		// Begin:
		glBegin(GL_LINES);

		// TOP:
		// Left, Top:
		gl::color( Color( 1, 0, 0 ) );
		glVertex2f( 0.25, 0.25 );
		// Right, Top:
		gl::color( Color( 0, 1, 0 ) );
		glVertex2f( 0.75, 0.25 );
		
		// RIGHT:
		// Right, Top:
		gl::color( Color( 0, 1, 0 ) );
		glVertex2f( 0.75, 0.25 );
		// Right, Bottom:
		gl::color( Color( 0, 0, 1 ) );
		glVertex2f( 0.75, 0.75 );
		
		// BOTTOM:
		// Right, Bottom:
		gl::color( Color( 0, 0, 1 ) );
		glVertex2f( 0.75, 0.75 );
		// Left, Bottom:
		gl::color( Color( 1, 1, 1 ) );
		glVertex2f( 0.25, 0.75 );
		
		// LEFT:
		// Left, Bottom:
		gl::color( Color( 1, 1, 1 ) );
		glVertex2f( 0.25, 0.75 );
		// Left, Top:
		gl::color( Color( 1, 0, 0 ) );
		glVertex2f( 0.25, 0.25 );
		
		// End:
		glEnd();
	}
	// GL_LINE_STRIP:
	else if ( tIter == 1 ) {
		// Begin:
		glBegin(GL_LINE_STRIP);
		
		// Left, Top:
		gl::color( Color( 1, 0, 0 ) );
		glVertex2f( 0.25, 0.25 );
		
		// Right, Top:
		gl::color( Color( 0, 1, 0 ) );
		glVertex2f( 0.75, 0.25 );
		
		// Right, Bottom:
		gl::color( Color( 0, 0, 1 ) );
		glVertex2f( 0.75, 0.75 );
		
		// Left, Bottom:
		gl::color( Color( 1, 1, 1 ) );
		glVertex2f( 0.25, 0.75 );
		
		// Left, Top:
		gl::color( Color( 1, 0, 0 ) );
		glVertex2f( 0.25, 0.25 );
		
		// End:
		glEnd();
	}
	// GL_LINE_LOOP:
	else {
		// Begin:
		glBegin(GL_LINE_LOOP);
		
		// Left, Top:
		gl::color( Color( 1, 0, 0 ) );
		glVertex2f( 0.25, 0.25 );
		
		// Right, Top:
		gl::color( Color( 0, 1, 0 ) );
		glVertex2f( 0.75, 0.25 );
		
		// Right, Bottom:
		gl::color( Color( 0, 0, 1 ) );
		glVertex2f( 0.75, 0.75 );
		
		// Left, Bottom:
		gl::color( Color( 1, 1, 1 ) );
		glVertex2f( 0.25, 0.75 );
		
		// End:
		glEnd();
	}
}

CINDER_APP_NATIVE( GLLinesApp, RendererGl )
