#include <iostream>
#include <math.h>
#include <time.h>

class Point {
public:
	int x;
	int y;

	void print() {
		std::cout << "x:" << x << " y:" << y << std::endl;
	}
};

int collision(Point a, Point b) {
	int result = 0;
	int x_Distance = b.x - a.x;
	int y_Distance = b.y - a.y;

	// Now that we have the distance (Length)
	// between point a and b
	// Use Pythagoras theorem to get the distance between
	// the Point a and Point b 
	result = sqrt(pow(x_Distance, 2) + pow(y_Distance, 2));

	return result == 0 ? true : false;
}


int main() {

	srand(time(NULL));

	int player_x_coordinate = rand() % 10 + 1;
	int player_y_coordinate = rand() % 10 + 1;

	Point one;
	one.x = player_x_coordinate;
	one.y = player_y_coordinate;

	int npc_x_coordinate = rand() % 10 + 1;
	int npc_y_coordinate = rand() % 10 + 1;

	Point two;
	two.x = npc_x_coordinate;
	two.y = npc_y_coordinate;

	while (true) {
		if (collision(one, two))
		{
			std::cout << "Collision" << std::endl;
			one.print();
			two.print();
			std::cin.get();
		}
		else
		{
			std::cout << "No Collision" << std::endl;
			one.print();
			two.print();
		}

		player_x_coordinate = rand() % 10 + 1;
		player_y_coordinate = rand() % 10 + 1;

		one.x = player_x_coordinate;
		one.y = player_y_coordinate;

		npc_x_coordinate = rand() % 10 + 1;
		npc_y_coordinate = rand() % 10 + 1;

		two.x = npc_x_coordinate;
		two.y = npc_y_coordinate;
	}

	std::cin.get();

	return 0;
}