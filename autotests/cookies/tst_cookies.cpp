/*
 * Copyright 2008 Oliver Gerlich <oliver.gerlich@gmx.de>
 * (based on tst_tabbar.cpp which is:
 *  Copyright 2008 Benjamin C. Meyer <ben@meyerhome.net> )
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

// #include <QtGui/QtGui>
#include <QtTest/QtTest>
#include "qtest_arora.h"
#include <cookiejar.h>

class tst_Cookies : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void init();

private slots:
//     void sometest();

    void checkCheckDefaults();
    void checkDefaults();

    void defaultAddAndRetrieve_data();
    void defaultAddAndRetrieve();
};

void tst_Cookies::initTestCase()
{
    // set org name and app name so all test config data is stored in own directories:
    QCoreApplication::setOrganizationName("arora_devs");
    QCoreApplication::setApplicationName("cookietest");
}

void tst_Cookies::init()
{
    // make sure we start with default settings:
    QSettings settings;
    settings.clear();
}

// void tst_Cookies::sometest()
// {
//     CookieJar cjar;
//     QCOMPARE(cjar.blockedCookies().size(), 0);
// 
//     cjar.setKeepPolicy(CookieJar::KeepUntilExpire);
//     QCOMPARE(cjar.keepPolicy(), CookieJar::KeepUntilExpire);
//     cjar.setKeepPolicy(CookieJar::KeepUntilExit);
//     QCOMPARE(cjar.keepPolicy(), CookieJar::KeepUntilExit);
// //     cjar.setKeepPolicy(CookieJar::KeepUntilTimeLimit);
// //     QCOMPARE(cjar.keepPolicy(), CookieJar::KeepUntilTimeLimit);
// }

void tst_Cookies::checkCheckDefaults ()
{
    // Meta-test: make sure that every test case starts with default settings:
    CookieJar cjar;
    QCOMPARE(cjar.keepPolicy(), CookieJar::KeepUntilExpire);
    cjar.setKeepPolicy(CookieJar::KeepUntilExit);
    QCOMPARE(cjar.keepPolicy(), CookieJar::KeepUntilExit);
}

void tst_Cookies::checkDefaults ()
{
    CookieJar cjar;
    QCOMPARE(cjar.acceptPolicy(), CookieJar::AcceptOnlyFromSitesNavigatedTo);
    QCOMPARE(cjar.keepPolicy(), CookieJar::KeepUntilExpire);
    QCOMPARE(cjar.blockedCookies().size(), 0);
    QCOMPARE(cjar.allowedCookies().size(), 0);
    QCOMPARE(cjar.allowForSessionCookies().size(), 0);
}


void tst_Cookies::defaultAddAndRetrieve_data()
{
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<QByteArray>("cookieString");

    QTest::newRow("example.com") << QUrl("http://www.example.com/")
            << QByteArray("Set-Cookie:  testkey1=testvalue1; expires=Sun, 27-Jul-2008 10:08:54 GMT; Max-Age=1209600; Path=/;");
    QTest::newRow("testhost") << QUrl("http://testhost/")
            << QByteArray("Set-Cookie:  testkey2=testvalue2; expires=Sun, 27-Jul-2008 10:08:54 GMT; Max-Age=1209600; Path=/;");
    QTest::newRow("example.com:1234") << QUrl("http://www.example.com:1234/")
            << QByteArray("Set-Cookie:  testkey3=testvalue3; expires=Sun, 27-Jul-2008 10:08:54 GMT; Max-Age=1209600; Path=/;");
}

void tst_Cookies::defaultAddAndRetrieve()
{
    CookieJar cjar;

    QFETCH(QUrl, url);
    QFETCH(QByteArray, cookieString);

    // check pre-conditions
    QCOMPARE(cjar.cookiesForUrl(url).size(), 0);

    // parse and add cookie
    QList<QNetworkCookie> newCookies = QNetworkCookie::parseCookies(cookieString);
    const bool success = cjar.setCookiesFromUrl(newCookies, url);
    QCOMPARE(success, true);

    // retrieve and check cookie
    QList<QNetworkCookie> retrCookies = cjar.cookiesForUrl(url);
    QCOMPARE(retrCookies.size(), 1);
    QCOMPARE(retrCookies[0].domain(), url.host());
    QCOMPARE(retrCookies[0].name(), newCookies[0].name());
    QCOMPARE(retrCookies[0].value(), newCookies[0].value());
}

QTEST_MAIN(tst_Cookies)
#include "tst_cookies.moc"

