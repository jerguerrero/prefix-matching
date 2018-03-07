/* Query prefix matching program
 * Written by Judd Eduardo Guerrero, 2017
 * Student Number 917105
 */

#include<stdio.h>
#include<ctype.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define NEWLINE '\n' /* New line character */
#define SPACE ' ' /* Character space */
#define MAX_VAL 5 /* Maximum value of lines, points, and line number */
#define MAX_LENGTH 1000 /* Maximum characters for each line */

/****************************************************************/

/* function prototypes */

int mygetchar(); 
int islowercase(char character);
int isalphanumeric(char character);
int isuppercase(char character);
int match(char *line, int size, char query[]);
void printdiv(void);
void printerror(void);


/****************************************************************/

/* Main program controls all the function */

int main(int argc, char* argv[]){
	
	/* Create all variables */
	int i  = 0, j = 0, k = 0, l = 0, m = 0, n = 0, 
	character, line_count = 0, byte_count = 0,
	word_count = 0, word_index = 0, 
	match_count[argc], line_index = 0, rank = 0,
    temp_index = 0, num_elem = 0, line_number[MAX_VAL] = {0,0,0,0,0}, 
    newline = 1, prev_word_count = 1;
	
	char *temp_word, prev_char = ' ', 
	word[MAX_LENGTH], 
	line_temp[1][MAX_LENGTH] = {""},
	top_lines[MAX_VAL][MAX_LENGTH] = {"\n","\n","\n","\n","\n"}, 
	*non_alnum_words[argc];
	
	double score = 0, final_score = 0, line_score[MAX_VAL] = {0.0,0.0,0.0,0.0,0.0};
	
	/* Stage 1 no queries */
	if (argc < 2){
		printerror();
		exit(0);
	}
	/* Stage 1 with incorrect queries */
	else{
		printf("S1: query = ");
		
		for(i = 1; i<argc; i++){
			
			/* Prints queries */
			printf("%s ", argv[i]);
			temp_word = argv[i];
			
			for(j = 0; j < strlen(temp_word); j++){
				
				/* Adds each word to an array that aren't alphanumeric lowercase */
				if( (!islowercase(temp_word[j])) && (!isdigit(temp_word[j])) ){
					non_alnum_words[k] = temp_word;
				    k++;
					break;
					
				}
			}
		}
		printf("\n");
	}
	
	/* Prints the incorrect queries from Stage 1 */
	if(!(sizeof(non_alnum_words)==0)){
		for(i = 0; i < k; i++){
			printf("S1: %s: invalid character(s) in query\n", non_alnum_words[i]);
		}
		//exit(0);
	}

	/* Initialise match_count */
	for(l = 1; l < argc; l++){
				match_count[l] = 0;
			}
			
    /* Initialise line_score */
	for(l = 1; l < MAX_VAL; l++){
				line_score[l] = 0;
			}
			
	/* Reads the input file till EOF */		
	while((character = mygetchar()) != EOF){
		
		/* prints "---" divider when it's a newline and the previous line
		 * is not empty */
		if(newline == 1 && prev_word_count !=0){
			printdiv();
			prev_word_count = 0;
			newline = 0;
		}
		
		/* prints each character for every line when the word count in the 
		 * line is zero 
		 */
		if((character == NEWLINE && word_count > 0) || (character != NEWLINE)){
			putchar(character);
		}
		
		/* Stores line temporarily */
        line_temp[rank][line_index] = character;
        line_index ++;
        
        /* Converts each letter to lowercase and temporarily stores each word 
         * to compare with query words 
         */
		if(isalphanumeric(character)){
				word[word_index] = tolower(character);
				word_index ++;
		}
       
        
		/* Counts new words */
		if((isalphanumeric(prev_char)) && (!isalphanumeric(character))){
			word_count ++;	
			word_index = 0;
			
			/* Counts the number each query matches with a prefix */
			for(l = 1; l < argc; l++){
				match_count[l] = match_count[l] + match(word, strlen(argv[l]), argv[l]);
			}
			
			
		}
	
		/* Counts new lines */
		if(character == NEWLINE){
			
			/* Sets the newline to true and sets previous word count to current
			 * for next iteration 
			 */
            newline = 1;
		    prev_word_count = word_count;
		    
		    /* Calculates the numerator for the line that's being read */
			for(l = 1; l<argc; l++){
				score += log(1.0 + match_count[l]) / log(2.0);
				match_count[l] = 0;
			}
			
			/* Counts the line number*/
			line_count ++;
		    /* calculates final score for line being iterated */
			final_score = score/(log(8.5+word_count)/log(2)); 
			
			/* Prints line number, score, bytes and number of words for
			 * each line 
			 */
			if(word_count != 0){
				printf("S2: line = %d, bytes = %d, words = %d\n", line_count, byte_count-1, word_count);
				printf("S3: line = %d, score = %.3f\n", line_count, final_score);
			}
		    
		    
		    /* Reads lines that have scores higher than zero */
		    if(final_score > 0){
		    	
		    	/* Counts number of elements that has been stored to the
		    	 * array of points, line count, and strings 
		    	 */
		    	if(num_elem < MAX_VAL){
		    		num_elem ++;
		    	}
		    	
		    	/* Compares the string of line being iterated against the ones 
		    	 * stored in the array of lines with the highest scores
		    	 * and stores the index where to insert the line being iterated
		    	 * to temp_index 
		    	 */
		    	for (m = 0; m < MAX_VAL; m++){
		    		if((final_score > line_score[m]) && (line_count > line_number[m])){
		    			temp_index = m;
		    			break;
		    		}
		    		temp_index = m;
		    	}
		    	
		    	/* Shifts all the elements of the array of top lines to the 
		    	 * right
		    	 */
		    	for (n = num_elem - 1; n > temp_index ; n--){
		    		line_score[n] = line_score[n-1];
		    		line_number[n] = line_number[n-1];		    	    
		    		memmove(top_lines[n], top_lines[n-1], MAX_LENGTH);
		    	}
		    	
		    	/* Inserts string to temp_index location of top_lines array */
		    	if(m != MAX_VAL){
		    	line_score[temp_index] = final_score;
		    	line_number[temp_index] = line_count;
		    	memcpy(top_lines[temp_index], line_temp[0], MAX_LENGTH);
		    	}
		    }
		    
		    /* Resets values to 0 for the next loop*/
		    memset(line_temp[0], 0, MAX_LENGTH);
		    line_index = 0;
			score = 0;
			word_count = 0;
		    byte_count = 0;   
		}
		
		/* Store previous character to check against current character for 
		word count */
		prev_char = character;
        
		/* Counts byte number for each time the while loop reads a character*/
		byte_count++;	
	}
	
	printf("------------------------------------------------\n");

	/* Stage 4: prints the lines with the highest scores */
	for(i = 0; i < num_elem; i++){
		//strcpy(top_lines[i+1], top_lines[i]);
		printf("S4: line = %d, score = %.3f\n", line_number[i], line_score[i]);
		printf("%s", top_lines[i]);
        printdiv();
	}
	
	
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
 * a size (length of prefix which is equal to the length of query passed as
 * argument), and a string of query
 * returns false if there are no word matches
 * returns true if a the query matches a prefix
 */

int match(char *line, int size, char query[]){
	int i = 0;
	for(i = 0; i < size; i++){
		if(!(line[i] == query [i])){
			return 0;
		}
	}
	return 1;
}

/****************************************************************/

/* getchar equivalent that takes into account formatting saved into a PC
 */

int mygetchar() {
		int c;
		while ((c=getchar())=='\r') {
		}
		return c;
	}
	
/****************************************************************/

/* Reads a single character
 * returns true if the character is lowercase letter
 */
	
int islowercase(char character){
	if(character >= 'a' && character <= 'z'){
		return 1;
	}
	return 0;
}

/****************************************************************/

/* Reads a single character
 * returns true if the character is uppercase letter
 */

int isuppercase(char character){
	if(character >= 'A' && character <= 'Z'){
		return 1;
	}
	return 0;
}

/****************************************************************/

/* Reads a single character
 * returns true if the character is alphanumeric.
 */

int isalphanumeric(char character){
	if(isuppercase(character) || islowercase(character) || isdigit(character)){
		return 1;
	}
	return 0;
}

/****************************************************************/

/* Prints "---" and a new line
 */
 
void printdiv(void){
	printf("---\n");
}

/****************************************************************/

/* Prints error message for no queries
 */ 

void printerror(void){
	printf("S1: No query specified, must provide at least one word\n");
}

