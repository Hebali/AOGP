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

class TriangleStripMeshApp : public AppNative {
public:
	void prepareSettings(Settings* settings);
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
	
	void reset();
	
	std::vector<ci::Vec3f>	mVertices;
	
	float					mSubdivLengthU, mSubdivLengthV;
	int						mSubdivisionsU, mSubdivisionsV;
	
	int						mCurrU, mCurrV;
	bool					mGoRight, mStepA, mReachedEnd;
	
	ci::Font				mFont;
};

void TriangleStripMeshApp::prepareSettings(Settings* settings)
{
    settings->setWindowSize( 800, 800 );
}

void TriangleStripMeshApp::reset()
{
	// Reset construction state variables:
	mReachedEnd = false;
	mGoRight    = true;
	mStepA      = true;
	mCurrU      = 0;
	mCurrV      = 0;
	// Clear the container:
	mVertices.clear();
}

void TriangleStripMeshApp::setup()
{
	// Set the length of a segment in each axis:
	mSubdivLengthU = 100.0;
	mSubdivLengthV = 100.0;
	
	// Set the number of subdivisions per axis:
	mSubdivisionsU = 5;
	mSubdivisionsV = 5;
	
	// Initialize construction variables:
	reset();
	
	// Create font:
	mFont = ci::Font( "Helvetica", 24 );
}

void TriangleStripMeshApp::mouseDown( MouseEvent event )
{
}

void TriangleStripMeshApp::update()
{
}

void TriangleStripMeshApp::draw()
{
	// Clear the window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Push matrix:
	gl::pushMatrices();
	
	// Translate to center of screen:
	gl::translate(( getWindowWidth()  - mSubdivLengthU * mSubdivisionsU ) / 2.0,
				  ( getWindowHeight() - mSubdivLengthV * mSubdivisionsV ) / 2.0 );
	
	// Render polygon as wireframe:
	gl::enableWireframe();
	
	// Draw mesh:
	{
		// Get vertex count:
		size_t vertCount = mVertices.size();
		// Set point size:
		glPointSize( 5 );
		// Set line width:
		gl::lineWidth( 1 );
		// Draw vertices:
		glBegin(GL_POINTS);
		for(int i = 0; i < vertCount; i++) {
			const Vec3f& currVert = mVertices.at(i);
			glVertex3f( currVert.x, currVert.y, currVert.z );
		}
		glEnd();
		// Draw triangle strip:
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < vertCount; i++) {
			const Vec3f& currVert = mVertices.at(i);
			glVertex3f( currVert.x, currVert.y, currVert.z );
		}
		glEnd();
	}
	
	// Pop matrix:
	gl::popMatrices();
	
	// Reset polygon mode:
	gl::disableWireframe();
	
	// Draw the info text label
	string textLabel = ("Vertex Index: " + to_string(mCurrV*mSubdivisionsU + mCurrU)
						+ "\t\t Coordinate: (" + to_string( mCurrU ) + "," + to_string( mCurrV ) + ")\t\t Direction: "
						+ (mGoRight?"Right":"Left") + "\t\t Step: " + (mStepA?"A":"B") );
	
	// Draw text label:
	Vec2f textPos = Vec2f( getWindowWidth() / 2.0, 30.0 );
	gl::drawStringCentered( textLabel, textPos, ColorA::white(), mFont );
	
	// Update animation
	if(getElapsedFrames() % 30 == 0) {
		// If we've reached the end of our mesh, as determined below, reset program.
		if( mReachedEnd ) {
			reset();
		}
		
		// Based on the construction technique described at:
		// http://dan.lecocq.us/wordpress/2009/12/25/triangle-strip-for-grids-a-construction
		
		// Triangle strips are, as the name suggests, a continuous strip of triangles. Each triangle
		// in the strip must pick up where the last one left off. In examples 003A-003C, we iterate
		// over each column in a given row, moving from left to right. At the end of each row,
		// we go to the first column of the next row. This method will not work for a triangle strip
		// because the end of a row does not leave off where the next row begins. If we modify this
		// pattern so that at the end of the first row, we move to the last column of the next row
		// and then iterate columns backwards, the strip will remain continuous. Then, at the end of
		// the second row, we move to the third row and again iterate the columns in a forward direction.
		// That is, in even numbered rows, we iterate columns in rightward manner and in odd
		// numbered rows, we iterate leftward. Within each row, we need to create two triangles
		// for every rectangular subdivision of the grid. This is achieved with a repeated two step
		// process. In stepA, we move forward by one row increment. In stepB (aka !stepA),
		// we move back by one row increment and move forward or back by one column depending on whether
		// the current row is a rightward or leftward one.
		
		// For rightward rows (goingRight):
		//   A  C
		//   | /
		//   B
		
		// For leftward rows (!goingRight):
		//   C  A
		//    \ |
		//      B
		
		// In stepA, we go from A to B. In stepB, we go from B to C.
		// Then, vertex C becomes the "A" in the next iteration of the pattern and we repeat until the end of the row.
		
		// When we reach the end of a row, we need to pivot back in the opposite direction.
		// To do this, we add the last vertex of the previous row twice. These "degenerate triangles"
		// will act as a pivot, reversing the direction in which the strip is traveling and correcting
		// the normal orientation for the next row. (We'll look at normals more closely in a later example)
		
		// Add the current vertex to the triangle strip container
		// Notice, we are not sharing vertices here. We'll need to address this issue in a later implementation! (See note above)
		Vec3f currVert = Vec3f( mSubdivLengthU * mCurrU, mSubdivLengthV * mCurrV, 0.0 );
		mVertices.push_back( currVert );
		
		// Handle row endings when necessary:
		// We need to determine whether we are at the last column in the row given the direction we're traveling.
		// There are two Y values that will be enumerated for the final currU, we want the second of these value, step B.
		if( !mStepA && ( (mGoRight && mCurrU == mSubdivisionsU) || (!mGoRight && mCurrU == 0) ) ) {
			// When turning around, we add the last point again as a pivot and reverse normal.
			mVertices.push_back( currVert );
			mVertices.push_back( currVert );
			// Reset to step type A
			mStepA   = true;
			// Alternate between row types
			mGoRight = !mGoRight;
			// If we are at the end of a row on a step B move in the final column,
			// then we've added the last vertex in the mesh.
			mReachedEnd = (mCurrV == mSubdivisionsV);
		}
		
		// In step A, we always move forward one row (V axis).
		// In step B, we always move back a row.
		// In step B, we also move one column (U axis) forward for a rightward row or one column back for a leftward row.
		if( mGoRight ) {
			if(mStepA) {
				mCurrV++;
			}
			else {
				mCurrU++;
				mCurrV--;
			}
		}
		else {
			if(mStepA) {
				mCurrV++;
			}
			else {
				mCurrU--;
				mCurrV--;
			}
		}
		// Alternate between step types
		mStepA = !mStepA;
	}
}

CINDER_APP_NATIVE( TriangleStripMeshApp, RendererGl )
