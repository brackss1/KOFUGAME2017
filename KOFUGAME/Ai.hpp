#pragma once

#include "Member.hpp"

class Com : public Member {

	std::mt19937 rng{ std::random_device()() };

	static const Goal goal;

	GhostList opponent;

public:

	void init(const GhostList& _list) {

		motionF = false;

		motionT = 30;

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

		if (!motionF) {

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

			for (const auto& x : opponent) {
				if (x.getFlag() == GhostFlag::Good) {
					for (int i : indicies) {
						if (it->getPos() + Point(dx[i], dy[i]) == x.getPos()) {

							it->setPos(it->getPos() + Point(dx[i], dy[i]));

							return;
						}
					}
				}
				else {
					for (int i : indicies) {
						if (it->getPos() + Point(dx[i], dy[i]) == x.getPos() && rng() % 4 != 0) {

							it->setPos(it->getPos() + Point(dx[i], dy[i]));

							return;
						}
					}
				}
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

				prev = it->getPos();

				it->setPos(it->getPos() + Point(dx[res], dy[res]));

				currentIt = it;
			}

			else {

				int res, min = 11;

				for (int i : indicies) {

					int tmp = searchOpp(it->getPos() + Point(dx[i], dy[i]));

					if (min > tmp) {

						min = tmp;

						res = i;
					}
				}

				prev = it->getPos();

				it->setPos(it->getPos() + Point(dx[res], dy[res]));

				currentIt = it;
			}

			motionF = true;
		}

		else {

			if (motion(prev, currentIt->getPos(),Turn::Com))
				turn = Turn::Player;
		}
	}

	void draw() const override {

		for (const auto& x : removedlist)
			DrawGhost::draw(Turn::Player, x);

		for (const auto& x : list) {
			if (!motionF || motionF && (x.getPos() != currentIt->getPos()))
				DrawGhost::draw(Turn::Com, x);
		}
	}

	Goal getGoal() const { return goal; }

private:

	int disTo(const Point& _pos,const Point& target) {

		return (target - _pos).x + (target - _pos).y;
	}

	Point nextGarbagePos() const override {

		return Point(-1, removedlist.size());
	}

	int searchOpp(Point _pos) {

		const int INF = 11;

		int d[6][6];

		for (int i = 0; i < 6; ++i) {
			for (int j = 0; j < 6; ++j)
				d[i][j] = INF;
		}

		std::queue<Point> que;

		d[_pos.y][_pos.x] = 0;

		que.emplace(_pos);

		while (!que.empty()) {

			Point p = que.front();

			que.pop();

			for (const auto& x : opponent) {
				if (p == x.getPos())
					return d[p.y][p.x];
			}

			for (int i = 0; i < 4; ++i) {

				int nx = p.x + dx[i];

				int ny = p.y + dy[i];

				if (0 <= nx&&nx < 6 && 0 <= ny&&ny < 6 && d[ny][nx] == INF) {

					d[ny][nx] = d[p.y][p.x] + 1;

					que.emplace(nx, ny);
				}
			}
		}
		return INF;
	}
};

const Goal Com::goal = { Point(0,5),Point(5,5) };