//////////////////////////////////////////////////
/*          Art of Graphics Programming         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2014                   */
//////////////////////////////////////////////////

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Capture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define STRINGIFY(x) #x

static const string kVertGlsl =
STRINGIFY(
		  // Pass-through vertex shader:
		  void main()
		  {
			  gl_FrontColor = gl_Color;
			  gl_TexCoord[0] = gl_MultiTexCoord0;
			  gl_Position = ftransform();
		  }
		  );

static const string kFragGlsl =
STRINGIFY(
		  uniform sampler2D mTexture;
		  
		  void main()
		  {
			  // Get texture coordinate:
			  vec2 tCoord  = gl_TexCoord[0].xy;
			  // Get color value for current texture coordinate:
			  vec4 tColor  = texture2D( mTexture, tCoord );
			  // Flip color:
			  tColor.rgb   = vec3( 1.0 ) - tColor.rgb;
			  // Set final color:
			  gl_FragColor = tColor;
		  }
		  );

class GLSLImageFilterApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void update();
	void draw();
	
	CaptureRef			mCapture;
	gl::TextureRef		mTexture;
	gl::GlslProg		mShader;
};

void GLSLImageFilterApp::setup()
{
	// Load shader from strings (using the stringify macro):
	mShader = gl::GlslProg( kVertGlsl.c_str(), kFragGlsl.c_str() );
	
	// Try to load camera capture:
	try {
		mCapture = Capture::create( 640, 480 );
		mCapture->start();
	}
	catch(...) {
		cout << "Failed to initialize capture" << endl;
	}
}

void GLSLImageFilterApp::mouseDown(MouseEvent event)
{
}

void GLSLImageFilterApp::update()
{
	if( mCapture && mCapture->checkNewFrame() ) {
		mTexture = gl::Texture::create( mCapture->getSurface() );
	}
}

void GLSLImageFilterApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set matrices from window size:
	gl::setMatricesWindow( getWindowSize() );
	
	// Set color:
	gl::color( 1.0, 1.0, 1.0 );
	
	// Check whether texture has been initialized:
	if( mTexture ) {
		// Draw texture (using traditional approach):
		//gl::draw( mTexture, getWindowBounds() );
		
		// Bind texture:
		mTexture->bind( 0 );
		
		// Bind shader:
		mShader.bind();
		
		// Set shader uniform for texture id:
		mShader.uniform( "mTexture", 0 );
		
		// Draw a solid rectangle:
		// (The shader will be drawn onto the rect surface)
		gl::drawSolidRect( getWindowBounds() );
		
		// Unbind shader:
		mShader.unbind();
		
		// Unbind texture:
		mTexture->unbind();
	}
}

CINDER_APP_NATIVE( GLSLImageFilterApp, RendererGl )
