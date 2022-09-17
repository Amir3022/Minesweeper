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
		void ToggleFlag();
		bool isFlagged();
		void setNeighborMines(int nMines);
		void Draw(const Vei2& screenPos, bool lost , Graphics& gfx);
	private:
		State state = State::Hidden;
		bool bMine=false;
		int nNeighborMines = -1;
	};
public:
	Minefield(int nMines,int screenWidth, int screenHeight);
	void Draw(Graphics& gfx);
	RectI GetRect();
	bool ClickReveal(const Vei2& screenPos);
	void ClickFlag(const Vei2& screenPos);
private:
	Tile& ToTile(const Vei2& gridPos);
	const Tile& ToTile(const Vei2& gridPos) const;	
	Vei2 ScreenToGrid(const Vei2& gridPos);
	int countNeighborMines(const Vei2& gridPos);
private:
	static constexpr int width = 6;
	static constexpr int height = 4;
	int top;
	int left;
	Tile Tiles[width * height];
	bool bLost = false;
	int tilesRemaining;
};