#ifndef PUBLICFILE_H
#define PUBLICFILE_H


class publicfile
{
public:
    publicfile();


};


struct  FileRecord
{
   unsigned int  No;                /*当前计数  */
   char          Date[12];          /*当前年月日*/
   char          Time[12];          /*当前时分秒*/
   double        MC[2][10];
   float         Conc[6];      //秒浓度        0- 1振荡浓度A B  2 -3背景浓度A B  4 -5光学浓度A B
   float         RMC[2];        //最终浓度

};
#endif // PUBLICFILE_H
