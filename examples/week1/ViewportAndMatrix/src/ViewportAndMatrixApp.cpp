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

class ViewportAndMatrixApp : public AppNative {
public:
	void prepareSettings(Settings* settings);
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
};

void ViewportAndMatrixApp::prepareSettings(Settings* settings)
{
	// Initialize a square window:
	settings->setWindowSize( 800, 800 );
}

void ViewportAndMatrixApp::setup()
{
}

void ViewportAndMatrixApp::mouseDown( MouseEvent event )
{
}

void ViewportAndMatrixApp::update()
{
}

void ViewportAndMatrixApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set viewport to window area:
	gl::setViewport( getWindowBounds() );
	
	{
		// Set matrices window from window size:
		gl::setMatricesWindow( getWindowSize() );
		
		// Set color:
		gl::color( 1, 0, 0 );
		
		// Draw red centered rectangle at 3/4 of window size:
		gl::drawSolidRect( Rectf( getWindowSize() * 0.125, getWindowSize() * 0.875 ) );
	}
	
	{
		// Set matrices window from unit size:
		gl::setMatricesWindow( Vec2i( 1, 1 ) );
		
		// Set color:
		gl::color( 0, 1, 0 );
		
		// Draw green centered rectangle at 1/2 of unit size:
		gl::drawSolidRect( Rectf( 0.25, 0.25, 0.75, 0.75 ) );
	}
	
	// Set viewport to center 1/4 of window area:
	gl::setViewport( Area( getWindowSize() * 0.375, getWindowSize() * 0.625 ) );
	
	{
		// Set matrices window from sine output range:
		gl::setMatricesWindow( Vec2i( 2, 2 ) );
		
		// Set color:
		gl::color( 0, 0, 1 );
		
		// Draw outline of viewport region:
		gl::drawStrokedRect( Rectf( 0.0, 0.0, 2.0, 2.0 ) );
		
		// Draw circle, oscillating across viewport x-axis:
		gl::drawSolidCircle( Vec2f( sin( getElapsedSeconds() ) + 1.0, 1.0 ), 0.5, 25 );
	}
	
	// Notice what happens when you change the window shape.
}

CINDER_APP_NATIVE( ViewportAndMatrixApp, RendererGl )
