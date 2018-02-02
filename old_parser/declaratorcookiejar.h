#ifndef DECLARATORCOOKIEJAR_H
#define DECLARATORCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>

class DeclaratorCookieJar : public QNetworkCookieJar{
    public:
        DeclaratorCookieJar();
        void setAllCookies();
        ~DeclaratorCookieJar();
        QList<QNetworkCookie> getAllCookies();

    private:
        QList<QNetworkCookie> allTheCookies;
};

#endif // DECLARATORCOOKIEJAR_H
