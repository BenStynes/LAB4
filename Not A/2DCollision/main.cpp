#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <GameObject.h>
#include <Player.h>
#include <NPC.h>
#include <Input.h>
#include <Debug.h>

using namespace std;

int main()
{
	enum Shapes
	{
		Box,
		Circle,
		Ray

	};

	Shapes play = Box;
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
	bool drawColision = false;
	// Load a NPC's sprites to display
	sf::Texture npc_texture;
	if (!npc_texture.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture player_texture;
	if (!player_texture.loadFromFile("assets\\player.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}
	float yoMamma = 50;
	// Setup NPC's Default Animated Sprite
	AnimatedSprite npc_animated_sprite(npc_texture);
	npc_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	npc_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup Players Default Animated Sprite
	AnimatedSprite player_animated_sprite(player_texture);
	player_animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	player_animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	// Setup the NPC
	GameObject &npc = NPC(npc_animated_sprite);

	// Setup the Player
	GameObject &player = Player(player_animated_sprite);

	//Setup NPC AABB
	c2AABB aabb_npc;
	aabb_npc.min = c2V(npc.getAnimatedSprite().getPosition().x, npc.getAnimatedSprite().getPosition().y);
	aabb_npc.max = c2V(
		npc.getAnimatedSprite().getPosition().x +
		npc.getAnimatedSprite().getGlobalBounds().width, 
		npc.getAnimatedSprite().getPosition().y +
		npc.getAnimatedSprite().getGlobalBounds().height);
	sf::RectangleShape npc_box;
	c2Circle player_circle;
	sf::CircleShape playerCircle;
	player_circle.r = yoMamma;
	playerCircle.setRadius(yoMamma);
	npc_box.setOutlineThickness(2);
	npc_box.setOutlineColor(sf::Color::Green);
	npc_box.setSize(sf::Vector2f{ 84,84 });
	npc_box.setFillColor(sf::Color{ 0,0,0,0 });
	//setup capsule

	sf::CircleShape top;
	sf::CircleShape bottom;
	sf::RectangleShape body;
	top.setFillColor(sf::Color::White);
	bottom.setFillColor(sf::Color::White);
	body.setFillColor(sf::Color::White);
	top.setRadius(30);
	bottom.setRadius(30);
	body.setSize(sf::Vector2f{ 60,30 });
	top.setPosition(200, 180);
	body.setPosition(200, 210);
	bottom.setPosition(200, 200);
	
	//Poly Setup, want a cracker?

	sf::VertexArray pointless{ sf::Triangles ,3};
	pointless.append({ sf::Vector2f{ 400,300 },sf::Color::White });
	pointless.append({ sf::Vector2f{ 300,200 },sf::Color::White });
	pointless.append({ sf::Vector2f{ 400,400 },sf::Color::White });
	c2Poly poly_npc;
	
	poly_npc.count = 3;
	poly_npc.verts[0] = c2v{ 400,300 };
	poly_npc.verts[1] = c2v{ 300,200 };
	poly_npc.verts[2] = c2v{ 400,400 };
	c2MakePoly(&poly_npc);
	//capsule setip 2
	c2Capsule capsule_npc;
	capsule_npc.a.x = top.getPosition().x + top.getRadius();
	capsule_npc.a.y = top.getPosition().y + top.getRadius();

	capsule_npc.b.x = bottom.getPosition().x + bottom.getRadius();
	capsule_npc.b.y = bottom.getPosition().y + bottom.getRadius();

	capsule_npc.r = 30;

	//ray setup;
	sf::Vector2f dv{ sf::Vector2f{ 420,300 } - sf::Vector2f{ 100,300 }};
	float magnitude{ sqrt(dv.x * dv.x + dv.y * dv.y) };
	sf::VertexArray pointless2{ sf::Lines ,2 };
	pointless2.append({ sf::Vector2f{ 100,300 },sf::Color::White });
	pointless2.append({ sf::Vector2f{ 420,300 },sf::Color::White });
	sf::Vector2f unit{ dv / magnitude };
	c2Ray raymond;
	raymond.p = c2v{ 100,300 };
	raymond.d = { unit.x,unit.y};
	raymond.t = {  magnitude };

	c2Raycast re;
	//Setup Player AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(player.getAnimatedSprite().getPosition().x, player.getAnimatedSprite().getPosition().y);
	aabb_player.max = c2V(player.getAnimatedSprite().getGlobalBounds().width / 6, player.getAnimatedSprite().getGlobalBounds().width / 6);
	sf::RectangleShape player_box;
	player_box.setOutlineThickness(2);
	player_box.setOutlineColor(sf::Color::Green);
	player_box.setSize(sf::Vector2f{ 84,84 });
	player_box.setFillColor(sf::Color{ 0,0,0,0 });
	
	c2Circle npc_circle;
	sf::CircleShape npcCircle;
	npc_circle.r = yoMamma;
	npcCircle.setRadius(yoMamma);
	npcCircle.setPosition(200, 400);
	
	sf::Vector2f dv2{ sf::Vector2f{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) } -sf::Vector2f{ 400,300 } };
	float magnitude2{ sqrt(dv2.x * dv2.x + dv2.y * dv2.y) };
	sf::VertexArray pointless3{ sf::Lines ,2 };
	pointless3.append({ sf::Vector2f{ 400,300 },sf::Color::White });
	pointless3.append({ sf::Vector2f{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) },sf::Color::White });
	sf::Vector2f unit2{ dv2 / magnitude2 };
	c2Ray raymond2;
	raymond2.p = c2v{ 400,300 };
	raymond2.d = { unit.x,unit.y };
	raymond2.t = { magnitude };
	// Initialize Input
	Input input;

	// Collision result
	int result = 0;

	// Direction of movement of NPC
	sf::Vector2f direction(0.1f, 0.2f);
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		player.getAnimatedSprite().setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		player_box.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		playerCircle.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		pointless3.clear();
		pointless3.append({ sf::Vector2f{ 600,300 },sf::Color::White });
		pointless3.append({ sf::Vector2f{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) },sf::Color::White });
		dv2 = { sf::Vector2f{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) } -sf::Vector2f{ 600,300 } };
		magnitude2 = { sqrt(dv2.x * dv2.x + dv2.y * dv2.y) };
		unit2 = { dv2 / magnitude2 };
		raymond2.p = c2v{ 600,300 };
		raymond2.d = { unit2.x,unit2.y };
		raymond2.t = { magnitude2 };
		// Move The NPC
		sf::Vector2f move_to(npc.getAnimatedSprite().getPosition().x + direction.x, npc.getAnimatedSprite().getPosition().y + direction.y);
		npc_box.setPosition(npc.getAnimatedSprite().getPosition());
		if (move_to.x < 0) {
			direction.x *= -1;
			move_to.x = 0 + npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.x + npc.getAnimatedSprite().getGlobalBounds().width >= 800) { 
			direction.x *= -1;
			move_to.x = 800 - npc.getAnimatedSprite().getGlobalBounds().width;
		}
		else if (move_to.y < 0) { 
			direction.y *= -1;
			move_to.y = 0 + npc.getAnimatedSprite().getGlobalBounds().height;
		}
		else if (move_to.y + npc.getAnimatedSprite().getGlobalBounds().height >= 600) {
			direction.y *= -1;
			move_to.y = 600 - npc.getAnimatedSprite().getGlobalBounds().height;
		}
		
		npc.getAnimatedSprite().setPosition(move_to);

		// Update NPC AABB set x and y
		aabb_npc.min = c2V(
			npc.getAnimatedSprite().getPosition().x, 
			npc.getAnimatedSprite().getPosition().y
		);

		aabb_npc.max = c2V(
			npc.getAnimatedSprite().getPosition().x +
			npc.getAnimatedSprite().getGlobalBounds().width,
			npc.getAnimatedSprite().getPosition().y +
			npc.getAnimatedSprite().getGlobalBounds().height
		);

		// Update Player AABB
		aabb_player.min = c2V(
			player.getAnimatedSprite().getPosition().x, 
			player.getAnimatedSprite().getPosition().y
		);
		aabb_player.max = c2V(
			player.getAnimatedSprite().getPosition().x +
			player.getAnimatedSprite().getGlobalBounds().width, 
			player.getAnimatedSprite().getPosition().y + 
			player.getAnimatedSprite().getGlobalBounds().height
		);

		player_circle.p.x = playerCircle.getPosition().x + player_circle.r;
		player_circle.p.y = playerCircle.getPosition().y + player_circle.r;
		npc_circle.p.x = npcCircle.getPosition().x + npc_circle.r;
		npc_circle.p.y = npcCircle.getPosition().y + npc_circle.r;
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					input.setCurrent(Input::Action::LEFT);
					play = Circle;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					input.setCurrent(Input::Action::RIGHT);
					play = Ray;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					input.setCurrent(Input::Action::UP);
					play = Box;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
				{
					if (drawColision == true)
					{
						drawColision = false;

					}
					else
					{
						drawColision = true;
					}
				}
				
				break;
			default:
				input.setCurrent(Input::Action::IDLE);
				break;
			}
		}
	
		// Handle input to Player
		player.handleInput(input);

		// Update the Player
		player.update();

		// Update the Player
		npc.update();

		if (play == Box)
		{
			// Check for collisions
			result = c2AABBtoAABB(aabb_player, aabb_npc);


			cout << ((result != 0) ? ("Collision") : "") << endl;



			if (result) {
				player.getAnimatedSprite().setColor(sf::Color(255, 255, 255));

				npc_box.setOutlineColor(sf::Color::Green);
				player_box.setOutlineColor(sf::Color::Green);
			}
			else {
				player.getAnimatedSprite().setColor(sf::Color(100, 100, 100));
				player_box.setOutlineColor(sf::Color::White);
				npc_box.setOutlineColor(sf::Color::White);
			}

			//capsule collison
			if (c2AABBtoCapsule(aabb_player, capsule_npc))
			{
				player.getAnimatedSprite().setColor(sf::Color(255, 255, 255));

				player_box.setOutlineColor(sf::Color::Green);
				top.setFillColor(sf::Color::Red);
				body.setFillColor(sf::Color::Red);
				bottom.setFillColor(sf::Color::Red);
			}

			else
			{

				top.setFillColor(sf::Color::White);
				body.setFillColor(sf::Color::White);
				bottom.setFillColor(sf::Color::White);
			}
			//poly collsioin
			if (c2AABBtoPoly(aabb_player, &poly_npc, NULL))
			{
				player.getAnimatedSprite().setColor(sf::Color(255, 255, 255));

				player_box.setOutlineColor(sf::Color::Green);
				pointless.clear();
				pointless.append({ sf::Vector2f{ 400,300 },sf::Color::Red });
				pointless.append({ sf::Vector2f{ 300,200 },sf::Color::Red });
				pointless.append({ sf::Vector2f{ 400,400 },sf::Color::Red });

			}

			else
			{

				pointless.append({ sf::Vector2f{ 400,300 },sf::Color::White });
				pointless.append({ sf::Vector2f{ 300,200 },sf::Color::White });
				pointless.append({ sf::Vector2f{ 400,400 },sf::Color::White });
			}

			if (c2RaytoAABB(raymond, aabb_player, &re))
			{
				player.getAnimatedSprite().setColor(sf::Color(255, 255, 255));

				player_box.setOutlineColor(sf::Color::Green);
				pointless2.clear();
				pointless2.append({ sf::Vector2f{ 100,300 },sf::Color::Red });
				pointless2.append({ sf::Vector2f{ 420,300 },sf::Color::Red });

			}

			else
			{

				pointless2.append({ sf::Vector2f{ 100,300 },sf::Color::White });
				pointless2.append({ sf::Vector2f{ 420,300 },sf::Color::White });
			}
		}
		if (play == Circle)
		{
			if (c2CircletoAABB(player_circle, aabb_npc))
			{
				playerCircle.setFillColor(sf::Color::Red);
				player.getAnimatedSprite().setColor(sf::Color(255, 255, 255));
			}
			else
			{
				player.getAnimatedSprite().setColor(sf::Color(100, 100, 100));
				playerCircle.setFillColor(sf::Color::White);
			}

			if (c2CircletoCircle(player_circle, npc_circle))
			{
				playerCircle.setFillColor(sf::Color::Red);
				npcCircle.setFillColor(sf::Color::Red);
			}
			else
			{
				npcCircle.setFillColor(sf::Color::White);
			}
			if (c2RaytoCircle(raymond, player_circle, &re))
			{

				pointless2.append({ sf::Vector2f{ 100,300 },sf::Color::Red });
				pointless2.append({ sf::Vector2f{ 420,300 },sf::Color::Red });
				playerCircle.setFillColor(sf::Color::Red);

			}
			else
			{
				pointless2.append({ sf::Vector2f{ 100,300 },sf::Color::White });
				pointless2.append({ sf::Vector2f{ 420,300 },sf::Color::White });
			}
			if (c2CircletoCapsule(player_circle, capsule_npc))
			{
				playerCircle.setFillColor(sf::Color::Red);
				top.setFillColor(sf::Color::Red);
				body.setFillColor(sf::Color::Red);
				bottom.setFillColor(sf::Color::Red);
			}
			else
			{
				top.setFillColor(sf::Color::White);
				body.setFillColor(sf::Color::White);
				bottom.setFillColor(sf::Color::White);
			}
			if (c2CircletoPoly(player_circle, &poly_npc, NULL))
			{
				

				playerCircle.setFillColor(sf::Color::Red);
				pointless.clear();
				pointless.append({ sf::Vector2f{ 400,300 },sf::Color::Red });
				pointless.append({ sf::Vector2f{ 300,200 },sf::Color::Red });
				pointless.append({ sf::Vector2f{ 400,400 },sf::Color::Red });

			}

			else
			{

				pointless.append({ sf::Vector2f{ 400,300 },sf::Color::White });
				pointless.append({ sf::Vector2f{ 300,200 },sf::Color::White });
				pointless.append({ sf::Vector2f{ 400,400 },sf::Color::White });
			}

			
		}

	if (play == Ray)
	{
			if(c2RaytoAABB(raymond2, aabb_npc, &re))
		{
			npc.getAnimatedSprite().setColor(sf::Color(255, 255, 255));

			
			pointless3.clear();
			pointless3.append({ sf::Vector2f{ 600,300 },sf::Color::Red });
			pointless3.append({ sf::Vector2f{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) },sf::Color::Red });

		}

		else
		{
				npc.getAnimatedSprite().setColor(sf::Color(100, 100, 100));
				pointless3.clear();
				pointless3.append({ sf::Vector2f{ 600,300 },sf::Color::White });
				pointless3.append({ sf::Vector2f{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) },sf::Color::White });
		}
		if (c2RaytoCapsule(raymond2, capsule_npc, &re))
			{
				

			top.setFillColor(sf::Color::Red);
			body.setFillColor(sf::Color::Red);
			bottom.setFillColor(sf::Color::Red);
				pointless3.clear();
				pointless3.append({ sf::Vector2f{ 600,300 },sf::Color::Red });
				pointless3.append({ sf::Vector2f{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) },sf::Color::Red });

			}

		else
			{
				
				
				top.setFillColor(sf::Color::White);
				body.setFillColor(sf::Color::White);
				bottom.setFillColor(sf::Color::White);
			}
		if (c2RaytoCircle(raymond2, npc_circle, &re))
		{

			pointless3.clear();
			pointless3.append({ sf::Vector2f{ 600,300 },sf::Color::Red });
			pointless3.append({ sf::Vector2f{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) },sf::Color::Red });
			npcCircle.setFillColor(sf::Color::Red);

		}
		else
		{
			npcCircle.setFillColor(sf::Color::White);
		}
		if (c2RaytoPoly(raymond2, &poly_npc, NULL, &re))
		{
			pointless3.clear();
			pointless3.append({ sf::Vector2f{ 600,300 },sf::Color::Red });
			pointless3.append({ sf::Vector2f{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) },sf::Color::Red });
			pointless.clear();
			pointless.append({ sf::Vector2f{ 400,300 },sf::Color::Red });
			pointless.append({ sf::Vector2f{ 300,200 },sf::Color::Red });
			pointless.append({ sf::Vector2f{ 400,400 },sf::Color::Red });
		}
		else
		{
			pointless.clear();
			pointless.append({ sf::Vector2f{ 400,300 },sf::Color::White });
			pointless.append({ sf::Vector2f{ 300,200 },sf::Color::White });
			pointless.append({ sf::Vector2f{ 400,400 },sf::Color::White });
		}
	}
		// Clear screen
		window.clear();
		if (play == Box)
		{
			if (drawColision == true)
			{
				window.draw(npc_box);
				window.draw(player_box);
			}
			else
			{
				// Draw the Players Current Animated Sprite
				window.draw(player.getAnimatedSprite());

				
			}
		}

		if (play == Circle)
		{
			window.draw(playerCircle);
		}
		if (play == Ray)
		{
			window.draw(pointless3);
		}
		// Draw the NPC's Current Animated Sprite
		window.draw(npc.getAnimatedSprite());
		window.draw(body);
		window.draw(top);
		window.draw(bottom);
		window.draw(npcCircle);

		window.draw(pointless);
		window.draw(pointless2);
		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};