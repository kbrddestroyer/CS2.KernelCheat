# Contributing [En]
## Introduction 

You can join my [Dev:: Playground](https://discord.com/invite/KCYk7z7msN) Discord server. 

---

# Contribution Guideline

Thank you for your interest in contributing to our project! We appreciate your help and strive to make the process as easy and clear as possible. Please follow these guidelines to ensure your changes are accepted without delays.

## Branches and Pull Requests

Our repository has two main branches: `main` and `release`.

- **Branch Protection**: Both `main` and `release` branches are protected against direct changes. You must create a pull request to make any changes.
- **Pull Request Restrictions**: Creating a pull request directly into the `release` branch is strictly prohibited, except for pull requests coming from the `main` branch. Pull requests into the `main` branch are allowed and encouraged.

## Branch Naming Convention

To maintain consistency and clarity in our repository, please adhere to the following branch naming convention:

```
wip/<section-name>/nickname/<message>
```

Where `<section-name>` refers to the subproject being changed. We have three subprojects: `kernelmode`, `usermode`, and `docs`.

### Example:

```
wip/kernelmode/KeyboardDestroyer/fix-memory-leak
```

## Commit Messages

Commit messages should be clear and descriptive. Each commit message must include the section name in uppercase as the first word.

### Example:

```
USERMODE Fixed memory leak
```

## Code Style

Our project is written in C++. Please follow these coding standards:

- **Smart Pointers**: Use smart pointers instead of raw pointers.
- **Avoid using `namespace`**: Do not use `using namespace`; be explicit with your namespaces.
- **Variable Naming**: Follow Hungarian notation. Prefix the variable name with its data type (e.g., `uint32_t uVariable`).
- **Descriptive Variable Names**: Variable names must clearly reflect the data they hold.
- **Preferred Data Types**: Avoid using `int` and `unsigned int`. Instead, use fixed-width integer types like `int8_t`, `int32_t`, `uint32_t`, etc.

### Example:

```cpp
int32_t iCount;  // Good
int count;      // Bad

uint8_t uFlag;  // Good
unsigned int flag; // Bad
```

## Getting Started

1. **Fork the repository** on GitHub.
2. **Create a branch** for your work:
   ```sh
   git checkout -b wip/<section-name>/nickname/<message>
   ```
3. **Make your changes** to the code. Ensure your code adheres to our style.
4. **Commit** your changes with a descriptive message:
   ```sh
   git commit -m "SECTION_NAME Description of your changes"
   ```
5. **Push your changes** to your fork:
   ```sh
   git push origin wip/<section-name>/nickname/<message>
   ```
6. **Create a Pull Request** via GitHub.

## Testing

Before submitting a pull request, ensure that your code builds successfully using MSBuild.

To build the project, use the following command:

```sh
msbuild /t:Build /p:Configuration=Release
```

> Opening a pull request to main will also trigger an MSBuild action. 

## Pull Request

When you're ready to create a Pull Request:

1. Ensure your code is up to date with the main repository:
   ```sh
   git checkout main
   git pull origin main
   git checkout wip/<section-name>/nickname/<message>
   git rebase main
   ```
2. Create a Pull Request on GitHub.
3. In the Pull Request description, provide a detailed explanation of your changes and why they are necessary.

## Feedback

We strive to respond promptly to all Pull Requests. Sometimes we may ask you to make additional changes or clarify details. Please feel free to ask questions if anything is unclear.

## Code of Conduct

Please respect all project participants and follow our [Code of Conduct](CODE_OF_CONDUCT.md).

Thank you for your contribution!

---

KbrdDestroyer 09.06.24