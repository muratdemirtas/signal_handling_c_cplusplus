/**
 * Signal Handling Example
 * @author muratdemirtas <muratdemirtastr@gmail.com>
 * @version 1.0.0
 * @since  2017-06-29
 */

/*include libraries*/
#include <csignal>
#include <QThread>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

/*define variables for logging*/
#define LOG_DIR "Data.txt"
QTextStream stream;
QFile file(LOG_DIR);

/*declare function prototype for logging*/
void print2logFile(QString data);

/*enum for log file status*/
typedef enum {
    NO_ERR,
    FAILED
}log_status;
log_status fileStatus = FAILED;

/**
 * Signal Handling Example
 *@brief this will receive signals from Operating System
 *@return operating system signal
 */
void signalHandler( int signum ) {

   /*check which signal received*/
   switch (signum)
           {
           case SIGTERM:
               qDebug() <<  "SIGTERM signal received ...";
               break;
           case SIGINT:
               qDebug() << "SIGINT (Ctrl-C) signal received ...";
               print2logFile("Ctrl-C Received, Exiting without close file");
               break;
           case SIGUSR1:
               qDebug() << "SIGUSR1,MYSignal received ...";
               qDebug() << "Program will close safely,lets finish operations";
               print2logFile("MySignal with close my file safely..");
               file.close();

               /*
                * @note Both ensure that all file buffers in memory (buffers on the device itself are
                * a different matter) are written to all currently attached storage devices
                *  (be they SD card, USB stick, USB HDD etc).
                *  The command doesn't need to be run with sudo as it needs no superpowers.
               */
               system("sync");

               qDebug() << "Program closing...";
               break;
           default:
               qDebug() << "Unknown signal received ...";
           }

   /*terminate program with this signal, you can also contuine your program
    * if you type //exit(signum)
    */

   exit(signum);

}

/**
 *@brief for prepare text file for logging
 *@return log file status
 */
int createLogFile()
{

    if (file.open(QIODevice::ReadWrite|QIODevice::Truncate)) {

             stream.setDevice(&file);
             stream << "Program Started at: " <<
                       QDateTime::currentDateTime().toString() << endl;
             fileStatus = NO_ERR;
             return 0;
    }
    else
    {
       qDebug() << "File can not open, is disk full or sudo required?";
            fileStatus = FAILED;
       return 1;
    }
}

/**
 *@brief to write log file
 *@return none
 */
void print2logFile(QString data)
{
    if(fileStatus==NO_ERR)
        stream << data.toStdString().c_str() << endl;
    else
    {
        qDebug() << "Check your log file settings,that not open";
    }
}
/**
 *@brief main function of program
 *@return return value
 */
int main () {

   if(createLogFile()!=1){
       qDebug() << "Log File Opened Successfully";
   }

   // register signal SIGINT and signal handler
   signal(SIGINT, signalHandler);
   signal(SIGUSR1, signalHandler);

   /*infinite loop of program*/
   while(1)
   {
      QThread::sleep(1);
      print2logFile("Program Working");
      qDebug() << "Program Working";
   }

   return 0;
}
