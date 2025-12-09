# SmartCalculator

SmartCalculator is a small desktop calculator written in C++ using Qt and CMake. It supports parsing and evaluating mathematical expressions via a custom parser and generates an AST for expressions. The project is configured with CMake and is commonly developed with CLion on Windows.

## Features
- Evaluate arithmetic expressions and parentheses
- Custom parser and AST representation (see `AST.h`, `Parser.h`)
- Qt-based GUI (`MainWindow.h`, `main.cpp`)
- CMake build system for cross-platform builds

## Prerequisites
- Windows 10/11
- CLion (recommended) or CMake + a C++ compiler (MSVC, MinGW)
- Qt 5 or 6 (installed and available to CMake/CLion)
- Git (for source control)
- Optional: GitHub CLI (`gh`) for repository tasks

## Quick build & run (CLion)
1. Open CLion and select "Open" then choose the project folder (where `CMakeLists.txt` is).
2. Configure the CMake profile to point to your Qt kit if required.
3. Build the project (Build → Build Project).
4. Run the target from CLion (Run → Run 'SmartCalculator').

## Quick build & run (command-line, PowerShell)
1. Create an out-of-source build directory:
   mkdir build; cd build
2. Configure and generate the build system (example using Ninja):
   cmake -S .. -B . -G "Ninja" -DCMAKE_BUILD_TYPE=Release
3. Build:
   cmake --build . --config Release
4. Run:
   .\SmartCalculator.exe

## Git / GitHub — push this project to your remote
If your GitHub repository is empty (you created https://github.com/Seif-Sallam-1/SmartCalculator), here are the commands to push your local project to that remote. Run these in PowerShell from the project root (where this `README.md` lives).

1) Initialize (if you haven't already) and commit:
   git init
   git add --all
   git commit -m "Initial commit — add project files"

2) Make sure you are using `main` as the primary branch:
   git branch -M main

3) Add the remote (HTTPS):
   git remote add origin https://github.com/Seif-Sallam-1/SmartCalculator.git

   Or add via SSH (if you set up SSH keys on GitHub):
   git remote add origin git@github.com:Seif-Sallam-1/SmartCalculator.git

4) Push and set upstream:
   git push -u origin main

Authentication tips:
- Recommended: run `gh auth login` and follow the interactive flow to authenticate.
- Or create a Personal Access Token (PAT) on GitHub and use it when prompted for your password for HTTPS pushes.

If you prefer, you can create the remote and push in one command using GitHub CLI:
   gh repo create Seif-Sallam-1/SmartCalculator --public --source=. --remote=origin --push

## Repository metadata
After pushing, you can set the repo description and topics (tags) with the GitHub CLI:
   gh repo edit Seif-Sallam-1/SmartCalculator --description "SmartCalculator — Qt/C++ desktop calculator with expression parsing and AST." --add-topic qt --add-topic calculator --add-topic c++ --add-topic cmake

Or edit the "About" section on the repository page in your browser.

## Contributing
Contributions are welcome. Suggested workflow:
1. Fork the repository or create a branch: `git checkout -b feature/your-feature`
2. Make changes, add tests where applicable.
3. Commit and push your branch, then open a pull request.

## License
Add a `LICENSE` file to the project root to declare the project's license (for example MIT).

## Contact
For questions, open an issue on GitHub or contact the maintainer via your GitHub profile.
