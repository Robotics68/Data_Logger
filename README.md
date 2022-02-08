# Data_Logger
Basic C++ Data Logger


/*
By: J. Krause, Robotics and Motion lab, The University of Illinois at Chicago
Email: JKraus3@uic.edu

I tried to keep the code here as basic as possible. It should function with C++11 and possibly earlier.
Tested and coded for Ubuntu 20.04 with 5.4.93-rt51 real time kernel.

Purpose: Quick data logger with highc capacity.
Function:
    Call the following functions at setup:
        SetOutputFilename("name");              //Set the output filename.
        SetOutputFiletype("type");              //Set the output file extension.
        SetDelimiter("char");                   //Set the data delimiter. Can be any character except escape characters.
        SetDescription("Description");          //Set a short description in first line of data file
        AddLabels({"Label1","Label2"});         //Set labels (second line of data file)

    Call the following functions cyclically:
        AddData(time,data vector);              //Store a time (1st col) and set of data points

    Call the following just before shutdown
        WriteToFile();                          //Write data to file

Operation:
    This is a simple data logger that offers quite a bit of versatility. Rows can have any number of
    entries, but it is recommended that each data set be the same size for readability. File extension
    can be any type that can be streamed to. Output destination for files is the same as the
    executing program directory. The filename to be written to is resolved upon first AddData call
    and will attempt to detect any previous data files with the same name. If earlier data files are
    detected, it will increment numeric designator and append it to the new data file name.

For operation example, view the test() function in the Logger.cpp class file.

*/
