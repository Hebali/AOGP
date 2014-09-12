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

class NSidedPolygonApp : public AppNative {
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

void NSidedPolygonApp::setup()
{
	/*
	 * In this example, we create an n-sided polygon where n cycles between 3 and 50. The fifty-sided figure is almost
	 * indistinguishable from a "perfect circle," making this method a candidate for implementing circles, cylinder caps, etc.
	 * This method could also be useful in drawing tessellations (see http://mathworld.wolfram.com/Tessellation.html).
	 * However, the figures generated here could also be created with triangles, a triangle strip or a triangle fan.
	 * Please note that GL_POLYGON is not particularly efficient and is only being used for demonstration purposes.
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

void NSidedPolygonApp::mouseDown( MouseEvent event )
{
}

void NSidedPolygonApp::update()
{
}

void NSidedPolygonApp::draw()
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
	// Prepare to draw the polygon:
	glBegin(GL_POLYGON);
	// Iterate over each vertex:
	for(int i = 0; i < mNumSides; i++) {
		// Compute the x and y position of each vertex:
		float x = mRadius * cos( i * deltaTheta + mTheta );
		float y = mRadius * sin( i * deltaTheta + mTheta );
		glVertex2f( x, y );
	}
	// Close the polygon:
	glEnd();
	
	// Set color:
	gl::color( 0, 0, 1 );
	// Set line width:
	gl::lineWidth( 2.0 );
	// Prepare to draw the line loop:
	glBegin(GL_LINE_LOOP);
	// Iterate over each vertex:
	for(int i = 0; i < mNumSides; i++) {
		// Compute the x and y position of each vertex:
		float x = mRadius * cos( i * deltaTheta + mTheta );
		float y = mRadius * sin( i * deltaTheta + mTheta );
		glVertex2f( x, y );
	}
	// Close the loop:
	glEnd();
	
	// Pop matrix:
	gl::popMatrices();
	
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

CINDER_APP_NATIVE( NSidedPolygonApp, RendererGl )
