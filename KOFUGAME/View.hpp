#pragma once

#include "Base.hpp"

int blockSize;

std::array<std::array<Vec2, 6>, 6> translateBoard;

class DrawGhost {

public:

	DrawGhost() = default;

	static void draw(Turn play, const GhostList& ghostList) {

		for (const auto& ghost : ghostList) {
			if (play == Turn::Player)
				Circle(getRealPos(ghost.getPos()), blockSize / 2).draw(getColor(ghost.getFlag()));
			else
				Circle(getRealPos(ghost.getPos()), blockSize / 2).draw(Palette::White);
		}
	}

	static Vec2 getRealPos(const Point& _pos) {

		return Vec2(Window::Center().x - blockSize * 3 + _pos.x*blockSize + blockSize / 2,
			Window::Center().y-blockSize*3+_pos.y*blockSize + blockSize / 2);
	}

private:

	static Color getColor(const GhostFlag flag) {

		if (flag == GhostFlag::Good)
			return Palette::Aqua;
		else
			return Palette::Plum;
	}
};

class DrawBoard {

	static std::array<Line, 7> hlines, wlines;

public:

	static void init() {

		Graphics::SetBackground(Palette::Gray);

		blockSize = Window::Height() / 7;

		for (int i = 0; i < hlines.size(); ++i) {

			hlines[i] = Line(Vec2(Window::Center().x - blockSize * 3 + i*blockSize, Window::Center().y - blockSize * 3),
				Vec2(Window::Center().x - blockSize * 3 + i*blockSize, Window::Center().y + blockSize * 3));
		}

		for (int i = 0; i < wlines.size(); ++i) {

			wlines[i] = Line(Vec2(Window::Center().x - blockSize * 3, Window::Center().y + (i - 3)*blockSize),
				Vec2(Window::Center().x + blockSize * 3, Window::Center().y + (i - 3)*blockSize));
		}
	}

	static void draw() {

		for (auto&& x : hlines)
			x.draw(Palette::Black);

		for (auto&& x : wlines)
			x.draw(Palette::Black);
	}
};

std::array<Line, 7> DrawBoard::hlines = {};

std::array<Line, 7> DrawBoard::wlines = {};