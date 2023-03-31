/*******************************************************************
 * @Descripttion: 
 * @version: 
 * @Author: tylerytr
 * @Date: 2023-03-31 16:12:57
 * @LastEditTime: 2023-03-31 21:29:03
 * @LastEditors: tylerytr
 * @FilePath: /db_example/src/commandparser.cpp
 * @Email:601576661@qq.com
 * @Copyright (c) 2023 by tyleryin, All Rights Reserved. 
*******************************************************************/
#include "command_parser.h"
#include <assert.h>
using std::cout;
using std::endl;
using std::vector;
/*******************************************************************
 * @brief : 按照空格分割command存储到commandList中
 * @param {string&} command
 * @return {*} 是否超过最大长度
 * @description: 
*******************************************************************/
bool commandSplit(vector<string>& commandList,const string& command){
    string temp;
    for(int i=0;i<command.size();i++){
        if(command[i]==' '){
            if(temp.size()>0){
                commandList.push_back(temp);
                temp.clear();
                if(commandList.size()>7){
                    //超过了select * from db where id in ({minIndex,maxIndex})的长度
                    return false;
                }
            }
        }else{
            temp+=command[i];
        }
    }
    if(temp.size()>0){
        commandList.push_back(temp);
    }
    return true;

}
bool safeAtoi(int &result,const string&str){
    if(str.size()>10){
        return false;
    }
    long long temp=0;
    for(int i=0;i<str.size();i++){
        if(str[i]<'0'||str[i]>'9'){
            return false;
        }
        temp=temp*10+str[i]-'0';
    }
    if(temp>INT_MAX){
        return false;
    }
    result=temp;
    return true;
}
ParserResult CommandParser::command(const std::string& command){
    if(command.size()>256){
        return ParserResult::STRING_TOO_LONG;
    }

    vector<string> commandList;
    if(!commandSplit(commandList,command)){
        //cout<<"commandList.size()1:"<<commandList.size()<<endl;
        return ParserResult::UNRECONGNIZED_STATEMENT;
    }
    //cout<<"commandList.size():"<<commandList.size()<<endl;
    if(commandList[0]==".help"){
        if(commandList.size()>1){
            return ParserResult::SYBTAX_ERROR;
        }
        return ParserResult::HELP;
    }else if(commandList[0]==".exit"){
        if(commandList.size()>1){
            return ParserResult::SYBTAX_ERROR;
        }
        return ParserResult::EXIT;
    }else if(commandList[0]==".reset"){
        if(commandList.size()>1){
            return ParserResult::SYBTAX_ERROR;
        }
        return ParserResult::RESET;
    }else if(commandList[0]=="insert"){
        if(commandList.size()!=6){
            return ParserResult::SYBTAX_ERROR;
        }
        if(commandList[1]!="db"){
            return ParserResult::SYBTAX_ERROR;
        }

        if(!safeAtoi(state.row.id,commandList[2])){
            return ParserResult::SYBTAX_ERROR;
        }
        if(state.row.id<0){
            return ParserResult::BAD_ID;
        }

        state.row.username=commandList[3];
        if(!safeAtoi(state.row.age,commandList[4])){
            return ParserResult::SYBTAX_ERROR;
        }
        state.row.email=commandList[5];
        state.type=StatementType::INSERT;
        return ParserResult::SUCCESS;

    }else if(commandList[0]=="update"){
        if(commandList.size()!=7){
            return ParserResult::SYBTAX_ERROR;
        }
        if(commandList[1]!="db"){
            return ParserResult::SYBTAX_ERROR;
        }
        state.row.username=commandList[2];
        if(!safeAtoi(state.row.age,commandList[3])){
            return ParserResult::SYBTAX_ERROR;
        }
        state.row.email=commandList[4];
        if(commandList[5]!="where"){
            return ParserResult::SYBTAX_ERROR;
        }
        int position=commandList[6].find("id=");
        if(position!=0){
            return ParserResult::SYBTAX_ERROR;
        }
        if(!safeAtoi(state.row.id,commandList[6].substr(3))){
            return ParserResult::SYBTAX_ERROR;
        }
        state.type=StatementType::UPDATE;
        return ParserResult::SUCCESS;

    }else if(commandList[0]=="delete"){
        if(commandList.size()!=5){
            return ParserResult::SYBTAX_ERROR;
        }
        if(commandList[1]!="from"){
            return ParserResult::SYBTAX_ERROR;
        }
        if(commandList[2]!="db"){
            return ParserResult::SYBTAX_ERROR;
        }
        if(commandList[3]!="where"){
            return ParserResult::SYBTAX_ERROR;
        }
        int position=commandList[4].find("id=");
        if(position!=0){
            return ParserResult::SYBTAX_ERROR;
        }
        if(!safeAtoi(state.row.id,commandList[4].substr(3))){
            return ParserResult::SYBTAX_ERROR;
        }
        state.type=StatementType::DELETE;
        return ParserResult::SUCCESS;
    }else if(commandList[0]=="select"){
        if(commandList.size()!=6&&commandList.size()!=8){
            return ParserResult::SYBTAX_ERROR;
        }
        if(commandList[1]!="*"){
            return ParserResult::SYBTAX_ERROR;
        }
        if(commandList[2]!="from"){
            return ParserResult::SYBTAX_ERROR;
        }
        if(commandList[3]!="db"){
            return ParserResult::SYBTAX_ERROR;
        }
        if(commandList[4]!="where"){
            return ParserResult::SYBTAX_ERROR;
        }
        if(commandList.size()==6){
            int position=commandList[5].find("id=");
            if(position!=0){
                return ParserResult::SYBTAX_ERROR;
            }
            if(!safeAtoi(state.row.id,commandList[5].substr(3))){
                return ParserResult::SYBTAX_ERROR;
            }
            state.type=StatementType::SELECT_ONE;
            return ParserResult::SUCCESS;
        }else{
            if(commandList[5]!="id"){
                return ParserResult::SYBTAX_ERROR;
            }
            if(commandList[6]!="in"){
                return ParserResult::SYBTAX_ERROR;
            }
            //如果末尾有空格的话会在前面return
            int position=commandList[7].find(",");
            if(position==string::npos){
                return ParserResult::SYBTAX_ERROR;
            }
            if(!safeAtoi(state.row.min_index,commandList[7].substr(1,position-1))){
                return ParserResult::SYBTAX_ERROR;
            }
            int distance=commandList[7].size()-position-2;

            if(!safeAtoi(state.row.max_index,commandList[7].substr(position+1,distance))){
                return ParserResult::SYBTAX_ERROR;
            }
            state.type=StatementType::SELECT_RANGE;
            return ParserResult::SUCCESS;
        }
    }else{
        return ParserResult::UNRECONGNIZED_STATEMENT;
    }
    // assert(0);
    return ParserResult::UNRECONGNIZED_STATEMENT;
}
void CommandParser::printState(){
    cout<<"state.row.id:"<<state.row.id<<endl;
    cout<<"state.row.username:"<<state.row.username<<endl;
    cout<<"state.row.age:"<<state.row.age<<endl;
    cout<<"state.row.email:"<<state.row.email<<endl;
    cout<<"state.row.min_index:"<<state.row.min_index<<endl;
    cout<<"state.row.max_index:"<<state.row.max_index<<endl;
    //cout<<"state.type:"<<state.type<<endl;
}
CommandParser::CommandParser(){
    state.type=StatementType::NONE;
    state.row.id=0;
    state.row.username="";
    state.row.age=0;
    state.row.email="";
    state.row.min_index=0;
    state.row.max_index=0;
}
