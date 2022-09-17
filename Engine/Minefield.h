#pragma once
#include "Graphics.h"
#include "Vei2.h"
#include "RectI.h"

class Minefield
{
private:
	class Tile
	{
	public:
		enum class State : short
		{
			Hidden,
			Flagged,
			Revealed
		};
	public:
		Tile() = default;
		void SpawnMine();
		bool hasMine();
		void Reveal();
		bool isRevealed();
		void Draw(const Vei2& screenPos, Graphics& gfx);
	private:
		State state = State::Hidden;
		bool bMine=false;
	};
public:
	Minefield(int nMines);
	void Draw(Graphics& gfx);
private:
	Tile& ToTile(const Vei2& gridPos);
	const Tile& ToTile(const Vei2& gridPos) const;
	RectI GetRect();
private:
	static constexpr int width = 10;
	static constexpr int height = 10;
	int top = 0;
	int left = 0;
	Tile Tiles[width * height];

};