enum HEADER
{
    ERROR,
    OK,
    LOGIN,
    TEXT
};

typedef struct _message
{
    int header;
    char username[512];
    char text[1024];
} Message;