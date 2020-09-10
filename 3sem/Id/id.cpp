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
	
	int additional_groups_count = 0;
	gid_t* additional_groups = nullptr; 
	
	
public:
	
	User();
	User(const char* user_name);
	User(const uid_t uid);
	
	void represent();
	
	~User();
	
	enum CONSTANTS
	{
		EMPTY_ID = -1,
		OK,
		NOT_EXISTANCE
	};
};


bool isNumber(const char* str);



int main(int argc, char* argv[])
{	
	if (argc == 1)
	{
		User user;
		user.represent();
	}
	
	for (int counter = 1; counter < argc; counter++)
	{
		User user = argv[counter];
		user.represent();
	}
	
	return 0;
}



User::User()
{
	this -> uid = getuid();
	this -> gid = getgid();
	
	this -> additional_groups_count = MAX_GROUPS_COUNT;
	this -> additional_groups = new gid_t[MAX_GROUPS_COUNT];
	
	this -> additional_groups_count  = getgroups(MAX_GROUPS_COUNT, this -> additional_groups);
	
	this -> status = OK;
}


User::User(const char* parameter)
{
	passwd* user = isNumber(parameter) ? getpwuid(atoi(parameter)) : getpwnam(parameter);
		
	if (user == nullptr)
	{
		printf("id: «%s»: there is no such user\n", parameter);
		return;
	}
	
	this -> uid = user -> pw_uid;
	this -> gid = user -> pw_gid;
	
	this -> additional_groups_count = MAX_GROUPS_COUNT;
	this -> additional_groups = new gid_t[MAX_GROUPS_COUNT];
	
	this -> additional_groups_count  = getgrouplist(user -> pw_name, this -> gid, this -> additional_groups, &(this -> additional_groups_count));
	
	this -> status = OK;
}



void User::represent()
{
	if (this -> status == OK)
	{
		printf("uid=%d(%s) ", this -> uid, getpwuid(this -> uid) -> pw_name);
		printf("gid=%d(%s)", this -> gid, getgrgid(this -> gid) -> gr_name);
		
		if (additional_groups_count != 0)
		{
			printf(" groups=");
			
			for (int counter = 0; counter < additional_groups_count; counter++)
				printf("%d(%s)%c", (this -> additional_groups)[counter], 
								    getgrgid((this -> additional_groups)[counter]) -> gr_name, 
									counter < additional_groups_count - 1 ? ',' : '\0');
		}
		
		printf("\n");
	}
}



User::~User()
{
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
