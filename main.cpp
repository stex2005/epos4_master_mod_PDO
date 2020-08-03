/** @file
 *  @brief This file contains the template for the main program for the Esmacat slave
 *  project */
/*****************************************************************************************
 * INCLUDES
 ****************************************************************************************/
#include <iostream>
#include "my_app.h"

/*****************************************************************************************
 * FUNCTIONS
 ****************************************************************************************/
/**
 * @brief Initializes the execution of the Ethercat communication and
 *        primary real-time loop for your application for the desired
 *        slave
 * @return
 */

int main()
{
    //this is defined in my_app.cpp and my_app.h
    static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("esmacat_log.csv", 80000, 10); // Create the 1st appender.
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender; // Create the 2nd appender.

    plog::init(plog::warning, &fileAppender).addAppender(&consoleAppender); // Initialize the logger with the both appenders.

    //this is defined in my_app.cpp and my_app.h
    my_app app;

    // if you already know the ethernet adapter name for EtherCAT, uncomment and use the line below
    //    app.set_ethercat_adapter_name(" WRITE YOUR ETHERNET ADAPTER NAME");

    // If the name is not known, select through the terminal an ethernet adapter (the slave)
    // you'd like to communicate with over EtherCAT
    // app.set_ethercat_adapter_name_through_terminal();

    // start the esmacat application customized for your slave
    app.start();

    // set the cycle time in ns
    app.set_one_cycle_time_ns(1000000L);

    //the application runs as long as the esmacat master and slave are in communication
    while (app.is_esmacat_app_closed() == false );
    app.close();
    return 0;
}

