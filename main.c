// main.c -- id3info: read the id3 data of an mp3 file

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

void help();

int main(int argc, char** argv) {
	if(argc == 1) {
		help();
		return 0;
	}
	char* filename = argv[1];
	int fd = open(filename, O_RDONLY);
	if(fd == -1) {
		puts("Failed to open file");
		return 1;
	}
	struct stat st;
	fstat(fd, &st);
	long filesize = st.st_size;
	printf("Filesize: %ldB\n", filesize);
	long start_index = filesize - 128;
	char* file = malloc(filesize);
	if(file == NULL)
		return 2;
	read(fd, file, filesize);
	char* data = malloc(128);
	if(data == NULL)
		return 3;
	char* p_start = file + start_index;
	memcpy(data, p_start, 128);
	char* tag_cmp = malloc(3);
	memcpy(tag_cmp, data, 3);
	if(0 != strncmp(tag_cmp, "TAG", 3)) {
		puts("ID3v1 tag not found.");
		return -1;
	}
	char* title = malloc(30);
	char* artist = malloc(30);
	char* album = malloc(30);
	char* year = malloc(4);
	char* comment = malloc(30);
	char* genre = malloc(1);
	memcpy(title, data + 3, 30);
	memcpy(artist, data + 33, 30);
	memcpy(album, data + 63, 30);
	memcpy(year, data + 93, 4);
	memcpy(comment, data + 97, 30);
	memcpy(genre, data + 127, 1);
	printf("Title:		");
	puts(title);
	printf("Artist:		");
	puts(artist);
	printf("Album:		");
	puts(album);
	printf("Year:		");
	puts(year);
	printf("Comment:	");
	puts(comment);
	printf("Edit a field?(y/n)> ");
	fflush(stdout);
	char* buffer = malloc(255);
	read(0, buffer, 255);
	if(*buffer == 'Y' || *buffer == 'y') {
		printf("What field?(lowercase)> ");
		fflush(stdout);
		memset(buffer, 0, 255);
		read(0, buffer, 255);
		if(0 == strncmp(buffer, "title", 5)) {
			printf("Enter new title(max 30)>");
			fflush(stdout);
			memset(buffer, 0, 255);
			read(0, buffer, 30);
			printf("Setting title to ");
			puts(buffer);
		} else if(0 == strncmp(buffer, "artist", 6)) {
			puts("Editing artist");
		} else if(0 == strncmp(buffer, "album", 5)) {
			puts("Editing album");
		} else if(0 == strncmp(buffer, "year", 4)) {
			puts("Editing year");
		} else if(0 == strncmp(buffer, "comment", 7)) {
			puts("Editing comment");
		} else {
			puts("Unknown option.");
		}
		
	} else if(*buffer == 'N' || *buffer == 'n') {
		puts("Not editing field.");
	} else {
		puts("Unknown field.");
	}

	close(fd);
	return 0;
}

void help() {
	puts(\
	"id3info: id3v1 reader\n"\
	"usgae: id3info <filename>"\
	);
}
