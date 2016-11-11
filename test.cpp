#include <iostream>
#include <fstream>
#include "DancingSFM.h"
using namespace std;
DancingSFM* SFMer;
int main()
{
	SFMer=new DancingSFM();
	//SFMer->MVE();
	SFMer->PMVS();
	SFMer->Poisson();
	SFMer->MeshClean(5);//Left !!!!!
	//ifstream fin("dino.nvm");
	//ofstream fout("test.ply");
	//SFMer->ConvertNVMtoPLY(fin,fout);
	return 1;
}
