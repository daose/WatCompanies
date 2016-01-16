#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;


string extractDate(string date){
    string month, word, day, year;
    istringstream dstream(date);
    dstream >> word;
    if(word.compare("\"January") == 0) month = "01/";
    else if(word.compare("\"February") == 0) month = "02/";
    else if(word.compare("\"March") == 0) month = "03/";
    else if(word.compare("\"April") == 0) month = "04/";
    else if(word.compare("\"May") == 0) month = "05/";
    else if(word.compare("\"June") == 0) month = "06/";
    else if(word.compare("\"July") == 0) month = "07/";
    else if(word.compare("\"August") == 0) month = "08/";
    else if(word.compare("\"September") == 0) month = "09/";
    else if(word.compare("\"October") == 0) month = "10/";
    else if(word.compare("\"November") == 0) month = "11/";
    else if(word.compare("\"December") == 0) month = "12/";
    getline(dstream, day, ' ');
    getline(dstream, day, ',');
    month += (day + "/");
    getline(dstream, year, ' ');
    getline(dstream, year, '"');
    month += year;
    return month;
}

string extract(string erase, string line){
    string e = "";
    string word;
    istringstream linestream(line);
    int first = 0;
    while(linestream >> word){
        if(word.compare(erase) == 0){
            first = 1;
            continue;
        }
        if(first){
            e = "\"" + (e + word);
            first = 0;
        } else 
        e = (e + " " + word);
    }
    e += "\"";
    if(erase.compare("Date:") == 0)
        e = extractDate(e);
    return e;
}

string extractTime(string erase, string line, int flag){
    string e(line, 7);
    string time = "";
    string word, endTime;
    int found = 0;
    int i;
    istringstream tstream(e);
    while(tstream >> word){
        if((word.compare("AM") == 0) || (word.compare("PM") == 0)){
            time += (" " + word + ",");
            if(flag == 1) return time;
            else continue;
        }else if(word.compare("-") == 0){
            continue;
        }
        time += word;
    }
    for(i = 0; i < time.length(); i++){
        if(time[i] == ',') found = 1;
        if(found){
            endTime += time[i];
        }
    }
    return endTime;
}
/*
0123456 78
Time: 	11:30 AM - 1:30 PM
*/
int main(){
    string line, word, date;
    ifstream ifile;
    ofstream ofile ("februaryInfoSessions.csv"); 
    if(ofile.is_open()){
        ofile << "Subject,Start Date,Start Time,End Date,End Time,Location" << endl;
    }
    ifile.open("parseFeb.txt");
    while(getline(ifile, line)){
        if(line.find("Employer:") != string::npos){
            ofile << extract("Employer:", line) << ",";
        } else if(line.find("Date:") != string::npos){
            date = extract("Date:", line);
            ofile << date << ",";
        } else if(line.find("Time:") != string::npos){
            string startTime = extractTime("Time:", line, 1);
            string endTime = extractTime("Time:", line, 0);
            ofile << startTime << date << endTime;
        } else if(line.find("Location:") != string::npos){
            ofile << extract("Location:", line) << endl;
        } else continue;
    }
    ofile.close();
    ifile.close();
    return 0;
}

/* *
    Subject
    The name of the event, required.
    Example: Final exam
    Start Date
    The first day of the event, required.
    Example: 05/30/2020
    Start Time
    The time the event begins.
    Example: 10:00 AM
    End Date
    The last day of the event.
    Example: 05/30/2020
    End Time
    The time the event ends.
    Example: 1:00 PM
    All Day Event
    Whether the event is an all-day event. Enter True if it is an all-day event, and False if it isn't.
    Example: False
    Description
    Description or notes about the event.
    Example: 50 multiple choice questions and two essay questions 
    Location
    The location for the event.
    Example: "Columbia, Schermerhorn 614"
    Private
    Whether the event should be marked private. Enter True if the event is private, and False if it isn't.
    Example: True
**/
