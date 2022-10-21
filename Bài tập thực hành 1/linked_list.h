typedef struct _Account {
    char username[30];
    char password[30];
    int status;
    int is_signed_in;
    struct _Account *next;
} Account;

Account *create_new_account(char *username, char *password, int status);
Account *add_account(Account *account, char *username, char *password, int status);
int check_user(Account *account, char *username);
int check_password(Account *account, char *password);
int check_blocked(Account *account, char *username);
int check_signed_in(Account *account, char *username);
Account *read_account(Account *account);
Account *register_account(Account *account);
void update_file(Account *account);
void sign_in(Account *account);
void search(Account *account);
void sign_out(Account *account);
void free_list(Account *account);
void activate(Account *account);