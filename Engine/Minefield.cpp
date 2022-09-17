#include "Minefield.h"
#include <random>
#include <assert.h>
#include <cmath>
#include "SpriteCodex.h"

void Minefield::Tile::SpawnMine()
{
	if (!hasMine())
	{
		bMine = true;
	}
}

bool Minefield::Tile::hasMine()
{
	return bMine;
}

void Minefield::Tile::Reveal()
{
	if (!isRevealed() &&!isFlagged() )
	{
		state = State::Revealed;
	}
}

bool Minefield::Tile::isRevealed()
{
	return state == State::Revealed;
}

void Minefield::Tile::ToggleFlag()
{
	if (!isFlagged())
	{
		state = State::Flagged;
	}
	else
	{
		state = State::Hidden;
	}
}

bool Minefield::Tile::isFlagged()
{
	return state == State::Flagged;
}

void Minefield::Tile::setNeighborMines(int nMines)
{
	assert(nMines >= 0 && nMines <= 8);
	if (nNeighborMines == -1)
	{
		nNeighborMines = nMines;
	}
}

void Minefield::Tile::Draw(const Vei2& screenPos, bool lost, Graphics& gfx)
{
	if (!lost)
	{
		switch (state)
		{
		case State::Hidden:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			break;
		case State::Flagged:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			SpriteCodex::DrawTileFlag(screenPos, gfx);
			break;
		case State::Revealed:
			SpriteCodex::DrawTileNumber(screenPos, nNeighborMines, gfx);
			if (hasMine())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
		}
	}
	else
	{
		switch (state)
		{
		case State::Hidden:
			if (hasMine())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileButton(screenPos, gfx);
			}
			break;
		case State::Flagged:
			if (hasMine())
			{
				SpriteCodex::DrawTileButton(screenPos, gfx);
				SpriteCodex::DrawTileFlag(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileCross(screenPos, gfx);
			}
			break;
		case State::Revealed:
			SpriteCodex::DrawTileNumber(screenPos, nNeighborMines, gfx);
			if (hasMine())
			{
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
		}
	}

}

Minefield::Minefield(int nMines, int screenWidth, int screenHeight)
{
	left = (screenWidth / SpriteCodex::tileSize - width) / 2;
	top = (screenHeight / SpriteCodex::tileSize - height) / 2;
	assert(nMines > 0 && nMines < width* height);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(left,left+width-1);
	std::uniform_int_distribution<int> yDist(top,top+height-1);
	bool temp= Tiles[0].hasMine();
	for (int i = 0; i < nMines; i++)
	{
		Vei2 gridPos; 
		do
		{
			gridPos = Vei2(xDist(rng), yDist(rng));
		} while (ToTile(gridPos).hasMine());
		ToTile(gridPos).SpawnMine();
	}
	for (Vei2 gridPos = { left, top }; gridPos.y < top+height; gridPos.y++)
	{
		for (gridPos.x = left; gridPos.x < left+width; gridPos.x++)
		{
			int nMines = countNeighborMines(gridPos);
			ToTile(gridPos).setNeighborMines(nMines);
		}
	}

}


void Minefield::Draw(Graphics& gfx)
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPos = { left,top }; gridPos.y < top+height; gridPos.y++)
	{
		for (gridPos.x = left; gridPos.x < left+width; gridPos.x++)
		{
			ToTile(gridPos).Draw(gridPos * SpriteCodex::tileSize, bLost, gfx);
		}
	}
}

RectI Minefield::GetRect()
{
	return RectI(left* SpriteCodex::tileSize, (left + width) * SpriteCodex::tileSize, top* SpriteCodex::tileSize, (top + height) * SpriteCodex::tileSize);
}

void Minefield::ClickReveal(const Vei2& screenPos)
{
	if (!bLost)
	{
		Vei2 gridPos = ScreenToGrid(screenPos);
		Tile& tile = ToTile(gridPos);
		if (!tile.isRevealed())
		{
			tile.Reveal();
			if (tile.hasMine())
			{
				bLost = true;
			}
		}
	}
}

void Minefield::ClickFlag(const Vei2& screenPos)
{
	if (!bLost)
	{
		Vei2 gridPos = ScreenToGrid(screenPos);
		Tile& tile = ToTile(gridPos);
		if (!tile.isRevealed())
		{
			tile.ToggleFlag();
		}
	}
}

Minefield::Tile& Minefield::ToTile(const Vei2& gridPos)
{
	return Tiles[gridPos.x - left + (gridPos.y - top) * width];
}

const Minefield::Tile& Minefield::ToTile(const Vei2& gridPos) const
{
	return Tiles[gridPos.x - left + (gridPos.y - top) * width];
}

Vei2 Minefield::ScreenToGrid(const Vei2& screenPos)
{
	return screenPos / SpriteCodex::tileSize;
}

int Minefield::countNeighborMines(const Vei2& gridPos)
{
	int xStart = std::max(left, gridPos.x - 1);
	int yStart = std::max(top, gridPos.y - 1);
	int xEnd = std::min(left+width - 1, gridPos.x + 1);
	int yEnd = std::min(top+height - 1, gridPos.y + 1);
	int counter = 0;
	for (Vei2 pos = { xStart,yStart }; pos.y <= yEnd; pos.y++)
	{
		for (pos.x = xStart; pos.x <= xEnd; pos.x++)
		{
			if (ToTile(pos).hasMine())
			{
				counter++;
			}
		}
	}
	return counter;
}

