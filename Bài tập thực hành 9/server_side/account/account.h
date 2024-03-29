#define BUFFER_SIZE 1024

typedef struct _user
{
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    int password_error;
} User;

typedef struct _Account
{
    char username[30];
    char password[30];
    char homepage[100];
    int status;
    int is_signed_in;
    struct _Account *next;
} Account;

Account *create_new_account(char *username, char *password, char *homepage, int status);
Account *add_account(Account *account, char *username, char *password, char *homepage, int status);
int check_user(Account *account, char *username);
int check_password(Account *account, char* username, char *password);
int check_signed_in(Account *account, char *username);
Account *read_account(Account *account);
Account *register_account(Account *account);
void update_file(Account *account);
int account_sign_in(Account *acc, char *username, char *password);
void search(Account *account);
int account_sign_out(Account *account, char *username);
void free_list(Account *account);
int account_change_password(Account *acc, char *username, char *new_password);
void change_current_account_status(Account *acc, char *username, int status);
void current_user_get_info(Account* acc, char* username);