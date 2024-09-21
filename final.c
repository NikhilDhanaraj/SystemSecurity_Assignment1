#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define MAX_USERS 10
#define MAX_LENGTH 21
#define MAX_DOB_LENGTH 11

struct UserStruct
{
    char name[MAX_LENGTH];
    char dob[MAX_DOB_LENGTH];
    char password[MAX_LENGTH];
};

typedef struct UserStruct User;

struct PasswordStruct
{
    char password[MAX_LENGTH];
};

typedef struct PasswordStruct Passwords;

char *strcasestr(const char *haystack, const char *needle) {
    if (!*needle) {
        return (char *)haystack;
    }

    for (; *haystack; haystack++) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
            h++;
            n++;
        }

        if (!*n) {
            return (char *)haystack;
        }
    }

    return NULL;
}

// void backoff(int n)
// {
//     // printf("Wait for %d seconds....\n", n);
//     // sleep(n);
//     for(int i = n;i>=1;i--){
//         if(i==1){
//             printf("Wait for %d second....\n", i);
//             sleep(1);
//             continue;
//         }
//         printf("Wait for %d seconds....\n", i);
//         sleep(1);
//     }
//     return;
// }

void backoff(int n)
{
    for (int i = n; i >= 1; i--)
    {
        printf("\rWait for %d second%s....", i, (i == 1) ? "" : "s");
        fflush(stdout);

        sleep(1);
    }
    printf("\n");
}

int passwordValidate(char *newpass, int attempt, char *username, char *userdate, Passwords *password_list, int line_count)
{
    int flag = 1;
    int r1 = 0;
    int r2 = 0;
    int r3 = 0;
    int r4 = 0;
    int r5 = 0;
    int r6 = 1;
    int r7 = 0;
    int r8 = 1;

    int length = strlen(newpass);

    // checking r1
    for (int i = 0; i < length; i++)
    {
        if (length >= 12)
        {
            r1 = 1;
        }
    }

    // checking r2
    for (int i = 0; i < length; i++)
    {
        if (isupper(newpass[i]))
        {
            r2 = 1;
        }
    }

    // checking r3
    for (int i = 0; i < length; i++)
    {
        if (islower(newpass[i]))
        {
            r3 = 1;
        }
    }

    // checking r4
    for (int i = 0; i < length; i++)
    {
        if (isdigit(newpass[i]))
        {
            r4 = 1;
        }
    }

    // checking r5
    for (int i = 0; i < length; i++)
    {
        if ((newpass[i] == 33) || (newpass[i] >= 35 && newpass[i] <= 39) || (newpass[i] == 42) || (newpass[i] == 45) || (newpass[i] == 46) || (newpass[i] == 64) || (newpass[i] == 95))
        {
            r5 = 1;
        }
    }

    //check r6
    int r6flag = 0;
    int c = 0;

    for (int k = 0; k < line_count; k++)
    {
        if (r6flag == 1)
        {
            break;
        }

        char temppass[MAX_LENGTH];
        strcpy(temppass, password_list[k].password);
        int templen = strlen(temppass);

        int dp_r6[length + 1][templen + 1];
        for (int i = 0; i <= length; i++)
        {
            for (int j = 0; j <= templen; j++)
            {
                dp_r6[i][j] = 0;
            }
        }

        for (int i = 1; i <= length; i++)
        {
            for (int j = 1; j <= templen; j++)
            {
                if (newpass[i - 1] == temppass[j - 1])
                {
                    dp_r6[i][j] = dp_r6[i - 1][j - 1] + 1;

                    if (dp_r6[i][j] >= 4)
                    {
                        int current_common_length = dp_r6[i][j];
                        int ni = i;
                        int nj = j;

                        while (newpass[ni] == temppass[nj] && newpass[ni] != '\0' && temppass[nj] != '\0')
                        {
                            ni++;
                            nj++;
                            current_common_length++;
                        }

                        if (current_common_length > 4)
                        {
                            c = current_common_length;
                            r6 = 0;
                            r6flag = 1;
                            break;
                        }
                    }
                }
            }
            if (r6flag == 1)
            {
                break;
            }
        }
    }

    // checking r7
    char r7out[40];
    char firstname[MAX_LENGTH];
    int fnlen = 0;
    char lastname[MAX_LENGTH];
    int lnlen = 0;
    int unlength = strlen(username);
    int unflag = 0;
    for (int i = 0; i < unlength; i++)
    {
        if (username[i] == '.')
        {
            unflag = 1;
            i++;
        }
        if (unflag == 0)
        {
            firstname[fnlen++] = username[i];
        }
        else
        {
            lastname[lnlen++] = username[i];
        }
    }
    firstname[fnlen++] = '\0';
    lastname[lnlen++] = '\0';

    // printf("%s\n",firstname);
    // printf("%s\n",lastname);

    if (strcasestr(newpass, firstname) && strcasestr(newpass, lastname))
    {
        strcpy(r7out, "name and surname portions");
    }
    else if (strcasestr(newpass, firstname))
    {
        strcpy(r7out, "name portion");
    }
    else if (strcasestr(newpass, lastname))
    {
        strcpy(r7out, "surname portion");
    }
    else
    {
        r7 = 1;
    }

    // checking r8
    int d = 0;
    char dob[9];
    int dblen = 0;

    for (int i = 0; i <= 10; i++)
    {
        if (userdate[i] != '-')
        {
            dob[dblen++] = userdate[i];
        }
    }
    dob[dblen] = '\0';

    int dp[length + 1][dblen + 1];

    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j <= dblen; j++)
        {
            dp[i][j] = 0;
        }
    }

    for (int i = 1; i <= length; i++)
    {
        for (int j = 1; j <= dblen; j++)
        {
            if (newpass[i - 1] == dob[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;

                if (dp[i][j] > 3)
                {
                    d = dp[i][j];
                    r8 = 0;
                    break;
                }
            }
        }
        if (r8 == 0)
        {
            break;
        }
    }

    if (r1 && r2 && r3 && r4 && r5 && r6 && r7 && r8)
    {
        flag = 1;
    }
    else
    {
        flag = 0;
        switch (attempt)
        {
        case 1:
            printf("First attempt failed.\n");
            break;
        case 2:
            printf("Second attempt failed.\n");
            break;
        case 3:
            printf("Third attempt failed.\n");
            break;
        }
        if (r1 == 0)
        {
            printf("Password does not contain a minimum of 12 characters.\n");
        }
        if (r2 == 0)
        {
            printf("Password does not contain at least one uppercase letter.\n");
        }
        if (r3 == 0)
        {
            printf("Password does not contain at least one lowercase letter.\n");
        }
        if (r4 == 0)
        {
            printf("Password does not contain at least one digit.\n");
        }
        if (r5 == 0)
        {
            printf("Password does not contain at least one of the allowed special characters.\n");
        }
        if (r6 == 0)
        {
            printf("Password contains %d characters consecutively similar to one of the past %d passwords.\n", c, line_count);
        }
        if (r7 == 0)
        {
            printf("Password contains %s of username.\n", r7out);
        }
        if (r8 == 0)
        {
            printf("Password contains %d digits consecutively similar to the date of birth.\n", d);
        }
    }

    return flag;
}

void changePassword(char *passwordfile, char *username, char *userdate)
{
    FILE *file;
    file = fopen(passwordfile, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    int line_count = 0;
    Passwords password_list[10];
    while (fscanf(file, "%s", password_list[line_count].password) == 1)
    {
        line_count++;
        if (line_count >= 10)
        {
            break;
        }
    }
    fclose(file);

    int attempts = 0;
    char newpass[MAX_LENGTH];
    while (1)
    {
        if (attempts == 0)
        {
            printf("Enter your new password (1st attempt):");
            scanf("%s", newpass);
            int flag = passwordValidate(newpass, 1, username, userdate, password_list, line_count);
            if (flag == 0)
            {
                attempts++;
                backoff(8);
                continue;
            }
            else
            {
                break;
            }
        }
        else if (attempts == 1)
        {
            printf("Enter your new password (2nd attempt):");
            scanf("%s", newpass);
            int flag = passwordValidate(newpass, 2, username, userdate, password_list, line_count);
            if (flag == 0)
            {
                attempts++;
                backoff(16);
                continue;
            }
            else
            {
                break;
            }
        }
        else if (attempts == 2)
        {
            printf("Enter your new password (3rd attempt):");
            scanf("%s", newpass);
            int flag = passwordValidate(newpass, 3, username, userdate, password_list, line_count);
            if (flag == 0)
            {
                attempts++;
                backoff(32);
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            printf("Enter your new password (4th attempt):");
            scanf("%s", newpass);
            int flag = passwordValidate(newpass, 4, username, userdate, password_list, line_count);
            if (flag == 0)
            {
                printf("All 4 attempts failed. You need to try again later.\n");
                return;
            }
            else
            {
                break;
            }
        }
    }
    Passwords newpassword_list[10];
    for (int i = 0; i < 10; i++)
    {
        newpassword_list[i].password[0] = '\0';
    }
    for (int i = 0; i < 9 && i < line_count; i++)
    {
        strcpy(newpassword_list[i + 1].password, password_list[i].password);
    }
    strcpy(newpassword_list[0].password, newpass);
    file = fopen(passwordfile, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < 10; i++)
    {
        if (strlen(newpassword_list[i].password) > 0)
        {
            fprintf(file, "%s\n", newpassword_list[i].password);
        }
    }
    fclose(file);
    printf("Password changed successfully.\n");
    return;
}

int main()
{
    User userdata[MAX_USERS];
    FILE *file;
    if ((file = fopen("masterfile.txt", "r")) == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    int line_count = 0;
    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    while (fscanf(file, "%s %s %s", userdata[line_count].name, userdata[line_count].dob, userdata[line_count].password) == 3)
    {
        line_count++;
        if (line_count >= MAX_USERS)
        {
            break;
        }
    }
    fclose(file);

    // for(int i = 0;i<line_count;i++){
    //     printf("%s %s %s\n", userdata[i].name, userdata[i].dob, userdata[i].password);
    // }

    char username[MAX_LENGTH];
    char passtext[MAX_LENGTH];
    char password[MAX_LENGTH];
    char passkey[MAX_LENGTH];
    char userdate[MAX_DOB_LENGTH];
    int userExists = 0;

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < line_count; i++)
    {
        if (strcmp(userdata[i].name, username) == 0)
        {
            userExists = 1;
            strcpy(passtext, userdata[i].password);
            strcpy(userdate, userdata[i].dob);
        }
    }
    if (userExists == 0)
    {
        printf("User Not Found Try Again\n");
        return 0;
    }

    file = NULL;
    file = fopen(passtext, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    fscanf(file, "%s", passkey);
    fclose(file);

    int count = 0;
    while (strcmp(passkey, password) != 0)
    {
        count++;
        if (count >= 3)
        {
            printf("Wrong password entered 3 times. Application exiting....\n");
            return 0;
        }
        printf("Wrong password! Enter password again:");
        scanf("%s", password);
    }

    changePassword(passtext, username, userdate);

    return 0;
}