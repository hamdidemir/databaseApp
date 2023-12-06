# DatabaseApp

**Description:** This is a database application that aims to mimic the functionality of a real database application.

## Project Structure

- **Databases:** A folder to store database files created by the app.
- **Dependencies:** A folder for the dependencies used in the project (cJSON, Unity).
- **Include:** A folder to store header files.
- **Obj:** A folder to store the .o files created by the compiler.
- **Src:** Source code
  - **DatabaseConnection:**
    - `connect.c:` Establishes connection by initializing two global variables: one for the database name and the other for the root of the data structure. This allows the app to have only one database connection at a time.
    - `disconnect.c:` Disconnects the app, allowing it to be connected to other databases by releasing the global variables.
    - `createDatabase.c:` Creates a database file.
    - `deleteDatabase.c:` Deletes the selected database.
    - `processDatabaseFile.c:` Processes the database file by loading its content into memory through the proper data structure (B+ tree).
  - **DataStructure:**
    - `arrayList.c:` A custom-made dynamic array to handle data of unknown size.
    - `BPlusTree.c:` A simple B+ tree implementation to achieve logarithmic time complexity for searching.
  - **Helpers:**
    - `CheckTable.c:` Checks the existence of a given table and retrieves information about the table.
    - `compare.c:` Contains different comparison functions; compares and returns the min/max value, and compares values of unknown types.
    - `fileExist.c:` Checks the existence of a given database file.
    - `isDataEntry.c:` Checks whether a given string is a data entry or a table entry.
    - `printTable.c:` Prints the given data as a table.
    - `trim.c:` Trims white spaces from the start and end of a given string.
  - **LoadFromFile:**
    - **LoadCSV:** A function to load a large amount of data from a CSV file to the proper table. Checks if the table has the same format as the given CSV file.
  - **ParseSQL:**
    - `parseMainQuery.c:` Parses the main SQL query from the user and directs it to other parsers.
    - `parseCreate.c:` Parses and processes queries starting with "create."
    - `parseSelect.c:` Parses and processes queries starting with "select."
    - `parseInsert.c:` Parses and processes queries starting with "insert."
- **Main.c:** Contains the main program as a menu-based app.
- **Test:** Contains tests for the source codes with the same structure as the src folder. Most tests use the Unity Test Framework.
