#define SMALL 256
#define MEDIUM 512
#define LARGE 1024

#define USERS_DIR "data/user.txt"
#define USER_DIR "data/"

enum HEADER
{
    ERROR,
    OK,
    LOGIN,
    TEXT,
    EXIT
};

typedef struct _message
{
    int header;
    char login_name[MEDIUM];
    char text[LARGE];
} Message;