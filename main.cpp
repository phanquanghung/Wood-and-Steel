#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <string>
#include <iostream>
#include <time.h>
#include "card.h"
#include <vector>
#include <cmath>
#include <algorithm>


using namespace sf;

int size = 80;
Vector2f offset(6, 30);
Sprite f[9]; //figures
std::vector <std::vector<int> > board(5, std::vector<int>(13));
int board0[5][13] =
{ 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0,
  3, 0,-2, 0, 0, 0, 0, 0, 0, 0,-2, 0,-3,
  0, 0, 0, 0, 0, 0,-2, 0, 0, 0, 0, 0, 0,
  3, 0,-2, 0, 0, 0, 0, 0, 0, 0,-2, 0,-3,
  0, 3, 0, 3, 2, 3, 0, 3, 2, 3, 0, 3, 0 };

const double tan_edge = 1.612903225806452; // canh cheo top-left
// a = 58.83026432033091 b = 63

/*
std::string toChessNote(Vector2f p)
{
	std::string s = "";
	s += char(p.x / size + 97);
	s += char(p.y / size + 49);
	return s;
}

Vector2f toCoord(char a, char b)
{
	float x = (int(a) - 97) * size;
	float y = (int(b) - 49) * size;
	return Vector2f(x, y);
}

void move(std::string str)
{
	Vector2f oldPos = toCoord(str[0], str[1]);
	Vector2f newPos = toCoord(str[2], str[3]);

	for (int i = 0; i < 6; i++)
		if (f[i].getPosition() == newPos) f[i].setPosition(-100, -100);

	for (int i = 0; i < 6; i++)
		if (f[i].getPosition() == oldPos) f[i].setPosition(newPos);

}

*/

void loadPosition(Sprite cards[], std::vector <std::vector<int> > &boardCheck, std::vector <std::vector<float> > &boardPoint, Sprite sPoint[], Sprite heads[])
{
	int count = 0;
	for (int m = 0; m < 5; ++m)
	{
		for (int n = 0; n < 13; ++n)
		{
			if (board0[m][n] == 3) continue;
			if (n % 2 == 0) {
				boardPoint[count][0] = (n / 2) * (123 + 61);
				boardPoint[count][1] = m * 100 - 25;
				count++;
			}
			else {
				boardPoint[count][0] = ((n - 1) / 2) * (123 + 61) + 94; //67.264973081;
				boardPoint[count][1] = m * 100 + 25;
				count++;
			}
		}
	}

	int k = 0, so = 0, j = 12, rectx, recty;

	while (so < 5)
	{
		rectx = (j / 2) * (123 + 61);
		recty = so * 100;
		f[k].setPosition(rectx, recty - 25);
		boardCheck[k][0] = so;
		boardCheck[k][1] = 12;
		f[k + 3].setPosition(0, recty - 25);
		boardCheck[k+3][0] = so;
		boardCheck[k+3][1] = 0;
		so += 2;
		k++;
	}

	heads[0].setPosition(80, 460);
	cards[0].setPosition(80, 510);

	for (int i = 1; i < 6; ++i)
	{
		cards[i].setPosition(80 + (184)*i, 460);
		heads[i].setPosition(-100, -100);
	}
	for (int i = 0; i < 57; ++i)
		sPoint[i].setPosition(-100, -100);
}
/*
void checkGoc(double rectx, double recty, Vector2i pos)
{
	double x = pos.x - rectx;
	double y = pos.y - recty;

	if (y < 0 || y > 100) std::cout << "do y ";
	if (x < 0 || x > 125) std::cout << "do x " << pos.x << ' ' << x;

	if (y < 50 - (tan_edge * x) ) std::cout << "\n goc 1 " << y << " va " << (tan_edge * x);
	if (y > tan_edge * x + 100 / 2) std::cout << " goc 2 ";

	if (y < tan_edge * x - 100 * 3 / 2) std::cout << " goc 3 ";
	if (y > -tan_edge * x + 100 * 5 / 2) std::cout << " goc 4 ";

}
*/

void swap(int &a, int &b)
{
	int thay = a;
	a = b;
	b = thay;
}

bool MouseInside(int rectx, int recty, Vector2i pos)
{
	int x = pos.x - rectx;
	int y = pos.y - recty;

	if (y < 0 || y > 100) return 0;
	if (x < 0 || x > 125) return 0;

	if (y < -tan_edge * x + 100 / 2) return 0;
	if (y > tan_edge * x + 100 / 2) return 0;

	if (y < tan_edge * x - 100 * 3 / 2) return 0;
	if (y > -tan_edge * x + 100 * 5 / 2) return 0;

	return 1;
}

Vector2i HandleMouse(Vector2i pos)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			double rectx, recty;
			if (j % 2 == 0) {
				rectx = (j / 2) * (123 + 61);
				recty = i * 100;

				if (MouseInside(rectx, recty, pos)) return Vector2i(i,j); 
//				else if (i == 0 && j == 12) checkGoc(rectx, recty, pos);
			}
			else {
				rectx = ((j - 1) / 2) * (123 + 61) + 94; //67.264973081;
				recty = i * 100 + 50;

				if (MouseInside(rectx, recty, pos)) return Vector2i(i, j);

			}
		}
	}
}

Vector2i selectPlayer(std::vector <std::vector<int> > boardCheck, Vector2i pos)
{
	Vector2i ij = HandleMouse(pos);
	for (int i = 0; i < 6; ++i)
	{
		if (boardCheck[i][0] == ij.x && boardCheck[i][1] == ij.y) {
//			std::cout << i;
			return Vector2i(ij.x, ij.y);
		}
			
	}
	return Vector2i(-1, -1);
}

int distanceCount(Vector2i pos1, Vector2i pos2)
{
	swap(pos1.x, pos1.y);
	swap(pos2.x, pos2.y);
	pos1.x++;
	pos2.x++;
	int du = pos2.x - pos1.x;
	int	dv = (pos2.y + (pos2.x /2) ) - (pos1.y + (pos1.x / 2) );
	if ((du >= 0 and dv >= 0) || (du < 0 and dv < 0))
		return std::max(abs(du), abs(dv));
	else return abs(du) + abs(dv);
}

void renderMelee(Vector2i player, std::vector <std::vector<float> > boardPoint, std::vector <std::vector<int> > board, Sprite sPoint[], std::vector <std::vector<int> > &possibleMove, std::vector <std::vector<int> > boardCheck, int buoc)
{
	int k = 0;
	Vector2i pos2;
	for (int i = 0; i < 57; ++i)
	{
		sPoint[i].setPosition(-100, -100);
		possibleMove[i][0] = -1;
		possibleMove[i][1] = -1;
	}
	int id = 0;
	while (id < 6)
	{
		if (boardCheck[id][0] == player.x && boardCheck[id][1] == player.y) break;
		id++;
	}
	if (buoc == 1) buoc = 2; else buoc = 1;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (board[i][j] == 3) continue;
//			else if (id < 3 && board[i][j] == -1) continue;
//			else if (id > 2 && board[i][j] == 1) continue;
			else if (distanceCount(player, Vector2i(i, j)) == buoc && ((id < 3 && board[i][j] != -1) || (id > 2 && board[i][j] != 1)) && board[i][j] != 2 && board[i][j] != -2 )
			{
				sPoint[k].setPosition(boardPoint[k][0], boardPoint[k][1]);
				possibleMove[k][0] = i;
				possibleMove[k][1] = j;
			}
			++k;
		}
	}
}

int arrayToNo(int so1, int so2)
{
	int so = 0;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 13; j++)
		{
			if (board0[i][j] == 3) continue;
			if (i == so1 && j == so2)
				return so;
			so++;
		}
}

void playerMove(Vector2i player, std::vector <std::vector<int> > possibleMove, Vector2i pos, std::vector <std::vector<float> > boardPoint, std::vector <std::vector<int> > &boardCheck, bool &moved, Sprite sPoint[], bool &playerSelected)
{
	int j = 0, dich=0;
	while (j<6)
	{
		if (boardCheck[j][0] == player.x && boardCheck[j][1] == player.y) break;	
		j++;
	}
	
	for (int i = 0; i < 57; i++)
	{
		if (HandleMouse(pos).x == possibleMove[i][0] && HandleMouse(pos).y == possibleMove[i][1])
		{
			moved = true;
			playerSelected = false;
			int so = arrayToNo(possibleMove[i][0], possibleMove[i][1]);
			board[player.x][player.y] = board0[player.x][player.y]; // Vi tri vua roi khoi se tro ve nhu moi
			f[j].setPosition(boardPoint[so][0], boardPoint[so][1]);

			if (j < 3) // Wood
			{
				if (board[possibleMove[i][0]][possibleMove[i][1]] == 1)
				{
//					std::cout << "giet steel";
					while (dich < 6)
					{
						if (boardCheck[dich][0] == possibleMove[i][0] && boardCheck[dich][1] == possibleMove[i][1]) break;
						++dich;
					}
					boardCheck[dich][0] = -2;
					boardCheck[dich][0] = -2;
					f[dich].setPosition(-100, -100);
					board[possibleMove[i][0]][possibleMove[i][1]] = -1;
				}
				else if (board[possibleMove[i][0]][possibleMove[i][1]] == -2 || board[possibleMove[i][0]][possibleMove[i][1]] == 2) // moi truong
				{
					board[possibleMove[i][0]][possibleMove[i][1]] = -4;
				}
				else board[possibleMove[i][0]][possibleMove[i][1]] = -1; // di chuyen thuong
			}
			else { // Steel
				if (board[possibleMove[i][0]][possibleMove[i][1]] == -1)
				{
//					std::cout << "giet wood";
					while (dich < 6)
					{
						if (boardCheck[dich][0] == possibleMove[i][0] && boardCheck[dich][1] == possibleMove[i][1]) break;
						++dich;
					}
					boardCheck[dich][0] = -2;
					boardCheck[dich][0] = -2;
					f[dich].setPosition(-100, -100);
				}
				board[possibleMove[i][0]][possibleMove[i][1]] = 1;
			}
			boardCheck[j][0] = possibleMove[i][0];
			boardCheck[j][1] = possibleMove[i][1];
			for (int k = 0; k < 57; ++k)
				sPoint[k].setPosition(-100, -100);
		}
	}
}

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

void checkWin(std::vector <std::vector<int> > board, bool &won, Music &music)
{
	if (board[1][12] == 1 || board[3][12] == 1)
	{
		music.openFromFile("Assets/Music/Mizuiro.wav");
		music.setLoop(true);
		music.play();
		std::string Title = "   --- STEEL WINS ---";
		gotoxy(0, 0);
		std::cout << Title;
		won = true;
	}
	int mang = 0;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 13; j++)
		{
			if (board[i][j] == 1) mang++;
		}
	if (mang == 0) {
		music.openFromFile("Assets/Music/Mizuiro.wav");
		music.setLoop(true);
		music.play();
		std::string Title = "   --- WOOD WINS ---";
		gotoxy(0, 0);
		std::cout << Title;
		won = true;
	}
}

int selectCard(Vector2i pos, Sprite cards[])
{
	for (int i = 0; i < 6; ++i)
	{
		if (cards[i].getGlobalBounds().contains(pos.x, pos.y))
		{
			return i+1;
		}
	}
}

int main()
{
	RenderWindow window(VideoMode(1240, 800), "Wood And Steel", Style::Close);
	sf::Music music;
	music.openFromFile("Assets/Music/Soundtrack.wav");
	music.setLoop(true);
	music.play();

	bool won = false, woodFirst = false;
	std::vector <std::vector<int> > boardCheck(6, std::vector<int>(2, -1)); // Luu vi tri cua quan co
	std::vector <std::vector<float> > boardPoint(57, std::vector<float>(2, 0)); // Luu vi tri cua point pixel
	std::vector <std::vector<int> > possibleMove(57, std::vector<int>(2, -1)); // Luu vi tri di duoc
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 13; j++)
		{
			board[i][j] = board0[i][j]; // Map vector
		}
	int spawn = 0;
	while (spawn < 5)
	{
		board[spawn][0] = 1;
		board[spawn][12] = -1;
		spawn += 2;
	}
	Sprite heads[6]; //cards
	Sprite cards[6]; //cards
	Sprite sPoint[57];
	Sprite winBackgr;
	Texture t1, t2, t3, t4, t5, t6, t7, t8;
	t1.loadFromFile("Assets/Play/card.png");
	t2.loadFromFile("Assets/Play/map.png");
	t3.loadFromFile("Assets/Play/steel.png");
	t4.loadFromFile("Assets/Play/wood.png");
	t5.loadFromFile("Assets/Play/point.png");
	t6.loadFromFile("Assets/Play/head.png");
	t7.loadFromFile("Assets/Play/card0.png");
	t8.loadFromFile("Assets/Backgrounds/wallpaper.jpg");
	for (int i = 0; i < 3; i++) f[i].setTexture(t4);
	for (int i = 3; i < 9; i++) f[i].setTexture(t3);
	cards[0].setTexture(t7);
	for (int i = 1; i < 6; i++) cards[i].setTexture(t1);
	for (int i = 0; i < 6; i++) heads[i].setTexture(t6);
	for (int i = 0; i < 57; i++) sPoint[i].setTexture(t5);
	winBackgr.setTexture(t8);
	Sprite sBoard(t2);
	sBoard.setPosition(offset);

	Clock clock;
	int clockCount = 0;
	std::string pngString = ".png";
	loadPosition(cards, boardCheck,boardPoint, sPoint, heads);

//	float dx = 0, dy = 0;
//	Vector2f oldPos = f[0].getPosition(), newPos = f[0].getPosition();
//	std::string str = "";
//	int n = 0;
	
	Vector2i player;
	int buoc = 0;
	bool playerSelected = false, cardSelected = false, moved = false;
	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window) - Vector2i(offset);
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
			if (e.type == Event::MouseButtonPressed)
			{
				if (e.key.code == Mouse::Left)
				{
					if (selectCard(pos, cards) == 1)
					{
						buoc = selectCard(pos, cards);
						cardSelected = true;
					}
					
					//						std::cout << HandleMouse(pos).x << ' ' << HandleMouse(pos).y << std::endl;
					else if (playerSelected)
					{
						playerMove(player, possibleMove, pos, boardPoint, boardCheck, moved, sPoint, playerSelected);
						if (buoc = 1) buoc = 0;
					}

					if (!moved && selectPlayer(boardCheck, pos) != Vector2i(-1, -1))
					{
						player = selectPlayer(boardCheck, pos);
						playerSelected = true;
						renderMelee(player, boardPoint, board, sPoint, possibleMove, boardCheck, buoc);
						

					}
/*					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 13; j++)
						{
							std::cout << board[i][j] << ' '; // Map vector
						}
						std::cout << std::endl;
					}
					std::cout << std::endl;
*/
				}
			}
		}
		moved = false;
		
		if (clock.getElapsedTime().asSeconds() > 0.15f) {		
			t3.loadFromFile("Assets/Play/steel" + std::to_string(clockCount) + pngString);
			t4.loadFromFile("Assets/Play/wood" + std::to_string(clockCount) + pngString);
			if (clockCount == 17) clockCount = 0;
			else clockCount++;
			clock.restart();
		}
		
		if (!won) checkWin(board, won, music);

		/*			/////drag and drop///////
					if (e.type == Event::MouseButtonPressed)
						if (e.key.code == Mouse::Left)
							for (int i = 0; i < 6; i++)
								if (f[i].getGlobalBounds().contains(pos.x, pos.y))
								{
									isMove = true; n = i;
									dx = pos.x - f[i].getPosition().x;
									dy = pos.y - f[i].getPosition().y;
									oldPos = f[i].getPosition();
								}

					if (e.type == Event::MouseButtonReleased)
						if (e.key.code == Mouse::Left)
						{
							isMove = false;
							Vector2f p = f[n].getPosition() + Vector2f(size / 2, size / 2);
							newPos = Vector2f(size*int(p.x / size), size*int(p.y / size));
							str = toChessNote(oldPos) + toChessNote(newPos);
							move(str);
							std::cout << str << std::endl;
							f[n].setPosition(newPos);
						}

				}

				if (isMove) f[n].setPosition(pos.x - dx, pos.y - dy);
		*/
		////// draw  ///////		
		window.clear();
		window.draw(sBoard);
		for (int i = 0; i < 6; i++) f[i].move(offset);
		for (int i = 0; i < 6; i++) window.draw(f[i]); // window.draw(f[n]);
		for (int i = 0; i < 6; i++) f[i].move(-offset);

		for (int i = 0; i < 6; i++) heads[i].move(offset);
		for (int i = 0; i < 6; i++) window.draw(heads[i]);
		for (int i = 0; i < 6; i++) heads[i].move(-offset);

		for (int i = 0; i < 6; i++) cards[i].move(offset);
		for (int i = 0; i < 6; i++) window.draw(cards[i]);
		for (int i = 0; i < 6; i++) cards[i].move(-offset);

		for (int i = 0; i < 57; i++) sPoint[i].move(offset);
		for (int i = 0; i < 57; i++) window.draw(sPoint[i]);
		for (int i = 0; i < 57; i++) sPoint[i].move(-offset);
		if (won) {
			winBackgr.setPosition(0, 0);
			window.draw(winBackgr);
		}
		window.display();
	
	}
	return 0;
}

// sprite.setScale({ -1, 1 });