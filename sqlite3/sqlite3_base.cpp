#include "sqlite3_base.h"
#include <QDebug>

#include <QTimer>
#include <sstream>

sqlite3_base::sqlite3_base()
{
//    QTextStream out(stdout);
//    out << "The current datetime is 111111111111" << << endl;
}


int sqlite3_base::sqlite3_base_open(const char *filename)
{
   int rc=0;
   rc = sqlite3_open(filename, &sql_db);
   if( rc )
   {
       qDebug()<<"Can't open database"<<sqlite3_errmsg(sql_db)<<"!"<<endl;
       printf("Can't open database");
       sqlite3_close(sql_db);
//       exit(1);
       return -1;
   }
   printf("open database");
   return 1;

}

int sqlite3_base::sqlite3_base_close()
{
   sqlite3_close(sql_db); //关闭数据库
   return 1;
}

int sqlite3_base::sqlite3_base_create_tab()
{
    int rc;
    char *zErrMsg = 0;
//    char *sql_ceate_t1 = " CREATE TABLE Sensor(ID INTEGER PRIMARY KEY,UserID INTEGER,UserName TEXT,Time VARCHAR(12),ADDRESS CHAR(50),SensorParameter REAL);";
//    char *sql_ceate_t2 = " CREATE TABLE Sensor2(ID INTEGER PRIMARY KEY,UserID INTEGER,UserName TEXT,Time VARCHAR(12),ADDRESS CHAR(50),SensorParameter REAL);";
//    char *sql_ceate_t3 = " CREATE TABLE Sensor3(ID INTEGER PRIMARY KEY,UserID INTEGER,UserName TEXT,Time VARCHAR(12),ADDRESS CHAR(50),SensorParameter REAL);";

     char *sql_ceate_myir = " CREATE TABLE MYIR_RHMI(ID INTEGER PRIMARY KEY,UserID INTEGER,UserName TEXT,Time VARCHAR(12));";
    rc=sqlite3_exec( sql_db , sql_ceate_myir , NULL , NULL , &zErrMsg );

//    rc=sqlite3_exec( sql_db , sql_ceate_t1 , NULL , NULL , &zErrMsg );
//    rc=sqlite3_exec( sql_db , sql_ceate_t2 , NULL , NULL , &zErrMsg );
//    rc=sqlite3_exec( sql_db , sql_ceate_t3 , NULL , NULL , &zErrMsg );
    if(rc != SQLITE_OK)
    {
        if( zErrMsg != NULL  ) {
            qDebug()<<"SQL error11"<<zErrMsg<<"!"<<endl;
            sqlite3_free(zErrMsg) ;
            return -1;
          }
    }
    sqlite3_free(zErrMsg);
    return 1;
}

int sqlite3_base::sqlite3_base_insert_data(){
    int rc;
    char *zErrMsg = 0;
	char *sql;
    char sql_insert[512];
    char temp_str[20];
  	
     //插入数据  OK
//     sql = "INSERT INTO sensor (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
//           "VALUES (NULL,1,'Alex1',2018080316,'California_1',20015.00 ); " \
//           "INSERT INTO sensor (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
//           "VALUES (NULL,2,'Alex2',2018080317,'California_2',20016.00 ); " \
//           "INSERT INTO Sensor2 (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
//           "VALUES (NULL,3, 'Paul_4', 2018080314, 'California3', 20003.00 ); " \
//           "INSERT INTO Sensor2 (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
//           "VALUES (NULL,4, 'Paul_7', 2018080315, 'California4', 20004.00 ); " \
//           "INSERT INTO Sensor3 (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
//           "VALUES (NULL,7, 'Paul1', 2018080318, 'California_9', 20004.00 ); " \
//           "INSERT INTO Sensor3 (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
//           "VALUES (NULL,8, 'Paul2', 2018080319, 'California_8', 20005.033); ";
	
//    char*sql_insert_1 ="INSERT INTO 'sensor'VALUES(NULL,1,'Paul5',2018080316,'California511',20005.00);";
//    sqlite3_exec(sql_db,sql_insert_1,NULL,NULL,&zErrMsg);
//    char*sql_insert_2 ="INSERT INTO 'sensor'VALUES(NULL,4,'Paul5',20180803161111,'California512',20005.00);";
//    sqlite3_exec(sql_db,sql_insert_2,NULL,NULL,&zErrMsg);

//    char*sql_insert_11 ="INSERT INTO 'sensor2'VALUES(NULL,1,'Paul5',2018080316,'California513',20005.00);";
//    sqlite3_exec(sql_db,sql_insert_11,NULL,NULL,&zErrMsg);
//    char*sql_insert_21 ="INSERT INTO 'sensor2'VALUES(NULL,5,'Paul5',20180803161111,'California5',20005.00);";
//    sqlite3_exec(sql_db,sql_insert_21,NULL,NULL,&zErrMsg);


//    QTextStream out(stdout);
    // 获取当前时间
    QDateTime cdt = QDateTime::currentDateTime();
//    out << "The current datetime is 111111111111" << cdt.toString() << endl;
//    out << "The current date is 222222222222" << cdt.date().toString() << endl;
//    out << "The current time is 33333333333" << cdt.time().toString() << endl;

    char*  ch;
    QByteArray ba = cdt.toString().toLatin1();  // must
    ch=ba.data();
    sprintf(sql_insert,"INSERT INTO 'MYIR_RHMI'VALUES(NULL,1,'MYIR-RHMI','%s');",ch);

//    int a1 = 1;
//    int a2 = 10;
//    std::ostringstream oss;
//    oss << "insert into MYIR_RHMI values(NULL,1,'MYIR-RHMI'," << a1 << ")";
//    char *sqliteInsert = const_cast<char*>(oss.str().c_str());

//    sql ="INSERT INTO 'MYIR_RHMI'VALUES(NULL,1,'MYIR-RHMI',2018080316);";
//    sql="insert into tab(name,sex,age) values('" & na &"','" se & "','" & ag& "')";
//    sqlite3_exec(sql_db,sql,NULL,NULL,&zErrMsg);

//     sql = "INSERT INTO MYIR_RHMI (ID,UserID,UserName,Time) " \
//           "VALUES (NULL,1,'MYIR-RHMI','" & cdt.toString() &"' ); " ;

//     rc=SQLSet

     rc=sqlite3_exec( sql_db , sql_insert , NULL , NULL , &zErrMsg );
     if(rc != SQLITE_OK)
     {
         if( zErrMsg != NULL  ) {
             qDebug()<<"SQL error"<<zErrMsg<<"!"<<endl;
             sqlite3_free(zErrMsg) ;
             return -1;
           }
     }
    sqlite3_free(zErrMsg);
    return 1;
}
char** sqlite3_base:: sqlite3_base_read_data(){

    int rc=0;
    char *zErrMsg = 0;
    char **azResult; //二维数组存放结果
    int nrow;
    int ncolumn;
    //查询数据
    /*  int sqlite3_get_table(sqlite3*, const char *sql,char***result , int *nrow , int *ncolumn ,char **errmsg );
	    result中是以数组的形式存放你所查询的数据，首先是表名，再是数据。
	    nrow ,ncolumn分别为查询语句返回的结果集的行数，列数，没有查到结果时返回0
    */
    char* sql = "SELECT * FROM MYIR_RHMI";
    rc=sqlite3_get_table( sql_db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
    if(rc != SQLITE_OK)
    {
        if( zErrMsg != NULL  ) {
            qDebug()<<"SQL error"<<zErrMsg<<"!"<<endl;
            sqlite3_free(zErrMsg) ;
            return NULL;
          }
    }

    sqlread_nrow = nrow;
    sqlread_ncolumn = ncolumn;


/*method 2*/
//    sprintf(strSQL,"select * from SensorDate where ID ");
//    sqlite3_stmt *stat;//写二进制数据时要用的结构
//    sqlite3_prepare(sql_db,strSQL , -1, &stat, 0);
//    rc = sqlite3_step(stat);

//释放掉  azResult 的内存空间
//    sqlite3_free_table(azResult);

    return azResult;
}

int sqlite3_base::sqlite3_base_delete_data(int id){

    int rc;
    char sql[300];
    char *pErrMsg = 0;

    char num[10];
    sprintf(num,"%d",id);
    sprintf(sql, "DELETE FROM MYIR_RHMI WHERE ID=%s",num);

    rc = sqlite3_exec(sql_db, sql,NULL, NULL, &pErrMsg);
    if (rc != SQLITE_OK)
    {;
        sqlite3_free(pErrMsg);
        return -1;
    }

    return 0;
}



