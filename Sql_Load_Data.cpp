/*---------------------------------------------------------------*/
/*																 */
/* Programa:   load_data.c										 */
/*																 */
/* Objetivo: Conectarse a una base de datos MySQL y hacer una	 */
/*			 operación tipo LOAD DATA [LOCAL] INFILE...			 */
/*																 */
/*---------------------------------------------------------------*/

#include "stdafx.h"
//#include "WebGrabber.h"
//#include "WebGrabberDlg.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#pragma comment(lib,"libmySQL.lib")


void sql_dump()
{

  MYSQL my_connection;
  int res;
  mysql_init(&my_connection);

  /*mysql_real_connect(&mysql,host,user,passwd,dbname,0,NULL,0) == NULL)*/

  if (mysql_real_connect(&my_connection, "localhost", "root", "enrique","smi",0,NULL,CLIENT_FOUND_ROWS)) {   
        printf("Connection success\n");   
        
		res = mysql_query(&my_connection, "delete from webgrabber;");
        if (!res)   {      
                printf("Deleted %lu rows\n",(unsigned long)mysql_affected_rows(&my_connection));
        }   else   {      
                fprintf(stderr, "Delete error %d: s\n",mysql_errno,(&my_connection),mysql_error(&my_connection));   
        }  
		res = mysql_query(&my_connection, "load data local infile 'c:/bankpymenet/webgrabber.csv' into table webgrabber fields terminated by ',' lines terminated by '\n';");
        if (!res)   {      
                printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(&my_connection));
        }   else   {      
                fprintf(stderr, "Insert error %d: s\n",mysql_errno,(&my_connection),mysql_error(&my_connection));   
        }   

        mysql_close(&my_connection);
   } else {   
        fprintf(stderr, "Connection failed\n");   
        if (mysql_errno(&my_connection))   {      
                 fprintf(stderr, "Connection error %d: %s\n",      
                           mysql_errno(&my_connection),       mysql_error(&my_connection));      
        }
  }

return EXIT_SUCCESS;
}