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

class TriangleStripMeshCylinderApp : public AppNative {
public:
	void prepareSettings(Settings* settings);
	void setup();
	void mouseDown( MouseEvent event );
	void resize();
	void update();
	void draw();
	
	void reset();
	
	std::vector<ci::Vec3f>	mVertices;
	
	float					mProfileRadius, mCylinderLen;
	int						mSubdivisionsU, mSubdivisionsV;
	
	float					mDeltaThetaU;
	int						mCurrU, mCurrV;
	bool					mGoRight, mStepA, mReachedEnd;
	float					mCamProfileRadiusMult;
	
	ci::Font				mFont;
	
	ci::CameraPersp			mCam;
	ci::Vec3f				mCamEye;
};

void TriangleStripMeshCylinderApp::prepareSettings(Settings* settings)
{
    settings->setWindowSize( 800, 600 );
}

void TriangleStripMeshCylinderApp::reset()
{
	// Reset construction state variables:
	mReachedEnd = false;
	mGoRight    = true;
	mStepA      = true;
	mCurrU      = 0;
	mCurrV      = 0;
	// Compute the angle between each vertex in a profile:
	mDeltaThetaU = ( M_PI * 2.0 ) / mSubdivisionsU;
	// Set initial camera position:
	mCamEye = Vec3f( mProfileRadius * mCamProfileRadiusMult, 0.0, 0.0 );
	// Clear the container:
	mVertices.clear();
}

void TriangleStripMeshCylinderApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Set the radius of the cylinder:
	mProfileRadius = 100.0;
	// Set the length of the cylinder:
	mCylinderLen   = 200.0;
	
	// Set the number of subdivisions around each circular profile:
	mSubdivisionsU  = 40;
	// Set the number of profiles in the cylinder:
	mSubdivisionsV  = 10;
	
	// Set the camera's radius multiplier:
	mCamProfileRadiusMult = 3.0;
	
	// Initialize construction variables:
	reset();
}

void TriangleStripMeshCylinderApp::mouseDown( MouseEvent event )
{
}

void TriangleStripMeshCylinderApp::resize()
{
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	gl::setMatrices( mCam );
}

void TriangleStripMeshCylinderApp::update()
{
}

void TriangleStripMeshCylinderApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set matrices from camera:
	mCam.lookAt( Vec3f( mCamEye.x, mCamEye.y + 200.0, mCamEye.z ), Vec3f( 0.0, mCamEye.y, 0.0 ) );
	gl::setMatrices( mCam );
	
	// Push matrix:
	gl::pushMatrices();
	
	// Set color:
	gl::color( 1.0, 0.0, 0.0 );
	
	// Get vertex count:
	size_t tVertCount = mVertices.size();
	
	// Set line width:
	gl::lineWidth( 2 );
	
	// Draw mesh (in fill mode):
	{
		// Draw triangle strip:
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < tVertCount; i++) {
			const Vec3f& currVert = mVertices.at(i);
			glVertex3f( currVert.x, currVert.y, currVert.z );
		}
		glEnd();
	}
	
	// Render polygon as wireframe:
	gl::enableWireframe();
	
	// Set color:
	gl::color( 1.0, 1.0, 1.0 );
	
	// Draw mesh (in wireframe mode):
	{
		// Draw triangle strip:
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < tVertCount; i++) {
			const Vec3f& currVert = mVertices.at(i);
			glVertex3f( currVert.x, currVert.y, currVert.z );
		}
		glEnd();
	}
	
	// Draw origin:
	gl::drawCoordinateFrame( mProfileRadius );
	
	// Reset polygon mode:
	gl::disableWireframe();
	
	// Pop matrix:
	gl::popMatrices();
	
	// If we've reached the end of our mesh, as determined below, reset program.
	if( mReachedEnd ) {
		reset();
	}
	
	// To create a 3D cylinder, we will associate the U axis of the 2D mesh with the circumference of
	// each of the cylinder's circular profiles and associate the V axis with the cylinder's length.
	// We could easily turn this into a cone by varying the radius of each profile in accordance with our
	// current position along the V axis.
	
	// We find our position along the circumference of the current profile by multiplying the angle of a
	// single step in the circular profile by the current index in the U axis (radial axis).
	float thetaU = mDeltaThetaU * mCurrU;
	
	// Using thetaU, we find the coordinates of the current point in relation to the center of the current profile
	// and then multiply these coordinates by the profile radius to size it appropriately.
	// Here we are thinking of the current profile as a circle inscribed in a plane spanning the X and Z axes.
	float x = mProfileRadius * cos( thetaU );
	float z = mProfileRadius * sin( thetaU );
	
	// We find the y-value of the current vertex by computing the length of one cylinder segment
	// and then multiplying this by current index in the V axis (non-radial axis) of our mesh.
	float y = ( mCylinderLen / mSubdivisionsV ) * mCurrV;
	
	// Set camera position:
	mCamEye.x = x * mCamProfileRadiusMult;
	mCamEye.z = z * mCamProfileRadiusMult;
	if(y > mCamEye.y) {
		mCamEye.y = y;
	}
	
	// Add the current vertex to the triangle strip container
	// Notice, we are not sharing vertices here. We'll need to address this issue in a later implementation!
	Vec3f currVert = Vec3f( x, y, z );
	mVertices.push_back( currVert );
	
	// Handle row endings when necessary:
	// We need to determine whether we are at the last column in the row given the direction we're traveling.
	// There are two V values that will be enumerated for the final currU, we want the second of these value, step B.
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

CINDER_APP_NATIVE( TriangleStripMeshCylinderApp, RendererGl )
