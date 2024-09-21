# Password Validation Program

This program is designed to validate passwords for **previously registered users** based on a set of predefined conditions. The program does not accept new users, and it only works for users who have already been registered with a password history file.

## Program Requirements

For this program to function, it requires two files:
1. **masterfile.txt** – This file contains information about registered users.
2. **password.txt** – Corresponding password files for each user, storing their last passwords.

---

## masterfile.txt Specifications

The `masterfile.txt` file stores the following information for registered users:
- **username**: Consists of the format `name.surname`, all in lowercase. The username must not contain spaces or special characters, except for the dot (`.`) separating the name and surname.
- **date_of_birth**: Stored in the format `dd-mm-yyyy`. The separator between day, month, and year is a hyphen (`-`).
- **password_file_name**: Name of the file where the user's passwords are stored. The file name is derived from the first letter of the user’s name, the first letter of their surname, followed by the string `pass.txt`.

### Format Example:
Each line in the `masterfile.txt` file follows this structure:

Example:  
    ramesh.yadav 19-09-1985 rypass.txt  
    puja.bedi 23-08-1991 pbpass.txt  
    arun.kumar 01-11-1973 akpass.txt  
    anjali.sharma 30-01-1996 aspass.txt  

---

## Password File Specifications

Each registered user has a corresponding password file, which stores the last **10 passwords** created by the user. If a user has changed their password fewer than 10 times, the file will contain fewer passwords. The most recent password is the first entry in the file, while the oldest (or least recent) password is the last.

- **File Naming Convention**: The name of the password file is created by taking:
  - The first letter from the user’s first name.
  - The first letter from the user’s surname.
  - The string `pass.txt` appended to the two letters.

For example, for a user named **Ramesh Yadav**, the password file will be named `rypass.txt`.

### Password History:
Passwords are stored in descending order of their creation date:
- The most recent password appears as the first line in the file.
- If a user changes their password more than 9 times, the oldest passwords are removed from the file.

### Password File Format:
The file will look like this:

Example:  2px34sa19h.fS  lkA@!o90a$5p  m.M90a21gth*k  xCXtimPOT23!p  Abcd.1234.*S  9091@asdfOOP$



---

## Password Validation Rules

The program will validate whether the new password meets the following conditions:

### R1: Minimum Length
- The password must be at least **12 characters** long. 
- The maximum permissible length is **20 characters**.

### R2: Uppercase Letters
- The password must contain at least **one uppercase letter**.

### R3: Lowercase Letters
- The password must contain at least **one lowercase letter**.

### R4: Digits
- The password must contain at least **one digit** (0-9).

### R5: Special Characters
- The password must contain at least **one special character** from the set: {., @, !, #, $, %, ^, &, *, -, _}

- Any other special characters will result in the password being rejected.

### R6: Similarity to Previous Passwords
- The password must not have more than **4 consecutive characters** that are the same as any of the last 10 passwords stored in the user’s password file.
- This check is **case insensitive**.

### R7: Containing Name or Surname
- The password must not contain the user's **name** or **surname** (from their username).
- This check is **case insensitive**.

### R8: Containing Date of Birth Digits
- The password must not contain more than **3 consecutive digits** from the user's date of birth.
- For example, if the date of birth is `17-11-2001`, passwords containing `1711`, `2001`, or `1120` are not valid, but `1171`, `0211`, or `1701` are allowed.

---
