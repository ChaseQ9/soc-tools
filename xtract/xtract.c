#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>



#define FILE_NAME_SIZE_MAX 64
#define BUFF_SIZE 256

void chop_newline(char *b);
int r_find_ip(char *l);
void find_ips(FILE *f);

int r_find_ip(char *line) {
	// Regex function to find things that look like an ip lol
	regex_t regex;
	// might have to delete the back slash below
	const char* IPv4_Pattern = "((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])";

	int status = regcomp(&regex, IPv4_Pattern, REG_EXTENDED);
    if (status != 0) {
        char err_buf[128];
        regerror(status, &regex, err_buf, sizeof(err_buf));
        fprintf(stderr, "Regex compilation failed: %s\n", err_buf);
        return EXIT_FAILURE;
    }

	// store the matches
	regmatch_t matches[1];

	const char *cursor = line;

	// loop over all matches
	while (regexec(&regex, cursor, 1, matches, 0) == 0) {

		int start = matches[0].rm_so;
		int end = matches[0].rm_eo;

		printf("Extracted IPv4: %.*s\n",
			   end - start,
			   cursor + start);

		// Advance past the matched IP
		cursor += end;
	}

    regfree(&regex);	
	return EXIT_SUCCESS;
}
void find_ips(FILE *fp) {
	char buff[BUFF_SIZE];
	while (fgets(buff, sizeof(buff), fp) != NULL) {
		chop_newline(buff);
		r_find_ip(buff);
		printf("%s", buff);
		printf("\n==NEW-LINE==\n");
	}
	return;

}
void chop_newline(char *buff) {
	buff[strcspn(buff, "\n")] = '\0';
}

int main(int argc, char **argv) {
	int opt;
	int defang_mode = 0;
	// set em all to 0
	char *file_input = NULL;

	// -d defang mode
	// -f pass file name
	while ((opt = getopt(argc, argv, "df:")) != -1) {
		switch(opt) {
			case 'd': 
				defang_mode = 1;
			case 'f':
				file_input = optarg;
				break;
		}
	}
	if (file_input == NULL) {
		printf("[ERROR]: xtract requires a filename (-f)\n");
		return 1;
	}
	// chop off newline (dont need it!)
	FILE *fp = fopen(file_input, "r");

	if (fp == NULL) {
		printf("[ERROR]: Cannot read file %s\n", file_input);
		return 1;
	}
	find_ips(fp);
	fclose(fp);
	return 0;
}
