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

class CinderDrawApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	bool	mPaused;
	size_t	mIndex, mMinIndex, mMaxIndex;
};

void CinderDrawApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Setup state handlers:
	mMinIndex =  0;
	mMaxIndex = 20;
	mIndex    = mMinIndex;
	mPaused   = false;
}

void CinderDrawApp::mouseDown(MouseEvent event)
{
}

void CinderDrawApp::keyUp(KeyEvent event)
{
	switch( event.getChar() ) {
		case ' ': {
			mPaused = !mPaused;
			break;
		}
		default: { break; }
	}
}

void CinderDrawApp::update()
{
}

void CinderDrawApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set color:
	gl::color( Color( 1, 0, 0 ) );
	
	// Set matrices for 2D:
	if( mIndex < 12 ) {
		gl::setMatricesWindow( getWindowSize() );
		
		gl::lineWidth( 2.0 );
	}
	// Set camera and matrices for 3D:
	else {
		CameraPersp tCam;
		tCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
		tCam.lookAt( Vec3f( cos( getElapsedSeconds() ) * 25.0, 0, sin( getElapsedSeconds() ) * 25.0 ), Vec3f::zero() );
		gl::setMatrices( tCam );
		
		gl::lineWidth( 1.0 );
	}
	
	// Cast window size to Vec2f:
	Vec2f tWinSize = static_cast<Vec2f>( getWindowSize() );
	
	// Handle the current index:
	switch ( mIndex ) {
		case 0: {
			gl::drawLine( tWinSize * 0.25, tWinSize * 0.75 );
			break;
		}
		case 1: {
			Path2d tPath;
			tPath.arc( tWinSize * 0.5, tWinSize.x * 0.25, 0.0, M_PI * 1.5 );
			gl::draw( tPath );
			break;
		}
		case 2: {
			Vec2f tA = tWinSize * Vec2f( 0.5,  0.25 ) ;
			Vec2f tB = tWinSize * Vec2f( 0.25, 0.75 );
			Vec2f tC = tWinSize * Vec2f( 0.75, 0.75 );
			gl::drawStrokedTriangle( tA, tB, tC );
			break;
		}
		case 3: {
			Vec2f tA = tWinSize * Vec2f( 0.5,  0.25 ) ;
			Vec2f tB = tWinSize * Vec2f( 0.25, 0.75 );
			Vec2f tC = tWinSize * Vec2f( 0.75, 0.75 );
			gl::drawSolidTriangle( tA, tB, tC );
			break;
		}
		case 4: {
			gl::drawStrokedRect( Rectf( tWinSize * 0.25, tWinSize * 0.75 ) );
			break;
		}
		case 5: {
			gl::drawSolidRect( Rectf( tWinSize * 0.25, tWinSize * 0.75 ) );
			break;
		}
		case 6: {
			gl::drawStrokedRoundedRect( Rectf( tWinSize * 0.25, tWinSize * 0.75 ), 25.0 );
			break;
		}
		case 7: {
			gl::drawSolidRoundedRect( Rectf( tWinSize * 0.25, tWinSize * 0.75 ), 25.0 );
			break;
		}
		case 8: {
			gl::drawStrokedCircle( tWinSize * 0.5, tWinSize.x * 0.25 );
			break;
		}
		case 9: {
			gl::drawSolidCircle( tWinSize * 0.5, tWinSize.x * 0.25 );
			break;
		}
		case 10: {
			gl::drawStrokedEllipse( tWinSize * 0.5, tWinSize.x * 0.25, tWinSize.y * 0.25 );
			break;
		}
		case 11: {
			gl::drawSolidEllipse( tWinSize * 0.5, tWinSize.x * 0.25, tWinSize.y * 0.25 );
			break;
		}
		case 12: {
			gl::drawStrokedCube( Vec3f::zero(), Vec3f( 10.0, 10.0, 10.0 ) );
			break;
		}
		case 13: {
			gl::drawCube( Vec3f::zero(), Vec3f( 10.0, 10.0, 10.0 ) );
			break;
		}
		case 14: {
			gl::drawColorCube( Vec3f::zero(), Vec3f( 10.0, 10.0, 10.0 ) );
			break;
		}
		case 15: {
			gl::enableWireframe();
			gl::drawSphere( Vec3f::zero(), 10.0, 25 );
			gl::disableWireframe();
			break;
		}
		case 16: {
			gl::drawSphere( Vec3f::zero(), 10.0, 25 );
			break;
		}
		case 17: {
			gl::enableWireframe();
			gl::pushMatrices();
			gl::translate( 0.0, -5.0, 0.0 );
			gl::drawCylinder( 5.0, 5.0, 10.0, 25, 25 );
			gl::popMatrices();
			gl::disableWireframe();
			break;
		}
		case 18: {
			gl::pushMatrices();
			gl::translate( 0.0, -5.0, 0.0 );
			gl::drawCylinder( 5.0, 5.0, 10.0, 25, 25 );
			gl::popMatrices();
			break;
		}
		case 19: {
			gl::enableWireframe();
			gl::drawTorus( 10.0, 2.0, 25, 25 );
			gl::disableWireframe();
			break;
		}
		case 20: {
			gl::drawTorus( 10.0, 2.0, 25, 25 );
			break;
		}
		default: { break; }
	}
	
	// Update index:
	if( !mPaused && getElapsedFrames() % 100 == 0 ) {
		mIndex++;
		if( mIndex > mMaxIndex ) {
			mIndex = mMinIndex;
		}
	}
}

CINDER_APP_NATIVE( CinderDrawApp, RendererGl )
