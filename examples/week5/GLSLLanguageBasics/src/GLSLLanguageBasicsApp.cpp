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

using namespace ci;
using namespace ci::app;
using namespace std;

#define STRINGIFY(x) #x

static const string kVertGlsl =
STRINGIFY(
		  // The "vertex shader" handles the tranformation of a given vertex
		  // In this example, we're only manipulating pixel information,
		  // so our vertex shader just needs to perform a default transformation
		  // of the bound geometry. This is often called a "pass-through" shader
		  void main()
		  {
			  gl_FrontColor = gl_Color;
			  gl_TexCoord[0] = gl_MultiTexCoord0;
			  gl_Position = ftransform();
		  }
		  );

static const string kFragGlsl =
string( "#version 120\n" ) +
STRINGIFY(
		  // The "fragment shader" (sometimes called the "pixel shader")
		  // handles the coloring of a given vertex.
		  void main(void)
		  {
			  // Types:
			  float tFloat = 0.0;
			  vec2  tVec2  = vec2( 0.1, 0.2 );
			  vec3  tVec3  = vec3( 0.1, 0.2, 0.3 );
			  vec4  tVec4  = vec4( 0.1, 0.2, 0.3, 0.4 );
			  
			  // Uninitialized local variables:
			  vec3	tVec3a;
			  vec4	tVec4a;
			  
			  // Conversions (R-value):
			  tVec3a = vec3( tFloat );		// Equiv. to vec3( tFloat, tFloat, tFloat )
			  tVec3a = vec3( tVec2, 1.0 );  // Equiv. to vec3( tVec2.x, tVec2.y, 1.0 )
			  tVec3a = tVec3.zyx;			// Equiv. to vec3( tVec3.z, tVec3.y, tVec3.x )
			  tVec3a = tVec4.xyz;			// Equiv. to vec3( tVec4.x, tVec4.y, tVec4.z )
			  tVec3a = tVec4.xxy;			// Equiv. to vec3( tVec4.x, tVec4.x, tVec4.y )
			  tVec3a = tVec4.zzz;			// Equiv. to vec3( tVec4.z, tVec4.z, tVec4.z )
			  
			  // Conversions (L-value):
			  tVec3a.zyx = tVec3.xyz;
			  tVec3a.xz  = vec2( 0.1, 0.3 );
			  
			  // XYZW and RGBA are interchangeable swizzles:
			  tVec4a.xyzw = tVec4.rgba;
			  
			  // etc...
			  // See: https://www.opengl.org/wiki/Data_Type_(GLSL) for more type conversion examples
			  
			  // A shader is written from the perspective of the operations that
			  // will be performed on an individual vertex within the bound geomery.
			  // In other words, the GPU will run this program on each vertex in parallel.
			  // So, the question often arises within a shader of which vertex we're operating on.
			  // We can get the two-dimensional texture coordinate (in the range [0,1] for each axis)
			  // for this vertex by accessing: gl_TexCoord[0].xy
			  vec2 tCoord = gl_TexCoord[0].xy;
			  
			  // Ultimately, we want the frag shader to set the color of the current vertex.
			  // We do this by setting gl_FragColor from a vec4 (representing RGBA)
			  gl_FragColor = vec4( tCoord, 1.0, 1.0 );
		  }
		  );

class GLSLLanguageBasicsApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void update();
	void draw();
	
	gl::GlslProg mShader;
};

void GLSLLanguageBasicsApp::setup()
{
	// Load shader from strings (using the stringify macro):
	mShader = gl::GlslProg( kVertGlsl.c_str(), kFragGlsl.c_str() );
}

void GLSLLanguageBasicsApp::mouseDown(MouseEvent event)
{
}

void GLSLLanguageBasicsApp::update()
{
}

void GLSLLanguageBasicsApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set matrices from window size:
	gl::setMatricesWindow( getWindowSize() );
	
	// Bind shader:
	mShader.bind();
	
	// Draw a solid rectangle:
	// (The shader will be drawn onto the rect surface)
	gl::drawSolidRect( getWindowBounds() );
	
	// Unbind shader:
	mShader.unbind();
}

CINDER_APP_NATIVE( GLSLLanguageBasicsApp, RendererGl )
