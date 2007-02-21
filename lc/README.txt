    L���Ա���/�������ĵ�


1.  L���Ա�����(������)�ṹ:

    ���������L���Ե�Դ�����ļ�,��parse�Ĺ�����,���õ���tokenд��lex.txt�ļ���.
    ���parse�����ͼ�鶼û�д���,�������������Ԫʽ,����д��quad.txt�ļ���.
    Ȼ�����õĽ����������ִ�����е���Ԫʽ,���еı������ʶ��ӷ��ű��ж�ȡ.
    ��˷��ű��ڱ������в�����¼���е��û���������,���Ҹ��𱣴�����ʱ��ֵ.
	���������lex��yacc��ɴʷ��������﷨�����Ĺ���.

    ���������ܴ����ά����,write�ַ����ȹ���,����֧�ֺ���.
    ͬʱ�������ܹ�������ȡδ��ʼ����������������ֵ������ʱ����.

    ������L���Ե��﷨����(����ʽ)

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

2.  ���ű�����
    
    ������ķ��ű����Hash������. parse�����з����������б���������ӵ�
    ���ű���.ӳ���ϵΪ"������"->����ֵ
    ���з���ֵ�����Ͷ�������:

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

	ʵ�ʵķ���������vtypeָ��,���������ΪInteger,Double,Boolean,String
    ����dims�����������������,��ʱ����ʵ�ʵ�����,v.arrInt,v.arrDbl,v.arrBool��Ч
    ���dimsΪNULL,��ô�Ǳ���,��ʱv.Integer,v.Double,v.Boolean��v.String��Ч

3.  ���Գ���

    p1.l��p2.l�ǲ��Գ���,����p1.l���������п��ƽṹ������,����,������.
    p2.l�������3ά�����ʹ��.

	
    ��Project��Linux��Cygwinƽ̨�Ͼ��ɱ��������
