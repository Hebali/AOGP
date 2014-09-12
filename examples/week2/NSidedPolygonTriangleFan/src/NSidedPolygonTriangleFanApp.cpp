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

class NSidedPolygonTriangleFanApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	int			mMinSides, mMaxSides, mNumSides;
	float		mRadius, mTheta;
	bool		mGoingUp;
	
	ci::Font	mFont;
};

void NSidedPolygonTriangleFanApp::setup()
{
	/*
	 * Triangle fans are an excellent way to create circles, cylinder caps, cones, parasols, etc.
	 * The geometry we will create below is not a single polygon, per se, but rather a strip of triangles
	 * organized around a center point. For OpenGL to properly compute the brightness of a polygon, all of the
	 * polygon's vertices must reside on the same plane. A triangle fan is composed of multiple polygons and
	 * by definition, the three vertices of each triangle will always be coplanar. Therefore,
	 * we can position a triangle fan's vertices in multiple planes without rendering errors.
	 */
	
	// Choose a radius for the circle in which the polygon is inscribed:
	mRadius = 200.0;
	
	// Choose a starting point on the inscribing circle:
	// http://en.wikipedia.org/wiki/File:Degree-Radian_Conversion.svg
	mTheta    = -M_PI_2;
	
	// Animation cycle settings:
	mMinSides = 3;
	mMaxSides = 50;
	mNumSides = mMinSides;
	mGoingUp = true;
	
	// Create font:
	mFont = ci::Font( "Helvetica", 24 );
}

void NSidedPolygonTriangleFanApp::mouseDown( MouseEvent event )
{
}

void NSidedPolygonTriangleFanApp::update()
{
}

void NSidedPolygonTriangleFanApp::draw()
{
	// Clear the window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Push matrix:
	gl::pushMatrices();
	
	// Translate to center of window:
	gl::translate( getWindowWidth() / 2.0, getWindowHeight() / 2.0, 0.0 );
	
	// Compute the angle between each point on the polygon:
	float deltaTheta = ( M_PI * 2.0 ) / mNumSides;
	
	// Set color:
	gl::color( 1, 0, 0 );
	// Set line width:
	gl::lineWidth( 2.0 );
	
	// Render polygon as wireframe on the way up:
	if( mGoingUp ) {
		gl::enableWireframe();
	}
	
	// Prepare to draw the polygon:
	glBegin(GL_TRIANGLE_FAN);
	// Add center point to triangle fan:
	glVertex2f( 0.0, 0.0 );
	// Add circumference points to triangle fan:
	for(int i = 0; i < mNumSides; i++) {
		// Compute the x and y position of each vertex:
		float x = mRadius * cos( i * deltaTheta + mTheta );
		float y = mRadius * sin( i * deltaTheta + mTheta );
		glVertex2f( x, y );
	}
	// To close the triangle fan, we need to return to the first circumference vertex
	// At that vertex, i = 0, so (i * deltaTheta + theta) reduces to (theta):
	glVertex2f( mRadius * cos( mTheta ), mRadius * sin( mTheta ) );
	// Close the polygon:
	glEnd();
	
	// Pop matrix:
	gl::popMatrices();
	
	// Reset polygon mode:
	gl::disableWireframe();
	
	// Draw text label:
	Vec2f textPos = Vec2f( getWindowWidth() / 2.0, 10.0 );
	gl::drawStringCentered( std::to_string( mNumSides ) + " sides", textPos, ColorA::white(), mFont );
	
	// Every 15th frame, change the polygon's side count:
	if( getElapsedFrames() % 15 == 0 ) {
		// When we reach the top or bottom side count, turn around:
		if(mNumSides == mMinSides) {
			mGoingUp = true;
		}
		else if(mNumSides == mMaxSides) {
			mGoingUp = false;
		}
		// Update the side count:
		if( mGoingUp ) {
			mNumSides++;
		}
		else {
			mNumSides--;
		}
	}
}

CINDER_APP_NATIVE( NSidedPolygonTriangleFanApp, RendererGl )
