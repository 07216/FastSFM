#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

class DancingSFM
{

private:
    string int2str(int );

public:
	DancingSFM();
	bool PMVS();
	bool Poisson();
	bool MeshClean(int param);
	bool MVE();
	bool ConvertNVMtoPLY(ifstream&,ofstream&);
};
