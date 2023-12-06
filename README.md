# DatabaseApp

**Description:** This database application emulates the functionality of a real database system. It enables users to input data through SQL insert queries or from a well-formatted CSV file, assuming a corresponding table exists in the database. Data is stored in files using JSON, with the application utilizing the cJSON library for effective JSON handling. Presently, the application offers limited functionality. Users can create, delete, connect, and disconnect from databases. Additionally, they can create tables, insert data into these tables, and perform selection operations using a "where" clause and aggregate functions.

## Project Structure

- **Databases:** A directory to store database files created by the application.
- **Dependencies:** A directory for the project's dependencies, including cJSON and Unity.
- **Include:** A directory for storing header files.
- **Obj:** A directory for storing the .o files generated by the compiler.
- **Src:** Source code
  - **DatabaseConnection:**
    - `connect.c:` Establishes a connection by initializing two global variables: one for the database name and another for the root of the data structure. This design ensures that the application maintains only one database connection at a time.
    - `disconnect.c:` Disconnects the application, allowing it to connect to other databases by releasing the global variables.
    - `createDatabase.c:` Creates a new database file.
    - `deleteDatabase.c:` Deletes the selected database.
    - `processDatabaseFile.c:` Processes the database file by loading its content into memory through the appropriate data structure (B+ tree).
  - **DataStructure:**
    - `arrayList.c:` A custom dynamic array designed to handle data of unknown size.
    - `BPlusTree.c:` A simple B+ tree implementation, achieving logarithmic time complexity for searching.
  - **Helpers:**
    - `CheckTable.c:` Verifies the existence of a given table and retrieves information about it.
    - `compare.c:` Houses various comparison functions that compare and return min/max values or compare values of unknown types.
    - `fileExist.c:` Checks for the existence of a given database file.
    - `isDataEntry.c:` Determines whether a given string is a data entry or a table entry.
    - `printTable.c:` Prints the given data as a table.
    - `trim.c:` Removes white spaces from the beginning and end of a given string.
  - **LoadFromFile:**
    - `LoadCSV.c:` A function to load a substantial amount of data from a CSV file into the appropriate table. It verifies if the table has the same format as the provided CSV file.
  - **ParseSQL:**
    - `parseMainQuery.c:` Parses the main SQL query from the user and directs it to other parsers.
    - `parseCreate.c:` Parses and processes queries starting with "create."
    - `parseSelect.c:` Parses and processes queries starting with "select."
    - `parseInsert.c:` Parses and processes queries starting with "insert."
- **Main.c:** Contains the main program as a menu-based application.
- **Test:** Includes tests for the source codes with a structure mirroring the src folder. Most tests utilize the Unity Test Framework.
