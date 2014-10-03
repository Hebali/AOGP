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

#include "MeshFactory.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define STRINGIFY(x) #x

static const string kVertGlsl =
STRINGIFY(
		  uniform float mTime;
		  uniform float mAmplitude;
		  uniform float mFrequency;
		  uniform vec3	mNormal;
		  
		  varying float vCameraDistance;
		  
		  const float PI = 3.1415926535897932384626433832795;
		  
		  void main()
		  {
			  // Get vertex position:
			  vec4 tVertPos = gl_Vertex;
			  // Compute ripple and translate vertex:
			  tVertPos.xyz += mNormal * mAmplitude * sin( -PI * length( tVertPos.xyz ) * mFrequency + mTime );
			  // Set vertex position:
			  gl_Position = gl_ModelViewProjectionMatrix * tVertPos;
			  // Get camera's distance from vertex:
			  vCameraDistance = gl_Position.w;
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
	
	ProtoMesh		mMeshProto;
	gl::VboMesh		mMeshVbo;
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
	
	// Initialize a mesh:
	createPlane( 1000, 1000, 20.0, mMeshProto );
	// Convert mesh to VBO:
	createMeshVbo( mMeshProto, mMeshVbo );
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
	
	// Set matrix from camera:
	gl::setMatrices( mCam );
	
	// Bind shader:
	mShader.bind();
	
	// Set shader uniform parameters:
	mShader.uniform( "mTime",			static_cast<float>( getElapsedSeconds() ) );
	mShader.uniform( "mAmplitude",		0.4f );
	mShader.uniform( "mFrequency",		1.0f );
	mShader.uniform( "mNormal",			Vec3f::yAxis() );
	mShader.uniform( "mDistanceRange",	Vec2f( 0.0, 30.0 ) );
	
	// Draw VBO mesh:
	gl::draw( mMeshVbo );

	// Notice that we're drawing a flat mesh (all y-values are 0.0).
	// The ripple displacement is happening within the vertex shader.
	
	// Unbind shader:
	mShader.unbind();
}

void GLSLDepthShaderApp::resize()
{
	// Setup camera:
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	mCam.lookAt( Vec3f( 12.0, 3.0, 12.0 ), Vec3f::zero() );
}

CINDER_APP_NATIVE( GLSLDepthShaderApp, RendererGl )
