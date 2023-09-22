#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

#define MAX_DESCRIPTION_LENGHT 80
#define MAX_PASSWORD_LENGHT 20

typedef struct{
    char type[MAX_DESCRIPTION_LENGHT]; //* "Deposit" or "Withdrawal"
    float amount;
    char date[MAX_DESCRIPTION_LENGHT];
} Transaction;

typedef struct{
    char name[MAX_DESCRIPTION_LENGHT];
    char address[MAX_DESCRIPTION_LENGHT];
    char password[MAX_PASSWORD_LENGHT];
    int account_number;
    float money_in_bank;
    Transaction *transaction_history;
    int transaction_count;
} User;

int create_account(User *users, int user_count, int *account_number);
void get_password(char *password, size_t max_length);
User* log_in(User *users, int user_count);
void perform_action(User *user);
void deposit(User *user, float amount);
void withdraw(User *user, float amount);
void check_balance(User *user);
void check_transaction_history(User *user);
int main(){
    User users[MAX_DESCRIPTION_LENGHT];
    User* logged_in;
    int account_number = 4213;
    int *pAccount_number = &account_number;
    int user_count = 0;
    printf("Welcome to the PirkunBank!\n");
    while(1){
        int option;

        printf("\nPlease choose an option:\n");
        printf("1. Create new account\n");
        printf("2. Log in to existing account\n");
        printf("3. Exit\n");

        printf("\nOption: ");
        scanf(" %d", &option);
        while ((getchar()) != '\n');

        if (option == 1){
            user_count = create_account(users, user_count, pAccount_number);
        }else if (option == 2){
            logged_in = log_in(users, user_count);
            if(logged_in != NULL){
                perform_action(logged_in);
            }
        }
    }
}

int create_account(User *users, int user_count, int *account_number){
    int p = 0;
    char ch;
    float amount;
    users[user_count].transaction_count = 0;

    printf("Please enter your name: ");
    fgets(users[user_count].name, MAX_DESCRIPTION_LENGHT, stdin);
    size_t length = strlen(users[user_count].name);
    if (users[user_count].name[length - 1] == '\n') {
        users[user_count].name[length - 1] = '\0'; // replace the newline character with a null terminator
    }
            
    printf("Please enter your address: ");
    fgets(users[user_count].address, MAX_DESCRIPTION_LENGHT, stdin);
    length = strlen(users[user_count].address);
    if (users[user_count].address[length - 1] == '\n') {
        users[user_count].address[length - 1] = '\0'; // replace the newline character with a null terminator
    }

    printf("Please enter your initial deposit amount: ");
    scanf(" %f", &amount);
    users[user_count].money_in_bank = amount;

    Transaction *new_transaction = realloc(users[user_count].transaction_history, sizeof(Transaction) * (users[user_count].transaction_count + 1));
    if (new_transaction == NULL) {
        printf("Error: Memory allocation failed.\n");
    }
    users[user_count].transaction_history = new_transaction;

    Transaction transaction;
    strcpy(transaction.type, "Deposit");
    transaction.amount = amount;
    time_t current_time = time(NULL);
    strftime(transaction.date, MAX_DESCRIPTION_LENGHT, "%Y-%m-%d %H:%M:%S", localtime(&current_time));
    users[user_count].transaction_history[users[user_count].transaction_count] = transaction;
    users[user_count].transaction_count++;

    printf("Please choose a password: ");
    get_password(users[user_count].password, MAX_DESCRIPTION_LENGHT);

    users[user_count].account_number = *account_number;
    printf("\nYour account has been created your account number is %d\n\n", users[user_count].account_number);
    *(account_number) += 1;
    user_count++;

    return user_count;
}

void get_password(char *password, size_t max_length) {
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r') {
        if (ch == '\b' && i > 0) {  // handle backspace character
            i--;
            printf("\b \b");
        } else if (ch != '\b' && i < max_length - 1) {  // ignore other control characters
            printf("*");
            password[i] = ch;
            i++;
        }
    }
    password[i] = '\0';
    printf("\n");
}

User* log_in(User *users, int user_count){
    int account_number;
    char password[MAX_PASSWORD_LENGHT];

    printf("Please enter your account number: ");
    scanf("%d", &account_number);

    printf("Please enter your password: ");
    get_password(password, MAX_PASSWORD_LENGHT);

    for(int i = 0; i < user_count; i++){
        if(users[i].account_number == account_number && strcmp(users[i].password, password) == 0){
            printf("\nWelcome, %s!\n", users[i].name);
            return &users[i];
        }
    }
    printf("Your password or account number is wrong.\n");
    return NULL;
}

void perform_action(User *user){
    int action;
    float amount;
    do {
        printf("\nPlease choose an action:\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Check balance\n");
        printf("4. Check transaction history\n");
        printf("5. Exit\n");

        printf("\nOption: ");
        scanf("%d", &action);

        if(action == 1){
            printf("Enter the amount to deposit: ");
            scanf("%f", &amount);
            deposit(user, amount);
        }else if(action == 2){
            printf("Enter the amount to withdraw: ");
            scanf("%f", &amount);
            withdraw(user, amount);
        }else if(action == 3){
            check_balance(user);
        }else if(action == 4){
            check_transaction_history(user);
        }
    }while (action != 5);

}

void deposit(User *user, float amount){
    if (amount < 0) {
        printf("Error: Cannot deposit a negative amount.\n");
        return;
    }

    user->money_in_bank += amount;

    Transaction *new_transaction = realloc(user->transaction_history, sizeof(Transaction) * (user->transaction_count + 1));
    if (new_transaction == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    user->transaction_history = new_transaction;

    Transaction transaction;
    strcpy(transaction.type, "Deposit");
    transaction.amount = amount;
    time_t current_time = time(NULL);
    strftime(transaction.date, MAX_DESCRIPTION_LENGHT, "%Y-%m-%d %H:%M:%S", localtime(&current_time));
    user->transaction_history[user->transaction_count] = transaction;
    user->transaction_count++;

    printf("Successfully deposited %.2f dollars into your account.\n", amount);
}

void withdraw(User *user, float amount){
    if (amount < 0) {
        printf("Error: Cannot withdraw a negative amount.\n");
        return;
    } else if (amount > user->money_in_bank) {
        printf("Error: Insufficient funds.\n");
        return;
    }

    user->money_in_bank -= amount;

    Transaction *new_transaction = realloc(user->transaction_history, sizeof(Transaction) * (user->transaction_count + 1));
    if (new_transaction == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    user->transaction_history = new_transaction;

    Transaction transaction;
    strcpy(transaction.type, "Withdrawal");
    transaction.amount = amount;
    time_t current_time = time(NULL);
    strftime(transaction.date, MAX_DESCRIPTION_LENGHT, "%Y-%m-%d %H:%M:%S", localtime(&current_time));
    user->transaction_history[user->transaction_count] = transaction;
    user->transaction_count++;

    printf("Successfully withdrew %.2f dollars from your account.\n", amount);
}

void check_balance(User *user){
    printf("You have %.2f dollars in your bank account.\n", user->money_in_bank);
}

void check_transaction_history(User *user) {
    if (user->transaction_count == 0) {
        printf("No transactions found.\n");
        return;
    }

    printf("Transaction history for account number %d:\n", user->account_number);
    printf("%-15s%-10s%-20s%-10s\n", "Type", "Amount", "Date", "Balance");
    float balance = 0.0;
    for (int i = 0; i < user->transaction_count; i++) {
        Transaction transaction = user->transaction_history[i];
        if(strcmp(transaction.type, "Withdrawal") == 0){
            balance -= transaction.amount;
        }else{
            balance += transaction.amount;
        }
        printf("%-15s%-10.2f%-20s%-10.2f\n", transaction.type, transaction.amount, transaction.date, balance);
    }
}
