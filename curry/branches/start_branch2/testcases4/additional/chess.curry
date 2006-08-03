// amin-gopals, CS143 Winter 2000-01

// Classes
class Piece {
  bool isWhite;
  bool moved;
  
  // No abstract base classes so just return false here
  bool canMove(int x1, int y1, int x2, int y2) { return false; }

  bool amWhite()                  { return isWhite;       }
  void setColor(bool white)       { isWhite = white;      }

  string getSymbol()
  {
    if (isWhite) return " X";
    else         return "*X";
  }
  
  bool isKing()                   { return false;          }
  
  bool setMoved(bool omoved)      { moved = omoved;        }
  bool hasMoved()                 { return moved;          }
}

class Pawn extends Piece {
  bool canMove(int x1, int y1, int x2, int y2)
  {
    return true;
  }
  
  string getSymbol()
  {
    if (isWhite) return " P";
    else         return "*P";
  }
}

class Bishop extends Piece {
  bool canMove(int x1, int y1, int x2, int y2)
  {
    return true;
  }

  string getSymbol()
  {
    if (isWhite) return " B";
    else         return "*B";
  }
}

class Knight extends Piece {
  bool canMove(int x1, int y1, int x2, int y2)
  {
    return true;
  }

  string getSymbol()
  {
    if (isWhite) return " N";
    else         return "*N";
  }
}

class Rook extends Piece {
  bool canMove(int x1, int y1, int x2, int y2)
  {
    return ((x1 - x2 == 0) || (y1 - y2 == 0));
  }
  
  string getSymbol()
  {
    if (isWhite) return " R";
    else         return "*R";
  }
}

class Queen extends Piece {
  bool canMove(int x1, int y1, int x2, int y2)
  {
    return true;
  }
  
  string getSymbol()
  {
    if (isWhite) return " Q";
    else         return "*Q";
  }
}

class King extends Piece {
  bool canMove(int x1, int y1, int x2, int y2)
  {
    return true;
  }
  
  bool isKing()                 { return true; }
  
  string getSymbol()
  {
    if (isWhite) return " K";
    else         return "*K";
  }
}

class Square {
  bool hasPiece;
  Piece piece;

  int x; int y;
  
  string symbol;
  
  bool hasAPiece() { return hasPiece; }
  
  void setPiece(Piece newPiece) {
    if (newPiece == null) {
      hasPiece = false;
    } else {
      piece = newPiece;
      hasPiece = true;
    }
  }
  
  Piece getPiece() {
    if (hasPiece)
      return piece;
    else
      return null;
  }
  
  string getSymbol() {
    if (hasPiece)
      return piece.getSymbol();
    else
      return ("  ");
  }
  
  void setPos(int posX, int posY) {
    x = posX;
    y = posY;
  }
  
  int getX() { return x;  }
  int getY() { return y;  }
}

class Board {
  Square[][] board;
  int width;
  int height;
  string[] columnLabels;

  void InitPieces(int pieceRow, int pawnRow, bool white) {
    int i;
    Piece piece;
    Square square;
    
    for (i = 0; i < width; i = i + 1) {
      piece = New(Pawn);
      piece.setColor(white);
      square = board[pawnRow][i];
      piece.setMoved(false);
      square.setPiece(piece);
    }
    
    piece = New(King);
    piece.setColor(white);
    piece.setMoved(false);
    board[pieceRow][4].setPiece(piece);
    
    piece = New(Queen);
    piece.setColor(white);
    piece.setMoved(false);
    board[pieceRow][3].setPiece(piece);
    
    piece = New(Bishop);
    piece.setColor(white);
    piece.setMoved(false);
    board[pieceRow][2].setPiece(piece);
    
    piece = New(Bishop);
    piece.setColor(white);
    piece.setMoved(false);
    board[pieceRow][5].setPiece(piece);
    
    piece = New(Knight);
    piece.setColor(white);
    piece.setMoved(false);
    board[pieceRow][1].setPiece(piece);
    
    piece = New(Knight);
    piece.setColor(white);
    piece.setMoved(false);
    board[pieceRow][6].setPiece(piece);
    
    piece = New(Rook);
    piece.setColor(white);
    piece.setMoved(false);
    board[pieceRow][0].setPiece(piece);
    
    piece = New(Rook);
    piece.setColor(white);
    piece.setMoved(false);
    board[pieceRow][7].setPiece(piece);
  }

  void InitBoard() {
    int i;
    int j;
  
  	width = 8;
  	height = 8;

  	board = NewArray(height, Square[]);
  	for (i = 0; i < height; i = i + 1) {
  	  board[i] = NewArray(width, Square);
  	}
  	
  	for (i = 0; i < width; i = i + 1) {
  	  for (j = 0; j < height; j = j + 1) {
  	    board[i][j] = New(Square);
  	    board[i][j].setPos(i, j);
  	    board[i][j].setPiece(null);
  	  }
  	}
  	
  	InitPieces(0, 1, true);
  	InitPieces(7, 6, false);
  }
  
  void PrintBetweenLines() {
    int i;
    
    Print("        |");
    for (i = 0; i < width - 1; i = i + 1) {
      Print("---+");
    }
    Print("---|\n");
  }
  
  void PrintTop() {
    int j;
    
    Print("        -");
    for (j = 0; j < width; j = j + 1) {
      Print("----");
    }
    Print("\n");
  }

  void PrintBottomLabels() {
  	int i;
  	
  	Print("        ");
  	for (i = 0; i < width; i = i + 1) {
  	  Print("  ", i + 1, " ");
  	}
  	Print("\n");
  }
  
  void DrawBoard() {
  	int i;
  	int j;
    Square square;
  
	PrintTop();
	for (i = height - 1; i >= 0; i = i - 1) {
	  Print("     ", i + 1, "  |");
	  for (j = 0; j < width; j = j + 1) {
	  	square = board[i][j];
	    Print(" ", square.getSymbol(), "|");
	  }
	  Print("\n");
	  if (i > 0) {
	    PrintBetweenLines();
	  }
	}
	PrintTop();
	PrintBottomLabels();
  }
  
  int Move(int x1, int y1, int x2, int y2, bool whiteTurn) {
    Square square1;
    Square square2;
    Piece piece;
    
    square1 = board[y1 - 1][x1 - 1];
    square2 = board[y2 - 1][x2 - 1];
    if (!square1.hasAPiece()) {
      Print("*** invalid move: no piece on starting square\n");
      return -1;
    }
    
    if (x1 == x2 && y1 == y2) {
      Print("*** invalid move: cannot move piece on to itself\n");
      return -1;
    }
    
    piece = square1.getPiece();
    
    if (piece.amWhite() != whiteTurn) {
      Print("*** invalid move: cannot move opponents pieces\n");
      return -1;
    }
    
    if (piece.canMove(x1, y1, x2, y2)) {
      // Check if the king has been taken.
      // If so, the game is over!
      if (square2.hasAPiece()) {
        if (square2.getPiece().isKing())
          return 1;
      } else {
        square2.setPiece(piece);
        square1.setPiece(null);
        return 0;
      }
    }
    
    return -1;
  }
}

bool MakeMove(Board board, bool whiteTurn) {
  int x1;
  int y1;
  int x2;
  int y2;
  int gameOver;
  
  string move;

  gameOver = 0;
  x1 = 0;
  y1 = 0;
  x2 = 0; 
  y2 = 0;
  Print("\n*************************************************\n");
  board.DrawBoard();
  if (whiteTurn) {
    Print("It is white's move.\n");
  }
  else {
    Print("It is black's move.\n");
  }
  
  while (x1 < 1 || x1 > 8) {
    Print("  Enter initial column number: ");
    x1 = ReadInteger();
    if (x1 < 1 || x1 > 8) {
      Print("  *** That is not a valid column.\n");
    }
  }
  while (y1 < 1 || y1 > 8) {
    Print("  Enter initial row number: ");
    y1 = ReadInteger();
    if (y1 < 1 || y1 > 8) {
      Print("  *** That is not a valid row.\n");
    }
  }
  
  while (x2 < 1 || x2 > 8) {
    Print("  Enter final column number: ");
    x2 = ReadInteger();
    if (x2 < 1 || x2 > 8) {
      Print("  *** That is not a valid column.\n");
    }
  }
  while (y2 < 1 || y2 > 8) {
    Print("  Enter final row number: ");
    y2 = ReadInteger();
    if (y2 < 1 || y2 > 8) {
      Print("  *** That is not a valid row.\n");
    }
  }

  Print("\n");

  // Board::Move returns -1 if illegal, 0 if legal, and 1 if game over
  gameOver = board.Move(x1, y1, x2, y2, whiteTurn);
  
  if (gameOver == 0) {
    return true;
  } else if (gameOver == 1) {
    return false;
  } else {
    Print("\n*** ILLEGAL MOVE.  TRY AGAIN. ***\n");
    return (MakeMove(board, whiteTurn));
  }
}


void main() {
  bool keepPlaying;
  Board board;
  bool whiteTurn;

  Print("Welcome to CheezyChess!!\n");
  
  board = New(Board);
  board.InitBoard();

  keepPlaying = true;
  whiteTurn = true;
  while (keepPlaying) {
    keepPlaying = MakeMove(board, whiteTurn);
    whiteTurn = !whiteTurn;
  }

  Print("Thank you for playing...come again soon.\n");
  Print("\nCheezyChess Copyright (c) 2001 by Andrew Min and Gopal Santhanam.\n");
}
