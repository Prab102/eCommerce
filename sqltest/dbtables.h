#pragma once
#include<iostream>
#include<pqxx/pqxx>

using namespace std;
using namespace pqxx;

void fillDb();
void insertNewUser(string username, string password);
void addtoCart(string username, int productid,int quantity);
void removeFromCart(string username, int productID);
void updateQuantityCart(string username, int productID, int newQunatity);
vector<string> saleList();
string cartToOrder(string username);
void updateOrderNumbers(string username);  
void removeAllCart(string username); 
bool findOrder(string orderID);
bool findUser(string username, string password);


