#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"


using namespace ci;
using namespace ci::app;
using namespace std;
Font mFont;//custom font for optimized drawing
gl::TextureFontRef mTextureFont;//custom opengl::texture
class tree {
public:
	float branch_scalar = 0.67;
	int angle = 45;
	void branch(float len = 100) {
		glPushMatrix();
		gl::drawLine(Vec2f(0, 0), Vec2f(0, -len));//single branch;
		gl::translate(0, -len);
		if (len > 2) {
			glPushMatrix();
			gl::rotate(angle);
			branch(len * branch_scalar);
			glPopMatrix();
			glPushMatrix();
			gl::rotate(-angle);
			branch(len * branch_scalar);
			glPopMatrix();
		}
		glPopMatrix();
	}
};
class recursionApp : public AppNative {
  public:
	void prepareSettings(Settings* settings);
	void setup();
	void mouseMove( MouseEvent event );	
	void update();
	void drawFontText(float text, Vec2f pos);
	void draw();
	tree t;
	Vec2f corner_fps;
private:
	// Change screen resolution
	int mScreenWidth, mScreenHeight;
	float initWidth, initHeight;
	void getScreenResolution(int& width, int& height);
};
void recursionApp::prepareSettings(Settings* settings) {
	//! setup our window
	settings->setTitle("Fractal Tree Simulation (MOVE MOUSE AROUND TO SEE ANGLE CHANGE)");
	settings->setFrameRate(60);//60fps
	gl::enableVerticalSync();//vsync
	getScreenResolution(mScreenWidth, mScreenHeight);//gets resolution relative to monitor
	settings->setWindowPos(mScreenWidth / 6, mScreenHeight / 6);
	int aspectRatio = mScreenWidth / 7;//using 4/7ths of monitor resolution
	initWidth = aspectRatio * 4;
	initHeight = aspectRatio * 3;
	settings->setWindowSize(initWidth, initHeight);//maintains 4:3 aspect ratio
}
void recursionApp::getScreenResolution(int& width, int& height) {
	const HWND hDesktop = GetDesktopWindow();// Get a handle to the desktop window
	RECT rDesktop;// Get the size of the screen into a rectangle
	GetWindowRect(hDesktop, &rDesktop);
	width = rDesktop.right;
	height = rDesktop.bottom;
}
void recursionApp::setup()
{
	corner_fps.x = getWindowWidth() - 120;
	corner_fps.y = 0 + 80;
	mFont = Font("Arial", 40);//fixed custom font
	mTextureFont = gl::TextureFont::create(mFont);
}

void recursionApp::mouseMove( MouseEvent event )
{
	t.angle = ((getWindowWidth() / 2) - event.getX())/3;
}

void recursionApp::update()
{
}
void recursionApp::drawFontText(float text, Vec2f pos) {
	std::stringstream dummyText;
	std::string PRINT;
	dummyText << text;
	dummyText >> PRINT;
	pos.y += 20;//constant down
	mTextureFont->drawString(PRINT, pos);
}
void recursionApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	glPushMatrix();
	gl::scale(2, 2);
	gl::translate(300, 400);// getWindowWidth() / 2, getWindowHeight() * 0.8);//origin now at 300x 400y
	t.branch();
	glPopMatrix();
	gl::color(0, 1, 0);
	drawFontText(getAverageFps(), corner_fps);
	gl::color(1, 1, 1);
	drawFontText(t.angle, Vec2f(corner_fps.x, corner_fps.y + 80));
}

CINDER_APP_NATIVE( recursionApp, RendererGl )
