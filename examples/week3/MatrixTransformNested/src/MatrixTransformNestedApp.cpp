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

using namespace ci;
using namespace ci::app;
using namespace std;

class MatrixTransformNestedApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	CameraPersp mCam;
};

void MatrixTransformNestedApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Setup camera:
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
}

void MatrixTransformNestedApp::mouseDown( MouseEvent event )
{
}

void MatrixTransformNestedApp::keyUp(KeyEvent event)
{
}

void MatrixTransformNestedApp::update()
{
}

void MatrixTransformNestedApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Update camera position:
	mCam.lookAt( Vec3f( cos( getElapsedSeconds() ), 1.0, sin( getElapsedSeconds() ) ) * 5.0, Vec3f::zero() );
	
	// Set matrix from camera:
	gl::setMatrices( mCam );
	
	// Set color:
	gl::color( Color( 1.0, 1.0, 1.0 ) );
	
	// Set line width:
	gl::lineWidth( 1.0 );
	
	// Draw origin:
	gl::drawCoordinateFrame();
	
	// Prepare matrix transformation settings:
	Vec3f tOuterCubeDimension   = Vec3f( 1.0, 2.0, 2.0 );
	Vec3f tOuterCubeTranslation = Vec3f( 1.5, 0.0, 0.0 );
	Vec3f tOuterCubeScale       = Vec3f( 0.5, 0.5, 0.5 );
	Vec3f tOuterCubeRotation    = Vec3f( getElapsedSeconds() * 90.0, 0.0, 0.0 );
	
	Vec3f tInnerCubeDimension   = Vec3f( 3.0, 3.0, 3.0 );
	Vec3f tInnerCubeTranslation = Vec3f( 0.0, 3.0, 0.0 );
	Vec3f tInnerCubeScale       = Vec3f( 0.25, 0.25, 0.25 );
	Vec3f tInnerCubeRotation    = Vec3f( getElapsedSeconds() * 180.0, 0.0, 0.0 );
	
	// Draw 3D geometry (that HAS been pre-centered about the origin):
	{
		// Set color:
		gl::color( Color( 0.0, 1.0, 0.0 ) );
		
		// Push outer matrix:
		gl::pushMatrices();
		
		// Apply translation:
		gl::translate( tOuterCubeTranslation );
		
		// Apply rotation:
		gl::rotate( tOuterCubeRotation );
		
		// Apply scaling:
		gl::scale( tOuterCubeScale );
		
		// Draw cube:
		gl::drawStrokedCube( Vec3f::zero(), tOuterCubeDimension );
		
		{
			// Set color:
			gl::color( Color( 0.0, 1.0, 1.0 ) );
			
			// Push inner matrix:
			gl::pushMatrices();
			
			// Apply translation:
			gl::translate( tInnerCubeTranslation );
			
			// Apply rotation:
			gl::rotate( tInnerCubeRotation );
			
			// Apply scaling:
			gl::scale( tInnerCubeScale );
			
			// Draw cube:
			gl::drawStrokedCube( Vec3f::zero(), tInnerCubeDimension );
			
			// Pop inner matrix:
			gl::popMatrices();
		}
		
		// Pop outer matrix:
		gl::popMatrices();
	}
}

CINDER_APP_NATIVE( MatrixTransformNestedApp, RendererGl )
