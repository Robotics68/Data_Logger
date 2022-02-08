#include "Logger.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>


Logger::Logger()
{
    //ctor
}

void Logger::SetOutputFilename(std::string inName){
    WriteFilename = inName;
}

void Logger::SetOutputFiletype(std::string inType){
    WriteFiletype = inType;
}

void Logger::SetDelimiter(std::string inDel){
    WriteDelimiter = inDel;
}

void Logger::SetDescription(std::string inDesc){
    WriteDescription = inDesc;
}

void Logger::AddData(uint64_t inTime, std::vector<double> inData){
    //Assemble filename if not already done
    if(CompositeFilename.compare("") == 0){
        //Assemble composite filename
        AssembleFilePath();
    }

    //Push time onto storage vector
    Timestamp.push_back(inTime);

    //Add new vector to 2D vector
    Data.push_back(inData);
}

void Logger::AddLabels(std::vector<std::string> inLabels){
    //Assemble filename if not already done
    if(CompositeFilename.compare("") == 0){
        //Assemble composite filename
        AssembleFilePath();
    }

    Labels = inLabels;
}

bool Logger::WriteToFile(){
    //Open file
    std::ofstream outFile(CompositeFilename);

    //Write Description
    if(WriteDescription.compare("No Description") == 0){}else{
        outFile << WriteDescription << "\n";
    }

    //Write Labels with delimiters
    outFile << "Time" << WriteDelimiter; //Manually write time label
    for (int i = 0; i < Labels.size(); i++){
        outFile << Labels[i];
        if(i < Labels.size()-1){
            outFile << WriteDelimiter; //write delimiter
        }else{
            outFile << "\n"; //write end of line
        }
    }

    //Write 2d vector
    for (int i = 0; i < Data.size(); i++){
        //Write time to each line (if time value exists for line)
        if(i < Timestamp.size()){
            outFile << Timestamp[i] << WriteDelimiter;
        }else{ //if time value does not exist, write a blank space so columns line up
            outFile << WriteDelimiter;
        }
        for (int j = 0; j < Data[i].size(); j++){
            outFile << Data[i][j];
            if(j < Data[i].size()-1){
                outFile << WriteDelimiter;
            }else if( i == Data.size()-1 && j == Data[i].size()-1){
                //Do nothing for last line
            }else{
                outFile << "\n";
            }
        }
    }

    //Close file
    outFile.close();

    return true;
}

//Test functions
void Logger::test(){
    std::cout << "in Test Mode\n";

    SetOutputFilename("_test_Data");
    SetOutputFiletype("csv");
    SetDelimiter(",");
    SetDescription("This is a test file");

    AddLabels({"A","B","C","D","E","F","G","H","I","J"});

    //Create 10x10 2d vector
    int len_2dV = 10; //length of 2d vector
    for (int i = 0; i < len_2dV; i++){
        AddData(nanos(),testVector(10));
    }

    WriteToFile();

    //Print test vector
    //print2DVector(Data);
}

void Logger::print2DVector(std::vector<std::vector<double>> inVec){ //Print 2d vector
    std::cout << "2D printed vector:\n";
    for (int i = 0; i < inVec.size(); i++)
    {
        for (int j = 0; j < inVec[i].size(); j++)
        {
            std::cout << inVec[i][j] << ",";
        }
        std::cout << "\n";
    }
    std::cout << "END 2D printed vector\n";
}

std::vector<double> Logger::testVector(int inLen){ //Create test vector of length inLen
    std::vector<double> outVect;

    int rnum = rand() % 100 + 1;

    for (int i = 0; i < inLen; i++){
        outVect.push_back((double)(i+rnum)+0.1);
    }

    //Print out vector
    std::cout << "Created Vector:";
    for (int i=0; i<outVect.size(); ++i){
        std::cout << outVect[i] << ' ';
    }
    std::cout << "\n";

    return outVect;
}

///Private functions
int Logger::FileExists(std::string inFile,std::string inExt){
    std::ifstream cFile;
    std::ifstream dFile;

    //Check zero cases
    cFile.open(inFile+"."+inExt);
    dFile.open(inFile+"0."+inExt);
    if(!cFile && !dFile){
        //Original file does not exist (or is not readable)
        cFile.close();
        dFile.close();
        return 0;
    }

    //clear dFile, we do not need it anymore. Leave cFile open
    dFile.close();

    //counter
    int cnt = 0;

    //Cycle through files until we find first available name
    while(cFile || cnt == 0){
        cnt++;
        cFile.close(); //Close first open file
        std::string scnt = std::to_string(cnt);
        cFile.open(inFile+scnt+"."+inExt);
    }
    cFile.close();

    return cnt;
}

void Logger::AssembleFilePath(){
    int fileNum = FileExists(WriteFilename,WriteFiletype);

    if(fileNum == 0 && Filename_hideZero){
        CompositeFilename = WriteFilename + "." + WriteFiletype;
    }else{
        CompositeFilename = WriteFilename + std::to_string(fileNum) + "." + WriteFiletype;
    }

    std::cout << "Log output filename (program directory): " << CompositeFilename << "\n";
}

uint64_t Logger::nanos()
{
    uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::
                  now().time_since_epoch()).count();
    return ns;
}
