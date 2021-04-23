#pragma once
#include <vector>
#include <stdlib.h>

class Snake {
private:
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> snake{ std::make_pair(
		std::make_pair(13, 13),
		std::make_pair(14, 14)) 
	};
	std::pair<int, int> direction;
	std::pair<int, int> pDirection;
	std::pair<std::pair<int, int>, std::pair<int, int>> apple;
public:
	int setD(int x, int y);
	void setPD() { pDirection == std::make_pair(0, 0); }
	void move();
	bool death = false;
	void bDeath();
	void sDeath();
	void setApple();
	void appleProcedure();
	void CheckApple();
	void INIT();
	void INIT_snake();
	int sizeS() { return snake.size();  };
	std::pair<std::pair<int, int>, std::pair<int, int>> getS(int i) { return snake[i]; }
	std::pair<std::pair<int, int>, std::pair<int, int>> getA() { return apple; }
};

int Snake::setD(int x, int y) {
	if (x != 0 || y != 0) {
		if (direction.first == 0 && pDirection.first == 0) {
			switch (x) {
			case 1:
				direction.first = 1;
				direction.second = 0;
				return 0;
			case -1:
				direction.first = -1;
				direction.second = 0;
				return 0;
			}
		}
		if (direction.second == 0 && pDirection.second == 0) {
			switch (y) {
			case 1:
				direction.first = 0;
				direction.second = 1;
				return 0;
			case -1:
				direction.first = 0;
				direction.second = -1;
				return 0;
			}
		}
	}
	else { direction.first = 0; direction.second = 0; }

	return -1;
}

void Snake::move() {
		snake.insert(snake.begin(), std::make_pair(
			std::make_pair(snake[0].first.first + direction.first, snake[0].first.second + direction.second),
			std::make_pair(snake[0].second.first + direction.first, snake[0].second.second + direction.second))
		);
		snake.resize(snake.size() - 1);

		pDirection = direction;
	
}

void Snake::bDeath() {
	if (direction.first != 0) {
		switch (direction.first) {
		case 1:
			if (snake[0].second.first == 25) {
				death = true;
			}
			break;
		case -1:
			if (snake[0].first.first == 0) {
				death = true;
			}
			break;
		}
	}
	else if (direction.second != 0) {
		switch (direction.second) {
		case 1:
			if (snake[0].second.second == 25) {
				death = true;
			}
			break;
		case -1:
			if (snake[0].first.second == 0) {
				death = true;
			}
			break;
		}
	}
}

void Snake::sDeath() {
	if (direction.first != 0) {
		switch (direction.first) {
		case 1:
			for (int i = 1; i < snake.size(); i++) {
				if (snake[0].second.first == snake[i].first.first) {
					if (snake[0].first.second == snake[i].first.second) { death = true; }
				}
			}
			break;
		case -1:
			for (int i = 1; i < snake.size(); i++) {
				if (snake[0].first.first == snake[i].second.first) {
					if (snake[0].first.second == snake[i].first.second) { death = true; }
				}
			}
			break;
		}
	}
	else if (direction.second != 0) {
		switch (direction.second) {
		case 1:
			for (int i = 1; i < snake.size(); i++) {
				if (snake[0].second.second == snake[i].first.second) {
					if (snake[0].first.first == snake[i].first.first) { death = true; }
				}
			}
			break;
		case -1:
			for (int i = 1; i < snake.size(); i++) {
				if (snake[0].first.second == snake[i].second.second) {
					if (snake[0].first.first == snake[i].first.first) { death = true; }
				}
			}
			break;
		}
	}
}

void Snake::setApple() {
	apple.first.first = rand() % 24;
	apple.first.second = rand() % 24;
	apple.second.first = apple.first.first + 1;
	apple.second.second = apple.first.second + 1;

	for (int i = 0; i < snake.size(); i++) {
		if (snake[i].first.first == apple.first.first && snake[i].first.second == apple.first.second) {
			setApple();
		}
	}
}

void Snake::appleProcedure() {

	if (sizeS() == 1) {
		snake.resize(3);
		snake.push_back(std::make_pair(std::make_pair
		(snake[0].first.first + direction.first, snake[0].first.second + direction.second),
			std::make_pair(snake[0].second.first + direction.first, snake[0].second.second + direction.second))
		);
		snake.push_back(std::make_pair(std::make_pair
		(snake[1].first.first + direction.first, snake[1].first.second + direction.second),
			std::make_pair(snake[1].second.first + direction.first, snake[1].second.second + direction.second))
		);
		snake.push_back(std::make_pair(std::make_pair
		(snake[2].first.first + direction.first, snake[2].first.second + direction.second),
			std::make_pair(snake[2].second.first + direction.first, snake[2].second.second + direction.second))
		);
	}
	else {
		std::pair<int, int> dir = std::make_pair(snake[snake.size() - 1].first.first - snake[snake.size() - 2].first.first,
			snake[snake.size() - 1].first.second - snake[snake.size() - 2].first.second);

		snake.push_back(std::make_pair(std::make_pair
		(snake[snake.size() - 1].first.first + dir.first, snake[snake.size() - 1].first.second + dir.second),
			std::make_pair(snake[snake.size() - 1].second.first + dir.first, snake[snake.size() - 1].second.second + dir.second))
		);
		snake.push_back(std::make_pair(std::make_pair
		(snake[snake.size() - 1].first.first + dir.first, snake[snake.size() - 1].first.second + dir.second),
			std::make_pair(snake[snake.size() - 1].second.first + dir.first, snake[snake.size() - 1].second.second + dir.second))
		);
		snake.push_back(std::make_pair(std::make_pair
		(snake[snake.size() - 1].first.first + dir.first, snake[snake.size() - 1].first.second + dir.second),
			std::make_pair(snake[snake.size() - 1].second.first + dir.first, snake[snake.size() - 1].second.second + dir.second))
		);
	}
};

void Snake::CheckApple() {
	if (snake[0].first.first == apple.first.first && snake[0].first.second == apple.first.second) {
		appleProcedure();
		setApple();
	}
};

void Snake::INIT() {
	INIT_snake();
	setApple();
	setD(0,0);
	setPD();
	death = false;
}

void Snake::INIT_snake() {
	snake.resize(0);
	snake.push_back(std::make_pair(
		std::make_pair(13, 13),
		std::make_pair(14, 14))
	);
}