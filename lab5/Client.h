#ifndef LAB_5_CLIENT_H
#define LAB_5_CLIENT_H

#include <iostream>
#include <string>

class ClientBuilder;

class Client {
public:
    Client(const unsigned ID, const std::string &firstName, const std::string &lastName, const unsigned passNum = 0, const std::string &address = "")
    {
        this->ID = ID;
        this->firstName = firstName;
        this->lastName = lastName;
        this->address = address;
        this->passNum = passNum;
        if (passNum != 0 && !address.empty())
            trusted = true;
        else trusted = false;
    }

    bool addPassNum(const unsigned passNum)
    {
        if (!trusted && this->passNum == 0)
        {
            this->passNum = passNum;
            if (!this->address.empty())
                trusted = true;
            return true;
        }
        else return false;
    }

    bool addAddress(const std::string &address)
    {
        if (!trusted && this->address.empty())
        {
            this->address = address;
            if (this->passNum != 0)
                trusted = true;
            return true;
        }
        else return false;
    }

    unsigned getID() const { return ID; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getAddress() const { return address; }
    int getPassNum() const { return passNum; }
    bool getTrusted() const { return trusted; }

private:
    unsigned ID;
    bool trusted;
    std::string firstName;
    std::string lastName;
    std::string address;
    unsigned passNum;
};

class ClientBuilder {
public:

    void reset()
    {
        firstName = lastName = address = "";
        passNum = 0;
    }

    std::optional<Client> build(const unsigned ID)
    {
        if (!firstName.empty() && !lastName.empty())
        {
            if (!address.empty())
                return Client(ID, firstName, lastName, passNum, address);
            else
                return Client(ID, firstName, lastName, passNum);
        }
        else return {};
    }

    void setName(const std::string &firstName, const std::string &lastName) { this->firstName = firstName; this->lastName = lastName; }
    void setAddress(const std::string &address) { this->address = address; }
    void setPass(const unsigned num) { if (num > 0) this->passNum = num; }

private:
    std::string firstName;
    std::string lastName;
    std::string address;
    unsigned passNum = 0;
};

#endif