/*
 convert dos, mac text files into linux text files
 works inplace
*/
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>

using namespace std;

char buffer[65536];
char writeBuffer[65536*2];

struct cleanupTemp
{
	FILE *fp;
	cleanupTemp(FILE *fp) : fp(fp) { }
	~cleanupTemp() {
		if (fp != NULL) fclose(fp);
	}
};

// returns true on success
bool process(FILE *fi, FILE *fo)
{
	bool usedTmp = false;
	if (fo == NULL) {
		fo = tmpfile();
		if (fo == NULL) {
			fprintf(stderr, "Cannot create temp file for writing.\n");
			return false;
		}
		usedTmp = true;
	}

	cleanupTemp defer(usedTmp ? fo : NULL);

	bool needConversion = false;
	bool cr = false, lf = false;
	bool win = false;
	bool nonewline = true;
	for(;;) {
		size_t read = fread(buffer, 1, sizeof(buffer), fi);
		if (ferror(fi) && !feof(fi)) {
			fprintf(stderr, "Error reading the file: %d\n", ferror(fi));
			return false;
		}
		if (read == 0) break;

		size_t writingLength = 0;
		for(size_t p = 0; p < read; p++) {
			if (buffer[p] == '\r') {
				nonewline = false;
				needConversion = true;
				if (cr || lf) {
					writeBuffer[writingLength++] = '\n';
					cr = lf = false;
				}
				cr = true;
			} else if (buffer[p] == '\n') {
				nonewline = false;
				if (cr) { win = true; }
				if (lf) {
					writeBuffer[writingLength++] = '\n';
					cr = lf = false;
				}
				lf = true;
			} else {
				if (cr || lf) {
					writeBuffer[writingLength++] = '\n';
					cr = lf = false;
				}
				writeBuffer[writingLength++] = buffer[p];
			}
		}

		if (fwrite(writeBuffer, 1, writingLength, fo) != writingLength) {
			fprintf(stderr, "Cannot write\n");
			return false;
		}
	}

	if (nonewline) {
		// probably windows text file
		win = true;
		needConversion = true;
	}

	if (!needConversion) return true;

	// put EOL
	bool needeol =
		(win && (!cr && !lf)) ||
		(cr || lf);

	if (needeol) {
		if(fputc('\n', fo) == EOF) {
			fprintf(stderr, "Cannot write\n");
			return false;
		}
	}

	if (usedTmp) {
		// copy back to the file
		rewind(fi);
		rewind(fo);
		ftruncate(fileno(fi), 0);
		for(;;) {
			size_t read = fread(buffer, 1, sizeof(buffer), fo);
			if (read == 0) break;
			fwrite(buffer, 1, read, fi);
		}
	}

	return true;
}

int main(int argc, char *argv[]){
	if (argc >= 2) {
		for(int i = 1; i < argc; i++) {
			FILE *fp = fopen(argv[i], "r+b");
			if (fp == NULL) {
				fprintf(stderr, "The file '%s' cannot be opened.\n", argv[i]);
				return 1;
			}
			process(fp, NULL);
			fclose(fp);
		}
	} else {
		printf("Usage: %s [text-file-path]...\n", argc >= 1 ? argv[0] : "tolinuxfile");
	}
	return 0;
}

