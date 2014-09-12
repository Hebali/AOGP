//////////////////////////////////////////////////
/*          Art of Graphics Programming         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2014                   */
//////////////////////////////////////////////////

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GLPoints2dApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	std::vector<ci::Vec2f> mPoints;
};

void GLPoints2dApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Seed random number generator:
	ci::randSeed( (unsigned)time( NULL ) );
	
	// Choose a point count:
	size_t tPointCount = 100;
	
	// Resize point vector:
	mPoints.resize( tPointCount );
	
	// Generate random points:
	for(size_t i = 0; i < tPointCount; i++) {
		mPoints[ i ] = Vec2f( ci::randFloat( 0.0, 1.0 ), ci::randFloat( 0.0, 1.0 ) );
	}
}

void GLPoints2dApp::mouseDown(MouseEvent event)
{
}

void GLPoints2dApp::keyUp(KeyEvent event)
{
}

void GLPoints2dApp::update()
{
}

void GLPoints2dApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set color:
	gl::color( Color( 1, 0, 0 ) );
	
	// Set matrices window from unit size:
	gl::setMatricesWindow( Vec2i( 1, 1 ) );
	
	// Get point count:
	size_t tPointCount = mPoints.size();
	
	// Set point display size:
	glPointSize( 5.0 );
	
	// Prepare to draw the points:
	glBegin(GL_POINTS);
	// Iterate over and draw each point:
	for(int i = 0; i < tPointCount; i++) {
		glVertex2f( mPoints[ i ].x, mPoints[ i ].y );
	}
	// End draw:
	glEnd();
}

CINDER_APP_NATIVE( GLPoints2dApp, RendererGl )
