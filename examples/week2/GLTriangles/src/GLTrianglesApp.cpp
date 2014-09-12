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

class GLTrianglesApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	void resetVertices();
	
	bool					mWireframeMode;
	std::vector<ci::Vec2f>	mTriangles;
	std::vector<ci::Vec2f>	mTriangleStrip;
	
	bool					mBestIsA;
	int						mBestIdx;
	
	Vec2f					mMouseAdjustedPos;
};

void GLTrianglesApp::resetVertices()
{
	// Set initial vertex positions:
	Vec2f tVertices[8];
	tVertices[0] = Vec2f( 0.0,  0.0 );
	tVertices[1] = Vec2f( 0.0,  0.1 );
	tVertices[2] = Vec2f( 0.1,  0.0 );
	tVertices[3] = Vec2f( 0.1,  0.1 );
	tVertices[4] = Vec2f( 0.2,  0.0 );
	tVertices[5] = Vec2f( 0.2,  0.1 );
	tVertices[6] = Vec2f( 0.3,  0.0 );
	tVertices[7] = Vec2f( 0.3,  0.1 );
	
	// Copy vertices into triangle strip (while also translating to upper portion of window):
	mTriangleStrip.resize( 8 );
	for(size_t i = 0; i < 8; i++) {
		mTriangleStrip[ i ] = tVertices[ i ] + Vec2f( 0.35, 0.2 );
	}
	
	// Copy vertices into triangles (while also translating to lower portion of window):
	mTriangles.resize( 6 * 3 );
	for(size_t i = 0; i < 6; i++) {
		for(size_t j = 0; j < 3; j++) {
			mTriangles[ i * 3 + j ] = tVertices[ i + j ] + Vec2f( 0.35, 0.7 );
		}
	}
}

void GLTrianglesApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Set initial vertex positions:
	resetVertices();
	
	// Set initial wireframe mode:
	mWireframeMode = true;
	
	// Set initial picker index:
	mBestIdx = -1;
}

void GLTrianglesApp::mouseDown(MouseEvent event)
{
	// Compute normalized mouse position:
	mMouseAdjustedPos = static_cast<Vec2f>( event.getPos() ) / static_cast<Vec2f>( getWindowSize() );
	
	// Compute mouse distance from triangle strip vertices:
	size_t tCountA    = mTriangleStrip.size();
	float  tBestDistA = 1e6;
	int    tBestA     = -1;
	//
	for(size_t i = 0; i < tCountA; i++) {
		float tDistA = mMouseAdjustedPos.distance( mTriangleStrip[ i ] );
		if( tDistA < tBestDistA ) {
			tBestDistA = tDistA;
			tBestA = i;
		}
	}
	
	// Compute mouse distance from triangle vertices:
	size_t tCountB    = mTriangles.size();
	float  tBestDistB = 1e6;
	int    tBestB     = -1;
	//
	for(size_t i = 0; i < tCountB; i++) {
		float tDistB = mMouseAdjustedPos.distance( mTriangles[ i ] );
		if( tDistB < tBestDistB ) {
			tBestDistB = tDistB;
			tBestB = i;
		}
	}
	
	// Check whether mouse is closer to triangle strip or triangles
	// and set member flag/index accordingly:
	if( tBestDistA < tBestDistB ) {
		mBestIsA = true;
		mBestIdx = tBestA;
	}
	else {
		mBestIsA = false;
		mBestIdx = tBestB;
	}
}

void GLTrianglesApp::mouseDrag(MouseEvent event)
{
	// Compute normalized mouse position:
	Vec2f tAdjustedPos = static_cast<Vec2f>( event.getPos() ) / static_cast<Vec2f>( getWindowSize() );
	
	// Compute mouse delta:
	Vec2f tDelta = tAdjustedPos - mMouseAdjustedPos;
	
	// Make sure that we have a best index:
	if( mBestIdx >= 0 ) {
		// Handle triangle strip vertex delta:
		if( mBestIsA ) { mTriangleStrip[ mBestIdx ] += tDelta; }
	
		// Handle triangle vertex delta:
		else { mTriangles[ mBestIdx ] += tDelta; }
	}
	
	// Update normalized mouse position:
	mMouseAdjustedPos = tAdjustedPos;
}

void GLTrianglesApp::keyUp(KeyEvent event)
{
	switch( event.getChar() ) {
		case 'r': {
			resetVertices();
			break;
		}
		case 'w': {
			mWireframeMode = !mWireframeMode;
			break;
		}
		default: { break; }
	}
}

void GLTrianglesApp::update()
{
}

void GLTrianglesApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set matrices window from unit size:
	gl::setMatricesWindow( Vec2i( 1, 1 ) );
	
	// Set point display size:
	glPointSize( 5.0 );
	
	if( mWireframeMode ) { gl::enableWireframe(); }
	else { gl::disableWireframe(); }
	
	{
		// Get triangle strip vertex count:
		size_t tVertCount = mTriangleStrip.size();
		
		// Set color:
		gl::color( Color( 1, 0, 0 ) );
		
		// Begin:
		glBegin(GL_TRIANGLE_STRIP);
		
		// Iterate over and draw each vertex:
		for(size_t i = 0; i < tVertCount; i++) {
			glVertex2f( mTriangleStrip[i].x, mTriangleStrip[i].y );
		}
		
		// End:
		glEnd();
	}

	{
		// Get triangles vertex count:
		size_t tVertCount = mTriangles.size();
		
		// Set color:
		gl::color( Color( 0, 0, 1 ) );

		// Begin:
		glBegin(GL_TRIANGLES);
		
		// Iterate over and draw each vertex:
		for(size_t i = 0; i < tVertCount; i++) {
			glVertex2f( mTriangles[i].x, mTriangles[i].y );
		}
		
		// End:
		glEnd();
	}
}

CINDER_APP_NATIVE( GLTrianglesApp, RendererGl )
