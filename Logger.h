#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <string>
#include <vector>

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

This software is unlicensed and free to use or modify. I would appreciate a mention if it helps anyone out, tho.
*/

class Logger
{
    public:
        Logger();

        void SetOutputFilename(std::string);        //Set output file name. If exists, will be incremented: OutFilename[n].ext
        void SetOutputFiletype(std::string);        //Set output file extension.
        void SetDelimiter(std::string);             //Set the delimiter to be printed between entries
        void SetDescription(std::string);           //Set description on first line of output file.
        void AddLabels(std::vector<std::string>);   //Add text data to cache for writing (for labels)
        void AddData(uint64_t,std::vector<double>); //Add time and data to cache for writing
        bool WriteToFile();                         //Write cached data to file (can be slow), returns true once write is complete

        //Test functions
        void test();                                //Testing function
        std::vector<double> testVector(int);        //Create test vector of given length
        void print2DVector(std::vector<std::vector<double>>); //Prints 2d data vector to screen

    protected:

    private:
        //Settings
        bool Filename_hideZero = false; //if true, first filename does not have indicator number
        bool Desc_WriteDateTime = true; //if true, appends current system date and time to beginning of description

        //These are changed from default with public functions
        std::string WriteFiletype = "csv";
        std::string WriteFilename = "_Data";
        std::string WriteDelimiter = ",";
        std::string WriteDescription = "No Description";

        //IO related functions
        int FileExists(std::string,std::string);    //Check if filename exists. If so, increments until it finds next highest numeric, returns this number
        void AssembleFilePath();                    //Assemble directory, filename, filetype into composite
        std::string CompositeFilename = "";         //Assembled filepath (with location, name and ext)
                                                    //note: This function adds numeric to avoid overwriting existing files
                                                    //Note: This must be initialized to "" for program to work correctly. Do not change.
        //Other functions
        uint64_t nanos();                           //Returns system time in nanoseconds

        //Data Storage
        std::vector<std::string> Labels;            //Storage for Lables (second row)
        std::vector<std::vector<double>> Data;      //Data storage hash (vector of vectors)
        std::vector<uint64_t> Timestamp;            //Timestamp storage vector
};

#endif // LOGGER_H
