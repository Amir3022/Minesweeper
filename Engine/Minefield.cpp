#include "Minefield.h"
#include <random>
#include <assert.h>
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
	if (!isRevealed())
	{
		state = State::Revealed;
	}
}

bool Minefield::Tile::isRevealed()
{
	return state == State::Revealed;
}

void Minefield::Tile::Draw(const Vei2& screenPos, Graphics& gfx)
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
	{
		SpriteCodex::DrawTile0(screenPos, gfx);
		if (hasMine())
		{
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
	}	
	}
}

Minefield::Minefield(int nMines)
{
	assert(nMines > 0 && nMines < width* height);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0,width-1);
	std::uniform_int_distribution<int> yDist(0,height-1);
	bool temp= Tiles[0].hasMine();
	for (int i = 0; i < nMines; i++)
	{
		Vei2 gridPos; 
		do
		{
			gridPos = Vei2(xDist(rng), yDist(rng));
		} while (ToTile(gridPos).hasMine());
		ToTile({ xDist(rng),yDist(rng) }).SpawnMine();
	}
	for(int i=0;i<100;i++)
	{
		ToTile({ xDist(rng),yDist(rng) }).Reveal();
	}
}


void Minefield::Draw(Graphics& gfx)
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			ToTile(gridPos).Draw(gridPos * SpriteCodex::tileSize, gfx);
		}
	}
}

Minefield::Tile& Minefield::ToTile(const Vei2& gridPos)
{
	return Tiles[gridPos.x + gridPos.y * width];
}

const Minefield::Tile& Minefield::ToTile(const Vei2& gridPos) const
{
	return Tiles[gridPos.x + gridPos.y * width];
}

RectI Minefield::GetRect()
{
	return RectI(left, left + width * SpriteCodex::tileSize, top, top + height * SpriteCodex::tileSize);
}

