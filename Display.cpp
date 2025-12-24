#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
using namespace std;

struct App{
    int appId;
    string appName;
    string description;
    double appPrice;
    string link;
    string Icon;
};
struct Watch{
    int watchId;
    string watchName;
    string password;
    string status;
    double watchPrice;
    string orderDate;
    vector<App> Apps;

};
struct User{
    string userId;
    string username;
    string password;
    string email;
    string type;
    string FirstName;
    string LastName;
    vector<Watch> watches;
};

// return a one-line summary for an App (excluding Icon)
string getAppSummary(const App &app){
    ostringstream ss;
    ss << "ID: " << app.appId << ", ";
    ss << app.appName << ", ";
    ss << "($" << fixed << setprecision(2) << app.appPrice << "), ";
    ss << "Desc: " << app.description << ", ";
    ss << "Link: " << app.link;
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
    cout << left << setw(8) << (string("    Watch: ")+to_string(w.watchId)) <<endl
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

// set all watches for a user to active
void setAllWatchesActive(User &u){
    for(auto &w : u.watches) w.status = "active";
}

int main(){
    // create one user with 2 watches and 2 apps per watch
    User u;
    u.userId = "u1";
    u.username = "jdoe";
    u.password = "pass";
    u.email = "jdoe@example.com";
    u.type = "standard";
    u.FirstName = "John";
    u.LastName = "Doe";

    // watch 1
    Watch w1;
    w1.watchId = 101;
    w1.watchName = "AlphaWatch";
    w1.password = "wp1";
    w1.status = "inactive"; // will set to active below
    w1.watchPrice = 199.99;
    w1.orderDate = "2025-12-19";
    App a11{1, "WeatherApp", "Shows weather", 0.0, "http://a11", "icon1"};
    App a12{2, "FitnessApp", "Tracks steps", 1.99, "http://a12", "icon2"};
    w1.Apps.push_back(a11);
    w1.Apps.push_back(a12);

    // watch 2
    Watch w2;
    w2.watchId = 102;
    w2.watchName = "BetaWatch";
    w2.password = "wp2";
    w2.status = "inactive";
    w2.watchPrice = 249.99;
    w2.orderDate = "2025-12-19";
    App a21{3, "MusicApp", "Plays music", 2.99, "http://a21", "icon3"};
    App a22{4, "SleepApp", "Monitors sleep", 0.0, "http://a22", "icon4"};
    w2.Apps.push_back(a21);
    w2.Apps.push_back(a22);

    u.watches.push_back(w1);
    u.watches.push_back(w2);

    // set all watches active
    setAllWatchesActive(u);

    // display user (we only have one user, index 1 -> index 0)
    vector<User> users{u};
    for(size_t i=0;i<users.size();++i){
        displayUser(users[i], (int)i);
        cout << "\n";
    }

    return 0;
}