#pragma once

#include "View.hpp"

constexpr int dx[] = { -1,0,1,0 }, dy[] = { 0,-1,0,1 };

using Goal = std::array<Point, 2>;

class Member {

protected:

	int good, bad;

	GhostList list, removedlist;

	Font font{ 20 };
	
public:

	const GhostList& getList() const {

		return list;
	}

	std::pair<int, int> getScore() const {

		return std::make_pair(good, bad);
	}

	virtual void update() = 0;

	virtual void draw() const = 0;

	void clearPiece(const GhostList& opponent) {

		list.erase(std::remove_if(list.begin(), list.end(), [this,&opponent](const GhostInfo& info) {

			for (auto&& x : opponent) {
				if (info.getPos() == x.getPos()) {

					removedlist.emplace_back(nextGarbagePos(), info.getFlag());

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

private:

	virtual Point nextGarbagePos() const = 0;
};

Turn turn;