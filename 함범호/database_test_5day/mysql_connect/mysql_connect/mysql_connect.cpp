#include <mysqlx_version.h>
#include <winsock2.h>
#include <mysql.h>

#include <iostream>
#pragma comment(lib,"libmysql.lib")


int main() {

	//printf("MYSQL client version : %s\n", mysql_get_client_info);

	MYSQL mysql;
	mysql_init(&mysql);

	if(!mysql_real_connet(&mysql, "" ))
}