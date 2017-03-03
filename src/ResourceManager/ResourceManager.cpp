#include "ResourceManager.hpp"
#include "iostream"

using namespace ResourceManager_n;

ResourceManager_c::ResourceManager_c(SDL_Renderer* renderer) :
   mTextures(*new TextureMap())
{
   gRenderer = renderer;
   // mTextures = *new TextureMap();
}

ResourceManager_c::~ResourceManager_c()
{
   // free all sprites
   /*SpriteMap::iterator sprite_it;
   for (sprite_it = mSprites.begin(); sprite_it != mSprites.end(); ++sprite_it)
   {
      delete *it;
   }*/

   // free all textures
   TextureMap::iterator texture_it;
   for (texture_it = mTextures.begin(); texture_it != mTextures.end(); ++texture_it)
   {
      SDL_DestroyTexture(texture_it->get()->second);
   }

   delete &mTextures;

   /* 
    * Cleans any memory dynamically allocated to the image loading process.
    * May need to be moved around (along with the whole SDL_image library),
    * possibly to the main StandoffApp class. 
    *
    * -> does this need to be fired BEFORE SDL_Quit()???
    * -> does this get properly called from the destructor???
    *
    */
   IMG_Quit();
}

Sprite_s& ResourceManager_c::getSprite(ImageType_e image_type)
{
   std::cout << "mSprites is " << mSprites.size() << " long!" << std::endl;
   std::cout << "mTextures is " << mTextures.size() << " long!" << std::endl;

   Sprite_s* sprite = NULL;
   SpriteMap::iterator it;
   /*for (it = mSprites.begin(); it != mSprites.end(); ++it)
   {
      if (it->get()->first == image_type)
      {
         sprite = &(it->get()->second);
         break;
      }
   }*/
   return *sprite;
}

bool ResourceManager_c::loadTextures()
{
   bool success = true;

   // initialize PNG loading
   int imgFlags = IMG_INIT_PNG;
   if( !( IMG_Init( imgFlags ) & imgFlags ) )
   {
      printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
      success = false;
   }

   std::vector<std::string>::const_iterator it; 
   for (it = ResourceManager_n::IMG_PATHS.begin(); it != ResourceManager_n::IMG_PATHS.end(); ++it)
   {
      SDL_Texture* loaded_texture = NULL;
      std::string img_path = PATH_TO_ASSETS + *it;

      loaded_texture = IMG_LoadTexture(gRenderer, img_path.c_str());

      if (loaded_texture == NULL)
      { 
         printf("Unable to load texture %s! SDL_image Error: %s\n", img_path.c_str(), IMG_GetError());
         success = false;
      }
      else
      {
         std::cout << "DOGY" << std::endl;
         std::unique_ptr<std::pair<const std::string&, SDL_Texture*>> ptr;
         std::pair<const std::string&, SDL_Texture*> mapped_val = std::make_pair(*it, &(*loaded_texture));
         mTextures.push_back());
         std::cout << "Loading: " << img_path << std::endl;
         // loadSprites(*it, *loaded_texture);
      }
   }

   // an alterante location for the call to IMG_Quit()
   // IMG_Quit()

   return success;
}

void ResourceManager_c::loadSprites(const std::string& img_path, SDL_Texture& spritesheet)
{
   SDL_Rect clip;
   Sprite_s* sprite = NULL;

   if (img_path == "board.png")
   {
      sprite =  &Sprite_s(img_path, &spritesheet, NULL);
      mSprites.push_back(std::make_unique<std::pair<ImageType_e, Sprite_s>>(BOARD, *sprite));
   }
   else if (img_path == "tiles.png")
   {
      // base tile
      clip = { 0, 0, TILE_WIDTH , TILE_WIDTH };
      sprite = &Sprite_s(img_path, &spritesheet, &clip);
      mSprites.push_back(std::make_unique<std::pair<ImageType_e, Sprite_s>>(BASE_TILE, *sprite));

      // edge tile
      clip = { TILE_WIDTH, 0, TILE_WIDTH , TILE_WIDTH };
      sprite = &Sprite_s(img_path, &spritesheet, &clip);
      mSprites.push_back(std::make_unique<std::pair<ImageType_e, Sprite_s>>(EDGE_TILE, *sprite));

      // corner tile
      clip = { TILE_WIDTH * 2, 0, TILE_WIDTH , TILE_WIDTH };
      sprite = &Sprite_s(img_path, &spritesheet, &clip);
      mSprites.push_back(std::make_unique<std::pair<ImageType_e, Sprite_s>>(CORNER_TILE, *sprite));

      // p1 scoring tile
      clip = { 0, TILE_WIDTH, TILE_WIDTH , TILE_WIDTH };
      sprite = &Sprite_s(img_path, &spritesheet, &clip);
      mSprites.push_back(std::make_unique<std::pair<ImageType_e, Sprite_s>>(P1_SCORING_TILE, *sprite));

      // p1 manhole tile
      clip = { TILE_WIDTH, TILE_WIDTH, TILE_WIDTH , TILE_WIDTH };
      sprite = &Sprite_s(img_path, &spritesheet, &clip);
      mSprites.push_back(std::make_unique<std::pair<ImageType_e, Sprite_s>>(P1_MANHOLE_TILE, *sprite));

      // p2 scoring tile
      clip = { 0, TILE_WIDTH * 2, TILE_WIDTH , TILE_WIDTH };
      sprite = &Sprite_s(img_path, &spritesheet, &clip);
      mSprites.push_back(std::make_unique<std::pair<ImageType_e, Sprite_s>>(P2_SCORING_TILE, *sprite));

      // p2 manhole tile
      clip = { TILE_WIDTH, TILE_WIDTH * 2, TILE_WIDTH , TILE_WIDTH };
      sprite = &Sprite_s(img_path, &spritesheet, &clip);
      mSprites.push_back(std::make_unique<std::pair<ImageType_e, Sprite_s>>(P2_MANHOLE_TILE, *sprite));
   }
   //else if (img_path == "pieces.png")
   //{
   //   // p1 pawn
   //   clip = { 0, 0, TILE_WIDTH , TILE_WIDTH };
   //   sprite = &Sprite_s(img_path, &spritesheet, &clip);
   //   mSprites.push_back(std::pair<ImageType_e, Sprite_s&>(P1_PAWN, *sprite));

   //   // p1 gun
   //   clip = { TILE_WIDTH, 0, TILE_WIDTH , TILE_WIDTH };
   //   sprite = &Sprite_s(img_path, &spritesheet, &clip);
   //   mSprites.push_back(std::pair<ImageType_e, Sprite_s&>(P1_GUN, *sprite));

   //   // p1 slinger
   //   clip = { TILE_WIDTH * 2, 0, TILE_WIDTH , TILE_WIDTH };
   //   sprite = &Sprite_s(img_path, &spritesheet, &clip);
   //   mSprites.push_back(std::pair<ImageType_e, Sprite_s&>(P1_SLINGER, *sprite));

   //   // p2 pawn
   //   clip = { 0, TILE_WIDTH, TILE_WIDTH , TILE_WIDTH };
   //   sprite = &Sprite_s(img_path, &spritesheet, &clip);
   //   mSprites.push_back(std::pair<ImageType_e, Sprite_s&>(P2_PAWN, *sprite));

   //   // p2 gun
   //   clip = { TILE_WIDTH, TILE_WIDTH, TILE_WIDTH , TILE_WIDTH };
   //   sprite = &Sprite_s(img_path, &spritesheet, &clip);
   //   mSprites.push_back(std::pair<ImageType_e, Sprite_s&>(P2_GUN, *sprite));

   //   // p2 slinger
   //   clip = { TILE_WIDTH * 2, TILE_WIDTH, TILE_WIDTH , TILE_WIDTH };
   //   sprite = &Sprite_s(img_path, &spritesheet, &clip);
   //   mSprites.push_back(std::pair<ImageType_e, Sprite_s&>(P2_SLINGER, *sprite));
   //}
   //else if (img_path == "special.png")
   //{
   //   // briefcase
   //   clip = { 0, 0, TILE_WIDTH , TILE_WIDTH };
   //   sprite = &Sprite_s(img_path, &spritesheet, &clip);
   //   mSprites.push_back(std::pair<ImageType_e, Sprite_s&>(BRIEFCASE, *sprite));

   //   // cursor
   //   clip = { TILE_WIDTH, 0, TILE_WIDTH , TILE_WIDTH };
   //   sprite = &Sprite_s(img_path, &spritesheet, &clip);
   //   mSprites.push_back(std::pair<ImageType_e, Sprite_s&>(CURSOR, *sprite));
   //}

   std::cout << img_path << " successfully loaded..." << std::endl;
   std::cout << "mSprites is " << mSprites.size() << " long!" << std::endl;
}