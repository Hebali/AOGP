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

class MatrixTransformApp : public AppNative {
  public:
	void setup();
	void mouseDown(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	void drawStrokedCubeUncentered(const Vec3f &position, const Vec3f &size);
	
	CameraPersp mCam;
	bool		mPaused;
	size_t		mIndex, mMinIndex, mMaxIndex;
};

void MatrixTransformApp::drawStrokedCubeUncentered(const Vec3f& ori, const Vec3f& size)
{
	Vec3f max = ori + size;
	
	gl::drawLine( Vec3f(ori.x, ori.y, ori.z), Vec3f(max.x, ori.y, ori.z) );
	gl::drawLine( Vec3f(max.x, ori.y, ori.z), Vec3f(max.x, max.y, ori.z) );
	gl::drawLine( Vec3f(max.x, max.y, ori.z), Vec3f(ori.x, max.y, ori.z) );
	gl::drawLine( Vec3f(ori.x, max.y, ori.z), Vec3f(ori.x, ori.y, ori.z) );
	
	gl::drawLine( Vec3f(ori.x, ori.y, max.z), Vec3f(max.x, ori.y, max.z) );
	gl::drawLine( Vec3f(max.x, ori.y, max.z), Vec3f(max.x, max.y, max.z) );
	gl::drawLine( Vec3f(max.x, max.y, max.z), Vec3f(ori.x, max.y, max.z) );
	gl::drawLine( Vec3f(ori.x, max.y, max.z), Vec3f(ori.x, ori.y, max.z) );
	
	gl::drawLine( Vec3f(ori.x, ori.y, ori.z), Vec3f(ori.x, ori.y, max.z) );
	gl::drawLine( Vec3f(ori.x, max.y, ori.z), Vec3f(ori.x, max.y, max.z) );
	gl::drawLine( Vec3f(max.x, max.y, ori.z), Vec3f(max.x, max.y, max.z) );
	gl::drawLine( Vec3f(max.x, ori.y, ori.z), Vec3f(max.x, ori.y, max.z) );
}

void MatrixTransformApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Setup camera:
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	
	// Setup state handlers:
	mMinIndex = 0;
	mMaxIndex = 4;
	mIndex    = mMinIndex;
	mPaused   = false;
}

void MatrixTransformApp::mouseDown( MouseEvent event )
{
}

void MatrixTransformApp::keyUp(KeyEvent event)
{
	switch( event.getChar() ) {
		case ' ': {
			mPaused = !mPaused;
			break;
		}
		default: { break; }
	}
}

void MatrixTransformApp::update()
{
}

void MatrixTransformApp::draw()
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
	Vec3f tCubeDimension   = Vec3f( 1.0, 2.0, 2.0 );
	Vec3f tCubeTranslation = Vec3f( 1.5, 0.0, 0.0 );
	Vec3f tCubeScale       = Vec3f( 0.5, 0.5, 0.5 );
	Vec3f tCubeRotation    = Vec3f( getElapsedSeconds() * 45.0, 0.0, 0.0 );
	
	// Draw 3D geometry (that HAS been pre-centered about the origin):
	{
		// Set color:
		gl::color( Color( 0.0, 1.0, 0.0 ) );
		
		// Push matrix:
		gl::pushMatrices();
		
		// Apply translation:
		gl::translate( tCubeTranslation );
		
		// Apply rotation:
		gl::rotate( tCubeRotation );
		
		// Apply scaling:
		gl::scale( tCubeScale );
		
		// Draw cube:
		gl::drawStrokedCube( Vec3f::zero(), tCubeDimension );
		
		// Pop matrix:
		gl::popMatrices();
	}
	
	// Draw 3D geometry (that HAS NOT been pre-centered about the origin):
	{
		// Set color and line width:
		if( mIndex > 3 ) {
			gl::color( Color( 1.0, 1.0, 1.0 ) );
			gl::lineWidth( 3.0 );
		}
		else {
			gl::color( Color( 1.0, 0.0, 0.0 ) );
			gl::lineWidth( 1.0 );
		}
		
		// Push matrix:
		gl::pushMatrices();
		
		if( mIndex > 0 ) {
			// Apply translation:
			gl::translate( tCubeTranslation );
		}
		
		if( mIndex > 1 ) {
			// Apply rotation:
			gl::rotate( tCubeRotation );
		}
		
		if( mIndex > 2 ) {
			// Apply scaling:
			gl::scale( tCubeScale );
		}
		
		if( mIndex > 3 ) {
			// Center cube about its origin:
			gl::translate( - tCubeDimension * 0.5 );
		}
		
		// Draw cube:
		drawStrokedCubeUncentered( Vec3f::zero(), tCubeDimension );
		
		// Pop matrix:
		gl::popMatrices();
	}
	
	// Update index:
	if( !mPaused && getElapsedFrames() % 100 == 0 ) {
		mIndex++;
		if( mIndex > mMaxIndex ) {
			mIndex = mMinIndex;
		}
	}
}

CINDER_APP_NATIVE( MatrixTransformApp, RendererGl )
