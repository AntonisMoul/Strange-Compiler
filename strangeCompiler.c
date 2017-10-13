//Antonios Moulopoulos 2104
//Aristidis Chronarakis 2171

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//Defines ascii
#define equal_tk 		61
#define greater_tk 		62
#define less_tk 		60
#define semicolon_tk		59
#define comma_tk		44
#define parenthesisLeft_tk	40
#define parenthesisRight_tk	41
#define bracketLeft_tk		91
#define bracketRight_tk		93
#define curlyBracketLeft_tk	123
#define curlyBracketRight_tk	125
#define plus_tk 		43
#define minus_tk 		45
#define multiplication_tk	42
#define division_tk 		47

//Defines  
#define and_tk 			0
#define exit_tk 		1
#define if_tk 			2
#define program_tk		3
#define when_tk 		4
#define declare_tk 		5
#define procedure_tk 		6
#define in_tk			7
#define or_tk 			8
#define call_tk 		9
#define do_tk 			10
#define function_tk 		11
#define inout_tk 		12
#define return_tk 		13
#define else_tk 		14
#define print_tk 		15
#define not_tk 			16
#define while_tk 		17
#define enddeclare_tk 		18
#define incase_tk 		19
#define forcase_tk 		20
#define copy_tk 		21
#define id_tk			22
#define notEqual_tk		23
#define greaterOrEqual_tk	24
#define lessOrEqual_tk		25
#define assignment_tk		26
#define commentStart_tk		27		
#define commentFinish_tk	28
#define constat_tk		29
#define variable_tk		30
#define number_tk		31
#define EOF_tk			32

#define variable_tp		33
#define	function_tp		34		

FILE *programFile;
FILE *outFinal;
int lines;
int variableNumber;
int quadNumber;
int endOfSubProgram;
char *buffer;
char *programName;
char *name;
int token;

void program();
void programblock();
void declarations();
void varlist();
void subprograms();
void func();
void funcbody();
void formalpars();
void formalparlist();

void sequence();
void brackersseq();
void brackorstat();
void statement();
void assignmentstat();
void ifstat();
void elsepart();
void dowhilestat();
void exitstat();
void returnstat();
void printstat();
void whilestat();
void incasestat();
void forcasestat();
void callstat();
void actualpars();
void actualparlist();
void actualparitem();
void expression(char **aTerm);
void term();
void factor();
void idtail(char *****aTerm);
void relationaloper(char **aRelOper);
void addoper();
void muloper();
void optionalsign();

typedef struct namesOfSubprograms{
	char *name;
	struct namesOfSubprograms *nextName;
}Names;

Names *names;

insertName(char *name){
	Names *list;
	if(names == NULL){	
		names = (Names*)malloc(sizeof(Names));
		names->name=(char*)malloc(sizeof(char));
		strcpy(names->name,name);
		names->nextName = NULL;	
	}
	else{
		list = names;
		while(list->nextName!=NULL){
			list=list->nextName;
		}
		list->nextName = (Names*)malloc(sizeof(Names));
		list->nextName->name=(char*)malloc(sizeof(char));
		strcpy(list->nextName->name,name);
		list->nextName->nextName = NULL;		
	}
}

deleteLastName(){
	Names *b = names;
	if(b->nextName!=NULL){
		while(b->nextName->nextName!=NULL&&b->nextName!=NULL){
			b=b->nextName;
		}
		b->nextName=NULL;	
	}
	else{
		names=NULL;
	}
}

char *getNameOfSubProgram(){
	Names *list;
	list = names;
	char *name;
	name = (char*)malloc(sizeof(char));	
	int i =0;
	while(list->nextName!=NULL){	
		list=list->nextName;
	}
	strcpy(name,list->name);
	deleteLastName();
	return name;
}

typedef struct Quads {
	int label;
	char *op;
	char *x;
	char *y;
	char *z;
}Quad;

void insert(Quad *aQuad);

Quad *genQuad(char *op,char *x,char *y,char *z){
	Quad *quad;
	quad=(Quad*)malloc(sizeof(Quad));
	quad->op=(char*)malloc(sizeof(char));
	quad->x=(char*)malloc(sizeof(char));
	quad->y=(char*)malloc(sizeof(char));
	quad->z=(char*)malloc(sizeof(char));
	stpcpy(quad->op,op);
	stpcpy(quad->x,x);
	stpcpy(quad->y,y);
	stpcpy(quad->z,z);
	quad->label=nextquad();
   	insert(quad);
	return quad;
}

typedef  struct ListOfQuads{
	Quad *quad;
	struct ListOfQuads *nextQuad;
}List;

List *quadsList;

List *makeList(Quad *aQuad){
	List *aList;
	aList = (List*)malloc(sizeof(List));
	aList->quad=aQuad;
	aList->nextQuad=NULL;	
	return aList;
}

void insert(Quad *aQuad){
	if(quadsList == NULL){
		quadsList=(List*)malloc(sizeof(List));
		quadsList->quad=aQuad;
		quadsList->nextQuad=NULL;
	}
	else{
		List *aList=quadsList;
		while(aList->nextQuad!=NULL){
			aList=aList->nextQuad;
		}
		aList->nextQuad=(List*)malloc(sizeof(List));
		aList->nextQuad->quad=aQuad;
		aList->nextQuad->nextQuad=NULL;
	}
	quadNumber=quadNumber+10;
}

List *backPatch(List *list,int z){
	List *aList=list;
	char *tempZ;
	tempZ = (char*)malloc(sizeof(char));
	sprintf(tempZ,"%d",z);
	while(aList!=NULL){
		strcpy(aList->quad->z,tempZ);
		aList=aList->nextQuad;
	}	
	return list;
}

List *emptyList(){

	List *aList;
	aList = (List*)malloc(sizeof(List));
	return aList;

}

char *newTemp(){
	char *temp;
	char *buffer;
	temp=(char*)malloc(sizeof(char));
	buffer=(char*)malloc(sizeof(char));
	strcpy(temp,"T_");
	sprintf(buffer,"%s%d",temp,variableNumber);
	variableNumber++;
	return buffer;
}

int nextquad(){
	int nextLabel=quadNumber;
	return nextLabel;
}

List *merge(List *list1,List *list2){
	List *aList,*aList2;
	aList=list1;
	aList2=list2;
	if(aList2==NULL){//kenh h 2h lista
		return aList;	
	}
	if(aList==NULL){//kenh h  1h lista
		return aList2;
	}
	while(aList->nextQuad!=NULL){
		aList = aList->nextQuad;
	}
	aList->nextQuad = (List*)malloc(sizeof(List));
	aList->nextQuad=aList2;
	return list1;
}

void boolterm(List **aList1,List **aList2);
void boolfactor(List **aList1,List **aList2);
void condition(List **aList1,List **aList2);

typedef struct RecordArgument{
	char *parMode;
	//int offset;
	struct RecordArgument *nextArgument;
}Argument;

typedef struct  RecordVariable{
	int offset;
}Variable;

typedef struct RecordFunction{
	char *type;
	int startQuad;
	int frameLength;
	Argument *argument;
}Function;

typedef struct  RecordType{
	Variable *variable;
	Function *function;
}Type;

typedef struct RecordEntity{
	char *name;
	Type *type;
	struct RecordEntity *nextEntity;
}Entity;

void formalparitem(Argument **argList,Entity **entity);

typedef struct RecordScope{
	int nestingLevel;
	int frameLength;
	Entity *entity;
	struct RecordScope *nextScope; 
}Scope;

Scope *scope;

Entity *search(char *varName,int *aChoice,int *nestingLevel){

	Scope *help = scope;
	Scope *help2=scope;
	Argument *argument;
	Entity *findEntity;
	Entity *helpEntity;
	Entity *helpEntity2;
	int findVar = 0;
	int sum = 0;
	int flag = 0;
	int argmumentSum = 0;
	int nl = 0;
	int nl1 = 0;
	if(scope->nextScope!=NULL){
		while(help2!=NULL){
			sum++;
			help2 = help2->nextScope;

		}
		while(help->nextScope != NULL){
			help2 = help;
			help = help->nextScope;

		}
		nl = help->nestingLevel;//trexon nesting level
	
/**************************************************************************************************

blepw posous parametrous pernane sto neo teleueto scope gia na 3erw an kapoia metavlhth 
pernaei ws parametros h einai topikh

***************************************************************************************************/
		helpEntity2 = help2->entity;
		while(helpEntity2->nextEntity != NULL){
			helpEntity2 = helpEntity2->nextEntity;
		}
		if(helpEntity2->type->function!=NULL){
			argument = helpEntity2->type->function->argument;
		}else{
			argument = NULL;
		}
		while(argument!=NULL){
			argmumentSum++;
			argument = argument->nextArgument;

		}	

/************************************************************************************************************
************************************************************************************************************/
		while(flag == 0 && sum>=0 ){
			helpEntity = help->entity;
			while(helpEntity!=NULL && flag == 0){
				if(helpEntity->type->function==NULL){
					
			
					if(strcmp(helpEntity->name,varName)==0){
						
						flag = 1;
						findEntity = helpEntity;
						nl1 = help->nestingLevel;
						findVar++;
						if(nl1 == 0){//variable is in main programm
							
							*aChoice = 0;

						}else if(/**nestingLevel*/nl1 == nl ){//variable is in the same nestingLevel

							*aChoice = 1;
						}else{

							*aChoice = 2; //to nesting level einai mikrotero apo to trexon	 	

						}					
				
						if(findVar <=  argmumentSum && argmumentSum != 0){
							int i = 1;
							if(helpEntity2->type->function!=NULL){
								argument = helpEntity2->type->function->argument;
							}else{
								argument = NULL;
							}
							while(argument!=NULL){
								if(i == findVar){
								
									if(strcmp(argument->parMode,"in")==0){
											
										if( *aChoice == 2){//to nesting level einai mikrotero apo to trexon										
											*aChoice = 3; 

										}else if(*aChoice == 1 ){

											*aChoice = 1;
 
										}

									}else if(strcmp(argument->parMode,"inout")==0){
									//	printf("here inout %s\n",helpEntity->name);
										if(*aChoice == 2){//to nesting level einai mikrotero apo to trexon											
											*aChoice = 4;

										}else if(*aChoice == 1){

											*aChoice =2; 
										}

									}

								}
								i++;
								argument = argument->nextArgument;

							}

						}				
					
					}
					
					
				}
				findVar++;
				helpEntity = helpEntity->nextEntity;
			}
			sum--;
		
			int i1 = 1;
			argmumentSum = 0;
			help = scope;
			findVar =0;
			while(i1<=sum && flag==0){
					
				i1++;
				help2 = help;
				help = help->nextScope;
			
			}
		
		
/**************************************************************************************************

blepw posous parametrous pernane sto neo teleueto scope gia na 3erw an kapoia metavlhth 
pernaei ws parametros h einai topikh

***************************************************************************************************/

			helpEntity2 = help2->entity;

			while(helpEntity2->nextEntity != NULL){

				helpEntity2 = helpEntity2->nextEntity;

			}

			if(helpEntity2->type->function!=NULL){

				argument = helpEntity2->type->function->argument;
		
			}else{

				argument = NULL;

			}


			while(argument!=NULL){
		
				argmumentSum++;
				argument = argument->nextArgument;

			}

/************************************************************************************************************
************************************************************************************************************/


		}

	}else{

		*aChoice = 0;
		helpEntity = scope->entity;
			while(helpEntity!=NULL && findVar == 0){
				if(strcmp(helpEntity->name,varName)==0){
					flag = 1;
					findEntity = helpEntity;
					nl = help->nestingLevel;					

							
					
				}
				helpEntity = helpEntity->nextEntity;
			}


	}

	*nestingLevel = nl;
	if(flag != 0){//we find a variable/function with the selected name in scopes
		return findEntity;
	}
	else{//we don't find a variable/function with the selected name in scopes
		return NULL;
	}
}

Entity *searchFunction(char *funName,int *aChoice){

	Scope *help = scope;
	int nestingLevel;
	int curentNestingLevel;
	int flag = 0;
	Entity *entity;
	Entity *findEntity;

	if(help->nextScope!=NULL){
	
		while(help != NULL){
		
			entity = help->entity;
			while(entity != NULL && flag == 0){
			
				if(entity->type->function != NULL){
					
					if(strcmp(funName,entity->name) == 0){
					
						flag=1;
						nestingLevel = help->nestingLevel + 1;
						findEntity = entity;
				
					}
				}
				
				entity = entity->nextEntity;
			
			}
			curentNestingLevel = help->nestingLevel;
			help = help->nextScope;
		
		}
	
	
	}else{
	
		entity = scope->entity;
		while(entity != NULL && flag == 0){
			
			if(entity->type->function != NULL){
				if(strcmp(funName,entity->name) == 0){
					
					flag=1;
					nestingLevel = 1;
					findEntity = entity;
				}
			}
				
			entity = entity->nextEntity;
			
		}
	
	}
	
	
	
	if(flag == 1){
	
		if(nestingLevel == curentNestingLevel){
		
			*aChoice = 0;
		
		}else{

			*aChoice = 1;		
		
		}
		
		return findEntity;
	
	}else{
	
		return NULL;
		
	}

}


void calculateFrame(){

	Scope *s = scope;
	Scope *s1;
	Entity *e1,*e2,*help1=NULL,*help2=NULL;
	int i=0,j=0;
	if(scope->nextScope!=NULL){
		while(s->nextScope!=NULL){
			s1 = s;
			s = s->nextScope;
		}
		e1 = s1->entity;
		while(e1!=NULL){
			if(e1->type->function!=NULL){			
				e2 = e1;
			}
			e1=e1->nextEntity;
		}
		help1 = s->entity;
		while(help1!=NULL){
			if(help1->type->variable!=NULL){
				i=help1->type->variable->offset;			
			}		
		 	help1=help1->nextEntity;
		}
		e2->type->function->frameLength = i+4;
	}
	else{
		help1 = scope->entity;
		while(help1!=NULL){
			if(help1->type->variable!=NULL){
				i=help1->type->variable->offset;
			}		
		 	help1=help1->nextEntity;
		}
		scope->frameLength = i+4;
	}
}

Entity *insertEntity(Entity *anEntity,int type){

	Entity *helpEntity;
	if(type == variable_tp){
		if(anEntity == NULL){
			anEntity = (Entity*)malloc(sizeof(Entity));
			Type *type;
			type = (Type*) malloc(sizeof (Type));
			type->variable = (Variable*)malloc(sizeof(Variable));
			type->function = (Function*)malloc(sizeof(Function));
			type->function = NULL;
			type->variable->offset = 12 ;
			anEntity->name = (char*)malloc(sizeof(char));
			strcpy(anEntity->name,buffer);
			anEntity->type = type;
			anEntity->nextEntity = NULL;
		}
		else{
			helpEntity = anEntity;
			while(helpEntity->nextEntity != NULL){
				helpEntity = helpEntity->nextEntity;
			}	
			helpEntity->nextEntity = (Entity*)malloc(sizeof(Entity));
			Type *type;
			type = (Type*) malloc(sizeof (Type));
			type->variable = (Variable*)malloc(sizeof(Variable));
			type->function = (Function*)malloc(sizeof(Function));
			type->function = NULL;
			type->variable->offset = helpEntity->type->variable->offset + 4 ;
			helpEntity->nextEntity->name = (char*)malloc(sizeof(char));
			strcpy(helpEntity->nextEntity->name,buffer);
			helpEntity->nextEntity->type = type;
			helpEntity->nextEntity->nextEntity = NULL;
		}
	}
	else{
		if(anEntity == NULL){
			anEntity = (Entity*)malloc(sizeof(Entity));
			Type *type;
			type = (Type*) malloc(sizeof (Type));
			type->variable = (Variable*)malloc(sizeof(Variable));
			type->function = (Function*)malloc(sizeof(Function));
			type->variable = NULL;
			type->function->startQuad = nextquad();
			anEntity->name = (char*)malloc(sizeof(char));
			strcpy(anEntity->name,buffer);
			anEntity->type = type;
			anEntity->nextEntity = NULL;
		}
		else{//if we have function
			helpEntity = anEntity;
			while(helpEntity->nextEntity != NULL){
				helpEntity = helpEntity->nextEntity;
			}	
			helpEntity->nextEntity = (Entity*)malloc(sizeof(Entity));
			Type *type;
			type = (Type*) malloc(sizeof (Type));
			type->variable = (Variable*)malloc(sizeof(Variable));
			type->function = (Function*)malloc(sizeof(Function));
			type->variable = NULL;
			type->function->startQuad = nextquad();
			helpEntity->nextEntity->name = (char*)malloc(sizeof(char));
			strcpy(helpEntity->nextEntity->name,buffer);
			helpEntity->nextEntity->type = type;
			helpEntity->nextEntity->nextEntity = NULL;
		}
	}
	return anEntity;
}

Entity *insertTempVar(Entity *anEntity,char *var){
	Entity *helpEntity;
	int offset=12;
	int of;
	if(anEntity == NULL){
		anEntity = (Entity*)malloc(sizeof(Entity));
		Type *type;
		type = (Type*) malloc(sizeof (Type));
		type->variable = (Variable*)malloc(sizeof(Variable));
		type->function = (Function*)malloc(sizeof(Function));
		type->function = NULL;
		type->variable->offset = 12 ;
		anEntity->name = (char*)malloc(sizeof(char));
		strcpy(anEntity->name,var);
		anEntity->type = type;
		anEntity->nextEntity = NULL;
	}
	else{
		helpEntity = anEntity;
		while(helpEntity != NULL){
			if(helpEntity->type->variable!=NULL){
					offset = offset + 4;
					of = helpEntity->type->variable->offset;
			}
			helpEntity = helpEntity->nextEntity;
		}	
		helpEntity = (Entity*)malloc(sizeof(Entity));
		Type *type;
		type = (Type*) malloc(sizeof (Type));	
		type->variable = (Variable*)malloc(sizeof(Variable));
		type->function = (Function*)malloc(sizeof(Function));
		type->function = NULL;
		type->variable->offset = of + 4 ;
		helpEntity->name = (char*)malloc(sizeof(char));
		strcpy(helpEntity->name,var);
		helpEntity->type = type;
		helpEntity->nextEntity = NULL;
		Entity *help = anEntity;
		while(help->nextEntity!=NULL){
			help = help->nextEntity;
		}
		help->nextEntity = helpEntity;
	}
	return anEntity;
}

Entity *insertArgumentListToEntity(Entity *entity,Argument *argList){

	Entity *ent = entity;
	while(ent->nextEntity!=NULL){
		ent=ent->nextEntity;
	}
	ent->type->function->argument = argList;
	return entity;
}

Entity *getEntity(){

	if(scope==NULL){
		return NULL;
	}
	else{
		Scope *helpScope=scope;
		while(helpScope->nextScope!=NULL){
			helpScope = helpScope->nextScope;
		}
		return helpScope->entity;
	}
}

Scope *createScope(){

	if(scope == NULL){
		return NULL;
	}
	else{
		Scope *helpScope = scope;
		while(helpScope->nextScope!=NULL){
			helpScope=helpScope->nextScope;	
		}
		helpScope->nextScope = (Scope*)malloc(sizeof(Scope));
		helpScope->nextScope->nextScope = NULL;
		return scope;
	}
}

void deleteScope(){
	if(scope!=NULL){
		if(scope->nextScope==NULL){
			scope = NULL;	
		}
		else{
			Scope *helpScope = scope;
			while(helpScope->nextScope->nextScope!=NULL){
				helpScope = helpScope->nextScope;	
			}
			helpScope->nextScope=NULL;
		}
	}
}

void insertScope(Entity *anEntity){

	if(scope == NULL){
		scope = (Scope*)malloc(sizeof(Scope));
		scope->nestingLevel=endOfSubProgram;
		scope->entity = anEntity;
		scope->nextScope = NULL;
	}
	else{
		Scope *helpScope = scope;
		while(helpScope->nextScope!=NULL){
			helpScope=helpScope->nextScope;	
		}
		helpScope->nestingLevel=endOfSubProgram;
		helpScope->entity = anEntity;
	}
}

Argument *insertArgument(Argument *argList){

	if(argList==NULL){
		argList = (Argument*) malloc(sizeof(Argument));
		argList->parMode = (char*)malloc(sizeof(char));
		if(token == in_tk){
			strcpy(argList->parMode,"in");
		}
		else if(token == inout_tk){
			strcpy(argList->parMode,"inout");
		}
		else{
			strcpy(argList->parMode,"copy");
		}
		argList->nextArgument = NULL;
	}
	else{
		Argument *argHelp;
		argHelp = argList;
		while(argHelp->nextArgument!=NULL){
			argHelp = argHelp->nextArgument;
		}
		argHelp->nextArgument = (Argument*) malloc(sizeof(Argument));
		argHelp->nextArgument->parMode = (char*)malloc(sizeof(char));
		if(token == in_tk){
			strcpy(argHelp->nextArgument->parMode,"in");
		}
		else if(token == inout_tk){
			strcpy(argHelp->nextArgument->parMode,"inout");
		}
		else{
			strcpy(argHelp->nextArgument->parMode,"copy");
		}
		argHelp->nextArgument->nextArgument = NULL;
	}
	return argList;
}



int getStartQuad(){

	Entity *entity;
	Scope *help=scope;
	
	
	while(help->nextScope->nextScope!= NULL){

		help = help->nextScope;

	}

	entity = help->entity;

	while(entity->nextEntity!= NULL){

		entity = entity->nextEntity;

	}

	
	return entity->type->function->startQuad;
		

}

void deleteQuads(int aLabel){

	List *list1,*list2,*list3;
	int flag1 = 0;
	int flag2 = 0;
	int i = 0;

	list1 = quadsList;
	
	while(list1 != NULL && flag1 == 0){
	
		if(list1->quad->label==aLabel){
		
			flag1 = 1;
			
			list3 = list1;
		

		}else{
			list2 = list1;//krataw ta previous
		
			list1=list1->nextQuad;
		}
		i++;
	}

	

	while(list3 != NULL && flag2 == 0){

		List *tempList = list3->nextQuad;

		if(strcmp(list3->quad->op,"end_block") == 0){

			flag2 = 1;	
				

		}

		list3 = NULL;
		list3 = tempList;
		
		


	}

	

	if(i==1 ||i==0){//elegxos gia to an to begin_block einai to prwto label ths listas
		
		quadsList = list3;
	}else{
	
		list2->nextQuad = list3;

	}

	


}



void gnlvcode(int nestingLevel,int offset){

	int i = 0; 
	
	fprintf(outFinal,"movi R[255], M[4+R[0]]\n");
	while(i<=nestingLevel){
		fprintf(outFinal,"movi R[255], M[4+R[255]]\n");
		i++;
	}

	
	fprintf(outFinal,"movi R[254], %d\n",offset);
	fprintf(outFinal,"addi R[255], R[254], R[255]\n");
}


void loadvr(char *v,char *r){

	int choice=0;
	int currentnl=0;
	Entity *entity;
	int op=0;

	
	if(*v >= 48 && *v <=57){//check if v is a number
		
		fprintf(outFinal,"movi R[%s], %s\n",r,v);

	}else{
	
		entity = search(v,&choice,&currentnl);
		
		if(entity==NULL){
			printf("variable %s is undeclared\n",v);
			exit(0);		
		}
		
		
		switch(choice){
		
		case 0: op = 600 +  entity->type->variable->offset;
			fprintf(outFinal,"movi R[%s],M[%d]\n",r,op);
			break;
		case 1: fprintf(outFinal,"movi R[%s],M[%d+R[0]]\n",r,entity->type->variable->offset);
			break;
		case 2: fprintf(outFinal,"movi R[255],M[%d+R[0]]\n",entity->type->variable->offset);
			fprintf(outFinal,"movi R[%s],M[R[255]]\n",r);
			break;
		case 3:	gnlvcode(currentnl,entity->type->variable->offset);
			fprintf(outFinal,"movi R[%s],M[R[255]]\n",r);
			break;
		case 4: gnlvcode(currentnl,entity->type->variable->offset);
			fprintf(outFinal,"movi R[255],M[R[255]]\n");
			fprintf(outFinal,"movi R[%s],M[R[255]]\n",r);
			break;
		

		}
	}
	




}
 void storerv(char *r,char *v){
	int choice=0;
	int currentnl=0;
	int op = 0;
	Entity *entity;
	entity = search(v,&choice,&currentnl);
	if(entity==NULL){
		printf("variable %s is undeclared\n",v);
		exit(0);		
	}
	switch(choice){
		case 0: op = 600 +  entity->type->variable->offset;
			fprintf(outFinal,"movi M[%d],R[%s]\n",op,r);
			break;
		case 1: fprintf(outFinal,"movi M[%d+R[0]],R[%s],\n",entity->type->variable->offset,r);
			break;
		case 2: fprintf(outFinal,"movi R[255],M[%d+R[0]]\n",entity->type->variable->offset);
			fprintf(outFinal,"movi M[R[255]],R[%s],\n",r);
			break;
		case 3:	gnlvcode(currentnl,entity->type->variable->offset);
			fprintf(outFinal,"movi M[R[255]],R[%s]\n",r);
			break;
		case 4: gnlvcode(currentnl,entity->type->variable->offset);
			fprintf(outFinal,"movi R[255],M[R[255]]\n");
			fprintf(outFinal,"movi M[R[255]],R[%s]\n",r);
			break;
	}

 }

void checkQuad(int start){

	int param=-1;
	int op = 0;
	List *aList;
	aList = (List*)malloc(sizeof(List));
	aList=quadsList;
	
	int choice=0;
	int currentnl=0;
	Entity *entity;
	Entity *entityFun;
	
	
	
	while(aList->quad->label!=start){
	
		aList=aList->nextQuad;
	
	}

	fprintf(outFinal,"L%d:\n",aList->quad->label);
	while(strcmp(aList->quad->op,"end_block")!=0){

		

		
		if(strcmp(aList->quad->op,"jump")==0){
			
			
			fprintf(outFinal,"jmp  L%s\n",aList->quad->z);
			
		}
		else if(strcmp(aList->quad->op,"=")==0||strcmp(aList->quad->op,"<")==0||strcmp(aList->quad->op,"<=")==0||strcmp(aList->quad->op,">")==0||strcmp(aList->quad->op,">=")==0||strcmp(aList->quad->op,"<>")==0){

			fprintf(outFinal,"L%d: ",aList->quad->label);		

			loadvr(aList->quad->x,"1");
			loadvr(aList->quad->y,"2");
			fprintf(outFinal,"cmpi R[1],R[2]\n");
			if(strcmp(aList->quad->op,"=")==0){
				fprintf(outFinal,"je L%s\n",aList->quad->z);
			}
			else if(strcmp(aList->quad->op,"<>")==0){
				fprintf(outFinal,"jne L%s\n",aList->quad->z);
			}
			else if(strcmp(aList->quad->op,">")==0){
				fprintf(outFinal,"jb L%s\n",aList->quad->z);
			}
			else if(strcmp(aList->quad->op,">=")==0){
				fprintf(outFinal,"jbe L%s\n",aList->quad->z);
			}
			else if(strcmp(aList->quad->op,"<")==0){
				fprintf(outFinal,"ja L%s\n",aList->quad->z);
			}
			else if(strcmp(aList->quad->op,"<=")==0){
				fprintf(outFinal,"jae L%s\n",aList->quad->z);
			}
		
		}
		else if(strcmp(aList->quad->op,":=")==0){
			fprintf(outFinal,"L%d: ",aList->quad->label);
			loadvr(aList->quad->x,"1");
			storerv("1",aList->quad->z);
		}
		else if(strcmp(aList->quad->op,"+")==0||strcmp(aList->quad->op,"-")==0||strcmp(aList->quad->op,"*")==0||strcmp(aList->quad->op,"/")==0){

			fprintf(outFinal,"L%d: ",aList->quad->label);	
			loadvr(aList->quad->x,"1");
			loadvr(aList->quad->y,"2");
			if(strcmp(aList->quad->op,"+")==0){
				fprintf(outFinal,"addi R[3],R[1],R[2]\n");
			}
			else if(strcmp(aList->quad->op,"-")==0){
				fprintf(outFinal,"subi R[3],R[1],R[2]\n");
			}
			else if(strcmp(aList->quad->op,"*")==0){
				fprintf(outFinal,"muli R[3],R[1],R[2]\n");
			}	
			else if(strcmp(aList->quad->op,"/")==0){
				fprintf(outFinal,"divi R[3],R[1],R[2]\n");
			}
			storerv("3",aList->quad->z);
		}
		else if(strcmp(aList->quad->op,"out")==0){
			fprintf(outFinal,"L%d: ",aList->quad->label);
			loadvr(aList->quad->x,"1");
			fprintf(outFinal,"outi R[1]\n");
	
		}
		else if(strcmp(aList->quad->op,"retv")==0){
			loadvr(aList->quad->x,"1");
			fprintf(outFinal,"movi R[255],M[8+R[0]]\n");
			fprintf(outFinal,"movi M[R[255]],R[1]\n");
		}
		else if(strcmp(aList->quad->op,"par")==0){

			fprintf(outFinal,"L%d:\n",aList->quad->label);
	
			
			param++;
				
			List *list = aList;
			while(strcmp(list->quad->op,"call") != 0){

				list = list->nextQuad;

			}

			entityFun = searchFunction(list->quad->x,&choice); 
			entity = search(aList->quad->x,&choice,&currentnl);

			if(entityFun == NULL){
				printf("function %s is undeclared\n",list->quad->x);
				exit(0);
			}

			if(entity == NULL){
				printf("variable %s is undeclared\n",aList->quad->x);
				exit(0);
			}

			int fl = entityFun->type->function->frameLength; 

		

			if(strcmp(aList->quad->y,"CV")==0){

				
				loadvr(aList->quad->x,"255");
				fprintf(outFinal,"movi M[%d+R[0]],R[255]\n",12+(4*param)+fl);
			}
			else if(strcmp(aList->quad->y,"REF")==0&&(choice==1||choice==0)){
				
				
				fprintf(outFinal,"movi R[255],R[0]\n");
				fprintf(outFinal,"movi R[254],%d\n",entity->type->variable->offset);
				fprintf(outFinal,"addi R[255],R[254],R[255]\n");
				fprintf(outFinal,"movi M[%d+R[0]],R[255]\n",12+(4*param)+fl);

			}
			else if(strcmp(aList->quad->y,"REF")==0&&choice==2){
			
				fprintf(outFinal,"movi R[255],R[0]\n");
				fprintf(outFinal,"movi R[254],%d\n",entity->type->variable->offset);
				fprintf(outFinal,"addi R[255],R[254],R[255]\n");
				fprintf(outFinal,"movi R[1],M[R[255]]\n");
				fprintf(outFinal,"movi M[%d+R[0]],R[1]\n",12+(4*param)+fl);
			}
			else if(strcmp(aList->quad->y,"REF")==0&&choice==3){
			
				gnlvcode(currentnl,entity->type->variable->offset);
				fprintf(outFinal,"addi R[255],R[254],R[255]\n");
				fprintf(outFinal,"movi M[%d+R[0]],R[255]\n",12+(4*param)+fl);
				
			}
			else if(strcmp(aList->quad->y,"REF")==0&&choice==4){
			
				gnlvcode(currentnl,entity->type->variable->offset);
				fprintf(outFinal,"addi R[255],R[254],R[255]\n");
				fprintf(outFinal,"movi R[1],M[R[255]]\n");
				fprintf(outFinal,"movi M[%d+R[0]],R[1]\n",12+(4*param)+fl);
			}
			else if(strcmp(aList->quad->y,"RET")==0){
				
				entity = search(aList->quad->x,&choice,&currentnl);
				if(entity == NULL){
					printf("variable %s is undeclared\n",aList->quad->x);
					exit(0);
				}
				
				
			
				fprintf(outFinal,"movi R[255],R[0]\n");
				fprintf(outFinal,"movi R[254],%d\n",/*offset proswrinhs metavlhths*/entity->type->variable->offset);
				fprintf(outFinal,"addi R[255],R[254],R[255]\n");
				
				
				Scope *help = scope;
		
				if(scope->nextScope!=NULL){
		
					while(help->nextScope->nextScope!=NULL){
			
						help = help->nextScope; 
			
					}
					entity = help->entity;
					while(entity->nextEntity!=NULL){
			
						entity = entity->nextEntity;
			
					}
				}else{
		
					entity = scope->entity;
			
					while(entity->nextEntity!=NULL){
			
						entity = entity->nextEntity;
			
					}
		
				}
				
				op = 8 + entity->type->function->frameLength;
				fprintf(outFinal,"movi[%d+R[0]],R[255]\n",op);
			}
		}else if(strcmp(aList->quad->op,"call")==0){

			fprintf(outFinal,"L%d: ",aList->quad->label);
			entity = searchFunction(aList->quad->x,&choice);
			
			if(entity == NULL){
			
				printf("function %s is undefined\n",aList->quad->x);
				exit(EXIT_FAILURE);
			}else{
			
				switch (choice){
				
					case 0: op = 4 + entity->type->function->frameLength;
						fprintf(outFinal,"movi R[255],M[4+R[0]]\n");
						fprintf(outFinal,"movi M[%d + R[0]], R[255]\n",op);
						break;

					case 1: op = 4 + entity->type->function->frameLength;
						fprintf(outFinal,"movi M[%d + R[0]], R[0]\n",op);
						break;				

				}
				
				fprintf(outFinal,"movi R[255], %d\n",entity->type->function->frameLength);
				fprintf(outFinal,"addi R[0], R[255], R[0]\n");
				fprintf(outFinal,"movi R[255], $\n");
				fprintf(outFinal,"movi R[254], 15\n");
				fprintf(outFinal,"addi R[255], R[255], R[254]\n");
				fprintf(outFinal,"movi M[R[0]],  R[255]\n");
				fprintf(outFinal,"jmp  L%d\n",entity->type->function->startQuad); 
				fprintf(outFinal,"movi R[255], %d\n",entity->type->function->frameLength);
				fprintf(outFinal,"subi R[0], R[0], R[255]\n");			

			}
		
		
		}
		
		aList=aList->nextQuad;
	}
	
	if(strcmp(aList->quad->op,"end_block")==0){
		fprintf(outFinal,"L%d: ",aList->quad->label);
		fprintf(outFinal,"jmp  M[R[0]]\n");

		

	}


}

void checkMainQuad(){

	int param=-1;
	List *aList;
	aList = (List*)malloc(sizeof(List));
	aList=quadsList;
	
	int choice=0;
	int currentnl=0;
	int op = 0;
	Entity *entity;
	Entity *entityFun;
	

	if(strcmp(aList->quad->op,"begin_block")!=0){
		fprintf(outFinal,"L%d: ",aList->quad->label);
	}
	
	while(strcmp(aList->quad->op,"end_block") != 0){
	
		
		if(strcmp(aList->quad->op,"halt")==0){
			fprintf(outFinal,"L%d: ",aList->quad->label);
			fprintf(outFinal,"halt\n");
		
		}else if(strcmp(aList->quad->op,"par")==0){
			
			fprintf(outFinal,"L%d:\n",aList->quad->label);

		

			param++;
			List *list = aList;
			while(strcmp(list->quad->op,"call") != 0){

				list = list->nextQuad;

			}

			entityFun = searchFunction(list->quad->x,&choice); 
			entity = search(aList->quad->x,&choice,&currentnl);
			
			if(entityFun == NULL){
				printf("function %s is undeclared\n",list->quad->x);
				exit(0);
			}

			if(entity == NULL){
				printf("variable %s is undeclared\n",aList->quad->x);
				exit(0);
			}

			int fl = entityFun->type->function->frameLength; 
			
			if(strcmp(aList->quad->y,"CV")==0){


				loadvr(aList->quad->x,"255");
				fprintf(outFinal,"movi M[%d+R[0]],R[255]\n",12+(4*param)+fl);
			}
			else if(strcmp(aList->quad->y,"REF")==0&&(choice==1||choice==0)){

				
				fprintf(outFinal,"movi R[255],R[0]\n");
				fprintf(outFinal,"movi R[254],%d\n",entity->type->variable->offset);
				fprintf(outFinal,"addi R[255],R[254],R[255]\n");
				fprintf(outFinal,"movi M[%d+R[0]],R[255]\n",12+(4*param)+fl);
			}
			else if(strcmp(aList->quad->y,"REF")==0&&choice==2){
			
				fprintf(outFinal,"movi R[255],R[0]\n");
				fprintf(outFinal,"movi R[254],%d\n",entity->type->variable->offset);
				fprintf(outFinal,"addi R[255],R[254],R[255]\n");
				fprintf(outFinal,"movi R[1],M[R[255]]\n");
				fprintf(outFinal,"movi M[%d+R[0]],R[1]\n",12+(4*param)+fl);
			}
			else if(strcmp(aList->quad->y,"REF")==0&&choice==3){
			
				gnlvcode(currentnl,entity->type->variable->offset);
				fprintf(outFinal,"addi R[255],R[254],R[255]\n");
				fprintf(outFinal,"movi M[%d+R[0]],R[255]\n",12+(4*param)+fl);
				
			}
			else if(strcmp(aList->quad->y,"REF")==0&&choice==4){
			
				gnlvcode(currentnl,entity->type->variable->offset);
				fprintf(outFinal,"addi R[255],R[254],R[255]\n");
				fprintf(outFinal,"movi R[1],M[R[255]]\n");
				fprintf(outFinal,"movi M[%d+R[0]],R[1]\n",12+(4*param)+fl);
			}
			else if(strcmp(aList->quad->y,"RET")==0){


				Scope *help = scope;
		
				if(scope->nextScope!=NULL){
		
					while(help->nextScope->nextScope!=NULL){
			
						help = help->nextScope; 
			
					}
					entity = help->entity;
				

						while(entity->nextEntity->nextEntity!=NULL){
			
							entity = entity->nextEntity;
			
						}

				
				}else{
		
					entity = scope->entity;

				

						while(entity->nextEntity->nextEntity!=NULL){
			
							entity = entity->nextEntity;
			
						}
				}

				int fl = 8 + entity->type->function->frameLength; 
				
				entity = search(aList->quad->x,&choice,&currentnl);
				if(entity == NULL){
					printf("variable %s is undeclared\n",aList->quad->x);
					exit(0);
				}
				
				
			
				fprintf(outFinal,"movi R[255],R[0]\n");
				fprintf(outFinal,"movi R[254],%d\n",/*offset proswrinhs metavlhths*/entity->type->variable->offset);
				fprintf(outFinal,"addi R[255],R[254],R[255]\n");
				
				
				help = scope;
		
				if(scope->nextScope!=NULL){
		
					while(help->nextScope->nextScope!=NULL){
			
						help = help->nextScope; 
			
					}
					entity = help->entity;
					while(entity->nextEntity->nextEntity!=NULL){
			
						entity = entity->nextEntity;
			
					}
				}else{
		
					entity = scope->entity;
			
					while(entity->nextEntity->nextEntity!=NULL){
			
						entity = entity->nextEntity;
			
					}
		
				}

				
				op = 8 + entity->type->function->frameLength;

				fprintf(outFinal,"movi[%d+R[0]],R[255]\n",op);
			}
		}

		else if(strcmp(aList->quad->op,"jump")==0){
			
			
			fprintf(outFinal,"jmp  L%s\n",aList->quad->z);
			
			
		}
		else if(strcmp(aList->quad->op,"=")==0||strcmp(aList->quad->op,"<")==0||strcmp(aList->quad->op,"<=")==0||strcmp(aList->quad->op,">")==0||strcmp(aList->quad->op,">=")==0||strcmp(aList->quad->op,"<>")==0){

			fprintf(outFinal,"L%d: ",aList->quad->label);

			loadvr(aList->quad->x,"1");
			loadvr(aList->quad->y,"2");
			fprintf(outFinal,"cmpi R[1],R[2]\n");
			if(strcmp(aList->quad->op,"=")==0){
				
				fprintf(outFinal,"je L%s\n",aList->quad->z);
			}
			else if(strcmp(aList->quad->op,"<>")==0){
				fprintf(outFinal,"jne L%s\n",aList->quad->z);
			}
			else if(strcmp(aList->quad->op,">")==0){
				fprintf(outFinal,"jb L%s\n",aList->quad->z);
			}
			else if(strcmp(aList->quad->op,">=")==0){
				fprintf(outFinal,"jbe L%s\n",aList->quad->z);
			}
			else if(strcmp(aList->quad->op,"<")==0){
				fprintf(outFinal,"ja L%s\n",aList->quad->z);
			}
			else if(strcmp(aList->quad->op,"<=")==0){
				fprintf(outFinal,"jae L%s\n",aList->quad->z);
			}
		
		}
		else if(strcmp(aList->quad->op,":=")==0){
			fprintf(outFinal,"L%d: ",aList->quad->label);
			loadvr(aList->quad->x,"1");
			storerv("1",aList->quad->z);
		}
		else if(strcmp(aList->quad->op,"+")==0||strcmp(aList->quad->op,"-")==0||strcmp(aList->quad->op,"*")==0||strcmp(aList->quad->op,"/")==0){

				
			loadvr(aList->quad->x,"1");
			loadvr(aList->quad->y,"2");
			if(strcmp(aList->quad->op,"+")==0){
				fprintf(outFinal,"L%d: ",aList->quad->label);
				fprintf(outFinal,"addi R[3],R[1],R[2]\n");
			}
			else if(strcmp(aList->quad->op,"-")==0){

				fprintf(outFinal,"L%d: ",aList->quad->label);
				fprintf(outFinal,"subi R[3],R[1],R[2]\n");
			}
			else if(strcmp(aList->quad->op,"*")==0){
				fprintf(outFinal,"L%d: ",aList->quad->label);
				fprintf(outFinal,"muli R[3],R[1],R[2]\n");
			}	
			else if(strcmp(aList->quad->op,"/")==0){
				fprintf(outFinal,"L%d: ",aList->quad->label);
				fprintf(outFinal,"divi R[3],R[1],R[2]\n");
			}
			storerv("3",aList->quad->z);
		}
		else if(strcmp(aList->quad->op,"out")==0){
			fprintf(outFinal,"L%d: ",aList->quad->label);
			loadvr(aList->quad->x,"1");
			fprintf(outFinal,"outi R[1]\n");
	
		}
		else if(strcmp(aList->quad->op,"retv")==0){
			loadvr(aList->quad->x,"1");
			fprintf(outFinal,"movi R[255],M[8+R[0]]\n");
			fprintf(outFinal,"movi M[R[255]],R[1]\n");
		}
		else if(strcmp(aList->quad->op,"call")==0){

			
			fprintf(outFinal,"L%d: ",aList->quad->label);
			entity = searchFunction(aList->quad->x,&choice);
			
			if(entity == NULL){
			
				printf("function %s is undefined\n",aList->quad->x);
				exit(EXIT_FAILURE);
			}else{
			
				op = 4 + entity->type->function->frameLength;
				fprintf(outFinal,"movi M[%d + R[0]], R[0]\n",op);
				fprintf(outFinal,"movi R[255], %d\n",entity->type->function->frameLength);
				fprintf(outFinal,"addi R[0], R[255], R[0]\n");
				fprintf(outFinal,"movi R[255], $\n");
				fprintf(outFinal,"movi R[254], 15\n");
				fprintf(outFinal,"addi R[255], R[255], R[254]\n");
				fprintf(outFinal,"movi M[R[0]],  R[255]\n");
				fprintf(outFinal,"jmp  L%d\n",entity->type->function->startQuad);
				fprintf(outFinal,"movi R[255], %d\n",entity->type->function->frameLength);
				fprintf(outFinal,"subi R[0], R[0], R[255]\n"); 
			}
		
		
		}
		
		aList = aList->nextQuad;
	}
	

}


int lex(){
	bool notNumber=false;
	int myNumber;
	int character,nextCharacter;
	char *p;
	p = (char *)malloc(sizeof(char));
	
	strcpy(buffer,"\0");
	character = fgetc(programFile);
	while((character == '\n') || (character == ' ') || (character == '\t')||(character==13)&&character!=EOF){
		if(character == '\n'){
			lines++;
		}
		character = fgetc(programFile);
	}
	 if(character == '<'){
		strcpy(buffer,"<");
		nextCharacter=getc(programFile);
		if(nextCharacter == '='){
			strcat(buffer,"=");
			return lessOrEqual_tk;
		}
		else if(nextCharacter == '>'){
			strcat(buffer,">");
			return notEqual_tk;
		}
		else{
			ungetc(nextCharacter,programFile);
			return less_tk;
		}		
	}
	else if(character == '>'){
		nextCharacter=getc(programFile);
		if(nextCharacter == '='){
			strcpy(buffer,">=");
			return greaterOrEqual_tk;
		}
		else{
			ungetc(nextCharacter,programFile);
			strcpy(buffer,">");
			return greater_tk;
		}			
	}
	else if(character == '='){
			strcpy(buffer,"=");
			return equal_tk;		
	}
	else if(character == '+'){
		strcpy(buffer,"+");
		return plus_tk;
	}
	else if(character == '-'){
		strcpy(buffer,"-");
		return minus_tk;
	}
	else if(character == '*'){
		nextCharacter=getc(programFile);
		if(nextCharacter == division_tk){
			return commentFinish_tk;
		}else{
			ungetc(nextCharacter,programFile);
			strcpy(buffer,"*");
			return multiplication_tk;
		}				
	}
	else if(character == division_tk){
		nextCharacter=getc(programFile);
		if(nextCharacter == '*'){
			while(1){
				character=getc(programFile);
				if(character=='*'){
					nextCharacter=getc(programFile);
					if(nextCharacter=='/'){
						break;
					}
					else{
						ungetc(nextCharacter,programFile);
					}
				}
				else if(character==EOF){
						printf("error message (lexical) :: comments do not close\n");
						break;	
				}else if(character==10&&character!=EOF){
					lines++;
				}
			}
			return commentStart_tk;
		}
		else{
			ungetc(nextCharacter,programFile);
			strcpy(buffer,"/");
			return division_tk;
		}			
	}
	else if(character == ';'){
		return semicolon_tk;
	}
	else if(character == ','){
		return comma_tk;
	}
	else if(character == ':'){
		nextCharacter=getc(programFile);
		if(nextCharacter == '='){
			return assignment_tk;
		}else{
			printf("error message (lexical) :: symbol ':' is not supported possible expexted '=' \n");
			return  -1;
		}
	}
	else if(character=='{'){
		return curlyBracketLeft_tk;
	}
	else if(character=='}'){
		return curlyBracketRight_tk;
	}
	else if(character=='('){
		return parenthesisLeft_tk;
	}
	else if(character==')'){
		return parenthesisRight_tk;
	}
	else if(character=='['){
		return bracketLeft_tk;
	}
	else if(character==']'){
		return bracketRight_tk;
	}
	else if(character==EOF){
		printf("End Of File at line %d \n",lines);
		return EOF_tk;
	}
	else if((character >=65 && character <=90) || (character >=97 && character <=122)){
                strcpy(p,(char*) &character);
		strcat(buffer,p);
		character=getc(programFile);
		while(((character >=65 && character <=90) || (character >=97 && character <=122) ||  (character >=48 &&character <=57))&&(character!=' ')&&character!=assignment_tk){
			if(character=='\n'){
				lines++;
			}else if(character!='\n'){
				strcpy(p,(char*) &character);
				strcat(buffer,p);
			}
			character=getc(programFile);
			if(character=='\n'){
				lines++;
			}
		}
		ungetc(character,programFile);
		if(strcmp(buffer,"and")==0){
			return and_tk;
		}
		else if(strcmp(buffer,"exit")==0){
			return exit_tk;
		}
		else if(strcmp(buffer,"if")==0){
			return if_tk;
		}
		else if(strcmp(buffer,"program")==0){
			return program_tk;
		}
		else if(strcmp(buffer,"when")==0){
			return when_tk;
		}
		else if(strcmp(buffer,"declare")==0){
			return declare_tk;
		}
		else if(strcmp(buffer,"procedure")==0){
			return procedure_tk;
		}
		else if(strcmp(buffer,"in")==0){
			return in_tk;
		}
		else if(strcmp(buffer,"or")==0){
			return or_tk;
		}
		else if(strcmp(buffer,"call")==0){
			return call_tk;
		}
		else if(strcmp(buffer,"do")==0){
			return do_tk;
		}
		else if(strcmp(buffer,"function")==0){
			return function_tk;
		}
		else if(strcmp(buffer,"inout")==0){
			return inout_tk;
		}
		else if(strcmp(buffer,"return")==0){
			return return_tk;
		}
		else if(strcmp(buffer,"else")==0){
			return else_tk;
		}
		else if(strcmp(buffer,"print")==0){
			return print_tk;
		}
		else if(strcmp(buffer,"not")==0){
			return not_tk;
		}
		else if(strcmp(buffer,"while")==0){
			return while_tk;
		}
		else if(strcmp(buffer,"enddeclare")==0){
			return enddeclare_tk;
		}
		else if(strcmp(buffer,"incase")==0){
			return incase_tk;
		}
		else if(strcmp(buffer,"forcase")==0){
			return forcase_tk;
		}
		else if(strcmp(buffer,"copy")==0){
			return copy_tk;
		}
		else{
		
			return variable_tk;	
		}	
	}
	else if((character >=48 && character <=57 && character!= ' ')){
		strcpy(p,(char*) &character);
		strcat(buffer,p);
		character=getc(programFile);
		while((character!=32 )&&character!=10&&character!=13 &&character!=':'&&character!=';'&&character!=','&&character!=division_tk&&(character!=']'&& character!='['&&character!='('&&character!=')'&&character!=':' &&character!='='&&character!=assignment_tk)){
		
			if(((character < 48) || character > 57) && (character!=10)&&(character!=']' || character!='['||character!='('||character!=')'||character!='='||character!=':')){
				notNumber=true;
			}
			else{	
				strcpy(p,(char*) &character);
				strcat(buffer,p);
			}
			if(notNumber){
				printf("error message (lexical) :: error in line %d\n",lines);
				return -1;
			}
			character=getc(programFile);
		}
		ungetc(character,programFile);
		myNumber=atoi(buffer);
		if(myNumber+32767>=0 && myNumber+32767<=65535 && !notNumber){
			return number_tk;
		}
		else if((myNumber+32767<0 || myNumber+32767>65535) && !notNumber){
			printf("error message (lexical) :: number at line %d is out of bounds [-32768,32767]\n",lines);
			return -1;
		}	
	}
	else if(character=='\\'){
		printf("error message (lexical) :: character %c is unknown at line  %d\n",character,lines);
		return -1;
	}
}

void actualpars(){
	actualparlist();
}

void actualparlist(){
	actualparitem();
	while(token==comma_tk){
		actualparitem();
	}
}

void actualparitem(){
	char *term;
	term = (char*)malloc(sizeof(char));
	token=lex();
	while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
		token=lex();	
	}
	if(token==in_tk){
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
		expression(&term);
		genQuad("par",term,"CV","       ");
		
	}
	else if(token==inout_tk){
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
		if(token!=variable_tk){
			printf("error message (syntax) :: expected id after inout at line %d\n",lines);

		}else{

			strcpy(term,buffer);
			genQuad("par",term,"REF","       ");
			token=lex();
			while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
				token=lex();
			}
		}
	}
	else if(token==copy_tk){
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
		if(token!=variable_tk){
			printf("error message (syntax) :: expected id after copy at line %d\n",lines);
		}
		else{
			strcpy(term,buffer);
			genQuad("par",term,"CPY","       ");
			token=lex();
			while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
				token=lex();
			}
		}
	}
}

void condition(List **aList1,List **aList2){
	List *Btrue,*Bfalse;
	Btrue = emptyList();
	Bfalse = emptyList();
	Btrue = NULL;
	Bfalse = NULL;
	List *t;
	boolterm(&Btrue,&Bfalse);
	t = *aList2;
	*aList1 = Btrue;
	*aList2 = Bfalse;
	while(token==or_tk){
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
		backPatch(*aList2,nextquad());
		Bfalse = NULL;
		boolterm(&Btrue,&Bfalse);	
		*aList1 = Btrue;		
		*aList2 = Bfalse;	
	}	
}

void boolterm(List **aList1,List **aList2){
	List *Bt,*Bf;
	Bt = emptyList();
	Bf = emptyList();
	List *t,*t2;
	Bt = NULL;
	Bf = NULL;
	boolfactor(&Bt, &Bf);
	t = *aList1;
	t2 = *aList2;	
	*aList1=merge(t,Bt);	
	*aList2=merge(t2,Bf);	
	while(token==and_tk){
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
		backPatch(*aList1,nextquad());
		boolfactor(&Bt, &Bf);
		*aList1=Bt;
		t2 = *aList2;
		*aList2=merge(t2,Bf);	
	}
}

void boolfactor(List **aList1,List **aList2){
	Quad *trueQuad,*falseQuad;
	List *Rtrue,*Rfalse;
	Rtrue = NULL;
	Rfalse = NULL;
	trueQuad = (Quad*)malloc(sizeof(Quad));
	falseQuad = (Quad*)malloc(sizeof(Quad));
	token=lex();	
	while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
		token=lex();
	}
	if(token==not_tk){
		token = lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
		if(token==bracketLeft_tk){
			condition(aList1,aList2);
		}
		else{
			printf("error message (syntax) :: expected '[' after not at line %d\n",lines);
		}
	
	}
	else if(token==bracketLeft_tk){
		condition(aList1,aList2);
	}
	else if(token==bracketRight_tk){
	
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
	}
	else{
		char *term1,*term2,*relOper;
		term1 = (char*)malloc(sizeof(char));
		term2 = (char*)malloc(sizeof(char));
		relOper = (char*)malloc(sizeof(char));
		expression(&term1);
		relationaloper(&relOper);
		expression(&term2);
		trueQuad = genQuad(relOper,term1,term2,"       ");
		falseQuad = genQuad("jump","       ","       ","       ");
		Rtrue = makeList(trueQuad);
		Rfalse = makeList(falseQuad);
		*aList1 = Rtrue;
		*aList2 = Rfalse;
		if(token==bracketRight_tk){
			token=lex();
			while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
				token=lex();
			}
		}
		else{
			printf("error message (syntax) :: expected ']' at line %d\n",lines);			
		}	
	}
}

void expression(char **aTerm){
	char *op,*term1,*term2;
	op = (char*)malloc(sizeof(char));
	term1 = (char*)malloc(sizeof(char));
	term2 = (char*)malloc(sizeof(char));
	optionalsign();
	term(&aTerm);
	if(strcmp(*aTerm,"")!=0)
		strcpy(term1,*aTerm);
	while(token==plus_tk||token==minus_tk){
		strcpy(op,buffer);
		addoper();
		term(&aTerm);
		strcpy(term2,*aTerm);	
		char *w = newTemp();
		Entity *en;
		en=getEntity();
		en = insertTempVar(en,w);
		genQuad(op,term1,term2,w);
		strcpy(term1,w);
	}
	if(strcmp(term1,"")!=0)
		strcpy(*aTerm,term1);	
}

void term(char ***aTerm){
	char *op,*term1,*term2;
	op = (char*)malloc(sizeof(char));
	term1 = (char*)malloc(sizeof(char));
	term2 = (char*)malloc(sizeof(char));	
	strcpy(term1,buffer);
	factor(&aTerm);
	if(strcmp(**aTerm,"")!=0)
		strcpy(term1,**aTerm);
	while(token==multiplication_tk||token==division_tk){	
		strcpy(op,buffer);
		muloper();
		term(&*aTerm);
		strcpy(term2,**aTerm);
		char *w = newTemp();

		Entity *en;
		en=getEntity();
		en = insertTempVar(en,w);
		genQuad(op,term1,term2,w);
		strcpy(term1,w);
		strcpy(**aTerm,term1);	
	}
}

void factor(char ****aTerm){
	if(token==number_tk){
		strcpy(***aTerm,buffer);
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){	
			token=lex();
		}
	}
	else if(token==parenthesisLeft_tk){	
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
		expression(**aTerm);
		if(token!=parenthesisRight_tk){
			printf("error message (syntax) :: expected ')' after expression at line %d\n",lines);
		}
		else{
			token=lex();
			while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
				token=lex();
			}
		}
	}
	else if(token==variable_tk){
		strcpy(***aTerm,buffer);
		idtail(&aTerm);
	}
}

void idtail(char *****aTerm){
	char *name;
	name = (char*)malloc(sizeof(char));
	strcpy(name,buffer);
	token=lex();
	while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){	
		token=lex();	
	}
	if(token==parenthesisLeft_tk){
		actualpars();
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){	
			token=lex();
		}
		name = newTemp();
		genQuad("par",name,"RET","       ");
		
		Entity *en;
		en=getEntity();
		en = insertTempVar(en,name);
		
		genQuad("call",****aTerm,"       ","       ");
		strcpy(****aTerm,name);
	}
}

void relationaloper(char **aRelOper){
	if((token>=60&&token<=62)||(token>=23&&token<=25)){
		strcpy(*aRelOper,buffer);
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
	}
	else{
		printf("error message (syntax) :: expected '=' '>' '<' '>=' '<=' '<>' at line %d\n",lines);
	}
}

void addoper(){
	token=lex();
	while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){	
		token=lex();	
	}
}

void muloper(){
	token=lex();
	while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){		
		token=lex();	
	}
}


void optionalsign(){
	if(token==plus_tk || token==minus_tk){
		addoper();
	}
}

void whilestat(){
	List *Btrue,*Bfalse;
	Btrue = emptyList();
	Bfalse = emptyList();
	Btrue = NULL;
	Bfalse = NULL;
	int rW=nextquad();
	char *returnWhile;
	returnWhile = (char*)malloc(sizeof(char));
	sprintf(returnWhile,"%d",rW);
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){	
		token=lex();	
	}
	if(token==parenthesisLeft_tk){
		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
		}
		condition(&Btrue,&Bfalse);
		backPatch(Btrue,nextquad());
		brackorstat();
		genQuad("jump","       ","       ",returnWhile);
		backPatch(Bfalse,nextquad());
		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
		}
	}
	else{
		printf("error message (syntax) :: expected '(' after if at line %d\n",lines);
	}
}


void forcasestat(){
	List *Btrue,*Bfalse;
	int Bsquad;
	char *squad;
	squad = (char*)malloc(sizeof(char));	
	Btrue  = emptyList();
	Bfalse = emptyList();
	Bsquad = nextquad();
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){		
		token=lex();		
	}
	if(token==curlyBracketLeft_tk){
		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){		
			token=lex();		
		}
		while(token==parenthesisLeft_tk){
			Btrue = NULL;
			Bfalse = NULL;
			if(token==parenthesisLeft_tk){
				token=lex();
				while(token==commentStart_tk||token==commentFinish_tk){		
					token=lex();		
				}
				if(token==when_tk){
					
					token=lex();
					while(token==commentStart_tk||token==commentFinish_tk){
						token=lex();
					}
					if(token==parenthesisLeft_tk){
						token=lex();
						while(token==commentStart_tk||token==commentFinish_tk){
							token=lex();
						}
						condition(&Btrue,&Bfalse);
						
						backPatch(Btrue,nextquad());
						brackorstat();
						token=lex();
						while(token==commentStart_tk||token==commentFinish_tk){
							token=lex();
						}
						
					
						if(token==parenthesisRight_tk){
							token=lex();
							while(token==commentStart_tk||token==commentFinish_tk){		
								token=lex();		
							}
							
							if(token==curlyBracketRight_tk){
								token=lex();
								while(token==commentStart_tk||token==commentFinish_tk){		
									token=lex();		
								}
								
							}
							sprintf(squad,"%d",Bsquad);
							genQuad("jump","       ","       ",squad);
							backPatch(Bfalse,nextquad());
						}
		
					}
				}else{
					printf("expected keyword when\n");
				}
	
	
			}
		}
	}
}


void incasestat(){
	List *Btrue,*Bfalse;
	Btrue  = emptyList();
	Bfalse = emptyList();
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){		
		token=lex();		
	}
	if(token==curlyBracketLeft_tk){
		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){		
			token=lex();		
		}
		while(token==parenthesisLeft_tk){
			Btrue = NULL;
			Bfalse = NULL;
			if(token==parenthesisLeft_tk){
				token=lex();
				while(token==commentStart_tk||token==commentFinish_tk){		
					token=lex();		
				}
				if(token==when_tk){
					token=lex();
					while(token==commentStart_tk||token==commentFinish_tk){
						token=lex();
					}
					if(token==parenthesisLeft_tk){
						token=lex();
						while(token==commentStart_tk||token==commentFinish_tk){
							token=lex();
						}
						condition(&Btrue,&Bfalse);
						backPatch(Btrue,nextquad());
						
						brackorstat();
						token=lex();
						while(token==commentStart_tk||token==commentFinish_tk){
							token=lex();
						}
						if(token==parenthesisRight_tk){
							token=lex();
							while(token==commentStart_tk||token==commentFinish_tk){		
								token=lex();		
							}
							if(token==curlyBracketRight_tk){
								token=lex();
								while(token==commentStart_tk||token==commentFinish_tk){		
									token=lex();		
								}
							}
							backPatch(Bfalse,nextquad());
						}
					}
				}
				else{
					printf("expected keyword when\n");
				}
			}
		}
	}
}




void printstat(){
	char *term;
	term = (char*)malloc(sizeof(char));
	token=lex();
	while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){	
		token=lex();	
	}
	if(token==parenthesisLeft_tk){
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
		expression(&term);
		genQuad("out",term,"       ","       ");
		if(token!=parenthesisRight_tk){
			printf("error message (syntax) :: expected ')' at line %d\n",lines);	
		}
		else{
			token=lex();
			while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
				token=lex();
			}
		}
	}
	else{
		printf("error message (syntax) :: expected '(' at line %d\n",lines);
	}
}

void returnstat(){
	char *term;
	term = (char*)malloc(sizeof(char));
	token=lex();
	while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){	
		token=lex();
	}
	if(token==parenthesisLeft_tk){
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		}
		expression(&term);
		genQuad("retv",term,"       ","       ");
		if(token!=parenthesisRight_tk){
			printf("error message (syntax) :: expected ')' at line %d\n",lines);	
		}
		else{
			token=lex();
			while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
				token=lex();
			}
		}
	}
	else{
		printf("error message (syntax) :: expected '(' at line %d\n",lines);
	}
}

void exitstat(){
	genQuad("exit","       ","       ","       ");
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){	
		token=lex();	
	}
}

void callstat(){
	char *nameSubProgram;
	nameSubProgram = (char*)malloc(sizeof(char));
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){		
		token=lex();	
	}
	if(token!=variable_tk){
		printf("expected a name for subprogram\n");
		exit(0);
	}
	else{
		strcpy(nameSubProgram,buffer);
	}
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){		
		token=lex();	
	}
	if(token==parenthesisLeft_tk){
		actualpars();
		genQuad("call",nameSubProgram,"       ","      ");
		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
		}
	}
	else{
		printf("error message (syntax) :: expected '(' after call at line %d\n",lines);	
	}
}

void dowhilestat(){
        List *Btrue,*Bfalse;
	Btrue = emptyList();
	Bfalse = emptyList();
	Btrue = NULL;
	Bfalse = NULL;
	int rW=nextquad();
	char *returnWhile;
	returnWhile = (char*)malloc(sizeof(char));
	sprintf(returnWhile,"%d",rW);
	brackorstat();
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){
		token=lex();
	}
	if(token==while_tk){
			token=lex();
			while(token==commentStart_tk||token==commentFinish_tk){
				token=lex();
			}
			if(token==parenthesisLeft_tk){
				condition(&Btrue,&Bfalse);
				backPatch(Btrue,nextquad());
				genQuad("jump","       ","       ",returnWhile);
				backPatch(Bfalse,nextquad());
				if(token!=parenthesisRight_tk){
					printf("error message (syntax) :: expected ')' at line %d\n",lines);	
				}
				else{
					token = lex();
					while(token==commentStart_tk||token==commentFinish_tk){
						token=lex();
					}
				}
			}
			else{
				printf("error message (syntax) :: expected '(' at line %d\n",lines);
			}	
	}
	else{
		printf("error message (syntax) :: expected keyword while at line %d\n",lines);
	}
}

void elsepart(){
	if(token==else_tk){
		brackorstat();
		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
		}
	}
}

void ifstat(){	
	Quad *jump;
	List *Btrue,*Bfalse;
	Btrue = emptyList();
	Bfalse = emptyList();
	Btrue = NULL;
	Bfalse = NULL;	
	List *elseList;
	elseList=emptyList();
	elseList=NULL;
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){
		token=lex();	
	}
	if(token==parenthesisLeft_tk){
		condition(&Btrue,&Bfalse);
		backPatch(Btrue,nextquad());
		brackorstat();
		jump = genQuad("jump","       ","       ","       ");
		elseList=makeList(jump);
		backPatch(Bfalse,nextquad());
		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
		}
		elsepart();
		backPatch(elseList,nextquad());
	}
	else{
		printf("error message (syntax) :: expected '(' after if at line %d\n",lines);
	}	
}

void assigmentstat(){
	char *term,*w;
	term = (char*)malloc(sizeof(char));

	char *name = (char*)malloc(sizeof(char));

	strcpy(name,buffer);

	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){
		token=lex();
	}
	if(token==assignment_tk){
		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
		}	
		expression(&term);
		genQuad(":=",term,"       ",name);
	}
	else {
		printf("error message (syntax) :: expected ':=' at line %d\n",lines);
	}
}

void statement(){
	if(token==variable_tk){
		assigmentstat();
	}
	else if(token==if_tk){
		ifstat();
	}	
	else if(token==while_tk){
		whilestat();
	}
	else if(token==do_tk){
		dowhilestat();
	}
	else if(token==exit_tk){
		exitstat();
	}
	else if(token==return_tk){
		returnstat();
	}
	else if(token==print_tk){
	
		printstat();
	}
	else if(token==incase_tk){
		incasestat();
	}
	else if(token==forcase_tk){
		forcasestat();
	}
	else if(token==call_tk){
		callstat();
	}
}

void bracketsseq(){
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){
		token=lex();
	}
	sequence();
	if(token!=curlyBracketRight_tk){
		printf("error message (syntax) :: expected '}' at line %d\n",lines);
		exit(0);
	}
}

void brackorstat(){
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){
		token=lex();
	}
	if(token==curlyBracketLeft_tk){
		bracketsseq();
	}
	else{
		statement();
	}
}

void sequence(){
	statement();
	while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
		token=lex();		
	}
	while(token==semicolon_tk){
		token=lex();
		while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
			token=lex();
		
		}
		if(token!=125){
			statement();
		}
		else{
			printf("expected statement after ';'\n");
			exit(0);
		}
	}
}


void formalparitem(Argument **argList,Entity **entity){
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
	}
	if(token==in_tk || token == inout_tk ||token==copy_tk){
	
		Argument *arg = *argList;
		*argList=insertArgument(arg);
		
		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
		}
		if(token!=variable_tk){
			printf("error message (syntax) :: expected an ID after in/inout/copy at line %d\n",lines);
		}else{
		
			Entity *ent = *entity;
			*entity = insertEntity(ent,variable_tp);
		}
	}
	else{
		printf("error message (syntax) :: expected keywords such as in/inout/copy  at line %d\n",lines);
	}
}


void formalparlist(){

	Argument *argument;
	Entity *entity;
	argument = NULL;
	entity = NULL;

	do{	
		formalparitem(&argument,&entity);
		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
		}
	}while(token==comma_tk);	
	if(argument==NULL){
	
		printf("argumet = NULL\n");
		
	}
	Entity * en = getEntity();
	en=insertArgumentListToEntity(en,argument);
	createScope();
	insertScope(entity);
}

void formalpars(){
	token=lex();
	while(token==commentStart_tk||token==commentFinish_tk){
		token=lex();
	}
	if(token==parenthesisLeft_tk){
		formalparlist();
		if(token!=parenthesisRight_tk){
			printf("error message (syntax) :: expected ')' at line %d\n",lines);
		}
		else{
			token = lex();
			while(token==commentStart_tk||token==commentFinish_tk){
				token=lex();
			}
		}
	}
}


void funcbody(){
	formalpars();
	programblock();
}

void varlist(){
	token = lex();
	while(token==commentStart_tk||token==commentFinish_tk){	
		token=lex();	
	}
	if(token==variable_tk){

		Entity *entity;
		entity = getEntity();

		while(token==variable_tk){

			entity=insertEntity(entity,variable_tp);
			
			token=lex();
			while(token==commentStart_tk||token==commentFinish_tk){
				token=lex();
			}
			if(token==comma_tk){
				token=lex();
				while(token==commentStart_tk||token==commentFinish_tk){
					token=lex();
				}
				if(token!=variable_tk&&token==enddeclare_tk){
					printf("error message (syntax) :: expected variable after ','at line %d\n",lines);
				}
			}
			else if(token==variable_tk){
				printf("error message (syntax) :: expected ',' before variable %s at line %d\n",buffer,lines);
			}
		}
		
		insertScope(entity);
	}
	

}

void declarations(){
	token = lex();
	while((token==commentStart_tk||token==commentFinish_tk)&&token!=curlyBracketRight_tk){
		token=lex();
	}
	if(token==declare_tk){
		varlist();
		if(token != enddeclare_tk){
			printf("error message (syntax) :: keyword enddeclare expected at line %d\n",lines);
			exit(EXIT_FAILURE);
		}
		else{
			token = lex();
			while(token==commentStart_tk||token==commentFinish_tk){
				token=lex();
			}
		}
	}
	else{		
	}
}


void func(){
	token = lex();
	while(token==commentStart_tk||token==commentFinish_tk){
		token=lex();
	}
	if(token == variable_tk){
		name = (char*)malloc(sizeof(char));
		strcpy(name,buffer);
		insertName(name);
		Entity *entity;
		entity = getEntity();
		entity = insertEntity(entity,function_tp);
		genQuad("begin_block",name,"       ","       ");
		funcbody();	
	}
	else{
		printf("error message (syntax) ::  name for function or procedure expected\n");
	}
}

void subprograms(){

	char *subname;
	int label;	

	subname = (char*)malloc(sizeof(char));

	while(token == procedure_tk || token == function_tk){

		endOfSubProgram++;

		func();
		subname=getNameOfSubProgram();	
		genQuad("end_block",subname,"       ","       ");

		endOfSubProgram--;

		calculateFrame();
		label = getStartQuad();
		checkQuad(label);
		deleteQuads(label);
		deleteScope();

		token=lex();
		while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
		}	
	}
}

void programblock(){
	if(token == curlyBracketLeft_tk){
		declarations();
		subprograms();
		if(endOfSubProgram==0){
			genQuad("begin_block",programName,"       ","       ");
		}
		sequence();
		if(endOfSubProgram==0){
			genQuad("halt","       ","       ","       ");
		}		
		if(endOfSubProgram==0){
			genQuad("end_block",programName,"       ","       ");
		}
		if(token!=curlyBracketRight_tk){
			printf("error message (syntax) :: expected '}' at line %d\n",lines);
		}
	}
	else{
		printf("error message (syntax) :: expected '{' at line %d\n",lines);
	}
}

void program(){

	Entity *e =NULL ;

	programName = (char*)malloc(sizeof(char));
	token = lex();
	while(token==commentStart_tk||token==commentFinish_tk){
		token=lex();	
	}
	if(token == program_tk){
		token = lex();
		while(token==commentStart_tk||token==commentFinish_tk){
			token=lex();
		}
		if(token == variable_tk){
			strcpy(programName,buffer);
			token = lex();
			while(token==commentStart_tk||token==commentFinish_tk){
				token=lex();
			}
			programblock();
		}
		else{
			printf("error message (syntax) :: a program name expected\n");	
		}
	}
	else{
		printf("error message (syntax) :: the keyword 'program' expected\n");
	}
	calculateFrame();
	
	fprintf(outFinal,"L0:\n");
	checkMainQuad();
	deleteScope();

	printf("Syntax Analyzing is OVER. \n");
}


main(int argc, char **argv){
	

	buffer = (char *) malloc(sizeof(char));
	char *outFile = (char*)malloc(sizeof(char));
	char *p = (char*)malloc(sizeof(char));
	lines = 1;
	variableNumber = 0;
	quadNumber=100;
	endOfSubProgram = 0;
	quadsList = NULL ;
	names = NULL;
	scope = NULL;

	if(argc != 2){

		printf("error :: no input file\n");
		exit(EXIT_FAILURE);

	}
	
	if(!(programFile=fopen(argv[1],"r"))){

		printf("error file %s does not exist\n",argv[1]);
		exit(EXIT_FAILURE);	

	}

	outFile = strtok(argv[1],".");
	p = strtok(0," ");

	if(strcmp(p,"st")!=0){

		printf("file must end with .st\n");
		exit(EXIT_FAILURE);

	}

	sprintf(outFile,"%s%s",outFile,".ascii");
	outFinal = fopen(outFile,"w");
		
	fprintf(outFinal,"L1: jmp L0\n");
	program();
	
	close(programFile);
	close(outFinal);
		
	
	
	
}

