/* Kenneth Ashcraft, CS143 Winter 2000-01
 * kash@stanford.edu
 */

/* This random module was taken from the blackjack game written by
 * Peter Mork
 */
class RandomModule {

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

  bool TrueOrFalse(){
    return RndInt(2) == 0;
  }

}

RandomModule rand;
bool gameOver;  //a square sets this to false when player steps on mine
bool playerWon;

class Square {
  bool isMine;
  bool isHidden;
  bool hasPlayer;

  void InitSquare()
  {
    isMine = false;
    isHidden = true;
    hasPlayer = false;
  }    

  void SetMine(){
    if(!hasPlayer)
      isMine = true;
  }

  void AcceptPlayer(){
    isHidden = false;
    hasPlayer = true;
    if(isMine){
      gameOver = true;
    }
  }

  void PrintSelf(){
    if(isHidden){
      if(gameOver){
	if(isMine)
	  Print("M");
	else
	  Print("_");
      }else
	Print("#");
      return;
    }
    if(hasPlayer){
      if(gameOver){
	Print("X");
      }else {
	Print("$");
      }
      hasPlayer = false;
      return;
    }
    Print("_");
  }   

  bool HasMine(){
    return isMine;
  }
}

class GoalSquare extends Square {
  void AcceptPlayer(){
    gameOver = true;
    playerWon = true;
  }

  void SetMine(){ }

  void PrintSelf(){
    if(playerWon){
      Print("!");
    }else
      Print("@");
  }
}

class Board {

  Square[][] squares;
  int playerX;
  int playerY;
  int dimension;


  /* Allocate the memory for the board, initializing squares
   * along the way.  Then place the mines.
   */
  void InitBoard(int boardSize){
    int i;
    int j;
    bool hasMine;
    
    dimension = boardSize;
    squares = NewArray(dimension, Square[]);
    for(i = 0; i < dimension; i = i + 1){
      squares[i] = NewArray(dimension, Square);
      for(j = 0; j < dimension; j = j + 1){
	if(i == dimension -1 && j == dimension -1){
	  squares[i][j] = New(GoalSquare);
	  squares[i][j].InitSquare();
	}else{
	  squares[i][j] = New(Square);
	  squares[i][j].InitSquare();
	}
      }
    }
    playerX = 0;
    playerY = 0;
    squares[playerY][playerX].AcceptPlayer();
    AssignMines();
   
  }
    
  /* Randomly try to place a certain number of mines.  If
   * the square has the player, already has a mine, or is
   * the goal square, nothing will happen and a mine is lost.
   */
  void AssignMines(){
    int randX;
    int randY;
    int i;

    for(i = 0; i < 12; i = i + 1){
      randX = rand.RndInt(dimension - 1);
      randY = rand.RndInt(dimension - 1);
      squares[randX][randY].SetMine();
    }
  }

  /* Figure out how many mines are surrounding the player. */
  int NumMinesNear(){
    int i; 
    int j;
    int numMines;

    numMines = 0;
    for(i = playerY - 1; i <= playerY + 1; i = i + 1){
      for(j = playerX - 1; j <= playerX + 1; j = j + 1){
	if(j >= 0 && i >= 0 && 
	   j < dimension && i < dimension){
	  if(squares[i][j].HasMine())
	    numMines = numMines + 1;
	}
      }
    }
    return numMines;
  }

  /* Prints the board and how many mines the player is near. */
  void PrintSelf(){
    int i; 
    int j;

    Print("\n\n\t");
    for(i = 0; i < dimension; i = i + 1){
      for(j = 0; j < dimension; j = j + 1){
	squares[i][j].PrintSelf();
	Print(" ");
      }
      Print("\n\t");
    }
    Print("\n");
    if(!gameOver)
      Print("You are near ", NumMinesNear(), " mine(s).\n");
  }

  /* Changes the player's coordinates and then updates his position.
   * The user can move the player with the numeric keypad. 
   */
  void MovePlayer(string dir){
    if(dir == "8"){
      if(playerY > 0)
	playerY = playerY - 1;
    }else if(dir == "2"){
      if(playerY < dimension - 1)
	playerY = playerY + 1;
    }else if(dir == "4"){
      if(playerX > 0)
	playerX = playerX - 1;
    }else if(dir == "6"){
      if(playerX < dimension - 1)
	playerX = playerX + 1;
    }else if(dir == "7"){
      if(playerY > 0 && playerX > 0){
	playerY = playerY - 1;
	playerX = playerX - 1;
      }
    }else if(dir == "9"){
      if(playerY > 0 && playerX < dimension - 1){
	playerY = playerY - 1;
	playerX = playerX + 1;
      }
    }else if(dir == "3"){
      if(playerY < dimension - 1 && playerX < dimension - 1){
	playerY = playerY + 1;
	playerX = playerX + 1;
      }
    }else if(dir == "1"){
      if(playerY < dimension - 1 && playerX > 0){
	playerY = playerY + 1;
	playerX = playerX - 1;
      }
    }else {
      Print("That's not a direction.\n");
    }
    squares[playerY][playerX].AcceptPlayer();
  }
}

/* I took this function from Peter Mork's blackjack */
bool GetYesOrNo(string prompt)
{
   string answer;
   Print(prompt, " (y/n) ");
   answer = ReadLine();
   return (answer == "y" || answer == "Y");
}

void PrintInst(){
  Print("\nThe object of the game is to move your player from the top\n");
  Print("left of the board to the bottom right without stepping on\n");
  Print("a mine.  You have the ability to detect if you are near to\n");
  Print("mines or not.  A '#' square means that you don't know if\n");
  Print("a mine is there or not.  The player is the '$' symbol.\n\n");
  Print("You can move in any direction using the numeric keypad.\n\n");
}

void SetupGame(){
  Print("\nWelcome to CS143 Minefield!\n");
  if(GetYesOrNo("Would you like instructions?")){
    PrintInst();
  }
  rand = New(RandomModule);
  Print("Please enter a random number seed: ");
  rand.Init(ReadInteger());
}
  

void main(){
  Board board;
  
  gameOver = false;
  playerWon = false;
  SetupGame();
  board = New(Board);
  board.InitBoard(10);
  
  while(!gameOver){
    board.PrintSelf();
    Print("Which direction? ");
    board.MovePlayer(ReadLine());
  }
  board.PrintSelf();
  if(playerWon){
    Print("Congratulations!  You won!\n");
  }else {
    Print("You lose.\n");
  }
  Print("Thanks for playing. :)\n");
  Print("CS143 Minefield by Kenneth Ashcraft\n");
}
