// Sandra Liu & Victor Wong, CS143 Winter 2000-01

/* Connect Four w/ a Better (Offensive-Tactic-Only) Brain */

/* From Peter Mork's Blackjack */
bool GetYesOrNo(string prompt)
{
   string answer;
   Print(prompt, " (y/n) ");
   answer = ReadLine();
   return (answer == "y" || answer == "Y");
}

/* From Peter Mork's Blackjack */
class rndModule {

  int seed;

  void Init(int seedVal) {
    seed = seedVal;
  }

  int Random() {
    seed = (15625 * (seed % 10000) + 22221) % 65536;
    return seed;
  }

  int RndInt(int max) {
    return (Random() % max);
  }

}

/* Pause 
 * -----
 * Pauses the program for a bit of time roughly determined by 
 * the integer parameter passed in.
 */
void Pause(int sec) {
  int i;
  for (i = 0; i < 100000 * sec; i = i + 1);
}


/* class Point
 * -----------
 * Class structure for storing two integers representing the
 * coordinates of a certain point on the grid.
 * 
 * Methods: SetPoint(x, y), GetCol(), GetRow()
 */
class Point {
  int col;
  int row;

  void SetPoint(int x, int y) {
    col = x;
    row = y;
  }

  int GetCol() {
    return col;
  }

  int GetRow() {
    return row;
  }
}

/* class Board
 * -----------
 * Board maintains the playing grid, represented by a two
 * dimentional array of strings.  Storing strings was 
 * unncessary, integers would have sufficed, but I wanted
 * to just print the contents directly and also use the 
 * null ptr too.
 *
 * Methods: GetColumns(), GetRows(), InitBoard(), ClearBoard(),
 *          DisplayBoard(), CheckColumnDrop(col), MarkDrop(pt, num),
 *          DrawCheck(), FindWinner(pt)
 */
class Board {
  int cols;
  int rows;
  string[][] grid;
 
  int GetColumns() {
    return cols;
  }

  int GetRows() {
    return rows;
  }

  void InitBoard() {
    int i;
    int j;
    
    cols = 7;
    rows = 6;
    
    grid = NewArray(cols, string[]);
    for (i = 0; i < cols; i = i + 1) 
      grid[i] = NewArray(rows, string);
    
    for (i = 0; i < cols; i = i + 1) 
      for (j = 0; j < rows; j = j + 1) 
        grid[i][j] = "";
  }
  
  void ClearBoard() {
    int i;
    int j;

    for (i = 0; i < cols; i = i + 1) 
      for (j = 0; j < rows; j = j + 1) 
        grid[i][j] = "";
  }
    
  void DisplayBoard() {
    int i;
    int j;
    
    Print("\n\n");
    for (j = 0; j < cols; j = j + 1)      
      Print("  ", j + 1, " ");
    Print("\n");
    
    for (i = rows - 1; i >= 0; i = i - 1) {
      Print("-----------------------------\n");
      Print("|");
      
      for (j = 0; j < cols; j = j + 1) {                  
        if(grid[j][i] != "") 
          Print(grid[j][i]);
        else 
          Print("   ");
        Print("|");
      }
      
      Print("\n");
    } 
    Print("-----------------------------\n");
  }

  // returns the row a token would come to rest if 
  // it can validly be placed in the column passed 
  // it can validly be placed in the column passed 
  // in, otherwise -1 is the sentinel returned
  int CheckColumnDrop(int column) {
    int i;
    i = 0;

    if ((column >= 1) && (column <= 7)) {
      while (i < rows) {
        if(grid[column - 1][i] == "") return i;
        i = i + 1;
      }
      return -1; // column full already
    }
    return -1; // outside range of cols
  }

  // updates the grid with the latest token drop
  void MarkDrop(Point pt, int playerNum) {
    if (playerNum == 1) 
      grid[pt.GetCol()][pt.GetRow()] = "[31m(X)[0m";
    else
      grid[pt.GetCol()][pt.GetRow()] = "[30m(O)[0m";
  }
  
  // checks each column to see if all are full as 
  // in the case of a draw between players  
  bool DrawCheck() {
    int i;
    for (i = 0; i < cols; i = i + 1) 
      if (grid[i][rows - 1] == "") return false;
    return true;
  }

  // takes in the most recently dropped token. checks
  // on either side of the token, in the four directions
  // (horizontally, vertically, and 2 diagonal directions);
  // keeps a cumulative count for any given direction aiming
  // to catch four in a row, if found, true returned
  bool FindWinner(Point pt) {
    string target;
    int ptCol;
    int ptRow;
    int i;
    int connection;
    int j;

    ptCol = pt.GetCol();
    ptRow = pt.GetRow();

    if ((ptCol == -1) && (ptRow == -1)) return false; // beginning of game

    target = grid[ptCol][ptRow];

    /* Horizontally */
    connection = 1;
    // To the right
    for (i = ptCol + 1; i < cols; i = i + 1) {
      if (grid[i][ptRow] != "") { // non-null
        if (grid[i][ptRow] == target) connection = connection + 1;
        else break;
      } else break;
    }
    // To the left
    for (i = ptCol - 1; i >= 0; i = i - 1) {
      if (grid[i][ptRow]!="") {
        if (grid[i][ptRow] == target) connection = connection + 1;
        else break;
      } else break;
    }
    if (connection >= 4) return true;
    

    /* Vertically */
    connection = 1;
    // Upwards 
    for (i = ptRow + 1; i < rows; i = i + 1) {
      if (grid[ptCol][i]!="") {
        if (grid[ptCol][i] == target) connection = connection + 1;
        else break;
      } else break;
    }
    // Downwards
    for (i = ptRow - 1; i >= 0; i = i - 1) {
      if (grid[ptCol][i]!="") {
        if (grid[ptCol][i] == target) connection = connection + 1;
        else break;
      } else break;
    }
    if (connection >= 4) return true;



    /* Diagonally NE->SW */
    connection = 1;
    // Upwards
    j = ptRow + 1;
    for (i = ptCol + 1; i < cols; i = i + 1) {
      if (j < rows) {
        if (grid[i][j]!="") {
          if (grid[i][j] == target) connection = connection + 1;
          else break;
        } else break;
      } else break;
      j = j + 1;
    }
    // Downwards
    j = ptRow - 1;
    for (i = ptCol - 1; i >= 0; i = i - 1) {
      if (j >= 0) {
        if (grid[i][j]!="") {
          if (grid[i][j] == target) connection = connection + 1;
          else break;
        } else break;
      } else break;
      j = j - 1;
    }
    if (connection >= 4) return true;


    /* Diagonally NW->SE */
    connection = 1;
    // Upwards
    j = ptRow - 1;
    for (i = ptCol + 1; i < cols; i = i + 1) {
        if (j >= 0) {
          if (grid[i][j]!="") {
            if (grid[i][j] == target) connection = connection + 1;
            else break;
          } else break;
        } else break;
        j = j - 1;
    }
    // Downwards
    j = ptRow + 1;
    for (i = ptCol - 1; i >= 0; i = i - 1) {
      if (j < rows) {
        if (grid[i][j]!="") {
          if (grid[i][j] == target) connection = connection + 1;
          else break;
        } else break;
      } else break;
      j = j + 1;
    }
    if (connection >= 4) return true;

    return false; // no four connected!
  }



  // uses same algorithm as FindWinner() but matches
  // for the computers token and returns a relative score
  // for each column that is the ranking of a column
  // (the number of potential straights) for the 
  // computer's AI...
  int RatePoint(Point pt, string target) {
    int ptCol;
    int ptRow;
    int i;
    int connection;
    int score;
    int j;

    score = 1;
    ptCol = pt.GetCol();
    ptRow = pt.GetRow();

    /* Horizontally */
    connection = 1;
    // To the right
    for (i = ptCol + 1; i < cols; i = i + 1) {
      if (grid[i][ptRow] != "") { // non-null
	if (grid[i][ptRow] == target) connection = connection + 1;
        else break;
      } else break;
    }
    // To the left
    for (i = ptCol - 1; i >= 0; i = i - 1) {
      if (grid[i][ptRow] != "") {
	if (grid[i][ptRow] == target) connection = connection + 1;
        else break;
      } else break;
    }
    if (connection > score) score = connection;

    /* Vertically */
    connection = 1;
    // Upwards 
    for (i = ptRow + 1; i < rows; i = i + 1) {
      if (grid[ptCol][i]!="") {
        if (grid[ptCol][i] == target) connection = connection + 1;
        else break;
      } else break;
    }
    // Downwards
    for (i = ptRow - 1; i >= 0; i = i - 1) {
      if (grid[ptCol][i]!="") {
        if (grid[ptCol][i] == target) connection = connection + 1;
        else break;
      } else break;
    }
    if (connection > score) score = connection;



    /* Diagonally NE->SW */
    connection = 1;
    // Upwards
    j = ptRow + 1;
    for (i = ptCol + 1; i < cols; i = i + 1) {
      if (j < rows) {
        if (grid[i][j]!="") {
          if (grid[i][j] == target) connection = connection + 1;
          else break;
        } else break;
      } else break;
      j = j + 1;
    }
    // Downwards
    j = ptRow - 1;
    for (i = ptCol - 1; i >= 0; i = i - 1) {
      if (j >= 0) {
        if (grid[i][j]!="") {
          if (grid[i][j] == target) connection = connection + 1;
          else break;
        } else break;
      } else break;
      j = j - 1;
    }
    if (connection > score) score = connection;

    /* Diagonally NW->SE */
    connection = 1;
    // Upwards
    j = ptRow - 1;
    for (i = ptCol + 1; i < cols; i = i + 1) {
        if (j >= 0) {
          if (grid[i][j]!="") {
            if (grid[i][j] == target) connection = connection + 1;
            else break;
          } else break;
        } else break;
        j = j - 1;
    }
    // Downwards
    j = ptRow + 1;
    for (i = ptCol - 1; i >= 0; i = i - 1) {
      if (j < rows) {
        if (grid[i][j]!="") {
          if (grid[i][j] == target) connection = connection + 1;
          else break;
        } else break;
      } else break;
      j = j + 1;
    }
    if (connection > score) score = connection;

    return score; // highest connection possible
  }
}

/* class Player
 * ------------
 * Manages the information for a given player, such as name, the
 * player number and the total number of games won.  Also supports
 * the method TakeATurn of inputing a column and doing the appropriate
 * check calls to complete a turn.
 *
 * Methods: SetInfo(name, num), GetName(), GetPlayerNum(), AddWin(),
 *          GetWins(), TakeATurn(board)
 */
class Player {
  string name;
  int playerNum;
  int numWins;

  void SetInfo(string input, int number) {
    name = input;
    playerNum = number;
    numWins = 0;
  }
  
  string GetName() {
    return name;
  }
 
  int GetPlayerNum() {
    return playerNum;
  }
  
  void AddWin() {
    numWins = numWins + 1;
  }

  int GetWins() {
    return numWins;
  }

  Point TakeATurn(Board board) {
    int column;
    int row;
    Point pt;

    pt = New(Point);
  
    Print("Which column would you like to drop your token? (1-7) ");
    column = ReadInteger();
    row = board.CheckColumnDrop(column);
    while (row == -1) {
      Print("That is not a valid column number, please choose again! (1-7) ");
      column = ReadInteger();
      row = board.CheckColumnDrop(column);
    }
    pt.SetPoint(column - 1, row);
    board.MarkDrop(pt, playerNum);
    return pt;
  }
}

/* class Computer
 * --------------
 * Extends class Player.  Overrides 2 methods.  SetInfo takes in the
 * random seed as the second parameter instead of the player number.
 * The computer is always player 2 to let the human user go first.
 * TakeATurn is also overriden and this computer takes a completely
 * random choice at one of the 7 columns.  TakeATurn still does the
 * necessary checks to complete the computer's turn.
 *
 * Methods: overrides SetInfo(name, seed), TakeATurn(board), BestPoint(board)
 */
class Computer extends Player {
  rndModule rnd;

  void SetInfo(string input, int number) {
    name = input;
    playerNum = 2;
    numWins = 0;
    rnd = New(rndModule);
    rnd.Init(number);
  }

 
  Point BestOffPt(Board board) {
    int i;
    int row;
    int ptRank;
    int bestRank;
    int numSame;
    int random;
    Point[] samePts; // array of high ranking options
    Point pt; // currently testing pt
    Point best; // best pt so far
    
    numSame = 0;
    bestRank = 0;
    samePts = NewArray(board.GetColumns(), Point);

    for (i = 1; i <= board.GetColumns(); i = i + 1) {
      row = board.CheckColumnDrop(i);
      pt = New(Point);
      if (row != -1) {
	pt.SetPoint(i - 1, row);
	ptRank = board.RatePoint(pt, "[30m(O)[0m");
	if (ptRank > bestRank) {
	  best = pt;
	  bestRank = ptRank;
	  samePts[0] = pt;  // start *new* array
	  numSame = 1;
	} else if (ptRank == bestRank) {
	  samePts[numSame] = pt;
	  numSame = numSame + 1;
	}
      }
    }

    // if more than one w/ high rank, then 
    // select one of those columns at random
    for (i = 0; i < numSame; i = i + 1) 
      if (numSame > 0) {
       random = rnd.RndInt(numSame);
       best = samePts[random];
    }
    return best;
  }

 Point BestDefPt(Board board) {
    int i;
    int row;
    int ptRank;
    int bestRank;
    int numSame;
    int random;
    Point[] samePts; // array of high ranking options
    Point pt; // currently testing pt
    Point best; // best pt so far
    
    numSame = 0;
    bestRank = 0;
    samePts = NewArray(board.GetColumns(), Point);

    for (i = 1; i <= board.GetColumns(); i = i + 1) {
      row = board.CheckColumnDrop(i);
      pt = New(Point);
      if (row != -1) {
	pt.SetPoint(i - 1, row);
	ptRank = board.RatePoint(pt, "[31m(X)[0m");
	if (ptRank > bestRank) {
	  best = pt;
	  bestRank = ptRank;
	  samePts[0] = pt;  // start *new* array
	  numSame = 1;
	} else if (ptRank == bestRank) {
	  samePts[numSame] = pt;
	  numSame = numSame + 1;
	}
      }
    }

    // if more than one w/ high rank, then 
    // select one of those columns at random
    for (i = 0; i < numSame; i = i + 1) 
      if (numSame > 0) {
       random = rnd.RndInt(numSame);
       best = samePts[random];
    }
    return best;
  }

 Point BestPoint(Board board) {
   Point off;
   Point def;

   // reduntant for lack of time to clean up
   off = BestOffPt(board);
   def = BestDefPt(board);

   if (board.RatePoint(off, "[30m(O)[0m") > 
       board.RatePoint(def, "[31m(X)[0m")) 
     return off;
   return def;
 }
   
  

  // manages a computer's turn
  Point TakeATurn(Board board) {
    int col;
    int row;
    Point pt;
    
    pt = BestPoint(board);
    board.MarkDrop(pt, playerNum);
    
    Pause(1); // let the computer *think*
    
    return pt;
  }
}


/* class Game
 * ----------
 * Manages the overall structure and organization of the game.  Store
 * and instance of the board, and does handling of game on larger level
 * tracking the board and alternating turns between players.  The 
 * start and ending messages of a game are also handled here.
 *
 * Methods: GiveInstructions(), ReportWinner(num), ReportDraw(), InitGame(),
 *          ClearGame(), PlayOneGame()
 */
class Game {
  Board board;
  Player player1;
  Player player2;
  int player;

  void GiveInstructions() {
    Print("\nThis version of ConnectFour supports a one-player version\n");
    Print("as well as a two-player version.  In both versions, players\n");
    Print("(or the computer) take turns choosing a column of the playing\n");
    Print("board to drop one of their tokens into.  The first player to\n");
    Print("make a sequence of four tokens (either horizontally, vertically\n");
    Print("or diagonally) wins!\n\n");
    Print("Hit any key to start playing!\n");
    ReadLine();
  }
    
  void ReportWinner(int playerNum) {
    board.DisplayBoard();
    Print("\n\nCongratulations!  We have a winner!\n");
    if (playerNum == 1) {
      Print("Player #1, ", player1.GetName(), ", has connected FOUR!!\n\n");
      player1.AddWin();
    } else {
      Print("Player #2, ", player2.GetName(), ", has connected FOUR!!\n\n");
      player2.AddWin();
    }

    Print("Overall, ", player1.GetName(), " has ", player1.GetWins());
    if (player1.GetWins() == 1) 
      Print(" win, and ");
    else 
      Print(" wins, and ");
    Print(player2.GetName(), " has ", player2.GetWins());
    if (player2.GetWins() == 1)
      Print(" win.\n\n");
    else 
      Print(" wins.\n\n");
  } 

  void ReportDraw() {
    board.DisplayBoard();
    Print("We have a tie!\n");
    Print("Congrats to both players for keeping up good defenses!\n\n");
  }

  void InitGame() { 
    string name1;
    string name2;
    int players;

    Print("\nWelcome to Decaf ConnectFour!\n");
    Print("-----------------------------\n");
    if (GetYesOrNo("Do you need instructions?")) 
      GiveInstructions();

    Print("How many players? (1 or 2) ");
    players = ReadInteger();
    while(!((players == 1) || (players == 2))) {
      Print("Please choose either 1 or 2 players. ");
      players = ReadInteger();
    }

    if (players == 2) {
      player1 = New(Player);
      player2 = New(Player);

      Print("What is player 1's name? ");
      player1.SetInfo(ReadLine(), 1);
      Print("What is player 2's name? ");
      player2.SetInfo(ReadLine(), 2);
    } else {
      player1 = New(Player);
      Print("What the player's name? ");
      player1.SetInfo(ReadLine(), 1);
      player2 = New(Computer);
      Print("Enter a random seed for the computer: ");
      player2.SetInfo("the Computer", ReadInteger());

    }
    board = New(Board);    
    board.InitBoard();
       
    player = 1; // start w/ player 1
  }       
  
  void ClearGame() {
    board.ClearBoard();
  }

  void PlayOneGame() {
    Point pt;
    pt = New(Point);
    pt.SetPoint(-1, -1);
    
    Print("\nLet's play ConnectFour!\n");

    while(!board.FindWinner(pt)) { // while no winner found
      if(board.DrawCheck()) { // check that board isn't full
        player = 0; // sentinel to indicate neither player won
        break;
      }
      board.DisplayBoard();
      if (player == 1) {
        Print("\n\nIt is ", player1.GetName(), "'s turn! [31m(X)[0m\n"); 
        pt = player1.TakeATurn(board);
        player = 2;
      } else {
        Print("\n\nIt is ", player2.GetName(), "'s turn! [30m(O)[0m\n");
        pt = player2.TakeATurn(board);
        player = 1;
      }
    }

    if (player == 1) 
      ReportWinner(2);
    else if (player == 2) 
      ReportWinner(1);
    else 
      ReportDraw();
  }
}


void main()
{
  Game game;
  bool anotherGame;
  
  anotherGame = true;

  game = New(Game);
  game.InitGame();

  while (anotherGame) {
    game.PlayOneGame();
    anotherGame = GetYesOrNo("Would you like to play another game?");
    game.ClearGame();
  }
  Print("\nThank you for playing Decaf ConnectFour!\n");

}
