#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>
#include <memory>

using ::testing::_;
using ::testing::Return;

class DBConnection
{
public:
    virtual ~DBConnection() = default;

    virtual void open()
    {
        std::cout << "Opening a connection to the database\n";
    }

    virtual void close()
    {
        std::cout << "Closing the database connection\n";
    }

    virtual std::string execQuery(const std::string &query)
    {
        std::cout << "Making a query to the database:: " << query << "\n";
        return;
    }
};

class ClassThatUsesDB
{
private:
    std::shared_ptr<DBConnection> dbConnection;

public:
    ClassThatUsesDB(std::shared_ptr<DBConnection> conn) : dbConnection(conn) {}

    void openConnection()
    {
        dbConnection->open();
    }

    void useConnection()
    {
        std::string result = dbConnection->execQuery("SELECT * FROM table;");
        std::cout << "Query result: " << result << "\n";
    }

    void closeConnection()
    {
        dbConnection->close();
    }
};

class MockDBConnection : public DBConnection
{
public:
    MOCK_METHOD(void, open, (), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(std::string, execQuery, (const std::string &query), (override));
};

TEST(ClassThatUsesDBTests, OpenConnectionCallsDBOpen)
{
    auto mockDB = std::make_shared<MockDBConnection>();
    EXPECT_CALL(*mockDB, open()).Times(1);

    ClassThatUsesDB classUnderTest(mockDB);
    classUnderTest.openConnection();
}

TEST(ClassThatUsesDBTests, UseConnectionExecutesQuery)
{
    auto mockDB = std::make_shared<MockDBConnection>();
    EXPECT_CALL(*mockDB, execQuery("SELECT * FROM table;"))
        .Times(1)
        .WillOnce(Return("Mocked Result"));

    ClassThatUsesDB classUnderTest(mockDB);
    classUnderTest.useConnection();
}

TEST(ClassThatUsesDBTests, CloseConnectionCallsDBClose)
{
    auto mockDB = std::make_shared<MockDBConnection>();
    EXPECT_CALL(*mockDB, close()).Times(1);

    ClassThatUsesDB classUnderTest(mockDB);
    classUnderTest.closeConnection();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
