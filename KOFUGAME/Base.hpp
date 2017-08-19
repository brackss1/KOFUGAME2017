#pragma once

#include <Siv3D.hpp>
#include <vector>
#include <algorithm>
#include <array>
#include <memory>
#include <unordered_map>
#include <random>
#include <type_traits>
#include <queue>
#include <optional>

enum class State
{
	Title,
	Rule,
	Play,
	Result
};

enum class Turn
{
	Player,
	Com,
	PlayerToCom,
	ComToPlayer
};

class Scene {
public:

	virtual void init() = 0;

	virtual void update() = 0;

	virtual void draw() const = 0;
};

enum class GhostFlag
{
	Good,
	Bad
};

using KeyConfig = std::array<bool, 4>;

class GhostInfo {

	Point pos;

	GhostFlag flag;

	KeyConfig config;

public:

	GhostInfo() = default;

	GhostInfo(const Point& _pos, GhostFlag _flag) : pos(_pos), flag(_flag) {}

	const Point& getPos() const { return pos; }

	GhostFlag getFlag() const { return flag; }

	const KeyConfig& getKeyConfig() const{ return config; }

	void setPos(const Point& _pos) { pos = _pos; }

	void setFlag(GhostFlag _flag) { flag = _flag; }

	void setKeyConfig(const KeyConfig& _config) {

		config = _config;
	}
};

bool operator< (const GhostInfo& lhs, const GhostInfo& rhs) {

	return (lhs.getPos().y == rhs.getPos().y) ? lhs.getPos().x < rhs.getPos().x : lhs.getPos().y < rhs.getPos().y;
}

bool operator> (const GhostInfo& lhs, const GhostInfo& rhs) {

	return (lhs.getPos().y == rhs.getPos().y) ? lhs.getPos().x > rhs.getPos().x : lhs.getPos().y > rhs.getPos().y;
}

using GhostList = std::vector<GhostInfo>;

GhostList userGhostList;

Turn winner;