#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <mysql/errmsg.h>

MYSQL mysql;

void fai_query1();
#if 0
static char *server_args[] = {
  "this_program",       /* this string is not used */
  "--datadir=.",
  "--key_buffer_size=32M"
};
static char *server_groups[] = {
  "embedded",
  "server",
  "this_program_SERVER",
  (char *)NULL
};

int main(void) {
  mysql_server_init(sizeof(server_args) / sizeof(char *),
                    server_args, server_groups);

  /* Use any MySQL API functions here */

  mysql_server_end();

  return EXIT_SUCCESS;
}
#endif

int main(int argc, char *argv[])
{
	char password[21]; 
	char *pc;
	char *foo;

	if(argc > 1) foo=argv[1]; // avoids warnings !!!

	printf( " Please enter password for user gabo : " );
	fgets( password, 20, stdin); putchar('\n');
	pc = &password[strlen(password)-1];
	if( pc[0] == '\n' || pc[0] == '\r' ) pc[0] = '\0';
	printf( "Password is (%s)\n", password );
	
	mysql_init(&mysql);
	mysql_options(&mysql,MYSQL_OPT_COMPRESS,0);
	// mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"odbc");
	if (!mysql_real_connect(&mysql,
											"localhost","gabo",password,"divelogbook",
												0,NULL,0))
	{
				fprintf(stderr, "Failed to connect to database: Error: %s\n",
											mysql_error(&mysql));
	}

	fai_query1();

	return 0;
}

void
fai_query1()
{
	char query_string[1001];
	MYSQL_RES *result;
  MYSQL_ROW row;
  // MYSQL_ROW end_row;
	unsigned int num_fields;
	unsigned int num_rows;
	int query;
	register unsigned int i;
	unsigned long *lengths;



	strcpy(query_string, "SELECT * from log where code > 20" );
	if ((query=mysql_real_query(&mysql,query_string,strlen(query_string))))
	{
			// error
			printf( "Query failed !\n");
			printf( "Error is : (%s) \n ",mysql_error(&mysql) );
			printf( "Error is : (%d) : ",query );
			switch(query)
			{
				case CR_COMMANDS_OUT_OF_SYNC:
						printf( "Commands were executed in an improper order. \n" );
						break;
				case CR_SERVER_GONE_ERROR:
						printf( "The MySQL server has gone away. \n" );
						break;
				case CR_SERVER_LOST:
						printf( "The connection to the server was lost during the query. \n" );
						break;
				case CR_UNKNOWN_ERROR:
						printf( "An unknown error occurred. \n" );
						break;
				default:
						printf( "I don't know how to interpret this damn error!. \n" );
						break;
			}


	}
	else // query succeeded, process any data returned by it
	{
			result = mysql_store_result(&mysql);
			if (result)  // there are rows
			{
					num_fields = mysql_num_fields(result);
					printf("Ok: there are %d fields!\n", num_fields );
					// retrieve rows, then call mysql_free_result(result)

					/*
					while ((row = mysql_fetch_row(result)))
					{
						(void)fputs(">> ", stdout);
						for (end_row = row + num_fields; row < end_row; ++row)
							(void)printf("%s\t", row ? (char*)*row : "NULL");
						(void)fputc('\n', stdout);
					}
					(void)fputc('\n', stdout);
					*/

					while ((row = mysql_fetch_row(result)))
					{
						for(i = 0; i < num_fields; i++)
						{
								 lengths = mysql_fetch_lengths(result);
								 // printf("Column %u is %lu bytes and keeps [%s].\n", i, lengths[i], row[i] ? row[i] : "NULL" );
								 printf("[%s]\t", row[i] ? row[i] : "NULL" );
						}
						putchar('\n');
					}
					putchar('\n'); putchar('\n');

			}
			else  // mysql_store_result() returned nothing; should it have?
			{
					if(mysql_field_count(&mysql) == 0)
					{
							// query does not return data
							// (it was not a SELECT)
							num_rows = mysql_affected_rows(&mysql);
						printf("Not a query: %d rows have been affected!\n", num_rows );
					}
					else // mysql_store_result() should have returned data
					{
							fprintf(stderr, "Error: %s\n", mysql_error(&mysql));
					}
			}
	}

	mysql_close(&mysql);


	return;
}
