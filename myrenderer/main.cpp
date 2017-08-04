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
		_width = width;
		_height = height;
	}
	~Renderer()
	{
		if (_videoBuffer != nullptr)
			delete _videoBuffer;
		_videoBuffer = nullptr;
	}

	void setFrustum(float left, float right, float top, float bottom, float near, float far)
	{
		_matProjection = gmtl::Matrix44f();
		_matProjection[0][0] = (2 * near) / (right - left);
		_matProjection[0][2] = (right + left) / (right - left);
		_matProjection[1][1] = (2 * near) / (top - bottom);
		_matProjection[1][2] = (top + bottom) / (top - bottom);
		_matProjection[2][2] = (far + near) / (far - near);
		_matProjection[2][3] = (2 * far*near) / (far - near);
		_matProjection[3][2] = 1;
		_matProjection[3][3] = 0;
	}

	void setWorldTransform(gmtl::Matrix44f& matWorld)
	{
		_matWorld = matWorld;
	}
	void setViewtransform(gmtl::Matrix44f& matView)
	{
		_matWorld = matView;
	}

	void drawVertex(gmtl::Vec3f& vert)
	{
		//vertex shading
		gmtl::Vec4f transformedVert = vertexShader(vert);
		//rasterizing
		//pixel shading

		//depth test
		if (transformedVert.mData[2] <= 0) 
			return;
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
	gmtl::Vec4f vertexShader(gmtl::Vec3f& vert)
	{
		gmtl::Vec4f transformedVert;
		transformedVert.mData[0] = vert.mData[0];
		transformedVert.mData[1] = vert.mData[1];
		transformedVert.mData[2] = vert.mData[2];
		transformedVert.mData[3] = 1;

		//apply world transform 
		gmtl::Vec4f worldVec = transformedVert;
		gmtl::xform(transformedVert, _matWorld, worldVec);

		//apply view transform
		gmtl::Vec4f viewVec = transformedVert;
		gmtl::xform(transformedVert, _matView, viewVec);

		//apply projection trasnform
		gmtl::Vec4f projVec = transformedVert;
		gmtl::xform(transformedVert, _matProjection, projVec);
		transformedVert /= transformedVert.mData[3]; // devide by w

		//NDC transform ( [0,1] ) it means center of the screen is (0,0)
		transformedVert.mData[0] = (transformedVert.mData[0] + 1.0f) / 2.0f;
		transformedVert.mData[1] = (transformedVert.mData[1] + 1.0f) / 2.0f;
		
		//viewport transform
		transformedVert.mData[0] = transformedVert.mData[0] * _width;
		transformedVert.mData[1] = transformedVert.mData[1] * _height;

		return transformedVert;
	}


private:
	VideoBuffer*		_videoBuffer;
	int					_width, _height;
	gmtl::Matrix44f		_matWorld;
	gmtl::Matrix44f		_matView;
	gmtl::Matrix44f		_matProjection;
};


int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MNRENDERER");
	sf::View view(sf::FloatRect(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT));
	view.setCenter(RESOLUTION_WIDTH/2, RESOLUTION_HEIGHT/2);
	window.setView(view);

	Renderer renderer(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);

	Mesh mesh;
	mesh._lstVerts.push_back(gmtl::Vec3f(1, -1, 1));
	mesh._lstVerts.push_back(gmtl::Vec3f(1, -1, -1));
	mesh._lstVerts.push_back(gmtl::Vec3f(1, 1, 1));
	mesh._lstVerts.push_back(gmtl::Vec3f(1, 1, -1));
	mesh._lstVerts.push_back(gmtl::Vec3f(-1, -1, 1));
	mesh._lstVerts.push_back(gmtl::Vec3f(-1, -1, -1));
	mesh._lstVerts.push_back(gmtl::Vec3f(-1, 1, 1));
	mesh._lstVerts.push_back(gmtl::Vec3f(-1, 1, -1));
	mesh._pos.mData[2] = 1.8f;


	sf::Clock clock;

	float angle = 0.0f;
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
			renderer.clearBuffer(Color::White);
			/*****************************************/


			//world space transform
			gmtl::Matrix44f worldTransform;
			
			angle += 0.01f;
			gmtl::EulerAngleXYZf angle(0.0f, angle, 0.0f);
			gmtl::Matrix44f rotation = gmtl::make<gmtl::Matrix44f>(angle);
			worldTransform = rotation * worldTransform;

			gmtl::Matrix44f translation = gmtl::makeTrans<gmtl::Matrix44f>(mesh._pos);
			worldTransform = translation * worldTransform;

			renderer.setWorldTransform(worldTransform);

			//view transform
			//todo

			//projection transform
			renderer.setFrustum(-1, 1, -1, 1, 0.1f, 1.0f);



			//draw
			for (gmtl::Vec3f& vert : mesh._lstVerts)
			{
				renderer.drawVertex(vert);
			}

			/*****************************************/
			renderer.swapBuffer(&window);
			window.display();
			clock.restart();
		}
	}

	return 0;
}