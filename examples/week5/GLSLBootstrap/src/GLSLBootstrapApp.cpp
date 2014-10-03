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

#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define STRINGIFY(x) #x

class GLSLBootstrapApp : public AppNative {
  public:
	void setup();
	void mouseDown(MouseEvent event);
	void update();
	void draw();
	
	gl::GlslProg mShader;
};

void GLSLBootstrapApp::setup()
{
	// Load a simple shader from resources:
	
	/*
	mShader = gl::GlslProg( loadResource( RES_GLSL_BASIC_VERT ), loadResource( RES_GLSL_BASIC_FRAG ) );
	*/
	
	// Load a simple shader from strings:
	
	/*
	mShader = gl::GlslProg("void main() { gl_FrontColor = gl_Color; gl_TexCoord[0] = gl_MultiTexCoord0; gl_Position = ftransform(); }",
						   "void main(void) { gl_FragColor = vec4( gl_TexCoord[0].x, gl_TexCoord[0].y, 1.0, 1.0 ); }" );
	*/
	
	// Load a simple shader from strings (using the stringify macro):
	
	string tVert = STRINGIFY(
							 void main()
							 {
								 gl_FrontColor = gl_Color;
								 gl_TexCoord[0] = gl_MultiTexCoord0;
								 gl_Position = ftransform();
							 }
							 );
	
	string tFrag = STRINGIFY(
							 void main(void)
							 {
								 gl_FragColor = vec4( gl_TexCoord[0].x, gl_TexCoord[0].y, 1.0, 1.0 );
							 }
							 );
	
	mShader = gl::GlslProg( tVert.c_str(), tFrag.c_str() );
	
}

void GLSLBootstrapApp::mouseDown(MouseEvent event)
{
}

void GLSLBootstrapApp::update()
{
}

void GLSLBootstrapApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set matrices from window size:
	gl::setMatricesWindow( getWindowSize() );
	
	// Set color:
	gl::color( 1.0, 1.0, 1.0 );
	
	// Bind shader:
	mShader.bind();
	
	// Draw a solid rectangle:
	// (The shader will be drawn onto the rect surface)
	gl::drawSolidRect( getWindowBounds() );
	
	// Unbind shader:
	mShader.unbind();
}

CINDER_APP_NATIVE( GLSLBootstrapApp, RendererGl )
