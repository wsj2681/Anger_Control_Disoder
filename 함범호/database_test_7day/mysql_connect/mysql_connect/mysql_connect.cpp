#include <stdio.h>
#include <winsock2.h>
#include <mysql.h>

#include <iostream>
#pragma comment(lib,"libmysql.lib")


const char* host = "localhost";
const char* user = "root";
const char* pw = "1234";
const char* db = "shopdb";


int main() {

	////printf("MYSQL client version : %s\n", mysql_get_client_info);

	//MYSQL mysql;
	//mysql_init(&mysql);

	//if(!mysql_real_connet(&mysql, "" ))

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES * sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL) //  ���� ���� ó��
	{
		printf("%d ���� : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}
	else 
	{
		printf("���� ����\n"); //���Ἲ�� �޼��� ���

		if (mysql_select_db(&conn, db)) // �����ͺ��̽� ����
		{
			printf("%d ���� : %s, %d\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;


		}
		
		const char* query = "select *from all";
		int state = 0;

		state = mysql_query(connection, query);
		if (state == 0) {
			sql_result = mysql_store_result(connection);	// Result set�� ����
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) // Result set ���� 1���� �迭�� ������
			{
				printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2],  sql_row[3]);	// ����� �迭�� ���
			}
			mysql_free_result(sql_result); // Result set ����
		}
		
		mysql_close(connection); //db���� ����
	}


}