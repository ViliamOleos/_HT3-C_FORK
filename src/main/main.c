//!-I. -std=c23 -Wall

/// ================================ [ DECLARATIONS ] ================================ ///

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

	#ifdef COMP_WINDOWS
#include <windows.h>
	#endif

/// #define TOKENNAME_DEBUG /// uncomment for token names

#include "../lexer/lexer.h"
#include "../parser/parser.h"

/// =================================== [ MACROS ] =================================== ///

/// === colour console ================

			#ifdef COMP_WINDOWS
#define CC_ENABLECOLOURCONSOLE() \
	do { \
		HANDLE console; DWORD consoleState; \
	 	\
		console = GetStdHandle(STD_OUTPUT_HANDLE); \
	 	\
		GetConsoleMode(console, &consoleState); \
		consoleState |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; \
		SetConsoleMode(console, consoleState); \
	} while(0)
			#else
#define CC_ENABLECOLOURCONSOLE() /* no action */
			#endif

#define RED     "\033[1;31m"
#define RESET   "\033[0m"

/// === print error ===================

/// prints the given message in this template: "error: <urmsg> \n", colouring "error:"
#define ERR(msg) printf(RED "error: " RESET msg "\n")

/// ==================================== [ FUNC ] ==================================== ///

char* readFile(const char *path, size_t* bytesread) {
    FILE *fp = fopen(path, "r");
    if(!fp) {
		ERR("File opening failed.");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = malloc(size + 1);
    if(!buffer) {
		ERR("Memory allocation failed.");
        fclose(fp);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, size, fp);
    buffer[read_size] = '\0';

    fclose(fp);

	*bytesread = read_size;
    return buffer;
}

/// ==================================== [ MAIN ] ==================================== ///

int main(int argc, char* argv[]) {
	size_t charread;

		CC_ENABLECOLOURCONSOLE();

	/// === file checks ===================

	/// 1 - exec call; 2 - file
    if(argc<2) { 
		ERR("Expected a source file.");
        return 1;
    }
    
    // source file: argv[1]
    if(access(argv[1], F_OK) != 0) {
		ERR("Source file doesn't exist.");
        return 1;
    }

	/// === main ==========================
    
    char* content = readFile(argv[1], &charread); printf("%llu bytes read.\n", charread);
    Token* tokenStream = lexer(content);
	free(content); /// content is presumably never used after this, so i recommend freeing

	for(; tokenStream->type != TOKEN_EOF; tokenStream++) {
		printf("TOKENNAME: " TOKENNAMEFMT "; TOKENVAL: %s;\n", 
				TOKENNAME(tokenStream->type), tokenStream->value
		); free(tokenStream->value);
	} printf("REACHED EOF\n");

    parser(tokenStream);

	/// === exfiltration ==================

    return(0);

	/// ===================================
}

/// ==================================== [ NOTE ] ==================================== ///
#if 0

#endif
/// ================================================================================== ///
