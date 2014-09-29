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

#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TextureLoadingApp : public AppNative {
public:
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
	
	gl::Texture mTexture;
};

void TextureLoadingApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Print app path:
	cout << "App path: " << getAppPath() << endl;
	
	// Print resource directory path:
	cout << "Resource path: " << getResourcePath() << endl;
	
	// Notice that the resource directory is within the application bundle.
	
	// Load texture from resource:
	mTexture = loadImage( loadResource( CINDER_LOGO_IMG ) );
	// Another approach:
	//mTexture = loadImage( loadResource( "SplashScreen.png" ) );
	
	// The options below do not copy the image into the application bundle.
	
	// Print resource directory path:
	cout << "Asset path: " << getAssetPath( "SplashScreen.png" ) << endl;
	
	// Load texture from asset:
	//mTexture = loadImage( loadAsset( "SplashScreen.png" ) );
	
	// Load texture from arbitrary filepath:
	//mTexture = loadImage( "/Users/you/Desktop/SplashScreen.png" );
}

void TextureLoadingApp::mouseDown( MouseEvent event )
{
}

void TextureLoadingApp::update()
{
}

void TextureLoadingApp::draw()
{
	// Clear window:
	gl::clear( Color( 1, 1, 1 ) );
	
	// Set color:
	gl::color( 1.0, 1.0, 1.0 );
	
	// Check whether texture has been instantiated:
	if( mTexture ) {
		// Draw texture, centered in window:
		gl::draw( mTexture, getWindowCenter() - mTexture.getSize() / 2.0 );
	}
}

CINDER_APP_NATIVE( TextureLoadingApp, RendererGl )
