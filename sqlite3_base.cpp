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
//    char *sql = " CREATE TABLE SensorData(ID INTEGER PRIMARY KEY,SensorID INTEGER,SiteNum INTEGER,Time VARCHAR(12),SensorParameter REAL);" ;
//    char *sql = " CREATE TABLE SensorData(ID INTEGER PRIMARY KEY,SensorID INTEGER,SiteNum INTEGER,Time VARCHAR(12),SensorParameter REAL);" ;

    char *sql_ceate = "CREATE TABLE sensor_iot("  \
         "ID INT PRIMARY   KEY     NOT NULL," \
         "USERNAME         TEXT    NOT NULL," \
		 "Time 			   VARCHAR(12)," \
         "ADDRESS          CHAR(50)," \
         "SensorParameter  REAL );";

	rc=sqlite3_exec( sql_db , sql_ceate , 0 , 0 , &zErrMsg );
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
	
    //插入数据
	sql = "INSERT INTO sensor_iot (ID,USERNAME,Time,ADDRESS,SensorParameter) "  \
         "VALUES (1, 'Paul', '2018080312', 'California', 20000.00 ); " \
         "INSERT INTO sensor_iot (ID,USERNAME,Time,ADDRESS,SensorParameter) "  \
         "VALUES (2, 'Mark', '2018080302', 'Rich-Mond ', 65000.00 );";
	
    sqlite3_exec( sql_db , sql , 0 , 0 , &zErrMsg );
    if(rc != SQLITE_OK)
    {
        if( zErrMsg != NULL  ) {
//            fprintf( stderr , " SQL error : %s\n " , zErrMsg);
            qDebug()<<"SQL error"<<zErrMsg<<"!"<<endl;
            sqlite3_free(zErrMsg) ;
            return -1;
          }
    }
    sqlite3_free(zErrMsg);
    return 1;
}
char** sqlite3_base:: sqlite3_base_read_data(){

    int rc=0,nrow=0,ncolumn=0,i=0;
    char *zErrMsg = 0;
    char **azResult; //二维数组存放结果

	char **temp;
	
    //查询数据
    /*
	    int sqlite3_get_table(sqlite3*, const char *sql,char***result , int *nrow , int *ncolumn ,char **errmsg );
	    result中是以数组的形式存放你所查询的数据，首先是表名，再是数据。
	    nrow ,ncolumn分别为查询语句返回的结果集的行数，列数，没有查到结果时返回0
    */
    char* sql = "SELECT * FROM sensor_iot";
    rc=sqlite3_get_table( sql_db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );

	sqlread_nrow=nrow;
	sqlread_ncolumn=ncolumn;

   // display
//   qDebug()<<"nrow-"<<nrow<<"ncolumn-"<<ncolumn<<endl;
//	for( i=0 ; i<( nrow + 1 ) * ncolumn ; i++ )
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





