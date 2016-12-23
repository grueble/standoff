#ifndef _STANDOFF_MAIN_CPP_
#define _STANDOFF_MAIN_CPP_

#include <cstdio>
#include "StandoffApp/StandoffApp.hpp"

int main(int argc, char* argv[])
{
   // the game instance
   StandoffApp_n::StandoffApp_c standoff_app;

   if (!standoff_app.loadMedia())
   {
      printf("Failed to load media!\n");
   }
   else
   {
      if (!standoff_app.init())
      {
         printf("Failed to initialize application!\n");
      }
      else 
      {
         int exit_reason = standoff_app.run();
      }
   }

   standoff_app.close();

   return 0;
}

#endif _STANDOFF_MAIN_CPP_