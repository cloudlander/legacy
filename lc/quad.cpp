#include "quad.h"
#include "errors.h"

List<Quad*> insts;
List<int> forwardTargets;
FILE* quadout;

/* for operand which points to a Quad */
Operand::Operand(ValueType vt, int t)
{
	lvalue=false;
	sym=NULL;
	type=vt;
	char buf[20];
	if(t<0) return;
	sprintf(buf,"T%d",t);
	name=new char[strlen(buf)+1];
	strncpy(name,buf,strlen(buf));
}

void Operand::SetValue(int v)
{
	if(indexes)
	{
		Assert(sym);
		int idx[10];
		for(int i=0;i<indexes->NumElements();i++)
		{
			Value val;
			if(Integer!=indexes->Nth(i)->GetValue(&val))
			{
				ReportError::Formatted(NULL,"%s: invalid index",sym->GetName());
			}
			idx[i]=val.Integer;
		}
		return sym->SetValue(v,idx);
	}
	
	if(sym)
		return sym->SetValue(v);

	value.Integer=v;
}

void Operand::SetValue(double v)
{
	if(indexes)
	{
		Assert(sym);
		int idx[10];
		for(int i=0;i<indexes->NumElements();i++)
		{
			Value val;
			if(Integer!=indexes->Nth(i)->GetValue(&val))
			{
				ReportError::Formatted(NULL,"%s: invalid index",sym->GetName());
			}
			idx[i]=val.Integer;
		}
		return sym->SetValue(v,idx);
	}
	
	if(sym)
		return sym->SetValue(v);

	value.Double=v;
}

void Operand::SetValue(bool v)
{
	if(indexes)
	{
		Assert(sym);
		int idx[10];
		for(int i=0;i<indexes->NumElements();i++)
		{
			Value val;
			if(Integer!=indexes->Nth(i)->GetValue(&val))
			{
				ReportError::Formatted(NULL,"%s: invalid index",sym->GetName());
			}
			idx[i]=val.Integer;
		}
		return sym->SetValue(v,idx);
	}
	
	if(sym)
		return sym->SetValue(v);

	value.Boolean=v;
}

ValueType Operand::GetValue(Value* v)
{
	if(indexes)
	{
		Assert(sym);
		int idx[10];
		for(int i=0;i<indexes->NumElements();i++)
		{
			Value val;
			if(Integer!=indexes->Nth(i)->GetValue(&val))
			{
				ReportError::Formatted(NULL,"%s: invalid index",sym->GetName());
			}
			idx[i]=val.Integer;
		}
		return sym->GetValue(v,idx);
	}

	if(sym)
		return sym->GetValue(v);

	switch(type)
	{
		case Integer:
			v->Integer=value.Integer;
			return Integer;
		case Double:
			v->Double=value.Integer;
			return Double;
		case Boolean:
			v->Boolean=value.Integer;
			return Boolean;
		case String:
			v->String=value.String;
			return String;
		default:
			fprintf(stderr,"FFDT");
			exit(-1);
	}
	return type;
}

void Operand::Print()
{
	if(sym)
	{
		fprintf(quadout,"%s",sym->GetName());
		if(sym->ArrayDims())
			fprintf(quadout,"[]");
		return;
	}
	if(name)
	{
		fprintf(quadout,"%s",name);
		return;
	}
	switch(type)
	{
		case Integer:
			fprintf(quadout,"%d",value.Integer);
			break;
		case Double:
			fprintf(quadout,"%e",value.Double);
			break;
		case Boolean:
			fprintf(quadout,"%s",value.Boolean ? "true" : "false");
			break;
		case String:
			fprintf(quadout,"%s",value.String);
			break;
		default:
			fprintf(stderr,"FFDT");
			exit(-1);
	}
}

char * Quad::opName[Quad::NumOps] = {"Halt", "Read", "Write", ":=", "GoFalse", "Goto", "+", "-", "*", "/", "%", "==", "<", "&&", "||", "<=", "!=", "!"};

int Quad::Execute()
{
	switch(op)
	{
		case Read:
			{
						char buf[255];
						scanf("%s",buf);
				switch(src1->GetType())
				{
					case Integer:
						src1->SetValue(atoi(buf));
						break;
					case Double:
						src1->SetValue(atof(buf));
						break;
					case Boolean:
						if(0==strcasecmp(buf,"true"))
							src1->SetValue(true);
						else if(0==strcasecmp(buf,"false"))
							src1->SetValue(false);
						else
							fprintf(stderr,"Read Error\n");
						break;
					default:
						ReportError::RuntimeError("invalid read");
				}
				return 0;
			}
		case Write:
			{
				Value v;
				switch(src1->GetValue(&v))
				{
					case Integer:
						printf("%d",v.Integer);
						break;
					case Double:
						printf("%e",v.Double);
						break;
					case Boolean:
						printf("%s",v.Boolean ? "true" : "false");
						break;
					case String:
						{
						for(int i=0;i<strlen(v.String);i++)
						{
							if(v.String[i]!='\"' &&
							   v.String[i]!='\\')
								printf("%c",v.String[i]);
							else if(v.String[i]=='\\')
							{
								if(v.String[i+1]=='n')
								{
									printf("\n");
									i++;
								}
							}
						}
						break;
						}
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}
		case GoFalse:
			{
				Value v;
				if(Boolean==src1->GetValue(&v))
				{
					if(!v.Boolean)
						return target;
					else
						return -1;
				}
				else
				{
					fprintf(stderr,"FFDT");
					exit(-1);
				}
				return -1;
			}
		case Goto:
				return target;

		case Assign:
			{
				Value v;
				ValueType type=src1->GetValue(&v);	
				switch(type)
				{
					case Integer:
						dest->SetValue(v.Integer);
						break;
					case Double:
						dest->SetValue(v.Double);
						break;
					case Boolean:
						dest->SetValue(v.Boolean);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}

		case Add:
			{
				Value l,r;
				ValueType type=src1->GetValue(&l);
				src2->GetValue(&r);
				switch(type)
				{
					case Integer:
						dest->SetValue(l.Integer+r.Integer);
						break;
					case Double:
						dest->SetValue(l.Double+r.Double);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}
		case Sub:
			{
				Value l,r;
				ValueType type=src1->GetValue(&l);
				src2->GetValue(&r);
				switch(type)
				{
					case Integer:
						dest->SetValue(l.Integer-r.Integer);
						break;
					case Double:
						dest->SetValue(l.Double-r.Double);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}
			
		case Mul:
			{
				Value l,r;
				ValueType type=src1->GetValue(&l);
				src2->GetValue(&r);
				switch(type)
				{
					case Integer:
						dest->SetValue(l.Integer*r.Integer);
						break;
					case Double:
						dest->SetValue(l.Double*r.Double);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}

		case Div:
			{
				Value l,r;
				ValueType type=src1->GetValue(&l);
				src2->GetValue(&r);
				if(0==r.Integer || 0==r.Double)
					ReportError::RuntimeError("divide by zero");
				switch(type)
				{
					case Integer:
						dest->SetValue(l.Integer/r.Integer);
						break;
					case Double:
						dest->SetValue(l.Double/r.Double);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}

		case Eq:
			{
				Value l,r;
				ValueType type=src1->GetValue(&l);
				src2->GetValue(&r);
				switch(type)
				{
					case Integer:
						dest->SetValue(l.Integer==r.Integer);
						break;
					case Double:
						dest->SetValue(l.Double==r.Double);
						break;
					case Boolean:
						dest->SetValue(l.Boolean==r.Boolean);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}


		case NotEq:
			{
				Value l,r;
				ValueType type=src1->GetValue(&l);
				src2->GetValue(&r);
				switch(type)
				{
					case Integer:
						dest->SetValue(l.Integer!=r.Integer);
						break;
					case Double:
						dest->SetValue(l.Double!=r.Double);
						break;
					case Boolean:
						dest->SetValue(l.Boolean!=r.Boolean);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}
			
		case Less:
			{
				Value l,r;
				ValueType type=src1->GetValue(&l);
				src2->GetValue(&r);
				switch(type)
				{
					case Integer:
						dest->SetValue(l.Integer<r.Integer);
						break;
					case Double:
						dest->SetValue(l.Double<r.Double);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}

		case LessThan:
			{
				Value l,r;
				ValueType type=src1->GetValue(&l);
				src2->GetValue(&r);
				switch(type)
				{
					case Integer:
						dest->SetValue(l.Integer<=r.Integer);
						break;
					case Double:
						dest->SetValue(l.Double<=r.Double);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}

		case And:
			{
				Value l,r;
				ValueType type=src1->GetValue(&l);
				src2->GetValue(&r);
				switch(type)
				{
					case Boolean:
						dest->SetValue(l.Boolean && r.Boolean);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}

		case Or:
			{
				Value l,r;
				ValueType type=src1->GetValue(&l);
				src2->GetValue(&r);
				switch(type)
				{
					case Boolean:
						dest->SetValue(l.Boolean || r.Boolean);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}

		case Not:
			{
				Value l;
				ValueType type=src1->GetValue(&l);
				switch(type)
				{
					case Boolean:
						dest->SetValue(!l.Boolean);
						break;
					default:
						fprintf(stderr,"FFDT");
				}
				return 0;
			}
		default:
			fprintf(stderr,"%s: currently not supported",opName[op]);
			exit(-1);
	}
	return 0;
}

Quad::Quad(Operand* d,OpCode oc,Operand* s1,Operand* s2)
	{
		dest=d;	op=oc; src1=s1;	src2=s2;
		label=NULL;
		
		if(forwardTargets.NumElements())	// chech whether label current quad
		{
			int t=insts.NumElements();
			for(int i=0;i<forwardTargets.NumElements();i++)
				if(t==forwardTargets.Nth(i))
				{
					char buf[20];
					sprintf(buf,"L%d",t);
					label=new char[strlen(buf)+1];
					strncpy(label,buf,strlen(buf));
					forwardTargets.RemoveAt(i);
					break;
				}
		}
	}

void Quad::SetTarget(int t)
	{
		if(t<0)
		{
			fprintf(stderr,"FFDT");
			exit(-1);
		}
		target=t;
		char buf[20];
		sprintf(buf,"L%d",t);
		tlabel=new char[strlen(buf)+1];
		strncpy(tlabel,buf,strlen(buf));
		if(t>=insts.NumElements())	// save the forward targets
			forwardTargets.Append(t);
	}

void Quad::Print()
{
	if(label)
		fprintf(quadout,"%s:\t",label);
	else
		fprintf(quadout,"\t");

	if(dest)
	{
		dest->Print();
		fprintf(quadout,"\t");
	}
	
	fprintf(quadout,"%s\t",opName[op]);

	if(op==Goto)
	{
		fprintf(quadout,"%s\n",tlabel);
		return;
	}

	if(src1)
		src1->Print();
	fprintf(quadout,"\t");

	if(op==GoFalse)
	{
		Assert(tlabel);
		fprintf(quadout,"%s\n",tlabel);
		return;
	}
	if(src2)
	{
		src2->Print();
	}
	fprintf(quadout,"\n");
}

