// Sabine Gounder, CS143 Winter 2000-01



// Adapted from Peter Mork's BlackJack rndModule class
class rndModule {

  int seed;

  void Init(int seedVal) {
    seed = seedVal;
  }

  int Random() {
    seed = (15625 * (seed % 10000) + 22221) % 65536;
    return seed;
  }

  int RndInt(int min, int max) {
    int randomNum;
    randomNum = Random();

    return ((randomNum % (max - min + 1)) + min);
  }
}

// Global
rndModule gRnd;

class Square {
  bool isEmpty;
  string mark;

  void Init() {
    isEmpty = true;
  }

  void PrintSquare() {
    if (isEmpty == false) 
	Print(" ", mark, " ");
    else Print("   ");
  }

  void SetIsEmpty(bool isEmpty) {
    this.isEmpty = isEmpty;
  }
   
  bool GetIsEmpty() {
    return isEmpty;
  }

  void SetMark(string mark)
  {
      this.mark = mark;
  }

  bool IsMarked(string mark) { // Marks are generally 'X' or 'O'
   if (isEmpty) return false;
   else return (this.mark == mark);
  }
}

class Player {
  string mark; // 'X' or 'O'
  string name;
  
  string GetMark() {
    return mark;
  }

  string GetName() {
    return name;
  }
}

class Human extends Player{

  void Init() {
     mark = "X";
     Print("\nYou're playing against the computer.\nEnter your name: ");
     name = ReadLine();
  }

  int GetRow() {
     int row;
     row = -1;

     // Error check that row # is legal
     while (row > 3 || row < 1) {
       Print(name, " enter a row between 1 and 3: ");
       row = ReadInteger();
       if (row > 3 || row < 1) Print("Error: Pick a row between 1 and 3\n");
     }
     row = row - 1;
     return row;
   }

   int GetColumn() { 
     int column;
     column = -1;

     // Error check that the column # is legal
     while (column > 3 || column < 1) {
     	Print(name, " enter a column between 1 and 3: ");
        column = ReadInteger();
	if (column > 3 || column < 1) Print("Error: Pick a column between 1 and 3\n");
      }
      column = column - 1;
      return column;
    }
}


class Grid {
  Square [][] squares;
  
  void Init() {
   int i;
   int j;
   i = 0; 
   this.squares = NewArray(3, Square[]);
   while (i < 3) {
     this.squares[i] = NewArray(3, Square);
     i = i + 1;
   } 
   // Set all the squares to be empty
   i = 0;
   while (i < 3) {
     j = 0;
     while (j < 3) {
        this.squares[i][j] = New(Square);
	this.squares[i][j].Init();
        j = j + 1;
     }
     i = i + 1;
   }
  }

  bool Full() {
    int i;
    int j;
    bool full; 
    full = true;

    for (i = 0; i < 3; i = i + 1) {
      for (j = 0; j < 3; j = j + 1) {
	if (this.squares[i][j].GetIsEmpty()) 
	  full = false;
      }
    }
    return full;
   }

  void Draw() {
    int i;
    int j;
    int rowToPrint;
    i = 0;
    Print("  1   2   3\n");
    while (i < 3) {   
      rowToPrint = i + 1;    
      Print(rowToPrint);
      j = 0;
      while (j < 3) {
 	this.squares[i][j].PrintSquare();
        j = j + 1;
        if (j < 3) Print("|");
      }
      i = i + 1;
      Print("\n");
      if (i < 3) Print(" ---+---+---\n");
   } 	
  }
 
  void Update(int row, int column, string mark) {
    this.squares[row][column].SetIsEmpty(false);
    this.squares[row][column].SetMark(mark);
    this.Draw();
  } 

  bool IsMoveLegal(int row, int column) {
    return this.squares[row][column].GetIsEmpty();
  }

  bool GameNotWon(Player p) {
    string mark;
    mark = p.GetMark();   
    
    if ((this.squares[0][0].IsMarked(mark)) && (this.squares[0][1].IsMarked(mark)) &&  (this.squares[0][2].IsMarked(mark))) 
	return false;
    else if ((this.squares[1][0].IsMarked(mark)) && (this.squares[1][1].IsMarked(mark)) && (this.squares[1][2].IsMarked(mark))) 
	return false;
    else if ((this.squares[2][0].IsMarked(mark)) && (this.squares[2][1].IsMarked(mark)) && (this.squares[2][2].IsMarked(mark))) 
	return false;
    else if ((this.squares[0][0].IsMarked(mark)) && (this.squares[1][0].IsMarked(mark)) && (this.squares[2][0].IsMarked(mark))) 
	return false;
    if ((this.squares[0][1].IsMarked(mark)) && (this.squares[1][1].IsMarked(mark)) && (this.squares[2][1].IsMarked(mark))) 
	return false;
    if ((this.squares[0][2].IsMarked(mark)) && (this.squares[1][2].IsMarked(mark)) && (this.squares[2][2].IsMarked(mark))) 
	return false;
    if ((this.squares[0][0].IsMarked(mark)) && (this.squares[1][1].IsMarked(mark)) && (this.squares[2][2].IsMarked(mark))) 
	return false;
    if ((this.squares[0][2].IsMarked(mark)) && (this.squares[1][1].IsMarked(mark)) && (this.squares[2][0].IsMarked(mark))) 
	return false;
    else
	return true;
  }
  
  // returns if it was able to block the play of a player with the mark X
  bool BlockedPlay(string enemyMark, string playerMark) {
    int row;
    int column;
    
    row = -1;
    column = -1;
   
    // Block row wins -- brute force implementation 
    if (this.squares[0][0].IsMarked(enemyMark) && this.squares[0][1].IsMarked(enemyMark) && !this.squares[0][2].IsMarked(playerMark)) {
 	row = 0;
        column = 2;
    }
    else if (this.squares[1][0].IsMarked(enemyMark) && this.squares[1][1].IsMarked(enemyMark) && !this.squares[1][2].IsMarked(playerMark)) {
 	row = 1;
        column = 2;
    }    
    else if (this.squares[2][0].IsMarked(enemyMark) && this.squares[2][1].IsMarked(enemyMark) && !this.squares[2][2].IsMarked(playerMark)) {
 	row = 2;
        column = 2;
    }    
    else if (this.squares[0][1].IsMarked(enemyMark) && this.squares[0][2].IsMarked(enemyMark) && !this.squares[0][0].IsMarked(playerMark)) {
 	row = 0;
        column = 0;
    }    
    else if (this.squares[1][1].IsMarked(enemyMark) && this.squares[1][2].IsMarked(enemyMark) && !this.squares[1][0].IsMarked(playerMark)) {
 	row = 1;
        column = 0;
    }
    else if (this.squares[2][2].IsMarked(enemyMark) && this.squares[2][1].IsMarked(enemyMark) && !this.squares[2][0].IsMarked(playerMark)) {
 	row = 2;
        column = 0;
    }
    // Block column wins
    else if (this.squares[0][0].IsMarked(enemyMark) && this.squares[1][0].IsMarked(enemyMark) && !this.squares[2][0].IsMarked(playerMark)) {
 	row = 2;
        column = 0;
    }
    else if (this.squares[0][1].IsMarked(enemyMark) && this.squares[1][1].IsMarked(enemyMark) && !this.squares[2][1].IsMarked(playerMark)) {
 	row = 2;
        column = 1;
    }
    else if (this.squares[0][2].IsMarked(enemyMark) && this.squares[1][2].IsMarked(enemyMark) && !this.squares[2][2].IsMarked(playerMark)) {
 	row = 2;
        column = 2;
    }
    else if (this.squares[2][0].IsMarked(enemyMark) && this.squares[1][0].IsMarked(enemyMark) && !this.squares[0][0].IsMarked(playerMark)) {
 	row = 0;
        column = 0;
    }
    else if (this.squares[2][1].IsMarked(enemyMark) && this.squares[1][1].IsMarked(enemyMark) && !this.squares[0][1].IsMarked(playerMark)) {
 	row = 0;
        column = 1;
    }
    else if (this.squares[2][2].IsMarked(enemyMark) && this.squares[1][2].IsMarked(enemyMark) && !this.squares[0][2].IsMarked(playerMark)) {
 	row = 0;
        column = 2;
    }
    // Block diagonal wins
    else if (this.squares[0][0].IsMarked(enemyMark) && this.squares[1][1].IsMarked(enemyMark) && !this.squares[2][2].IsMarked(playerMark)) {
 	row = 2;
        column = 2;
    }
    else if (this.squares[2][2].IsMarked(enemyMark) && this.squares[1][1].IsMarked(enemyMark) && !this.squares[0][0].IsMarked(playerMark)) {
 	row = 0;
        column = 0;
    }
    else if (this.squares[2][0].IsMarked(enemyMark) && this.squares[1][1].IsMarked(enemyMark) && !this.squares[0][2].IsMarked(playerMark)) {
 	row = 0;
        column = 2;
    }
    else if (this.squares[0][2].IsMarked(enemyMark) && this.squares[1][1].IsMarked(enemyMark) && !this.squares[2][0].IsMarked(playerMark)) {
 	row = 2;
        column = 0;
    }

    if (row != -1) {
      Update(row, column, playerMark);
      return true;
    }
    else return false;
  }
}

class Computer extends Player {
  void Init() {
    mark = "0";
  }

  void TakeTurn(Grid grid, Human human) {
    int row;
    int column;
    bool legalMove;    
    legalMove = false;
 
    // First the computer tries to block a win. If it can't, it chooses a
    // square randomly. 
    if (!grid.BlockedPlay(human.GetMark(), this.GetMark())) {
      while (!legalMove) {
      	row = gRnd.RndInt(0,2);
        column = gRnd.RndInt(0,2);
        legalMove = grid.IsMoveLegal(row,column);
      } 

      grid.Update(row, column, this.GetMark());
      row = row + 1;
      column = column + 1;
      Print("\nThe computer's move is row ", row, " and column ", column, ".\n");
    }
    else Print("Ha! The computer blocked you from winning!\n");
  }
}

void InitGame() {
  Print("\nWelcome to TicTacToe!\n");
  Print("---------------------\n");
  Print("Please enter a random number seed: ");
  gRnd.Init(ReadInteger());
}

void main() 
{
  Grid grid;
  Human human;
  Computer computer;
  int row;
  int column;
  bool moveLegal;
  bool gameOver;

  grid = New(Grid);
  human = New(Human);
  computer = New(Computer);
  gRnd = New(rndModule);

  InitGame();
  grid.Init();
  human.Init();
  computer.Init();
  moveLegal = false;
  gameOver = false;
  
  grid.Draw(); 
  while (!gameOver) {
     // The Player Takes A Turn
     moveLegal = false;
     while(!moveLegal) {
     	row = human.GetRow();
     	column = human.GetColumn();
     	moveLegal = grid.IsMoveLegal(row,column);
     	if (!moveLegal) Print("Try again. The square is already taken.\n");
     	else grid.Update(row,column,human.GetMark());
      }
      // If the human didn't win and there isn't a tie, 
      // the computer takes a turn
      if (!grid.GameNotWon(human)) {
	Print(human.GetName(), ", you won!\n"); 
   	gameOver = true;
      }
      else if (!gameOver && grid.Full()) {
	gameOver = true;
	Print("There was a tie...You all lose!\n");
      }
      else 
	computer.TakeTurn(grid, human);
      
      if (!gameOver && !grid.GameNotWon(computer)) {
          Print("Loser -- the computer won!\n");
          gameOver = true;
      }
   }
}

