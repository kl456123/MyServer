#include "web_server.h"

#define SIZE 1024
int main() {
	char uri[] = "/cgi-bin?100&20";
	char filename[SIZE];
	char cgi_args[SIZE];
	int res = ParseUri(uri, filename, cgi_args);
	if (res) {
		printf("static serve\n");
		printf("static webpage: %s\n", filename);
	} else {
		printf("dynamic serve\n");
		printf("dynamic webpage: %s\n", filename);
		printf("args: %s\n", cgi_args);
	}
	return 0;
}