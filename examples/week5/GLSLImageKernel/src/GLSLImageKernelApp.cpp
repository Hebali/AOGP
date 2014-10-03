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
		  uniform float		mWidth;
		  uniform float		mHeight;
		  uniform float		mKernel[9];
		  void main()
		  {
			  // Get pixel dimension in [0,1] range per axis:
			  float tStepX = 1.0 / mWidth;
			  float tStepY = 1.0 / mHeight;
			  
			  // Compute neighbor offsets:
			  vec2 tOffsets[ 9 ];
			  tOffsets[ 0 ] = vec2( -tStepX, -tStepY );
			  tOffsets[ 1 ] = vec2(     0.0, -tStepY );
			  tOffsets[ 2 ] = vec2(  tStepX, -tStepY );
			  tOffsets[ 3 ] = vec2( -tStepX,     0.0 );
			  tOffsets[ 4 ] = vec2(     0.0,     0.0 );
			  tOffsets[ 5 ] = vec2(  tStepX,     0.0 );
			  tOffsets[ 6 ] = vec2( -tStepX,  tStepY );
			  tOffsets[ 7 ] = vec2(     0.0,  tStepY );
			  tOffsets[ 8 ] = vec2(  tStepX,  tStepY );
			  
			  // Initialize kernel sum:
			  vec4 tSum = vec4( 0.0 );
			  
			  // Iterate over kernel and sum components:
			  for(int i = 0; i < 9; i++) {
				  tSum += texture2D( mTexture, gl_TexCoord[0].xy + tOffsets[ i ] ) * mKernel[ i ];
			  }
			
			  // Set final color:
			  gl_FragColor = tSum;
		  }
		  );

// Prepare a few sample filter kernels:

// No Filter:
static const float kNoKernel[] =
{
	0.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 0.0
};

// Gaussian Filter:
static const float kGaussianKernel[] =
{
	1.0/16.0, 2.0/16.0, 1.0/16.0,
	2.0/16.0, 4.0/16.0, 2.0/16.0,
	1.0/16.0, 2.0/16.0, 1.0/16.0,
};

// Sharpen Filter:
static const float kSharpenKernel[] =
{
	 0.0, -1.0,  0.0,
	-1.0,  5.0, -1.0,
	 0.0, -1.0,  0.0,
};

// Laplacian Edge Detection Filter:
static const float kLaplacianKernel[] =
{
	 0.0,  1.0,  0.0,
	 1.0, -4.0,  1.0,
	 0.0,  1.0,  0.0
};

// Emboss Filter:
static const float kEmbossKernel[] =
{
	-2.0, -1.0,  0.0,
	-1.0,  1.0,  1.0,
	 0.0,  1.0,  2.0
};


class GLSLImageKernelApp : public AppNative {
public:
	void setup();
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	CaptureRef			mCapture;
	gl::TextureRef		mTexture;
	gl::GlslProg		mShader;
	size_t				mFilterIdx;
};

void GLSLImageKernelApp::setup()
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
	
	// Prepare initial state:
	mFilterIdx = 0;
}

void GLSLImageKernelApp::keyUp(KeyEvent event)
{
	char c = event.getChar();
	if( c >= '0' && c <= '9' ) {
		mFilterIdx = (int)c - 48;
	}
}

void GLSLImageKernelApp::update()
{
	if( mCapture && mCapture->checkNewFrame() ) {
		mTexture = gl::Texture::create( mCapture->getSurface() );
	}
}

void GLSLImageKernelApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set matrices from window size:
	gl::setMatricesWindow( getWindowSize() );
	
	// Set color:
	gl::color( 1.0, 1.0, 1.0 );
	
	// Check whether texture has been initialized:
	if( mTexture ) {
		// Bind texture:
		mTexture->bind( 0 );
		
		// Bind shader:
		mShader.bind();
		
		// Set shader uniform for texture id:
		mShader.uniform( "mTexture", 0 );
		
		// Pass texture dimensions to shader:
		mShader.uniform( "mWidth", static_cast<float>( mTexture->getWidth() ) );
		mShader.uniform( "mHeight", static_cast<float>( mTexture->getHeight() ) );
		
		// Pass filter kernel array:
		switch ( mFilterIdx ) {
			case 1: {
				mShader.uniform( "mKernel", kGaussianKernel, 9 );
				break;
			}
			case 2: {
				mShader.uniform( "mKernel", kSharpenKernel, 9 );
				break;
			}
			case 3: {
				mShader.uniform( "mKernel", kEmbossKernel, 9 );
				break;
			}
			case 4: {
				mShader.uniform( "mKernel", kLaplacianKernel, 9 );
				break;
			}
			default : {
				mShader.uniform( "mKernel", kNoKernel, 9 );
				break;
			}
		}
		
		// See:
		// http://en.wikipedia.org/wiki/Kernel_(image_processing)
		// http://www.ozone3d.net/tutorials/image_filtering.php
		// http://matlabtricks.com/post-5/3x3-convolution-kernels-with-online-demo
		
		// Draw a solid rectangle:
		// (The shader will be drawn onto the rect surface)
		gl::drawSolidRect( getWindowBounds() );
		
		// Unbind shader:
		mShader.unbind();
		
		// Unbind texture:
		mTexture->unbind();
	}
}

CINDER_APP_NATIVE( GLSLImageKernelApp, RendererGl )
