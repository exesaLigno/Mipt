#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <cstring>
#include <cstdlib>

#define DEBUG printf("DEBUG from %d\n", __LINE__);
const int MAX_GROUPS_COUNT = 20;


class User
{
private:
	
	int type = 0;
	
	uid_t uid = EMPTY_ID;
	gid_t gid = EMPTY_ID;
	
	char* user_name = nullptr;
	
	int additional_groups_count = 0;
	gid_t* additional_groups = nullptr;
	
	
	void setGroups();
	
	
public:
	
	User();
	User(const char* user_name);
	User(const uid_t uid);
	
	void setInfo();
	void representUser();
	
	~User();
	
	enum CONSTANTS
	{
		CURRENT,
		USER_NAME,
		UID,
		EMPTY_ID = -1
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
		users[counter] -> setInfo();
		users[counter] -> representUser();
		delete users[counter];
	}
	
	delete[] users;
	
	return 0;
}



User::User()
{
	this -> type = CURRENT;
}


User::User(const char* user_name)
{
	this -> type = USER_NAME;
	this -> user_name = new char[strlen(user_name) + 1]{0};
	strcpy(this -> user_name, user_name);
}


User::User(const uid_t uid)
{
	this -> type = UID;
	this -> uid = uid;
}



void User::setInfo()
{
	if (this -> type == CURRENT)
	{
		this -> uid = getuid();
		this -> gid = getgid();
		this -> user_name = getUserName(this -> uid);
	}
	
	else if (this -> type == USER_NAME)
	{
		this -> uid = getUid(this -> user_name);
		this -> gid = getGid(this -> uid);
	}
	
	else if (this -> type == UID)
	{
		this -> user_name = getUserName(this -> uid);
		this -> gid = getGid(this -> uid);
	}
	
	if (this -> uid != (uid_t) EMPTY_ID and this -> user_name != nullptr)
	{
		this -> setGroups();
	}
}



void User::setGroups()
{
	this -> additional_groups_count = MAX_GROUPS_COUNT;
	this -> additional_groups = new gid_t[MAX_GROUPS_COUNT];
	
	if (this -> type == CURRENT)
		this -> additional_groups_count  = getgroups(MAX_GROUPS_COUNT, this -> additional_groups);
	
	else
		this -> additional_groups_count  = getgrouplist(this -> user_name, this -> gid, this -> additional_groups, &(this -> additional_groups_count));
}



void User::representUser()
{
	if (this -> type == USER_NAME and this -> uid == (uid_t) EMPTY_ID)
		printf("id: «%s»: there is no such user\n", this -> user_name);
	
	else if (this -> type == UID and this -> user_name == nullptr)
		printf("id: «%d»: there is no such user\n", this -> uid);
	
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
		name = new char[strlen(user -> pw_name) + 1]{0};
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
		name = new char[strlen(group -> gr_name) + 1]{0};
		strcpy(name, group -> gr_name);
	}
	
	return name;
}


bool isNumber(const char* str)
{
	bool is_number = true;
	
	for (int counter = 0; str[counter] != '\0'; counter++)
	{
		if (str[counter] > '9' or str[counter] < '0')
		{
			is_number = false;
			break;
		}
	}
	
	return is_number;
}
