
#include <map>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <filesystem>
#include <regex>
#include <atomic>

#include "queue.hpp"

#define log(smth) std::cout << smth << std::endl;

class AudioImageFile{
    public:
    std::string audiofile, imageFile;
    AudioImageFile(std::string a, std::string i){
        audiofile = a;
        imageFile = i;
    }
};

namespace Filter{
    std::string GetFilename(std::string withPath){
        std::regex fileRegex(R"(.*/([^/]*))");
        std::smatch m1;
        std::regex_match(withPath, m1, fileRegex);
        return m1[1];
    }

    std::string GetFilenameWithoutType(std::string withType){
        std::regex fileRegex(R"((.*)(\..*))");
        std::smatch m1;
        std::regex_match(withType, m1, fileRegex);
        return m1[1];
    }
}

std::regex AudioRegex("audio");
std::regex thumbnailRegex("image");
std::regex outputRegex("output");

class ConvertFactory{

    public:

    void convert(AudioImageFile file, std::string outputFolder, std::atomic<int>& threadCount){
        threadCount += 1;

        std::string audio = file.audiofile;
        std::string thumbnail = file.imageFile;

        std::string ffmpeg = "ffmpeg -i  \"audio\" -i \"image\" -map 0 -map 1 \"output\" > /dev/null 2>&1";
        
        std::string result = std::regex_replace(ffmpeg, AudioRegex, audio);
        result = std::regex_replace(result, thumbnailRegex, thumbnail);
        std::string outputname = Filter::GetFilename(file.audiofile);
        outputname = Filter::GetFilenameWithoutType(outputname);
        
        result = std::regex_replace(result, outputRegex, outputFolder + "/" + outputname + ".mp3");
        //log(result)
        system(result.c_str());
        threadCount -= 1;
    }
    

    void FactoryHandler(std::vector<AudioImageFile> objects, std::string outputFolder){
        //Generate the queue
        Queue::Queue<AudioImageFile> queue = Queue::Queue<AudioImageFile>(objects);

        std::vector<std::thread> threadsInUse;

        std::atomic<int> threadCountInUse(0);

        //The factory

        while (true)
        {
            try
                {
                    if (threadCountInUse < 10)
                    {
                        AudioImageFile f = queue.pop_front();
                        threadsInUse.push_back(std::thread(&ConvertFactory::convert, this, f, outputFolder, std::ref(threadCountInUse)));  
                        //auto thread = std::thread(&ThreadFactory::test, this, 1);
                        log("there is still " << queue.size() << " audio to convert");
                    }
                    
                }
                catch(const std::out_of_range& e)
                {
                    break;
                }
        }
        
        for (int i = 0; i < threadsInUse.size(); i++)
        {
            threadsInUse[i].join();
        }
        
        
    }

    void FactorySetup(std::vector<AudioImageFile> objs, std::string outputFolder){
        std::thread factory(&ConvertFactory::FactoryHandler, this, objs, outputFolder);
        factory.join();
    }
};


bool findFiles(std::vector<AudioImageFile> &objects, std::string folder){
    std::vector<std::string> files;
    for (const auto entry : std::filesystem::directory_iterator(folder))
    {
        files.push_back(entry.path().string());
    }

    std::regex fileRegex(R"((.*)(\..*))");

    log("there is " << files.size() << " files")

    for (int i = 0; i < files.size(); i++)
    {
        for (int j = 0; j < files.size(); j++)
        {
            std::smatch m1, m2;
            std::regex_match(files[i], m1, fileRegex);
            std::regex_match(files[j], m2, fileRegex);

            //log(i  << "  ---   " << m1[1] << " : " << m2[1]);

            if (i!=j && (m1[1] == m2[1]))
            {
                //log(files[i] << " : " << files[j])
                objects.push_back(AudioImageFile(files[i], files[j]));
                
                files.erase(files.begin()+i);
                //files.erase(files.begin()+j);
            }   
        }
    }
    return true;  
}

int main(){
    std::vector<AudioImageFile> objects;
    std::string folder = "/home/hiha/Music/yt";
    findFiles(objects, folder);

    log("converting " << objects.size() << " files");
    ConvertFactory cF;

    cF.FactorySetup(objects, folder + "/mp3_output");


}