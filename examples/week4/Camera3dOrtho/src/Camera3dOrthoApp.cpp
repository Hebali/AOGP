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

class Camera3dOrthoApp : public AppNative {
public:
	void setup();
	void mouseMove(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	float		mParam;
	CameraOrtho mCam;
};

void Camera3dOrthoApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Set initial state:
	mParam = 0.5;
}

void Camera3dOrthoApp::mouseMove(MouseEvent event)
{
	mParam = max( 0.0f, min( 1.0f, static_cast<float>( event.getX() ) / static_cast<float>( getWindowWidth() ) ) );
}

void Camera3dOrthoApp::keyUp(KeyEvent event)
{
}

void Camera3dOrthoApp::update()
{
}

void Camera3dOrthoApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
		
	// Compute camera parameters:
	float tParamX = mParam * ( getWindowWidth() / 2.0 );
	float tParamY = mParam * ( getWindowHeight() / 2.0 );
	
	// Setup camera:
	mCam.setOrtho( -tParamX, tParamX, -tParamY, tParamY, -1000, 1000 );
	
	// Update camera position:
	mCam.lookAt( Vec3f( cos( getElapsedSeconds() ), sin( getElapsedSeconds() ) + 2.0, sin( getElapsedSeconds() ) ) * 30.0, Vec3f::zero() );
	
	// Set matrix from camera:
	gl::setMatrices( mCam );
	
	// Choose unit size:
	float tUnitLen = 10.0;
	float tHalfLen = tUnitLen / 2.0;
	
	// Draw color cube grid:
	for(int x = -10; x < 10; x++) {
		for(int z = -10; z < 10; z++) {
			gl::drawColorCube( Vec3f( x * tUnitLen, 0.0, z * tUnitLen ), Vec3f( tHalfLen, tHalfLen, tHalfLen ) );
		}
	}
}

CINDER_APP_NATIVE( Camera3dOrthoApp, RendererGl )
