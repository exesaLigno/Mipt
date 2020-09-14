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
#include <ctime>

#define DEBUG printf("DEBUG from %d\n", __LINE__);


class DirEntry
{
public:
	char* name = nullptr;
	char* pathname = nullptr;
	struct stat* meta = nullptr;			// Setting on creation
	
	short int inode_field_width = 0;
	short int link_count_field_width = 0;
	short int user_info_field_width = 0;
	short int group_info_field_width = 0;
	short int size_field_width = 0;
	
	DirEntry* next = nullptr;
	DirEntry* prev = nullptr;

public:
	DirEntry(const char* path, const char* dir_entry_name);
	
	void show(bool inod, bool detailed);
	
	
	~DirEntry();
};


class Directory
{
public:
	char* name = nullptr;
	
	DirEntry* entries = nullptr;
	
	int blocks_count = 0;
	
	short int max_inode_field_width = 0;			/// Setting up on reading directory
	short int max_link_count_field_width = 0;
	short int max_user_info_field_width = 0;
	short int max_group_info_field_width = 0;
	short int max_size_field_width = 0;
	
	int nesting_level = 0;
	
	Directory* next = nullptr;
	Directory* prev = nullptr;
	
public:
	Directory();
	Directory(const char* dir_name);
	Directory(const char* path, const char* dir_name);
	
	void read();
	void align();
	void show();
	
	~Directory();
};


int namecmp(const char* str1, const char* str2);
int intlen(long int number);


namespace Keys
{
	bool show_all = false;			// -a --all
	bool show_detailed = false;		// -l
	bool recusive = false;			// -R --recursive
	bool show_inod = false;			// -i --inod
	bool numeric_ids = false;		// -n --numeric-uid-gid
	bool line_by_line = false;		// -1
	bool tree_view = false;			// -T
	
	void parse(int argc, char* argv[]);
};


int main(int argc, char* argv[])
{
	Keys::parse(argc, argv);
	Directory* dir = new Directory(argv[1]);
	
	dir -> read();
	
	dir -> align();
	
	dir -> show();
	
	return 0;
}




Directory::Directory()
{
	this -> name = new char[strlen("./") + 1];
	strcpy(this -> name, "./");
}


Directory::Directory(const char* dir_name)
{
	this -> name = new char[strlen(dir_name) + 1];
	strcpy(this -> name, dir_name);
}


Directory::Directory(const char* path, const char* dir_name)
{
	this -> name = new char[strlen(path) + strlen(dir_name) + 2];
	
	strcpy(this -> name, path);
	
	if (*(strchr(this -> name, '\0') - 1) != '/')
		strcat(this -> name, "/");
	
	strcat(this -> name, dir_name);
}


Directory::~Directory()
{
	if (this -> name)
		delete[] this -> name;
	
	if (this -> entries)
		delete this -> entries;
	
	if (this -> next)
		delete this -> next;
}


void Directory::read()
{
	DIR* directory = opendir(this -> name);
	dirent64* dent = nullptr;
	
	while ((dent = readdir64(directory)) != nullptr)
	{		
		if (dent -> d_name[0] != '.' or Keys::show_all == true)
		{
			DirEntry* entry = new DirEntry(this -> name, dent -> d_name);
			this -> blocks_count += (entry -> meta -> st_blocks / 2);
			
			if (Keys::show_detailed or Keys::show_inod)
			{
				if (entry -> inode_field_width > this -> max_inode_field_width)
					this -> max_inode_field_width = entry -> inode_field_width;
				
				if (entry -> link_count_field_width > this -> max_link_count_field_width)
					this -> max_link_count_field_width = entry -> link_count_field_width;
				
				if (entry -> user_info_field_width > this -> max_user_info_field_width)
					this -> max_user_info_field_width = entry -> user_info_field_width;
				
				if (entry -> group_info_field_width > this -> max_group_info_field_width)
					this -> max_group_info_field_width = entry -> group_info_field_width;
				
				if (entry -> size_field_width > this -> max_size_field_width)
					this -> max_size_field_width = entry -> size_field_width;
			}
			
			if (this -> entries == nullptr)
				this -> entries = entry;
			
			else
			{
				
				DirEntry* current = this -> entries;
				
				if (namecmp(entry -> name, current -> name) < 0)
				{
					this -> entries = entry;
					entry -> next = current;
					current -> prev = entry;
					
					continue;
				}
				
				while (namecmp(entry -> name, current -> name) > 0 and current -> next != nullptr)
					current = current -> next;
				
				if (current -> next == nullptr and namecmp(entry -> name, current -> name) > 0)
				{
					current -> next = entry;
					entry -> prev = current;
				}
				
				else if (current -> prev != nullptr)
				{
					entry -> next = current;
					entry -> prev = current -> prev;
					current -> prev -> next = entry;
					current -> prev = entry;
				}
			}
			
		}
	}
}


void Directory::align()
{
	if (Keys::show_detailed or Keys::show_inod)		// passing throw if not detailed output
	{
		DirEntry* current = this -> entries;
		
		while (current != nullptr)
		{
			current -> inode_field_width = this -> max_inode_field_width;
			current -> link_count_field_width = this -> max_link_count_field_width;
			current -> user_info_field_width = this -> max_user_info_field_width;
			current -> group_info_field_width = this -> max_group_info_field_width;
			current -> size_field_width = this -> max_size_field_width;
			
			current = current -> next;
		}
	}
}


void Directory::show()
{	
	DirEntry* current = this -> entries;
	
	if (this -> next or this -> prev)
		printf("%s:\n", this -> name);
	
	printf("Total %d\n", this -> blocks_count);
	
	while (current)
	{		
		current -> show(Keys::show_inod, Keys::show_detailed);
		
		if (current -> next != nullptr)
		{
			if (Keys::line_by_line)
				printf("\n");
			
			else
				printf("  ");
		}
		
		if (S_ISDIR(current -> meta -> st_mode) and Keys::recusive)
		{
			Directory* nested = new Directory(this -> name, current -> name);
			
			nested -> nesting_level = this -> nesting_level + 1;
			
			nested -> read();
			nested -> align();

			if (Keys::tree_view)
			{
				nested -> show();
				delete nested;
			}
			
			else
			{
				Directory* current_dir = this;
				
				while (current_dir -> next)
				{
					if (current_dir -> next -> nesting_level == this -> nesting_level - 1)
						break;
					
					current_dir = current_dir -> next;
				}
				
				if (current_dir -> next == nullptr)
				{
					current_dir -> next = nested;
					nested -> prev = current_dir;
				}
				
				else
				{
					nested -> next = current_dir -> next;
					nested -> prev = current_dir;
					current_dir -> next = nested;
					nested -> next -> prev = nested;
				}
			}
		}
		
		current = current -> next;
	}
	
	printf("\n");
	
	if (this -> next)
	{	
		printf("\n");
		this -> next -> show();
	}
}


DirEntry::DirEntry(const char* path, const char* dir_entry_name)
{
	char* pathname = new char[strlen(path) + strlen(dir_entry_name) + 2];
	this -> pathname = pathname;
	
	strcpy(pathname, path);
	if (*(strchr(pathname, '\0') - 1) != '/')
		strcat(pathname, "/");
	strcat(pathname, dir_entry_name);
	
	this -> name = new char[strlen(dir_entry_name) + 1];
	strcpy(this -> name, dir_entry_name);
	
	this -> meta = new struct stat;
	lstat(pathname, this -> meta);
	
	if (Keys::show_detailed or Keys::show_inod)
	{
		this -> inode_field_width = intlen(this -> meta -> st_ino);
		this -> link_count_field_width = intlen(this -> meta -> st_nlink);
		this -> size_field_width = intlen(this -> meta -> st_size);
		
		this -> user_info_field_width = Keys::numeric_ids ? intlen(this -> meta -> st_uid) : strlen(getpwuid(this -> meta -> st_uid) -> pw_name);
		this -> group_info_field_width = Keys::numeric_ids ? intlen(this -> meta -> st_gid) : strlen(getgrgid(this -> meta -> st_gid) -> gr_name);
	}
}


DirEntry::~DirEntry()
{
	if (this -> name)
		delete[] this -> name;
	
	if (this -> next)
		delete this -> next;
}


void DirEntry::show(bool inod, bool detailed)
{
	mode_t mode = this -> meta -> st_mode;
	
	if (inod)
	{
		for (int i = 0; i < this -> inode_field_width - intlen(this -> meta -> st_ino); i++)
			printf(" ");
		
		printf("%lu ", this -> meta -> st_ino);
	}
	
	if (detailed)
	{
		printf("%c", S_ISDIR(mode) ? 'd' : 
					 S_ISLNK(mode) ? 'l' : 
					 S_ISCHR(mode) ? 'c' : 
					 S_ISBLK(mode) ? 'b' : 
					 S_ISSOCK(mode) ? 's' :
					 S_ISFIFO(mode) ? 'p' :  
					 S_ISREG(mode) ? '-' : '?');
		
		printf("%c%c%c", mode & 0400 ? 'r' : '-', mode & 0200 ? 'w' : '-', mode & 0100 ? 'x' : '-');
		printf("%c%c%c", mode & 0040 ? 'r' : '-', mode & 0020 ? 'w' : '-', mode & 0010 ? 'x' : '-');
		printf("%c%c%c ", mode & 0004 ? 'r' : '-', mode & 0002 ? 'w' : '-', mode & 0001 ? 'x' : '-');
		
		for (int i = 0; i < this -> link_count_field_width - intlen(this -> meta -> st_nlink); i++)
			printf(" ");
		
		printf("%lu ", this -> meta -> st_nlink);
		
		if (Keys::numeric_ids)
		{
			for (int i = 0; i < this -> user_info_field_width - intlen(this -> meta -> st_uid); i++)
				printf(" ");
			
			printf("%u ", this -> meta -> st_uid);
			
			for (int i = 0; i < this -> group_info_field_width - intlen(this -> meta -> st_gid); i++)
				printf(" ");
			
			printf("%u ", this -> meta -> st_gid);
		}
		
		else
		{
			printf("%s ", getpwuid(this -> meta -> st_uid) -> pw_name);
			
			for (int i = 0; i < this -> user_info_field_width - strlen(getpwuid(this -> meta -> st_uid) -> pw_name); i++)
				printf(" ");
			
			printf("%s ", getgrgid(this -> meta -> st_gid) -> gr_name);
			
			for (int i = 0; i < this -> group_info_field_width - strlen(getgrgid(this -> meta -> st_gid) -> gr_name); i++)
				printf(" ");
		}
		
		for (int i = 0; i < this -> size_field_width - intlen(this -> meta -> st_size); i++)
			printf(" ");
		
		printf("%lu ", this -> meta -> st_size);
		
		char date[20] = {};
		
		struct tm time;
		tzset();
		localtime_r(&(this -> meta -> st_mtim.tv_sec), &time);
		
		strftime(date, 20, "%b %-2d %-2H:%M", &time);
		
		printf("%s ", date);
	}
	
	printf("%s", S_ISDIR(mode) ? "\x1b[1;34m" :
				 S_ISLNK(mode) ? "\x1b[1;36m" : 
				 S_ISCHR(mode) ? "\x1b[1;33m" : 
				 mode & 0100 or mode & 0010 or mode & 0001 ? "\x1b[1;32m" : 
				 "\x1b[0m");
	
	printf("%s\x1b[0m", this -> name);
	
	printf("%s", S_ISDIR(mode) ? "/" :
				 S_ISLNK(mode) and detailed ? " → " : 
				 S_ISLNK(mode) and not detailed ? "@" : 
				 S_ISCHR(mode) ? "" : 
				 mode & 0100 or mode & 0010 or mode & 0001 ? "*" : 
				 S_ISREG(mode) ? "" : 
				 "?");
	
	if (S_ISLNK(mode) and detailed)
	{
		char real_address[100] = {};
		readlink(this -> pathname, real_address, 100);
		DirEntry real("", real_address);
		real.show(false, false);
	}
}



int namecmp(const char* str1, const char* str2)
{
	return strcasecmp(*str1 == '.' ? str1 + 1 : str1, *str2 == '.' ? str2 + 1 : str2);
}


int intlen(long int number)
{
	if (number == 0)
		return 1;
	
	int len = 0;
	
	while (number > 0)
	{
		number = number / 10;
		len++;
	}
	
	return len;
}



/*
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
	lstat(path, st);
	
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
		
		int seconds = time % 60;
		time = time / 60;
		
		int minutes = time % 60;
		time = time / 60;
		
		int hours = time % 24;
		time = time / 24;
		printf("%ld %d:%02d ", time, hours, minutes);
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



*/
void Keys::parse(int argc, char* argv[])
{
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
			
		}
	}
}
