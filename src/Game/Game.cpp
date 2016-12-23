#include <vector>

#include "Game.hpp"
#include "Piece.hpp"

using namespace Game_n;

Game_c::Game_c(const ResourceManager_n::ResourceManager_c& resource_manager) :
   mPlayer1(), //Player_n::Player_c(0)),
   mPlayer2() //Player_n::Player_c(1))
{
   // store off the IDs of the participating users
   // mUserIds['1'] = 0;
   // mUserIds['2'] = 1;

   // load board
}

Game_c::~Game_c()
{
}

void Game_c::start()
{
   // load the initial board image (feat. "manholes", deploy zones, etc.)

   mCurrentTurn = 0; 

   while (!gameOver())
   {
      mCurrentTurn++;

      // based on who's turn it is, wait for a move from that user

      drawPieces();

      // drawReservePieces();
      // drawLivePieces();
      // drawDeadPieces();
   }
}

// void Game_c::move(Player_n::Player_c& player, Piece_n::Piece_c& piece, int move_x, int move_y)
// {
//    player.move(piece, move_x, move_y);
// }

Player_n::Player_c& Game_c::currentPlayer()
{
   // this is a mediocre way to do this, need to be able to access both players easily, not just current player
}

void Game_c::nextPlayer()
{

}

// const User_c& Game_c::findUserById(int user_id)
// {
   // search the SQL DB for a certain USER, then return
   // do we want to use the User_c& or simply their ID
// }

void Game_c::drawPieces()
{

}

// void Game_c::drawReservePieces()
// {
//    const std::vector<Piece_n::Piece_c>& p1_reserve_pieces = mPlayer1.getReservePieces();
//    std::vector<Piece_n::Piece_c>::const_iterator p1_it;
//    for (p1_it = p1_reserve_pieces.begin(); p1_it != p1_reserve_pieces.end(); ++p1_it)
//    {

//    }

//    const std::vector<Piece_n::Piece_c>& p2_reserve_pieces = mPlayer2.getReservePieces();
//    std::vector<Piece_n::Piece_c>::const_iterator p2_it;
//    for (p2_it = p2_reserve_pieces.begin(); p2_it != p2_reserve_pieces.end(); ++p2_it)
//    {

//    }
// }

// void Game_c::drawLivePieces()
// {
//    const std::vector<Piece_n::Piece_c>& p1_live_pieces = mPlayer1.getLivePieces();
//    std::vector<Piece_n::Piece_c>::const_iterator p1_it;
//    for (p1_it = p1_live_pieces.begin(); p1_it != p1_live_pieces.end(); ++p1_it)
//    {

//    }

//    const std::vector<Piece_n::Piece_c>& p2_live_pieces = mPlayer2.getLivePieces();
//    std::vector<Piece_n::Piece_c>::const_iterator p2_it;
//    for (p2_it = p2_live_pieces.begin(); p2_it != p2_live_pieces.end(); ++p2_it)
//    {

//    }
// }

// void Game_c::drawDeadPieces()
// {
//    const std::vector<Piece_n::Piece_c>& p1_dead_pieces = mPlayer1.getDeadPieces();
//    std::vector<Piece_n::Piece_c>::const_iterator p1_it;
//    for (p1_it = p1_dead_pieces.begin(); p1_it != p1_dead_pieces.end(); ++p1_it)
//    {

//    }

//    const std::vector<Piece_n::Piece_c>& p2_dead_pieces = mPlayer2.getDeadPieces();
//    std::vector<Piece_n::Piece_c>::const_iterator p2_it;
//    for (p2_it = p2_dead_pieces.begin(); p2_it != p2_dead_pieces.end(); ++p2_it)
//    {

//    }
// }

bool Game_c::gameOver()
{
   return false;
}