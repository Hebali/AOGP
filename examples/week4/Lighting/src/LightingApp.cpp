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

#include "MeshFactory.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LightingApp : public AppNative {
public:
	void prepareSettings(Settings* settings);
	void setup();
	void update();
	void mouseMove(MouseEvent event);
	void draw();
	void resize();

	CameraPersp		mCam;
	Vec2f			mMousePos;
	
	float			mSphereRadius;
	float			mSphereOffset;
	int				mSphereDetail;
	
	ProtoMesh		mMeshProto;
	gl::VboMesh		mMeshVbo;
	
	Anim<ColorA>		mDiffuse;
	Anim<ColorA>		mAmbient;
	Anim<ColorA>		mSpecular;
	Anim<ColorA>		mEmissive;
	Anim<GLfloat>	mShininess;
};

void LightingApp::prepareSettings(Settings* settings)
{
	settings->setWindowSize( 800, 800 );
}

void LightingApp::setup()
{
	// In OpenGL, lighting is a pain. It has a variety of limitations,
	// the API is somewhat counter-intuitive and it often doesn't look
	// very good. There are lots of little tricks that can improve the quality.
	// But, in many cases, the best (or only) option is to use shader-based
	// lighting, which has some of the same limitations but is overall much
	// easier to control and tune. In this example, we'll look at the
	// basics of lighting in OpenGL without shaders.
	
	// Enable alpha blending:
	gl::enableAlphaBlending();
	
	// Enable depth buffer read/write:
	gl::enableDepthRead();
	gl::enableDepthWrite();
	
	// Enable smooth shading:
	glShadeModel( GL_SMOOTH );
	
	// Set the number of seconds per transition:
	float tSecPerTrans = 2.0;
	
	// Set the easing function:
	EaseFn tEase = EaseInOutCubic();
	
	// Set initial material parameters:
	mAmbient = ColorA( 0.0, 0.0, 0.0, 1.0 );
	mDiffuse = ColorA( 0.0, 0.0, 0.0, 1.0 );
	mSpecular = ColorA( 0.0, 0.0, 0.0, 1.0 );
	mEmissive = ColorA( 0.0, 0.0, 0.0, 1.0 );
	mShininess = 0.0;

	// Prepare custom timeline:
	ci::TimelineRef	tTimeline;
	tTimeline = cinder::Timeline::create();
	tTimeline->setInfinite( false );
	tTimeline->setDefaultAutoRemove( false );
	tTimeline->setAutoRemove( false );
	tTimeline->setPingPong( true );
	timeline().add( tTimeline );
	
	// Create timeline transitions:
	tTimeline->apply( &mAmbient, ColorA( 0.3, 0.3, 0.3, 1.0 ), tSecPerTrans, tEase );
	tTimeline->apply( &mDiffuse, ColorA( 0.5, 0.1, 0.1, 1.0 ), tSecPerTrans, tEase ).appendTo( &mAmbient );
	tTimeline->apply( &mSpecular, ColorA( 0.1, 0.5, 0.1, 1.0 ), tSecPerTrans, tEase ).appendTo( &mDiffuse );
	tTimeline->apply( &mShininess, 128.0f, tSecPerTrans, tEase ).appendTo( &mSpecular );
	tTimeline->apply( &mEmissive, ColorA( 0.1, 0.1, 0.5, 1.0 ), tSecPerTrans, tEase ).appendTo( &mShininess );
	
	// Prepare sphere parameters:
	mSphereRadius = 100.0;
	mSphereOffset = mSphereRadius + 5.0;
	mSphereDetail = 500;
	
	// Notice: The resolution of the geometry (mSphereDetail) will impact
	// the render quality of lit objects. There are a few things we could do about this.
	// But the most effective (when possible) will be to increase the geometry's resolution.
	// However, a higher resolution geometry will slow the render process. So, it's
	// time to start thinking about optimizing our geometries for the GPU...
	
	// Initialize a sphere mesh:
	createSphere( mSphereDetail, mSphereDetail, mSphereRadius, mMeshProto );
	
	// Convert mesh to VBO:
	createMeshVbo( mMeshProto, mMeshVbo );
}

void LightingApp::mouseMove(MouseEvent event)
{
	mMousePos.x = event.getX() - getWindowWidth() * 0.5f;
	mMousePos.y = getWindowHeight() * 0.5f - event.getY();
}

void LightingApp::update()
{
}

void LightingApp::draw()
{
	// Clear window:
	gl::clear( Color( 0, 0, 0 ) );
	
	// Set matrices from camera:
	gl::setMatrices( mCam );
	
	// Set color:
	gl::color( 1.0, 1.0, 1.0 );
	
	// Enable lighting:
	glEnable( GL_LIGHTING );
	
	// Enable light0:
	glEnable( GL_LIGHT0 );
	
	// Notice: Each light gets hard-coded with a light index - in this case, GL_LIGHT0
	// OpenGL only allows eight of these to be used at a time. By grouping your geometries,
	// you can increase this number. But, each geometry can only be related to eight lights.
	// The good news is that in most scenarios, eight lights is more than enough.
	// But, it's an annoying limitation nonetheless.
	
	// Create directional light vector:
	GLfloat tLightDir[] = { mMousePos.x, mMousePos.y, 100.0, 0.0 };
	
	// Set light0 with directional vector
	glLightfv( GL_LIGHT0, GL_POSITION, tLightDir );
	
	// Set material's ambient color:
	glMaterialfv( GL_FRONT, GL_AMBIENT, mAmbient.value() );
	
	// Set material's diffuse color:
	glMaterialfv( GL_FRONT, GL_DIFFUSE, mDiffuse.value() );
	
	// Set material's specular color and shininess:
	glMaterialfv( GL_FRONT, GL_SPECULAR, mSpecular.value() );
	glMaterialfv( GL_FRONT, GL_SHININESS,  mShininess.ptr() );
	
	// Set material's emissive color:
	glMaterialfv( GL_FRONT, GL_EMISSION, mEmissive.value() );

	// Draw sphere mesh (four times, with translation):
	{
		{
			gl::pushMatrices();
			gl::translate( Vec3f( -mSphereOffset, -mSphereOffset, 0.0 ) );
			gl::draw( mMeshVbo );
			gl::popMatrices();
		}
		
		{
			gl::pushMatrices();
			gl::translate( Vec3f(  mSphereOffset, -mSphereOffset, 0.0 ) );
			gl::draw( mMeshVbo );
			gl::popMatrices();
		}
		
		{
			gl::pushMatrices();
			gl::translate( Vec3f(  mSphereOffset,  mSphereOffset, 0.0 ) );
			gl::draw( mMeshVbo );
			gl::popMatrices();
		}
		
		{
			gl::pushMatrices();
			gl::translate( Vec3f( -mSphereOffset,  mSphereOffset, 0.0 ) );
			gl::draw( mMeshVbo );
			gl::popMatrices();
		}
	}

	// Disable light0:
	glDisable( GL_LIGHT0 );
	
	// Disable lighting:
	glDisable( GL_LIGHTING );
}

void LightingApp::resize()
{
	// Setup camera:
	mCam.lookAt( Vec3f( 0.0, 0.0, 500.0 ), Vec3f::zero() );
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
}

CINDER_APP_NATIVE( LightingApp, RendererGl )
