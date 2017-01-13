#include <vector>

#include "Game.hpp"
#include "Piece.hpp"

using namespace Game_n;

Game_c::Game_c(const ResourceManager_n::ResourceManager_c& resource_manager) :
   mResourceManager(resource_manager),
   mCurrentPlayer(mPlayer1),
   mShootoutFlag(false)
{
   initPieces(mPlayer1);
   initPieces(mPlayer2);
}

Game_c::~Game_c()
{
   destroyPieces(mPlayer1);
   destroyPieces(mPlayer2);
}

void Game_c::move(const std::pair<int, int>& screen_tile_coord)
{
   // if the mouse click event's "screen tile" coordinate is within the board's bounds...
   if (screen_tile_x_coord >= BOARD_COORD &&
       screen_tile_x_coord <= BOARD_COORD + BOARD_SIDE_LENGTH &&
       screen_tile_y_coord >= BOARD_COORD &&
       screen_tile_y_coord >= BOARD_COORD + BOARD_SIDE_LENGTH)
   {
      mMovedPiece = mCurrentPiece;

      // copy and store current piece state
      mPreMovePieceState = *mCurrentPiece;
      
      mCurrentPiece->setPosition(screen_tile_coord.x, screen_tile_coord.y);
   }
   else 
   {
      current_piece = NULL;
   }
}

void rotate(const Piece_n::Direction_e& rotate_direction)
{
   if (mCurrentPiece != NULL && mCurrentPiece.getPlayState() == Piece_n::LIVE)
   {
      mMovedPiece = mCurrentPiece;

      // copy and store current piece state
      mPreMovePieceState = *mCurrentPiece;

      mCurrentPiece->setDirection(rotate_direction);
   }
}

void Game_c::shootout()
{
   // container for all pieces that were hit
   std::vector<Piece_n::Piece_c> hit_pieces;

   std::vector<Piece_n::Piece_c>::iterator p1_it;
   for (p1_it = mPlayer1.mPieces.begin(); p1_it != mPlayer1.mPieces.end(); ++p1_it)
   {
      if (it->getPlayState() == Piece_n::LIVE && it->getPieceType() != Piece_n::PAWN)
      {
         detectHit(*it, mPlayer1.mPieces, hit_pieces);
      } 
   }
   std::vector<Piece_n::Piece_c>::iterator p2_it;
   for (p2_it = mPlayer2.mPieces.begin(); p2_it != mPlayer2.mPieces.end(); ++p2_it)
   {
      if (it->getPlayState() == Piece_n::LIVE && it->getPieceType() != Piece_n::PAWN)
      {
         detectHit(*it, mPlayer2.mPieces&, hit_pieces);
      } 
   }

   std::vector<Piece_n::Piece_c>::iterator hit_it;
   for (hit_it = hit_pieces.begin(); hit_it != hit_pieces.end(); ++hit_it)
   {
      // maybe just delete
      it->nextPlayState();
   }

   mShootoutFlag = false;
}

Player_s& Game_c::getCurrentPlayer()
{
   return mCurrentPlayer;
}

void Game_c::nextPlayer()
{
   if (mCurrentPlayer == mPlayer1)
   {
      mCurrentPlayer = mPlayer2;
   }
   else
   {
      mCurrentPlayer = mPlayer1;
   }
}

Piece_n::Piece_c& Game_c::getCurrentPiece()
{
   return mCurrentPiece;
}

void Game_c::emptyCurrentPiece()
{
   mCurrentPiece = NULL;
}

const std::vector<Piece_n::Piece_c>& Game_c::getPlayer1Pieces()
{
   return mPlayer1.mPieces;
}

const std::vector<Piece_n::Piece_c>& Game_c::getPlayer2Pieces()
{
   return mPlayer2.mPieces;
}

const Piece_n::Piece_c& Game_c::getMovedPiece()
{
   return *(&mMovedPiece);
}

const Piece_n::Piece_c& Game_c::getPreMovePieceState()
{
   return &mPreMovePieceStateState;
}

void Game_c::revertMove()
{
   // copy constructor
   *mMovedPiece = mPreMovePieceState;
   mMovedPiece = NULL;
}

const bool& Game_c::getShootoutFlag()
{
   return mShootoutFlag;
}

void Game_c::setShootoutFlag()
{
   mShootoutFlag = true;
}

void Game_c::initPieces(Player_s& player, bool player1)
{
   // initialize the player's starting reserve position (screen tile coordinates)
   std::pair<int, int> reserve_position = 
      player1 ? PLAYER_1_RESERVE_COORD : PLAYER_2_RESERVE_COORD;

   // add pawns
   for (int i = 0; i < NUM_PAWNS; ++i)
   {
      player1 ? reserve_position.first - 1 : reserve_position.first + 1;
      Piece_n::Piece_c* new_pawn = new Piece_n::Piece_c(Piece_n::PAWN);
      mPieces.push_back(*new_pawn);
   }

   // place guns on a new row
   player1 ? reserve_position.y + 1 : reserve_position.y - 1;

   // add guns
   for (int i = 0; i < NUM_GUNS; ++i)
   {
      player1 ? reserve_position.first - 1 : reserve_position.first + 1;
      Piece_n::Piece_c* new_gun = new Piece_n::Piece_c(Piece_n::PAWN);
      mPieces.push_back(*new_gun);
   }

   // place slingers on a new row
   player1 ? reserve_position.second + 1 : reserve_position.second - 1;

   // add slingers
   for (int i = 0; i < NUM_SLINGERS; ++i)
   {
      player1 ? reserve_position.first - 1 : reserve_position.first + 1;
      Piece_n::Piece_c>* new_slinger = new Piece_n::Piece_c(Piece_n::PAWN);
      mPieces.push_back(*new_slinger);
   }
}

void Game_c::destroyPieces(Player_s& player)
{
   std::vector<Piece_n::Piece_c>::iterator it;
   for (it = pieces.begin(); it != pieces.end(); ++it)
   {
      delete *it; 
   }
}

void Game_c::detectHit(const Piece_n::Piece_c& piece, 
                       const std::vector<Piece_n::Piece_c&>& pieces
                       std::vector<Piece_n::Piece_c>& hit_pieces)
{
   // true if the supplied piece is a slinger; triggers additional hit detections
   bool slinger_flag = false;
   if (piece.getPieceType()) { slingerFlag = true; }

   // true once if we have already hit a piece (monodirectional)
   bool hit_piece = false;
   bool slinger_hit_piece = false;

   // stores the hit piece closest to the shooter
   Piece_n::Piece_c hit_piece;
   Piece_n::Piece_c slinger_hit_piece;

   switch (piece.getDirection())
      {
      case UP :
      {
         std::vector<Piece_n::Piece_c>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if (it->getPosition().second < piece.getPosition().second && 
                it->getPosition().second >= BOARD_COORD.second) 
            {
               if (hit_piece)
               {
                  if (it->getPosition().second > hit_piece.getPosition().second)
                  {
                     hit_piece = &(*it);
                  }
               }
               else 
               {
                  hit_piece = &(*it);
                  hit_piece = true;
               }
            }
            if (slinger_flag)
            {
               if (it->getPosition().first > piece.getPosition().first && 
                   it->getPosition().first <= BOARD_COORD.first + BOARD_SIDE_LENGTH) 
               {
                  if (slinger_hit_piece)
                  {
                     if (it->getPosition.first < hit_piece.getPosition().first)
                     {
                        hit_piece = &(*it);
                     }
                  }
                  else 
                  {
                     slinger_hit_piece = &(*it);
                     slinger_hit_piece = true;
                  }
               }
            }
         }

         hit_pieces.push_back(hit_piece);

         if (slinger_flag)
         {
            hit_pieces.push_back(slinger_hit_piece);
         }
      }
      case DOWN :
      {
         std::vector<Piece_n::Piece_c>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if (it->getPosition().second > piece.getPosition().second && 
                it->getPosition().second <= BOARD_COORD.second + BOARD_SIDE_LENGTH) 
            {
               if (hit_piece)
               {
                  if (it->getPosition().second < hit_piece.getPosition().second)
                  {
                     hit_piece = &(*it);
                  }
               }
               else 
               {
                  hit_piece = &(*it);
                  hit_piece = true;
               }
            }
            if (slinger_flag)
            {
               if (it->getPosition().first < piece.getPosition().first && 
                   it->getPosition().first >= BOARD_COORD.first) 
               {
                  if (slinger_hit_piece)
                  {
                     if (it->getPosition.first > hit_piece.getPosition().first)
                     {
                        hit_piece = &(*it);
                     }
                  }
                  else 
                  {
                     slinger_hit_piece = &(*it);
                     slinger_hit_piece = true;
                  }
               }
            }
         }

         hit_pieces.push_back(hit_piece);

         if (slinger_flag)
         {
            hit_pieces.push_back(slinger_hit_piece);
         }
      }
      case LEFT :
      {
         std::vector<Piece_n::Piece_c>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if (it->getPosition().first < piece.getPosition().first && 
                it->getPosition().first >= BOARD_COORD.first) 
            {
               if (hit_piece)
               {
                  if (it->getPosition().first > hit_piece.getPosition().first)
                  {
                     hit_piece = &(*it);
                  }
               }
               else 
               {
                  hit_piece = &(*it);
                  hit_piece = true;
               }
            }
            if (slinger_flag)
            {
               if (it->getPosition().second < piece.getPosition().second && 
                   it->getPosition().second >= BOARD_COORD.second) 
               {
                  if (slinger_hit_piece)
                  {
                     if (it->getPosition.second > hit_piece.getPosition().second)
                     {
                        hit_piece = &(*it);
                     }
                  }
                  else 
                  {
                     slinger_hit_piece = &(*it);
                     slinger_hit_piece = true;
                  }
               }  
            }
         }

         hit_pieces.push_back(hit_piece);

         if (slinger_flag)
         {
            hit_pieces.push_back(slinger_hit_piece);
         }
      }
      case RIGHT :
      {
         std::vector<Piece_n::Piece_c>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if (it->getPosition().first > piece.getPosition().first && 
                it->getPosition().y <= BOARD_COORD.first + BOARD_SIDE_LENGTH) 
            {
               if (hit_piece)
               {
                  if (it->getPosition().first < hit_piece.getPosition().first)
                  {
                     hit_piece = &(*it);
                  }
               }
               else 
               {
                  hit_piece = &(*it);
                  hit_piece = true;
               }
            }
            if (slinger_flag)
            {
               if (it->getPosition().second > piece.getPosition().second && 
                   it->getPosition().second <= BOARD_COORD.second + BOARD_SIDE_LENGTH) 
               {
                  if (slinger_hit_piece)
                  {
                     if (it->getPosition.second < hit_piece.getPosition().second)
                     {
                        hit_piece = &(*it);
                     }
                  }
                  else 
                  {
                     slinger_hit_piece = &(*it);
                     slinger_hit_piece = true;
                  }
               }    
            }
         }

         hit_pieces.push_back(hit_piece);

         if (slinger_flag)
         {
            hit_pieces.push_back(slinger_hit_piece);
         }
      }
   }
}

bool Game_c::gameOver()
{
   return false;
}