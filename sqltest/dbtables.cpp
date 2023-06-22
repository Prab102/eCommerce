#include<iostream>
#include<pqxx/pqxx>
#include<string>
#include"dbtables.h"



using namespace std;
using namespace pqxx;

string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";


void insertNewUser(string username,string pass){

   // string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";  


    try
    {
        //connects db, if created C is used to make connection object
        pqxx::connection connectionObject(Conn.c_str());
        pqxx::work wk1(connectionObject);
        pqxx::result returned = wk1.exec("SELECT * FROM users WHERE username = '" + username + "'");
       

        if (to_string(returned[0][0]) == username) {

            cout <<endl<<endl<< "Username already exists"<<endl;
        }
        else {
            string ins_user = "INSERT INTO USERS( USERNAME, PASSWORD)"
                "VALUES('" + username + "','" + pass + "');";

            
            cout << endl << endl << "     Account Created Successfully!     " << endl <<endl<< "     Now Login To Your Account:   " << endl;
            wk1.exec(ins_user);
            wk1.commit();
        }

    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

   // system("pause");

}
void addtoCart(string uid,int pid,int quantity) {

  //  string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";  


    try
    {
        //connects db, if created C is used to make connection object
        pqxx::connection connectionObject(Conn.c_str());

       string ins_user = "INSERT INTO CART(USERID, PRODUCTID, QUANTITY)"
            "VALUES('"+uid+"','"+ to_string(pid) +"','" + to_string(quantity) + "' );";


        pqxx::work wk2(connectionObject);
        wk2.exec(ins_user);
        wk2.commit();


    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

    // system("pause");



}
void removeFromCart(string username,int productID) {

 //   string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";  


    try
    {
        
        pqxx::connection connectionObject(Conn.c_str());

        string removescr = "DELETE FROM CART WHERE userid ='"+ username +"' AND productid ='"+ to_string(productID) +"' ";
            


        pqxx::work wk2(connectionObject);
        wk2.exec(removescr);
        wk2.commit();


    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }


}
void updateQuantityCart(string username, int productID, int newQuantity) {

  //  string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";  


    try
    {
       
        pqxx::connection connectionObject(Conn.c_str());

        string updquan = "UPDATE CART SET quantity ='"+to_string(newQuantity)+"' WHERE userid ='" + username + "' AND productid ='" + to_string(productID) + "' ";

        pqxx::work wk2(connectionObject);
        wk2.exec(updquan);
        wk2.commit();


    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }
    


}
string cartToOrder(string username) {

  //  string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";  


    try
    {
        //connects db, if created C is used to make connection object
        pqxx::connection connectionObject(Conn.c_str());
       
        string shippingaddress;

        cout << "Enter a shipping address: " << endl;
        cin >> shippingaddress;
        //generate orderid
        string orderID;
        //combine username and ordernum
        pqxx::work onum(connectionObject);
        pqxx::result returned = onum.exec("SELECT numorders FROM USERS WHERE username = '" + username + "'");
        orderID = username + to_string(returned[0][0]);


        //each item in users cart

        pqxx::result cartproduct = onum.exec("SELECT * FROM CART WHERE userid = '" + username + "'");
        //loop through each item user cart
        for (size_t i = 0; i < cartproduct.size(); i++){
        
            string cTo = "INSERT INTO ORDERTB(ORDERID, USERID, PRODUCTID, STATUS,SHIPPINGADDRESS,QUANTITY)"
                "VALUES('"+ orderID +"','" + username + "','" + to_string(cartproduct[i][1]) + "',DEFAULT,'" + shippingaddress + "', '"+ to_string(cartproduct[i][2]) + "' );";
                
            onum.exec(cTo);
          
        }
        
        
        onum.commit();

        return orderID;
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

    

}
void updateOrderNumbers(string username) {

   // string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";  


    try
    {
        int updateNumOrders;
        //connects db, if created C is used to make connection object
        pqxx::connection connectionObject(Conn.c_str());
        pqxx::work wk(connectionObject);

        //GETS CURRENT NUMORDERS
        pqxx::result userOrder = wk.exec("SELECT numorders FROM USERS WHERE username = '" + username + "'");

        //increment by 1
        updateNumOrders = stoi(to_string(userOrder[0][0])); 
        updateNumOrders += 1;

        //INCREASE USER ORDER BY 1 CALLED WHEN ORDER IS SSUBMITTED
        string increaseTotalOrders = "UPDATE USERS SET numorders = '"+ to_string(updateNumOrders) +"' WHERE username ='" + username + "' ";

       

      
        wk.exec(increaseTotalOrders);
        wk.commit();


    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

}

void removeAllCart(string username) {


  //  string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";  


    try
    {
        //connects db, if created C is used to make connection object
        pqxx::connection connectionObject(Conn.c_str());

        string removecart = "DELETE FROM CART WHERE userid ='" + username + "' ";



        pqxx::work wk2(connectionObject);
        wk2.exec(removecart);
        wk2.commit();


    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }




} 

bool findOrder(string orderID) {

   // string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";  


    try
    {
        
        pqxx::connection connectionObject(Conn.c_str());
        bool found = false;
        
        pqxx::work wk(connectionObject);
        pqxx::result order = wk.exec(" SELECT * FROM ORDERTB WHERE orderid = '" + orderID + "'");

        if (order.size() >= 1) {

            found = true;
            cout << "found";
        }

        return found;


    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }


}
bool findUser(string username, string password) {

   // string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";  


    try
    {
        //connects db, if created C is used to make connection object
        pqxx::connection connectionObject(Conn.c_str());
        bool found = false;
        
        pqxx::work wk(connectionObject);
        pqxx::result user = wk.exec(" SELECT * FROM users WHERE username = '" + username + "' AND password = '" + password + "'");

        
        if (user.size() == 1) {

            found = true;
        }

        return found;


    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }


}
vector<string> saleList() { 

    vector<string> sale = { "apples","pineapples","cantaloupe","mangoes","limes","oranges","garlic","spinach","cucumbers" };

    return sale;
}

void fillDb() {

    
      //  string Conn = "host=localhost port=5432 dbname=eCom user=postgres password =1";  
        const char* productTb;
        const char* userTb;
        const char* cartTb;
        const char* orderTb;

        try
        {
           
            pqxx::connection connectionObject(Conn.c_str());

            // create tables here
            
            productTb = "CREATE TABLE IF NOT EXISTS PRODUCT("      
                "PID             SERIAL PRIMARY KEY,"
                "NAME            TEXT   NOT NULL, "
                "PRICE           NUMERIC  NOT NULL, "
                "CATEGORY        TEXT    NOT NULL, "
                "STOCK           INT    NOT NULL );";
            //makes transactional obj
            pqxx::work W(connectionObject); 

            //executes and commit to DB
            W.exec(productTb);
            W.commit();
            cout << "product table created successfully" << endl;


            userTb = "CREATE TABLE IF NOT EXISTS USERS("
                "USERNAME          VARCHAR(100)   PRIMARY KEY, "
                "PASSWORD          VARCHAR(100)   NOT NULL, "
                "NUMORDERS         INT            DEFAULT '0'); ";   

           // userTb = "ALTER TABLE USERS ADD NUMORDERS  INT DEFAULT '0' ";

            //makes transactional obj
            pqxx::work W1(connectionObject);

            //executes and commit to DB
            W1.exec(userTb);
            W1.commit();
            cout << "USERS table created successfully" << endl;

            cartTb = "CREATE TABLE IF NOT EXISTS CART("
                "USERID             VARCHAR(100)  REFERENCES USERS(USERNAME) , "
                "PRODUCTID          INT           REFERENCES PRODUCT(PID), "
                "QUANTITY           INT           NOT NULL );";
            //makes transactional obj
            pqxx::work W2(connectionObject);

            //executes and commit to DB
            W2.exec(cartTb);
            W2.commit();
            cout << "Cart table created successfully" << endl;

            orderTb = "CREATE TABLE IF NOT EXISTS ORDERTB("
                "ORDERID            VARCHAR(100)   NOT NULL , "
                "USERID             VARCHAR(100)   REFERENCES USERS(USERNAME) , "
                "PRODUCTID          INT            REFERENCES PRODUCT(PID), "
                "STATUS             VARCHAR(100)   DEFAULT 'Incomplete' , "
                "SHIPPINGADDRESS    TEXT           NOT NULL ) ,"   
                "QUANTITY           INT           NOT NULL );";
                


            // orderTb = "ALTER TABLE ORDERTB ADD QUANTITY  INT NOT NULL ";
            //makes transactional obj
            pqxx::work W3(connectionObject);

            //executes and commit to DB
            W3.exec(orderTb);
            W3.commit();
            cout << "ORDER table created successfully" << endl;
            
            
            /*
            //fill product table  (can only run once)
            string ins_products = "INSERT INTO PRODUCT (PID,NAME, PRICE,CATEGORY, STOCK) "
                "VALUES (DEFAULT,'carrot' ,0.99,'vegetable', 100),(DEFAULT,'onion',1.50,'vegetable',100),(DEFAULT,'potatoes',3.99,'vegetable',100),"
                "(DEFAULT,'tomatoes' ,1.59,'vegetable', 100),(DEFAULT,'bell peppers',4.84,'vegetable',100),(DEFAULT,'broccoli',1.99,'vegetable',100),"
                "(DEFAULT,'cucumbers' ,2.83,'vegetable', 100),(DEFAULT,'lettuce',1.69,'vegetable',100),(DEFAULT,'celery',2.52,'vegetable',100),"
                "(DEFAULT,'mushrooms' ,13.46,'vegetable', 100),(DEFAULT,'corn',6.53,'vegetable',100),(DEFAULT,'garlic',6.50,'vegetable',100),"
                "(DEFAULT,'spinach' ,6.56,'vegetable', 100),(DEFAULT,'green beans',2.13,'vegetable',100),(DEFAULT,'cabbage',1.15,'vegetable',100),"
                "(DEFAULT,'sweet potatoes' ,1.75,'vegetable', 100),(DEFAULT,'green onions',0.88,'vegetable',100),(DEFAULT,'cauliflower',4.03,'vegetable',100),"
                "(DEFAULT,'asparagus' ,4.59,'vegetable', 100),(DEFAULT,'plums',1.74,'fruit',100),(DEFAULT,'mangoes',1.13,'fruit',100),"
                "(DEFAULT,'bananas',0.99,'fruit',100),(DEFAULT,'apples',2.31,'fruit',100),(DEFAULT,'strawberries',1.99,'fruit',100),"
                "(DEFAULT,'grapes',2.49,'fruit',100),(DEFAULT,'watermelon',7.98,'fruit',100),(DEFAULT,'oranges',1.34,'fruit',100),"
                "(DEFAULT,'blueberries',3.80,'fruit',100),(DEFAULT,'lemons',1.61,'fruit',100),(DEFAULT,'peaches',1.99,'fruit',100),"
                "(DEFAULT,'avacadoes',1.79,'fruit',100),(DEFAULT,'pineapples',4.50,'fruit',100),(DEFAULT,'cherries',5.54,'fruit',100),"
                "(DEFAULT,'cantaloupe',2.87,'fruit',100),(DEFAULT,'raspberries',2.32,'fruit',100),(DEFAULT,'pears',2.00,'fruit',100),"
                "(DEFAULT,'limes',2.31,'fruit',100),(DEFAULT,'blackberries',1.58,'fruit',100),(DEFAULT,'clemintine',3.98,'fruit',100);";



            pqxx::work wk(connectionObject);
            wk.exec(ins_products);
            wk.commit();       
            */
            
        }
        catch (const exception& e)
        {
            cerr << e.what() << endl;
        }

        system("pause");

}


