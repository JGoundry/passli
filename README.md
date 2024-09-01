# Passli - Command Line Password Manager

Welcome to **Passli**, a simple and secure command line password manager that uses GPG for encryption. Passli stores your passwords in a vault within the `.passli` directory in your home directory. The passwords are symmetrically encrypted to ensure your sensitive data remains safe.

## Features

- **Secure Storage:** Passwords are encrypted using GPG for robust security.
- **Simple CLI Interface:** Easy to use command line interface for managing your passwords.
- **Local Storage:** Passwords are stored in a local vault within the `.passli` directory in your home directory.
- **Cross-Platform:** Works on any system with GPG installed.

## Dependencies

- [GPGME](https://gnupg.org/documentation/manuals/gpgme/)
- [Google Test (gtest)](https://github.com/google/googletest)
- [nlohmann/json](https://github.com/nlohmann/json)
