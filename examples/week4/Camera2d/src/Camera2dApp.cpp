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

class Camera2dApp : public AppNative {
  public:
	void setup();
	void mouseDown(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	bool		mPreserveAspect;
	CameraOrtho mCam;
};

void Camera2dApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Set initial state:
	mPreserveAspect = false;
}

void Camera2dApp::mouseDown(MouseEvent event)
{
}

void Camera2dApp::keyUp(KeyEvent event)
{
	if( event.getChar() == ' ' ) {
		mPreserveAspect = !mPreserveAspect;
	}
}

void Camera2dApp::update()
{
}

void Camera2dApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Choose checkerboard unit size:
	float tUnitLen = 50.0;
	
	// Try resizing the window!
	
	// Notice that when mPreserveAspect is false, we are setting our view
	// matrix from a square camera region in a potentially non-square window.
	// As a result, the checkerboard will appear distorted.
	// We can get around this by taking the window's aspect ratio into consideration
	// when setting the camera region, as shown below...
	
	// Hit the spacebar to toggle between modes!
	
	// Notice the order of parameters for the setOrtho method!
	// setOrtho( float left, float right, float bottom, float top, float nearPlane, float farPlane )
	
	// Set ortho camera region:
	if( mPreserveAspect ) {
		float tRatio = getWindowAspectRatio();
		mCam.setOrtho( -20 * tUnitLen, 20 * tUnitLen, ( 20 * tUnitLen ) / tRatio, ( -20 * tUnitLen ) / tRatio, -1.0, 1.0 );
	}
	else {
		mCam.setOrtho( -20 * tUnitLen, 20 * tUnitLen, 20 * tUnitLen, -20 * tUnitLen, -1.0, 1.0 );
	}
	
	// Set matrices from camera:
	gl::setMatrices( mCam );
	
	// Draw checkerboard centered on origin:
	for(int x = -10; x < 10; x++) {
		for(int y = -10; y < 10; y++) {
			float tMult = lmap<float>( y, -10.0, 10.0, 0.0, 1.0 );
			if( abs(x) % 2 != abs(y) % 2 ) {
				gl::color( tMult, 0.3, 0.3 );
			}
			else {
				gl::color( tMult, 0.7, 0.7 );
			}
			gl::drawSolidRect( Rectf( x * tUnitLen, y * tUnitLen, x * tUnitLen + tUnitLen, y * tUnitLen + tUnitLen ) );
		}
	}
	
}

CINDER_APP_NATIVE( Camera2dApp, RendererGl )
