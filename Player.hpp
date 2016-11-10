#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <vector>

namespace Player_n
{
   class Player_c
   {
   public:
      // default constructor
      Player_c(void);

      // default destructor
      ~Player_c(void);

      // \Name: GetReservePieces
      // \Description:
      // - returns a vector of this player's reserve pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece>&, the address of this player's reserve pieces
      std::vector<Piece>& getReservePieces(void);

      // \Name: GetLivePieces
      // \Description:
      // - returns a vector of this player's live pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece>&, the address of this player's live pieces
      std::vector<Piece>& getLivePieces(void);

      // \Name: GetDeadPieces
      // \Description:
      // - returns a vector of this player's dead pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece>&, the address of this player's dead pieces
      std::vector<Piece>& getDeadPieces(void);

      // \Name: Shoot
      // \Description:
      // - initiates a shootout event
      // \Argument:
      // - none
      // \Returns
      // - none
      void shoot(void);

   protected:
      // \Name: InitPieces
      // \Description:
      // - initializes mReservePieces; called prior to game start
      // \Argument:
      // - none
      // \Returns
      // - none
      void initPieces(void);

      // piece vectors
      std::vector<Piece> mReservePieces;
      std::vector<Piece> mLivePieces;
      std::vector<Piece> mDeadPieces;
   };
}

#endif _PLAYER_