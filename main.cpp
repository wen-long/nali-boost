//
//  main.cpp
//  nali-c_boost
//
//  Created by wenlong on 16/8/28.
//  Copyright © 2016年 wenlong. All rights reserved.
//
#include "qqwry.h"
#include <string.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <string>
#include "boost/timer.hpp"
#include "boost/filesystem.hpp"
#include "boost/locale/encoding.hpp"
#include "boost/xpressive/xpressive.hpp"


#define NALI_QQWRY_PATH "/usr/local/share/QQWry.Dat"

using namespace std;

char *g_country = NULL;
char *g_area = NULL;
char *g_output = NULL;
FILE *dataFile;

string f(const char *ipCstring) {
    memset(g_country, 0, BUFSIZ);
    memset(g_area, 0, BUFSIZ);
    memset(g_output, 0, BUFSIZ);
    
    qqwry_get_location(g_country,g_area,ipCstring,dataFile);
    
    string country = boost::locale::conv::between( g_country, "UTF-8", "GBK" );
    string area = boost::locale::conv::between( g_area, "UTF-8", "GBK" );
    string result;
    result += "[" + country + (area.size()>0? " " + area: "") + "]";
    return result;
}

std::string modifyString(boost::xpressive::smatch const &what){
    return what.str() + f(what.str().c_str());
}


int main(int argc, const char * argv[]) {
    g_country = (char*)malloc(BUFSIZ);
    g_area = (char*)malloc(BUFSIZ);
    g_output = (char*)malloc(BUFSIZ);
    dataFile = fopen(NALI_QQWRY_PATH,"r");
    string input;

    std::ios_base::sync_with_stdio(false);


//    const boost::xpressive::sregex rex = boost::xpressive::sregex::compile
//    ( "\\d|[1-9]\\d|1\\d{1,2}|2[0-4]\\d|25[0-5]\\.\\d|[1-9]\\d|1\\d{1,2}|2[0-4]\\d|25[0-5]\\.\\d|[1-9]\\d|1\\d{1,2}|2[0-4]\\d|25[0-5]\\.25[0-5]|2[0-4]\\d|1\\d{1,2}|[1-9]\\d|\\d" );
//    ( "([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])\\.([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])\\.([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])\\.(25[0-5]|2[0-4][0-9]|1[0-9]{1,2}|[1-9][0-9]|[0-9])" );
    const boost::xpressive::sregex rex = boost::xpressive::sregex::compile (
    "\\d\\d{0,2}\\.\\d\\d{0,2}\\.\\d\\d{0,2}\\.\\d\\d{0,2}");
    
    if (argc >= 2)
    {
        size_t index = 1;
        while (index < argc) {
            std::string out = boost::xpressive::regex_replace(string(argv[index++]), rex, modifyString);
            cout << out << endl;
        }
    } else {
        while (getline(cin, input)) {
            std::string out = boost::xpressive::regex_replace(input, rex, modifyString);
            cout << out << "\n";
        }
    }
    fclose(dataFile);
    return 0;
}
