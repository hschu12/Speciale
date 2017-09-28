#include <sqlite3.h>
#include <iostream>
#include <vector>
#include "include/HyperGraph.hpp"

std::vector<int> reactions;
std::vector<int> educts;
std::vector<int> products;


int getReactionList(void *NotUsed, int argc, char **argv, char **azColName) {
    
    NotUsed = 0;
    
    for (int i = 0; i < argc; i++) {
    	reactions.push_back(atoi(argv[i]));
    }
        
    return 0;
}

int getEducts(void *NotUsed, int argc, char **argv, char **azColName) {
    
    NotUsed = 0;

    educts.erase(educts.begin(),educts.end());
    
    for (int i = 0; i < argc; i++) {
    	educts.push_back(atoi(argv[i]));
    }
        
    return 0;
}

int getProducts(void *NotUsed, int argc, char **argv, char **azColName) {
    
    NotUsed = 0;
    
    products.erase(products.begin(),products.end());
    
    for (int i = 0; i < argc; i++) {
    	products.push_back(atoi(argv[i]));
    }
        
    return 0;
}


HyperGraph createGraph()
{
   	HyperGraph h;

	sqlite3 *db;
	sqlite3_stmt *res;
	int rc;
	char *err_msg = 0;
	const char *sql;

	rc = sqlite3_open("databasefolder/test.db", &db);  
	if (rc == SQLITE_OK) {
		std::cout << "Connection Succes" << std::endl;
	}
	else {
		std::cout << "Connection Failed" << std::endl;	
		sqlite3_close(db);
		exit(1);
	}

	sql = "SELECT reaction_id FROM reactions";
	sqlite3_exec(db, sql, getReactionList, 0, &err_msg);

	for (auto reaction : reactions) {
		std::cout << "reaction" << std::endl;
		
		char sql1[100];
		const char *base1 = "SELECT educt_id FROM educts WHERE reaction_id = ";
		const char *elm1 = std::to_string(reaction).c_str();
		strcpy(sql1, base1);
		strcat(sql1, elm1);
		
		sqlite3_exec(db, sql1, getEducts, 0, &err_msg);

		char sql2[100];
		const char *base2 = "SELECT product_id FROM products WHERE reaction_id = ";
		const char *elm2 = std::to_string(reaction).c_str();
		strcpy(sql2, base2);
		strcat(sql2, elm2);
		
		sqlite3_exec(db, sql2, getProducts, 0, &err_msg);



		std::cout << "" << std::endl;

	}


	sqlite3_close(db);

	return h;
}