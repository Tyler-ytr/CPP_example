/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-03-31 14:30:23
 * @LastEditTime: 2023-03-31 21:15:36
 * @LastEditors: tylerytr
 * @FilePath: /db_example/src/db_example.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
*******************************************************************/
#include "db_example.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;
void exitMessage(){
    cout<<"> EXIT"<<endl;
}
void TOBEDONE(){
    cout<<"> TOBEDONE"<<endl;
}


void printHelpMessage(){
    cout<<std::setfill('*')<<std::setw(80)<<""<<endl;
    cout<<"This is a db_example; Db file locates in \"./data/db.bin\"."<<endl;
    cout<<std::setfill('*')<<std::setw(80)<<""<<endl;
    cout<<".help                                                print help message;"<<endl;
    cout<<".exit                                                exit db_example;"<<endl;
    cout<<".reset                                               reset db.bin;"<<endl;
    cout<<"insert db {index} {name} {age} {email}               insert record;"<<endl;
    cout<<"update db {name} {age} {email} where id={index}      update record;"<<endl;
    cout<<"delete from db where id={index}                      delete record;"<<endl;
    cout<<"select * from db where id={index}                    select a record;"<<endl;
    cout<<"select * from db where id in ({minIndex,maxIndex})   select records;"<<endl;
    cout<<std::setfill('*')<<std::setw(80)<<""<<endl;
}



db_example::db_example(){
    //TODO
    ;
}
void db_example::run_db(){
    //TODO
    printHelpMessage();
    shellProgress();
}

void db_example::shellProgress(){
    //TODO
    string userCommand;
    
    while(true){ 
        std::cout<<"> ";
        getline(cin,userCommand);
        CommandParser parser;
        ParserResult result=parser.command(userCommand);
        if(result==ParserResult::EXIT){
            exitMessage();
            break;
        }

        switch (result){
            case ParserResult::HELP:
                printHelpMessage();
                continue;
            case ParserResult::RESET:
                TOBEDONE();
                continue;
            case ParserResult::SYBTAX_ERROR:
                cout<<"> SYBTAX_ERROR"<<endl;
                continue;
            case ParserResult::BAD_ID:
                cout<<"> BAD_ID"<<endl;
                continue;
            case ParserResult::STRING_TOO_LONG:
                cout<<"> STRING_TOO_LONG"<<endl;
                continue;
            case ParserResult::UNRECONGNIZED_STATEMENT:
                cout<<"> UNRECONGNIZED_STATEMENT"<<endl;
                continue;
            case ParserResult::SUCCESS:
                TOBEDONE();
                break;
        }
        if(result!=ParserResult::SUCCESS){
            cout<<"Should not go to here!!!\n\n"<<endl;
            std::cerr<<userCommand<<endl;
            break;
        }
        switch(parser.state.type){
            case StatementType::INSERT:
                cout<<"> INSERT"<<endl;
                parser.printState();
                TOBEDONE();
                break;
            case StatementType::UPDATE:
                cout<<"> UPDATE"<<endl;
                parser.printState();
                TOBEDONE();
                break;
            case StatementType::SELECT_ONE:
                cout<<"> SELECT_ONE"<<endl;
                parser.printState();
                TOBEDONE();
                break;
            case StatementType::SELECT_RANGE:
                cout<<"> SELECT_RANGE"<<endl;
                parser.printState();
                TOBEDONE();
                break;
            case StatementType::DELETE:
                cout<<"> DELETE"<<endl;
                parser.printState();
                TOBEDONE();
                break;
        }

        
        userCommand.clear();

    }    

}

