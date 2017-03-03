#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <memory>

// make sure to include the correct .dll in the running directory for the executable

namespace ResourceManager_n
{
   // the width of (both) the border tiles (and the margin)
   static const int TILE_WIDTH = 60; 

   // paths to game assets
   static const std::string PATH_TO_ASSETS = "../assets/";
   static const std::vector<std::string> IMG_PATHS =
   {
      "board.png", "tiles.png", "pieces.png", "special.png"
   };

   // screen dimension constants
   static const int SCREEN_WIDTH = ResourceManager_n::TILE_WIDTH * 19;
   static const int SCREEN_HEIGHT = ResourceManager_n::TILE_WIDTH * 11;

   enum ImageType_e
   {
      BOARD,
      BASE_TILE,
      EDGE_TILE,
      CORNER_TILE,
      P1_SCORING_TILE,
      P1_MANHOLE_TILE,
      P2_SCORING_TILE,
      P2_MANHOLE_TILE,
      P1_PAWN,
      P1_GUN,
      P1_SLINGER,
      P2_PAWN,
      P2_GUN,
      P2_SLINGER,
      BRIEFCASE,
      CURSOR
   };

   struct Sprite_s
   {
      // parameterized constructor
      Sprite_s(const std::string &img_path, SDL_Texture* texture, SDL_Rect* clip) :
         mImgPath(img_path),
         mTexture(texture),
         mClip(clip)
      {

      }

      // copy constructor
      Sprite_s(const Sprite_s& sprite) : 
         mImgPath(sprite.mImgPath),
         mTexture(sprite.mTexture),
         mClip(sprite.mClip)
      {

      }

      const std::string& mImgPath;
      SDL_Texture* mTexture;
      SDL_Rect* mClip;
   };

   class ResourceManager_c
   {
   public:
      // default constructor
      ResourceManager_c(SDL_Renderer* renderer);

      // default destructor
      ~ResourceManager_c();

      // \Name: getTexture
      // \Description:
      // - returns a texture based on the passed value
      // \Argument:
      // - Imagetype_e, the type of image to return
      // \Returns
      // - SDL_Texture*, pointer to the requested texture
      Sprite_s& getSprite(ImageType_e image_type);

      // \Name: loadTextures
      // \Description:
      // - loads all required textures to memory
      // \Argument:
      // - none
      // \Returns
      // - bool, success or failure
      bool loadTextures();

   protected:
      void loadSprites(const std::string &img_path, SDL_Texture& spritesheet);

      // the window renderer
      SDL_Renderer* gRenderer = NULL;

      // container mapping the loaded SDL_Textures to an indentifying enumeration
      // typedef std::vector<std::unique_ptr<std::pair<const std::string&, SDL_Texture*>>> TextureMap;
      // TextureMap& mTextures;
      std::vector<SDL_Texture*> mTextures;

      // typedef std::unordered_map<ImageType_e, Sprite_s> SpriteMap;
      // typedef std::vector<std::unique_ptr<std::pair<ImageType_e, Sprite_s>>> SpriteMap;
      // SpriteMap mSprites;
      std::vector<Sprite_s> mSprites;
   };
}

#endif _RESOURCE_MANAGER_HPP_