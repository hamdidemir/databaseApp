CC = gcc
CFLAGS = -Wall -Wextra -g -I. 
LDFLAGS = -lm 
TARGET = run
SRCS = src/main.c src/databaseConnection/connect.c src/databaseConnection/disconnect.c src/databaseConnection/processDatabaseFile.c src/databaseConnection/createDatabase.c src/databaseConnection/deleteDatabase.c src/dataStructure/bplustree.c src/dataStructure/arrayList.c src/parseSQL/parseCreate.c src/parseSQL/parseMainQuery.c src/parseSQL/parseInsert.c src/parseSQL/parseSelect.c src/helpers/trim.c src/helpers/checkTable.c src/helpers/isDataEntry.c src/helpers/fileExists.c src/helpers/printTable.c src/helpers/compare.c src/loadFromFile/loadCSV.c test/setUpAndTearDown.c test/testParsing/testParseCreate.c test/testDatabaseConnection/testingConnectAndDisconnect.c test/testDatabaseConnection/testingCreateAndDelete.c test/testDatabaseConnection/testingProcessDatabaseFile.c test/testHelpers/testingFileExist.c test/testHelpers/testingCheckTable.c test/testHelpers/testingIsDataEntry.c test/testHelpers/testingTrim.c test/testParsing/testParseSelect.c test/testDataStructure/testBPlusTree.c dependencies/cJSON.c dependencies/unity.c

OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))

VPATH = src:test

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)