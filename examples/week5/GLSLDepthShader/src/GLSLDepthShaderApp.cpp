//////////////////////////////////////////////////
/*          Art of Graphics Programming         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2014                   */
//////////////////////////////////////////////////

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define STRINGIFY(x) #x

static const string kVertGlsl =
STRINGIFY(
		  varying float vCameraDistance;
		  
		  void main()
		  {
			  // Get the vertex position in camera-space:
			  vec4 tVertexPositionCameraSpace = gl_ModelViewMatrix * gl_Vertex;
			  // Get camera's distance from vertex:
			  vCameraDistance = -tVertexPositionCameraSpace.z;
			  // Set vertex position:
			  gl_Position = gl_ProjectionMatrix * tVertexPositionCameraSpace;
		  }
		  );

static const string kFragGlsl =
STRINGIFY(
		  uniform vec2  mDistanceRange;
		  varying float vCameraDistance;
		  
		  float map_value(float value, float istart, float istop, float ostart, float ostop)
		  {
			  return ostart + ( ostop - ostart ) * ( ( value - istart ) / ( istop - istart ) );
		  }
		  
		  void main(void)
		  {
			  // Map specified distance range to unit range:
			  float tMappedDist = map_value( vCameraDistance, mDistanceRange.x, mDistanceRange.y, 0.0, 1.0 );
			  
			  // Set color based on mapped distance:
			  gl_FragColor = vec4( vec3( 1.0 - tMappedDist ), 1.0 );
		  }
		  );

class GLSLDepthShaderApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void update();
	void draw();
	void resize();
	
	CameraPersp		mCam;
	gl::GlslProg	mShader;
};

void GLSLDepthShaderApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Load shader from strings (using the stringify macro):
	mShader = gl::GlslProg( kVertGlsl.c_str(), kFragGlsl.c_str() );
}

void GLSLDepthShaderApp::mouseDown(MouseEvent event)
{
}

void GLSLDepthShaderApp::update()
{
}

void GLSLDepthShaderApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Update camera position:
	mCam.lookAt( Vec3f( cos( getElapsedSeconds() ), sin( getElapsedSeconds() ) + 2.0, sin( getElapsedSeconds() ) ) * 10.0, Vec3f::zero() );
	
	// Set matrix from camera:
	gl::setMatrices( mCam );
	
	// Choose unit size:
	float tUnitLen = 10.0;
	float tHalfLen = tUnitLen / 2.0;
	
	// Bind shader:
	mShader.bind();
	
	// Set shader uniform parameter for distance-mapping range:
	mShader.uniform( "mDistanceRange", Vec2f( 10.0, 40.0 ) );
	
	// Draw color cube grid:
	for(int x = -10; x < 10; x++) {
		for(int z = -10; z < 10; z++) {
			gl::drawCube( Vec3f( x * tUnitLen, 0.0, z * tUnitLen ), Vec3f( tHalfLen, tHalfLen, tHalfLen ) );
		}
	}
	
	// Unbind shader:
	mShader.unbind();
}

void GLSLDepthShaderApp::resize()
{
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
}

CINDER_APP_NATIVE( GLSLDepthShaderApp, RendererGl )
