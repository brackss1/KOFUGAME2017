#pragma once

#include "View.hpp"

constexpr int dx[] = { -1,0,1,0 }, dy[] = { 0,-1,0,1 };

using Goal = std::array<Point, 2>;

enum class Phase {

	Clear,
	Select,
	Move,
	Motion,
};

class Member {

protected:

	Phase phase;

	GhostList::iterator currentIt;

	Point prev;

	double motionT;

	int good, bad;

	GhostList list, removedlist;

	Font font{ 20 };

	Optional<std::pair<Point, Point>> opt;
	
public:

	const GhostList& getList() const {

		return list;
	}

	const GhostList& getrRemovedList() const {

		return removedlist;
	}

	std::pair<int, int> getScore() const {

		return std::make_pair(good, bad);
	}

	void update() {

		switch (phase) {

		case Phase::Clear:

			clear();

			break;

		case Phase::Select:

			select();

			break;

		case Phase::Move:

			move();

			break;

		case Phase::Motion:

			motion();

			break;
		}
	}

	virtual void draw() const = 0;

protected:

	Optional<std::pair<Point, Point>> clearPiece(const GhostList& _opp) {

		Optional<std::pair<Point, Point>> _opt = none;

		list.erase(std::remove_if(list.begin(), list.end(), [this, &_opt,&_opp](const GhostInfo& info) {

			for (auto&& x : _opp) {
				if (info.getPos() == x.getPos()) {

					Point _pos = nextGarbagePos();

					removedlist.emplace_back(_pos, info.getFlag());

					_opt = std::make_pair(x.getPos(), _pos);

					return true;
				}
			}

			return false;

		}), list.end());

		good = bad = 0;

		for (const auto& ghost : list) {
			if (ghost.getFlag() == GhostFlag::Good)
				++good;
			else
				++bad;
		}

		return _opt;
	}

	bool checkKey(const Point& _pos) {

		for (auto&& ghost : list) {
			if (ghost.getPos() == _pos)
				return false;
		}

		return true;
	}

	bool checkUsableGhost(const KeyConfig& _config) {

		return _config[0] || _config[1] || _config[2] || _config[3];
	}

	bool motionG(const Point& _pos, const Point& _next, Turn _turn, GhostFlag _flag = GhostFlag::Good) {
		motionT -= 0.5;

		if (motionT <= 0.0)
			return true;

		Vec2 vec = (DrawGhost::getRealPos(_next) - DrawGhost::getRealPos(_pos)) / 30.0;

		((_turn == Turn::Com) ? DrawGhost::getTextureWhite() :
			(_flag == GhostFlag::Good) ? DrawGhost::getTextureBlue() : DrawGhost::getTextureRed())
			.drawAt(DrawGhost::getRealPos(_pos) + vec*(30 - motionT));

		return false;
	}

private:

	virtual Point nextGarbagePos() const = 0;

	virtual void select() = 0;

	virtual void move() = 0;

	virtual void motion() = 0;

	virtual void clear() = 0;
};

Turn turn;