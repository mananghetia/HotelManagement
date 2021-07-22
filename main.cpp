#include <bits/stdc++.h>
using namespace std;
#define type 3/*defining macros so that it can easily be changed and more readable*/
#define type1price 2000/*Single*/
#define type1room 10 /*1001 1002 ... 1010*/
#define type2price 3500/*Duo*/
#define type2room 10/*2001 2002 ... 2010*/
#define type3price 5000/*Quad*/
#define type3room 5/*3001 3002 ... 3005*/
const string password="1234";/*password for getting official data*/
struct reserved_days/*Data structure to compare two pairs of date(starting and ending) for knowing the vacancy of room during particular duration containing valid set pairs of day*/
{
    int st_day,st_month,st_year;
    int end_day,end_month,end_year;
    int no_of_days;
    bool operator<(const reserved_days &t) const/*operator overloading: it checks a pair of date is always less (in terms of time) than other pair of date,it will help into sorting of data so that it can be search easily by using find function of set stl*/
    {/*by defining this operator we implicitely define that two reserved_days object are equal if their is collisions between two pairs of date,or in simple words both have some atleast 1 day in common*/
        if(end_year<t.st_year)
            return true;
        else if(end_year==t.st_year)
        {
            if(end_month<t.st_month)
                return true;
            else if(end_month==t.st_month)
            {
                if(end_day<t.st_day)
                    return true;
            }
        }
        return false;
    }
    bool varify_date()/*verify input date according to gregorian calendar and also verify start date is lower than end date*/
    {
        bool a=false,b=false;
        if(st_year>0)/*verifing start date*/
        {
            switch(st_month)
            {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:{if(st_day>0&&st_day<=31)a=true; break;}
                case 2:{
                    if(st_year%400==0||(st_year%100!=0&&st_year%4==0))//leap year
                        if(st_day>0&&st_day<=29)
                        a=true;
                    if(st_day>0&&st_day<29)//not leap year
                        a=true;
                    break;
                }
                case 4:
                case 6:
                case 9:
                case 11:{if(st_day>0&&st_day<31)a=true; break;}
                default : break;
            }
        }
        if(a!=true)
            return false;
        if(end_year>0)/*verifing end date*/
        {
            switch(end_month)
            {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:{if(end_day>0&&end_day<=31)b=true; break;}
                case 2:{
                    if(end_year%400==0||(end_year%100!=0&&end_year%4==0))//leap year
                        if(end_day>0&&end_day<=29)
                        b=true;
                    if(end_day>0&&end_day<29)//not leap year
                        b=true;
                    break;
                }
                case 4:
                case 6:
                case 9:
                case 11:{if(end_day>0&&end_day<31)b=true; break;}
                default : break;
            }
        }
        if(b!=true)
            return false;
        if(end_year>st_year) /*verifing start date is lower than end date*/
            return true;
        else if(end_year==st_year)
        {
            if(end_month>st_month)
                return true;
            else if(end_month==st_month)
            {
                if(end_day>st_day)
                    return true;
            }
        }
        return false;
    }
    void calculate_days()/*function to calculate accurate no. of days, used for calculating bill amount*/
    {/*using from <ctime> library declared in time.h*/
        struct tm x = {0,0,12,st_day,st_month,st_year-1900};
        struct tm y = {0,0,12,end_day,end_month,end_year-1900};
        time_t start_time = mktime(&x);
        time_t end_time = mktime(&y);
        if ( start_time != (time_t)(-1) && end_time != (time_t)(-1) )
        {
            no_of_days = ceil(difftime(end_time,start_time) / (60 * 60 * 24));/*difftime function return difference in seconds which is converted into days*/
        }
    }
};
struct room/*Data Structure for storing a room of a particular type in form of linkedlist*/
{
    int room_no;
    int room_type;
    double price_per_day;
    set<reserved_days> schedule;/*stl set: used to store reserved dates and also used to check vacany of this particular room for a given duration(pair of dates)*/
    struct room * next_room;
    room (int room_no,double price_per_day,int room_type)/*constructor: called in build_hotelfunction to intiliased default value for every room*/
    {
        this->room_no=room_no;
        this->price_per_day=price_per_day;
        this->room_type=room_type;
        next_room=NULL;
    }
}*total_rooms[type];/*an array of pointer of room to store starting position of rooms of particular type*/
struct guest/*Data structure to store details of the guest in form of linked list*/
{
    string guest_name;
    string guest_ID;
    int no_of_guest;
    struct reserved_days duration;
    vector<room *> rooms;/*vector stl:rooms which are reserved by this guest*/
    struct guest *next;
}*checkedIn_guests,*past_guests,*reserved_guests;/*storing data in different linked list will be efficient for performing different task by hotel management crew, like cooking food,retriving past guest contact etc.*/
void seperator(int n)/*function used to enhance readability of output in console by displaying a '-' line*/
{
    if(n==0)
        return;
    for(int i=0;i<100;i++)
        cout<<"-";
    cout<<endl;
    seperator(n-1);
}
void space(int n,char t)/*function used to enhance readability of output in console by appending spaces or characters*/
{
    for(int i=1;i<=n;i++)
        cout<<t;
}
void welcome()/*function used to display hotel name*/
{
    seperator(1),space(34,'-'),cout<<"<<<<< Welcome to Hotel CSE >>>>>",space(34,'-'),cout<<endl,seperator(1);
}
void hotel_package()
{   /*display types of room varieties offered by hotel*/
    cout<<"Type of room facilities offered by us\n";
    seperator(1);
    cout<<"Type 1: Single room  price="<<type1price<<" Rs per day\n";
    cout<<"Type 2: Double room  price="<<type2price<<" Rs per day\n";
    cout<<"Type 3: Quad   room  price="<<type3price<<" Rs per day\n";
    seperator(1);
}
void build_hotel()
{   /* foundation of hotel, pricing and total no of rooms with particular types*/
    total_rooms[0] = new struct room(1001,type1price,1);
    struct room *t=total_rooms[0];
    for(int i=2;i<=type1room;i++)
    {
            struct room *q=new struct room(1000+i,type1price,1);
            t->next_room=q;
            t=q;
    }
    total_rooms[1] = new struct room(2001,type2price,2);
    t=total_rooms[1];
    for(int i=2;i<=type2room;i++)
    {
            struct room *q=new struct room(2000+i,type2price,2);
            t->next_room=q;
            t=q;
    }
    total_rooms[2] = new struct room(3001,type3price,3);
    t=total_rooms[2];
    for(int i=2;i<=type3room;i++)
    {
            struct room *q=new struct room(3000+i,type3price,3);
            t->next_room=q;
            t=q;
    }
}
vector<room *> availability_check(struct room * rooms,struct reserved_days days)
{   /*utility function for reserving rooms doing purpose of returning available rooms in row(so that guest can get nearer rooms afterwards) during "days" duration of particular type*/
    struct room * cur=rooms;
    vector<room *> ans;
    while(cur!=NULL)
    {
        if((cur->schedule).size()==0||(cur->schedule).find(days)==(cur->schedule).end())
        {/*size is 0 means no reservation of cur room in future or present and hence it is available*/
            /*if (cur->schedule).find(days)==(cur->schedule).end() , that implies that no reserved days are there which will collide with these days and hence it is available*/
            ans.emplace_back(cur);
        }
        cur=cur->next_room;
    }
    return ans;
}
void reserving_rooms()
{   /*reserving rooms and filing guests details and maintaing reserved_guests list*/
    hotel_package();
    struct guest *person=new struct guest;
    struct reserved_days duration;
    person->next=NULL;
    cout<<"Please Enter necessary guest's details for room booking\n"; //detailing
    cout<<"Name :";
    getline(cin.ignore(numeric_limits<streamsize>::max(), '\n'),person->guest_name);
    cout<<"ID : ";
    getline(cin,person->guest_ID);
    cout<<"no. of peoples : ";
    cin>>person->no_of_guest;
    while(1)/*varifing entered detail by the guest*/
    {
        cout<<"Enter boarding date in dd mm yyyy format  :";
        cin>>duration.st_day>>duration.st_month>>duration.st_year;
        cout<<"Enter departure date in dd mm yyyy format :";
        cin>>duration.end_day>>duration.end_month>>duration.end_year;
        if(duration.varify_date())
            break;
            cout<<"Please Enter valid dates\n";
    }
    duration.calculate_days();/*calculating no. of days for stay*/
    person->duration=duration;
    int i,j=0,no_of_rooms[type], no_of_available_rooms[type];
    vector<room*> available_rooms[type];
    for(i=0;i<type;i++)
    {
        available_rooms[i]=availability_check(total_rooms[i],duration); /*checking available rooms of particular type;*/
        no_of_available_rooms[i]=available_rooms[i].size();/*assigning max no. of available room of particluar type*/
    }
    for(i=0;i<type;)/*entering no. of rooms*/
    {
        cout<<"Enter no. of rooms for Type "<<i+1<<":";
        cin>>no_of_rooms[i];
        if(no_of_rooms[i]>=0&&no_of_available_rooms[i]>=no_of_rooms[i]) /*bound checking for available rooms*/
        {
            if(no_of_rooms[i]>0)/*if atleat 1 room is reseverd then j==1,otherwise j==0 ,if j==0 we don't save the detail of guest at all*/
                j=1;
            i++;
        }
        else if(no_of_rooms[i]<0)/*verifing entered detail*/
        {
            cout<<"Please Enter valid no. of rooms\n\n";
        }
        else
        {
            if(no_of_available_rooms[i]!=0)
            cout<<"Sorry to inform you, currently we are very short for type "<<i+1<<" rooms. Only "<<no_of_available_rooms[i]<<" rooms are available\n\n";
            else
            cout<<"Sorry to inform you, currently all rooms of type "<<i+1<<" are booked.\n\n";
            int ans;
            cout<<"Did you wish to continue booking process.\nEnter 1 for YES or 0 for NO:";
            cin>>ans;
            if(!ans) /*if guest want more rooms and we don't have it and if they don't want to book*/
                return ;
            else
                i=0,seperator(1); /*rechoosing no. of rooms*/
        }
    }
    seperator(1);
    if(j==0) /*if no rooms are choosen*/
        return;
    for(i=0;i<type;i++)/*assigning nearer rooms to guest from available ones*/
    {
        available_rooms[i].resize(no_of_rooms[i]);
        if(person->rooms.size()==0)
        person->rooms=available_rooms[i];
        else
        person->rooms.insert(person->rooms.end(),available_rooms[i].begin(),available_rooms[i].end());
    }
    if(reserved_guests==NULL) /*maintaining list of guest by always adding at begining (for obtaining O(1) time complexity)*/
        reserved_guests=person;
    else
    {
        struct guest *temp=reserved_guests;
        reserved_guests=person;
        reserved_guests->next=temp;
    }
    cout<<"room no. ";/*display information to guest*/
    for(i=0;i<person->rooms.size();i++)
    {
        cout<<(person->rooms)[i]->room_no<<" ";
        person->rooms[i]->schedule.insert(duration);
    }
    cout<<"are reserved for you from "<<duration.st_day<<"-"<<duration.st_month<<"-"<<duration.st_year<<" to "<<duration.end_day<<"-"<<duration.end_month<<"-"<<duration.end_year<<"\n";
    seperator(1);
}
void checkIn()
{   /*assigning rooms with keys to varified guest and transferring guest from reserved_guests list to checkedIn_guest list*/
    string name,ID;
    cout<<"Enter guest's name:";
    getline(cin.ignore(numeric_limits<streamsize>::max(), '\n'),name);
    cout<<"Enter guest's ID:";
    getline(cin,ID);
    struct guest * temp=reserved_guests;
    struct guest * prev=temp;
    while(temp!=NULL)/*varification of guest's identity and searching of guest from reserved_guests list*/
    {
        if(temp->guest_name==name&&temp->guest_ID==ID)
            break;
        prev=temp;
        temp=temp->next;
    }
    if(temp==NULL)
    {
        cout<<"Sorry! Rooms are not reserved for this guest.\n";
        seperator(1);
        return ;
    }/*Removal of guest from reserved_guests list*/
    if(prev==temp)/*removal at begining*/
    {
        reserved_guests=prev->next;
    }
    else /*removal at end or middle*/
    {
        prev->next=temp->next;
    }
    temp->next=NULL;
    prev=checkedIn_guests;
    if(checkedIn_guests==NULL)/*additon in front of checkedIn_guest list*/
        checkedIn_guests=temp;
    else
    {
        temp->next=checkedIn_guests;
        checkedIn_guests=temp;
    }
    /*displaying message for guest*/
    cout<<"your rooms are reserved from "<<temp->duration.st_day<<"-"<<temp->duration.st_month<<"-"<<temp->duration.st_year<<" to "<<temp->duration.end_day<<"-"<<temp->duration.end_month<<"-"<<temp->duration.end_year<<"\n";
    cout<<"Here are your keys for Room no. ";
    for(int i=0;i<temp->rooms.size();i++)
        cout<<temp->rooms[i]->room_no<<" ";
    cout<<endl<<"Enjoy your stay\n";
    seperator(1);
}
void print_bill(struct guest *temp)
{   /*utiltiy function used by checkout for printing total amount for guest payment & removing days timeslot from reserved rooms*/
    seperator(1);
    /*printing bill information for client*/
    cout<<"from : "<<temp->duration.st_day<<"-"<<temp->duration.st_month<<"-"<<temp->duration.st_year<<"\n";
    cout<<"  to : "<<temp->duration.end_day<<"-"<<temp->duration.end_month<<"-"<<temp->duration.end_year<<"\n";
    cout<<"Days : "<<temp->duration.no_of_days<<endl;
    cout<<"name : "<<temp->guest_name<<endl;
    cout<<"  ID : "<<temp->guest_ID;
    int rt,i=0,j=-1;
    double amount=0;
    struct reserved_days d=temp->duration;
    /*display of type of room against room no. booked by guest,removing timeslot from the booked room's scheduled and calculating expanse*/
    while(i<temp->rooms.size())
    {
        struct room * t=temp->rooms[i];
        t->schedule.erase(d);
        rt=t->room_type;
        amount+=t->price_per_day;
        if(j!=-1&&j!=rt)/*end of that type of booked room*/
            cout<<">";
        if(j!=rt)
        cout<<"\nRoomType : "<<rt<<" : < ",j=rt;/*start of particular type of room*/
        cout<<t->room_no<<" ";
        i++;
    }
    cout<<">"<<endl;
    seperator(1);
    amount*=d.no_of_days;
    /*printing expanse of guest*/
    cout<<"Total amount : Rs"<<amount<<endl;
    seperator(1);
}
void checkOut()
{   /*verifing guest detail and printing bill and transfer this record from checkedIn_guests list to past_guests list*/
    struct guest * temp=checkedIn_guests;
    if(temp==NULL)
     {
         cout<<"NO customer has checkedIn to hotel and hence no checkout possible\n";
         seperator(1);
         return;
     }
     struct guest *prev =checkedIn_guests;
    string name,ID;
    cout<<"Enter guest's name:";
    getline(cin.ignore(numeric_limits<streamsize>::max(), '\n'),name);
    cout<<"Enter guest's ID:";
    getline(cin,ID);
    while(temp!=NULL)/*varification of guest's identity and searching of guest from checkedIn_guests list*/
    {
        if(temp->guest_name==name&&temp->guest_ID==ID)
            break;
        prev=temp;
        temp=temp->next;
    }
    if(temp==NULL)
    {
        cout<<"Sorry! Rooms are not reserved or not checkedIn by this guest.\n";
        seperator(1);
        return ;
    }
    /*printing bill*/
    cout<<"Please pay the require amount\n";
    print_bill(temp);
    cout<<"We hope that you enjoy our hospitality , hope to see you again\n";
    seperator(1);
    /*removal from checked_guests list*/
    if(prev==temp)//removal at begining
    {
        checkedIn_guests=prev->next;
    }
    else
    {
        prev->next=temp->next;//removal at end and middle
    }
    temp->next=NULL;
    prev=past_guests;
    /*addition of guest to past_guests list record at begininig*/
    if(past_guests==NULL)
        past_guests=temp;
    else
    {
        temp->next=past_guests;
        past_guests=temp;
    }

}
void printlist(struct guest * temp)
{
    if(temp==NULL)
    {cout<<"Empty\n";return;}
    while(temp!=NULL)
        cout<<"Name: "<<temp->guest_name<<" | ID: "<<temp->guest_ID<<" | "<<temp->no_of_guest<<" peoples"<<" | from : "<<temp->duration.st_day<<"-"<<temp->duration.st_month<<"-"<<temp->duration.st_year<<"  to : "<<temp->duration.end_day<<"-"<<temp->duration.end_month<<"-"<<temp->duration.end_year<<" | "<<endl,temp=temp->next;
        cout<<endl;
}
int main()
{
    build_hotel();
    welcome();
    int choice;
    while(1)
    {
        cout<<"Enter one of the following option"<<endl<<"1.To reserve a room\n2.To CheckIn\n3.To CheckOut\n4.Printing Past guest list\n5.Printing CheckedIn guest list\n6.Printing Reserved guest(future guest) list\n7.exit\n";
        seperator(1);
        cin>>choice;
        switch(choice)
        {
            case 1: reserving_rooms();break;
            case 2: checkIn();break;
            case 3: checkOut();break;
            case 4: /*password authorization for security purpose for option 4,5,6*/
            case 5:
            case 6: {
                string s;
                cout<<"Enter a password\n";
                cin>>s;
                if(s==password)
                {
                    cout<<"Access Granted\n";
                    seperator(1);
                    if(choice==4)
                    {
                        cout<<"Past Guest list:\n";
                        printlist(past_guests);
                    }
                    else if(choice==5)
                    {
                        cout<<"CheckedIn Guest list:\n";
                        printlist(checkedIn_guests);
                    }
                    else if(choice==6)
                    {
                        cout<<"Reserved Guest List:\n";
                        printlist(reserved_guests);
                    }
                }
                else
                    cout<<"wrong password\n";
                break;
            }
            default :return 0;
        }
        seperator(1);
    }
    return 0;
}
