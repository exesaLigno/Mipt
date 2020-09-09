#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <cstring>
#include <cstdlib>
#include <ctype.h>

#define DEBUG printf("DEBUG from %d\n", __LINE__);
const int MAX_GROUPS_COUNT = 20;


class User
{
private:
	
	int status = NOT_EXISTANCE;
	
	uid_t uid = EMPTY_ID;
	gid_t gid = EMPTY_ID;
	
	char* user_name = nullptr;
	
	int additional_groups_count = 0;
	gid_t* additional_groups = nullptr;
	
	
public:
	
	User();
	User(const char* user_name);
	User(const uid_t uid);
	
	void representUser();
	
	~User();
	
	enum CONSTANTS
	{
		EMPTY_ID = -1,
		OK,
		NOT_EXISTANCE
	};
};


uid_t getUid(const char* user_name);
gid_t getGid(uid_t uid);
char* getUserName(uid_t uid);
char* getGroupName(gid_t gid);
bool isNumber(const char* str);



int main(int argc, char* argv[])
{
	User** users = nullptr;
	int users_count = 0;
	
	if (argc == 1)
	{
		users_count = 1;
		users = new User*[users_count]{nullptr};
		users[0] = new User;
	}
	
	else
	{
		users_count = argc - 1;
		
		users = new User*[users_count]{nullptr};
		
		for (int counter = 1; counter < argc; counter++)
		{
			if (isNumber(argv[counter]))
				users[counter - 1] = new User(atoi(argv[counter]));
				
			else
				users[counter - 1] = new User(argv[counter]);
		}
	}
	
	for (int counter = 0; counter < users_count; counter++)
	{
		users[counter] -> representUser();
		delete users[counter];
	}
	
	delete[] users;
	
	return 0;
}



User::User()
{
	this -> uid = getuid();
	this -> gid = getgid();
	
	this -> user_name = getUserName(this -> uid);
	
	this -> additional_groups_count = MAX_GROUPS_COUNT;
	this -> additional_groups = new gid_t[MAX_GROUPS_COUNT];
	
	this -> additional_groups_count  = getgroups(MAX_GROUPS_COUNT, this -> additional_groups);
	
	this -> status = OK;
}


User::User(const char* user_name)
{
	this -> user_name = new char[strlen(user_name) + 1]{0};
	strcpy(this -> user_name, user_name);
	
	this -> uid = getUid(this -> user_name);
	
	if (this -> uid == (uid_t) EMPTY_ID)
		return;
	
	this -> gid = getGid(this -> uid);
	
	this -> additional_groups_count = MAX_GROUPS_COUNT;
	this -> additional_groups = new gid_t[MAX_GROUPS_COUNT];
	
	this -> additional_groups_count  = getgrouplist(this -> user_name, this -> gid, this -> additional_groups, &(this -> additional_groups_count));
	
	this -> status = OK;
}


User::User(const uid_t uid)
{
	this -> uid = uid;
	this -> gid = getGid(this -> uid);
	
	if (this -> gid == (gid_t) EMPTY_ID)
		return;
	
	this -> user_name = getUserName(this -> uid);
	
	this -> additional_groups_count = MAX_GROUPS_COUNT;
	this -> additional_groups = new gid_t[MAX_GROUPS_COUNT];
	
	this -> additional_groups_count  = getgrouplist(this -> user_name, this -> gid, this -> additional_groups, &(this -> additional_groups_count));
	
	this -> status = OK;
}



void User::representUser()
{
	if (this -> status == NOT_EXISTANCE and this -> uid != (uid_t) EMPTY_ID)
		printf("id: «%d»: there is no such user\n", this -> uid);
	
	else if (this -> status == NOT_EXISTANCE and this -> user_name != nullptr)
		printf("id: «%s»: there is no such user\n", this -> user_name);
	
	else
	{
		printf("uid=%d(%s) ", this -> uid, this -> user_name);
		char* group_name = getGroupName(this -> gid);
		printf("gid=%d(%s)", this -> gid, group_name);
		
		delete[] group_name;
		
		if (additional_groups_count != 0)
		{
			printf(" groups=");
			
			for (int counter = 0; counter < additional_groups_count; counter++)
			{
				char* additional_group_name = getGroupName((this -> additional_groups)[counter]);
				printf("%d(%s)%c", (this -> additional_groups)[counter], additional_group_name, counter < additional_groups_count - 1 ? ',' : '\n');
			
				delete[] additional_group_name;
			}
		}
		
		else
			printf("\n");
	}
}



User::~User()
{
	if (this -> user_name != nullptr)
		delete[] this -> user_name;
	
	if (this -> additional_groups != nullptr)
		delete[] this -> additional_groups;
}





uid_t getUid(const char* user_name)
{
	uid_t uid = User::EMPTY_ID;
	
	passwd* user = getpwnam(user_name);
	
	if (user != nullptr)
	{
		uid = user -> pw_uid;
	}
	
	return uid;
}


gid_t getGid(uid_t uid)
{
	gid_t gid = User::EMPTY_ID;
	
	passwd* user = getpwuid(uid);
	
	if (user != nullptr)
	{
		gid = user -> pw_gid;
	}
	
	return gid;
}


char* getUserName(uid_t uid)
{
	char* name = nullptr;
	passwd* user = getpwuid(uid);
	
	if (user != nullptr)
	{
		name = new char[strlen(user -> pw_name) + 1];
		strcpy(name, user -> pw_name);
	}

	return name;
}


char* getGroupName(gid_t gid)
{
	char* name = nullptr;
	
	group* group = getgrgid(gid);
	
	if (group != nullptr)
	{
		name = new char[strlen(group -> gr_name) + 1];
		strcpy(name, group -> gr_name);
	}
	
	return name;
}


bool isNumber(const char* str)
{
	bool is_number = true;
	
	for (int counter = 0; str[counter] != '\0'; counter++)
	{
		if (!isdigit(str[counter]))
		{
			is_number = false;
			break;
		}
	}
	
	return is_number;
}
