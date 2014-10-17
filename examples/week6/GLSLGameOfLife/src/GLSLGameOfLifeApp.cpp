//////////////////////////////////////////////////
/*          Art of Graphics Programming         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2014                   */
//////////////////////////////////////////////////

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "cinder/Perlin.h"

#define STRINGIFY(s) #s

using namespace ci;
using namespace ci::app;
using namespace std;

static const string kVertGlsl =
STRINGIFY(
		  void main()
		  {
			  gl_FrontColor  = gl_Color;
			  gl_TexCoord[0] = gl_MultiTexCoord0;
			  gl_Position    = ftransform();
		  }
		  );

static const string kFragGlsl =
STRINGIFY(
		  uniform float       mWidth;
		  uniform float       mHeight;
		  uniform sampler2D   mTexture;
		  
		  void main(void) {
			  // Get current position within rect:
			  vec2 texCoord	= gl_TexCoord[0].xy;
			  
			  // Determine the ratio dimension of a single pixel:
			  float w			= 1.0 / mWidth;
			  float h			= 1.0 / mHeight;
			  
			  // Get the value of the current pixel:
			  // (Since GOL uses binary states black/white, we only need one color channel)
			  float texColor 	= texture2D( mTexture, texCoord ).r;
			  
			  // Get neighbor positions:
			  vec2 offset[8];
			  offset[0] = vec2(  -w,  -h );
			  offset[1] = vec2( 0.0,  -h );
			  offset[2] = vec2(   w,  -h );
			  offset[3] = vec2(  -w, 0.0 );
			  offset[4] = vec2(   w, 0.0 );
			  offset[5] = vec2(  -w,   h );
			  offset[6] = vec2( 0.0,   h );
			  offset[7] = vec2(   w,   h );
			  
			  // Sum the active neighbors:
			  int sum = 0;
			  for(int i = 0; i < 8; i++) {
				  if( texture2D( mTexture, texCoord + offset[i] ).r > 0.5 ) { sum++; }
			  }
			  
			  // Determine pixel value based on the GOL rules:
			  float outVal = 0.0;
			  if     ( ( texColor >= 0.5 ) && (sum == 2 || sum == 3) ) { outVal = 1.0; }
			  else if( ( texColor <  0.5 ) && (sum == 3) )             { outVal = 1.0; }
			  
			  // Set final pixel value:
			  gl_FragColor = vec4( outVal, outVal, outVal, 1.0 );
		  }
		  );

class GLSLGameOfLifeApp : public AppNative {
  public:
	void prepareSettings(Settings *settings);
	void setup();
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	void reset();
	
	int					mCurrentFBO;
	int					mOtherFBO;
	ci::gl::Fbo			mFBOs[2];
	ci::gl::GlslProg    mShader;
	ci::gl::Texture		mTexture;
	ci::Vec2i			mDimension;
};

void GLSLGameOfLifeApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize( 1920, 1080 );
	settings->setFrameRate( 60.0f );
}

void GLSLGameOfLifeApp::reset()
{
	// Allocate a pixel array to store the initial board state:
	unsigned char* tData = new unsigned char[ mDimension.x * mDimension.y ];
	
	// Generate the initial board state using Perlin noise:
	Perlin tPerlin = Perlin( 8, rand() );
	float tOffset  = 0.0;
	float tIncr    = 0.01;
	for(int x = 0; x < mDimension.x; x++) {
		tOffset += tIncr;
		float yOffset = 0.0;
		for(int y = 0; y < mDimension.y; y++) {
			yOffset += tIncr;
			tData[ y * mDimension.x + x ] = ( ( tPerlin.fBm( tOffset, yOffset ) + 0.5 >= 0.5 ) ? 255 : 0 );
		}
	}
	
	// Create a b&w (luminence) texture from pixel array:
	mTexture = gl::Texture( tData, GL_LUMINANCE, mDimension.x, mDimension.y );
	
	// Free pixel array memory:
	delete [] tData;
	
	// Set viewport and matrices based on framebuffer dimension:
	gl::setMatricesWindow( mFBOs[0].getSize(), false );
	gl::setViewport( mFBOs[0].getBounds() );
	
	// Load the texture into both framebuffers:
	mTexture.bind( 0 );
	for(int i = 0; i < 2; i++) {
		mFBOs[i].bindFramebuffer();
		gl::draw( mTexture, mFBOs[i].getBounds() );
		mFBOs[i].unbindFramebuffer();
	}
	mTexture.unbind();
}

void GLSLGameOfLifeApp::setup()
{
	// Seed random number generator:
	srand( (unsigned)time( NULL ) );
	
	// Prepare FBO dimensions:
	mDimension = Vec2i( 1920, 1080 );
	
	// Initialize framebuffers:
	gl::Fbo::Format format;
	mFBOs[0]    = gl::Fbo( mDimension.x, mDimension.y, format );
	mFBOs[1]    = gl::Fbo( mDimension.x, mDimension.y, format );
	mCurrentFBO = 0;
	mOtherFBO   = 1;
	
	// Load Game of Life shader:
	mShader = gl::GlslProg( kVertGlsl.c_str(), kFragGlsl.c_str() );
	
	// Initialize board:
	reset();
	
	// Set texture GL parameters:
	// Texture should REPEAT so that the GOL environment wraps around the texture edges.
	// Texture should use NEAREST interpolation to prevent value blending (though GL_LINEAR works too).
	mTexture.setWrap( GL_REPEAT, GL_REPEAT );
	mTexture.setMinFilter( GL_NEAREST );
	mTexture.setMagFilter( GL_NEAREST );
	mTexture.bind( 1 );
}

void GLSLGameOfLifeApp::keyUp(KeyEvent event)
{
	// The 'r' key resets framebuffers:
	if( event.getChar() == 'r' ) { reset(); }
}

void GLSLGameOfLifeApp::update()
{
	// Choose the next framebuffer (ping-pong):
	mCurrentFBO = ( mCurrentFBO + 1 ) % 2;
	mOtherFBO   = ( mCurrentFBO + 1 ) % 2;
	
	// Bind the current framebuffer:
	mFBOs[ mCurrentFBO ].bindFramebuffer();
	
	// Bind the current texture:
	mFBOs[ mOtherFBO ].bindTexture();
	
	// Set GL texture parameters:
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	
	// Bind shader:
	mShader.bind();
	mShader.uniform( "mTexture", 0 );
	mShader.uniform( "mWidth", static_cast<float>( mDimension.x ) );
	mShader.uniform( "mHeight", static_cast<float>( mDimension.y ) );
	
	// Draw shader onto a rectangle of the appropriate dimension:
	gl::drawSolidRect( mFBOs[ mCurrentFBO ].getBounds() );
	
	// Unbind shader:
	mShader.unbind();
	
	// Unbind the current texture:
	mFBOs[ mOtherFBO ].unbindTexture();
	
	// Unbind the current framebuffer:
	mFBOs[ mCurrentFBO ].unbindFramebuffer();
}

void GLSLGameOfLifeApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set viewport from framebuffer dimension (with origin in lower right):
	gl::setMatricesWindow( mFBOs[ 0 ].getSize(), false );
	
	// Draw the current framebuffer texture:
	gl::draw( mFBOs[ mCurrentFBO ].getTexture(), getWindowBounds() );
}

CINDER_APP_NATIVE( GLSLGameOfLifeApp, RendererGl )
