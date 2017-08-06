#pragma once

#include "Member.hpp"

class Com : public Member {

	std::mt19937 rng{ std::random_device()() };

	static const Goal goal;

	GhostList opponent;

public:

	void init(const GhostList& _list) {

		opponent = _list;

		KeyConfig tmp;

		for (auto&& ghost : list) {
			for (int i = 0; i < 4; ++i) {

				Point p = ghost.getPos() + Point(dx[i], dy[i]);

				tmp[i] = checkKey(p);

				if (p.x < 0 || p.x > 5 ||
					p.y < 0 || p.y > 5)
					tmp[i] = false;
			}

			if (ghost.getFlag() == GhostFlag::Good) {

				if (ghost.getPos() == goal[0])
					tmp[3] = true;
				else if (ghost.getPos() == goal[1])
					tmp[3] = true;
			}

			ghost.setKeyConfig(tmp);
		}
	}

	void setList() {

		removedlist.clear();

		good = bad = 4;

		int _good = 4, _bad = 4;

		list.clear();

		for (int i = 1; i < 5; ++i) {
			for (int j = 0; j < 2; ++j) {

				GhostFlag flag;

				if (rng() % 2 == 0) {

					if (_good > 0) {

						flag = GhostFlag::Good;

						--_good;
					}
					else {

						flag = GhostFlag::Bad;

						--_bad;
					}
				}

				else {

					if (_bad > 0) {

						flag = GhostFlag::Bad;

						--_bad;
					}
					else {

						flag = GhostFlag::Good;

						--_good;
					}
				}
				
				list.emplace_back(Point(i, j), flag);
			}
		}
	}

	void update() override {

		std::vector<GhostList::iterator> vec;

		for (auto it = list.begin(); it != list.end(); ++it) {

			if (checkUsableGhost(it->getKeyConfig()))
				vec.emplace_back(it);
		}

		auto it = vec.at(rng() % vec.size());

		std::vector<int> indicies;

		for (int i = 0; i < 4; ++i) {

			Point p = it->getPos() + Point(dx[i], dy[i]);

			if (checkKey(p) &&
				0 <= p.x&&p.x < 6 &&
				0 <= p.y&&p.y < 6)
				indicies.emplace_back(i);
		}

		if (it->getFlag() == GhostFlag::Good) {

			int tmp, dis = 11;

			for (int i : indicies) {

				if (dis > std::min(disTo(it->getPos() + Point(dx[i], dy[i]), goal[0]),
					disTo(it->getPos() + Point(dx[i], dy[i]), goal[1]))) {

					dis = std::min(disTo(it->getPos() + Point(dx[i], dy[i]), goal[0]),
						disTo(it->getPos() + Point(dx[i], dy[i]), goal[1]));

					tmp = i;
				}
			}

			indicies.emplace_back(tmp);

			int res = indicies.at(rng() % indicies.size());

			it->setPos(it->getPos() + Point(dx[res], dy[res]));
		}

		else {

			int res = indicies.at(rng() % indicies.size());

			it->setPos(it->getPos() + Point(dx[res], dy[res]));
		}

		turn = Turn::Player;
	}

	void draw() const override {

		DrawGhost::draw(Turn::Player, removedlist);

		DrawGhost::draw(Turn::Com, list);
	}

	Goal getGoal() const { return goal; }

private:

	int disTo(const Point& _pos,const Point& target) {

		return (target - _pos).x + (target - _pos).y;
	}

	Point nextGarbagePos() const override {

		return Point(-1, removedlist.size());
	}
};

const Goal Com::goal = { Point(0,5),Point(5,5) };