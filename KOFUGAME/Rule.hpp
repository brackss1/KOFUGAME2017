#pragma once

#include "Model.hpp"

class Rule : public Scene {

	Font font, blue{ 20 }, red{ 20 };

	Point pos;

	std::array<bool, 8> arr;

	double r = 60.0;

	int blueCnt, redCnt;

public:

	void init() override {

		font = Font(Window::Height() / 8);

		blue = Font(Window::Height() / 24);

		red = Font(Window::Height() / 24);

		blueCnt = redCnt = 4;

		Graphics::SetBackground(Palette::Gray);

		userGhostList.clear();

		for (int i = 1; i < 5; ++i) {
			for (int j = 4; j < 6; ++j) {
				userGhostList.push_back({});
				userGhostList.rbegin()->setPos({ i, j });
			}
		}

		for (auto& x : arr)
			x = false;

		pos = Point(1,4);
	}

	void update() override {

		if (KeyUp.down())
			--pos.y;
		else if (KeyDown.down())
			++pos.y;
		else if (KeyLeft.down())
			--pos.x;
		else if(KeyRight.down())
			++pos.x;

		if (pos.x < 1)
			++pos.x;
		else if (pos.x > 4)
			--pos.x;
		if (pos.y < 4)
			++pos.y;
		else if (pos.y > 5)
			--pos.y;

		if (check()) {

			if (KeyEnter.down())
				Game::changeScene(State::Play);
		}

		else if (KeyZ.down()&&blueCnt>0) {

			--blueCnt;

			if (arr[getIndex(pos)]) {
				if (userGhostList[getIndex(pos)].getFlag() == GhostFlag::Good)
					++blueCnt;
				else
					++redCnt;
			}

			arr[getIndex(pos)] = true;

			userGhostList[getIndex(pos)].setFlag(GhostFlag::Good);
		}

		else if (KeyX.down()&&redCnt>0) {

			--redCnt;

			if (arr[getIndex(pos)]) {
				if (userGhostList[getIndex(pos)].getFlag() == GhostFlag::Good)
					++blueCnt;
				else
					++redCnt;
			}

			arr[getIndex(pos)] = true;

			userGhostList[getIndex(pos)].setFlag(GhostFlag::Bad);
		}
	}

	void draw() const override {

		font(L"Please arrange ghosts").drawAt(Window::Center().x, Window::Center().y / 4, Palette::White);

		blue(L"Žc‚è:", blueCnt).drawAt(Window::Center().x - r * 4.5, Window::Center().y - r/2, Palette::Blue);

		red(L"Žc‚è:", redCnt).drawAt(Window::Center().x - r * 4.5, Window::Center().y + r/2, Palette::Red);

		for (int i = 0; i < arr.size(); ++i) {

			if (arr[i])
				Circle(getRealPos(userGhostList[i].getPos()), r)
				.draw((userGhostList[i].getFlag() == GhostFlag::Good) ? Color(L"#6633FF") : Color(L"#CC0033"));
			else
				Circle(getRealPos(userGhostList[i].getPos()), r)
				.drawFrame(2, 0, Palette::White);

			if (pos == userGhostList[i].getPos())
				Circle(getRealPos(pos), r)
				.drawFrame(4, 0, Palette::Yellow);
		}
	}

private:

	Vec2 getRealPos(const Point& _pos) const{

		Vec2 tmp((_pos.x - 1)*r * 2, (_pos.y - 4)*r * 2);

		tmp.x += Window::Center().x - r * 3;

		tmp.y += Window::Center().y - r;

		return tmp;
	}

	bool check() {

		for (auto x : arr) {
			if (!x)
				return false;
		}

		return true;
	}

	int getIndex(const Point& _pos) const {

		return (_pos.x - 1) * 2 + (_pos.y - 4);
	}
};