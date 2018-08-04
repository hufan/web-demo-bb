#include "sqlite3_base.h"
#include <QDebug>

sqlite3_base::sqlite3_base()
{

}


int sqlite3_base::sqlite3_base_open(const char *filename)
{
   int rc=0;
   rc = sqlite3_open(filename, &sql_db);
   if( rc )
   {
       qDebug()<<"Can't open database"<<sqlite3_errmsg(sql_db)<<"!"<<endl;
       sqlite3_close(sql_db);
//       exit(1);
       return -1;
   }
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
    char *sql_ceate_t1 = " CREATE TABLE Sensor(ID INTEGER PRIMARY KEY,UserID INTEGER,UserName TEXT,Time VARCHAR(12),ADDRESS CHAR(50),SensorParameter REAL);";
    char *sql_ceate_t2 = " CREATE TABLE Sensor2(ID INTEGER PRIMARY KEY,UserID INTEGER,UserName TEXT,Time VARCHAR(12),ADDRESS CHAR(50),SensorParameter REAL);";
    char *sql_ceate_t3 = " CREATE TABLE Sensor3(ID INTEGER PRIMARY KEY,UserID INTEGER,UserName TEXT,Time VARCHAR(12),ADDRESS CHAR(50),SensorParameter REAL);";

    rc=sqlite3_exec( sql_db , sql_ceate_t1 , NULL , NULL , &zErrMsg );
    rc=sqlite3_exec( sql_db , sql_ceate_t2 , NULL , NULL , &zErrMsg );
    rc=sqlite3_exec( sql_db , sql_ceate_t3 , NULL , NULL , &zErrMsg );
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
  	
    // //插入数据
	// sql = "INSERT INTO sensor_iot (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
    //       "VALUES (NULL,1, 'Paul1', '2018080312', 'California1', 20001.00 ); " \
    //       "INSERT INTO sensor_iot (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
    //       "VALUES (NULL,2, 'Paul2', '2018080313', 'California2', 20002.00 ); " \
    //       "INSERT INTO sensor_iot (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
    //       "VALUES (NULL,3, 'Paul3', '2018080314', 'California3', 20003.00 ); " \
    //       "INSERT INTO sensor_iot (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
    //       "VALUES (NULL,4, 'Paul4', '2018080315', 'California4', 20004.00 ); " \
    //       "INSERT INTO sensor_iot (ID,UserID,UserName,Time,ADDRESS,SensorParameter) " \
    //       "VALUES (NULL,8, 'Paul5', '2018080316', 'California5', 20005.00 ); ";
	
    char*sql_insert_1 ="INSERT INTO 'sensor'VALUES(NULL,1,'Paul5',2018080316,'California511',20005.00);";
    sqlite3_exec(sql_db,sql_insert_1,NULL,NULL,&zErrMsg);
    char*sql_insert_2 ="INSERT INTO 'sensor'VALUES(NULL,4,'Paul5',20180803161111,'California512',20005.00);";
    sqlite3_exec(sql_db,sql_insert_2,NULL,NULL,&zErrMsg);

    char*sql_insert_11 ="INSERT INTO 'sensor2'VALUES(NULL,1,'Paul5',2018080316,'California513',20005.00);";
    sqlite3_exec(sql_db,sql_insert_11,NULL,NULL,&zErrMsg);
    char*sql_insert_21 ="INSERT INTO 'sensor2'VALUES(NULL,5,'Paul5',20180803161111,'California5',20005.00);";
    sqlite3_exec(sql_db,sql_insert_21,NULL,NULL,&zErrMsg);

    char*sql_insert_13 ="INSERT INTO 'sensor3'VALUES(NULL,1,'Paul5',2018080316,'California5',20005.022);";
    sqlite3_exec(sql_db,sql_insert_13,NULL,NULL,&zErrMsg);
    char*sql_insert_23 ="INSERT INTO 'sensor3'VALUES(NULL,3,'Paul5',20180803161111,'California5',20005.033);";
    sqlite3_exec(sql_db,sql_insert_23,NULL,NULL,&zErrMsg);

    // rc=sqlite3_exec( sql_db , sql , NULL , NULL , &zErrMsg );
    // if(rc != SQLITE_OK)
    // {
    //     if( zErrMsg != NULL  ) {
    //         qDebug()<<"SQL error"<<zErrMsg<<"!"<<endl;
    //         sqlite3_free(zErrMsg) ;
    //         return -1;
    //       }
    // }
    sqlite3_free(zErrMsg);
    return 1;
}
char** sqlite3_base:: sqlite3_base_read_data(){

    int rc=0,i=0;
    char *zErrMsg = 0;
    char **azResult; //二维数组存放结果
	char **temp;
    int nrow;
    int ncolumn;
	
    //查询数据
    /*
	    int sqlite3_get_table(sqlite3*, const char *sql,char***result , int *nrow , int *ncolumn ,char **errmsg );
	    result中是以数组的形式存放你所查询的数据，首先是表名，再是数据。
	    nrow ,ncolumn分别为查询语句返回的结果集的行数，列数，没有查到结果时返回0
    */
    char* sql = "SELECT * FROM sensor_iot";
    rc=sqlite3_get_table( sql_db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
    sqlread_nrow = nrow;
    sqlread_ncolumn = ncolumn;
// display
// 	for( i=0 ; i<( nrow + 1 ) * ncolumn ; i++ )
//        qDebug()<<"SQL data-"<<i<<"data-"<<azResult[i]<<endl;
 
   		temp=azResult;

/*method 2*/
//    sprintf(strSQL,"select * from SensorDate where ID ");
//    sqlite3_stmt *stat;//写二进制数据时要用的结构
//    sqlite3_prepare(sql_db,strSQL , -1, &stat, 0);
//    rc = sqlite3_step(stat);

//释放掉  azResult 的内存空间
//    sqlite3_free_table(azResult);
//    return rc;
	return temp;
}





