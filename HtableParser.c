#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 80

double H_array[10];
const char *str_ptr ;

int priority(char op) { 
    switch(op) { 
    	case '~': 			return 1;
        case '+': case '-':	return 2;
        case '*': case '/': return 3;
        default:            return 0;
    } 
}

void inToPostfix(char* infix, char* postfix) { 
    char stack[MAX] = {'\0'};
    int i, j, top;
    for(i = 0, j = 0, top = 0; infix[i] != '\0'; i++)	switch(infix[i]) { 
	        case '(':case '[':                // 運算子堆疊 
	            stack[++top] = infix[i]; 
	            break; 
	        case '+':  case '*': case '/':case '-':
	            while(priority(stack[top]) >= priority(infix[i])) { 
	                postfix[j++] = stack[top--];
	            } 
	            stack[++top] = infix[i]; // 存入堆疊 
	            break; 
			/*case '~':  
	            while(priority(stack[top]) >= priority(infix[i])) { 
	                postfix[j++] = stack[top--];
	            } 
	            stack[++top] = infix[i]; // 存入堆疊 
	            break; */
	        case ')': case ']':
	            //while(stack[top] != '(' ) { // 遇 ) 輸出至 ( 
	            while(stack[top] != '(' && stack[top] != '[') { // 遇 ) 輸出至 ( 
	                postfix[j++] = stack[top--];
	            } 
	            top--;  // 不輸出 ( 
	            break; 
	        case ' ':
	        	while(infix[i+1] == ' ') i++;
	        	postfix[j++] = infix[i];
	        	break;
	        default:  // 運算元直接輸出 
	            postfix[j++] = infix[i];
    	}
   	while(top > 0) { 
       	postfix[j++] = stack[top--];
   	}
   
   printf("inToPostfix %s\n",postfix);
} 


// Return the position of character c
// in string s, or -1 if c not found
static int chrpos(char *s, int c) {
  char *p;

  p = strchr(s, c);
  return (p ? p - s : -1);
}


void ReplaceHkeyWord(double *table_Array,const char *inputStr,char *outputStr){
	const char *leftStr;
	double temp;
	char NewFloatStr[16] ={0};
	int i,j,k;
	//str_ptr = strstr(inputStr, "H");
	//printf("PostfixToDouble %s\n",inputCmd);
	for(i = 0,j = 0; !(inputStr[i] == '\0' || inputStr[i] == '\n'); i++){
	
		switch(inputStr[i]){
			case ' ':case '=':
				break;
			case '-':
				if(chrpos("0123456789",inputStr[i+1]) >0){
					outputStr[j++] = '~';
				}else{
					outputStr[j++] = inputStr[i];
				}
				break;
			case 'H':
				//temp = H_array[atoi((inputStr+i+1))];		
				temp = table_Array[atoi((inputStr+i+1))];				
				if(temp < 0){
					outputStr[j++] = '~';
					gcvt(-temp,7,NewFloatStr);
				}else{
					gcvt(temp,7,NewFloatStr);
				}
				for(k = 0 ; k < strlen(NewFloatStr) ; k++){
					outputStr[j++] = NewFloatStr[k];
					
				}
				
				if(NewFloatStr[k-1] == '.'){
					j--;
				}
				outputStr[j++] = ' ';
				i++;
				
				while(inputStr[i] >= '0' && inputStr[i] <= '9' ){
					i++;					
				} 
				if(inputStr[i] == '+' || inputStr[i] == '-' || inputStr[i] == '*' || inputStr[i] == '/' || inputStr[i] == ']' || inputStr[i] == ')'){
					i--;					
				} 
				break;
			default:
				outputStr[j++] = inputStr[i];
				break;
		}
		
	}
	outputStr[j] = '\0';
  	printf("ReplaceHkeyWord %s\n",outputStr);
	
	
  	
}



const char *FindKeyWord(char c){
	while (' ' == c || '\t' == c || '\r' == c || '\f' == c || *str_ptr != c) {
		c = *str_ptr;
		
		str_ptr = str_ptr + 1;	
		printf("FindKeyWord %s\n",str_ptr);
  	}
  	str_ptr = str_ptr+1;
  	return str_ptr;
}


int scanHaddr(char *Str){
  	int k, val = 0;
    int c = *Str;
	Str = Str + 1; 
    //printf("scanHaddr: %c \n",c);
    // Convert each character into an int value
    while ((k = chrpos("0123456789", c)) >= 0) {
      	val = val * 10 + k;
      	//c = next();
      	c = *Str;
		Str = Str + 1; 
    }
  return val; 
}


double mathcal(char op,double x,double y){
	printf("mathcal %c\n",op);
	switch(op){
		case '+':
			return x+y;	
		case '-':
			return x-y;
		case '*':
			return x*y;
		case '/':
			if(y == 0){
				return 0;
			}else{
				return x/y;
			}
			
		default:
			return -999.;
	}
  	
}

double PostfixToDouble(char *inputCmd){
	char NewFloatStr[16];
	double stack[MAX] = {0};
	double result,temp;
	int sign = 1;
	int i,j,top = 0;
	printf("PostfixToDouble %s\n",inputCmd);
	//str_ptr = strstr(inputStr, "H");
	for(i = 0; !(inputCmd[i] == '\0' || inputCmd[i] == '\n'); i++){
		switch(inputCmd[i]){
			
			case '+': case '*': case '/': case '-': 
				
	           	for(j = 0 ; j< top ; j++){
	           		printf("1.stack[%d]: %f\n",j,stack[j]);
				}
				temp = mathcal(inputCmd[i],stack[top-2],stack[top-1]);
				stack[top-2] = temp;
				top--;
				//printf("temp %f \n",temp);
	            break;
	        case '~': 
	        	sign = -sign;
	            break;
	        case '1': case '2': case '3': case '4':case '5': case '6': case '7': case '8':case '9': case '0': case '.':
	        	
	        	for(j = 0 ; (inputCmd[i] != ' ' && isdigit(inputCmd[i]) ) || inputCmd[i] == '.'; i++){
	        		NewFloatStr[j++] = inputCmd[i];
				}
				NewFloatStr[j] = '\0';
				
				i--;
				
	        	stack[top] = sign*atof(NewFloatStr);
	        	sign = 1;
	        	
				
				top++;
	        	break;
			case ' ':case '=':
				break;
			default:
				
				break;
		}
		
	}
	result = stack[0];
  	printf("PostfixToDouble %f\n",result);
  	return  result;
}

int HTable_Parse(double *table_Array,const char *inputCmd){
	
	char Outstr[128];
	char postfix[MAX]= {'\0'}; 
	const char *leftWord;
	double result;
	int H_index;
	str_ptr = inputCmd;
	printf("HTable_Parse %s\n",str_ptr);
	leftWord = FindKeyWord('H');
	
	str_ptr = strstr(str_ptr, "=");
	
	printf("After FindKeyWord %s\n",str_ptr);
	ReplaceHkeyWord(table_Array,str_ptr,Outstr);
	//printf("After ReplaceHkeyWord %s\n",Outstr);
	inToPostfix(Outstr,postfix);
	printf("After inToPostfix %s %s\n",str_ptr,postfix);
	result = PostfixToDouble(postfix);
	H_index = scanHaddr((char *)leftWord);
	if(H_index >= 0){
		table_Array[H_index] = result;
	}
	printf("H_array[%d]: %f\n",H_index,table_Array[H_index]);
	return 0;
}

static void usage(char *prog) {
  fprintf(stderr, "Usage: %s infile\n", prog);
  exit(1);
}

int main(int argc,char *argv[]){
	int i;
	for(i = 0 ; i < 10 ; i++){
    	H_array[i] = -i;
    	H_array[i]+= 0.1;
  	}
  	//H_array[3] = -1.3;
  	for(i = 0 ; i < 10 ; i++){
    	printf("H[%d]: %f\n",i,H_array[i]);
  	}	
	if (argc != 2)
    	usage(argv[0]);
    HTable_Parse(H_array,argv[1]);
    for(i = 0 ; i < 10 ; i++){
    	printf("H[%d]: %f\n",i,H_array[i]);
  	}	
    return 0;
	
}

