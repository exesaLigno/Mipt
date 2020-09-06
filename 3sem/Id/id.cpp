#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <cstring>



const int MAX_GROUPS_COUNT = 20;



struct User
{
	bool exist = false;
	
	uid_t uid = -1;
	gid_t gid = -1;
	char* username = nullptr;
	char* groupname = nullptr;
	
	int groups_count = 0;
	gid_t* groups = nullptr;
};


User* getUsers(int users_count, char* user_names[]);
void getUsersGroups(User* users, int users_count);
int giveResponce(User* users, int users_count);



int main(int argc, char* argv[])
{
	int users_count = argc - 1;		// Assuming that all parameters is usernames
	
	User* users = getUsers(users_count, argv + 1);
	
	if (argc == 1)
		users_count = 1;
	
	getUsersGroups(users, users_count);

	int status = giveResponce(users, users_count);
	
	delete[] users;
	
	return status;
}


User* getUsers(int users_count, char* user_names[])
{
	User* users = nullptr;
	
	if (users_count == 0)
	{
		users = new User[1];
		users[0].exist = true;
		users[0].uid = getuid();
		users[0].gid = getgid();
		
		passwd* user = getpwuid(users[0].uid);
		group* group = getgrgid(users[0].gid);
		
		users[0].username = new char[strlen(user -> pw_name + 1)]{0};
		strcpy(users[0].username, user -> pw_name);
		
		users[0].groupname = new char[strlen(group -> gr_name + 1)]{0};
		strcpy(users[0].groupname, group -> gr_name);
	}
	
	else
	{
		users = new User[users_count];
		
		for (int counter = 0; counter < users_count; counter++)
		{
			passwd* user = getpwnam(user_names[counter]);								// Getting passwd object with uid and gid
			
			users[counter].username = new char[strlen(user_names[counter] + 1)]{0};
			strcpy(users[counter].username, user_names[counter]);
			
			if (user != nullptr)
			{
				users[counter].exist = true;
				users[counter].uid = user -> pw_uid;
				users[counter].gid = user -> pw_gid;
				
				group* group = getgrgid(users[counter].gid);							// Getting group name from gid
				
				users[counter].groupname = new char[strlen(group -> gr_name + 1)]{0};
				strcpy(users[counter].groupname, group -> gr_name);
			}
			
			else
				users[counter].exist = false;
		}
	}
	
	return users;
}


void getUsersGroups(User* users, int users_count)
{
	for (int counter = 0; counter < users_count; counter++)
	{
		if (users[counter].exist)
		{
			users[counter].groups_count = MAX_GROUPS_COUNT;
			users[counter].groups = new gid_t[MAX_GROUPS_COUNT];
			users[counter].groups_count = getgrouplist(users[counter].username, users[counter].gid, users[counter].groups, &(users[counter].groups_count));
		}
	}
}


int giveResponce(User* users, int users_count)
{
	int status = 0;
	
	for (int counter = 0; counter < users_count; counter++)
	{
		if (users[counter].exist)
		{
			printf("uid=%d(%s) gid=%d(%s)", users[counter].uid, users[counter].username, users[counter].gid, users[counter].groupname);
			
			if (users[counter].groups_count != 0)
				printf(" groups=");
			
			for (int groups_counter = 0; groups_counter < users[counter].groups_count; groups_counter++)
			{
				printf("%d(%s)", users[counter].groups[groups_counter], getgrgid(users[counter].groups[groups_counter]) -> gr_name);
				
				if (groups_counter != users[counter].groups_count - 1)
					printf(",");
			}
			
			printf("\n");
		}
		
		else
		{
			printf("id: «%s»: there is no such user\n", users[counter].username);
			status = 1;
		}
	}
	
	return status;
}





/* 
 * if (user != nullptr)
 *		printf("uid=%d(%s) gid=()\n", user -> pw_uid, user -> pw_name);
 *	
 *	else
 *		printf("id: «%s»: there is no such user\n", argv[1]);
 */
