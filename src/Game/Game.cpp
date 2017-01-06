#include <vector>

#include "Game.hpp"
#include "Piece.hpp"

using namespace Game_n;

Game_c::Game_c(const ResourceManager_n::ResourceManager_c& resource_manager) :
   mResourceManager(resource_manager),
   GameState_s()
{

}

Game_c::~Game_c()
{
}

// void Game_c::start()
// {
//    // load the initial board image (feat. "manholes", deploy zones, etc.)

//    mCurrentTurn = 0; 

//    while (!gameOver())
//    {
//       mCurrentTurn++;

//       // based on who's turn it is, wait for a move from that user

//       drawPieces();
//    }
// }

// void Game_c::move(Piece_n::Piece_c& piece, const int& move_x, const int& move_x)
// {
//    Piece_n::PieceType_e play_state = piece.getPlayState();

//    switch(play_state)
//    {
//       case Piece_n::RESERVE :
//       {
//          if (piece.setPosition(make_pair(x, y)))
//          {
//             piece.nextPlayState();

//             // client also calls for a rotation on deployment
//          }
//          else
//          {
//             // deploymenty failed
//          }
//       }
//       case Piece_n::LIVE :
//       {
//          if (!piece.setPosition(make_pair(x, y)))
//          {
//             // move failed
//          }
//       }
//       case Piece_n::DEAD :
//       {
//          // cannot move a dead piece, shouldn't ever come here
//       }
//    }
// }

// void rotate(Piece_n::Piece_c& piece, const Piece_n::Direction_e& rotate_direction)
// {
//    if (live_piece.getPlayState() == Piece_n::LIVE)
//    {
//       if (live_piece.getPieceType() != Piece_n::PAWN)
//       {
//          live_piece.setDirection(rotate_direction);
//       }
//    }
// }

void Game_c::shootout()
{

}

Player_s& Game_c::currentPlayer()
{
   return mGameState.mCurrentPlayer;
}

void Game_c::nextPlayer()
{
   if (mGameState.mCurrentPlayer == mPlayer1)
   {
      mGameState.mCurrentPlayer = mPlayer2;
   }
   else
   {
      mGameState.mCurrentPlayer = mPlayer1;
   }
}

void Game_c::drawPieces()
{
   const std::vector<Piece_n::Piece_c>& p1_pieces = mPlayer1.mPieces;
   std::vector<Piece_n::Piece_c>::const_iterator p1_it;
   for (p1_it = p1_pieces.begin(); p1_it != p1_pieces.end(); ++p1_it)
   {

   }

   const std::vector<Piece_n::Piece_c>& p2_pieces = mPlayer2.mPieces;
   std::vector<Piece_n::Piece_c>::const_iterator p2_it;
   for (p2_it = p2_pieces.begin(); p2_it != p2_pieces.end(); ++p2_it)
   {

   }
}

void Game_c::detectHit()
{
   
}

bool Game_c::gameOver()
{
   return false;
}