#include <stdlib.h>
#include <libpq-fe.h>
#include <stdio.h>
#include <string.h>

void doSQL(PGconn *conn, char *command) {
  PGresult *result;
  result = PQexec(conn, command);
  printf("status is %s\n", PQresStatus(PQresultStatus(result)));
  printf("#rows affected %s\n", PQcmdTuples(result));
  printf("result message: %s\n", PQresultErrorMessage(result));

  switch(PQresultStatus(result)) {
  case PGRES_TUPLES_OK:
    {
      int m, n;
      int nrows = PQntuples(result);
      int nfields = PQnfields(result);
      for(m = 0; m < nrows; m++) {
	for(n = 0; n < nfields; n++) {
	  printf(" %s = %s,", PQfname(result, n), PQgetvalue(result, m, n));
	    }
	printf("\n");
      }
    }
  }
  PQclear(result); 
}


void dropTable(PGconn *conn) {
  doSQL(conn, "DROP TABLE footballer");
}

void createTable(PGconn *conn) {
  doSQL(conn, "CREATE TABLE footballer(idfootballer SERIAL PRIMARY KEY, name VARCHAR, lastName VARCHAR, birthday DATE, salary MONEY, avarageNumberOfGoals numeric, numberOfAssists int)");
}

void addFootballer(PGconn *conn) {
  char command[1000];
  char *name = (char *)malloc(100*sizeof(char));
  char *lastName =(char *)malloc(100*sizeof(char)); 
  char *birthday = (char *)malloc(100*sizeof(char));
  float salary, avgGoals;
  int assists;
  printf("Podaj imie: ");
  scanf("%s", name);
  printf("Podaj nazwisko: ");
  scanf("%s", lastName);
  printf("Podaj date urodzin (DD-MM-RRRR): ");
  scanf("%s", birthday);
  printf("Podaj zarobki: ");
  scanf("%f", &salary);
  printf("Podaj srednia liczbe golli: ");
  scanf("%f", &avgGoals);
  printf("Podaj liczbe asyst: ");
  scanf("%d", &assists);
  getchar();
  sprintf(command, "INSERT INTO footballer(name, lastName, birthday, salary, avarageNumberOfGoals, numberOfAssists) VALUES ('%s', '%s', '%s', %f, %f, %d)", name, lastName, birthday, salary, avgGoals, assists), 
    printf("%s\n", command);
  doSQL(conn, command);
}

void updateFootballer(PGconn *conn) {
  printf("Podaj id do modyfikacji: ");
  int id;
  scanf("%d", &id);
  getchar();
  char command[100];
  sprintf(command, "SELECT * FROM footballer WHERE idfootballer=%d", id);
  PGresult *result;
  result = PQexec(conn, command);
  if (PQresultStatus(result) == PGRES_TUPLES_OK) {
    int nrows = PQntuples(result);
    if (nrows == 0) {
      printf("Nie ma rekordu o takim id\n");
      return;
    }
    int m;
    int nfields = PQnfields(result);
    printf("\n");
    for (m = 0; m < nfields; m++) {
      printf("%s  ", PQfname(result, m));
    }
    printf("\n");
    for (m = 0; m < nfields; m++) {
      printf("%s\t", PQgetvalue(result, 0, m));
    }
    printf("\n");
    char *name = PQgetvalue(result, 0, 1);
    char *lastName = PQgetvalue(result, 0, 2);
    char *date = PQgetvalue(result, 0, 3);
    char *wholeSalary = PQgetvalue(result, 0, 4);
    char *sSalary = (char *)malloc(strlen(wholeSalary)*sizeof(char));
    strcpy(sSalary, wholeSalary+1);
    float salary = atof(sSalary);
    float avgGoals = atof(PQgetvalue(result, 0, 5));
    int assists = atoi(PQgetvalue(result, 0, 6));
    char ans;
    printf("Czy chcesz zmienic imie? (y jesli tak): ");
    if ( (ans=getchar()) == 'y') {
      printf("Podaj nowe imie: ");
      char newName[100];
      scanf("%s", newName);
      getchar();
      name = (char *)malloc((strlen(newName)+1)*sizeof(char));
      strcpy(name, newName);
    }
    else {
      getchar();
    }
    printf("Czy chcesz zmienic nazwisko? (y jesli tak): ");
    if ((ans=getchar()) == 'y') {
      printf("Podaj nowe nazwisko: ");
      char newLastName[100];
      scanf("%s", newLastName);
      getchar();
      lastName = (char *)malloc((strlen(newLastName)+1)*sizeof(char));
      strcpy(lastName, newLastName);
    }
    else {
      getchar();
    }
    printf("Czy chcesz zmienic data urodzenia? (y jesli tak): ");
    if ((ans=getchar()) == 'y') {
      printf("Podaj nowa date urodzenia: ");
      char newDate[100];
      scanf("%s", newDate);
      getchar();
      date = (char *)malloc((strlen(newDate)+1)*sizeof(char));
      strcpy(date, newDate);
    }
    else {
      getchar();
    }
    printf("Czy chcesz zmienic zarobki? (y jesli tak): ");
    if ((ans=getchar()) == 'y') {
      printf("Podaj nowe zarobki: ");
      float newSalary;
      scanf("%f", &newSalary);
      getchar();
      salary = newSalary;
    }

    printf("Czy chcesz zmienic srednia liczbe goli? (y jesli tak): ");
    if ((ans=getchar()) == 'y') {
      printf("Podaj nowa srednia liczba goli: ");
      float newAvgGoals;
      scanf("%f", &newAvgGoals);
      getchar();
      avgGoals = newAvgGoals;
    }
    else {
      getchar();
    }
    printf("Czy chcesz zmienic liczbe asyst? (y jesli tak): ");
    if ((ans=getchar()) == 'y') {
      printf("Podaj nowa liczbe asyst: ");
      int newAssists;
      scanf("%d", &newAssists);
      getchar();
      assists = newAssists;
    }
    else {
      getchar();
    }
    char command[1000];
    sprintf(command, "UPDATE footballer SET name='%s', lastname='%s', birthday='%s', salary=%f, avaragenumberofgoals=%f, numberofassists=%d WHERE idfootballer=%d", name, lastName, date, salary, avgGoals, assists, id);
    printf("\n");
    printf("%s\n", command);
    doSQL(conn, command);
  } 
  else {
    printf("Nie ma rekordu o takim id\n");
  }
    PQclear(result);
}

void showTable(PGconn *conn) {
  char *command = "SELECT * FROM footballer";
  doSQL(conn, command);
}

void deleteFootballer(PGconn *conn) {
  printf("Podaj id do usuniecia: ");
  int id;
  scanf("%d", &id);
  getchar();
  char command[1000];
  sprintf(command, "DELETE FROM footballer WHERE idfootballer=%d", id);
  doSQL(conn, command);
}

void findById(PGconn *conn) {
  printf("Podaj id: ");
  int id;
  scanf("%d", &id);
  getchar();
  char command[1000];
  sprintf(command, "SELECT * FROM footballer WHERE idfootballer=%d", id);
  doSQL(conn, command);
}


void findBySalary(PGconn *conn) {
  printf("Podaj minimalne zarobki: ");
  float salary;
  scanf("%f", &salary);
  getchar();
  char command[1000];
  sprintf(command, "SELECT * FROM footballer WHERE salary>%f::money", salary);
  doSQL(conn, command);
}

void createTrigger(PGconn *conn) {
  doSQL(conn, "CREATE OR REPLACE FUNCTION checkData() RETURNS TRIGGER AS $$ BEGIN IF length(NEW.name) < 1 THEN RAISE EXCEPTION 'Imie musi miec dlugosc wieksza niz jeden'; END IF; IF length(NEW.lastname) < 1 THEN RAISE EXCEPTION 'Nazwisko musi miec dlugosc wieksza niz jeden'; END IF; IF NEW.salary < 0::money THEN RAISE EXCEPTION 'Pensja nie moze byc ujemna'; END IF; IF NEW.avaragenumberofgoals < 0 THEN RAISE EXCEPTION 'Srednia liczba goli nie moze byc ujemna'; END IF; IF NEW.numberofassists < 0 THEN RAISE EXCEPTION 'Liczba asyst nie moze byc ujemna'; END IF; NEW.name := initcap(NEW.name); NEW.lastname := replace(initcap(replace(NEW.lastname, '-', ' ')), ' ', '-'); RETURN NEW; END; $$ LANGUAGE plpgsql;");
  doSQL(conn, "CREATE TRIGGER footballer_check BEFORE INSERT OR UPDATE ON footballer FOR EACH ROW EXECUTE PROCEDURE checkData();");
}

void deleteTrigger(PGconn *conn) {
  doSQL(conn, "DROP TRIGGER footballer_check on footballer;");
}


int main() {
  PGresult *result;
  PGconn *conn;

  conn = PQconnectdb("host=localhost port=5432 dbname=aleksander user=aleksander password=12345abc");
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
	showTable(conn);
	break;
      }
      case '3' : {
	addFootballer(conn);
	break;
      }
      case '4' : {
	deleteFootballer(conn);
	break;
      }
      case '5' : {
	updateFootballer(conn);
	break;
      }
      case '6' : {
	findById(conn);
	break;
      }
      case '7' : {
	findBySalary(conn);
	break;
      }
      case '8' : {
	createTrigger(conn);
	break;
      }
      case '9' : {
	deleteTrigger(conn);
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
