#pragma once

#include "View.hpp"

constexpr int dx[] = { -1,0,1,0 }, dy[] = { 0,-1,0,1 };

using Goal = std::array<Point, 2>;

bool motionG(double& T, const Point& _pos, const Point& _next, Turn _turn, GhostFlag _flag = GhostFlag::Good) {

	T -= 1;

	if (T == 0.0)
		return true;

	Vec2 vec = (DrawGhost::getRealPos(_next) - DrawGhost::getRealPos(_pos)) / 30.0;

	((_turn == Turn::Com) ? DrawGhost::getTextureWhite() :
		(_flag == GhostFlag::Good) ? DrawGhost::getTextureBlue() : DrawGhost::getTextureRed())
		.drawAt(DrawGhost::getRealPos(_pos) + vec*(30 - T));

	return false;
}

class Member {

protected:

	GhostList::iterator currentIt;

	Point prev;

	bool motionF;

	double motionT;

	int good, bad;

	GhostList list, removedlist;

	Font font{ 20 };
	
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

	virtual void update() = 0;

	virtual void draw() const = 0;

	Optional<std::pair<Point,Point>> clearPiece(const GhostList& opponent) {

		Optional<std::pair<Point,Point>> opt = none;

		list.erase(std::remove_if(list.begin(), list.end(), [this,&opponent,&opt](const GhostInfo& info) {

			for (auto&& x : opponent) {
				if (info.getPos() == x.getPos()) {

					Point _pos = nextGarbagePos();

					removedlist.emplace_back(_pos, info.getFlag());

					opt = std::make_pair(x.getPos(), _pos);

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

		return opt;
	}

protected:

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

	bool motion(const Point& _pos, const Point& _next, Turn _turn, GhostFlag _flag = GhostFlag::Good) {

		return motionG(motionT, _pos, _next, _turn, _flag);
	}

private:

	virtual Point nextGarbagePos() const = 0;
};

Turn turn;