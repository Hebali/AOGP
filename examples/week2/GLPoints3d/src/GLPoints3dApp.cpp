//////////////////////////////////////////////////
/*          Art of Graphics Programming         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2014                   */
//////////////////////////////////////////////////

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GLPoints3dApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	std::vector<ci::Vec3f> mPoints;
};

void GLPoints3dApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Seed random number generator:
	ci::randSeed( (unsigned)time( NULL ) );
	
	// Choose a point count:
	size_t tPointCount = 1000;
	
	// Resize point vector:
	mPoints.resize( tPointCount );
	
	// Generate random points:
	for(size_t i = 0; i < tPointCount; i++) {
		mPoints[ i ] = Vec3f( ci::randFloat( -1.0, 1.0 ), ci::randFloat( -1.0, 1.0 ), ci::randFloat( -1.0, 1.0 ) );
	}
}

void GLPoints3dApp::mouseDown(MouseEvent event)
{
}

void GLPoints3dApp::keyUp(KeyEvent event)
{
}

void GLPoints3dApp::update()
{
}

void GLPoints3dApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set color:
	gl::color( Color( 1, 0, 0 ) );
	
	// Set camera and matrices for 3D:
	CameraPersp tCam;
	tCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	tCam.lookAt( Vec3f( cos( getElapsedSeconds() ), 0, sin( getElapsedSeconds() ) ), Vec3f::zero() );
	gl::setMatrices( tCam );
	
	// Get point count:
	size_t tPointCount = mPoints.size();
	
	// Set point display size:
	glPointSize( 5.0 );
	
	// Prepare to draw the points:
	glBegin(GL_POINTS);
	// Iterate over and draw each point:
	for(int i = 0; i < tPointCount; i++) {
		glVertex3f( mPoints[ i ].x, mPoints[ i ].y, mPoints[ i ].z );
	}
	// End draw:
	glEnd();
}

CINDER_APP_NATIVE( GLPoints3dApp, RendererGl )
