USE appStore;

DROP TABLE IF EXISTS AppItems;
DROP TABLE IF EXISTS Apps;
DROP TABLE IF EXISTS WatchAccounts;
DROP TABLE IF EXISTS Developers;
DROP TABLE IF EXISTS Customers;

CREATE TABLE IF NOT EXISTS `Customers` (
    `Userid` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `Fname` VARCHAR(255) NOT NULL,
    `Lname` VARCHAR(255) NOT NULL,
    `Email` VARCHAR(255) NOT NULL,
    `Username` VARCHAR(255) NOT NULL,
    `Password` VARCHAR(255) NOT NULL,
    `Type` VARCHAR(255) NOT NULL,
    `Icon` BLOB,
    UNIQUE KEY `customers_username_unique` (`Username`)
);

CREATE TABLE IF NOT EXISTS `WatchAccounts` (
    `Watchid` BIGINT NOT NULL ,
    `Userid` BIGINT UNSIGNED NOT NULL,          -- FIXED (unsigned to match Customers.Userid)
    `Username` VARCHAR(255) NOT NULL,
    `Password` BIGINT NOT NULL,
    `Status` VARCHAR(255) NOT NULL,
    `OrderDate` DATETIME NOT NULL,
    `Price` DECIMAL(8, 2) NOT NULL,
    PRIMARY KEY (`Watchid`)
);

CREATE TABLE IF NOT EXISTS `Developers` (
    `Devid` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `Fname` VARCHAR(255) NOT NULL,
    `Lname` VARCHAR(255) NOT NULL,
    `Github` VARCHAR(255) NOT NULL,
    `Username` VARCHAR(255) NOT NULL,
    `Password` VARCHAR(255) NOT NULL,
    `Description` VARCHAR(255),
    `Icon` BLOB,
    `Email` VARCHAR(255) NOT NULL
);

CREATE TABLE IF NOT EXISTS `Apps` (
    `Appid` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `Devid` BIGINT UNSIGNED NOT NULL,           -- FIXED (unsigned to match Developers.Devid)
    `Name` VARCHAR(255) NOT NULL,
    `Link` VARCHAR(255) NOT NULL,
    `Description` VARCHAR(255) NOT NULL,
    `Icon` BLOB NOT NULL,
    `Price` DECIMAL(8, 2) NOT NULL
);

CREATE TABLE IF NOT EXISTS `AppItems` (
    `Itemid` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `Watchid` BIGINT NOT NULL,
    `Appid` BIGINT UNSIGNED NOT NULL            -- FIXED (unsigned to match Apps.Appid)
);

-- Add foreign keys (see note below about IF NOT EXISTS)
ALTER TABLE `WatchAccounts`
    ADD CONSTRAINT `watchaccounts_userid_foreign`
    FOREIGN KEY (`Userid`) REFERENCES `Customers` (`Userid`);

ALTER TABLE `AppItems`
    ADD CONSTRAINT `appitems_watchid_foreign`
    FOREIGN KEY (`Watchid`) REFERENCES `WatchAccounts` (`Watchid`);

ALTER TABLE `AppItems`
    ADD CONSTRAINT `appitems_appid_foreign`
    FOREIGN KEY (`Appid`) REFERENCES `Apps` (`Appid`);

ALTER TABLE `Apps`
    ADD CONSTRAINT `apps_devid_foreign`
    FOREIGN KEY (`Devid`) REFERENCES `Developers` (`Devid`);
    
INSERT INTO Customers (Fname, Lname, Email, Username, Password, Type, Icon)
VALUES
('Alice', 'Johnson', 'alice@example.com', 'alicej', 'hashed_pw_1', 'user', '00'),
('Bob', 'Smith', 'bob@example.com', 'bobsmith', 'hashed_pw_2', 'user', '00');
INSERT INTO Developers (Fname, Lname, Github, Username, Password, Description, Icon,Email)
VALUES
('Carol', 'Dev', 'https://github.com/caroldev', '1001', '123456', 'Fitness app developer', '00','carol@gmail.com'),
('Dave', 'Coder', 'https://github.com/davecoder', 1002, '789012', 'Productivity app developer', '00','dave@gmail.com');

INSERT INTO WatchAccounts (Watchid, Userid, Username, Password, Status, OrderDate, Price)
VALUES
(5001, 1, 'alicej', 1111, 'ACTIVE', '2024-01-10 10:30:00', 299.99),
(5002, 2, 'bobsmith', 2222, 'ACTIVE', '2024-01-12 14:00:00', 249.99);

INSERT INTO Apps (Devid, Name, Link, Description, Icon, Price)
VALUES
(1, 'FitTrack', 'https://apps.example.com/fittrack', 'Fitness tracking app', '00', 4.99),
(1, 'HeartPlus', 'https://apps.example.com/heartplus', 'Heart rate monitor', '00', 2.99),
(2, 'TaskTime', 'https://apps.example.com/tasktime', 'Task productivity app', '00', 3.99);

INSERT INTO AppItems (Watchid, Appid)
VALUES
(5001, 1),  -- Alice's watch has FitTrack
(5001, 2),  -- Alice's watch has HeartPlus
(5002, 3);  -- Bob's watch has TaskTime



