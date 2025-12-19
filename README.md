# Budget Manager

A desktop application for managing personal budget with income and expense tracking.

## Prerequisites

Before running the project, you need to have the following installed on your Windows system:

1. MinGW-w64 GCC Compiler
   - Must be installed at: C:\mingw64\
   - Download from: https://www.mingw-w64.org/

2. Git for Windows (optional, only if cloning from repository)

## Project Structure

- src/ - Source code files
- include/ - Header files (Clay UI and Raylib)
- lib/ - Library files
- bin/ - Compiled executable and DLL files
- fuente/ - Font files
- budget_data.csv - Your budget data storage file

## How to Build

1. Open PowerShell or Command Prompt

2. Navigate to the project directory:
   ```
   cd path\to\Ana_Transcriptor
   ```

3. Run the build script:
   ```
   .\build_simple.bat
   ```

The build script will:
- Compile the source code
- Link all libraries
- Copy necessary DLL files to the bin folder
- Automatically run the application

## How to Run

After building, you can run the application directly:

```
.\bin\BudgetManager.exe
```

Or simply run the build script again:

```
.\build_simple.bat
```

## Features

- Add income transactions
- Add expense transactions
- View total income, total expenses, and net balance
- Save data to CSV file
- Load data from CSV file
- Export report to text file
- Interactive graphical user interface

## Usage

1. Add Income:
   - Enter amount in the Amount field
   - Enter description
   - Click the blue "+ Add Income" button

2. Add Expense:
   - Enter amount in the Amount field
   - Enter description
   - Click the red "- Add Expense" button

3. Save Data:
   - Click "Save to CSV" button to save your transactions

4. Load Data:
   - Click "Load from CSV" button to load saved transactions

5. View Transactions:
   - All transactions are displayed in the list with type, amount, description, and date

## Data Storage

All budget data is stored in the budget_data.csv file in the project root directory. This file is automatically created when you save your data.

## Troubleshooting

1. If the application does not start:
   - Make sure MinGW-w64 is installed at C:\mingw64\
   - Check that all DLL files are present in the bin folder

2. If you get compilation errors:
   - Verify that gcc.exe is in your PATH or at C:\mingw64\bin\gcc.exe
   - Make sure all required files are present in the project

3. If the window appears blank:
   - Check that the fuente folder contains the Roboto-Regular.ttf font file

## System Requirements

- Windows 7 or later
- 4GB RAM minimum
- 50MB free disk space
- Display resolution: 1280x720 or higher

## Credits

Built with:
- Raylib - Graphics library
- Clay UI - Layout engine
- MinGW-w64 GCC - Compiler

