#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <cstring>
#include <pwd.h>
#include <grp.h>


enum FILES
{
	DIRECTORY,
	SYMBOLIC_DEVICE,
	BLOCK_DEVICE,
	SIMPLE_FILE,
	FIFO,
	SYMBOLIC_LINK,
	UNKNOWN
};


namespace Keys
{
	bool show_all = false;			// -a --all
	bool show_detailed = false;		// -l
	bool recusive = false;			// -R --recursive
	bool show_inod = false;			// -i --inod
	bool numeric_ids = false;		// -n --numeric-uid-gid
	bool line_by_line = false;		// -1
	
	char** pathes = nullptr;
	int pathes_count = 0;
	
	void parse(int argc, char* argv[]);
};


void showDir(const char* dir_name);
void showEntry(const char* dir, dirent64* entry);


int main(int argc, char* argv[])
{	
	Keys::parse(argc, argv);
	
	if (Keys::pathes_count == 0)
		showDir(".");
	
	for (int counter = 0; counter < Keys::pathes_count; counter++)
	{
		if (Keys::pathes_count > 1)
			printf("%s:\n", Keys::pathes[counter]);
		
		showDir(Keys::pathes[counter]);
		
		if (counter < Keys::pathes_count - 1)
			printf("\n");
	}
	
	return 0;
}



void showDir(const char* dir_name)
{
	DIR* directory = opendir(dir_name);
	dirent64* entry = nullptr;
	
	while ((entry = readdir64(directory)) != nullptr)
	{
		if (entry -> d_name[0] != '.' or Keys::show_all == true)
		{
			showEntry(dir_name, entry);
			
			if (Keys::line_by_line)
				printf("\n");
			
			else
				printf("  ");
		}
	}
	
	if (not Keys::line_by_line)
		printf("\n");
}


void showEntry(const char* dir, dirent64* entry)
{
	struct stat* st = new struct stat;
	char path[strlen(dir) + strlen(entry -> d_name) + 2];
	strcpy(path, dir);
	strcat(path, "/");
	strcat(path, entry -> d_name);
	stat(path, st);
	
	mode_t mode = st -> st_mode;
	
	int file_type = mode & 0040000 ? DIRECTORY : 
					mode & 0020000 ? SYMBOLIC_DEVICE : 
					mode & 0060000 ? BLOCK_DEVICE : 
					mode & 0100000 ? SIMPLE_FILE :
					mode & 0010000 ? FIFO :
					mode & 0120000 ? SYMBOLIC_LINK : UNKNOWN;
	
	nlink_t link_count = st -> st_nlink;
	uid_t uid = st -> st_uid;
	gid_t gid = st -> st_gid;
	off_t size = st -> st_size;
	time_t time = st -> st_mtim.tv_sec;
	
	if (Keys::show_inod)
		printf("%lu ", entry -> d_ino);
	
	if (Keys::show_detailed)
	{
		printf("%c", file_type == DIRECTORY ? 'd' : 
					 file_type == SYMBOLIC_LINK ? 'l' : 
					 file_type == SYMBOLIC_DEVICE ? 'c' : 
					 file_type == SIMPLE_FILE ? '-' : '?');
		
		printf("%c%c%c", mode & 0400 ? 'r' : '-', mode & 0200 ? 'w' : '-', mode & 0100 ? 'x' : '-');
		printf("%c%c%c", mode & 0040 ? 'r' : '-', mode & 0020 ? 'w' : '-', mode & 0010 ? 'x' : '-');
		printf("%c%c%c ", mode & 0004 ? 'r' : '-', mode & 0002 ? 'w' : '-', mode & 0001 ? 'x' : '-');
		
		printf("%lu ", link_count);
		
		if (Keys::numeric_ids)
			printf("%d %d ", uid, gid);
		
		else
			printf("%s %s ", getpwuid(uid) -> pw_name, getgrgid(gid) -> gr_name);
		
		printf("%ld ", size);
	}
	
	printf("%s", file_type == DIRECTORY ? "\x1b[1;34m" : 
				 file_type == SYMBOLIC_LINK ? "\x1b[1;35m" : 
				 file_type == SYMBOLIC_DEVICE ? "\x1b[1;33m" : 
				 mode & 0100 or mode & 0010 or mode & 0001 ? "\x1b[1;32m" : 
				 "\x1b[0m");
	
	printf("%s\x1b[0m", entry -> d_name);
	
	printf("%s", file_type == DIRECTORY ? "/" : 
				 mode & 0100 or mode & 0010 or mode & 0001 ? "*" : 
				 "");
}




void Keys::parse(int argc, char* argv[])
{
	pathes = new char*[argc - 1];
	
	for (int counter = 1; counter < argc; counter++)
	{
		char* str = argv[counter];
		
		if (str[0] == '-' and str[1] != '-' and str[strlen(str) - 1] != '/')	// parsing one-letter parameters
		{
			if (strchr(str, 'a') != nullptr)
				show_all = true;
			
			if (strchr(str, 'l') != nullptr)
			{
				show_detailed = true;
				line_by_line = true;
			}
			
			if (strchr(str, 'R') != nullptr)
			{
				recusive = true;
				line_by_line = true;
			}
			
			if (strchr(str, 'i') != nullptr)
				show_inod = true;
			
			if (strchr(str, 'n') != nullptr)
			{
				numeric_ids = true;
				show_detailed = true;
				line_by_line = true;
			}
			
			if (strchr(str, '1') != nullptr)
				line_by_line = true;
		}
		
		else if (str[0] == '-' and str[1] == '-' and str[strlen(str) - 1] != '/')	// parsing one-word parameter
		{
			
		}
		
		else
		{
			pathes[pathes_count] = str;
			pathes_count++;
		}
	}
}
