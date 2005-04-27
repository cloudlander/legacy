/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "symtable.h"
#include "errors.h"

// code implementation for type system/checking and code gen
Location* dummy_loc=new Location(fpRelative,0,"blah");		// used as a dummy operand

Type* ArithmeticExpr::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type *lefttype=NULL,*righttype;
	if(left)
	{
		lefttype=left->GetType(symtbl);
		Assert(lefttype);
		if(lefttype->IsEquivalentTo(Type::errorType))
			type=Type::errorType;
	}

	Assert(right);
	righttype=right->GetType(symtbl);
	Assert(righttype);
	if(righttype->IsEquivalentTo(Type::errorType))
		type=Type::errorType;

	if(type && type->IsEquivalentTo(Type::errorType))
		return type;
	
	if(NULL!=left && (lefttype->IsEquivalentTo(Type::intType) &&			// bug fixed '(' and ")' issue
	   righttype->IsEquivalentTo(Type::intType) ||
	   lefttype->IsEquivalentTo(Type::doubleType) &&
	   righttype->IsEquivalentTo(Type::doubleType))) 
		return type=lefttype;
	else if(NULL==left && ( righttype->IsEquivalentTo(Type::intType) ||		// bug fixed '(' and ')' issue
			righttype->IsEquivalentTo(Type::doubleType)) )
		return type=righttype;
	else
	{
//		Failure("type not match");
		if(left)
			ReportError::IncompatibleOperands(op,lefttype,righttype);
		else
			ReportError::IncompatibleOperand(op,righttype);
		return type=Type::errorType;
	}
}

Type* RelationalExpr::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type *lefttype,*righttype;
	Assert(left);
	lefttype=left->GetType(symtbl);
	Assert(lefttype);
	if(lefttype->IsEquivalentTo(Type::errorType))
		type=Type::errorType;

	Assert(right);
	righttype=right->GetType(symtbl);
	Assert(righttype);
	if(righttype->IsEquivalentTo(Type::errorType))
		type=Type::errorType;
	
	if(type && type->IsEquivalentTo(Type::errorType))
		return type;

	if(lefttype->IsEquivalentTo(Type::intType) &&
	   righttype->IsEquivalentTo(Type::intType) ||
	   lefttype->IsEquivalentTo(Type::doubleType) &&
	   righttype->IsEquivalentTo(Type::doubleType)) 
		return type=Type::boolType;
	else
	{
//		Failure("type not match");
		ReportError::IncompatibleOperands(op,lefttype,righttype);
		return type=Type::errorType;
	}
}

Type* EqualityExpr::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type *lefttype,*righttype;
	Assert(left);
	lefttype=left->GetType(symtbl);
	Assert(lefttype);
	if(lefttype->IsEquivalentTo(Type::errorType))
		type=Type::errorType;
	
	Assert(right);
	righttype=right->GetType(symtbl);
	Assert(righttype);
	if(righttype->IsEquivalentTo(Type::errorType))
		type=Type::errorType;

	if(type && type->IsEquivalentTo(Type::errorType))
		return type;

	if(lefttype->IsEquivalentTo(righttype))
		return type=Type::boolType;
	if(lefttype->IsCompatibleTo(righttype))
		return type=Type::boolType;
	if(righttype->IsCompatibleTo(lefttype))
		return type=Type::boolType;
//	Failure("type not match");
	ReportError::IncompatibleOperands(op,lefttype,righttype);
	return type=Type::errorType;
}
	
Type* LogicalExpr::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type *lefttype,*righttype;
	if(left)
	{
		lefttype=left->GetType(symtbl);
		Assert(lefttype);
		if(lefttype->IsEquivalentTo(Type::errorType))
			type=Type::errorType;
	}

	Assert(right);
	righttype=right->GetType(symtbl);
	Assert(righttype);
	if(righttype->IsEquivalentTo(Type::errorType))
		type=Type::errorType;
	
	if(type && type->IsEquivalentTo(Type::errorType))
		return type;

	if(left && lefttype->IsEquivalentTo(Type::boolType)
			&& righttype->IsEquivalentTo(Type::boolType))
		return type=Type::boolType;
	else
		if(righttype->IsEquivalentTo(Type::boolType))
			return type=Type::boolType;

//	Failure("type not match");
	if(left)
		ReportError::IncompatibleOperands(op,lefttype,righttype);
	else
		ReportError::IncompatibleOperand(op,righttype);

	return type=Type::errorType;	
}

Type* AssignExpr::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type *lefttype,*righttype;
	Assert(left);
	lefttype=left->GetType(symtbl);
	Assert(lefttype);
	if(lefttype->IsEquivalentTo(Type::errorType))
		type=Type::errorType;
	
	Assert(right);
	righttype=right->GetType(symtbl);
	Assert(righttype);
	if(righttype->IsEquivalentTo(Type::errorType))
		type=Type::errorType;

	if(type && type->IsEquivalentTo(Type::errorType))
		return type;

	if(righttype->IsEquivalentTo(lefttype) ||
		righttype->IsCompatibleTo(lefttype))
		return type=lefttype;
//	Failure("type not match");
	ReportError::IncompatibleOperands(op,lefttype,righttype);
	return type=Type::errorType;
}

Type* This::GetType(SymTable* symtbl)
{
	if(type) return type;
	
	Symbol* sym=symtbl->Find("this",true);
	if(sym)
	{
		Assert(sym->GetDecl() && sym->GetDecl()->GetType());
		return type=sym->GetDecl()->GetType();
	}
	else
	{
		ReportError::ThisOutsideClassScope(this);
		return type=Type::errorType;
	}
}

Type* ArrayAccess::GetType(SymTable* symtbl)
{
	if(type) return type;

	Assert(base);
	Type* basetype=base->GetType(symtbl);
	Assert(basetype);
	if(basetype->IsEquivalentTo(Type::errorType))
		type=Type::errorType;
	  
	Type* subscriptType=subscript->GetType(symtbl);
	if(!subscriptType->IsEquivalentTo(Type::intType))
	{
		ReportError::SubscriptNotInteger(subscript);
		type=Type::errorType;
	}

	if(type && type->IsEquivalentTo(Type::errorType))
		return type;

	if(typeid(ArrayType) == typeid(*basetype))
		return type=static_cast<ArrayType*>(basetype)->GetElemType();
	else
	{
//		Failure("array type not match");
		ReportError::BracketsOnNonArray(base);
		return type=Type::errorType;
	}
}

Type* FieldAccess::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type* basetype;
	if(base)
	{
		basetype=base->GetType(symtbl);
		Assert(basetype);
		if(basetype->IsEquivalentTo(Type::errorType))
			return type=Type::errorType;

		if(typeid(NamedType)!=typeid(*basetype))
		{
//			Failure("base not object type");
			ReportError::FieldNotFoundInBase(field,basetype);
			return type=Type::errorType;
		}
		const char* classname=static_cast<NamedType*>(basetype)->GetName();			
		Symbol* symclass=GetGlobalSymTable()->Find(classname);
		if(NULL==symclass)			// seems never happening
		{	
			Failure("type of base is an invalid class");
			return type=Type::errorType;
		}
		Symbol* symfield=symclass->GetDecl()->GetSymTable()->Find(field->GetName(),true);
		if(NULL==symfield || ! symfield->IsClassVar())
		{
//			Failure("field not found in class");
			ReportError::FieldNotFoundInBase(field,basetype);
			return type=Type::errorType;
		}
		/* should check the accessibility of field */
		Symbol* symthis=symtbl->Find("this",true);
		if(NULL==symthis)	// FieldAccess should be in Method
		{
			ReportError::InaccessibleField(field,basetype);
			return type=Type::errorType;
		}
		if(! symthis->GetDecl()->GetType()->IsCompatibleTo(basetype)) // check if the basetype is in its subclass
		{
			ReportError::InaccessibleField(field,basetype);
			return type=Type::errorType;
		}
		return type=symfield->GetDecl()->GetType();
	}
	else
	{
		Symbol* symfield=symtbl->Find(field->GetName(),true);
 #if 0
		if(NULL==symfield)	// try "this" as base , this seems omittable !!!!
		{
			Symbol* symthis=symtbl->Find("this",true);
			if(NULL==symthis)
			{
				Failure("symbol:%s (%d,%d) not found",field->GetName(),location->first_line,location->first_column);
				return type=Type::errorType;
			}
			Type* basetype=symthis->GetDecl()->GetType();
			const char* classname=static_cast<NamedType*>(basetype)->GetName();
			Symbol* symclass=GetGlobalSymTable()->Find(classname);
			if(NULL==symclass)
			{
				Failure("type of base is an invalid class");
				return type=Type::errorType;
			}
			Symbol* symfield=symclass->GetDecl()->GetSymTable()->Find(field->GetName(),true);
			if(NULL==symfield || ! symfield->IsClassVar())
			{
				Failure("field not found in class");
				return type=Type::errorType;
			}
			return type=symfield->GetDecl()->GetType();
		}
#endif
		if(NULL==symfield)
		{
			ReportError::IdentifierNotDeclared(field,LookingForVariable);
			return type=Type::errorType;
		}
		else if(!symfield->IsClassVar() && !symfield->IsGlobalVar() && !symfield->IsLocalVar())
		{
			ReportError::IdentifierNotDeclared(field,LookingForVariable);
			return type=Type::errorType;
		}

		return type=symfield->GetDecl()->GetType();
	}
}

void Call::CheckActuals(Decl* fnDecl,SymTable* symtbl)
{
	FnDecl* callee=static_cast<FnDecl*>(fnDecl);
	List<VarDecl*>* calleeFormals=callee->GetFormals();
	/* check the actual parameters */
	if(calleeFormals->NumElements() != actuals->NumElements())
	{
		ReportError::NumArgsMismatch(field, calleeFormals->NumElements(), actuals->NumElements());
		return;
	}
	
	Type* actualType;
	
	for(int i=0;i<calleeFormals->NumElements();i++)
	{
		actualType=actuals->Nth(i)->GetType(symtbl);
		if(! calleeFormals->Nth(i)->GetType()->IsEquivalentTo(actualType) && 
		   ! actualType->IsCompatibleTo(calleeFormals->Nth(i)->GetType()))
			ReportError::ArgMismatch(actuals->Nth(i),i+1,actualType,calleeFormals->Nth(i)->GetType());
	}
}

Type* Call::GetType(SymTable* symtbl)
{
	if(type) return type;

	/* bug fixed: must check all the actuals first */
	for(int i=0;i<actuals->NumElements();i++)
		actuals->Nth(i)->Check(symtbl);
	
	Type* basetype;
	if(base)
	{
		basetype=base->GetType(symtbl);
		Assert(basetype);
		if(basetype->IsEquivalentTo(Type::errorType))
			return type=Type::errorType;

		if(typeid(ArrayType)==typeid(*basetype))
		{
			if(0==strcmp("length",field->GetName()))
				return type=Type::intType;
			else
			{
//				Failure("length could only occur under array");
				ReportError::FieldNotFoundInBase(field,basetype);
				return type=Type::errorType;
			}
		}
		if(typeid(NamedType)!=typeid(*basetype))
		{
//			Failure("base not object type");
//			ReportError::CallMethodOnNonObject(base);
			ReportError::FieldNotFoundInBase(field,basetype);
			return type=Type::errorType;
		}
		const char* classname=static_cast<NamedType*>(basetype)->GetName();			
		Symbol* symclass=GetGlobalSymTable()->Find(classname);
		if(NULL==symclass)		// seems never happening
		{
			Failure("type of base is an invalid class");
			return type=Type::errorType;
		}
		Symbol* symfield=symclass->GetDecl()->GetSymTable()->Find(field->GetName(),true);
		if(NULL==symfield || ! symfield->IsMethod())
		{
//			Failure("field not found in class");
			ReportError::FieldNotFoundInBase(field,basetype);
			return type=Type::errorType;
		}
		
		CheckActuals(symfield->GetDecl(),symtbl);
	
		type=symfield->GetDecl()->GetType();
		if(type->IsEquivalentTo(Type::errorType))		// maybe this method has unmatched return type
			return type=Type::errorType;
		return type;
	
	}
	else
	{
		Symbol* symfield=symtbl->Find(field->GetName(),true);
		if(NULL==symfield)	// try "this" as base
		{
//			Failure("field not found in class");
			ReportError::IdentifierNotDeclared(field,LookingForFunction);
			return type=Type::errorType;
		}else if(!symfield->IsFunction() && !symfield->IsMethod())
		{
			ReportError::IdentifierNotDeclared(field,LookingForFunction);
			return type=Type::errorType;
		}
		
		CheckActuals(symfield->GetDecl(),symtbl);

		return type=symfield->GetDecl()->GetType();
	}
}

Type* ConditionalExpr::GetType(SymTable* symtbl)
{
	if(type) return type;
	Type *expr1type,*expr2type,*expr3type;
	expr1type=expr1->GetType(symtbl);
	Assert(expr1type);
	if(expr1type->IsEquivalentTo(Type::errorType))
		type=Type::errorType;
	
	expr2type=expr2->GetType(symtbl);
	Assert(expr2type);
	if(expr2type->IsEquivalentTo(Type::errorType))
		type=Type::errorType;
			
	expr3type=expr3->GetType(symtbl);
	Assert(expr3type);
	if(expr3type->IsEquivalentTo(Type::errorType))
		type=Type::errorType;
/*
 	if(type && type->IsEquivalentTo(Type::errorType))
		return type;
*/
	if(!expr1type->IsEquivalentTo(Type::errorType) && !expr1type->IsEquivalentTo(Type::boolType))
	{
//		Failure("conditional expr1 not bool type!");
		ReportError::TestNotBoolean(expr1);
//		return type=Type::errorType;
		type=Type::errorType;
	}
	if(expr2type->IsEquivalentTo(expr3type) || 
	   expr3type->IsCompatibleTo(expr2type) )
	{
		if(!type->IsEquivalentTo(Type::errorType))
			return type=expr2type;
	}
	else if(expr2type->IsCompatibleTo(expr3type))
	{
		if(!type->IsEquivalentTo(Type::errorType))
			return type=expr3type;
	}
	else
	{
//		Failure("conditional expr2's type not equal to expr3's type");
		ReportError::ConditionalExprUnmatch(op2,expr2type,expr3type);
		type=Type::errorType;
	}
	return type;
}

Location* IntConstant::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenLoadConstant(value,symtbl);
}

Location* DoubleConstant::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Failure("DOUBLE NOT SUPPORT!");
	return NULL;
}

Location* BoolConstant::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenLoadConstant(value ? 1 : 0,symtbl);
}

Location* StringConstant::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenLoadConstant(value,symtbl);
}

Location* NullConstant::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenLoadConstant(0,symtbl);
}

Location* ArithmeticExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location *lhs,*rhs;
	if(left)
	{
		lhs=left->GenTac(cg,symtbl);
		if(lhs->IsPointer())
			lhs=cg->GenLoad(lhs,0,symtbl);
	}
	else
		lhs=cg->GenLoadConstant(0,symtbl);
	rhs=right->GenTac(cg,symtbl);
	
	if(rhs->IsPointer())
		rhs=cg->GenLoad(rhs,0,symtbl);

	return cg->GenBinaryOp(op->GetOperName(),lhs,rhs,symtbl);
}


Location* RelationalExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location *lhs,*rhs;
	lhs=left->GenTac(cg,symtbl);
	
	if(lhs->IsPointer())
		lhs=cg->GenLoad(lhs,0,symtbl);

	rhs=right->GenTac(cg,symtbl);
	
	if(rhs->IsPointer())
		rhs=cg->GenLoad(rhs,0,symtbl);


	const char* opname=op->GetOperName();
	if(0==strncmp(">=",opname,2))
		return cg->GenBinaryOp("<=",rhs,lhs,symtbl);
	if(0==strncmp("<=",opname,2))
		return cg->GenBinaryOp("<=",lhs,rhs,symtbl);
	if(0==strncmp("<",opname,1))
		return cg->GenBinaryOp("<",lhs,rhs,symtbl);
	if(0==strncmp(">",opname,1))
		return cg->GenBinaryOp("<",rhs,lhs,symtbl);
	Failure("SHOULD NOT REACH HERE");
	return NULL;
}

Location* EqualityExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location *lhs,*rhs;
	lhs=left->GenTac(cg,symtbl);
	
	if(lhs->IsPointer())
		lhs=cg->GenLoad(lhs,0,symtbl);

	rhs=right->GenTac(cg,symtbl);
	
	if(rhs->IsPointer())
		rhs=cg->GenLoad(rhs,0,symtbl);

	if(left->GetType(symtbl)->IsEquivalentTo(Type::stringType) &&
	  right->GetType(symtbl)->IsEquivalentTo(Type::stringType))		// string == string
	{
//		return cg->GenBuiltInCall(StringEqual,lhs,rhs,symtbl);
		if(0==strcmp("==",op->GetOperName()))
			return cg->GenThunkCall(EqualString,lhs,rhs,symtbl);
		else
			return cg->GenBinaryOp("!", dummy_loc,cg->GenThunkCall(EqualString,lhs,rhs,symtbl),symtbl);
	}
	return cg->GenBinaryOp(op->GetOperName(),lhs,rhs,symtbl);
}


Location* LogicalExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location *lhs,*rhs;
	Assert(right);
	rhs=right->GenTac(cg,symtbl);

	if(rhs->IsPointer())
		rhs=cg->GenLoad(rhs,0,symtbl);
	
	if(NULL==left)
		return cg->GenBinaryOp(op->GetOperName(),dummy_loc,rhs,symtbl);
	else
	{
		lhs=left->GenTac(cg,symtbl);
		if(lhs->IsPointer())
			lhs=cg->GenLoad(lhs,0,symtbl);
	}
	return cg->GenBinaryOp(op->GetOperName(),lhs,rhs,symtbl);
}

Location* AssignExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location *lvalue,*rvalue;
	Assert(typeid(FieldAccess)==typeid(*left) ||
		   typeid(ArrayAccess)==typeid(*left));
//	lvalue=left->GenTac(cg,symtbl);	
	rvalue=right->GenTac(cg,symtbl);
	lvalue=left->GenTac(cg,symtbl);
	Location* derefRvalue=NULL;
	if(rvalue->IsPointer())
		derefRvalue=cg->GenLoad(rvalue,0,symtbl);
	if(lvalue->IsPointer())
	{
		if(NULL==derefRvalue)
			cg->GenStore(lvalue,rvalue,0);
		else
			cg->GenStore(lvalue,derefRvalue,0);
		return lvalue;
	}
	else
		if(NULL==derefRvalue)
			return cg->GenAssign(lvalue,rvalue);
		else
			return cg->GenAssign(lvalue,derefRvalue);
//	return cg->GenAssign(lvalue,rvalue);	
}
	
Location* ExprStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return expr ? expr->GenTac(cg,symtbl) : NULL;
}

bool NewExpr::Check(SymTable* symtbl)
{
	Type* ctype=GetType(symtbl);
	if(false==ctype->Check(symtbl))	// error should have been reported
		return false;
	return true;
}

Location* NewExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(cType);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Assert(typeid(ClassDecl)==typeid(*(symtbl->Find(cType->GetName(),true)->GetDecl())));
	int classsize=static_cast<ClassDecl*>(symtbl->Find(cType->GetName(),true)->GetDecl())->GetVarOffset();
	const char* vtaddr=static_cast<ClassDecl*>(symtbl->Find(cType->GetName(),true)->GetDecl())->GetVtableName();
	Assert(classsize>=4);
	Location* sizeofclass=cg->GenLoadConstant(classsize,symtbl);
	Location* vtableaddr=cg->GenLoadLabel(vtaddr,symtbl);
	return cg->GenThunkCall(NewClass,sizeofclass,vtableaddr ,symtbl);
}

bool NewArrayExpr::Check(SymTable* symtbl)
{
	Type* sizeType=size->GetType(symtbl);
	if(!sizeType->IsEquivalentTo(Type::intType))
	{
		ReportError::NewArraySizeNotInteger(size);
		return false;
	}
	return true;
}

Location* NewArrayExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(size && elemType);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location* arraysize=size->GenTac(cg,symtbl);
	
	if(arraysize->IsPointer())
		arraysize=cg->GenLoad(arraysize,0,symtbl);
	{
		// in current implementation, all array/basic type are 4 bytes
		Location *typesize=cg->GenLoadConstant(4,symtbl);
		return cg->GenThunkCall(NewArray,arraysize,typesize,symtbl);
	}
}		
	
Location* ReadLineExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenBuiltInCall(ReadLine,NULL,NULL,symtbl);
}


Location* ReadIntegerExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenBuiltInCall(ReadInt,NULL,NULL,symtbl);
}

Location* This::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	if(typeid(NamedType)==typeid(*GetType(symtbl)))
	{
		return symtbl->Find("this",true)->GetLocation();
	}
	else
	{
		Failure("not vaild this");
		return NULL;
	}
}


Location* Call::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(field && actuals);

	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));

	Location* baseaddr=NULL;
	
	int vfuncOffset;
	
	// check if it has return value
	bool hasretval=false;
	Type* basetype;
	
	if(base)
	{
		basetype=base->GetType(symtbl);
		if(typeid(NamedType)==typeid(*basetype))	// virtual call
		{
			const char* classname=static_cast<NamedType*>(basetype)->GetName();
			Symbol* symclass=GetGlobalSymTable()->Find(classname);
			Assert(symclass);
			SymTable* symtblclass=symclass->GetDecl()->GetSymTable();
			Assert(symtblclass->Find(field->GetName(),true));
			
			vfuncOffset=symtblclass->Find(field->GetName(),true)->GetLocation()->GetOffset();
			hasretval= ! static_cast<FnDecl*>(symtblclass->Find(field->GetName(),true)->GetDecl())->
									GetType()->IsEquivalentTo(Type::voidType);
			baseaddr=base->GenTac(cg,symtbl);
		}	
		else if(typeid(ArrayType)==typeid(*basetype))	// array.length
		{
			baseaddr=base->GenTac(cg,symtbl);
			if(baseaddr->IsPointer())
				baseaddr=cg->GenLoad(baseaddr,0,symtbl);
			return cg->GenThunkCall(ArrayLength,baseaddr,NULL,symtbl);
		}
		else
			Assert(0);
	}
	else
	{
		Symbol* symfunc=symtbl->Find(field->GetName(),true);
		if(symfunc && symfunc->IsMethod())	// this.call
		{
			vfuncOffset=symfunc->GetLocation()->GetOffset();
			hasretval= ! static_cast<FnDecl*>(symfunc->GetDecl())->
									GetType()->IsEquivalentTo(Type::voidType);
			baseaddr=symtbl->Find("this",true)->GetLocation();
		}
		else if(symfunc && symfunc->IsFunction())	// global function call
		{
			hasretval= ! static_cast<FnDecl*>(symfunc->GetDecl())->
									GetType()->IsEquivalentTo(Type::voidType);
		}
		else if(symfunc)
		{
			Failure("illegal call");
			return NULL;
		}
	}
	Location* result=NULL;	
	int i;
	Location* paramArray[20];
	Location* paramtmp;
	for(i=0;i<actuals->NumElements();i++)
	{
		paramtmp=actuals->Nth(i)->GenTac(cg,symtbl);
		if(paramtmp->IsPointer())
			paramArray[i]=cg->GenLoad(paramtmp,0,symtbl);
		else
			paramArray[i]=paramtmp;
	}

	if(baseaddr)	// virtual call
	{
		Location* vtbaddr;
		Location* indaddr;
		if(baseaddr->IsPointer())
		{
			indaddr=cg->GenLoad(baseaddr,0,symtbl);
			vtbaddr=cg->GenLoad(indaddr,0,symtbl);
		}
		else			
			vtbaddr=cg->GenLoad(baseaddr,0,symtbl);
//		Location *vfuncaddr=cg->GenLoad(vtbaddr,vfuncOffset,symtbl);
		Location *vfuncOffsetLoc=cg->GenLoadConstant(vfuncOffset,symtbl);
		Location *vfuncaddr=cg->GenLoad(cg->GenBinaryOp("+",vtbaddr,vfuncOffsetLoc,symtbl),0,symtbl);
		if(baseaddr->IsPointer())
			cg->GenPushParam(indaddr);
		else
			cg->GenPushParam(baseaddr);
		i=actuals->NumElements()-1;
		for(;i>=0;i--)
			cg->GenPushParam(paramArray[i]);
		
		result=cg->GenACall(vfuncaddr,hasretval,symtbl);
	}
	else
	{
		// check if it's builtin call, currently only support print/readline/readint builtins
		if(NULL==GetGlobalSymTable()->Find(field->GetName(),true))
		{
			Assert(0);
			return new Location(fpRelative,1000,"BBB");
		}
		
		Assert(GetGlobalSymTable()->Find(field->GetName(),true));
		i=actuals->NumElements()-1;
		for(;i>=0;i--)
			cg->GenPushParam(paramArray[i]);

		// global function call
		result=cg->GenLCall(static_cast<FnDecl*>(GetGlobalSymTable()->Find(field->GetName(),true)->GetDecl())->GetMangledName(),hasretval,symtbl);
		
	}
	if(baseaddr)	
		cg->GenPopParams(4 * (actuals->NumElements()+1));
	else
		cg->GenPopParams(4 * actuals->NumElements());
	return result;

}
		
		
Location* FieldAccess::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(field);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));


	int fieldoffset;
	Location* baseaddr=NULL;
	if(base)
	{
		Type* basetype;
		basetype=base->GetType(symtbl);
		if(typeid(NamedType)==typeid(*basetype))
		{
			const char* classname=static_cast<NamedType*>(basetype)->GetName();
			Symbol* symclass=GetGlobalSymTable()->Find(classname);
			Assert(symclass);
			SymTable* symtblclass=symclass->GetDecl()->GetSymTable();
			Assert(symtblclass->Find(field->GetName(),true));
			
			fieldoffset=symtblclass->Find(field->GetName(),true)->GetLocation()->GetOffset();
			baseaddr=base->GenTac(cg,symtbl);
		}
		else
		{
			Failure("field not found!");
			return NULL;
		}
	}
	else
	{
		Symbol* sym;
		if(NULL!=(sym=symtbl->Find(field->GetName(),true)))
		{
			if(sym->IsClassVar())
			{
				fieldoffset=sym->GetLocation()->GetOffset();
				baseaddr=symtbl->Find("this",true)->GetLocation();
			}
			else if(sym->IsLocalVar())
			{
				return sym->GetLocation();
			}
			else if(sym->IsGlobalVar())
			{
			}
		}
		else
		{
			Failure("sym not found");
			return NULL;
		}
	}
	if(baseaddr)
	{
		if(baseaddr->IsPointer())
			baseaddr=cg->GenLoad(baseaddr,0,symtbl);

		Location* fieldoff=cg->GenLoadConstant(fieldoffset,symtbl);
		Location* tmp=cg->GenBinaryOp("+",baseaddr,fieldoff,symtbl);
		tmp->SetPointer();
		return tmp;
//		return cg->GenLoad(baseaddr,fieldoffset,symtbl);
	}
	else
	{
		return symtbl->Find(field->GetName(),true)->GetLocation();
//		return cg->GenLoad(symtbl->Find(field->GetName(),true)->GetLocation(),0,symtbl);
	}
}	

Location* ArrayAccess::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(base && subscript);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));

	Location* subscriptaddr=subscript->GenTac(cg,symtbl);

	if(subscriptaddr->IsPointer())
		subscriptaddr=cg->GenLoad(subscriptaddr,0,symtbl);
	
	Location* baseaddr=base->GenTac(cg,symtbl);
	
	if(baseaddr->IsPointer())
		baseaddr=cg->GenLoad(baseaddr,0,symtbl);

	/* check index bound */
	cg->GenThunkCall(CheckIndex,baseaddr,subscriptaddr,symtbl);
	
	Location* elemsize=cg->GenLoadConstant(4,symtbl);
	Location* stepsize=cg->GenBinaryOp("*",subscriptaddr,elemsize,symtbl);
	Location* elemaddr=cg->GenBinaryOp("+",baseaddr,stepsize,symtbl);
	
	elemaddr->SetPointer();
	return elemaddr;	
//	return cg->GenLoad(elemaddr,0,symtbl);
}


Location* ConditionalExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(expr1 && expr2 && expr3);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));

	Location* testresult=expr1->GenTac(cg,symtbl);

	if(testresult->IsPointer())
		testresult=cg->GenLoad(testresult,0,symtbl);
	
	Location* result=cg->GenTempVar(symtbl);

	char* thenlabel=NULL;
	char* ifend=cg->NewLabel();
	if(expr3)
	{
		thenlabel=cg->NewLabel();
		cg->GenIfZ(testresult,thenlabel);
	}
	else
		cg->GenIfZ(testresult,ifend);

	Location* tmpresult=expr2->GenTac(cg,symtbl);
	if(tmpresult->IsPointer())
		tmpresult=cg->GenLoad(tmpresult,0,symtbl);
	
	cg->GenAssign(result,tmpresult);	

	if(expr3)
	{
		cg->GenGoto(ifend);		
		cg->GenLabel(thenlabel);
		
		Location* tmpexpr3=expr3->GenTac(cg,symtbl);
		if(tmpexpr3->IsPointer())
			tmpexpr3=cg->GenLoad(tmpexpr3,0,symtbl);
		
		cg->GenAssign(result, tmpexpr3);
	}
	cg->GenLabel(ifend);
	return result;
}
	
/////////////////////////////////////////////

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}
void IntConstant::PrintChildren(int indentLevel) { 
    printf("%d", value);
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}
void DoubleConstant::PrintChildren(int indentLevel) { 
    printf("%g", value);
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}
void BoolConstant::PrintChildren(int indentLevel) { 
    printf("%s", value ? "true" : "false");
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
}
void StringConstant::PrintChildren(int indentLevel) { 
    printf("%s",value);
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

void Operator::PrintChildren(int indentLevel) {
    printf("%s",tokenString);
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r) 
  : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r) 
  : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL; 
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}

void CompoundExpr::PrintChildren(int indentLevel) {
   if (left) left->Print(indentLevel+1);
   op->Print(indentLevel+1);
   right->Print(indentLevel+1);
}
   
  
ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}

void ArrayAccess::PrintChildren(int indentLevel) {
    base->Print(indentLevel+1);
    subscript->Print(indentLevel+1, "(subscript) ");
  }
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
}


  void FieldAccess::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
  }

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}

 void Call::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
    actuals->PrintAll(indentLevel+1, "(actuals) ");
  }
 

NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) { 
  Assert(c != NULL);
  (cType=c)->SetParent(this);
}

void NewExpr::PrintChildren(int indentLevel) {	
    cType->Print(indentLevel+1);
}

NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this); 
    (elemType=et)->SetParent(this);
}

void NewArrayExpr::PrintChildren(int indentLevel) {
    size->Print(indentLevel+1);
    elemType->Print(indentLevel+1);
}

ConditionalExpr::ConditionalExpr(Expr *e1, Operator *o1, Expr *e2, Operator *o2, Expr *e3) 
    : Expr(Join(e1->GetLocation(),e2->GetLocation()))
{
    Assert(e1 != NULL && e2 != NULL && e3 != NULL && o1 != NULL && o2 != NULL);
    (expr1=e1)->SetParent(this);
    (expr2=e2)->SetParent(this);
    (expr3=e3)->SetParent(this);
    (op1=o1)->SetParent(this);
    (op2=o2)->SetParent(this);
}       

void ConditionalExpr::PrintChildren(int indentLevel)
{
    expr1->Print(indentLevel+1);
    op1->Print(indentLevel+1);
    expr2->Print(indentLevel+1);
    op2->Print(indentLevel+1);
    expr3->Print(indentLevel+1);
}
