#include "DancingSFM.h"

DancingSFM::DancingSFM()
{

}

bool DancingSFM::PMVS()
{
    system("del fast.*.ply");
	system("VisualSFM sfm+pmvs images fast.nvm");
	return 1;
}

bool DancingSFM::Poisson()
{
	system("PoissonRecon.x64 --in fast.0.ply --out surface.ply  --density ");
	return 1;
}

string DancingSFM::int2str(int x)
{
    string s="";
    while(x>0)
    {
        s+=char(x%10+'0');
        x/=10;
    }
    for(int i=0;i<s.size()/2;i++)
        s[i]=s[s.size()-i-1];
    return s;
}

bool DancingSFM::MeshClean(int param)
{
    string ss=int2str(param);
    string s("SurfaceTrimmer.x64 --in surface.ply --out final.ply --trim ");
    s+=ss;
	system(s.c_str());
	return 1;
}

bool DancingSFM::ConvertNVMtoPLY(ifstream& in, ofstream& out)
{
    string buffer;
    std::getline(in, buffer);

    // read and skip the camera section.
    int ncam = 0, npoint = 0;
    in >> ncam;  if(ncam <= 1) return false;
     for(int i = 0; i < ncam; ++i) {in >> buffer; std::getline(in, buffer); }

    // read the number of points.
    in >> npoint;   if(npoint <= 0) return false;

    // here you can write PLY header
    out << "ply\nformat ascii 1.0\n"
         << "element vertex " << npoint << '\n'
         << "property float x\n"
         << "property float y\n"
         << "property float z\n"
         << "property uchar diffuse_red\n"
         << "property uchar diffuse_green\n"
         << "property uchar diffuse_blue\n"
         << "end_header\n";

    // Load all the vertices
    for(int i = 0; i < npoint; ++i)
    {
        float pt[3]; int cc[3];
        // read the point coordinate and color
        in  >> pt[0] >> pt[1] >> pt[2] >> cc[0] >> cc[1] >> cc[2];
        // skip the rest of data.
	std::getline(in, buffer);

        //here you should write a vertex to PLY
        out << pt[0] << ' ' << pt[1] << ' ' << pt[2] << ' '
             << cc[0] << ' ' << cc[1] << ' ' << cc[2] << '\n';
    }
    return true;
}

bool DancingSFM::MVE()
{
	system("makescene -i images scenes");
	system("sfmrecon scenes");
	system("dmrecon -s2 scenes");
	system("scene2pset -F2 scenes fast.ply");
	system("fssrecon fast.ply surface.ply");
	system("meshclean -t10 surface.ply final.ply");
	return 1;
}
