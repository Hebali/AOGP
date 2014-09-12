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

class GLDrawArraysApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	std::vector<float> mPointComponents;
};

void GLDrawArraysApp::setup()
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
	mPointComponents.resize( tPointCount * 3 );
	
	// Generate random points:
	for(size_t i = 0; i < tPointCount; i++) {
		for(size_t j = 0; j < 3; j++) {
			mPointComponents[ i * 3 + j ] = ci::randFloat( -1.0, 1.0 );
		}
	}
}

void GLDrawArraysApp::mouseDown(MouseEvent event)
{
}

void GLDrawArraysApp::keyUp(KeyEvent event)
{
}

void GLDrawArraysApp::update()
{
}

void GLDrawArraysApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set color:
	gl::color( Color( 1, 0, 0 ) );
	
	// Set point display size:
	glPointSize( 5.0 );
	
	// Set camera and matrices for 3D:
	CameraPersp tCam;
	tCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	tCam.lookAt( Vec3f( cos( getElapsedSeconds() ), 0, sin( getElapsedSeconds() ) ), Vec3f::zero() );
	gl::setMatrices( tCam );
	
	// Get vertex count:
	size_t tVertCount = mPointComponents.size() / 3;
	
	// Get current iterator:
	size_t tIter = getElapsedFrames() % 2;
	
	// Two approaches to drawing a container of point-components:
	
	if( tIter == 0 ) {
		// Prepare to draw the points:
		glBegin(GL_POINTS);
		
		// Iterate over and draw each point:
		for(size_t i = 0; i < tVertCount; i++) {
			glVertex3f( mPointComponents[ i * 3 ], mPointComponents[ i * 3 + 1 ], mPointComponents[ i * 3 + 2 ] );
		}
		
		// End draw:
		glEnd();
	}
	else {
		// Draw points, faster:
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3, GL_FLOAT, 0, &( mPointComponents[0] ) );
		glDrawArrays( GL_POINTS, 0, tVertCount );
		glDisableClientState( GL_VERTEX_ARRAY );
	}
}

CINDER_APP_NATIVE( GLDrawArraysApp, RendererGl )
