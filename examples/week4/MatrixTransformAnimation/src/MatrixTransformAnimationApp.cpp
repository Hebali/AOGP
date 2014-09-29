//////////////////////////////////////////////////
/*          Art of Graphics Programming         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2014                   */
//////////////////////////////////////////////////

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Timeline.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MatrixTransformAnimationApp : public AppNative {
public:
	void setup();
	void mouseDown(MouseEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	void resize();
	
	void drawStrokedCubeUncentered(const Vec3f &position, const Vec3f &size);
	
	CameraPersp mCam;

	Vec3f		mCubeDimension;
	Vec3f		mCubeTranslationInit, mCubeScaleInit, mCubeRotationInit, mCubeAnchorInit;
	Vec3f		mCubeTranslationTarg, mCubeScaleTarg, mCubeRotationTarg, mCubeAnchorTarg;
	Anim<Vec3f>	mCubeTranslationAnim, mCubeScaleAnim, mCubeRotationAnim, mCubeAnchorAnim;
};

void MatrixTransformAnimationApp::drawStrokedCubeUncentered(const Vec3f& ori, const Vec3f& size)
{
	Vec3f max = ori + size;
	
	gl::drawLine( Vec3f(ori.x, ori.y, ori.z), Vec3f(max.x, ori.y, ori.z) );
	gl::drawLine( Vec3f(max.x, ori.y, ori.z), Vec3f(max.x, max.y, ori.z) );
	gl::drawLine( Vec3f(max.x, max.y, ori.z), Vec3f(ori.x, max.y, ori.z) );
	gl::drawLine( Vec3f(ori.x, max.y, ori.z), Vec3f(ori.x, ori.y, ori.z) );
	
	gl::drawLine( Vec3f(ori.x, ori.y, max.z), Vec3f(max.x, ori.y, max.z) );
	gl::drawLine( Vec3f(max.x, ori.y, max.z), Vec3f(max.x, max.y, max.z) );
	gl::drawLine( Vec3f(max.x, max.y, max.z), Vec3f(ori.x, max.y, max.z) );
	gl::drawLine( Vec3f(ori.x, max.y, max.z), Vec3f(ori.x, ori.y, max.z) );
	
	gl::drawLine( Vec3f(ori.x, ori.y, ori.z), Vec3f(ori.x, ori.y, max.z) );
	gl::drawLine( Vec3f(ori.x, max.y, ori.z), Vec3f(ori.x, max.y, max.z) );
	gl::drawLine( Vec3f(max.x, max.y, ori.z), Vec3f(max.x, max.y, max.z) );
	gl::drawLine( Vec3f(max.x, ori.y, ori.z), Vec3f(max.x, ori.y, max.z) );
}

void MatrixTransformAnimationApp::setup()
{
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Set cube dimension:
	mCubeDimension = Vec3f( 1.0, 2.0, 2.0 );
	
	// Set cube initial transformations:
	mCubeTranslationInit = Vec3f::zero();
	mCubeScaleInit       = Vec3f::one();
	mCubeRotationInit    = Vec3f::zero();
	mCubeAnchorInit      = Vec3f::zero();
	
	// Set cube target transformations:
	mCubeTranslationTarg = Vec3f( 1.5, 0.0, 0.0 );
	mCubeScaleTarg       = Vec3f( 0.5, 0.5, 0.5 );
	mCubeRotationTarg    = Vec3f( 45.0, 0.0, 0.0 );
	mCubeAnchorTarg      = -mCubeDimension * 0.5;
	
	// Set cube anim transformations:
	mCubeTranslationAnim = mCubeTranslationInit;
	mCubeScaleAnim       = mCubeScaleInit;
	mCubeRotationAnim    = mCubeRotationInit;
	mCubeAnchorAnim      = mCubeAnchorInit;
	
	// Set the number of seconds per transition:
	float tSecPerTrans   = 2.0;
	
	// Set the easing transition function:
	// (Cubic is a nice, smooth one. But there are lots of options...)
	EaseFn tEase;
	tEase = EaseInOutCubic();
	//tEase = EaseInOutBounce();
	
	// Prepare custom timeline:
	ci::TimelineRef	tTimeline;
	tTimeline = cinder::Timeline::create();
	tTimeline->setInfinite( false );
	tTimeline->setDefaultAutoRemove( false );
	tTimeline->setAutoRemove( false );
	tTimeline->setPingPong( true );
	timeline().add( tTimeline );
	
	// Create timeline transitions:
	tTimeline->apply( &mCubeTranslationAnim, mCubeTranslationTarg, tSecPerTrans, tEase );
	tTimeline->apply( &mCubeScaleAnim, mCubeScaleTarg, tSecPerTrans, tEase ).appendTo( &mCubeTranslationAnim );
	tTimeline->apply( &mCubeRotationAnim, mCubeRotationTarg, tSecPerTrans, tEase ).appendTo( &mCubeScaleAnim );
	tTimeline->apply( &mCubeAnchorAnim, mCubeAnchorTarg, tSecPerTrans, tEase ).appendTo( &mCubeRotationAnim );
}

void MatrixTransformAnimationApp::mouseDown( MouseEvent event )
{
}

void MatrixTransformAnimationApp::keyUp(KeyEvent event)
{
}

void MatrixTransformAnimationApp::update()
{
}

void MatrixTransformAnimationApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Update camera position:
	mCam.lookAt( Vec3f( cos( getElapsedSeconds() * 0.25 ), 1.0, sin( getElapsedSeconds() * 0.25 ) ) * 5.0, Vec3f::zero() );

	// Set matrices from camera:
	gl::setMatrices( mCam );
	
	// Set color:
	gl::color( Color( 1.0, 1.0, 1.0 ) );
	
	// Draw origin:
	gl::drawCoordinateFrame();
	
	// Draw animated:
	gl::color( Color( 1.0, 1.0, 1.0 ) );
	gl::pushMatrices();
	gl::translate( mCubeTranslationAnim );
	gl::rotate( mCubeRotationAnim );
	gl::scale( mCubeScaleAnim );
	gl::translate( mCubeAnchorAnim );
	drawStrokedCubeUncentered( Vec3f::zero(), mCubeDimension );
	gl::popMatrices();
	
	// Draw initial:
	gl::color( Color( 1.0, 0.0, 0.0 ) );
	gl::pushMatrices();
	gl::translate( mCubeTranslationInit );
	gl::rotate( mCubeRotationInit );
	gl::scale( mCubeScaleInit );
	gl::translate( mCubeAnchorInit );
	drawStrokedCubeUncentered( Vec3f::zero(), mCubeDimension );
	gl::popMatrices();
	
	// Draw target:
	gl::color( Color( 0.0, 1.0, 0.0 ) );
	gl::pushMatrices();
	gl::translate( mCubeTranslationTarg );
	gl::rotate( mCubeRotationTarg );
	gl::scale( mCubeScaleTarg );
	gl::translate( mCubeAnchorTarg );
	drawStrokedCubeUncentered( Vec3f::zero(), mCubeDimension );
	gl::popMatrices();
}

void MatrixTransformAnimationApp::resize()
{
	// Setup camera:
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
}

CINDER_APP_NATIVE( MatrixTransformAnimationApp, RendererGl )
