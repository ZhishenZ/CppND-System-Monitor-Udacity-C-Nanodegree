#include <string>

#include "format.h"

using std::string;


// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 
    
    long HH= 0;
    int  MM =0, SS=0;
    string ElapsedTime;

    HH = seconds/3600;
    MM = (seconds-3600*HH)/60;
    SS = seconds-3600*HH-60*MM;
  
    // make sure MM and SS have two digits
    string sMM = std::to_string(MM);
    if (sMM.length() == 1){
        sMM = "0"+sMM;
    } 
    string sSS = std::to_string(SS);
    if (sSS.length() == 1){
        sSS = "0"+sSS;
    } 
  
    ElapsedTime = std::to_string(HH)+":"+sMM+":"+sSS;

    return ElapsedTime;

    
}