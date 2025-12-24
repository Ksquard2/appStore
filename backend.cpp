#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
using namespace std;

static void exitWithError(MYSQL* conn, const char* context) {//errpr checks each connection
    cerr << "[ERROR] " << context << ": " << mysql_error(conn) << endl;
    if (conn != nullptr) 
    {
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
//object layout to simulate both the json I am sending to premo
//and the table layout from the database
struct Dev{
    string devid;
    string username;
    string password;
    string firstName;
    string lastName;
    string github;
    string description;
    string icon;
    string email;
};
struct App{
    string appId;
    string appName;
    string description;
    string appPrice;
    string link;
    string Icon;
    Dev developer;
};
struct Watch{
    string watchId;
    string watchName;
    string password;
    string status;
    double watchPrice;
    string orderDate;
    vector<App> Apps;//each watch has a list of apps

};
struct User{
    string userId;
    string Icon;
    string username;
    string password;
    string email;
    string type;
    string FirstName;
    string LastName;
    vector<Watch> watches;//each app has a list of watch
};
string getAppSummary(const App &app){
    ostringstream ss;
    ss << "Name: "<<app.appName << ", ";
    ss << "Icon: "<<app.Icon<<", ";
    ss << "($" << fixed << setprecision(2) << app.appPrice << "), ";
    ss << "Desc: " << app.description << ", ";
    ss << "Link: " << app.link;
    return ss.str();
}
string signupQueryU(//creates the query used to insert customer data
    string username,
    string password,
    string email,
    string type,
    string FirstName,
    string LastName,
    string Icon = ""){
        ostringstream ss;

        ss<<"INSERT INTO Customers (Fname, Lname, Email, Username, Password, Type ";
        if(Icon.length() != 0){//allows the icon to be added to the query if they chose their icon in the moment
            ss<<",Icon";
        }
        ss<<") VALUES('";
        ss<<FirstName<<"','";
        ss<<LastName<<"','";
        ss<<email<<"','";
        ss<<username<<"','";
        ss<<password<<"','";
        ss<<type<<"'";
        if(Icon.length() !=0){//same thing here
            ss<<",'"<<Icon<<"'";
        }
        ss<<")";
        return ss.str();
        
    }
bool isNumber(string str){//checks for 
    for(int i = 0;i < str.length();i++){
        if(str[0] <= '0' || str[0] >= '9'){
            return false;
        }
    }
    return str.length();
}
string signupQueryD(//the sign up query for developers 
    string username,
    string password,
    string email,
    string github,
    string FirstName,
    string LastName,
    string description = "",
    string Icon = ""){
        ostringstream ss;
        if(isNumber(description)){//checks for if they send a Icon but not a description
            Icon = description;
            description = "";
        }
        ss<<"INSERT INTO Developers (Fname, Lname, Email, Github, Username, Password, ";
        if(Icon.length() != 0){
            ss<<"Icon";
        }
        if(description.length() != 0 ){
            ss<<",Description";
        }
        ss<<") VALUES('";
        ss<<FirstName<<"','";
        ss<<LastName<<"','";
        ss<<email<<"','";
        ss<<github<<"','";
        ss<<username<<"','";
        ss<<password<<"','";
        if(Icon.length() !=0){
            ss<<Icon<<"'";
        }
        if(description.length() != 0 ){
            ss<<" ,'"<<description<<"'";
        }
        ss<<")";
        return ss.str();
        
    }
// display apps on a single horizontal line separated by spacing
void displayAppsHorizontal(const vector<App> &apps, int indent){
    
    const int colWidth = 60;
    for(size_t i=0;i<apps.size();++i){
        for(int i=0;i<indent;i++) cout << ' ';
        string s = getAppSummary(apps[i]);
        cout << left << setw(colWidth) << s<<endl;
    }
    cout << '\n';
}

// display a single watch and its apps (apps horizontally)
void displayWatch(const Watch &w, int indent){
    cout << left << setw(8) << (string("    Watch: ") + w.watchId) << endl
         << "    Name: " << setw(16) << w.watchName << endl
         << "    Pass: " << setw(10) << w.password<< endl
         << "    Status: " << setw(8) << w.status<< endl
         << "    Price: $" << fixed << setprecision(2) << setw(8) << w.watchPrice<< endl
         << "    Order: " << w.orderDate << '\n';
    // apps on one line under the watch
    displayAppsHorizontal(w.Apps, indent + 4);
}

// display a single user vertically, then their watches with a right shift per user
void displayUser(const User &u, int userIndex){
    int userIndent = userIndex * 4; // shift per user
    cout << left << "UserID: " << setw(8) << u.userId << endl
         << "Username: " << setw(12) << u.username<< endl
         << "Icon: " << setw(12) << u.Icon<< endl
         << "Password: " << setw(12) << u.password << '\n';
    cout << left << "Email: " << setw(24) << u.email<< endl
         << "Type: " << setw(10) << u.type << '\n';
    cout << left << "Name: " << setw(20) << (u.FirstName + " " + u.LastName) << '\n';

    // watches with additional right shift relative to the user
    int watchBaseIndent = (userIndex*4) + 4;
    for(const auto &w : u.watches){
        displayWatch(w, watchBaseIndent);
    }
}

void printDev(Dev d){
    cout<<"Developer: "<<endl;
    cout<<"  - Name: "<<d.firstName<<" "<<d.lastName<<", Github: "<<d.github<<", Description: "<<d.description<<", Icon: "<<d.icon<<", Email: "<<endl;
}
void printApps2(vector<App> a){
    cout<<"Apps:"<<endl;
    for(int i = 0;i < a.size();i++){
    cout<<"  -AppID: "<<a[i].appId<<", ";
    cout<<"Description: "<<a[i].description<<", ";
    cout<<"Name: "<<a[i].appName<<", ";
    cout<<"Price: "<<a[i].appPrice<<", ";
    cout<<"Icon: "<<a[i].Icon<<", ";
    cout<<"Link: "<<a[i].link<<endl;
    }
}
void printDev2(Dev d){
    cout<<"Devid: "<<d.devid<<endl;
    cout<<"Firstname: "<<d.firstName<<endl;
    cout<<"Lastname: "<<d.lastName<<endl;
    cout<<"Github: "<<d.github<<endl;
    cout<<"Description: "<<d.description<<endl;
    cout<<"Icon: "<<d.icon<<endl;
    cout<<"Email: "<<d.email<<endl;
    
}
void printApps(vector<App> a){
    for(int i = 0;i < a.size();i++){
    cout<<"AppID: "<<a[i].appId<<endl;
    cout<<"Description: "<<a[i].description<<endl;
    cout<<"Name: "<<a[i].appName<<endl;
    cout<<"Price: "<<a[i].appPrice<<endl;
    cout<<"Icon: "<<a[i].Icon<<endl;
    cout<<"Link: "<<a[i].link<<endl;
    printDev(a[i].developer);
    }
}

void getAllApps(MYSQL* conn){
    //gathers all the app in a list
    vector<App> a;
    //Pulls all the app and dev data for every app. The group by ensures it the apps are distinct 
    string appquery = "SELECT Apps.*, Developers.* FROM Apps JOIN Developers ON Apps.DevID = Developers.DevID GROUP BY Apps.AppID";
    //copies the address of a query ready pointers
    MYSQL* appconn = conn;
    //sends the query to the database
    if (mysql_query(appconn, appquery.c_str()) != 0) {
        exitWithError(appconn, "select apps");
    }
    //stores the result of the query
    MYSQL_RES* result = mysql_store_result(appconn);
    MYSQL_ROW row;
    //iterates through each row of the 
    while((row = mysql_fetch_row(result))){
        //assigned the fields of each record to the object
        App f;
        Dev d;
        f.appId = row[0];
        f.appName = row[2];
        f.link = row[3];
        f.description = row[4];
        f.Icon = row[5];
        f.appPrice = row[6];
        d.firstName = row[8];
        d.lastName = row[9];
        d.github = row[10];
        d.description = row[13];
        d.icon = row[14];
        f.developer = d;
        a.push_back(f);
    }
    //printing the apps
printApps(a);
}
void getDevInfo(MYSQL* conn, string username,string password){
    Dev coder;
    //calls all the developer information based on the user name
    string devquery = "SELECT * FROM Developers where username = '"+username+"'";
    MYSQL* devconn = conn;
    //sends the query string
    if (mysql_query(devconn, devquery.c_str()) != 0) {
        exitWithError(devconn, "select devs");
    }
    MYSQL_RES* result = mysql_store_result(devconn);
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    if(row[5] != password){
        cout<<"Nah bruh";
        return;
    }
    //collects all dev information
    coder.devid = row[0];
    coder.firstName = row[1];
    coder.lastName = row[2];
    coder.github = row[3];
    coder.username = row[4];
    coder.password = row[5];
    coder.description = row[6];
    coder.icon = row[7];
    coder.email = row[8];
    vector<App> a;
    //finds all the apps for a specific developer 
    string appquery = "SELECT Apps.* FROM Developers join apps on Developers.Devid = Apps.Devid where Developers.Devid = "+coder.devid;
    MYSQL* appconn = conn;
    //sends that query 
    if (mysql_query(appconn, appquery.c_str()) != 0) {
        exitWithError(appconn, "select devs");
    }
    MYSQL_RES* result2 = mysql_store_result(appconn);
    MYSQL_ROW row2;
    //collect the list of watches
    while((row2 = mysql_fetch_row(result2))){
        App f;
        f.appId = row2[0];
        f.appName = row2[2];
        f.link = row2[3];
        f.description = row2[4];
        f.Icon = row2[5];
        f.appPrice = row2[6];
        cout<<"price: "<<row2[6]<<endl;
        a.push_back(f);
    }
    printDev2(coder);
    printApps2(a);


}

void getUserInfo(MYSQL* conn, string username,string password){
    User bob;
    //sends query to get all user info based on username
    string userquery = "SELECT * from Customers where username = '"+username+"'";
    MYSQL* userconn = conn;
    if (mysql_query(userconn, userquery.c_str()) != 0) {
        exitWithError(userconn, "select users");
    }
    MYSQL_RES* result = mysql_store_result(userconn);
    MYSQL_ROW row = mysql_fetch_row(result);
    bob.userId = row[0];
    bob.FirstName = row[1];
    bob.LastName = row[2];
    bob.email = row[3];
    bob.username = row[4];
    bob.password = row[5];
    bob.type = row[6];
    bob.Icon = row[7];
    //error checks for bad password
    if(password != bob.password){
        cout<<"Wrong Password!";
        return;
    }
    //gets all the watch accounts for the user based on their user id
    string watchquery = "Select Watchid, WatchAccounts.Username, WatchAccounts.password, Status, Price, OrderDate from WatchAccounts join Customers on Customers.Userid = WatchAccounts.Userid where WatchAccounts.Userid = "+bob.userId;
    MYSQL* watchconn = conn;
    if (mysql_query(watchconn, watchquery.c_str()) != 0) {
        exitWithError(watchconn, "select users");
    }
    result = mysql_store_result(watchconn);
    while((row = mysql_fetch_row(result))){
        //matches all the rows to a watch struct
        Watch w;
        w.watchId = row[0];
        w.watchName = row[1];
        w.password = row[2];
        w.status = row[3];
        w.watchPrice = stod(row[4]);
        w.orderDate = row[5];
        //once the watch id is derived we send another to query to the appitem table to get the info for all the apps. 
        //the appitem table is many to many so it requires a double join statement 
        string appquery = "Select Apps.Name, Apps.description, Apps.price, Apps.Icon, Apps.link from Apps join AppItems on Apps.Appid = AppItems.Appid join WatchAccounts on WatchAccounts.Watchid = AppItems.Watchid where WatchAccounts.Watchid = "+w.watchId;
        MYSQL* appconn = conn;
        if (mysql_query(appconn, appquery.c_str()) != 0) {
            exitWithError(appconn, "select users");
        }
        //once the watch id is derived we send another to 
        MYSQL_RES* result2 = mysql_store_result(appconn);
        while((row = mysql_fetch_row(result2))){
            //collect all the app information
            App a;
            a.appName = row[0];
            a.description = row[1];
            a.appPrice = row[2];
            a.Icon = row[3];
            a.link = row[4];
            //store it in a app vector the watch struct 
            w.Apps.push_back(a);
        }
        //store that watch to a watch vector in customer
        bob.watches.push_back(w);
    }
    displayUser(bob,1);
}

void signupU(MYSQL* conn,   
    string username,
    string password,
    string email,
    string type,
    string FirstName,
    string LastName,
    string Icon = ""){
        //makes the query for the signup based on the information given
        //reminder icon is optional
        string signquery = signupQueryU(username, password, email, type, FirstName, LastName,Icon);
        MYSQL* userconn = conn;
        //sends the query 
        if (mysql_query(userconn, signquery.c_str()) != 0) {
        exitWithError(userconn, "insert users");

    }
    }
void signupD(MYSQL* conn,   
     string username,
    string password,
    string email,
    string github,
    string FirstName,
    string LastName,
    string description = "",
    string Icon = ""){
        //makes the query for the signup based on the information given
        //reminder icon and description are is optional
        string signquery = signupQueryD(username, password, email, github, FirstName, LastName,description,Icon);
        MYSQL* devconn = conn;
        if (mysql_query(devconn, signquery.c_str()) != 0) {
        exitWithError(devconn, "insert dev");

    }
    getDevInfo(conn,username,password);
    }
    //makes query for ordering a watch for the user
string orderWQ(string Watchid, string Userid, string Username, string Password, string OrderDate, string Price){
        ostringstream ss;
        ss<<"INSERT INTO WatchAccounts (Watchid, Userid, Username, Password, Status, OrderDate, Price) VALUES('";
        ss<<Watchid<<"','";
        ss<<Userid<<"','";
        ss<<Username<<"','";
        ss<<Password<<"','";
        //when a watch is first order it is automatically in transit since it needs to be delivered
        ss<<"In-Transit"<<"','";
        ss<<OrderDate<<"',";
        ss<<Price<<")";
        return ss.str();
}
//function to order a watch 
void makeOrder(MYSQL* conn, string Watchid, string Userid, string Username, string Password, string OrderDate, string Price){
    string orderQuery = orderWQ(Watchid, Userid, Username, Password, OrderDate, Price);
    //sends query string to the database
    MYSQL* orderconn = conn;
        if (mysql_query(orderconn, orderQuery.c_str()) != 0) {
        exitWithError(orderconn, "insert watch");
        }
} 
//query for purchasing a new app on ur watch 
string orderAQ(string Watchid, string Appid){
        ostringstream ss;
        ss<<"INSERT INTO AppItems (Watchid, Appid) VALUES('";
        ss<<Watchid<<"','";
        ss<<Appid<<"')";
        return ss.str();
}

void makeOrderA(MYSQL* conn, string Watchid, string Appid){
  
    string orderQuery = orderAQ(Watchid, Appid);
    //sends query to database
    MYSQL* orderconn = conn;
        if (mysql_query(orderconn, orderQuery.c_str()) != 0) {
        exitWithError(orderconn, "insert app");
        }
        getUserInfo(conn, "AKK","123123");
}
//makes query to publish app
string makeAppQ(string Devid, string Name, string Link, string Description, string Icon, string Price){
        ostringstream ss;
        ss<<"INSERT INTO Apps (Devid, Name, Link, Description, Icon, Price) VALUES('";
        ss<<Devid<<"','";
        ss<<Name<<"','";
        ss<<Link<<"','";
        ss<<Description<<"','";
        ss<<Icon<<"','";
        ss<<Price<<"')";
        return ss.str();
}
//sends query to database
void makeApp(MYSQL* conn, string Devid, string Name, string Link, string Description, string Icon, string Price){
    string appquery = makeAppQ(Devid, Name, Link, Description, Icon, Price);
    MYSQL* appconn = conn;
    if (mysql_query(appconn, appquery.c_str()) != 0) {
    exitWithError(appconn, "insert app");
    }
    getDevInfo(conn, "1002","789012");
}
//makes query to update user info
string updateUserQ(string userid,string email = "",string password = "",string Icon = ""){
        ostringstream ss;
        ss<<"UPDATE Customers SET ";
        //condtional update statement consolidate code so that all account information that must be update can be called to one function
        if(email.length()){
            ss<<"email = '"<<email<<"'";
            if(password.length() ||Icon.length()){
                ss<<",";
            }
        }
        if(password.length()){
            ss<<" password = '"<<password<<"'";
            if(Icon.length()){
                ss<<",";
            }
        }
        if(Icon.length()){
            ss<<" Icon = '"<<Icon<<"'";
        }
        ss<<" Where userid = "<<userid;
        return ss.str();
    }
//sends query to database
void updateUser(MYSQL* conn,string userid,string email = "",string password = "",string Icon = ""){
        string updatequery = updateUserQ(userid,email,password,Icon);
        cout<<updatequery<<endl;
        MYSQL* updateconn = conn;
        if (mysql_query(updateconn, updatequery.c_str()) != 0) {
        exitWithError(updateconn, "insert app");
        }
        getUserInfo(conn,"alicej","321321");
    }
//does the same thing for the developer
string updateDevQ(string devid,string email = "",string description = "", string password = "",string Icon = ""){
        ostringstream ss;
        ss<<"UPDATE Developers SET ";
        if(email.length()){
            ss<<"email = '"<<email<<"'";
            if(password.length() ||Icon.length() ||description.length()){
                ss<<",";
            }
        }
        if(password.length()){
            ss<<" password = '"<<password<<"'";
            if(Icon.length()||description.length()){
                ss<<",";
            }
        }
        if(description.length()){
            ss<<" description = '"<<description<<"'";
            if(Icon.length()){
                ss<<",";
            }
        }
        if(Icon.length()){
            ss<<" Icon = '"<<Icon<<"'";
        }
        ss<<" Where devid = "<<devid;
        return ss.str();
    }
void updateDev(MYSQL* conn,string devid,string email = "",string password = "",string Icon = ""){
        string updatequery = updateDevQ(devid,email,password,Icon);
        cout<<updatequery<<endl;
        MYSQL* updateconn = conn;
        if (mysql_query(updateconn, updatequery.c_str()) != 0) {
        exitWithError(updateconn, "insert app");
        }
        getDevInfo(conn,"1002","789012");
    }
//does the same thing for the apps
string updateAppQ(string appid,string name = "",string link = "",string description = "",string Icon = ""){
        ostringstream ss;
        ss<<"UPDATE Apps SET ";
        if(name.length()){
            ss<<"name = '"<<name<<"'";
            if(description.length() ||Icon.length()||link.length()){
                ss<<",";
            }
        }
        if(link.length()){
            ss<<"link = '"<<link<<"'";
            if(description.length()||Icon.length()){
                ss<<",";
            }
        }
        if(description.length()){
            ss<<" description = '"<<description<<"'";
            if(Icon.length()){
                ss<<",";
            }
        }
        if(Icon.length()){

            ss<<" Icon = '"<<Icon<<"'";
        }
        ss<<" Where appid = "<<appid;
        return ss.str();
    }
void updateApp(MYSQL* conn,string appid,string name = "",string description = "",string Icon = ""){
        string updatequery = updateAppQ(appid,name,description,Icon);
        cout<<updatequery<<endl;
        MYSQL* updateconn = conn;
        if (mysql_query(updateconn, updatequery.c_str()) != 0) {
        exitWithError(updateconn, "insert app");
        }
        getAllApps(conn);
    }
int main() {
    //allows pointer to find my port 
    const char* host = "";            // Force TCP to avoid socket quirks
    const char* user = "";
    const char* pass = "";
    const char* dbName = "";
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
    //assgins sql pointer to my specific database
    if (!mysql_real_connect(conn, host, user, pass, nullptr, port, nullptr, 0)) {
        exitWithError(conn, "mysql_real_connect (server)");
    }
    cout << "[OK] Connected. Server: " << mysql_get_server_info(conn)
         << " | Host info: " << mysql_get_host_info(conn) << endl;
    //confirms connection is made
    if (mysql_ping(conn) == 0) {
        cout << "[OK] Ping successful: connection is alive." << endl;
    } else {
        exitWithError(conn, "mysql_ping after connect");
    }

    cout << "[INFO] Selecting database '" << dbName << "'..." << endl;
    //finds specfic database instance in my mqsq; environment 
    if (mysql_select_db(conn, dbName) != 0) {
        exitWithError(conn, "mysql_select_db");
    }
    cout << "[OK] Database selected." << endl;
    //confirms that database name was found
    if (mysql_ping(conn) == 0) {
        cout << "[OK] Ping successful inside database '" << dbName << "'." << endl;
    } else {
        exitWithError(conn, "mysql_ping after select_db");
    }
    //run the tester function
    updateApp(conn,"2","heartstop","heartstop.com");
    //deallocates pointers
    mysql_close(conn);
    cout << "[DONE] Closed connection. Exiting." << endl;
    return 0;
}
