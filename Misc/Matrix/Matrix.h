class Matrix
{
public:
	Matrix(int);  //construction function
	~Matrix();  //destruction function
	void input();   //input the Matrix
	void output();  //output the Matrix
	int Lplas(int cur_row);  //answer the result of the Matrix
	                         //use 拉普拉斯变换
private:
	int level;  //the level of Matrix
	
	//Matrix: 
	int mat[10][10];
	//make it 10*10 now

	int col_mark[10];  //0: current col is not in;
	                   //1: current col is in
};
