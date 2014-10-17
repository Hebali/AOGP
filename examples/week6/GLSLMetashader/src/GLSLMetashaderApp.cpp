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

#include <sstream>

#define STRINGIFY(s) #s

#define CAM_WIDTH  640
#define CAM_HEIGHT 480

using namespace ci;
using namespace ci::app;
using namespace std;

inline std::string generateBloomFilterGlslVert()
{
	stringstream ss;
	
	size_t indent = 0;
	
	ss << string( indent++, '\t' )	<< "void main() {" << endl;
	ss << string( indent, '\t' )	<< "gl_FrontColor = gl_Color;" << endl;
	ss << string( indent, '\t' )	<< "gl_TexCoord[0] = gl_MultiTexCoord0;" << endl;
	ss << string( indent, '\t' )	<< "gl_Position = ftransform();" << endl;
	ss << string( --indent, '\t' )	<< "}" << endl;
	
	return ss.str();
}

inline std::string generateBloomFilterGlslFrag(const int& xKernal, const int& yKernal)
{
	stringstream ss;
	
	size_t indent = 0;
	
	ss << string( indent, '\t' )	<< "uniform sampler2D texture;" << endl;
	ss << string( indent++, '\t' )	<< "void main() {" << endl;
	
	ss << string( indent, '\t' )	<< "int  i, j;" << endl;
	ss << string( indent, '\t' )	<< "vec4 bloom;" << endl;
	ss << string( indent, '\t' )	<< "vec4 sum = vec4( 0.0 );" << endl;
	ss << string( indent, '\t' )	<< "vec2 texCoord = gl_TexCoord[ 0 ].xy;" << endl;
	
	ss << string( indent++, '\t' )	<< "for(i = " << -xKernal << "; i < " << xKernal << "; i++) {" << endl;
	ss << string( indent++, '\t' )	<< "for(j = " << -yKernal << "; j < " << yKernal << "; j++) {" << endl;
	ss << string( indent, '\t' )	<< "sum += texture2D( texture, texCoord + vec2( j, i ) * 0.004 ) * 0.25;" << endl;
	ss << string( --indent, '\t' )	<< "}" << endl;
	ss << string( --indent, '\t' )	<< "}" << endl;
	
	ss << string( indent++, '\t' )	<< "if( texture2D( texture, texCoord ).r < 0.3 ) {" << endl;
	ss << string( indent, '\t' )	<< "bloom = sum * sum * 0.012 + texture2D( texture, texCoord );" << endl;
	ss << string( --indent, '\t' )	<< "}" << endl;
	ss << string( indent++, '\t' )	<< "else {" << endl;
	ss << string( indent++, '\t' )	<< "if( texture2D( texture, texCoord ).r < 0.5 ) {" << endl;
	ss << string( indent, '\t' )	<< "bloom = sum * sum * 0.009 + texture2D( texture, texCoord );" << endl;
	ss << string( --indent, '\t' )	<< "}" << endl;
	ss << string( indent++, '\t' )	<< "else {" << endl;
	ss << string( indent, '\t' )	<< "bloom = sum * sum * 0.0075 + texture2D( texture, texCoord );" << endl;
	ss << string( --indent, '\t' )	<< "}" << endl;
	ss << string( --indent, '\t' )	<< "}" << endl;
	
	ss << string( indent, '\t' )	<< "gl_FragColor = vec4( bloom );" << endl;
	ss << string( --indent, '\t' )	<< "}" << endl;
	
	return ss.str();
}

class GLSLMetashaderApp : public AppNative {
  public:
	void prepareSettings(Settings *settings);
	void setup();
	void keyUp(KeyEvent event);
	void update();
	void draw();
	
	void generateShader(const size_t& iKernelAxisLen);
	
	ci::CaptureRef			mCapture;
	ci::gl::TextureRef		mTexture;
	ci::gl::GlslProgRef		mShader;
};

void GLSLMetashaderApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize( CAM_WIDTH * 2, CAM_HEIGHT );
	settings->setFrameRate( 60.0f );
}


void GLSLMetashaderApp::setup()
{
	// Try to initialize camera capture:
	try {
		mCapture = Capture::create( CAM_WIDTH, CAM_HEIGHT );
		mCapture->start();
	}
	catch(...) { cout << "Failed to initialize capture" << endl; }
	
	// Create initial shader with a 3x3 kernel:
	generateShader( 3 );
}

void GLSLMetashaderApp::keyUp(KeyEvent event)
{
	char c = event.getChar();
	// Check whether character is a digit:
	if( c >= '0' && c <= '9' ) {
		// Cast digit character to integer:
		size_t tAxisLen = static_cast<int>( c ) - 48 + 1;
		// Create shader with a NxN kernel:
		generateShader( tAxisLen );
	}
}

void GLSLMetashaderApp::update()
{
	// Check whether capture is live and has a new frame:
	if( mCapture && mCapture->checkNewFrame() ) {
		// Get texture from camera:
		mTexture = gl::Texture::create( mCapture->getSurface() );
	}
}

void GLSLMetashaderApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set color:
	gl::color( 1.0, 1.0, 1.0 );
	
	// Check whether texture has been initialized:
	if( mTexture ) {
		// Draw camera texture:
		gl::draw( mTexture );
		
		// Push matrix and translate along x-axis:
		gl::pushMatrices();
		gl::translate( CAM_WIDTH, 0.0 );
		
		// Bind texture:
		mTexture->bind( 0 );
		
		// Bind shader:
		mShader->bind();
		
		// Set shader uniforms:
		mShader->uniform( "texture", 0 );
		mShader->uniform( "width", static_cast<float>( mTexture->getWidth() ) );
		mShader->uniform( "height", static_cast<float>( mTexture->getHeight() ) );
		
		// Draw rect with texture bounds:
		gl::drawSolidRect( mTexture->getBounds() );
		
		// Unbind shader:
		mShader->unbind();
		
		// Unbind texture:
		mTexture->unbind();
		
		// Pop matrix:
		gl::popMatrices();
	}
}

void GLSLMetashaderApp::generateShader(const size_t& iKernelAxisLen)
{
	string tVert = generateBloomFilterGlslVert();
	string tFrag = generateBloomFilterGlslFrag( iKernelAxisLen, iKernelAxisLen );
	try {
		mShader = gl::GlslProg::create( tVert.c_str(), tFrag.c_str() );
		cout << "// Vertex shader:" << endl << tVert << endl;
		cout << "// Fragment shader:" << endl << tFrag << endl << endl;
	}
	catch( gl::GlslProgCompileExc &exc ) {
		cout << "Shader compile error: " << endl;
		cout << exc.what();
		exit(1);
	}
	catch( ... ) {
		cout << "Unable to load shader" << endl;
		exit(1);
	}
}

CINDER_APP_NATIVE( GLSLMetashaderApp, RendererGl )
