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

// See:
// http://stackoverflow.com/questions/15588860/what-exactly-are-eye-space-coordinates
// https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php
// http://en.wikibooks.org/wiki/GLSL_Programming/GLUT/Multiple_Lights

static const string kVertGlsl =
STRINGIFY(
		  varying vec3 vVertex;
		  varying vec3 vNormal;
		  
		  void main(void)
		  {
			  // In camera-space:
			  
			  // Get vertex position:
			  vVertex = vec3( gl_ModelViewMatrix * gl_Vertex );
			  // Get vertex normal:
			  vNormal = normalize( gl_NormalMatrix * gl_Normal );
			  // Set vertex position:
			  gl_Position = ftransform();
		  }
		  );

static const string kFragGlsl =
STRINGIFY(
		  uniform vec3 mLightPosition;
		  uniform vec4 mAmbient;
		  uniform vec4 mDiffuse;
		  uniform vec4 mSpecular;
		  uniform float mShininess;
		  
		  varying vec3 vVertex;
		  varying vec3 vNormal;
		  
		  void main (void)
		  {
			  // In camera-space:
			  
			  // Convert light position from world-space to camera-space:
			  vec4 tLightCamSpace = gl_ModelViewMatrix * vec4( mLightPosition, 1.0 );
			  // Compute light direction:
			  vec3 tLightDir = normalize( tLightCamSpace.xyz - vVertex );
			  // Compute camera vector:
			  vec3 tCamVec = normalize( -vVertex );
			  // Compute reflection direction for light vector:
			  vec3 tRefDir = normalize( -reflect( tLightDir, vNormal ) );
			  // Compute diffuse color:
			  vec4 tDiff = clamp( mDiffuse * max( dot( vNormal, tLightDir ), 0.0 ), 0.0, 1.0 );
			  // Compute specular color:
			  vec4 tSpec = clamp( mSpecular * pow( max( dot( tRefDir, tCamVec ), 0.0 ), 0.3 * mShininess ), 0.0, 1.0 );
			  // Compute final color:
			  gl_FragColor = mAmbient + tDiff + tSpec;
		  }
		  );

class GLSLPhongApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void update();
	void draw();
	void resize();
	
	CameraPersp		mCam;
	gl::GlslProg		mShader;
};

void GLSLPhongApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Load shader from strings (using the stringify macro):
	mShader = gl::GlslProg( kVertGlsl.c_str(), kFragGlsl.c_str() );
}

void GLSLPhongApp::mouseDown(MouseEvent event)
{
}

void GLSLPhongApp::update()
{
}

void GLSLPhongApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set matrix from camera:
	gl::setMatrices( mCam );
	
	// Set color:
	gl::color( 1.0, 1.0, 1.0 );
	
	// Set light position (in world-space):
	Vec3f tLightPos = Vec3f( 5.0*cos(getElapsedSeconds()), 0.0, 5.0*sin(getElapsedSeconds()) );
	
	// Bind shader:
	mShader.bind();
	
	// Set shader uniform parameters:
	mShader.uniform( "mLightPosition",	tLightPos );
	mShader.uniform( "mAmbient",			ColorA( 0.1, 0.1, 0.1, 1.0 ) );
	mShader.uniform( "mDiffuse",			ColorA( 0.2, 0.8, 0.4, 1.0 ) );
	mShader.uniform( "mSpecular",		ColorA( 0.1, 0.4, 0.2, 1.0 ) );
	mShader.uniform( "mShininess",		128.0f );
	
	// Draw sphere:
	gl::drawSphere( Vec3f::zero(), 1.0, 50 );
	
	// Unbind shader:
	mShader.unbind();
	
	// Draw light position (as a small sphere):
	gl::drawSphere( tLightPos, 0.1 );
}

void GLSLPhongApp::resize()
{
	// Setup camera:
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
	mCam.lookAt( Vec3f( 0.0, 0.0, 10.0 ), Vec3f::zero() );
}

CINDER_APP_NATIVE( GLSLPhongApp, RendererGl )
