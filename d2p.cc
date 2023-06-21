#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

void GetFolderFiles(const string& path, vector<fs::path> &result, bool isRecursive = true)
{
    for (const auto& entry : fs::directory_iterator(path)) 
	{
        if (fs::is_regular_file(entry)) 
		{
			result.push_back(entry.path());
        } 
		if (isRecursive)
		{
			if (fs::is_directory(entry)) 
			{
        	    GetFolderFiles(entry.path().string(), result, isRecursive);
        	}
		}
    }
}
void EnsureParentFolder(string targetPath)
{
    fs::path path = targetPath;
    bool isExists = fs::exists(path.parent_path()) && fs::is_directory(path.parent_path());
    if (!isExists)
    {
        fs::create_directories(path.parent_path());
    }
}
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "arg : [your .dss files folder name]" << endl;
        return -1;
    }

    vector<fs::path> paths;
    GetFolderFiles(argv[1], paths, true);

    for (auto path : paths)
    {
        if (path.extension().string() == ".dds")
        {
            fs::path outPath = path;
            outPath.replace_extension(".png");
            outPath = outPath.parent_path().string() + "/output/" + outPath.filename().string();
            EnsureParentFolder(outPath.string());
            string cmd = "nvtt_export " + path.string() + " -o " + outPath.string();
            cout << "export path : " << outPath.string() << endl;
            system(cmd.c_str());
        }
    }

    return 0;
}