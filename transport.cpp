#include "transport.h"

Transport::Transport(){

}
void Transport::setType(int transType){
    type = transType;
}
void Transport::setName(QString transName){
    name = transName;
}
int Transport::getType(){
    return type;
}
QString Transport::getName(){
    return name;
}
Transport::~Transport(){

}

