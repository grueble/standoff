#include "ResourceManager.hpp"

using namespace ResourceManager_n;

ResourceManager_c::ResourceManager_c(SDL_Renderer* renderer)
{
   gRenderer = renderer;

   loadTextures();
}

ResourceManager_c::~ResourceManager_c()
{
   // free all textures
   std::unordered_map<ImageType_e, SDL_Texture*>::iterator it;
   for (it = mTextures.begin(); it != mTextures.end(); ++it)
   {
      SDL_DestroyTexture(it->second);
      it->second = NULL;
   }

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

SDL_Texture* ResourceManager_c::getTexture(ImageType_e image_type)
{
   // switch statement here to cover the different loaded images

   TextureMap::const_iterator it = mTextures.find(image_type);
   return it->second;
}

bool ResourceManager_c::loadTextures()
{
   // loading success flag
   bool success = true;

   // initialize PNG loading
   int imgFlags = IMG_INIT_PNG;
   if( !( IMG_Init( imgFlags ) & imgFlags ) )
   {
      printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
      success = false;
   }

   // SDL_Surface* loaded_surface = NULL;

   SDL_Texture* loaded_texture = NULL;

   std::map<ImageType_e, std::string>::const_iterator it; 
   for (it = ResourceManager_n::IMAGE_PATHS.begin(); it != ResourceManager_n::IMAGE_PATHS.end(); ++it)
   {
      std::string path_to_image = PATH_TO_ASSETS + it->second;

      loaded_texture = IMG_LoadTexture(path_to_image.c_str());

      if (loaded_texture == NULL)
      {
         printf( "Unable to load texture %s! SDL_image Error: %s\n", path_to_image.c_str(), IMG_GetError() );
         success = false;
      }
      else 
      {
         // converted_texture = SDL_CreateTextureFromSurface(gRenderer, loaded_surface);

         mTextures.insert(std::pair<ImageType_e, SDL_Texture*>(it->first, loaded_texture));

         // SDL_FreeSurface(loaded_surface);
         // loaded_surface = NULL;
      }
   }

   // an alterante location for the call to IMG_Quit()
   // IMG_Quit()

   return success;
}