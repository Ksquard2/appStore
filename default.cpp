#include <mysql/mysql.h>
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

static void exitWithError(MYSQL* conn, const char* context) {
    cerr << "[ERROR] " << context << ": " << mysql_error(conn) << endl;
    if (conn != nullptr) {
        mysql_close(conn);
    }
    std::exit(EXIT_FAILURE);
}

static void execOrDie(MYSQL* conn, const string& sql, const char* what) {
    if (mysql_query(conn, sql.c_str()) != 0) {
        exitWithError(conn, what);
    }
    // For non-SELECT statements, there is no result set to consume
}

int main() {
    const char* host = "127.0.0.1";            // Force TCP to avoid socket quirks
    const char* user = "root";
    const char* pass = "newlatiospower";
    const char* dbName = "demo";
    const unsigned int port = 3306;

    cout << "[INFO] Initializing MySQL client..." << endl;
    MYSQL* conn = mysql_init(nullptr);
    if (!conn) {
        cerr << "[ERROR] mysql_init failed." << endl;
        return 1;
    }

    // Optional but helpful: keep connection attempts snappy
    unsigned int timeoutSeconds = 5;
    mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeoutSeconds);

    cout << "[INFO] Connecting to MySQL server " << host << ":" << port << " as '" << user << "'..." << endl;
    if (!mysql_real_connect(conn, host, user, pass, nullptr, port, nullptr, 0)) {
        exitWithError(conn, "mysql_real_connect (server)");
    }
    cout << "[OK] Connected. Server: " << mysql_get_server_info(conn)
         << " | Host info: " << mysql_get_host_info(conn) << endl;

    if (mysql_ping(conn) == 0) {
        cout << "[OK] Ping successful: connection is alive." << endl;
    } else {
        exitWithError(conn, "mysql_ping after connect");
    }

    cout << "[INFO] Ensuring database '" << dbName << "' exists..." << endl;
    execOrDie(conn,
              "CREATE DATABASE IF NOT EXISTS `" + string(dbName) +
                  "` CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci",
              "create database");
    cout << "[OK] Database check complete." << endl;

    cout << "[INFO] Selecting database '" << dbName << "'..." << endl;
    if (mysql_select_db(conn, dbName) != 0) {
        exitWithError(conn, "mysql_select_db");
    }
    cout << "[OK] Database selected." << endl;

    if (mysql_ping(conn) == 0) {
        cout << "[OK] Ping successful inside database '" << dbName << "'." << endl;
    } else {
        exitWithError(conn, "mysql_ping after select_db");
    }

    cout << "[INFO] Creating table 'users' if it does not exist..." << endl;
    execOrDie(conn,
              "CREATE TABLE IF NOT EXISTS `users` ("
              "  `id` INT NOT NULL AUTO_INCREMENT,"
              "  `name` VARCHAR(100) NOT NULL,"
              "  `email` VARCHAR(100) NOT NULL,"
              "  PRIMARY KEY (`id`),"
              "  UNIQUE KEY `uniq_email` (`email`)"
              ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4",
              "create table users");
    cout << "[OK] Table ready." << endl;

    cout << "[INFO] Inserting sample data..." << endl;
    // Use INSERT IGNORE to keep re-runs idempotent if emails already exist
    execOrDie(conn,
              "INSERT IGNORE INTO `users` (`name`, `email`) VALUES "
              "('Alice', 'alice@example.com'),"
              "('Bob', 'bob@example.com'),"
              "('Carol', 'carol@example.com')",
              "insert sample data");
    cout << "[OK] Sample data inserted (or already present)." << endl;

    cout << "[INFO] Selecting all users..." << endl;
    const string selectSql = "SELECT `id`, `name`, `email` FROM `users` ORDER BY `id`";
    if (mysql_query(conn, selectSql.c_str()) != 0) {
        exitWithError(conn, "select users");
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        exitWithError(conn, "mysql_store_result");
    }

    cout << "----------------------------------------" << endl;
    cout << "id\tname\temail" << endl;
    cout << "----------------------------------------" << endl;
    MYSQL_ROW row;
    unsigned long numRows = 0;
    while ((row = mysql_fetch_row(result))) {
        const char* id = row[0] ? row[0] : "";
        const char* nameVal = row[1] ? row[1] : "";
        const char* emailVal = row[2] ? row[2] : "";
        cout << id << "\t" << nameVal << "\t" << emailVal << endl;
        ++numRows;
    }
    cout << "----------------------------------------" << endl;
    cout << "[OK] Retrieved " << numRows << " row(s)." << endl;

    mysql_free_result(result);

    if (mysql_ping(conn) == 0) {
        cout << "[OK] Final ping successful; database operations complete." << endl;
    } else {
        exitWithError(conn, "final mysql_ping");
    }

    mysql_close(conn);
    cout << "[DONE] Closed connection. Exiting." << endl;
    return 0;
}
