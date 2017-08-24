#pragma once

#include "Player.hpp"
#include "Ai.hpp"
#include "View.hpp"
#include "Model.hpp"

class Play : public Scene {

	Player player;

	Com com;

	Font font;

	double motionT;

	double s;

public:

	void init() override {

		font = Font(Window::Height()/8);

		motionT = 0;

		player.setList(userGhostList);

		com.setList();

		player.init(com.getList());

		com.init(player.getList());

		turn = Turn::Player;
	}

	void draw() const override {

		DrawBoard::draw();

		com.draw();

		player.draw();
	}

	void update() override {

		switch (turn) {

		case Turn::Player:

			player.update();

			break;

		case Turn::Com:

			com.update();

			break;

		case Turn::ComToPlayer:

			if (motionT<=0) {

				player.init(com.getList());

				turn = Turn::Player;

				motionT = 0;
			}

			else {

				font(L"PlayerTurn").drawAt(Window::Width() / 20 * (20-motionT), Window::Center().y, Palette::White);

				motionT -= 0.3;
			}

			break;

		case Turn::PlayerToCom:

			if (motionT <= 0) {

				com.init(player.getList());

				turn = Turn::Com;

				motionT = 0;
			}

			else {

				font(L"ComTurn").drawAt(Window::Width() / 20 * (20-motionT), Window::Center().y, Palette::White);

				motionT -= 0.3;
			}

			break;
		}
	
		playerWin();

		comWin();
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
