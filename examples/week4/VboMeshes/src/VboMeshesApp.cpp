//////////////////////////////////////////////////
/*          Art of Graphics Programming         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2014                   */
//////////////////////////////////////////////////

#include "cinder/app/AppNative.h"
#include "cinder/Camera.h"

#include "MeshFactory.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VboMeshesApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void update();
	void draw();
	void resize();
	
	CameraPersp mCam;
	
	ProtoMesh	mMeshProto;
	gl::VboMesh mMeshVbo;
};

void VboMeshesApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Initialize a sphere mesh:
	createSphere( 30, 30, 75.0, mMeshProto );
	
	// Convert mesh to VBO:
	createMeshVbo( mMeshProto, mMeshVbo );
}

void VboMeshesApp::mouseDown(MouseEvent event)
{
}

void VboMeshesApp::update()
{
}

void VboMeshesApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set matrices from camera:
	gl::setMatrices( mCam );
	
	// Set line width:
	glLineWidth( 1.0 );
	
	// Push matrix:
	gl::pushMatrices();
	
	// Rotate frame:
	gl::rotate( Vec3f( cos( getElapsedSeconds() ), cos( getElapsedSeconds() ), sin( getElapsedSeconds() ) ) * 25.0 );
	
	// Set color:
	gl::color( 1.0, 1.0, 1.0 );

	// Draw VBO mesh (filled):
	gl::draw( mMeshVbo );

	// Set color:
	gl::color( 1.0, 0.0, 0.0 );

	// Draw VBO mesh (wireframe):
	gl::enableWireframe();
	gl::draw( mMeshVbo );
	gl::disableWireframe();
	
	// Draw protomesh in debug mode:
	mMeshProto.drawDebug( 5.0 );
	
	// Pop matrix:
	gl::popMatrices();
}

void VboMeshesApp::resize()
{
	// Setup camera:
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	mCam.lookAt( Vec3f( 0.0, 0.0, 200.0 ), Vec3f::zero() );
}

CINDER_APP_NATIVE( VboMeshesApp, RendererGl )
