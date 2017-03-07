#ifndef _STANDOFF_MAIN_CPP_
#define _STANDOFF_MAIN_CPP_

#include <cstdio>
#include "iostream"
#include "StandoffApp/StandoffApp.hpp"

int main(int argc, char* argv[])
{
   // the game instance
   ResourceManager_n::ResourceManager_c resource_manager;

   if (!resource_manager.init())
   {
      printf("Failed to initialize application!\n");
   }
   else
   {
      if (!resource_manager.loadMedia())
      {
         printf("failed to load media!\n");
      }
      else 
      {
         StandoffApp_n::StandoffApp_c standoff_app(resource_manager, 
                                                   resource_manager.getRenderer());
         int exit_reason = standoff_app.run();
      }
   }

   resource_manager.close();

   return 0;
}

#endif _STANDOFF_MAIN_CPP_