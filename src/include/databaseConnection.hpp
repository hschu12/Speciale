#include <sqlite3.h>
#include <iostream>
#include <vector>
#include "HyperGraph.hpp"
#include <chrono>
#include <omp.h>
#include <string.h>

std::vector<int> reactions;
/*std::vector<int> educts;
#pragma threadprivate(educts)
std::vector<int> products;
#pragma threadprivate(products)
*/

int getReactionList(void *NotUsed, int argc, char **argv, char **azColName) {
    
    for (int i = 0; i < argc; i++) {
    	reactions.push_back(atoi(argv[i]));
    }
        
    return 0;
}

/*int getEducts(void *NotUsed, int argc, char **argv, char **azColName) {
    std::cout << "start" << std::endl;
    for (int i = 0; i < argc; i++) {
    	educts.push_back(atoi(argv[i]));
    }
    std::cout << "finished" << std::endl;

    return 0;
}

int getProducts(void *NotUsed, int argc, char **argv, char **azColName) {
 
 	std::cout << "start" << std::endl;

    for (int i = 0; i < argc; i++) {
    	products.push_back(atoi(argv[i]));
    }
    std::cout << "finished" << std::endl;

    return 0;
}
*/

HyperGraph createGraph(int numberOfCompounds, int numberOfReactions)
{
   	HyperGraph h (numberOfCompounds, numberOfReactions);
  	auto start = std::chrono::high_resolution_clock::now();
	sqlite3 *db;
	sqlite3_stmt *res;

	int rc;
	char *err_msg = 0;
	const char *sql;

	rc = sqlite3_open("src/databasefolder/test.db", &db);  
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

	std::cout << "Start creating graph" << std::endl;
	for (auto reaction : reactions) {	
		std::vector <int> educts;
		std::vector <int> products;	

		
		char sql1[100];
		const char *base1 = "SELECT educt_id FROM educts WHERE reaction_id = ";
		const char *elm1 = std::to_string(reaction).c_str();
		strcpy(sql1, base1);
		strcat(sql1, elm1);
				
		sqlite3_prepare_v2(db, sql1, -1, &res, 0);
		
		while (sqlite3_step(res) == SQLITE_ROW) {
			const char* line = (const char * ) sqlite3_column_text(res, 0);
    		educts.push_back(atoi(line));
		}
	
		sqlite3_finalize(res);
					
		char sql2[100];
		const char *base2 = "SELECT product_id FROM products WHERE reaction_id = ";
		const char *elm2 = std::to_string(reaction).c_str();
		strcpy(sql2, base2);
		strcat(sql2, elm2);
					
		sqlite3_prepare_v2(db, sql2, -1, &res, 0);
	

		while (sqlite3_step(res) == SQLITE_ROW) {
			const char* line = (const char * ) sqlite3_column_text(res, 0);
    		products.push_back(atoi(line));
		}
	
		sqlite3_finalize(res);
					
		h.addReaction(reaction, products, educts, 1);
	}
	std::cout << "Done!" << std::endl;
	sqlite3_close(db);
  	auto end = std::chrono::high_resolution_clock::now();
	auto dur = end - start;
	auto i_millis = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
	std::cout << i_millis.count() << "milliseconds" << '\n';

	return h;
}