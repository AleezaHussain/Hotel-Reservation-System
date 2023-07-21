#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
using namespace std;
SQLHANDLE sqlConnHandle;
SQLHANDLE sqlStmtHandle;
SQLHANDLE sqlEnvHandle;
string cnIc;
bool establishConnection() {
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

    //initializations
    sqlConnHandle = NULL;
    sqlStmtHandle = NULL;

    //allocations
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        return false;

    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        return false;

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        return false;

    //output
    //cout << "Attempting connection to SQL Server..." << endl;

    //connect to SQL Server
    switch (SQLDriverConnect(sqlConnHandle,
        NULL,
        (SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost;DATABASE=SavanzaInn;Trusted=true;",
        SQL_NTS,
        NULL,
        0,
        NULL,
        SQL_DRIVER_NOPROMPT)) {

    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        //cout << "Successfully connected to SQL Server" << endl;
        return true;

    case SQL_INVALID_HANDLE:
    case SQL_ERROR:
        cout << "Could not connect to SQL Server" << endl;
        return false;

    default:
        return false;
    }
}
void executeInsertQuery(string customername, string CNICc, string contactno) {
    // If there is a problem connecting, exit the function
    if (!establishConnection()) {
        cout << "Connection to SQL Server failed." << endl;
        return;
    }

    // Construct the query string
    string queryString = "INSERT INTO personalinfo(customerName, CNIC, contact_no) VALUES ('" + customername + "','" + CNICc + "','" + contactno + "');";

    // Convert the query string to wide characters (SQLWCHAR*)
    int queryLength = MultiByteToWideChar(CP_UTF8, 0, queryString.c_str(), -1, nullptr, 0);
    if (queryLength == 0) {
        cout << "Error converting query string to wide characters." << endl;
        return;
    }

    SQLWCHAR* query = new SQLWCHAR[queryLength];
    if (MultiByteToWideChar(CP_UTF8, 0, queryString.c_str(), -1, query, queryLength) == 0) {
        cout << "Error converting query string to wide characters." << endl;
        delete[] query;
        return;
    }

    // If there is a problem allocating the statement handle, exit the function
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
        cout << "Error allocating statement handle." << endl;
        delete[] query;
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        return;
    }

    // Output
    //cout << "Executing T-SQL query..." << endl;

    // If there is a problem executing the query, exit the function
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, query, SQL_NTS)) {
        cout << "Error executing query." << endl;
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLDisconnect(sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        delete[] query;
        return;
    }

    // Free the statement handle
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);

    // Disconnect from the SQL server
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);

    delete[] query; // Free allocated memory
}



void selectQuery(int number) {
    //if there is a problem connecting, exit function
    if (!establishConnection()) {
        std::cout << "Connection to SQL Server failed." << endl;
        return;
    }

    //if there is a problem allocating statement handle, exit function
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
        std::cout << "Error allocating statement handle." << endl;
        return;
    }

    //output
    //std::cout << "Executing T-SQL query..." << endl;
    std::wstring numberStr = std::to_wstring(number);
    std::wstring query = L"SELECT * FROM personalinfo WHERE CustomerID = " + numberStr;
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
        cout << "Error executing query." << endl;
        return;
    }
 

    //fetch and display the result set
        SQLCHAR CustomerID[SQL_RESULT_LEN];
        SQLCHAR customerName[SQL_RESULT_LEN];
        SQLCHAR CNIC[SQL_RESULT_LEN];
        SQLCHAR contact_no[SQL_RESULT_LEN];
        SQLLEN  ptrcustomerName, ptrCustomerID, ptrCNIC, ptrcontact_no;

        //std::cout << "\nQuery Result:\n";

        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
            SQLGetData(sqlStmtHandle, 1, SQL_CHAR, CustomerID, SQL_RESULT_LEN, &ptrCustomerID);
            SQLGetData(sqlStmtHandle, 2, SQL_CHAR, customerName, SQL_RESULT_LEN, &ptrcustomerName);
            SQLGetData(sqlStmtHandle, 3, SQL_CHAR, CNIC, SQL_RESULT_LEN, &ptrCNIC);
            SQLGetData(sqlStmtHandle, 4, SQL_CHAR, contact_no, SQL_RESULT_LEN, &ptrcontact_no);

            std::cout << "customer id: " << CustomerID << std::endl;
            std::cout << "customer Name: " << customerName << std::endl;
            std::cout << "cnic " << CNIC << std::endl;
            std::cout << "contact no " << contact_no << std::endl;
            std::cout << "-----------------------------\n";
        }
    
}
void selectQueryforroom(int number) {
    //if there is a problem connecting, exit function
    if (!establishConnection()) {
        std::cout << "Connection to SQL Server failed." << endl;
        return;
    }

    //if there is a problem allocating statement handle, exit function
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
        std::cout << "Error allocating statement handle." << endl;
        return;
    }

    //output
    //std::cout << "Executing T-SQL query..." << endl;
    std::wstring numberStr = std::to_wstring(number);
    std::wstring query = L"SELECT * FROM roominfo WHERE CustomerID = " + numberStr;
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
        cout << "Error executing query." << endl;
        return;
    }


    //fetch and display the result set
    SQLCHAR room[SQL_RESULT_LEN];
    SQLCHAR category[SQL_RESULT_LEN];
    SQLCHAR checkin[SQL_RESULT_LEN];
    SQLCHAR checkout[SQL_RESULT_LEN];
    SQLCHAR CustomerID[SQL_RESULT_LEN];
    SQLCHAR familynumbers[SQL_RESULT_LEN];

    SQLLEN  ptrroom, ptrcategory, ptrcheckin, ptrcheckout,ptrcustomerid,ptrfamilymembers;

    //std::cout << "\nQuery Result:\n";

    while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
        SQLGetData(sqlStmtHandle, 1, SQL_CHAR, room, SQL_RESULT_LEN, &ptrroom);
        SQLGetData(sqlStmtHandle, 2, SQL_CHAR, category, SQL_RESULT_LEN, &ptrcategory);
        SQLGetData(sqlStmtHandle, 3, SQL_CHAR, checkin, SQL_RESULT_LEN, &ptrcheckin);
        SQLGetData(sqlStmtHandle, 4, SQL_CHAR, checkout, SQL_RESULT_LEN, &ptrcheckout);
        SQLGetData(sqlStmtHandle, 5, SQL_CHAR, CustomerID, SQL_RESULT_LEN, &ptrcustomerid);
        SQLGetData(sqlStmtHandle, 6, SQL_CHAR, familynumbers, SQL_RESULT_LEN, &ptrfamilymembers);

        std::cout << "Room ID: " << room << std::endl;
        std::cout << "Category: " << category << std::endl;
        std::cout << "checkin " << checkin << std::endl;
        std::cout << "checkout" << checkout << std::endl;
        std::cout << "Customer ID: " << CustomerID<< std::endl;
        std::cout << "Family members: " << familynumbers << std::endl;
        std::cout << "-----------------------------\n";
    }

}
void selectcustomerid(string cnic) {
    //if there is a problem connecting, exit function
    if (!establishConnection()) {
        std::cout << "Connection to SQL Server failed." << endl;
        return;
    }

    //if there is a problem allocating statement handle, exit function
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
        std::cout << "Error allocating statement handle." << endl;
        return;
    }

    //output
    //std::cout << "Executing T-SQL query..." << endl;
    std::wstring cnicWstr(cnic.begin(), cnic.end());  // Convert cnic string to wstring
    std::wstring query = L"SELECT CustomerID FROM personalinfo WHERE CNIC = '" + cnicWstr + L"'";
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
        cout << "Error executing query." << endl;
        return;
    }

    //fetch and display the result set
    SQLCHAR CustomerID[SQL_RESULT_LEN];
    SQLLEN   ptrCustomerID;

    //std::cout << "\nQuery Result:\n";

    while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
        SQLGetData(sqlStmtHandle, 1, SQL_CHAR, CustomerID, SQL_RESULT_LEN, &ptrCustomerID);

        std::cout << "customer id: " << CustomerID << std::endl;
        std::cout << "-----------------------------\n";
    }

}
void executeDeleteQuery(int number){
    //if there is a problem connecting, exit function
    if (!establishConnection()) {
        cout << "Connection to SQL Server failed." << endl;
        return;
    }

    //if there is a problem allocating statement handle, exit function
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
        cout << "Error allocating statement handle." << endl;
        return;
    }

    //output
    //std::cout << "Executing T-SQL query..." << endl;
    std::wstring numberStr = std::to_wstring(number);
    std::wstring query = L"DELETE FROM personalinfo WHERE CustomerID = " + numberStr;

    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
        cout << "Error executing query." << endl;
        return;
    }
}
void executeInsertRoomQuery(int roomNO, string Category, string chkin, string checkout, int customerid, int familyMembers) {
    // If there is a problem connecting, exit the function
    if (!establishConnection()) {
        cout << "Connection to SQL Server failed." << endl;
        return;
    }

    // Construct the query string
    string queryString = "INSERT INTO roominfo(room,category,checkin,checkout,CustomerID,familynumbers) VALUES (" +
        std::to_string(roomNO) + ", '" + Category + "', '" + chkin + "', '" + checkout + "', " +
        std::to_string(customerid) + ", " + std::to_string(familyMembers) + ");";

    // Convert the query string to wide characters (SQLWCHAR*)
    int queryLength = MultiByteToWideChar(CP_UTF8, 0, queryString.c_str(), -1, nullptr, 0);
    if (queryLength == 0) {
        cout << "Error converting query string to wide characters." << endl;
        return;
    }

    SQLWCHAR* query = new SQLWCHAR[queryLength];
    if (MultiByteToWideChar(CP_UTF8, 0, queryString.c_str(), -1, query, queryLength) == 0) {
        cout << "Error converting query string to wide characters." << endl;
        delete[] query;
        return;
    }

    // If there is a problem allocating the statement handle, exit the function
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
        cout << "Error allocating statement handle." << endl;
        delete[] query;
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        return;
    }

    // Output
    //cout << "Executing T-SQL query..." << endl;

    // If there is a problem executing the query, exit the function
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, query, SQL_NTS)) {
        cout << "Error executing query." << endl;

        // Error handling code to retrieve and display detailed error information
        SQLSMALLINT i = 1;
        SQLWCHAR sqlState[6];
        SQLINTEGER nativeError;
        SQLWCHAR message[SQL_MAX_MESSAGE_LENGTH];
        SQLSMALLINT cbMessage;

        while (SQL_SUCCESS == SQLGetDiagRec(SQL_HANDLE_STMT, sqlStmtHandle, i, sqlState, &nativeError, message, sizeof(message), &cbMessage)) {
            cout << "SQLSTATE: " << sqlState << ", Native Error: " << nativeError << ", Message: " << message << endl;
            i++;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLDisconnect(sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        delete[] query;
        return;
    }

    // Free the statement handle
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);

    // Disconnect from the SQL server
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);

    delete[] query; // Free allocated memory
}
void paymentRsNormal(int number) {
    // if there is a problem connecting, exit function
    if (!establishConnection()) {
        cout << "Connection to SQL Server failed." << endl;
        return;
    }

    // if there is a problem allocating statement handle, exit function
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
        cout << "Error allocating statement handle." << endl;
        // Clean up resources
        SQLDisconnect(sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        return;
    }

    // output
    //cout << "Executing T-SQL query..." << endl;
    wstring numberStr = to_wstring(number);
    wstring query = L"SELECT DATEDIFF(minute, checkin, checkout) AS TotalMinutes, "
        L"DATEDIFF(hour, checkin, checkout) AS TotalHours, "
        L"DATEDIFF(day, checkin, checkout) AS TotalDays, "
        L"familynumbers "
        L"FROM roominfo WHERE CustomerID = " + numberStr;

    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
        cout << "Error executing query." << endl;
        // Clean up resources
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLDisconnect(sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        return;
    }

    // Fetch and display the result
    SQLLEN indicator;
    int totalMinutes, totalHours, totalDays, familyNumbers;

    if (SQL_SUCCESS == SQLFetch(sqlStmtHandle)) {
        SQLGetData(sqlStmtHandle, 1, SQL_C_LONG, &totalMinutes, sizeof(totalMinutes), &indicator);
        SQLGetData(sqlStmtHandle, 2, SQL_C_LONG, &totalHours, sizeof(totalHours), &indicator);
        SQLGetData(sqlStmtHandle, 3, SQL_C_LONG, &totalDays, sizeof(totalDays), &indicator);
        SQLGetData(sqlStmtHandle, 4, SQL_C_LONG, &familyNumbers, sizeof(familyNumbers), &indicator);

        //cout << "Total Minutes: " << totalMinutes << endl;
        cout << "Total Hours: " << totalHours << endl;
        //cout << "Total Days: " << totalDays << endl;
        cout << "Total payment you need to do: " << totalDays * 2000 * familyNumbers << " Pkr" << endl;
    }

    // Clean up resources
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
}


void paymentRsspecial(int number) {
    // if there is a problem connecting, exit function
    if (!establishConnection()) {
        cout << "Connection to SQL Server failed." << endl;
        return;
    }

    // if there is a problem allocating statement handle, exit function
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
        cout << "Error allocating statement handle." << endl;
        // Clean up resources
        SQLDisconnect(sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        return;
    }

    // output
    //cout << "Executing T-SQL query..." << endl;
    wstring numberStr = to_wstring(number);
    wstring query = L"SELECT DATEDIFF(minute, checkin, checkout) AS TotalMinutes, "
        L"DATEDIFF(hour, checkin, checkout) AS TotalHours, "
        L"DATEDIFF(day, checkin, checkout) AS TotalDays, "
        L"familynumbers "
        L"FROM roominfo WHERE CustomerID = " + numberStr;

    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) {
        cout << "Error executing query." << endl;
        // Clean up resources
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLDisconnect(sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        return;
    }

    // Fetch and display the result
    SQLLEN indicator;
    int totalMinutes, totalHours, totalDays, familyNumbers;

    if (SQL_SUCCESS == SQLFetch(sqlStmtHandle)) {
        SQLGetData(sqlStmtHandle, 1, SQL_C_LONG, &totalMinutes, sizeof(totalMinutes), &indicator);
        SQLGetData(sqlStmtHandle, 2, SQL_C_LONG, &totalHours, sizeof(totalHours), &indicator);
        SQLGetData(sqlStmtHandle, 3, SQL_C_LONG, &totalDays, sizeof(totalDays), &indicator);
        SQLGetData(sqlStmtHandle, 4, SQL_C_LONG, &familyNumbers, sizeof(familyNumbers), &indicator);

        //cout << "Total Minutes: " << totalMinutes << endl;
        cout << "Total Hours: " << totalHours << endl;
        //cout << "Total Days: " << totalDays << endl;
        cout << "Total payment you need to do: " << totalDays * 1500 * familyNumbers << " Pkr" << endl;
    }

    // Clean up resources
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
}
void Feedback(string cnic, string feeedback) {
    // If there is a problem connecting, exit the function
    if (!establishConnection()) {
        cout << "Connection to SQL Server failed." << endl;
        return;
    }

    // Construct the query string
    // Construct the query string
    string queryString = "INSERT INTO feedback (CNIC, feedback) VALUES ('" + cnic + "','" + feeedback + "');";


    // Convert the query string to wide characters (SQLWCHAR*)
    int queryLength = MultiByteToWideChar(CP_UTF8, 0, queryString.c_str(), -1, nullptr, 0);
    if (queryLength == 0) {
        cout << "Error converting query string to wide characters." << endl;
        return;
    }

    SQLWCHAR* query = new SQLWCHAR[queryLength];
    if (MultiByteToWideChar(CP_UTF8, 0, queryString.c_str(), -1, query, queryLength) == 0) {
        cout << "Error converting query string to wide characters." << endl;
        delete[] query;
        return;
    }

    // If there is a problem allocating the statement handle, exit the function
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
        cout << "Error allocating statement handle." << endl;
        delete[] query;
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        return;
    }

    // Output
    //cout << "Executing T-SQL query..." << endl;

    // If there is a problem executing the query, exit the function
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, query, SQL_NTS)) {
        cout << "Error executing query." << endl;
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLDisconnect(sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        delete[] query;
        return;
    }

    // Free the statement handle
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);

    // Disconnect from the SQL server
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);

    delete[] query; // Free allocated memory
}

class User{
protected:
    string customer_name;
    string CNIC, Contact;
public:
    virtual void getinfo() {
        cout << "Name" << endl;
        cin >> customer_name;
        cout << "CNIC" << endl;
        cin >> CNIC;
        ::cnIc = CNIC;
        cout << "Phone#" << endl;
        cin >> Contact;

    }
    virtual void connecttoSQLserver() {
        cout << "Welcome to savanaza Inn" << endl;
        establishConnection();
    }
    
    void maintainUserprofile() {
        executeInsertQuery(customer_name, CNIC, Contact);
    }
    void showCustomerID() {
        selectcustomerid(cnIc);
    }
    void deleteRecord() {
        int delID;
        cout << "Enter your customer ID to delete Record;" << endl; cin >> delID;
        executeDeleteQuery(delID);
    }
};

class Room{
protected:
    int Room, familyMembers;
    string categorry, checkIn,checkOut;
    int Customerid;
public:
    void getinfo() {
        cout << "room" << endl;
        cin >> Room;
        cout << "Category: Normal or Special" << endl;
        cin >> categorry;
        cin.ignore();
        cout << "Check in(date when you start living in our hotel please write in yyyy-mm-dd hh:mm:ss format)";
        getline(cin, checkIn);
        cout << "Check out(date when you leave our hotel please write in yyyy-mm-dd hh:mm:ss format)";
        getline(cin, checkOut);
        cout << "Enter your customerId" << endl;
        cin >> Customerid;
        cin.ignore();
        cout << "Enter your family members" << endl;
        cin >> familyMembers;
        cin.ignore();
    }
    void connecttoSQLserver() {
        establishConnection();
    }
    void maintainroominfo() {
        executeInsertRoomQuery(Room,categorry,checkIn,checkOut,Customerid,familyMembers);
    }
};
class payment:public Room {
protected:
    int custmrid;
    string catgory;
public:
    void totalamount() {
        cout << categorry;
        if (catgory=="Normal") {
            paymentRsNormal(custmrid);
        }
        else if (catgory=="Special") {
            paymentRsspecial(custmrid);
        }
    }
    void enterID() {
        cout << "Please enter your customer ID" << endl;
        cin >> custmrid;
        cout << "Please enter your category of room: " << endl;
        cin >> catgory;
    }
};
class Bill :public payment,Virtual public Room {
    int customerID;
public:
    void showBill() {
        cout << "Enter your customer id: " << endl;
        cin >> customerID;
        selectQuery(customerID);
        selectQueryforroom(customerID);
    }
};
class userFeedback {
    string Usercnic, Userfeedback;
public:
    void getUserfeeedback() {
        cout << "Enter your cnic;" << endl;
        cin >> Usercnic;
        cin.ignore();
        cout << "Enter your feedback;" << endl;
        getline(cin, Userfeedback);
        cout << Userfeedback;
        Feedback(Usercnic, Userfeedback);
    }

};
int main(){
    User kanza;
    kanza.connecttoSQLserver();
    kanza.getinfo();
    kanza.maintainUserprofile();
    kanza.showCustomerID();
    Room obj;
    obj.connecttoSQLserver();
    obj.getinfo();
    obj.maintainroominfo();
    cout << "\nBILL DETAILS:" << endl;
    payment obj1;
    obj1.enterID();
    obj1.totalamount();  
    Bill object1; 
    object1.showBill();
    userFeedback obj3;
    obj3.getUserfeeedback();
    
    
}