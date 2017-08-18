#pragma once

#include "Player.hpp"
#include "Ai.hpp"
#include "View.hpp"
#include "Model.hpp"
#include <iostream>

class Play : public Scene {

	Player player;

	Com com;

	Optional<std::pair<Point,Point>> opt;

	double motionT;

public:

	void init() override {

		motionT = 30.0;

		player.setList(userGhostList);

		player.init();

		com.setList();

		opt = none;

		turn = Turn::Player;
	}

	void draw() const override {

		DrawBoard::draw();

		com.draw();

		player.draw();
	}

	void update() override {

		if (!opt.has_value()) {

			if (turn == Turn::Player) {

				player.update();

				com.init(player.getList());

				opt = com.clearPiece(player.getList());
			}

			else {

				com.update();

				player.init();

				opt = player.clearPiece(com.getList());
			}

			playerWin();

			comWin();

			motionT = 30.0;
		}

		else {

			if (turn == Turn::Player)
				player.update();
			else
				com.update();

			Turn tmp = (turn == Turn::Player) ? Turn::Com : Turn::Player;

			if (motionG(motionT, opt->first, opt->second, tmp,
				(tmp == Turn::Player) ? player.getrRemovedList().back().getFlag() : GhostFlag::Good)) {
				opt = none;
			}
		}
	}

private:

	void playerWin() {

		for (auto&& ghost : player.getList()) {

			for (int i = 0; i < 2; ++i) {

				if (ghost.getFlag() == GhostFlag::Good&&ghost.getPos() == player.getGoal()[i] + Point(dx[1], dy[1])) {

					winner = Turn::Player;

					Game::changeScene(State::Result);
				}
			}
		}

		if (player.getScore().second == 0) {

			winner = Turn::Player;

			Game::changeScene(State::Result);
		}

		if (com.getScore().first == 0) {

			winner = Turn::Player;

			Game::changeScene(State::Result);
		}
	}

	void comWin() {

		for (auto&& ghost : com.getList()) {

			for (int i = 0; i < 2; ++i) {

				if (ghost.getFlag() == GhostFlag::Good&&ghost.getPos() == com.getGoal()[i] + Point(dx[3], dy[3])) {

					winner = Turn::Com;

					Game::changeScene(State::Result);
				}
			}
		}

		if (com.getScore().second == 0) {

			winner = Turn::Com;

			Game::changeScene(State::Result);
		}

		if (player.getScore().first == 0) {

			winner = Turn::Com;

			Game::changeScene(State::Result);
		}
	}

};
