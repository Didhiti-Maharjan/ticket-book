#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
struct movie{
    int mid;
    char mname[40];
    char theater[30];
    int total;
    int available;
    float price;
};
struct user{
    char username[30];
    char password[20];
};
//use for user ticket information
struct ticket{
    char name[50];
    int mobilno;
    int seatno;
    int mid;
};
void firstmain();
void displayforuser();
int read(struct user userone[MAX]);
void write(struct user userone[MAX],int usenum);
int login(struct user userone[MAX],int usenum,char username[30],char password[20]);
void signin(struct user userone[MAX],int *usenum);
void book(struct movie movieone[5],int movnum, struct ticket ticketone[400], int* ticknum, int uid);
void cancel(struct movie movieone[5],int movnum,struct ticket ticketone[400], int* ticknum, int uid);
void status(struct movie movieone[5],int numMovies,int uid);

//this function is used to display page for login or signup(thefirst page shown in program)
void firstmain(){
    printf("\n|---------------Movie Ticket Booking System---------------|\n");
    printf("1.Sign in\n");
    printf("2.Login\n");
    printf("3.Exit\n");
    printf("Enter your choice: ");
}

//this function is displayed when the user chooses to login and is successful
void displayforuser(){
    printf("\n|---------------User Menu---------------|\n");
    printf("1.Book Movie Ticket\n");
    printf("2.Cancel movie Ticket\n");
    printf("3.Check movie Status\n");
    printf("4.Logout\n");
    printf("Enter your choice: ");
}

// Function to write users to the file
void write(struct user userone[],int usenum){
    int i;
    FILE *fp=fopen("users.txt","w");
    for(i=0;i<usenum;i++){
        fprintf(fp,"%s %s\n",userone[i].username,userone[i].password);
    }
    fclose(fp);
}

//this one is used to sign in the user(a new one)
void signin(struct user userone[], int *usenum){
    char username[30];
    char password[20];
    printf("Enter your username: ");
    scanf("%s",username);
    printf("Enter the password: ");
    scanf("%s",password);
    int i;
    for (i = 0; i < *usenum;i++){//helps in preventing duplicate usernames
        if (strcmp(userone[i].username, username) == 0) {
            printf("Username already in existence please use other one!!!\n");
            return;
        }
    }
    //array userone using pointer helps in copying username and pass,(helps to add new user)
    strcpy(userone[*usenum].username,username);
    strcpy(userone[*usenum].password,password);
    (*usenum)=(*usenum)+1;
    write(userone, *usenum);
    printf("SUCCESSS REGISTRATION!!!Login can be done!!\n");
}

//this function is used to read the data of the users from the file
int read(struct user userone[]){
    FILE *fp = fopen("users.txt","r");
    if(fp == NULL) {
        return 0;
    }
    int i=0;
    //fscanf used to read from file and use those data in here
    while(fscanf(fp,"%s %s",userone[i].username,userone[i].password)!=EOF){
        i++;
    }
    fclose(fp);
    return i;
}

//this one is used for loginpage
int login(struct user userone[],int usenum,char username[],char password[]){
    int i;
    for(i = 0; i < usenum; i++){
        if(strcmp(userone[i].username, username) == 0 && strcmp(userone[i].password, password) == 0){
            return i;
            //returning i means 0 or other positive number, this then is used as loggedinid
        }
    }
    return -1;
    //returning -1 means loggedinid becoming -1(failed to login)
}

//this function books the ticket(display movies,find movies,check seats,update seats)
void book(struct movie movieone[],int movnum, struct ticket ticketone[], int* ticknum, int uid){
    printf("%-10s%-20s%-27s%-28s%-30s%-10s\n", "ID", "Movie Name", "Theatre", "Total seat", "Available seat", "Price");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    for(int p = 0; p < movnum; p++){
        printf("%-10d%-20s%-27s%-28d%-30d%-10f\n",movieone[p].mid,movieone[p].mname,movieone[p].theater,movieone[p].total,movieone[p].available,movieone[p].price);
    }
    printf("\nEnter Movie ID: ");
    int mid;
    scanf("%d",&mid);
    int mindex = -1;//initially -1(can change according to next step)
    for(int i = 0; i < movnum; i++){//movnum is 5(as total movies =5)
        if(movieone[i].mid == mid){//if any of the movie id match the given movie id
            mindex = i;//index change from -1 
            break;
        }
    }
    if(mindex == -1){//meaning id not found so index is -1
        printf("Movie not found!!! \n");
    } 
    else if(movieone[mindex].available == 0){//available are given in list, and if available is 0 print next step
        printf("Seats are fully booked, SORRY!!\n");
    } 
    else{//having available seats
        printf("Enter Your Name: ");
        scanf("%s", ticketone[*ticknum].name);//ticknum=400, having used as a parameter here
        printf("Enter Your Mobile number: ");
        scanf("%d", &ticketone[*ticknum].mobilno);
        //updating ticket's movie id with given movie id
        ticketone[*ticknum].mid = mid;
        //seat is booked so there is decrease in available seats
        movieone[mindex].available--;
        printf("SUCCESSFUL Ticket Booking!!!\n");
        (*ticknum)++;
    }
}

//function used to cancel tickets(display movies,using name to cancel tickets under the name,update seats)
void cancel(struct movie movieone[],int movnum,struct ticket ticketone[],int* ticknum,int uid){
    printf("%-10s%-20s%-27s%-28s%-30s%-10s\n", "ID", "Movie Name", "Theatre", "Total seat", "Available seat", "Price");
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
    for (int p = 0; p < movnum; p++) {
        printf("%-10d%-20s%-27s%-28d%-30d%-10f\n",movieone[p].mid,movieone[p].mname,movieone[p].theater,movieone[p].total,movieone[p].available,movieone[p].price);
    }
    printf("\nEnter Your Name: ");
    char name[50];
    scanf("%s",name);
    int f=0,i;//f used as the ticket being found 
    for (i = 0;i< *ticknum;i++){
        if(strcmp(ticketone[i].name,name)==0 && ticketone[i].mid==movieone[uid].mid){
            //if name matches
            //here again index set to-1
            int mindex=-1;
            for(int j=0;j<movnum;j++){
                if(movieone[j].mid==ticketone[i].mid){
                    mindex=j;
                    break;
                }
            }
            movieone[mindex].available++;
            // Remove the ticket entry
            for(int j=i;j<(*ticknum)-1;j++){
                ticketone[j] = ticketone[j + 1];
            }
            (*ticknum)--;
            f=1;
            printf("Cancelation sucess!!!\n");
            break;
        }
    }
    if(!f){
        printf("%s name is not here for this movie.\n",name);
    }
}

// Function to check movie status
void status(struct movie movieone[],int numMovies,int uid){
    printf("\nMovie ID: %d\n", movieone[uid].mid);
    printf("Movie Name: %s\n", movieone[uid].mname);
    printf("Theater: %s\n", movieone[uid].theater);
    printf("Total Seats: %d\n", movieone[uid].total);
    printf("Available Seats: %d\n", movieone[uid].available);
    printf("Price: %.2f\n", movieone[uid].price);
}

int main(){
    // Initialize user data
    struct user userone[MAX];
    int numUsers = read(userone);
    // Initialize movie data
    struct movie movieone[5] = {
   { 1, "Interstellar", "Theater 1", 100, 100, 12 },
        { 2, "Inside Out", "Theater 2", 80, 80, 10 },
        { 3, "Animal", "Theater 3", 90, 90, 13 },
        { 4, "Jatra", "Theater 4", 60, 60, 11 },
        { 5, "Zootopia", "Theater 3", 70, 70, 8 },
    };
    int numMovies=5;
    struct ticket ticketone[400]; // Array to store ticket information
    int numTickets=0; // Number of tickets
    int loggedinid=-1; // Index of the logged-in user
    while(1){
        if(loggedinid==-1){
            firstmain();
            int n;
            scanf("%d",&n);
            if(n==1){
                signin(userone, &numUsers);
            }
            else if(n==2){
                char username[30];
                char password[20];
                printf("Enter your username: ");
                scanf("%s",username);
                printf("Enter your password: ");
                scanf("%s",password);
                loggedinid=login(userone,numUsers,username,password);
                if(loggedinid==-1){
                    printf("UNsuccessful login!!!check username,password!!!\n");
                } 
                else{
                    printf("successful login!!!Hello, %s!\n",username);
                }
            }
            else if(n== 3) {
                printf("program exit\n");
                break;
            } 
            else{
                printf("Only choose from 1-3.\n");
            }
        } 
        else{
            displayforuser();
            int ch;
            scanf("%d", &ch);
            switch (ch) {
                case 1:
                    book(movieone, numMovies, ticketone, &numTickets, loggedinid);
                    break;
                case 2:
                    cancel(movieone, numMovies, ticketone, &numTickets, loggedinid);
                    break;
                case 3:
                    status(movieone, numMovies, loggedinid);
                    break;
                case 4:
                    printf("Now we log out.\n");
                    loggedinid = -1;
                    break;
                default:
                    printf("Only choose from 1-4.\n");
            }
        }
    }
}