    L语言编译/解释器文档


1.  L语言编译器(解释器)结构:

    本程序接受L语言的源程序文件,在parse的过程中,将得到的token写到lex.txt文件中.
    如果parse和类型检查都没有错误,编译器会产生四元式,并且写到quad.txt文件中.
    然后内置的解释器会解释执行所有的四元式,所有的变量访问都从符号表中读取.
    因此符号表在本程序中不仅记录所有的用户声明变量,而且负责保存运行时的值.
	本程序借助lex和yacc完成词法分析和语法分析的功能.

    本编译器能处理多维数组,write字符串等功能,但不支持函数.
    同时本程序能够检测出读取未初始化变量，给常量赋值等运行时错误.

    以下是L语言的语法规则(产生式)

    Program: T_Program T_Identifier ';' DeclList T_Begin StmtBlock T_End

    DeclList: Decl ';' DeclList
             | Decl

    Decl: VariableDecl
         | ConstDecl

    Const: T_Identifier T_Equal T_IntConstant
          | T_Identifier T_Equal T_DoubleConstant
          | T_Identifier T_Equal T_BoolConstant

    ConstDeclList: Const
                 | Const ',' ConstDeclList

    ConstDecl: T_Const ConstDeclList

    VariableDecl: Type ':' IdentList
                | Type ':' T_Array ArrayDeclList

    Dims: T_IntConstant
        | Dims ',' T_IntConstant

    ArrayDeclList: T_Identifier '[' Dims ']' ',' ArrayDeclList
                 | T_Identifier '[' Dims ']'

    Type: T_Bool
        | T_Int
        | T_Double

    StmtBlock: Stmt
             | Stmt ';' StmtBlock

    Stmt: Expr
        | T_Begin StmtBlock T_End
        | IfStmt
        | WhileStmt
        | RepeatStmt
        | ForStmt
        | T_Read IdentList
        | T_Write ExprList

    IFTHEN: T_If Expr T_Then

    IfStmt: IFTHEN Stmt

    IfStmt: IFTHEN Stmt T_Else  Stmt

    RepeatStmt: T_Repeat  StmtBlock T_Until Expr

    WhileStmt: T_While  Expr T_Do Stmt

    ForStmt: T_For T_Identifier T_Assign Expr T_To Expr T_Do  Stmt

    Expr: T_IntConstant
        | T_DoubleConstant
        | T_BoolConstant
        | T_StringConstant
        | Expr '+' Expr
        | Expr '*' Expr
        | Expr '/' Expr
        | Expr '>' Expr
        | Expr '<' Expr
        | '-' Expr
        | Expr '-' Expr
        | Expr T_LessEqual Expr
        | Expr T_GreaterEqual Expr
        | Expr T_Equal Expr
        | Expr T_NotEqual Expr
        | Expr T_Or Expr
        | Expr T_And Expr
        | T_Not Expr
        | '(' Expr ')'
        | LValue
        | LValue T_Assign Expr

    ExprList: Expr
            | ExprList ',' Expr

    IdentList: T_Identifier
             | T_Identifier ',' IdentList

    LValue: T_Identifier
          | T_Identifier '[' ExprList ']'

2.  符号表的设计
    
    本程序的符号表采用Hash表的设计. parse过程中分析到的所有变量都会添加到
    符号表中.映射关系为"变量名"->符号值
    其中符号值的类型定义如下:

		struct{
			List<int>* dims;	// NULL if not an array
			bool assigned;
			bool Const;
			ValueType vtype;
			union
			{
				int Integer;
				double Double;
				bool Boolean;
				char* String;
				int* arrInt;
				double* arrDbl;
				bool* arrBool;
			}v;
		}value;

	实际的符号类型由vtype指明,允许的类型为Integer,Double,Boolean,String
    其中dims表明这个符号是数组,此时根据实际的类型,v.arrInt,v.arrDbl,v.arrBool有效
    如果dims为NULL,那么是标量,此时v.Integer,v.Double,v.Boolean或v.String有效

3.  测试程序

    p1.l和p2.l是测试程序,其中p1.l测试了所有控制结构和数组,常量,变量等.
    p2.l则测试了3维数组的使用.

	
    本Project在Linux和Cygwin平台上均可编译和运行
