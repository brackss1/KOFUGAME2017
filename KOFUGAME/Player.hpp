#pragma once

#include "Member.hpp"

class Player : public Member {

	std::mt19937 rng;

	int moveKey;

	static const Goal goal;

	Font gFont;

public:

	void init(const GhostList& _list) {

		phase = Phase::Clear;

		opt = clearPiece(_list);

		currentIt = list.begin();

		motionT = 30;

		moveKey = -1;

		KeyConfig tmp;

		for (auto&& ghost : list) {
			for (int i = 0; i < 4; ++i) {

				tmp[i] = checkKey(Point(ghost.getPos().x + dx[i], ghost.getPos().y + dy[i]));

				if (ghost.getPos().x + dx[i] < 0 || ghost.getPos().x + dx[i]>5 ||
					ghost.getPos().y + dy[i] < 0 || ghost.getPos().y + dy[i]>5)
					tmp[i] = false;
			}

			if (ghost.getFlag() == GhostFlag::Good) {

				if (ghost.getPos() == goal[0])
					tmp[1] = true;
				else if (ghost.getPos() == goal[1])
					tmp[1] = true;
			}

			ghost.setKeyConfig(tmp);
		}
	}

	void draw() const override {

		for (int i = 0; i < removedlist.size(); ++i) {

			if (!opt.has_value() || i != removedlist.size() - 1)
				DrawGhost::draw(Turn::Player, removedlist[i]);
		}

		for (const auto& x : list) {
			if (phase!=Phase::Motion || x.getPos() != currentIt->getPos())
				DrawGhost::draw(Turn::Player, x);
		}

		for (const auto& x : goal)
			gFont(L"Å™").drawAt(DrawGhost::getRealPos(x), Color(255, 0, 0, 100));

		if ((phase == Phase::Select || phase == Phase::Move) && turn == Turn::Player) {

			if (phase == Phase::Move) {

				for (int i = 0; i < 4; ++i) {
					if (currentIt->getKeyConfig()[i])
						RectF(blockSize)
						.setCenter(DrawGhost::getRealPos(currentIt->getPos() + Point(dx[i], dy[i])))
						.drawFrame(5, 0, Palette::Yellow);
				}

				if (moveKey != -1) {

					RectF(blockSize)
						.setCenter(DrawGhost::getRealPos(currentIt->getPos() + Point(dx[moveKey], dy[moveKey])))
						.drawFrame(5, 0, Palette::Greenyellow);
				}
			}

			Circle(DrawGhost::getRealPos(currentIt->getPos()), blockSize / 2)
				.drawFrame(5, 0, (phase == Phase::Move) ? Palette::Greenyellow : Palette::Yellow);
		}
	}

	void setList(const GhostList& _list) {

		removedlist.clear();

		gFont = Font(blockSize);

		good = bad = 4;

		list = _list;
	}

	Goal getGoal() const { return goal; }

private:

	Point nextGarbagePos() const override {

		return Point(6, 5 - removedlist.size());
	}

	void clear() override{

		if (!opt.has_value())
			phase = Phase::Select;
		else {
			if (motionG(opt->first, opt->second, Turn::Player, removedlist.back().getFlag())) {

				opt = none;

				motionT = 30;
			}
		}
	}

	void select() override {

		std::sort(list.begin(), list.end());

		int p = currentIt - list.begin();

		if (KeyW.down()) {

			p = std::lower_bound(list.begin(), list.end(),
				GhostInfo(Point(currentIt->getPos().x, currentIt->getPos().y - 1), GhostFlag::Bad)) - list.begin();

			for (; p >= 0 && !checkUsableGhost((list.begin() + p)->getKeyConfig()); --p);
		}

		else if (KeyS.down()) {

			p = std::upper_bound(list.begin(), list.end(),
				GhostInfo(Point(currentIt->getPos().x, currentIt->getPos().y + 1), GhostFlag::Bad)) - list.begin() - 1;

			for (; p < list.size() && !checkUsableGhost((list.begin() + p)->getKeyConfig()); ++p);
		}

		else if (KeyA.down())
			for (--p; p >= 0 && !checkUsableGhost((list.begin() + p)->getKeyConfig()); --p);

		else if (KeyD.down())
			for (++p; p < list.size() && !checkUsableGhost((list.begin() + p)->getKeyConfig()); ++p);

		if (p < 0)
			p = 0;

		if (p >= list.size())
			p = list.size() - 1;

		currentIt = list.begin() + p;

		if (KeyZ.down()) {

			prev = currentIt->getPos();

			phase = Phase::Move;
		}
	}

	void move() override {

		if (KeyX.down())
			phase = Phase::Select;

		if (currentIt->getKeyConfig()[1] && KeyW.down())
			moveKey = 1;
		else if (currentIt->getKeyConfig()[3] && KeyS.down())
			moveKey = 3;
		else if (currentIt->getKeyConfig()[2] && KeyD.down())
			moveKey = 2;
		else if (currentIt->getKeyConfig()[0] && KeyA.down())
			moveKey = 0;

		if (moveKey != -1 && KeyZ.down()) {

			currentIt->setPos(currentIt->getPos() + Point(dx[moveKey], dy[moveKey]));

			phase = Phase::Motion;
		}
	}

	void motion() override {

		if (motionG(prev, currentIt->getPos(), Turn::Player, currentIt->getFlag())) {

			turn = Turn::PlayerToCom;

			phase = Phase::Clear;
		}
	}
};

const Goal Player::goal = { Point::Zero(),Point(5,0) };