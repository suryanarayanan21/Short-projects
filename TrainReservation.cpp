#include<iostream>
#include<conio.h>
#include<fstream>
#include<string.h>
#include<iomanip>
#include<ctime>
#include<ctype.h>
#define MenuSize(x) sizeof(x)/sizeof(*x)

using namespace std;

/// FILE DEFINITIONS ---------------------------------------------

char train_dir[] = "Trains.txt";
char account_dir[] = "Accounts.txt";
char ticket_dir[] = "Tickets.txt";

/// --------------------------------------------------------------

/// CLASS DECLARATIONS / DEFINITIONS -----------------------------

class Date;
class Train;
class Account;
class Ticket;
class DatabaseManager;
class ReservationSystem;
class UIManager;
class Calendar;
struct Option;

/// ---- MENU RELATED --------------------------------------------

struct Option {
    char* Name;
    void (*f)();
};

/// ---- DATE CLASS ----------------------------------------------

class Date {
    int day;
    int month;
    int year;
    char time[5];
public:
    Date()
    {

    }
    Date(int d, int m, int y, char* t)
    {
        day = d;
        month = m;
        year = y;
        strcpy(time,t);
    }
    void SetDate(int d, int m, int y, char* t)
    {
        day = d;
        month = m;
        year = y;
        strcpy(time,t);
    }
    void Display()
    {
        cout<<day<<"/"<<month<<"/"<<year;
    }

    int Day(){ return day; }
    int Month() { return month; }
    int Year() { return year; }
    char* Time() { return time; }

    static int Compare(Date,Date);
};

int Date::Compare(Date d1, Date d2)
{
    if(d1.Year() == d2.Year()){
        if(d1.Month() == d2.Month()){
            if(d1.Day() == d2.Day()){
                return 0;
            }
            else return d2.Day() - d1.Day();
        }
        else return d2.Month() - d1.Month();
    }
    else return d2.Year() - d1.Year();
}
/// ---- CALENDAR CLASS ------------------------------------------

class Calendar
{
    static Date curr;
    static char MName[15];
    static int grid[6][7];
    static char selector;
    static int pos[2];
    static int day1;
    static int lastDay;
    static void FillGrid();
    static int FirstDay(Date);
public:
    static int DayFromDate(Date);
    static void Display(Date);
    static Date Select(Date);
};

int Calendar::day1 = 0;
int Calendar::lastDay = 0;
int Calendar::grid[6][7];
char Calendar::selector = '*';
int Calendar::pos[2] = {0,0};
Date Calendar::curr;
char Calendar::MName[15];

int Calendar::FirstDay(Date d)
{
    int x=0; long day;
    switch(d.Month()-1){
        case 11 : x += 30;
        case 10 : x += 31;
        case 9 : x += 30;
        case 8 : x += 31;
        case 7 : x += 31;
        case 6 : x += 30;
        case 5 : x += 31;
        case 4 : x += 30;
        case 3 : x += 31;
        case 2 : x += (d.Year()%4==0)?29:28;
        case 1 : x += 31;
        case 0 : if (d.Year()%4==0)
                    x -= 1;
    }

    switch(d.Month()){
        case 1 : lastDay = 31; strcpy(MName,"JANUARY");break;
        case 2 : lastDay = (d.Year()%4 == 0)?29:28; strcpy(MName,"FEBRUARY");break;
        case 3 : lastDay = 31; strcpy(MName,"MARCH");break;
        case 4 : lastDay = 30; strcpy(MName,"APRIL");break;
        case 5 : lastDay = 31; strcpy(MName,"MAY");break;
        case 6 : lastDay = 30; strcpy(MName,"JUNE");break;
        case 7 : lastDay = 31; strcpy(MName,"JULY");break;
        case 8 : lastDay = 31; strcpy(MName,"AUGUST");break;
        case 9 : lastDay = 30; strcpy(MName,"SEPTEMBER");break;
        case 10 : lastDay = 31; strcpy(MName,"OCTOBER");break;
        case 11 : lastDay = 30; strcpy(MName,"NOVEMBER");break;
        case 12 : lastDay = 31; strcpy(MName,"DECEMBER");break;
    }

    day = (d.Year() * 365.25 + x - 1);
    day1 = day%7;
    return day%7;
}

int Calendar::DayFromDate(Date d)
{
    int x = FirstDay(d);
    return (x + d.Day() - 1)%7;
}

void Calendar::FillGrid()
{
    int flag = 0;
    for(int i=0; i<6; ++i){
        for(int j=0; j<7; ++j){
            if(flag == 1){
                grid[i][j] = 0;
                continue;
            }
            if(7*i + j<day1){
                grid[i][j] = 0;
                continue;
            }
            if(7*i + j>=day1)
                grid[i][j] = 7*i + j + 1 - day1;
            if(7*i + j + 1 - day1 >= lastDay)
                flag = 1;
        }
    }
}

void Calendar::Display(Date d)
{
    curr = d;
    system("cls");
    cout<<"\n\n\t\t\t\tSELECT DATE";
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    FirstDay(d);
    FillGrid();
    cout<<"\n\t\t\t\t"<<MName<<" "<<curr.Year()<<"\n\n";
    cout<<"\t\t\tSUN MON TUE WED THU FRI SAT\n\n\t\t\t";
    for(int i=0;i<6; ++i){
        for(int j=0; j<7; ++j){
            if(grid[i][j] != 0){
                if(i == pos[0] && j == pos[1])
                    cout<<selector<<setw(2)<<grid[i][j]<<" ";
                else
                    cout<<setw(3)<<grid[i][j]<<" ";
            }
            else {
                if(i == pos[0] && j == pos[1])
                    cout<<selector<<"   ";
                else cout<<"    ";
            }
        }
        cout<<"\n\n\t\t\t";
    }
    cout<<"\n--------------------------------------------------------------------------------\n\n";
    cout<<"\tUse w,s,a,d to move up,down,left,and right. Press enter to select date.\n";
    cout<<"\tMove selector right of calendar to go to next month.";
}

Date Calendar::Select(Date orig)
{
    do{
        char ch;
        Display(curr);
        ch = getch();
        if(ch == 'w') {
            if(pos[0] - 1 < 0)
                pos[0] = 5;
            else pos[0]--;
        }
        else if(ch == 's'){
            if(pos[0] + 1 > 5)
                pos[0] = 0;
            else pos[0]++;
        }
        else if(ch == 'a'){
            if(pos[1] - 1 < 0){
                pos[1] = 6;
                if(curr.Month() - 1 < 1)
                    curr.SetDate(curr.Day(),12,curr.Year() - 1,curr.Time());
                else
                    curr.SetDate(curr.Day(),curr.Month()-1,curr.Year(),curr.Time());
            }
            else pos[1]--;
        }
        else if(ch == 'd'){
            if(pos[1] + 1 > 6){
                pos[1] = 0;
                if(curr.Month() + 1 > 12)
                    curr.SetDate(curr.Day(),1,curr.Year() + 1,curr.Time());
                else
                    curr.SetDate(curr.Day(),curr.Month()+1,curr.Year(),curr.Time());
            }
            else pos[1]++;
        }
        else if(ch == 13){
            int d = grid[pos[0]][pos[1]];
            if(d == 0 ||
               Date::Compare(Date(d,curr.Month(),curr.Year(),"12:00"),orig) > 0){
                cout<<"\n\n\tPLEASE ENTER A VALID DATE";
                getch();
                continue;
            }
            return Date(grid[pos[0]][pos[1]],curr.Month(),curr.Year(),"12:00");
        }
        else {
            cout<<"\n\tPlease enter a valid key. Ensure that CAPS lock\n";
            cout<<"\thas been disabled";
            getch();
        }
    }while(1);
}



/// ---- TRAIN CLASS --------------------------------------------

class Train {
    int train_no;
    char name[30];
    char type;        /// CHAIR CAR(C), SLEEPER(S)
    int days_running[7]; /// 1 ON DAYS WHEN TRAIN IS AVAILABLE, 0 WHEN NOT

    char origin[30];
    char destination[30];
    int capacity[3];  /// FIRST, SECOND, THIRD CLASS
    float fare[3];    /// FIRST, SECOND, THIRD CLASS
    int num_seats_booked;
public:

    int init; /// CHECK INITIALISATION WITH REAL VALUES

    /// CONSTRUCTORS
    Train() {
        init = 0;
        num_seats_booked = 0;
        train_no = 0;
        strcpy(name,"NULL");
        strcpy(origin,"NULL");
        strcpy(destination,"NULL");
    }

    /// ACCESSORS / MODIFYERS

    void SetFare(float a, float b, float c){
        fare[0] = a; fare[1] = b; fare[2] = c;
    }
    void SetCapacity(int a,int b,int c) {
        capacity[0] = a; capacity[1] = b; capacity[2] = c;
    }
    void SetStations(char* o, char* d){
        strcpy(origin,o);
        strcpy(destination,d);
    }
    void SetDays(int a[]){
        for(int i=0; i<7; ++i){
            days_running[i] = a[i];
        }
    }
    void SetType(char t){ type = t; }
    void SetName(char* n){ strcpy(name,n); }
    void SetTrainNo(int t){ train_no = t; init = 1; }

    float* Fare() { return fare; }
    int* Capacity() { return capacity; }
    char* Origin(){ return origin; }
    char* Destination() { return destination; }
    int* DaysRunning(){ return days_running; }
    char Type(){ return type; }
    char* Name(){ return name; }
    int TrainNo(){ return train_no; }


    /// DISPLAY ANS READ

    void Display();
    void DisplayS();   /// DISPLAY (SMALL) MINIMAL DATA
};

void Train::Display()
{
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    cout<<"\n\tTRAIN NUMBER : "<<train_no;
    cout<<"\n\tTRAIN NAME   : "<<name;
    cout<<"\n\tORIGIN       : "<<origin;
    cout<<"\n\tDESTINATION  : "<<destination;
    cout<<"\n\tTYPE         : "; (type == 'S')?cout<<"SLEEPER" : cout<<"CHAIRCAR";
    cout<<"\n\n--------------------------------------------------------------------------------";
}

void Train::DisplayS()
{
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    cout<<"\n\tTRAIN NUMBER : "<<train_no;
    cout<<"\n\tTRAIN NAME   : "<<name;
    cout<<"\n\n--------------------------------------------------------------------------------";
}
/// ---- ACCOUNT CLASS -------------------------------------------

class Account {
    char username[30];
    char password[30];
    char name[30];
    int num_tickets;
public:

    int init; /// CHECK INITIALISATION WITH REAL VALUES

    /// CONSTRUCTORS
    Account() {
        init = 0;
        strcpy(username,"NULL");
        strcpy(password,"NULL");
        strcpy(name,"NULL");
    }
    Account(char u[], char p[], char n[]){
        init = 1;
        strcpy(username,u);
        strcpy(password,p);
        strcpy(name,n);
    }

    /// ACCESSORS / MODIFIERS
    void SetDetails(char u[], char p[], char n[]){
        init = 1;
        strcpy(username,u);
        strcpy(password,p);
        strcpy(name,n);
    }

    char* Username() { return username; }
    char* Password() { return password; }
    char* Name() { return name; }

    /// DISPLAY FUNCTION AND READ

    void Display();
};

void Account::Display()
{
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    cout<<"\n\tUSERNAME       : "<<username;
    cout<<"\n\tPASSWORD       : "<<password;
    cout<<"\n\tNAME           : "<<name;
    cout<<"\n\n--------------------------------------------------------------------------------";
}


/// ---- TICKET CLASS --------------------------------------------

class Ticket {

    int t_number;
    Account owner;
    Train train_details;
    Date journey_date;
    int seat_class;
public:

    int init; /// CHECK INITIALISATION WITH REAL VALUES

    /// CONSTRUCORS

    Ticket()
    {
        init = 0;
    };
    Ticket(Account o, Train f, Date d, int c){
        owner = o;
        train_details = f;
        journey_date = d;
        seat_class = c;
    }

    /// MODIFIERS / ACCESSORS

    void setDetails(Account o, Train f, Date d, int c){
        owner = o;
        journey_date = d;
        train_details = f;
        seat_class = c;
    }
    Account Owner() { return owner; }
    Train Train_det() { return train_details; }
    int TNumber() { return t_number; }
    Date JDate() { return journey_date; }
    int SClass() { return seat_class; }
    void SetTicketNo(int x) { t_number = x; init = 1; }

    /// DISPLAY AND READ

    void Display();
};

void Ticket::Display()
{
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    cout<<"\n\tTICKET NUMBER : "<<t_number;
    cout<<"\n\tNAME          : "<<owner.Name();
    cout<<"\n\tTRAIN NUMBER  : "<<train_details.TrainNo();
    cout<<"\n\tORIGIN        : "<<train_details.Origin();
    cout<<"\n\tDESTINATION   : "<<train_details.Destination();
    cout<<"\n\tDATE          : "; journey_date.Display();
    cout<<"\n\tCLASS         : "<<seat_class;
    cout<<"\n\tFARE          : "<<train_details.Fare()[seat_class - 1];
    cout<<"\n\n--------------------------------------------------------------------------------";
}

/// ---- DATABASE MANAGEMENT SYSTEM ------------------------------

class DatabaseManager {

public:
    static void Write(Account);
    static void Write(Ticket);
    static void Write(Train);
    static Account ReadAccount(char*);
    static Train ReadTrain(int);
    static Ticket ReadTicket(int);
    static int Delete(Account);
    static int Delete(Train);
    static int Delete(Ticket);
    static int DisplayTrain(char*,char*,Date);
    static void FindSeatsBooked(int,Date,int[]);
    static int DisplayTicket(Account);
    static int DisplayAccount();
};

int DatabaseManager::DisplayAccount()
{
    int c = 0;
    ifstream fin;
    fin.open(account_dir,ios::binary);
    Account temp;
    while(fin.read((char*)&temp, sizeof(temp))){
        temp.Display();
        c++;
    }
    fin.close();
    return c;
}

int DatabaseManager::DisplayTicket(Account a)
{
    Ticket temp;
    int c = 0;
    ifstream fin;
    fin.open(ticket_dir,ios::binary);
    while(fin.read((char*)&temp,sizeof(temp))){
        if(strcmp(temp.Owner().Username(),a.Username()) == 0){
            temp.Display();
            c++;
        }
    }
    fin.close();

    return c;
}

void DatabaseManager::FindSeatsBooked(int tno, Date d, int s[])
{
    ifstream fin;
    fin.open(ticket_dir, ios::binary);
    Ticket temp;
    s[0] = 0;
    s[1] = 0;
    s[2] = 0;
    while(fin.read((char*)&temp,sizeof(temp))){
        if(Date::Compare(temp.JDate(), d) == 0 &&
           temp.Train_det().TrainNo() == tno){
                s[temp.SClass() - 1]++;
           }
    }
    fin.close();
}

int DatabaseManager::DisplayTrain(char* ori, char* des, Date dep)
{
    system("cls");
    Train temp;
    int day_of_week = Calendar::DayFromDate(dep), c = 0;
    ifstream fin;
    fin.open(train_dir,ios::binary);

    while(fin.read((char*)&temp,sizeof(temp))){
        if(strcmpi(temp.Origin(),ori) == 0 &&
           strcmpi(temp.Destination(),des) == 0 &&
           temp.DaysRunning()[day_of_week]) {

            temp.DisplayS();
            c++;
        }
    }
    fin.close();

    return c;
}

int DatabaseManager::Delete(Train t)
{
    ifstream fin;
    ofstream fout;
    Train temp;
    int found = 0;
    fin.open(train_dir,ios::binary);
    fout.open("temp.txt", ios::binary);
    while(fin.read((char*)&temp,sizeof(temp)))
    {
        if(temp.TrainNo() == t.TrainNo()){
            found = 1;
            continue;
        }
        fout.write((char*)&temp,sizeof(temp));
    }

    fin.close();
    fout.close();

    remove(train_dir);
    rename("temp.txt",train_dir);

    return found;
}

int DatabaseManager::Delete(Account a)
{
    ifstream fin;
    ofstream fout;
    int found = 0;
    fin.open(account_dir,ios::binary);
    fout.open("temp.txt", ios::binary);
    Account temp;
    while(fin.read((char*)&temp,sizeof(temp)))
    {
        if(strcmp(a.Username(),temp.Username()) == 0){
            found = 1;
            continue;
        }
        fout.write((char*)&temp,sizeof(temp));
    }

    fin.close();
    fout.close();

    remove(account_dir);
    rename("temp.txt",account_dir);

    return found;
}

int DatabaseManager::Delete(Ticket t)
{
    ifstream fin;
    ofstream fout;
    Ticket temp;
    int found = 0;
    fin.open(ticket_dir,ios::binary);
    fout.open("temp.txt",ios::binary);
    while(fin.read((char*)&temp,sizeof(temp)))
    {
        if(temp.TNumber() == t.TNumber()){
            found = 1;
            continue;
        }
        fout.write((char*)&temp,sizeof(temp));
    }

    fin.close();
    fout.close();

    remove(ticket_dir);
    rename("temp.txt",ticket_dir);

    return found;
}

Account DatabaseManager::ReadAccount(char* u)
{
    Account temp, ret;
    ifstream fin;
    fin.open(account_dir,ios::binary | ios::in);
    fin.seekg(0);
    while(fin.read((char*)&temp,sizeof(temp)))
    {
        if(strcmp(u,temp.Username())==0)
        {
            ret = temp;
            break;
        }
    }
    fin.close();
    return ret;
}

Train DatabaseManager::ReadTrain(int tno)
{
    Train temp,ret;
    ifstream fin;
    fin.open(train_dir,ios::binary);
    while(fin.read((char*)&temp,sizeof(temp))){
        if(temp.TrainNo() == tno){
            ret = temp;
            break;
        }
    }
    fin.close();
    return ret;
}

Ticket DatabaseManager::ReadTicket(int t_no)
{
    Ticket temp,ret;
    ifstream fin;
    fin.open(ticket_dir,ios::binary | ios::in);
    while(fin.read((char*)&temp,sizeof(temp)))
    {
        if(temp.TNumber() == t_no)
        {
            ret = temp;
            break;
        }
    }
    fin.close();
    return ret;
}

void DatabaseManager::Write(Account a)
{
    ofstream fout;
    fout.open(account_dir,ios::binary | ios::app | ios::out);
    fout.write((char*)&a,sizeof(a));
    fout.close();
}

void DatabaseManager::Write(Train t)
{
    ofstream fout;
    fout.open(train_dir,ios::binary | ios::app);
    fout.write((char*)&t,sizeof(t));
    fout.close();
}

void DatabaseManager::Write(Ticket t)
{
    ofstream fout;
    fout.open(ticket_dir,ios::binary | ios::app);
    fout.write((char*)&t,sizeof(t));
    fout.close();
}

/// ---- MAIN RESERVATION SYSTEM ---------------------------------

class ReservationSystem{
public:
    static int CreateAccount(char*, char*, char*);
    static int ValidateCredentials(char*, char*);
    static void BookTrain(Account);

    /// ADMIN FEATURES

    static void CreateTrain();
    static void ShowTrains();

};

int ReservationSystem::ValidateCredentials(char* u, char* p)
{
    Account temp = DatabaseManager::ReadAccount(u);
    if(strcmp(p,temp.Password()) == 0){
        return 1;
    }
    else return 0;
}

int ReservationSystem::CreateAccount(char* u, char* p, char* n)
{
    Account temp;
    temp = DatabaseManager::ReadAccount(u);
    if(temp.init == 0){
        Account newAccount(u,p,n);
        DatabaseManager::Write(newAccount);
        return 1;
    }

    return 0;
}

void ReservationSystem::CreateTrain()
{
    system("cls");
    cout<<"\n\n\t\t\tCREATE TRAIN";
    cout<<"\n\n--------------------------------------------------------------------------------\n";


      //  CODE TO ACCEPT TRAIN DATA GOES HERE.

        Train STrain;

        int trainNo,running_days[7],Sfirst,Ssecond,Sthird;
        char trainName[25],typ,orig[25],destin[25];
        float Cfirst,Csecond,Cthird;


        cout<<"\n\tEnter the Train Details: \n";

        cout<<"\n\tTrain Name     : ";
        cin>>trainName;
        cout<<"\n\tTrain type     : ";
        cin>>typ;
        cout<<"\n\tTrain Number   : ";
        cin>>trainNo;
        if(DatabaseManager::ReadTrain(trainNo).init){
            cout<<"\n\tThis train number already exits.. Please try again..";
            system("pause");
            return;
        }
        cout<<"\n\tStarting point : ";
        cin>>orig;
        cout<<"\n\tDestination    : ";
        cin>>destin;
        cout<<"\n\tWeek days on which the train is running \n";
        cout<<"\t(Enter 0-Not running,1-running starting from Monday for next 6 days): ";

        for(int i=0;i<7;i++)
        {
             cin>>running_days[i];
             cout<<"/"; /// Format <mon/tue/wed/...../sun>
        }

        cout<<"\n\tEnter number of seats for first,second and third class: ";
        cin>>Sfirst>>Ssecond>>Sthird;

        cout<<"\n\tEnter fare for first,second and third class tickets(decimals): ";
        cin>>Cfirst>>Csecond>>Cthird;

        cout<<"\n\n\tTrain is successfully scheduled.\n";

        system("pause");


        STrain.SetFare(Cfirst,Csecond,Cthird);
        STrain.SetCapacity(Sfirst,Ssecond,Sthird);
        STrain.SetStations(orig,destin);
        STrain.SetDays(running_days);
        STrain.SetType(typ);
        STrain.SetName(trainName);
        STrain.SetTrainNo(trainNo);

       // CODE TO WRITE THE TRAIN OBJECT CREATED ONTO THE FILE..

        DatabaseManager::Write(STrain);
}

void ReservationSystem::ShowTrains()
{
    Train temp;
    ifstream fin(train_dir, ios::binary);
    system("cls");
    cout<<"\n\n\t\tTRAIN TABLE";
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    cout<<endl<<setw(20)<<"TRAIN NAME |"<<" SUN | MON | TUE | WED | THU | FRI | SAT | TRAIN No |\n\n";
    while(fin.read((char*)&temp, sizeof(temp))){
        cout<<"\n"<<setw(18)<<temp.Name()<<" |";
        for(int i=0; i<7; ++i){
            cout<<setw(4)<<temp.DaysRunning()[i]<<" |";
        }
        cout<<" "<<setw(8)<<temp.TrainNo()<<" |";
    }
    getch();
}

void ReservationSystem::BookTrain(Account a)
{
    char des[30],ori[30], ch;
    Date curr, sel;
    int tno;
    Train tr;

    do{
        system("cls");
        cout<<"\n\n\t\t\tTICKET BOOKING";
        cout<<"\n\n--------------------------------------------------------------------------------\n";
        cout<<"\n\tFROM  : ";
        cin>>ori;
        cout<<"\n\tTO    : ";
        cin>>des;

        time_t now = time(0);
        tm* t = localtime(&now);
        curr.SetDate(t->tm_mday,t->tm_mon+1,t->tm_year+1900,"12:00");

        Calendar::Display(curr);
        sel = Calendar::Select(curr);

        int e = DatabaseManager::DisplayTrain(ori,des,sel);
        if(e == 0){
            cout<<"\n\n\t\tSORRY ! NO TRAINS EXIST ..";
            getch();
            continue;
        }
        else break;
    }while(1);
    do{
        DatabaseManager::DisplayTrain(ori,des,sel);
        cout<<"\n\n\t\tDO YOU WANT TO BOOK A TRAIN ? (Y/n) : ";
        cin>>ch;
        if(ch == 'n' || ch == 'N') break;
        cout<<"\n\t\tPLEASE ENTER TRAIN NO. OF THE TRAIN TO BOOOK : ";
        cin>>tno;
        tr = DatabaseManager::ReadTrain(tno);
        if(tr.init == 0){
            cout<<"\n\t\tPLEASE ENTER VALID NUMBER.";
            getch();
            continue;
        }

        int c;
        int filled[3];
        DatabaseManager::FindSeatsBooked(tr.TrainNo(),sel,filled);
        int* capacity = tr.Capacity();

        if(filled[0] + filled[1] + filled[2] >= capacity[0] + capacity[1] + capacity[2]){
            cout<<"\n\n\t\tSORRY ! THIS TRAIN IS FULLY BOOKED ";
            getch();
            continue;
        }

        do{
            system("cls");
            cout<<"\n\n\t\t\tSELECT CLASS";
            cout<<"\n\n--------------------------------------------------------------------------------\n";
            cout<<"\n\t\tCLASS          "<<"    FARE   "<<"    SEATS FILLED    "<<"    CAPACITY    \n";
            cout<<"\n\t\tFIRST CLASS  : "<<setw(11)<<tr.Fare()[0]<<setw(16)<<filled[0]<<setw(12)<<capacity[0];
            cout<<"\n\t\tSECOND CLASS : "<<setw(11)<<tr.Fare()[1]<<setw(16)<<filled[1]<<setw(12)<<capacity[1];
            cout<<"\n\t\tTHIRD CLASS  : "<<setw(11)<<tr.Fare()[2]<<setw(16)<<filled[2]<<setw(12)<<capacity[2];
            cout<<"\n\n\t\tPLEASE SELECT CLASS (1/2/3) : ";
            cin>>c;
            if(c != 1 && c!= 2 && c != 3){
                cout<<"\n\n\t\tPLEASE ENTER VALID INPUT";
                getch();
                continue;
            }
            else if(filled[c-1] + 1 > tr.Capacity()[c-1]){
                cout<<"\n\n\t\tSORRY !";
                cout<<"\n\t\tTHIS CLASS IS FULLY BOOKED !";
                getch();
            }
            else break;
        }while(1);

        do{

            cout<<"\n\n\t\tDO YOU WANT TO BOOK THIS TICKET (Y/n) ? : ";
            cin>>ch;
            if(ch == 'Y' || ch =='y'){
                srand(time(NULL));
                int x = rand()%1000;
                while(DatabaseManager::ReadTicket(x).init == 1){
                    x = rand()%1000;
                }
                Ticket temp(a,tr,sel,c);
                temp.SetTicketNo(x);
                DatabaseManager::Write(temp);

                system("cls");
                cout<<"\n\n\t\tYOUR TICKET IS BOOKED !";
                cout<<"\n\n\t\tPLEASE PAY THE FARE ON REACHING THE TRAIN STATION..";
                temp.Display();
                getch();
                break;
            }
            else if(ch == 'N' || ch == 'n') break;
            else {
                cout<<"\n\n\t\tENTER VALID INPUT";
                continue;
            }

        }while(1);

        break;

    }while(1);

}

/// ---- USER INTERFACE MANAGER ----------------------------------

class UIManager {
    static char* curName;
    static Option* curEntries;
    static int curSize;
    static int curSelection;
    static char Selector[2];
public:
    static void Display();
    static void SetMenu(char*, Option*, int);
    static void RunMenu();
    static void GetPassword(char*);
};

char* UIManager::curName = NULL;
Option* UIManager::curEntries = NULL;
int UIManager::curSelection = 0;
int UIManager::curSize = 0;
char UIManager::Selector[] = {'>','<'};

void UIManager::Display()
{
    cout<<endl<<endl;
    cout<<"\t\t"<<curName<<"\n\n";
    cout<<"--------------------------------------------------------------------------------\n\n";
    for(int i=0; i<curSize; ++i){
        if(i == curSelection) {
            cout<<"\n\t\t\t";
            cout<<Selector[0]<<Selector[0];
            cout<<"      "<<curEntries[i].Name<<"      "<<Selector[1]<<Selector[1]<<endl;
        }
        else {
            cout<<"\n\t\t\t\t";
            cout<<curEntries[i].Name<<endl;
        }
    }
    cout<<"\n--------------------------------------------------------------------------------\n\n";
    cout<<"\tEnter 'w' to move up, 's' to move down, 'enter' to select.";
}

void UIManager::SetMenu(char* heading, Option* entries, int s)
{
    curName = heading;
    curEntries = entries;
    curSize = s;
    curSelection = 0;
}

void UIManager::RunMenu()
{
    char ch;
    do{
        system("cls");
        Display();
        ch = getch();
        if(ch == 119){
            (curSelection == 0)?curSelection = curSize-1:curSelection--;
        }
        else if (ch == 115){
            curSelection = (curSelection + 1)%curSize;
        }
        else if(ch == 13) {
            if(curSelection == curSize - 1){
                break;
            }
            curEntries[curSelection].f();
        }
        else {
            cout<<"\n\tPlease enter a valid key. Ensure that CAPS lock\n";
            cout<<"\thas been disabled";
            getch();
        }
    }while(1);
}

void UIManager::GetPassword(char* p)
{
    char ch;
    int i=0;
    do{
        ch = getch();
        if(ch !=  13){
            p[i++] = ch;
            cout<<'*';
        }
    }while(ch != 13);
}

/// --------------------------------------------------------------

/// MAIN APPLICATION ---------------------------------------------

class Application
{
    static Account curr_user;
    static Option StartMenu[];
    static Option LoginMenu[];
    static Option UserMenu[];
    static Option AdminMenu[];
    static Option UserDetMenu[];
public:
    static void StartApplication();
    static void LoginPage();
    static void UserLogin();
    static void AdminLogin();
    static void Signup();
    static void UserHome();
    static void AdminPage();
    static void BookingPage();
    static void UserDetails();
    static void BookedTickets();
    static void CancelTicket();
    static void ChangePassword();
    static void ViewAccounts();
};

Account Application::curr_user = Account();

Option Application::StartMenu[] = {
    "LOGIN", Application::LoginPage,
    "SIGN UP", Application::Signup,
    "EXIT"
};

Option Application::LoginMenu[] = {
    "USER", Application::UserLogin,
    "ADMIN", Application::AdminLogin,
    "BACK"
};

Option Application::UserMenu[] = {
    "BOOK TICKET", Application::BookingPage,
    "VIEW DETAILS", Application::UserDetails,
    "CANCEL TICKET", Application::CancelTicket,
    "LOGOUT"
};

Option Application::UserDetMenu[] = {
    "TICKETS BOOKED", Application::BookedTickets,
    "CHANGE PASSWORD", Application::ChangePassword,
    "BACK"
};

Option Application::AdminMenu[] = {
    "CREATE TRAIN", ReservationSystem::CreateTrain,
    "TRAIN TABLE", ReservationSystem::ShowTrains,
    "VIEW ACCOUNTS", Application::ViewAccounts,
    "LOGOUT"
};

void Application::ViewAccounts()
{
    system("cls");
    cout<<"\n\n\t\t\tACCOUNTS";
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    if(!DatabaseManager::DisplayAccount()){
        cout<<"\n\n\t\tNO ACCOUNTS TO DISPLAY";
    }
    getch();
}

void Application::UserDetails()
{
    UIManager::SetMenu("\t\tDETAILS",UserDetMenu,MenuSize(UserDetMenu));
    UIManager::RunMenu();
    Application::UserHome();
}

void Application::ChangePassword()
{
    system("cls");
    char curr_pass[30],new_pass[30];
    cout<<"\n\n\t\t\tCHANGE PASSWORD";
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    cout<<"\n\t\tENTER CURRENT PASSWORD : ";
    cin>>curr_pass;
    if(strcmp(curr_user.Password(),curr_pass) != 0){
        cout<<"\n\t\tWRONG PASSWORD ! ";
        getch();
        return;
    }
    cout<<"\n\t\tENTER NEW PASSWORD : ";
    cin>>new_pass;
    cout<<"\n\t\tYOUR PASSWORD HAS BEEN CHANGED";
    getch();
    curr_user.SetDetails(curr_user.Username(),new_pass,curr_user.Name());
    DatabaseManager::Delete(curr_user);
    DatabaseManager::Write(curr_user);
}

void Application::CancelTicket()
{
    char ch;
    int t;
    Ticket temp;
    do{
        system("cls");
        cout<<"\n\n\t\t\tCANCEL TICKET";
        cout<<"\n\n--------------------------------------------------------------------------------\n";
        cout<<"\n\t\tDO YOU WANT TO CANCEL TICKET (Y/n) ? : ";
        cin>>ch;
        if(ch == 'n' || ch == 'N')
            break;
        else if(ch != 'y' && ch != 'Y'){
            cout<<"\n\tPLEASE ENTER VALID INPUT";
            getch();
            continue;
        }
        cout<<"\n\t\tPLEASE ENTER TICKET NUMBER OF TICKET TO CANCEL : ";
        cin>>t;
        temp = DatabaseManager::ReadTicket(t);
        if(temp.init == 0){
            cout<<"\n\tPLEASE ENTER VALID TICKET NUMBER..";
            getch();
            continue;
        }
        else if(strcmp(temp.Owner().Username(),curr_user.Username()) != 0){
            cout<<"\n\tPLEASE ENTER VALID TICKET NUMBER..";
            getch();
            continue;
        }
        else {
            temp.Display();
            cout<<"\n\n\tDO YOU WANT TO CANCEL THIS TICKET ? (Y/n) : ";
            cin>>ch;
            if(ch != 'Y' && ch != 'y'){
                continue;
            }
            DatabaseManager::Delete(temp);
            cout<<"\n\n\t\tYOUR TICKET HAS BEEN CANCELED";
            getch();
        }

        break;
    }while(1);
}

void Application::BookedTickets()
{
    system("cls");
    cout<<"\n\n\t\t\tUSER DETAILS";
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    if(!DatabaseManager::DisplayTicket(curr_user)){
        cout<<"NO TICKETS BOOKED ! ";
    }
    getch();
    return;
}

void Application::BookingPage()
{
    ReservationSystem::BookTrain(curr_user);
    Application::UserHome();
}

void Application::AdminPage()
{
    UIManager::SetMenu("\t\tHELLO ADMIN",Application::AdminMenu,MenuSize(Application::AdminMenu));
    UIManager::RunMenu();
    Application::LoginPage();
}

void Application::StartApplication()
{
    UIManager::SetMenu("\tTRAIN RESERVATION SYSTEM",
                       Application::StartMenu, MenuSize(Application::StartMenu));
    UIManager::RunMenu();
    exit(0);
}

void Application::LoginPage()
{
    UIManager::SetMenu("\t\tLOGIN",Application::LoginMenu,MenuSize((Application::LoginMenu)));
    UIManager::RunMenu();
    Application::StartApplication();
}

void Application::UserHome()
{
    char greeting[40] = "\t\tHELLO ";
    strcat(greeting, curr_user.Name());

    UIManager::SetMenu(greeting,Application::UserMenu,MenuSize(Application::UserMenu));
    UIManager::RunMenu();
    Application::LoginPage();
}

void Application::UserLogin()
{
    system("cls");
    char u[30] = "";
    char p[30] = "";
    cout<<"\n\n\t\t\tUSER LOGIN";
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    cout<<"\n\tUSERNAME : ";
    cin>>u;
    cout<<"\n\tPASSWORD : ";
    UIManager::GetPassword(p);
    if(ReservationSystem::ValidateCredentials(u,p)){
        curr_user = DatabaseManager::ReadAccount(u);
        Application::UserHome();
    }
    else {
        cout<<"\n\n\tCREDENTIALS INVALID..., PLEASE TRY AGAIN.";
        getch();
    }
    Application::LoginPage();
}

void Application::Signup()
{
    system("cls");
    char u[30], p[30], n[30];
    cout<<"\n\n\t\t\tUSER SIGN-UP";
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    cout<<"\n\tNAME     : ";
    cin.getline(n,30);
    cout<<"\n\tUSERNAME : ";
    cin.getline(u,30);
    cout<<"\n\tPASSWORD : ";
    cin.getline(p,30);
    if(ReservationSystem::CreateAccount(u,p,n)){
        cout<<"\n\n\tYOUR ACCOUNT HAS BEEN CREATED";
        getch();
    }
    else{
        cout<<"\n\n\tTHIS NAME HAS BEEN TAKEN, PLEASE TRY AGAIN";
        getch();
    }
    Application::StartApplication();
}

void Application::AdminLogin()
{
    system("cls");
    char p[30] = "";
    cout<<"\n\n\t\t\tADMIN LOGIN";
    cout<<"\n\n--------------------------------------------------------------------------------\n";
    cout<<"\n\tPASSWORD : ";
    UIManager::GetPassword(p);
    if(strcmp(p,"ThisIsRoot") == 0){
        Application::AdminPage();
    }
    else {
        cout<<"\n\n\tCREDENTIALS INVALID..., PLEASE TRY AGAIN.";
        getch();
    }
    Application::LoginPage();
}

/// --------------------------------------------------------------

int main()
{
    //Application::StartApplication();
    Train t;
    t.SetTrainNo(461732);
    DatabaseManager::Delete(t);
}
