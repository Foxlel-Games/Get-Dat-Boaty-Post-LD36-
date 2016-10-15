#include "stdafx.hpp"
#include "TiledMap\TiledMap.hpp"

using namespace sf;

TiledMap::TiledMap(Uint32 TileSize)
	: TileSize_(TileSize)
{

}

TiledMap::~TiledMap()
{
}

void TiledMap::SetTexture(sf::Texture * Tex)
{
	if (Tex == nullptr)
	{
		return;
	}

	Texture_ = Tex;

	for (Uint32 i = 0; i < Dimensions_.x; ++i)
	{
		for (Uint32 j = 0; j < Dimensions_.y; ++j)
		{
			sf::Vertex* Tile = &Map_[(i + j * Dimensions_.x) * 4];

			Int32 TileID = TileIDs_[i][j];

			SetTextureCoord(Tile, TileID);
		}
	}
}

sf::Vector2f TiledMap::GetSize() const
{
	Vector2f Size;

	Size.x = (float)(Dimensions_.x * TileSize_);
	Size.y = (float)(Dimensions_.y * TileSize_);

	return sf::Vector2f(Size);
}

sf::FloatRect TiledMap::GetGlobalBounds() const
{
	return getTransform().transformRect(GetLocalBounds());
}

// private

void TiledMap::draw(sf::RenderTarget & RTarget, sf::RenderStates RStates) const
{
	RStates.transform *= getTransform();

	if (Texture_ != nullptr)
	{
		RStates.texture = Texture_;
	}

	RTarget.draw(Map_, RStates);
}

sf::FloatRect TiledMap::GetLocalBounds() const
{
	FloatRect R;

	R.left = 0.f;
	R.top = 0.f;
	R.width = GetSize().x;
	R.height = GetSize().y;

	return R;
}

void TiledMap::SetupVetices(const TMXLoader& Loader)
{
	Map_.setPrimitiveType(sf::Quads);
	//Map_.resize(Dimensions_.x * Dimensions_.y * 4);
	const Vector2i Size(Loader.GetLayer()[0]->Width, Loader.GetLayer()[0]->Height);
	Map_.resize(Size.x * Size.y * 4);

	for (Int32 i = 0; i < (signed)Size.x; ++i)
	{
		for (Int32 j = 0; j < (signed)Size.y; ++j)
		{
			sf::Vertex* Tile = &Map_[(i + j * Size.x) * 4];

			Tile[0].position = Vector2f((float)i* TileSize_, (float)j * TileSize_);
			Tile[1].position = Vector2f((float)(i + 1) * TileSize_, (float)j * TileSize_);
			Tile[2].position = Vector2f((float)(i + 1) * TileSize_, (float)(j + 1) * TileSize_);
			Tile[3].position = Vector2f((float)i * TileSize_, (float)(j + 1)*TileSize_);

			/*if (i == 0 || j == 0 || i == Dimensions_.x - 1 || j == Dimensions_.y - 1)
			{
				CollisionMap_.push_back(1);
			}
			else
			{
				CollisionMap_.push_back(0);
			}*/
			if (Texture_ == nullptr)
			{
				continue;
			}
			Int32 tilesetID = 0;
			Int32 tileID = Loader.GetLayer()[0]->Data[j][i] - Loader.GetTileSet()[0]->FirstGID_;

			Vector2i texturePos(tileID % (Texture_->getSize().x / TileSize_), tileID / (Texture_->getSize().x / TileSize_));

			Tile[0].texCoords = Vector2f((float)texturePos.x* TileSize_, (float)texturePos.y * TileSize_);
			Tile[1].texCoords = Vector2f((float)(texturePos.x + 1) * TileSize_, (float)texturePos.y* TileSize_);
			Tile[2].texCoords = Vector2f((float)(texturePos.x + 1) * TileSize_, (float)(texturePos.y + 1) * TileSize_);
			Tile[3].texCoords = Vector2f((float)texturePos.x* TileSize_, (float)(texturePos.y + 1)*TileSize_);

		}
	}
}

void TiledMap::SetTextureCoord(Vertex* Tile, Int32 TileID)
{
	assert(Texture_);

	Vector2f TexSize(Texture_->getSize());
	Int32 TexId(rand() % 100);

	Vector2f TexPos;
	TexPos.x = static_cast<float> (TileID % (Texture_->getSize().x / TileSize_));
	TexPos.y = static_cast<float> (TileID / (Texture_->getSize().x / TileSize_));

	TexPos *= (float)TileSize_;

	Tile[0].texCoords = Vector2f(TexPos.x, TexPos.y);
	Tile[1].texCoords = Vector2f(TexPos.x + (float)TileSize_, TexPos.y);
	Tile[2].texCoords = Vector2f(TexPos.x + (float)TileSize_, TexPos.y + (float)TileSize_);
	Tile[3].texCoords = Vector2f(TexPos.x, TexPos.y + (float)TileSize_);
}