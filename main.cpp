#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <iostream>
#include <time.h>
#include <vector>
#include <cmath>
#include <algorithm>
// #include "card.h"

using namespace sf;

int size = 80;
Vector2f offset(6, 30);
Sprite f[9]; //figures

// Ban do goc
int board0[5][13] =
{ 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0,
  3, 0,-2, 0, 0, 0, 0, 0, 0, 0,-2, 0,-3,
  0, 0, 0, 0, 0, 0,-2, 0, 0, 0, 0, 0, 0,
  3, 0,-2, 0, 0, 0, 0, 0, 0, 0,-2, 0,-3,
  0, 3, 0, 3, 2, 3, 0, 3, 2, 3, 0, 3, 0 };
int phe = 0;

const double tan_edge = 1.612903225806452; // canh cheo top-left
// a = 58.83026432033091 b = 63 // sprite.setScale({ -1, 1 }); de doi chieu anh

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
	return Vector2i(-1, -1);
}

Vector2i selectPlayer(std::vector <std::vector<int> > boardCheck, Vector2i pos, int woodPlay, int steelPlay, bool after)
{
//	std::cout << woodPlay << ' ' << steelPlay << "\n";
	Vector2i ij = HandleMouse(pos);
	for (int i = 0; i < 6; ++i)
	{
		if (boardCheck[i][0] == ij.x && boardCheck[i][1] == ij.y){
			if (!after)
			{
				return Vector2i(ij.x, ij.y);
			} else if ((i<3 && woodPlay>steelPlay) || (i > 2 && steelPlay > woodPlay))
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

void renderMelee(Vector2i player, std::vector <std::vector<float> > boardPoint, std::vector <std::vector<int> > board, Sprite sPoint[], std::vector <std::vector<int> > &possibleMove, std::vector <std::vector<int> > boardCheck, int buoc, std::vector <std::vector<int> > &possibleReso)
{
	int k = 0, l = 0;
//	Vector2i pos2; ???
	for (int i = 0; i < 57; ++i)
	{
		sPoint[i].setPosition(-100, -100);
		possibleMove[i][0] = -1;
		possibleMove[i][1] = -1;
		if (i < 9) {
			possibleReso[i][0] = -1;
			possibleReso[i][1] = -1;
		}
	}
	int id = 0;
	while (id < 6)
	{
		if (boardCheck[id][0] == player.x && boardCheck[id][1] == player.y) break;
		id++;
	}
	if (buoc == 1) buoc++; else buoc = 1;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (board[i][j] == 3) continue;
//			else if (id < 3 && board[i][j] == -1) continue;
//			else if (id > 2 && board[i][j] == 1) continue;
			else if (distanceCount(player, Vector2i(i, j)) == buoc && ((id < 3 && board[i][j] != -1) || (id > 2 && board[i][j] != 1)) && board[i][j] != 2 && board[i][j] != -2 )
			{
				sPoint[k].setColor(Color::Black);
				sPoint[k].setPosition(boardPoint[k][0], boardPoint[k][1]);
				possibleMove[k][0] = i;
				possibleMove[k][1] = j;
			}
			else if (buoc == 1 && distanceCount(player, Vector2i(i, j)) == 1 && (id < 3 && board[i][j] == -2 || id > 2 && board[i][j] == 2))
			{
				sPoint[k].setColor(Color::Red);
				sPoint[k].setPosition(boardPoint[k][0], boardPoint[k][1]);
				possibleReso[l][0] = i;
				possibleReso[l][1] = j;
				l++;
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

void animation(std::vector <std::vector<float> > boardPoint, int newXY, int old, RenderWindow &window, int so, Sprite sBoard)
{
	Vector2f newPos = Vector2f(boardPoint[newXY][0], boardPoint[newXY][1]);
	Vector2f oldPos = Vector2f(boardPoint[old][0], boardPoint[old][1]);
	for (int k = 0; k < 45; k++)
	{
		Vector2f p = newPos - oldPos;
		f[so].move(p.x / 45, p.y / 45);

		window.draw(sBoard);
		for (int i = 0; i < 6; i++) f[i].move(offset);
		for (int i = 0; i < 6; i++) window.draw(f[i]); window.draw(f[so]);
		for (int i = 0; i < 6; i++) f[i].move(-offset);
		window.display();
	}

}

void playerMove(RenderWindow &window, Sprite sBoard, Vector2i player, std::vector <std::vector<int> > possibleMove, Vector2i pos, std::vector <std::vector<float> > boardPoint, std::vector <std::vector<int> > &boardCheck, Sprite sPoint[], bool &playerSelected, int &woodPlay, int &steelPlay, int &j, int &buoc, bool &after, std::vector <std::vector<int> > &board, Sound sound2, std::vector <std::vector<int> > possibleReso, int &resource1, int &resource2)
{
	j = 0;
	int dich=0;
	while (j<6)
	{
		if (boardCheck[j][0] == player.x && boardCheck[j][1] == player.y) break;	
		j++;
	}
	
	for (int i = 0; i < 57; i++)
	{
		if (HandleMouse(pos).x == possibleMove[i][0] && HandleMouse(pos).y == possibleMove[i][1])
		{
			sound2.play();
			playerSelected = false;
			int so = arrayToNo(possibleMove[i][0], possibleMove[i][1]);
			int old = arrayToNo(player.x, player.y);
			board[player.x][player.y] = board0[player.x][player.y]; // Vi tri vua roi khoi se tro ve nhu moi
			animation(boardPoint, so, old, window, j, sBoard);
			f[j].setPosition(boardPoint[so][0], boardPoint[so][1]);
			if (buoc = 1) buoc = 0;
			if (!after) after = true;
			if (j < 3) // Wood
			{
				woodPlay++;
				if (woodPlay == 3) {
					woodPlay = -1;
					steelPlay = 0;
				}
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
				steelPlay++;
				if (steelPlay == 3) {
					woodPlay = 0;
					steelPlay = -1;
				}
				if (board[possibleMove[i][0]][possibleMove[i][1]] == -1)
				{
//					std::cout << "giet wood";
//					resource1++;
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
				int mang = 0;
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < 13; j++)
					{
						if (board[i][j] == -1) mang++;
					}
				}
				if (mang == 0) steelPlay = 4;

			}
			boardCheck[j][0] = possibleMove[i][0];
			boardCheck[j][1] = possibleMove[i][1];
			for (int k = 0; k < 57; ++k)
				sPoint[k].setPosition(-100, -100);
		}
		if (i < 9 && HandleMouse(pos).x == possibleReso[i][0] && HandleMouse(pos).y == possibleReso[i][1])
		{
			sound2.play();
			playerSelected = false;
			if (j < 3) // Wood
			{
				woodPlay++;
				if (woodPlay == 3) {
					woodPlay = -1;
					steelPlay = 0;
				}
				resource2++;
			}
			else { // Steel
				steelPlay++;
				if (steelPlay == 3) {
					woodPlay = 0;
					steelPlay = -1;
				}
				resource1++;
			}
			for (int k = 0; k < 57; ++k)
				sPoint[k].setPosition(-100, -100);
		}
	}
}
/*
void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
*/
void checkWin(std::vector <std::vector<int> > board, bool &won, Music &music)
{
	if (board[1][12] == 1 || board[3][12] == 1)
	{
		music.openFromFile("Assets/Music/Mizuiro.wav");
		music.setLoop(true);
		music.play();
		std::string Title = "STEEL WINS! \n";
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
		std::string Title = "WOOD WINS! \n";
		std::cout << Title;
		won = true;
	}
}

int selectCard(Vector2i pos, Sprite cards[], std::vector <int> &cQuantity)
{
	for (int i = 0; i < 6; ++i)
	{
		if (cards[i].getGlobalBounds().contains(pos.x, pos.y) && cQuantity[i+phe] > 0)
		{
			return i+1;
		}
	}
}

bool posReso(Vector2i pos, int steelPlay, int woodPlay, int resource1, int resource2)
{
	if (woodPlay > steelPlay && resource2 <= 0) return 0;
	else if (steelPlay > woodPlay && resource1 <= 0) return 0;
	int x = pos.x, y = pos.y + 50;
	if (woodPlay > steelPlay) x -= 644;
	else x -= 460;

	if (y > 100) return 0;
	if (x < 0 || x > 123) return 0;
	if (y > tan_edge * x + 100 / 2) return 0;
	if (y > -tan_edge * x + 100 * 5 / 2) return 0;
	return 1;
}

void spendReso(int &resource1, int &resource2, std::vector <int> &cQuantity, int steelPlay, int woodPlay)
{
	cQuantity[0+phe]++;
	if (woodPlay > steelPlay) {
		resource2--;
		//		cQuantity[(rand() % 6) + 6]++;
	}
	else {
		resource1--;
		//		cQuantity[rand() & 6]++;
	}
}

void cancelSelection(Sprite sPoint[], std::vector <std::vector<int> > &possibleMove, int &buoc)
{
	buoc = 0;

	for (int i = 0; i < 57; ++i)
	{
		sPoint[i].setPosition(-100, -100);
		possibleMove[i][0] = -1;
		possibleMove[i][1] = -1;
//		std::cout << i;
	}
}

void PlayGame(RenderWindow &window, Music &music)
{
	SoundBuffer cursor;
	cursor.loadFromFile("Assets/Music/Cursor3.wav");
	Sound sound;
	sound.setBuffer(cursor);

	SoundBuffer cursor2;
	cursor2.loadFromFile("Assets/Music/Cursor1.wav");
	Sound sound2;
	sound2.setBuffer(cursor2);

	bool after = false;
	std::vector <std::vector<int> > board(5, std::vector<int>(13));
	bool won = false;
	std::vector <std::vector<int> > boardCheck(6, std::vector<int>(2, -1)); // Luu vi tri cua quan co
	std::vector <std::vector<float> > boardPoint(57, std::vector<float>(2, 0)); // Luu vi tri cua point pixel
	std::vector <std::vector<int> > possibleMove(57, std::vector<int>(2, -1)); // Luu vi tri di duoc
	std::vector <std::vector<int> > possibleReso(9, std::vector<int>(2, -1)); // Luu vi tri reso
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
	Sprite winBackgr, ban;
	Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
	t1.loadFromFile("Assets/Play/card.png");
	t2.loadFromFile("Assets/Play/map.png");
	t3.loadFromFile("Assets/Play/steel.png");
	t4.loadFromFile("Assets/Play/wood.png");
	t5.loadFromFile("Assets/Play/point.png");
	t6.loadFromFile("Assets/Play/head.png");
	t7.loadFromFile("Assets/Play/card0.png");
	t8.loadFromFile("Assets/Backgrounds/wallpaper.jpg");
	t9.loadFromFile("Assets/Play/ban.png");
	t10.loadFromFile("Assets/Backgrounds/background.jpg");
	for (int i = 0; i < 3; i++) f[i].setTexture(t4);
	for (int i = 3; i < 9; i++) f[i].setTexture(t3);
	cards[0].setTexture(t7);
	for (int i = 1; i < 6; i++) cards[i].setTexture(t1);
	for (int i = 0; i < 6; i++) heads[i].setTexture(t6);
	for (int i = 0; i < 57; i++) sPoint[i].setTexture(t5);
	winBackgr.setTexture(t8);
	ban.setTexture(t9);
	ban.setPosition(300, 550);
	Sprite sBoard(t2);
	sBoard.setPosition(offset);
	Sprite background(t10);
	Clock clock;
	int clockCount = 0;
	int resource1 = 0, resource2 = 0;

	Font font;
	font.loadFromFile("Assets/Font/FS CoreMagicRough.otf");
	Text text, text2, cardText[12], turnText[2];
	text.setFont(font);
	text.setCharacterSize(35);
	text.setPosition(500, 34);

	text2.setFont(font);
	text2.setCharacterSize(35);
	text2.setPosition(685, 34);

	for (int i = 0; i < 12; ++i)
	{
		cardText[i].setFont(font);
		cardText[i].setCharacterSize(35);
		cardText[i].setFillColor(Color::Black);
		if (i < 6) cardText[i].setPosition(150 + i*184, 500);
		else cardText[i].setPosition(150 + ((i - 6) * 184), 500);
	}
	turnText[0].setString("Steel");
	turnText[1].setString("Wood");
	for (int i = 0; i < 2; ++i)
	{
		turnText[i].setFont(font);
		turnText[i].setCharacterSize(20);
		turnText[i].setPosition(300 + 552*i, 34);
	}
//	FloatRect bounds, bounds2;

	loadPosition(cards, boardCheck, boardPoint, sPoint, heads);

	Vector2i player;
	std::vector <int> cQuantity(12,0);
	int buoc = 0, buocPre = 0, cardSelected = -1;
	bool playerSelected = false;
	int woodPlay = 0, steelPlay = 0;
	int id = -1;
	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window) - Vector2i(offset);
		if (steelPlay > woodPlay) phe = 0; else phe = 6;
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
			if (e.type == Event::MouseButtonPressed)
			{
				if (e.key.code == Mouse::Left)
				{
					if (posReso(pos, steelPlay, woodPlay, resource1, resource2))
					{
						spendReso(resource1, resource2, cQuantity, steelPlay, woodPlay);
					}

					else if (selectCard(pos, cards, cQuantity) == 1 && buocPre != selectCard(pos, cards, cQuantity))
					{
						sound.play();
						buoc = selectCard(pos, cards, cQuantity);
						buocPre = buoc;
						cardSelected = 0;
						cQuantity[0+phe]--;
					}

					else if (playerSelected)
					{
						Vector2i rePlayer = selectPlayer(boardCheck, pos, woodPlay, steelPlay, after);
//						std::cout << rePlayer.x << ' ' << rePlayer.y << ' ' << after << std::endl;
						if (rePlayer == player) {
							cancelSelection(sPoint, possibleMove, buoc);
							playerSelected = false;
							buocPre = -1;
							if (cardSelected >= 0) {
								cQuantity[cardSelected+phe]++;
								cardSelected = -1;
							}
						}
						else if (HandleMouse(pos) != Vector2i(-1, -1))
						{
							playerMove(window, sBoard, player, possibleMove, pos, boardPoint, boardCheck, sPoint, playerSelected, woodPlay, steelPlay, id, buoc, after, board, sound2, possibleReso, resource1, resource2);
							buocPre = -1;
						}
					}

					else if (selectPlayer(boardCheck, pos, woodPlay, steelPlay, after) != Vector2i(-1, -1))
					{
						sound.play();
						player = selectPlayer(boardCheck, pos, woodPlay, steelPlay, after);
						playerSelected = true;
						renderMelee(player, boardPoint, board, sPoint, possibleMove, boardCheck, buoc, possibleReso);
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
		if (clock.getElapsedTime().asSeconds() > 0.15f) {
			t3.loadFromFile("Assets/Play/steel" + std::to_string(clockCount) + ".png");
			t4.loadFromFile("Assets/Play/wood" + std::to_string(clockCount) + ".png");
			if (clockCount == 17) clockCount = 0;
			else clockCount++;
			clock.restart();
		}
		if (!won) checkWin(board, won, music);

//		bounds = text.getLocalBounds();
//		bounds2 = text2.getLocalBounds();
		text.setString(std::to_string(resource1));
//		text.setPosition(501 - (bounds.width / 2), 34);
		text2.setString(std::to_string(resource2));
//		text2.setPosition(686 - (bounds2.width / 2), 34);
		for (int i = 0; i < 6; ++i)
		{
			cardText[i+phe].setString(std::to_string(cQuantity[i+phe]));
		}
		////// draw  ///////		
		window.clear();
		window.draw(background);
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
		window.draw(text);
		window.draw(text2);
		if (after) {
			if (phe == 0) window.draw(turnText[0]); else window.draw(turnText[1]);
		}
		for (int i = 0; i < 6; ++i) window.draw(cardText[i+phe]);
		if (won) {
			winBackgr.setPosition(0, 0);
			window.draw(winBackgr);
		}
		window.display();
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			music.openFromFile("Assets/Music/Soundtrack.wav");
			music.setLoop(true);
			music.play();
			break;
		}
	}
	return;
}

void AboutGame(RenderWindow &window)
{
	Texture background;
	background.loadFromFile("Assets/Backgrounds/instructions_window.jpg");

	Sprite Background(background);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(Background);
		window.display();
		if (Mouse::isButtonPressed(Mouse::Left) && Background.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) return;
	}
}

void Menu(RenderWindow &window)
{
	Music music;
	music.openFromFile("Assets/Music/Soundtrack.wav");
	music.setLoop(true);
	music.play();

	SoundBuffer cursor;
	cursor.loadFromFile("Assets/Music/Cursor3.wav");
	Sound sound;
	sound.setBuffer(cursor);

	Texture background, play, about, exit;

	background.loadFromFile("Assets/Backgrounds/wallpaper.jpg");
	play.loadFromFile("Assets/Backgrounds/Play.png");
	about.loadFromFile("Assets/Backgrounds/Instructions.png");
	exit.loadFromFile("Assets/Backgrounds/Exit.png");

	Sprite Background(background), Play(play), About(about), Exit(exit);

	Play.setOrigin(71, 20);
	About.setOrigin(200, 20);
	Exit.setOrigin(71, 20);

	Play.setPosition(620, 500);
	About.setPosition(620, 600);
	Exit.setPosition(620, 700);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(Background);
		window.draw(Play);
		window.draw(About);
		window.draw(Exit);
		window.display();

		if (Play.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			Play.setColor(Color::Black);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				sound.play();
				PlayGame(window, music);
			}
		}
		else Play.setColor(Color::White);

		if (About.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			About.setColor(Color::Black);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				sound.play();
				AboutGame(window);
			}
		}
		else About.setColor(Color::White);

		if (Exit.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
		{
			Exit.setColor(Color::Black);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				sound.play();
				return;
			}
		}
		else Exit.setColor(Color::White);
	}
}

int main()
{
//	window.setFramerateLimit(120);
	srand(time(NULL));
	RenderWindow window(VideoMode(1240, 800), "Wood And Steel", Style::Close);
	Menu(window);
	return 0;
}