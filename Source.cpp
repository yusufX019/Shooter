#include <iostream>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;


//   ------------------Window------------------
float windowWidth  = 800;
float windowHeight = 600;
sf::RenderWindow mainWindow(sf::VideoMode(windowWidth,windowHeight),"Shit");

sf::Clock timer;
class enemy;
class bullet;

vector<enemy>  enemies;
vector<bullet> bullets;

class enemy
{
	float width;
	float height;
	sf::Vector2f position;
	sf::Color    color;
	sf::RectangleShape rect;
	int disabledIndex;
	
	
public:
	void create(float width, float height, sf::Color color,float x, float y)
	{
		this->width		 = width;
		this->height	 = height;
		this->color	 	 = color;
		this->position.x = x;
		this->position.y = y;

		rect.setSize(sf::Vector2f(width, height));
		rect.setFillColor(color);
		rect.setPosition(x, y);
		rect.setOrigin(width / 2, height / 2);
		
		
	}

	void draw()
	{
		mainWindow.draw(rect);
	}

	void move(float elapsedTime)
	{
		rect.move(10 * elapsedTime, 100 * elapsedTime);
	}

	
	void appear(float elapsedTime)
	{
		enemy newEnemy;


		if(timer.getElapsedTime().asSeconds()>2)
		{
			newEnemy.create(50, 50, sf::Color::Blue, rand() % int(windowWidth), 0);
			
			enemies.push_back(newEnemy);
			
			timer.restart();
		}
		

		for (int i = 0; i < enemies.size(); i++)
		{
			
			enemies.at(i).draw();
			enemies.at(i).move(elapsedTime);
			
		}

	}

	float getPositionX()
	{
		return rect.getPosition().x;
	}

	float getPositionY()
	{
		return rect.getPosition().y;
	}

	float getWidth()
	{
		return width;
	}

	float getHeight()
	{
		return height;
	}

	sf::RectangleShape get()
	{
		return rect;
	}


};


class bullet
{
	float width;
	float height;
	sf::Color color;
	sf::RectangleShape rect;
	sf::Vector2f position;
	
public:
	bullet(float width, float height, sf::Color color, float x, float y)
	{
		this->width = width;
		this->height = height;
		this->color = color;
		position.x = x;
		position.y = y;


		rect.setSize(sf::Vector2f(width, height));
		rect.setFillColor(color);
		rect.setOrigin(width / 2, height / 2);
		rect.setPosition(x, y);


	}

	void move(float elapsedTime)
	{
		rect.move(0,-500*elapsedTime);
	}

	void draw()
	{
		mainWindow.draw(rect);
	}

	float getPositionX()
	{
		return rect.getPosition().x;
	}
	float getPositionY()
	{
		return rect.getPosition().y;
	}
	
	float getWidth()
	{
		return width;
	}

	float getHeight()
	{
		return height;
	}

};






class player
{
	float width;
	float height;
	sf::Color color;
	sf::Vector2f position;
	sf::RectangleShape rect;
	
public:
	player(float width,float height,sf::Color color,float x,float y)
	{
		this->width  = width;
		this->height = height;
		this->color  = color;
		position.x   = x;
		position.y   = y;

	// -----setting size-----
		rect.setSize(sf::Vector2f(width,height));
	
	// -----  origin  -----
		rect.setOrigin(width / 2, height / 2);

	// -----   color   -----
		rect.setFillColor(color);

	// -----   position   -----
		rect.setPosition(position.x,position.y);

	}

	void draw()
	{
		mainWindow.draw(rect);
	}


	void move(float elapsedTime)
	{
		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		rect.move(0, -100*elapsedTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		rect.move(0, 100 * elapsedTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		rect.move(-200 * elapsedTime, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		rect.move(200*elapsedTime, 0);

	}

//	-----------     getters       -----------
	float getWidth()
	{
		return width;
	}

	float getHeight()
	{
		return height;
	}

	
	bool shooted = false;

	void shoot(float elapsedTime)
	{
		bullet object(20, 20, sf::Color::Red, rect.getPosition().x, rect.getPosition().y );
		
		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (!shooted) {
				shooted = true;
				bullets.push_back(object);
			}
		}

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			shooted = false;
			
		


		for (int i = 0; i < bullets.size(); i++)
		{
			bullets.at(i).draw();
			bullets.at(i).move(elapsedTime);	
		}
	}



};

void reached()
{

	for(int i=0;i<enemies.size();i++)
	{
		float eWidth = enemies.at(i).getWidth();
		float eHeight = enemies.at(i).getHeight();
		float ePosX = enemies.at(i).getPositionX();
		float ePosY = enemies.at(i).getPositionY();

		for(int j=0;j<bullets.size();j++)
		{
			float bWidth  = bullets.at(j).getWidth();
			float bHeight = bullets.at(j).getHeight();
			float bPosX   = bullets.at(j).getPositionX();
			float bPosY   = bullets.at(j).getPositionY();

		
			if (int(ePosY + eHeight / 2) >= int(bPosY - bHeight / 2))
			{
				if ( ((bPosX-bWidth/2)<(ePosX+eWidth/2) && (bPosX - bWidth / 2) > (ePosX - eWidth / 2)) || ( (bPosX+bWidth/2) > (ePosX - eWidth / 2) && (bPosX + bWidth / 2)< (ePosX + eWidth / 2))  )
				{
					enemies.erase(enemies.begin() + i);
					bullets.erase(bullets.begin() + j);
				}
			}


			if(bPosY<0)
				bullets.erase(bullets.begin() + j);
			
			if (ePosY >windowHeight)
				enemies.erase(enemies.begin() + i);


		}
	}

}




void userExit()
{
	sf::Event mainEvent;
	while (mainWindow.pollEvent(mainEvent))if (mainEvent.type == sf::Event::Closed)mainWindow.close();
}


void main()
{
	//start the timer
	sf::Clock clock;
	


	player player(200,100,sf::Color::Green,windowWidth/2,windowHeight);
	enemy  enemy;
	
	//main loop
	while(mainWindow.isOpen())
	{
	

		
		float elapsedTime = clock.getElapsedTime().asSeconds();
		

		clock.restart();
		userExit();

		
		
//      -----------drawing section-----------------
		mainWindow.clear(sf::Color::Black);

		player.draw();
		player.move(elapsedTime);
		player.shoot(elapsedTime);
		enemy.appear(elapsedTime);
		reached();

		mainWindow.display();
	}

}


