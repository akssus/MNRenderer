#include <SFML/Graphics.hpp>
#include <vector>
#include <gmtl/gmtl.h>
#include <gmtl/Matrix.h>
#include "VideoBuffer.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define RESOLUTION_WIDTH 100
#define RESOLUTION_HEIGHT 100

using namespace sf;

struct Mesh
{
	gmtl::Vec3f _pos;
	std::vector<gmtl::Vec3f> _lstVerts;
};

class Renderer
{
public:
	Renderer(int width, int height)
	{
		_videoBuffer = new VideoBuffer(width,height);
	}
	~Renderer()
	{
		if (_videoBuffer != nullptr)
			delete _videoBuffer;
		_videoBuffer = nullptr;
	}

	void applyTransform(gmtl::Matrix44f matTransform)
	{
		_matRenderThrough = matTransform * _matRenderThrough;
	}
	void clearTransform()
	{
		_matRenderThrough = gmtl::Matrix44f();
	}
	void drawVertex(gmtl::Vec3f& vert)
	{
		gmtl::Vec3f transformedVert;
		gmtl::xform(transformedVert, _matRenderThrough, transformedVert);
		_videoBuffer->drawPixel(transformedVert.mData[0], transformedVert.mData[1],sf::Color::Black);
	}
	void clearBuffer(sf::Color color)
	{
		_videoBuffer->clearBuffer(color);
	}
	void swapBuffer(sf::RenderWindow* pWindow)
	{
		if(_videoBuffer != nullptr)
			_videoBuffer->swapBuffer(pWindow);
	}

private:
	VideoBuffer* _videoBuffer;
	gmtl::Matrix44f _matRenderThrough;
};


int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MNRENDERER");
	sf::View view(sf::FloatRect(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT));
	view.setCenter(RESOLUTION_WIDTH/2, RESOLUTION_HEIGHT/2);
	window.setView(view);

	Renderer renderer(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);

	sf::Clock clock;

	/*
	Point corners[8] = {
         { 1, -1, -5},
         { 1, -1, -3},
         { 1,  1, -5},
         { 1,  1, -3},
         {-1, -1, -5},
         {-1, -1, -3},
         {-1,  1, -5},
         {-1,  1, -3}
    };*/
	Mesh mesh;
	mesh._lstVerts.push_back(gmtl::Vec3f(1, -1, -5));
	mesh._lstVerts.push_back(gmtl::Vec3f(1, -1, -3));
	mesh._lstVerts.push_back(gmtl::Vec3f(1, 1, -5));
	mesh._lstVerts.push_back(gmtl::Vec3f(1, 1, -3));
	mesh._lstVerts.push_back(gmtl::Vec3f(-1, -1, -5));
	mesh._lstVerts.push_back(gmtl::Vec3f(-1, -1, -3));
	mesh._lstVerts.push_back(gmtl::Vec3f(-1, 1, -5));
	mesh._lstVerts.push_back(gmtl::Vec3f(-1, 1, -3));


	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		sf::Time elapsedTime = clock.getElapsedTime();
		if (elapsedTime.asMilliseconds() > 1000.0f / 60.0f)
		{

			window.clear();
			renderer.clearBuffer(Color::Black);
			/*****************************************/


			//world space transform
			for (gmtl::Vec3f vec : mesh._lstVerts)
			{

			}

			/*****************************************/
			renderer.swapBuffer(&window);
			window.display();
			clock.restart();
		}
	}

	return 0;
}