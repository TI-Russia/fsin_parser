#include "webpagehandler.h"
#include <QString>
#include <QWebFrame>
#include <QWebElement>

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QNetworkAccessManager>
#include <QFile>

#include "declaratorcookiejar.h"


WebPageHandler::WebPageHandler(){
    QString addressString = "http://dev.declarator.org/accounts/login/?next=/";
    view = new QWebView();
    QNetworkCookieJar* cookieJar = new QNetworkCookieJar();
    QNetworkAccessManager* nam = new QNetworkAccessManager();
    nam->setCookieJar(cookieJar);
    view->page()->setNetworkAccessManager(nam);
    view->load(QUrl(addressString));
    QObject::connect(view,SIGNAL(loadFinished(bool)),this,SLOT(fillLoginDetails()));
    qDebug() << "page init";
}
void WebPageHandler::fillLoginDetails(){
    rootElement = view->page()->mainFrame()->documentElement();
    qDebug() << "started logging in";
    view->disconnect(SIGNAL(loadFinished(bool)));
    QObject::connect(view,SIGNAL(loadFinished(bool)),this,SLOT(goToAddDeclarataion()));
    QString userLogin = "maks";
    QString userPassword = "jhGwUdfM";

    QString loginSelector = "input[id=\"id_username\"]";
    QWebElement loginField = rootElement.findFirst(loginSelector);
    loginField.evaluateJavaScript("this.value = '" + userLogin +  "'");

    QString passwordSelector = "input[id=\"id_password\"]";
    QWebElement passwordField = rootElement.findFirst(passwordSelector);
    passwordField.evaluateJavaScript("this.value = '" + userPassword +  "'");

    QString submitButtonSelector = "input[type=\"submit\"]";
    QWebElement submitButton = rootElement.findFirst(submitButtonSelector);
    submitButton.removeAttribute(QString("disabled"));
    submitButton.evaluateJavaScript("this.click()");
}
void WebPageHandler::goToAddDeclarataion(){
    // writing cookies to file
    DeclaratorCookieJar* declCookies = new DeclaratorCookieJar();
    declCookies = (DeclaratorCookieJar*)view->page()->networkAccessManager()->cookieJar();
    declCookies->getAllCookies();

    QList<QNetworkCookie> cookieList;
    cookieList = declCookies->getAllCookies();
    QListIterator<QNetworkCookie> iter(cookieList);
    int i = 1;
    QFile data("declarator-cookies.txt");
    if(data.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream ts(&data);
        while(iter.hasNext()){
            QNetworkCookie cookie = iter.next();
            qDebug() << "=========== NUMBER " << i << " ============";
            qDebug() << "cookie name: " << cookie.name();
            qDebug() << "for domain: " << cookie.domain();
            qDebug() << "expires: " << cookie.expirationDate();
            qDebug() << "path: " << cookie.path();
            qDebug() << "value: " << cookie.value();
            qDebug() << "RAW: " << cookie.toRawForm();
            ts << cookie.toRawForm() << endl;
            i++;
        }
        data.close();
    } else {
        qDebug() << "Error writing declarator-cookies.txt";
    }
    view->disconnect(SIGNAL(loadFinished(bool)));
    view->load(QUrl("http://dev.declarator.org/document/7660/section_add/"));
    QObject::connect(view,SIGNAL(loadFinished(bool)),this,SLOT(fillDeclarataion()));
    qDebug() << "ready to add declaration";
}
void WebPageHandler::fillDeclarataion(){
    // read cookies from file
    qDebug() << "reading cookies from file";
    QFile data("declarator-cookies.txt");
    if(data.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream ts(&data);
        //QString cookieString = ;
        while(!ts.atEnd()){
            qDebug() << ts.readLine();
        }
    }
    else {
        qDebug() << "Error reading declarator-cookies.txt";
    }
    view->disconnect(SIGNAL(loadFinished(bool)));
    rootElement = view->page()->mainFrame()->documentElement();
    qDebug() << "started filling declaration";
    QString personNameFieldSelector = "input[id=\"id_person_text\"]";
    QWebElement personNameField = rootElement.findFirst(personNameFieldSelector);
    QString personName = "Сидоров Сидор Сидорович";
    personNameField.evaluateJavaScript("this.value = '" + personName + "'");

    QString personNewButtonSelector = "input[id=\"new-person-button\"]";
    QWebElement personNewButton = rootElement.findFirst(personNewButtonSelector);
    personNewButton.evaluateJavaScript("this.click()");

    QString personPositionFieldSelector = "input[id=\"id_position\"]";
    QWebElement personPositionField = rootElement.findFirst(personPositionFieldSelector);
    QString personPosition = "дворник";
    personPositionField.evaluateJavaScript("this.value = '" + personPosition + "'");

    QString OKButtonSelector = "input[id=\"id_main_block_button\"]";
    QWebElement OKButton = rootElement.findFirst(OKButtonSelector);
    OKButton.evaluateJavaScript("this.click()");
    view->show();
}
WebPageHandler::~WebPageHandler(){

}
