/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Users.cpp
InversePalindrome.com
*/


#include "Users.hpp"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>


Users::Users() :
    crypto(0x0c2ad4a4acb9f023)
{
    load("Users.xml");
}

Users::~Users()
{
    save("Users.xml");
}

void Users::load(const QString& fileName)
{
    QDomDocument doc;
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    else
    {
        if(!doc.setContent(&file))
        {
            return;
        }

        file.close();
    }

    auto usersElement = doc.firstChildElement("Users");

    auto usersList = usersElement.elementsByTagName("User");

    for(int i = 0; i < usersList.count(); ++i)
    {
        auto user = usersList.at(i);

        if(user.isElement())
        {
            auto userElement = user.toElement();

            users.insert(userElement.attribute("name"), userElement.attribute("password"));
        }
    }
}

void Users::save(const QString& fileName)
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto usersElement = doc.createElement("Users");

    for(auto usuarioItr = users.constBegin(); usuarioItr != users.constEnd(); ++usuarioItr)
    {
        auto userElement = doc.createElement("User");

        userElement.setAttribute("name", usuarioItr.key());
        userElement.setAttribute("password", usuarioItr.value());

        usersElement.appendChild(userElement);
    }

    doc.appendChild(usersElement);

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       return;
    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

void Users::addUser(const QString& user, const QString& password)
{
    users.insert(user, crypto.encryptToString(password));

    QDir().mkdir(user);
}

bool Users::isLoginValid(const QString& user, const QString& password)
{
    if(users.count(user) && password == crypto.decryptToString(users.value(user)))
    {
        return true;
    }

    return false;
}

bool Users::isRegistrationValid(const QString& user)
{
    if(!users.count(user))
    {
        return true;
    }

    return false;
}
