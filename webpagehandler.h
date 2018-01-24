#ifndef WEBPAGEHANDLER_H
#define WEBPAGEHANDLER_H
#include <QWebView>
#include <QWebElement>

class WebPageHandler : public QObject{
    Q_OBJECT
    public:
        WebPageHandler();


        ~WebPageHandler();
    public slots:
        void fillLoginDetails();
        void goToAddDeclarataion();
        void fillDeclarataion();
    private:
        QWebView* view;
        QWebElement rootElement;

};

#endif // WEBPAGEHANDLER_H
