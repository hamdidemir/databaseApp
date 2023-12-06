# databaseApp

This is a crude database application which aims to mimic the functionality of a real database application.

Project Sturucture:
> Databases : a folder to keep database files created by the app.
> dependencies : a folder for the denpendencies used in the project. (There are two: cJSON, Unity)
> include : a folder to keep the header files
> obj : a folder to keep the .o file created by compiler
> src : Source code
> > databaseConnection : handles connection/diconnection functionlity of the app.
> > > connect.c : Establish connection by initilaizing two global variables one for name of the database other is the root for the data structure. This enables app to have a only one database connection once.
> > > diconnect.c : Disconnect the app so that it can be connected to other databases by relasing the global variables.
> > > createDatabase.c : Creates a database file.
> > > deleteDatabase.c : Deletes the selected database.
> > > processDatabaseFile.c : Process the database file by loading the content of the database to the memory though the proper data structure (which is B+ tree).
> > dataStructure:
> > > arrayList.c : A custom made dynamic array to handle the data of unknown size.
> > > BPlusTree.c : A simple b+ tree implementation to achieve the logarithm time complexity for searching.
> > helpers:
> > > CheckTable.c : Checks the existing of a given table and retrieves information of the given table.
> > > 
