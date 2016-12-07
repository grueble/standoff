#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <map>

#include "Player.hpp"
#include "ResourceManager/ResourceManager.hpp"

namespace Game_n
{
   class Game_c
   {
   public:
      // constructor
      Game_c(const ResourceManager_n::ResourceManager_c& resource_manager);

      // default destructor
      ~Game_c();

      // \Name: start
      // \Description:
      // - initializes the board and game state
      // \Argument:
      // - none
      // \Returns
      // - none
      void start();

      // \Name: shootout
      // \Description:
      // - initiates a shootout event for both players
      // \Argument:
      // - none
      // \Returns
      // - none
      void shootout();
   protected:
      // \Name: findUserById
      // \Description:
      // - returns the User_c object associated with id parameter
      // \Argument:
      // - int, the user's ID to search for
      // \Returns
      // - const User_c&, a reference to the user whose ID was passed
      // const User_c& findUserById(int id);

      // \Name: drawReservePieces
      // \Description:
      // - draws the reserve Piece_c objects in their proper places
      // \Argument:
      // - none
      // \Returns
      // - none
      void drawReservePieces();

      // \Name: drawLivePieces
      // \Description:
      // - draws the live Piece_c objects in their proper places
      // \Argument:
      // - none
      // \Returns
      // - none
      void drawLivePieces();

      // \Name: drawDeadPieces
      // \Description:
      // - draws the dead Piece_c objects in their proper places
      // \Argument:
      // - none
      // \Returns
      // - none
      void drawDeadPieces();

      // \Name: gameOver
      // \Description:
      // - checks for a win state from either player
      // \Argument:
      // - none
      // \Returns
      // - bool, true if the current player is the winner
      bool gameOver();

      // Store the two Player_c objects created to handle game events
      Player_n::Player_c& mPlayer1;
      Player_n::Player_c& mPlayer2;

      // a map that matches a string representation of an int key to a user's ID
      std::map<char, int> mUserIds;

      // may want to add a container for spectator IDs
      // std::map<char, int> mSpectatorIds;

      // tracks the number of turns the game has been active
      int mCurrentTurn; 

      // key associated with the ID of the user whose turn it is
      char mCurrentUser;

      // reference to the database of all users? (for findUserById)
   };
}

#endif _GAME_HPP_