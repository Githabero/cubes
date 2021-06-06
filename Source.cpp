#include <SFML/Graphics.hpp>
#include <cstdlib>
#include<ctime>
#include <math.h>
#include <iostream>
#include <vector>

using namespace sf; 
using namespace std;

///////////////////////								глобальные контроллеры
int FoodAtField = 10;			//еды на поле
int EnemyAtField = 10;			//врагов на поле
///////////////////////
int camera_x = 240;
int camera_y = 140;
int cameraS_x = 640;						//камера
int cameraS_y = 480;
///////////////////////
int border_north = 0;
int border_south = 1440;
int border_west = 0;			//	конец карты
int border_east = 2160;
///////////////////////
int Last_Collision_Index = 0;





RenderWindow window(VideoMode(1080, 720), "Cubes.io");
CircleShape shape(10.f);
Image img;
Texture texture;					//объявление основных переменных и объектов
Texture meat_text;
Texture grass_text;
Texture enemy_text;
View camera(FloatRect(camera_x, camera_y, cameraS_x, cameraS_y));
vector <FloatRect> Collision;




Clock clockse;



class Player
{
private:
	


	Clock clocks;
	int frame = 1;
	float freq = 500;
	float speed = 0.1;
	int toLvl = 3;
	int Score = 0;

public:
	FloatRect bounds;
	Sprite kvadr;
	Player(int x, int y)
	{
		kvadr.setTexture(texture);
		kvadr.setPosition(x, y);
		kvadr.setTextureRect(IntRect(0, 0, 25, 25));
	}
	void IncScore()
	{
		Score++;
		cout << "Score: " << Score << endl;
	}
	void Moving()
	{
		
		float speed = 3;
		if ((Keyboard::isKeyPressed(Keyboard::Right)) && (kvadr.getPosition().x != border_east))
		{

			kvadr.move(speed, 0);
			
			if ((kvadr.getPosition().x - (cameraS_x - camera_x) / 2 >= border_west) && (kvadr.getPosition().x + (cameraS_x - camera_x) / 2 <= border_east))
			{
				camera.move(speed, 0);
				
			}
		}
		if ((Keyboard::isKeyPressed(Keyboard::Left)) && (kvadr.getPosition().x != border_west))
		{
			kvadr.move(-speed, 0);
			if ((kvadr.getPosition().x + (cameraS_x - camera_x) / 2 <= border_east) && (kvadr.getPosition().x - (cameraS_x - camera_x) / 2 >= border_west))
			{
				camera.move(-speed, 0);
			}
		}																																				//прибавляет к координатам спрайта speed в зависимости от нажатой кнопки
		if ((Keyboard::isKeyPressed(Keyboard::Up))    && (kvadr.getPosition().y != border_north))
		{
			kvadr.move(0, -speed);
			if ((kvadr.getPosition().y - (cameraS_y - camera_y) / 2 >= border_north) && (kvadr.getPosition().y + (cameraS_y - camera_y) / 2 <= border_south))
			{
				camera.move(0, -speed);
			}
		}
		if ((Keyboard::isKeyPressed(Keyboard::Down))  && (kvadr.getPosition().y != border_south))
		{
			kvadr.move(0, speed);
			if ((kvadr.getPosition().y + (cameraS_y - camera_y) / 2  <= border_south) && (kvadr.getPosition().y - (cameraS_y - camera_y) / 2  >= border_north))
			{
				camera.move(0, speed);
			}
		}
	//	Vector2f pos(kvadr.getPosition().x, kvadr.getPosition().y);
	//	FloatRect bound(pos, Vector2f(25, 25));
		bounds = kvadr.getGlobalBounds();
		Collision[0] = bounds;
	}
	void Anim()
	{
		double timed = clocks.getElapsedTime().asMilliseconds();
		
		if ((timed >= freq) && (frame == 1))
		{
			frame = 2;
			clocks.restart();
			kvadr.setTextureRect(IntRect(26, 0, 25, 25));			//анимация, переключает кадры в зависимости от времени
			

		}
		timed = clocks.getElapsedTime().asMilliseconds();
	
		if ((timed >= freq) && (frame == 2))
		{
			frame = 1;
			clocks.restart();
			kvadr.setTextureRect(IntRect(0, 0, 25, 25));
			
		}
	}
};


Player player(540, 360);		//создание игрока


class Enemy
{
private:
	int pos_x, pos_y;
	int tar_x, tar_y;
	Clock clokf;
	Clock clocks;
	int level = 1;	//уровень																	//приватные переменные
	int toLvl = 3;  // определяет сколько еды надо съесть перед переходом на следующий уровень
	int FoodEaten = 0; // еды съедено на текущем уровне
	int frame = 1;  
	int freq = 400;
	int Col_numb;


public:

	int Feels = 3;
	int size = 1;
	float speed = 0.1;

	FloatRect IntersectionBound = enemy.getGlobalBounds();
	
	bool Dupl = false;
	FloatRect Bounds;
	Sprite enemy;
	void ShowInfo()		
	{
		cout << "speed: " << speed << endl;
		cout << "freq: " << freq << endl;
		cout << "target x: " << tar_x << endl;			
		cout << "target y: " << tar_y << endl;									//отладка
		cout << "coordinates x: " << trunc(enemy.getPosition().x) << endl;
		cout << "coordinates y: " << trunc(enemy.getPosition().y) << endl;
	}
	Enemy(int numb)
	{
		clokf.restart();
		enemy.setTexture(enemy_text);
		enemy.setTextureRect(IntRect(0, 0, 25, 25));
		enemy.setScale(5.0f, 5.0f);							//размер
		srand(time(0) + rand());
		////////////////
		speed = rand() % 3;
		if (speed == 0) speed += 1;					//установка текстуры, анимации, инициализация внутреннего времени
		freq = rand() % 600 / speed + 1000;
		////////////////
		pos_x = rand() % 1080;
		pos_y = rand() % 720;
		CheckPos();
		enemy.setPosition(pos_x, pos_y);
		tar_x = rand() % 1080;
		tar_y = rand() & 720;
		Col_numb = numb;
		Collision.emplace_back(IntersectionBound);

	}
	void IncScore()
	{
		FoodEaten++;
	}
	void IncLvl()
	{
		if (FoodEaten == toLvl)
		{
			level++;
			FoodEaten = 0;
		}
	}
	void Anim()
	{
		double timed = clocks.getElapsedTime().asMilliseconds();

		if ((timed >= freq) && (frame == 1))
		{
			frame = 2;
			clocks.restart();
			enemy.setTextureRect(IntRect(26, 0, 25, 25));


		}															//анимация
		timed = clocks.getElapsedTime().asMilliseconds();

		if ((timed >= freq) && (frame == 2))
		{
			frame = 1;
			clocks.restart();
			enemy.setTextureRect(IntRect(0, 0, 25, 25));

		}
	}    
	void Move()
	{
		
		if (((trunc(enemy.getPosition().x) == tar_x) || ((trunc(enemy.getPosition().x) + speed > tar_x) && (trunc(enemy.getPosition().x) - speed < tar_x))) && ((trunc(enemy.getPosition().y) == tar_y) || ((trunc(enemy.getPosition().y) + speed > tar_y) && (trunc(enemy.getPosition().y) - speed < tar_y)) )) // проверка условия: если координаты равны точке назначения или единичного сдвига - speed хватает, чтобы координаты + speed были больше координат точки назначения и при этом координаты - speed были меньше этих координат 
		{
			
			srand(time(0) + rand()); 
			tar_x = rand() % border_east;
			tar_y = rand() & border_south;
		}
		if (enemy.getPosition().x != tar_x)
		{
			if (enemy.getPosition().x > tar_x)
			{
				enemy.move(-speed, 0);
				enemy.setScale(0.5f, 0.5f);
			}
			if (enemy.getPosition().x < tar_x)
			{
				enemy.move(speed, 0);
				enemy.setScale(-0.5f, 0.5f);
			}																		
		}												//стремится сравнять свои координаты с координатами точки назначения
		if (enemy.getPosition().y != tar_y)
		{
			if (enemy.getPosition().y > tar_y)
			{
				enemy.move(0, -speed);
			}
			if (enemy.getPosition().y < tar_y)
			{
				enemy.move(0, speed);
			}
		}
		//cout << enemy.getPosition().x - Feels * 7 << "\t" << enemy.getPosition().y - Feels * 7 << "\t" << enemy.getPosition().x << endl;
		Vector2f pos(enemy.getPosition().x - Feels * 7, enemy.getPosition().y - Feels * 7);
		Vector2f Scal(Feels * 14, Feels * 14);
		FloatRect FeelBound(pos, Scal);
		Bounds = FeelBound;
		Collision[Col_numb] = IntersectionBound;
		
		

	}   
	void MakeTarget(int tx, int ty)
	{
		this->tar_x = tx;
		this->tar_y = ty;
	}
	void Duplicate()
	{
		Dupl = true;
		Last_Collision_Index++;
	}
	void CheckPos()
	{

		
		if ((pos_x > player.kvadr.getPosition().x - cameraS_x / 2) && (pos_x < player.kvadr.getPosition().x + cameraS_x / 2) )
		{
			if (pos_x < player.kvadr.getPosition().x) pos_x -= cameraS_x / 2;
			else pos_x += cameraS_x / 2 + 30;
			if (pos_x > border_east)
			{
				
				pos_x = rand() % border_east;
				CheckPos();
				return;
			}
			if (pos_x < border_west)
			{
				
				pos_x = rand() % border_east;
				CheckPos();
				return;
			}
		}
	}
	



};


vector <Enemy> y; //массив врагов


class Food
{
private:
	
public:
	int index;
	bool DelRequest = false;
	Sprite fooder;
	FloatRect bounds;

	Food(int ind)
	{
		
		this->index = ind;
		srand(time(0) + rand());
		Vector2f pos(rand() % border_east, rand() % border_south);
		if ((rand() % 100) % 2 == 0)
		{
			fooder.setTexture(meat_text);
													//размещение еды, наложение текстуры, определение коллизии
		}
		else
		{
			fooder.setTexture(grass_text);
		}
		fooder.setPosition(pos);
		float x = fooder.getPosition().x + 7;
		float y = fooder.getPosition().y + 4;
		bounds = fooder.getGlobalBounds();
		//this->bounds
	}

	~Food()
	{
		
	}

	void Intersect()
	{
	//	cout << "the food:" << endl << this->bounds.left << endl << this->bounds.top << endl << this->bounds.width << endl << this->bounds.height << endl;
		for (int b = 0; b <= Last_Collision_Index; b++)
		{
			if (this->bounds.intersects(Collision[b]))
			{
				cout << "Collision Event!" << endl;
				DelRequest = true;
				if (b == 0) player.IncScore();
				else { y[b - 1].IncScore(); cout << "Enemy-Food collision event!" << endl; }
			}
	//	cout << "the b:" << endl << b.left << endl << b.top << endl << b.width << endl << b.height << endl;
	}
	}
};

 
vector <Food> x;  //массив еды


int main() 
{
	
	
	
	window.setFramerateLimit(60);
	int frame = 1;
	
	shape.setFillColor(Color::Green);
	img.loadFromFile("noname.png");
	texture.loadFromImage(img);
	
	meat_text.loadFromFile("meat.png");			//загрузка текстур
	grass_text.loadFromFile("grass.png");
	enemy_text.loadFromFile("enemy.png");
	

	int Food_LastInd = 0;
	

	
	
	for (int o = 0; o < FoodAtField; o++)
	{															
		x.emplace_back(Food(o));				//создание еды
		Food_LastInd++;

	}
														
	Collision.emplace_back(player.bounds);
	
	for (int q = 0; q < EnemyAtField; q++)
	{
		y.emplace_back(Enemy(q+1));			//создание врагов
		Last_Collision_Index++;				
	}







	
	

	


	
	while (window.isOpen())

	{
		
		player.Anim();						//инициализация игрока
		



		for (int opi = 0; opi < EnemyAtField; opi++) //
		{
			y[opi].Anim();
			y[opi].Move();

			if (y[opi].Dupl)
			{
				y.emplace_back(Enemy(Last_Collision_Index + 1));
				EnemyAtField++;
				y[opi].Dupl = false;
			}
		}
		
		for (int goodFood = 0; goodFood < FoodAtField; goodFood++)
		{
			if (x[goodFood].DelRequest)
			{
				
				x.erase(x.begin() + x[goodFood].index);
			}
		}




		
		
		Event event;

		while (window.pollEvent(event))
		{
			
			if (event.type == Event::Closed)
			{
				window.close();
			}
			
			
		}

		player.Moving();		//движение
		
		
		window.clear();
		window.draw(player.kvadr);
		window.setView(camera);
		
		
		for (Food good : x)
		{
			good.Intersect();
			window.draw(good.fooder);
		}

		for (Enemy bad : y)
		{
			window.draw(bad.enemy);
		}


		

		window.display();
	}

	return 0;
}


