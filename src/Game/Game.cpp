#include <vector>

#include "Game.hpp"
#include "Piece.hpp"

using namespace Game_n;

Game_c::Game_c() : //  const ResourceManager_n::ResourceManager_c& resource_manager) :
   // mResourceManager(resource_manager),
   mCurrentPlayer(mPlayer1),
   mShootoutFlag(false)
{
   initPieces(mPlayer1, true);
   initPieces(mPlayer2, false);
}

Game_c::~Game_c()
{
   mPlayer1.mPieces.clear();
   mPlayer2.mPieces.clear();
}

void Game_c::move(const std::pair<int, int>& screen_tile_coord)
{
   // if the mouse click event's "screen tile" coordinate is within the board's bounds...
   if (screen_tile_coord.first >= BOARD_COORD.first &&
       screen_tile_coord.first <= BOARD_COORD.first + BOARD_SIDE_LENGTH &&
       screen_tile_coord.second >= BOARD_COORD.second &&
       screen_tile_coord.second >= BOARD_COORD.second + BOARD_SIDE_LENGTH)
   {
      mMovedPiece = mCurrentPiece;

      // copy and store current piece state
      mPreMovePiece = &Piece_n::Piece_c(*mCurrentPiece);
      
      mCurrentPiece->setPosition(screen_tile_coord);
   }
   else 
   {
      emptyCurrentPiece();
   }
}

void Game_c::rotate(const Piece_n::Direction_e& rotate_direction)
{
   if (mCurrentPiece != NULL && mCurrentPiece->getPlayState() == Piece_n::LIVE)
   {
      mMovedPiece = mCurrentPiece;

      // copy and store current piece state
      mPreMovePiece = &Piece_n::Piece_c(*mCurrentPiece);

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
      if (p1_it->getPlayState() == Piece_n::LIVE && p1_it->getPieceType() != Piece_n::PAWN)
      {
         detectHit(*p1_it, mPlayer1.mPieces, hit_pieces);
      } 
   }
   std::vector<Piece_n::Piece_c>::iterator p2_it;
   for (p2_it = mPlayer2.mPieces.begin(); p2_it != mPlayer2.mPieces.end(); ++p2_it)
   {
      if (p2_it->getPlayState() == Piece_n::LIVE && p2_it->getPieceType() != Piece_n::PAWN)
      {
         detectHit(*p2_it, mPlayer2.mPieces, hit_pieces);
      } 
   }

   std::vector<Piece_n::Piece_c>::iterator hit_it;
   for (hit_it = hit_pieces.begin(); hit_it != hit_pieces.end(); ++hit_it)
   {
      // maybe just delete
      hit_it->nextPlayState();
   }

   mShootoutFlag = false;
}

Player_s& Game_c::getCurrentPlayer()
{
   return mCurrentPlayer;
}

void Game_c::nextPlayer()
{
   mCurrentPiece = NULL;

   // if the addresses are equal
   if (&mCurrentPlayer == &mPlayer1)
   {
      mCurrentPlayer = mPlayer2;
   }
   else
   {
      mCurrentPlayer = mPlayer1;
   }
}

Piece_n::Piece_c* Game_c::getCurrentPiece()
{
   return mCurrentPiece;
}

void Game_c::setCurrentPiece(Piece_n::Piece_c& piece)
{
   mCurrentPiece = &piece;
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

Piece_n::Piece_c& Game_c::getMovedPiece()
{
   return *mMovedPiece;
}

Piece_n::Piece_c& Game_c::getPreMovePieceState()
{
   return *mPreMovePiece;
}

void Game_c::revertMove()
{
   mCurrentPiece = NULL;

   // copy constructor
   mMovedPiece = &Piece_n::Piece_c(*mPreMovePiece);
   mPreMovePiece = NULL;
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
   if (player1)
   {
      // initialize the player's starting reserve position (screen tile coordinates)
      std::pair<int, int> reserve_position = P1_RESERVE_COORD;

      // add pawns
      for (int i = 0; i < NUM_PAWNS; ++i)
      {
         Piece_n::Piece_c* new_pawn = 
            new Piece_n::Piece_c(Piece_n::PAWN,
                                 Piece_n::P1_PAWN_DEPLOYMENT_ZONES,
                                 reserve_position,
                                 Piece_n::PLAYER_ONE);
         player.mPieces.push_back(*new_pawn);
         reserve_position.first--;
      }

      // place guns on a new row
      reserve_position = P1_RESERVE_COORD;
      reserve_position.second++;

      // add guns
      for (int i = 0; i < NUM_GUNS; ++i)
      {
         Piece_n::Piece_c* new_gun = 
            new Piece_n::Piece_c(Piece_n::GUN,
                                 Piece_n::P1_GUNSLINGER_DEPLOYMENT_ZONES,
                                 reserve_position,
                                 Piece_n::PLAYER_ONE);
         player.mPieces.push_back(*new_gun);
         reserve_position.first--;
      }

      // place slingers on a new row
      reserve_position = P1_RESERVE_COORD;
      reserve_position.second = reserve_position.second + 2;

      // add slingers
      for (int i = 0; i < NUM_SLINGERS; ++i)
      {
         Piece_n::Piece_c* new_slinger = 
            new Piece_n::Piece_c(Piece_n::SLINGER,
                                 Piece_n::P1_GUNSLINGER_DEPLOYMENT_ZONES,
                                 reserve_position,
                                 Piece_n::PLAYER_ONE);
         player.mPieces.push_back(*new_slinger);
         reserve_position.first--;
      }
   }
   else //player2
   {
      // initialize the player's starting reserve position (screen tile coordinates)
      std::pair<int, int> reserve_position = P2_RESERVE_COORD;

      // add pawns
      for (int i = 0; i < NUM_PAWNS; ++i)
      {
         Piece_n::Piece_c* new_pawn =
            new Piece_n::Piece_c(Piece_n::PAWN,
                                 Piece_n::P2_PAWN_DEPLOYMENT_ZONES,
                                 reserve_position,
                                 Piece_n::PLAYER_TWO);
         player.mPieces.push_back(*new_pawn);
         reserve_position.first++;
      }

      // place guns on a new row
      reserve_position = P2_RESERVE_COORD;
      reserve_position.second--;

      // add guns
      for (int i = 0; i < NUM_GUNS; ++i)
      {
         Piece_n::Piece_c* new_gun =
            new Piece_n::Piece_c(Piece_n::GUN,
                                 Piece_n::P2_GUNSLINGER_DEPLOYMENT_ZONES,
                                 reserve_position,
                                 Piece_n::PLAYER_TWO);
         player.mPieces.push_back(*new_gun);
         reserve_position.first++;
      }

      // place slingers on a new row
      reserve_position = P2_RESERVE_COORD;
      reserve_position.second = reserve_position.second - 2;

      // add slingers
      for (int i = 0; i < NUM_SLINGERS; ++i)
      {
         Piece_n::Piece_c* new_slinger =
            new Piece_n::Piece_c(Piece_n::SLINGER,
                                 Piece_n::P2_GUNSLINGER_DEPLOYMENT_ZONES,
                                 reserve_position,
                                 Piece_n::PLAYER_TWO);
         player.mPieces.push_back(*new_slinger);
         reserve_position.first++;
      }
   }
}

void Game_c::detectHit(Piece_n::Piece_c& piece, 
                       std::vector<Piece_n::Piece_c>& pieces,
                       std::vector<Piece_n::Piece_c>& hit_pieces)
{
   // true if the supplied piece is a slinger; triggers additional hit detections
   bool slinger_flag = false;
   if (piece.getPieceType() == Piece_n::SLINGER) { slinger_flag = true; }

   // true once if we have already hit a piece (monodirectional)
   bool hit_piece_flag = false;
   bool slinger_hit_piece_flag = false;

   // stores the hit piece closest to the shooter
   Piece_n::Piece_c* hit_piece;
   Piece_n::Piece_c* slinger_hit_piece;

   switch (piece.getDirection())
      {
      case Piece_n::UP :
      {
         std::vector<Piece_n::Piece_c>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if (it->getPosition().second < piece.getPosition().second && 
                it->getPosition().second >= BOARD_COORD.second) 
            {
               if (hit_piece_flag)
               {
                  if (it->getPosition().second > hit_piece->getPosition().second)
                  {
                     hit_piece = &(*it);
                  }
               }
               else 
               {
                  hit_piece = &(*it);
                  hit_piece_flag = true;
               }
            }
            if (slinger_flag)
            {
               if (it->getPosition().first > piece.getPosition().first && 
                   it->getPosition().first <= BOARD_COORD.first + BOARD_SIDE_LENGTH) 
               {
                  if (slinger_hit_piece_flag)
                  {
                     if (it->getPosition().first < hit_piece->getPosition().first)
                     {
                        hit_piece = &(*it);
                     }
                  }
                  else 
                  {
                     slinger_hit_piece = &(*it);
                     slinger_hit_piece_flag = true;
                  }
               }
            }
         }

         hit_pieces.push_back(*hit_piece);

         if (slinger_flag)
         {
            hit_pieces.push_back(*slinger_hit_piece);
         }
      }
      case Piece_n::DOWN :
      {
         std::vector<Piece_n::Piece_c>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if (it->getPosition().second > piece.getPosition().second && 
                it->getPosition().second <= BOARD_COORD.second + BOARD_SIDE_LENGTH) 
            {
               if (hit_piece_flag)
               {
                  if (it->getPosition().second < hit_piece->getPosition().second)
                  {
                     hit_piece = &(*it);
                  }
               }
               else 
               {
                  hit_piece = &(*it);
                  hit_piece_flag = true;
               }
            }
            if (slinger_flag)
            {
               if (it->getPosition().first < piece.getPosition().first && 
                   it->getPosition().first >= BOARD_COORD.first) 
               {
                  if (slinger_hit_piece_flag)
                  {
                     if (it->getPosition().first > hit_piece->getPosition().first)
                     {
                        hit_piece = &(*it);
                     }
                  }
                  else 
                  {
                     slinger_hit_piece = &(*it);
                     slinger_hit_piece_flag = true;
                  }
               }
            }
         }

         hit_pieces.push_back(*hit_piece);

         if (slinger_flag)
         {
            hit_pieces.push_back(*slinger_hit_piece);
         }
      }
      case Piece_n::LEFT :
      {
         std::vector<Piece_n::Piece_c>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if (it->getPosition().first < piece.getPosition().first && 
                it->getPosition().first >= BOARD_COORD.first) 
            {
               if (hit_piece_flag)
               {
                  if (it->getPosition().first > hit_piece->getPosition().first)
                  {
                     hit_piece = &(*it);
                  }
               }
               else 
               {
                  hit_piece = &(*it);
                  hit_piece_flag = true;
               }
            }
            if (slinger_flag)
            {
               if (it->getPosition().second < piece.getPosition().second && 
                   it->getPosition().second >= BOARD_COORD.second) 
               {
                  if (slinger_hit_piece_flag)
                  {
                     if (it->getPosition().second > hit_piece->getPosition().second)
                     {
                        hit_piece = &(*it);
                     }
                  }
                  else 
                  {
                     slinger_hit_piece = &(*it);
                     slinger_hit_piece_flag = true;
                  }
               }  
            }
         }

         hit_pieces.push_back(*hit_piece);

         if (slinger_flag)
         {
            hit_pieces.push_back(*slinger_hit_piece);
         }
      }
      case Piece_n::RIGHT :
      {
         std::vector<Piece_n::Piece_c>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if (it->getPosition().first > piece.getPosition().first && 
                it->getPosition().first <= BOARD_COORD.first + BOARD_SIDE_LENGTH) 
            {
               if (hit_piece_flag)
               {
                  if (it->getPosition().first < hit_piece->getPosition().first)
                  {
                     hit_piece = &(*it);
                  }
               }
               else 
               {
                  hit_piece = &(*it);
                  hit_piece_flag = true;
               }
            }
            if (slinger_flag)
            {
               if (it->getPosition().second > piece.getPosition().second && 
                   it->getPosition().second <= BOARD_COORD.second + BOARD_SIDE_LENGTH) 
               {
                  if (slinger_hit_piece_flag)
                  {
                     if (it->getPosition().second < hit_piece->getPosition().second)
                     {
                        hit_piece = &(*it);
                     }
                  }
                  else 
                  {
                     slinger_hit_piece = &(*it);
                     slinger_hit_piece_flag = true;
                  }
               }    
            }
         }

         hit_pieces.push_back(*hit_piece);

         if (slinger_flag)
         {
            hit_pieces.push_back(*slinger_hit_piece);
         }
      }
   }
}

bool Game_c::gameOver()
{
   return false;
}