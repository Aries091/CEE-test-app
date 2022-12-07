#ifndef _SETTIMER_H_
#define _SETTIMER_H_

#include <iostream>
#include <sqlite3.h>
#include <fstream>

using namespace std;

string setTimer(const char* req){
   string time(req);
   ofstream timer_file;
   timer_file.open("timer.txt");
   if(timer_file) { 
      timer_file << time;
      timer_file.close();
      return "OK";
   }else{
      return "CANT_READ_FILE";
   }

}

string getTime(){
   ifstream timer_file;
   timer_file.open("timer.txt");
   if(timer_file.is_open()){
      string line;
      getline(timer_file, line);
      timer_file.close();
      return line;
   }else{
      return "0";
   }
}

#endif
