#include "Game.hpp"
#include "Player.hpp"

class Game
{
public:

   // default constructor
   Game(Player p1, Player p2)
   {
      // connect to the players' devices

      init_game();
   }

   // default destructor
   ~Game()
   {}

private:

   // set up the abstract board object
   void init_game()
   {
      // for each player, init_pieces()
   }
}