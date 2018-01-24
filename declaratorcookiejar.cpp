#include "declaratorcookiejar.h"

DeclaratorCookieJar::DeclaratorCookieJar(){

}

DeclaratorCookieJar::~DeclaratorCookieJar()
{

}
void DeclaratorCookieJar::setAllCookies(){
    //allTheCookies = this->allCookies();
}
QList<QNetworkCookie> DeclaratorCookieJar::getAllCookies(){
    return this->allCookies();
}


