#define BUFFER_SIZE 1024

typedef struct _user
{
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    int password_error;
} User;

void app(int);