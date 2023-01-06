#define BUFFER_SIZE 1024

typedef struct _user
{
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
} User;

void app(int);