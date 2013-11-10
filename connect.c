#include <stdlib.h>
#include <libpq-fe.h>
#include <stdio.h>

void doSQL(PGconn *conn, char *command) {
  PGresult *result;

  printf("%s\n", command);

  result = PQexec(conn, command);
  printf("status is %s\n", PQresStatus(PQresultStatus(result)));
  printf("#rows affected %s\n", PQcmdTuples(result));
  printf("result message: %s\n", PQresultErrorMessage(result));

  PQclear(result);
}


void dropTable(PGconn *conn) {
  doSQL(conn, "DROP TABLE footballer");
}

void createTable(PGconn *conn) {
  doSQL(conn, "CREATE TABLE footballer(idfootballer SERIAL PRIMARY KEY, name VARCHAR, lastName VARCHAR, birthday DATE, salary MONEY, avarageNumberOfGoals numeric, numberOfAssists int)");
}

void addFootbaler(PGconn *conn, char *name, char* lastName, char* birthday, double salary, double avgGoals, int assists) {
  char *command;
  sprintf(command, "INSERT INTO footballer(name, lastName, birthday, salary, avarageNumberOfGoals, numberOfAssists) VALUES(%s, %s, %s, %f, %f, %d)", name, lastName, birthday, salary, avgGoals, assists), 
    doSQL(conn, command);
}

void deleteFootballer(PGconn *conn) {
  printf("Podaj id do usuniecia: ");
  int id;
  scanf("%d", &id);
  char *command;
  sprintf(command, "DELETE FROM footballer WHERE id=%d", id);
  doSQL(conn, command);
}


int main()
{
  PGresult *result;
  PGconn *conn;

  conn = PQconnectdb("host=localhost port=5432 dbname=dbappsproject1 user=aleksander password=12345abc");
  if(PQstatus(conn) == CONNECTION_OK) {
    char z[2];
    do {    
      printf("\n");
      printf("MENU\n");
      printf("0 - Utworz tabele\n");
      printf("1 - Usun tabele\n");
      printf("2 - Wyswietl zawartosc\n");
      printf("3 - Dodaj do tabeli\n");
      printf("4 - Usun rekord z tabeli\n");
      printf("5 - Modyfikuj rekord\n");
      printf("6 - Wyszukaj po id\n");
      printf("7 - Wyszukaj pilkarzy o zarobkach wiekszych niz dane\n");
      printf("8 - Utworz wyzwalacz\n");
      printf("9 - Usun wyzwalacz\n");
      printf("q - Zakoncz\n");
      printf("Wybierz opcje: ");
      fgets(z, 100, stdin);
      printf("\n");
      switch (z[0]) {
      case '0' : {
	createTable(conn);
	break;
      }
      case '1' : {
	dropTable(conn);
	break;
      }
      case '2' : {
	break;
      }
      case '3' : {
	break;
      }
      case '4' : {
	break;
      }
      case '5' : {
	break;
      }
      case '6' : {
	break;
      }
      case '7' : {
	break;
      }
      case '8' : {
	break;
      }
      case '9' : {
	break;
      }

      }
      
    } while(z[0] != 'q');
  }
  else 
    printf("connection failed: %s\n", PQerrorMessage(conn));

  PQfinish(conn);
  return EXIT_SUCCESS;
}
