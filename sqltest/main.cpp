#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <pqxx/pqxx>
#include "dbtables.h"
//#include "functionality.h" //combine with other header

using namespace std;

int main()
{
    //run to create tables and fill initial products table for testing
   // fillDb();
    
    //create a c string for db information  use this to connect to your database respectivley
    string C = "host=localhost port=5432 dbname=eCom user=postgres password =1";  
    const char * sql;
    int i=0;
    try
    {
        //connects db, if created C is used to make connection object
        pqxx::connection connectionObject(C.c_str());
        bool loginFlag = false;
        string decision;
        string uid;
        
        bool exitCommand = false;
        vector<string> saleItems = saleList();
        while (true && exitCommand == false) {

            cout << "     WELCOME TO PRINCE OF PRODUCE!!!!     " << endl << endl;
            
            
            //Not Signed In
            while (true && loginFlag == false) {  


                cout <<endl<<endl<< "Press 1 to Create an account/Login " << endl;
                cout << "Press 2 to Browse the inventory " << endl;
                cout << "Press 3 to Manage Cart " << endl;
                cout << "Press 4 to View Order"<<endl;
                cout << "Press 5 to Exit " << endl;
                cin >> decision;

                // CREATE/LOGIN ACCOUNT
                if (decision == "1") {
                    string username;
                    string password;
                    string accDecision ;
                    while (true) {


                        cout << " Login or Sign Up for an account " << endl << endl;
                        cout << "1: Sign Up " << endl;
                        cout << "2: Login " << endl;
                        cout << "3: Back " << endl;
                        cin >> accDecision;

                        if (accDecision == "1") {
                            // string username;
                            // string password;
                            cout << "              Create an Account             " << endl << endl << endl;
                            cout << "     Enter a Username: ";
                            cin >> username;
                            cout << endl << endl;
                            cout << "     Enter a Password: ";
                            cin >> password;

                            insertNewUser(username, password);

                           // cout<<endl<<endl << "     Account Created Successfully!     " << endl << "     Now Login To Your Account:   " << endl;

                            //if account created succsefully then break 
                            break;
                        }
                        else if (accDecision == "2") {
                            cout << "              Login to an Account             " << endl << endl << endl;
                            cout << "     Enter a Username: ";
                            cin >> username;
                            cout << endl << endl;
                            cout << "     Enter a Password: ";
                            cin >> password;

                            //find account in table
                            if (findUser(username, password) == true) {
                                cout <<endl <<endl<< "     Login Successfull! " << endl << endl;
                                loginFlag = true;
                                uid = username;
                            }
                            else
                                cout << "     Username or Password entered is not correct. " << endl <<endl;

                            //if found an account store username id for later for logged in functionality 
                            //cout << "     Login Successfull!     " << endl <<endl;
                            break;

                        }
                        else if (accDecision == "3") {
       
                            break;
                        }

                    }

                }
                //BROWSE
                else if (decision == "2") { //can browse when not signed in so this should be functional
                    string searchDecision;

                    while (true) {
                        cout <<endl<< "     Inventory Search  " << endl;
                        cout <<endl<< "login to add items to your cart" <<endl<<endl; 
                        cout << "1: Search by keyword " << endl;
                        cout << "2: SALE Items!!!!" << endl;                      
                        cout << "3: Categories " << endl;
                        cout << "4: All Items " << endl;
                        cout << "5: Back " << endl;
                        cin >> searchDecision;


                        if (searchDecision == "1") { 
                            string keyword;
                            string keyDecision;
                            int productID = 0;
                            bool IDmatch = false;
                            int quantity = 0;
                            cout << "Enter a keyword:  ";
                            cin >> keyword;
                            //LOOP FOR RETURNED ITEMS
                            while (true) {

                                pqxx::work searchwk(connectionObject);
                                //search for word entered in database
                                pqxx::result returned = searchwk.exec("SELECT * FROM product WHERE name = '" + keyword + "' OR name LIKE '" + keyword + "%'");
                                //if word is found
                                if (returned.size() > 0) {
                                    for (int i = 0; i < returned.size(); i++) {
                                        //lists all items that include keyword
                                        if (find(saleItems.begin(), saleItems.end(), to_string(returned[i][1])) != saleItems.end()) {

                                            float salePrice = stof(to_string(returned[i][2])) * .20;
                                            salePrice = stof(to_string(returned[i][2])) - salePrice;
                                            //ON SALE FROM SELECTION
                                            cout << returned[i][1] << endl << " Price: " << returned[i][2] << "  -->  " << salePrice << " ID: " << returned[i][0] << " ON SALE!! " << endl << endl;
                                        }
                                        else {
                                            cout << endl << returned[i][1] << "  " << endl << "Price: " << returned[i][2] << " ID: " << returned[i][0] << endl << endl;
                                        }
                                    }

                                    cout << "1: New search word" << endl;
                                    cout << "2: back" << endl;
                                    cin >> keyDecision;

                                    //NEW KEYWORD
                                    if (keyDecision == "1") {

                                    cout << "Enter a new key word" << endl;
                                    cin >> keyword;
                                    }
                                    else if (keyDecision == "2") {
                                        break;
                                    }

                                }

                                //WORD NOT FOUND
                                else {
                                    cout << "Keyword not found; " << endl;
                                    cout << "Enter a Keyword:   ";
                                    cin >> keyword;
                                }

                            }

                        }





                        else if (searchDecision == "2") {

                            //GETS ITEMS ON SALE                          
                            string choice;                            
                            cout << "        All Items On Sale     " << endl << endl << endl;

                            pqxx::work salewk(connectionObject);

                            //ALL ITEMS FROM PRODUCT TABLE
                            pqxx::result returned = salewk.exec("SELECT * FROM product");


                            //LISTS ITMES FOUND only shows items on sale                      
                            for (size_t i = 0; i < returned.size(); i++)
                            {
                                //if queried items are included in saleitems.
                                if (find(saleItems.begin(), saleItems.end(), to_string(returned[i][1])) != saleItems.end()) {

                                    float salePrice = stof(to_string(returned[i][2])) * .20;
                                    salePrice = stof(to_string(returned[i][2])) - salePrice;
                                    //ON SALE FROM SELECTION
                                    cout << returned[i][1] << endl << " Price: " << returned[i][2] << "  -->  " << salePrice << " ID: " << returned[i][0] << endl << endl;
                                }
                            }

                            cout <<endl<< "Enter 1 to return" <<endl;
                            
                            cin >> choice;
                            if (choice == "2") {
                                break;
                            }

                        }
                        else if (searchDecision == "3") {

                            string categoryDec;
                            int productID = 0;

                            while (true) {
                                pqxx::work searchwk(connectionObject);

                                cout<<endl << "     Categories Available     " << endl << endl;
                                cout << "1: Vegetables " << endl;
                                cout << "2: Fruits " << endl;
                                cout << "3: Back " << endl;
                                cin >> categoryDec;

                                //VEGETABLES 
                                if (categoryDec == "1") {

                                    string choice;
                                    //selects all vegetable items from product tbale 
                                    pqxx::result returned = searchwk.exec("SELECT * FROM product WHERE category = 'vegetable'");
                                    cout << endl << endl << endl << "     Vegetables     " << endl;

                                    //lists found products for UI
                                    for (int i = 0; i < returned.size(); i++) {

                                        if (find(saleItems.begin(), saleItems.end(), to_string(returned[i][1])) != saleItems.end()) {

                                            float salePrice = stof(to_string(returned[i][2])) * .20;
                                            salePrice = stof(to_string(returned[i][2])) - salePrice;
                                            //ON SALE FROM SELECTION
                                            cout << returned[i][1] << endl << " Price: " << returned[i][2] << "  -->  " << salePrice << " ID: " << returned[i][0] << " ON SALE!! " << endl << endl;
                                        }
                                        else {
                                            cout << endl << returned[i][1] << "  " << endl << "Price: " << returned[i][2] << " ID: " << returned[i][0] << endl << endl;
                                        }
                                    }
                                    //loop for further operations for category chosen
                                    while (true) {

                                        
                                        cout << "1: back to categories " << endl;
                                        cin >> choice;                                    
                                        //back to category selection
                                        if (choice == "1") {

                                            break;
                                        }
                                    }
                                }
                                else if (categoryDec == "2") {
                                    //FRUITS CATEGORY
                                    string choice;
                                    //selects all fruits items from product tbale 
                                    pqxx::result returned = searchwk.exec("SELECT * FROM product WHERE category = 'fruit'");
                                    cout << endl << endl << endl << "     Fruits     " << endl;
                                    //lists found products for UI
                                    for (int i = 0; i < returned.size(); i++) {
                                        if (find(saleItems.begin(), saleItems.end(), to_string(returned[i][1])) != saleItems.end()) {

                                            float salePrice = stof(to_string(returned[i][2])) * .20;
                                            salePrice = stof(to_string(returned[i][2])) - salePrice;
                                            //ON SALE FROM SELECTION
                                            cout << returned[i][1] << endl << " Price: " << returned[i][2] << "  -->  " << salePrice << " ID: " << returned[i][0] << " ON SALE!! " << endl << endl;
                                        }
                                        else {
                                            cout << endl << returned[i][1] << "  " << endl << "Price: " << returned[i][2] << " ID: " << returned[i][0] << endl << endl;
                                        }
                                    }
                                    //further operations for fruit category
                                    while (true) {                                        
                                        cout << "1: Back to categories " << endl;
                                        cin >> choice;
                                     
                                        //back to category selection
                                        if (choice == "1") {
                                            break;
                                        }
                                    }

                                }
                                else if (categoryDec == "3") {
                                break;
                                }
                                else if (cin.fail()) { //maybe delete
                                cout << "Incorrect Input" << endl;
                                cin.clear();
                                }


                            }
                        



                        }
                        //ALL ITEMS
                        else if (searchDecision == "4") {
                                                       
                            pqxx::work allwk(connectionObject);
                                                       
                            //ALL ITEMS FROM PRODUCT TABLE
                            
                            pqxx::result response = allwk.exec("SELECT * FROM product");                          

                            //LISTS ITMES FOUND                           
                            for (size_t i = 0; i < response.size(); i++) 
                            {
                                //if queried items are included in saleitems.
                                if(find(saleItems.begin(), saleItems.end(), to_string(response[i][1])) != saleItems.end()){
                                    
                                    float salePrice = stof(to_string(response[i][2])) * .20;
                                    salePrice = stof(to_string(response[i][2])) - salePrice;
                                    //ON SALE FROM SELECTION
                                    cout << response[i][1] << endl << " Price: " << response[i][2] << "  -->  " << salePrice << " ID: " << response[i][0] << " ON SALE!! " << endl << endl;
                                }
                                else{
                                    cout << response[i][1] << endl << " Price: " << response[i][2] << " ID: " << response[i][0] << endl << endl;
                                }
                            }
                            string cartprocess;
                            
                            while (true) {                              
                                cout << endl << "1: Back" << endl;
                                cin >> cartprocess;
                              
                                if (cartprocess == "1") {
                                    break;
                                }
                                else {
                                    cout << "incorrect input " << endl << endl;
                                }
                            }

                        }
                        //BACK
                        else if (searchDecision == "5") {
                            break;
                        }
                        else {
                            cout << "Not a Valid entry" <<endl;
                        }

                    }
                    
                }

                else if (decision == "3" && loginFlag == true) { //technically if loginflag == true then it wont be here wont have cart
                    cout << "cart" << endl; //possibly remove
                }


                else if (decision == "3" && loginFlag == false) {
                    cout<<endl << "Login or create an account to manage cart. " << endl;
                }


                else if (decision == "4") { //view order
                    
                    string oid;
                    bool foundOrder;
                    cout<<endl << "       View Order       " << endl <<endl <<endl;

                    //SEARCH FOR ORDER
                    while (true) {
                        cout << endl << "Enter An OrderID: " << endl;
                        cout << "Or type 0 to return " << endl;
                        cin >> oid;
                        foundOrder = findOrder(oid);

                        if (oid == "0") {
                            break;
                        }

                        //if order exists
                        if (foundOrder == true) {

                            cout << endl << endl << "        Order Information        " << endl;

                            pqxx::work orderwk(connectionObject);
                            pqxx::result response = orderwk.exec("SELECT * FROM ORDERTB o , PRODUCT p WHERE o.orderid = '" + oid + "' AND o.productid = p.pid ");
                            float totalPrice = 0;
                            for (size_t i = 0; i < response.size(); i++)
                            {
                               // totalPrice += stof(to_string(response[i][8]));
                                totalPrice = totalPrice + (stof(to_string(response[i][8])) * stof(to_string(response[i][5])));
                                cout << response[i][7] << endl << " Price: " << response[i][8] << " QTY: " << response[i][5] << " Shipping Address: " << response[i][4] << " Status: " << response[i][3] << endl;

                            }


                            cout << endl << "Total Price: " << totalPrice << endl;

                        }
                        else {
                            cout << "Order doesnt exist" << endl;
                        }
                    } 

                }

                else if (decision == "5") {
                cout << "exiting" << endl;
                exitCommand = true;
                break;
                }


            }




            //WHILE LOGGED IN
            while (true && loginFlag == true) { 

                cout <<endl<<endl<< "      Welcome " + uid + "!!" << endl << endl;
                cout << "Press 1 to Switch account/Sign out: " << endl;
                cout << "Press 2 to Browse the inventory: " << endl;
                cout << "Press 3 to Manage Cart: " << endl;
                cout << "Press 4 to View Orders: " << endl;
                cout << "Press 5 to exit: " << endl;
                cin >> decision;


                if (decision == "1") {
                    string username;
                    string password;
                    string accDecision;
                    while (true && loginFlag == true) {


                        cout << " Sign Out or Switch account " << endl << endl;
                        cout << "1: Sign out " << endl;
                        cout << "2: Switch account " << endl;
                        cout << "3: Back " << endl;
                        cin >> accDecision;
                        
                        //SIGN OUT
                        if (accDecision == "1") {
                            cout << "              Sign Out             " << endl << endl << endl;
                               
                            //BREAKS SIGNED IN LOOP
                            loginFlag = false;                          
                            uid = "";                                  

                            cout << "     Signed Out!!    " << endl << endl;


                        }
                        //SWITCH ACCOUNT
                        else if (accDecision == "2") {
                            cout <<endl<<endl << "        Login to an Account             " << endl << endl << endl;
                            cout << "     Enter a Username: ";
                            cin >> username;
                            cout << endl ;
                            cout << "     Enter a Password: ";
                            cin >> password;
                            cout << endl <<endl;
                            //find account in table
                            if (findUser(username, password) == true) {
                                cout << "     Login Successfull! " << endl;
                                loginFlag = true;
                                uid = username;
                                break;
                            }
                            else {
                                cout << "     Username or Password entered is not correct. " << endl << endl;

                                break;
                            }
                        }
                        else if (accDecision == "3") {

                            break;
                        }


                    }

                }
                //INVENTORY SEARCH
                else if (decision == "2") {
                    string searchDecision;
                    int cartSelection = 0;
                    string keyword;
                    while (true) {
                       
                        cout << "     Inventory Search   " << endl;
                        cout << "1: Search by keyword " << endl;   
                        cout << "2: SALE Items!!!!" << endl;                         
                        cout << "3: Categories " << endl;    
                        cout << "4: All Items " << endl;   
                        cout << "5: Back " << endl;
                        cin >> searchDecision;
                        cout << endl << endl << endl;

                        //KEYWORD SEARCH
                        if (searchDecision == "1") {    

                            string keyDecision;
                            int productID = 0;
                            bool IDmatch = false;
                            int quantity = 0;
                            cout << "Enter a keyword:  ";
                            cin >> keyword;
                            //LOOP FOR RETURNED ITEMS
                            while (true) {
                                                               
                                pqxx::work searchwk(connectionObject); 
                                //search for word entered in database
                                pqxx::result returned = searchwk.exec("SELECT * FROM product WHERE name = '" + keyword + "' OR name LIKE '" + keyword + "%'");
                                //if word is found
                                if (returned.size() > 0) {
                                    for (int i = 0; i < returned.size(); i++) {
                                        //lists all items that include keyword
                                        if (find(saleItems.begin(), saleItems.end(), to_string(returned[i][1])) != saleItems.end()) {

                                            float salePrice = stof(to_string(returned[i][2])) * .20;
                                            salePrice = stof(to_string(returned[i][2])) - salePrice;
                                            //ON SALE FROM SELECTION
                                            cout << returned[i][1] << endl << " Price: " << returned[i][2] << "  -->  " << salePrice << " ID: " << returned[i][0] << " ON SALE!! " << endl << endl;
                                        }
                                        else {
                                            cout << endl << returned[i][1] << "  " << endl << "Price: " << returned[i][2] << " ID: " << returned[i][0] << endl << endl;
                                        }
                                    }
                                    //if found 3 options can occur
                                    cout << "1: Add to cart" << endl;
                                    cout << "2: New search word" << endl;
                                    cout << "3: back" << endl;
                                    cin >> keyDecision;
                                    //add to cart
                                    if (keyDecision == "1") {
                                        cout << "Enter ID of item to add to Cart: ";
                                        cin >> productID;    
                                        //INPUT VALIDATION
                                        if (cin.fail()) {
                                            cout << endl << "Choose a number " << endl;
                                            cin.clear();
                                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        }
                                        else {                                            
                                              //CHECKS FOR ID
                                            for (int i = 0; i < returned.size();i++) {

                                                if (to_string(returned[i][0]) == to_string(productID)) {
                                                   
                                                    IDmatch = true;
                                                }
                                            }
                                        }                                       
                                       
                                        //ADD TO CART
                                        if (IDmatch == true) {
                                            cout << endl << " Choose Quantity ";
                                            int quantity = 0;
                                            cin >> quantity;                            
                                            if (cin.fail()) {
                                                cout << endl << " Choose a number " << endl;
                                                cin.clear();
                                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                            }
                                            else {
                                            cout << endl << "Added to cart " << endl;
                                            
                                            addtoCart(uid, productID, quantity);
                                            }
                                        }
                                        else {
                                            cout << endl<<endl<<endl<<"Id entered is incorrect. " << endl;
                                        }
                                      
                                    }
                                    //NEW KEYWORD
                                    else if (keyDecision == "2") {

                                        cout << "Enter a new key word" << endl;
                                        cin >> keyword;
                                    }
                                    else if (keyDecision == "3") {
                                        break;
                                    }
                                    else if(cin.fail()) {  
                                        cout <<endl<<endl<<endl<< "Incorrect Input" << endl;                                      
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');                                       

                                    }
                                }
                                else{                                    
                                    cout << "Keyword not found; " << endl;
                                    cout << "Enter a Keyword:   ";
                                    cin >> keyword;                                    
                                }

                            }

                        }
                        //SALE ITEMS
                        else if (searchDecision == "2") {                           
                           //GETS ITEMS ON SALE                          
                           string choice;
                           int productID = 0;
                           cout << "        All Items On Sale     " <<endl <<endl <<endl;

                           pqxx::work salewk(connectionObject); 
                                            
                            //ALL ITEMS FROM PRODUCT TABLE
                           pqxx::result returned = salewk.exec("SELECT * FROM product");  
                         

                           //LISTS ITMES FOUND only shows items on sale                      
                           for (size_t i = 0; i < returned.size(); i++)
                           {
                               //if queried items are included in saleitems.
                               if (find(saleItems.begin(), saleItems.end(), to_string(returned[i][1])) != saleItems.end()) {

                                   float salePrice = stof(to_string(returned[i][2])) * .20;
                                   salePrice = stof(to_string(returned[i][2])) - salePrice;
                                   //ON SALE FROM SELECTION
                                   cout << returned[i][1] << endl << " Price: " << returned[i][2] << "  -->  " << salePrice << " ID: " << returned[i][0] <<  endl << endl;
                               }                             
                           }
                           //FURTHER PROCESS FOR ITEMS FOUND 
                           while (true) {

                               cout << "1: Add to cart" << endl;                              
                               cout << "2: back " << endl;
                               cin >> choice;

                               if (choice == "1") {
                                   bool IDmatch = false;
                                   cout << "Enter ID of Item to add to cart: ";
                                   cin >> productID;                                                             
                                   if (cin.fail()) {
                                       cout << endl << "Choose a number " << endl;
                                       cin.clear();
                                       cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                   }
                                   else { 
                                       cout << endl;
                                       //ID matching
                                       for (int i = 0; i < returned.size();i++) {                                         
                                           if (to_string(returned[i][0]) == to_string(productID)) {                                               
                                               IDmatch = true;
                                           }
                                       }
                                   }

                                   if (IDmatch == true) {
                                       cout << endl << " Choose Quantity ";
                                       int quantity = 0;
                                       cin >> quantity;                               
                                       if (cin.fail()) {
                                           cout << endl << "Incorrect input for Quantity choose a number " << endl;
                                           cin.clear();
                                           cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                       }
                                       else {
                                           cout << endl << endl << "Added to cart!! " << endl << endl;                                           
                                           addtoCart(uid, productID, quantity);
                                       }
                                   }
                                   else {
                                       cout << "Id entered is incorrect. " << endl;
                                   }
                               }
                               
                               else if (choice == "2") {
                                   break;
                               }
                           }

                        }                      

                        //CATEGORIES SEARCH
                        else if (searchDecision == "3") {
                            
                            string categoryDec;
                            int productID = 0;
                            
                            while (true) {
                                pqxx::work searchwk(connectionObject);

                                cout << "     Categories Available     " <<endl <<endl;
                                cout << "1: Vegetables " << endl;
                                cout << "2: Fruits " << endl;                                 
                                cout << "3: Back " << endl;
                                cin >> categoryDec;

                                //VEGETABLES 
                                if (categoryDec == "1") {
                                 
                                    string choice;
                                    //selects all vegetable items from product tbale 
                                    pqxx::result returned = searchwk.exec("SELECT * FROM product WHERE category = 'vegetable'");
                                    cout <<endl<<endl<<endl<< "     Vegetables     " << endl;
                                    
                                    //lists found products for UI
                                    for (int i =0; i < returned.size(); i++){
                                        
                                        if (find(saleItems.begin(), saleItems.end(), to_string(returned[i][1])) != saleItems.end()) {
                                            
                                            float salePrice = stof(to_string(returned[i][2])) * .20;
                                            salePrice = stof(to_string(returned[i][2])) - salePrice;
                                            //ON SALE FROM SELECTION
                                            cout << returned[i][1] << endl << " Price: " << returned[i][2] << "  -->  " << salePrice << " ID: " << returned[i][0] << " ON SALE!! " << endl << endl;
                                        }
                                        else {
                                            cout << endl << returned[i][1] << "  " << endl << "Price: " << returned[i][2] << " ID: " << returned[i][0] << endl << endl;
                                        }
                                    }
                                    //loop for further operations for category chosen
                                    while (true) {

                                        cout << "1: Add to cart" << endl;                                       
                                        cout << "2: back to categories " << endl;
                                        cin >> choice;

                                        if (choice == "1") {
                                            bool IDmatch = false;
                                            cout << "Enter ID of Item to add to cart: ";
                                            cin >> productID;                                                                
                                            if (cin.fail()) {
                                                cout << endl << "Choose a number " << endl;
                                                cin.clear();
                                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                            }
                                            else {
                                                cout << endl;
                                                //ID matching
                                                for (int i = 0; i < returned.size();i++) {                                                   
                                                    if (to_string(returned[i][0]) == to_string(productID)) {

                                                        cout << to_string(returned[i][0]) << "  equals   " << to_string(productID) << endl;
                                                        IDmatch = true;
                                                    }
                                                }
                                                if (IDmatch == true) {
                                                    cout << endl << " Choose Quantity ";
                                                    int quantity = 0;
                                                    cin >> quantity; 
                                                    if (cin.fail()) {
                                                        cout << endl << "Choose a number " << endl;
                                                        cin.clear();
                                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                                    }
                                                    else {
                                                        cout << endl << "Added to cart " << endl;                                                      
                                                        addtoCart(uid, productID, quantity);
                                                    }
                                                }
                                                else {
                                                    cout << "Id entered is incorrect. " << endl;
                                                }
                                            }
                                        }
                                        //back to category selection
                                        else if (choice == "2") {

                                            break; 
                                        }
                                    }
                                }
                                else if (categoryDec == "2") { 
                                 //FRUITS CATEGORY
                                    string choice;
                                    //selects all fruits items from product tbale 
                                    pqxx::result returned = searchwk.exec("SELECT * FROM product WHERE category = 'fruit'");
                                    cout << endl << endl << endl << "     Fruits     " << endl;
                                    //lists found products for UI
                                    for (int i = 0; i < returned.size(); i++) {                                        
                                        if (find(saleItems.begin(), saleItems.end(), to_string(returned[i][1])) != saleItems.end()) {

                                            float salePrice = stof(to_string(returned[i][2])) * .20;
                                            salePrice = stof(to_string(returned[i][2])) - salePrice;
                                            //ON SALE FROM SELECTION
                                            cout << returned[i][1] << endl << " Price: " << returned[i][2] << "  -->  " << salePrice << " ID: " << returned[i][0] << " ON SALE!! " << endl << endl;
                                        }
                                        else {
                                            cout << endl << returned[i][1] << "  " << endl << "Price: " << returned[i][2] << " ID: " << returned[i][0] << endl << endl;
                                        }
                                    }
                                    //further operations for fruit category
                                    while (true) {

                                        cout << "1: Add to cart" << endl;                                       
                                        cout << "2: back to categories " << endl;
                                        cin >> choice;

                                        if (choice == "1") {
                                            bool IDmatch = false;
                                            cout << "Enter ID of Item to add to cart: ";
                                            cin >> productID;     
                                            if (cin.fail()) {
                                                cout << endl << "Choose a number " << endl;
                                                cin.clear();
                                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                            }
                                            else {                                                
                                                cout << endl;
                                                //ID matching
                                                for (int i = 0; i < returned.size();i++) {                                                   
                                                    if (to_string(returned[i][0]) == to_string(productID)) {                                                       
                                                        IDmatch = true;
                                                    }
                                                }
                                                if (IDmatch == true) {
                                                    cout << endl << " Choose Quantity ";
                                                    int quantity = 0;
                                                    cin >> quantity; 
                                                    if (cin.fail()) {
                                                        cout << endl << "Incorrect input for Quantity choose a number " << endl;
                                                        cin.clear();
                                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                                    }
                                                    else {
                                                        cout << endl << endl << "Added to cart!! " << endl << endl;                                                       
                                                        addtoCart(uid, productID, quantity);
                                                    }
                                                }
                                                else {
                                                    cout << "Id entered is incorrect. " << endl;
                                                }
                                            }
                                        }
                                        //back to category selection
                                        else if (choice == "2") {
                                            break;
                                        }
                                    }

                                }
                                else if (categoryDec == "3") {
                                    break;
                                }
                                else if(cin.fail()) { //maybe delete
                                    cout << "Incorrect Input" <<endl;
                                    cin.clear();
                                }


                            }
                        }
                        //ALL INVENTORY 
                        else if (searchDecision == "4") {           
                            //work is used to create a transactional object using connection C  in this case its worker
                            pqxx::work allwk(connectionObject);
                            // used to execute sql statements in transactional mode 
                            

                            //ALL ITEMS FROM PRODUCT TABLE
                            //execexecutes sql statements and returns a result object in this case response.
                            pqxx::result response = allwk.exec("SELECT * FROM product"); 
                          

                            //LISTS ITMES FOUND                           
                            for (size_t i = 0; i < response.size(); i++) 
                            {
                                //if queried items are included in saleitems.
                                if(find(saleItems.begin(), saleItems.end(), to_string(response[i][1])) != saleItems.end()){
                                    
                                    float salePrice = stof(to_string(response[i][2])) * .20;
                                    salePrice = stof(to_string(response[i][2])) - salePrice;
                                    //ON SALE FROM SELECTION
                                    cout << response[i][1] << endl << " Price: " << response[i][2] << "  -->  " << salePrice << " ID: " << response[i][0] << " ON SALE!! " << endl << endl;
                                }
                                else{
                                    cout << response[i][1] << endl << " Price: " << response[i][2] << " ID: " << response[i][0] << endl << endl;
                                }
                            }
                            string cartprocess;
                            

                            
                            while(true){
                                
                                cout << endl << "1: Add an item to cart using ID ";
                                cout << endl << "2: Back" << endl;
                                cin >> cartprocess;

                                if (cartprocess == "1") {
                                    int productID;
                                    bool IDmatch = false;
                                    cout << "Enter ID of Item to add to cart: ";
                                    cin >> productID;                                                                      
                                    if (cin.fail()) {
                                        cout << endl << "Choose a number " << endl;
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    }
                                    else {
                                        cout << endl;
                                        //ID matching
                                        for (int i = 0; i < response.size();i++) {
                                            
                                            if (to_string(response[i][0]) == to_string(productID)) {
                                                
                                                IDmatch = true;
                                            }

                                        }
                                        if (IDmatch == true) {
                                            cout << endl << "Choose Quantity ";
                                            int quantity = 0;
                                            cin >> quantity; 
                                            if (cin.fail()) {
                                                cout << endl << "Incorrect input for Quantity choose a number " << endl;
                                                cin.clear();
                                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                            }
                                            else {
                                                cout << endl << endl << "Added to cart!! " << endl << endl;
                                                // check cart table quantity value
                                                addtoCart(uid, productID, quantity);
                                            }
                                        }
                                        else {
                                            cout << "Id entered is incorrect. " << endl;
                                        }
                                    }                                 

                                }
                                else if (cartprocess == "2") {
                                    break;
                                }
                                else {
                                    cout << "incorrect input " << endl << endl;
                                }
                            } 


                        }
                        else if (searchDecision == "5") {

                            break;
                        }
                        else if(cin.fail()){
                            cout << "Not a Valid entry";
                            cin.clear();
                        }

                    }
                }
                //CART MANAGMENT
                else if (decision == "3" && loginFlag == true) {                 
                  string cartDec;
                  int updatepID;
                  int newQuantity;
                 
                    while (true) {
                        cout <<endl<<endl<<endl<< "        Welcome To Your Cart        " << endl;
                        //LIST ITEMS IN CART
                        pqxx::work cartwk(connectionObject); 
                        pqxx::result response = cartwk.exec("SELECT * FROM PRODUCT p, CART c WHERE p.PID =c.productid AND c.userid = '"+uid+"'"); 
                        float totalPrice = 0;
                        for (size_t i = 0; i < response.size(); i++)
                        {
                            //if queried items are included in saleitems.
                            //calculate totlar price from here
                            if (find(saleItems.begin(), saleItems.end(), to_string(response[i][1])) != saleItems.end()) {

                                float salePrice = stof(to_string(response[i][2])) * .20;
                                salePrice = stof(to_string(response[i][2])) - salePrice;
                                //salePrice = salePrice * stof(to_string(response[i][7]));
                                //ON SALE FROM SELECTION
                               // totalPrice += salePrice;
                                cout << response[i][1] << endl << " Price: " << response[i][2] << "  --> Sale  " << salePrice << " ID: " << response[i][0] << " qty: " <<response[i][7] << endl;

                                salePrice = salePrice * stof(to_string(response[i][7]));
                                totalPrice += salePrice;


                            }
                            else {
                                totalPrice =totalPrice+ (stof(to_string(response[i][2]))*stof(to_string(response[i][7])));
                                cout << response[i][1] << endl << " Price: " << response[i][2] << " ID: " << response[i][0] << " qty: "<< response[i][7] << endl;
                            }
                        }
                        cout << endl << "Total Cart Amount: " << totalPrice <<endl; 
                        cout << endl << endl << "1: Remove Item " << endl;
                        cout << "2: Change quantity " << endl;
                        cout << "3: Checkout " << endl;
                        cout << "4: Back " << endl << endl;

                        cin >> cartDec;

                        if (cartDec == "1"){
                            
                                cout << endl << "Enter Id Of Item To Remove: ";
                                cin >> updatepID;                                      
                                if (cin.fail()) {
                                    cout << endl << "Choose a number " << endl;
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                }
                                else {
                                    removeFromCart(uid, updatepID);
                                }                        

                        }
                        else if (cartDec == "2"){
                                cout << "Enter Id of Item to Update: ";
                                cin >> updatepID;                                    
                                if (cin.fail()) {
                                    cout << endl << "Choose a number " << endl;
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                }
                                else {
                                    cout << endl << "Enter new qunatity: " << endl;
                                    cin >> newQuantity;                                    
                                    if (cin.fail()) {
                                        cout << endl << "Choose a number " << endl;
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    }
                                    else {
                                        updateQuantityCart(uid, updatepID, newQuantity);
                                    }                                                                   
                                }
                        }
                       
                        else if (cartDec == "3") {
                            
                            string orderID;
                            string orderDec;
                            cout << endl<<endl << "        Check Out Order        " <<endl <<endl;

                            cout << "Are you sure you want to check out" <<endl;
                            cout << "1: YES " << endl;
                            cout << "2: NO " << endl;
                            cin >> orderDec;

                            if (orderDec == "1"){
                            
                                //puts all of cart items into order table nice..
                                orderID = cartToOrder(uid);
                                

                                //update user ordernumbers by 1
                                updateOrderNumbers(uid);

                                //now delete items form users cart table
                                removeAllCart(uid);

                                //order is now placed
                                //and order number is given to customer to check order in future;
                                cout <<endl<<endl<<endl<< "Your OrderID is: " << orderID<<endl;
                            }
                            else if (orderDec == "2") {

                                break;

                            }
                                
                        }
                        else if (cartDec == "4") {
                             break;
                        }
                        
                    }

                }
                else if (decision == "3" && loginFlag == false) {
                    cout << "first login or create an account to manage cart. " << endl; 
                }


                //View ORDER PAGE
                else if (decision == "4") {
                string oid;
                bool foundOrder;
                    cout << "       View Order       " << endl <<endl <<endl;

                    //SEARCH FOR ORDER
                    while (true) {
                        cout << endl << "Enter An OrderID: " << endl;
                        cout << "Or type 0 to return " << endl;
                        cin >> oid;
                        foundOrder = findOrder(oid);

                        if (oid == "0") {
                            break;
                        }

                        //if order exists
                        if (foundOrder == true) {

                            cout << endl << endl << "        Order Information        " << endl;


                            pqxx::work orderwk(connectionObject);
                            pqxx::result response = orderwk.exec("SELECT * FROM ORDERTB o , PRODUCT p WHERE o.orderid = '" + oid + "' AND o.productid = p.pid ");
                            float totalPrice = 0;
                            for (size_t i = 0; i < response.size(); i++)
                            {
                               // totalPrice += stof(to_string(response[i][8]));
                                totalPrice = totalPrice + (stof(to_string(response[i][8])) * stof(to_string(response[i][5]))); 
                                cout << response[i][7] << endl << " Price: " << response[i][8] << " QTY: " << response[i][5] << " Shipping Address: " << response[i][4] << " Status: " << response[i][3] << endl;

                            }


                            cout << endl << "Total Price: " << totalPrice << endl;

                        }
                        else {
                            cout << "Order doesnt exist" << endl;
                        }
                    }                                                      

                }
                else if (decision == "5") {
                    cout << "exiting" << endl;
                    exitCommand = true;
                    break;
                }
            }


    }

        connectionObject.close();
   }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

    system("pause");  
}
