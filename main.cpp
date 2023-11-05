#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <vector>
#include <filesystem>
#include <atomic>
#include <thread> 

#define log(smth) std::cout << smth << std::endl;

class File{
    public:
    std::string name;
    std::string folder;
    
    File(std::string n, std::string f){
        name = n;
        folder = f;
    }
};

template<typename param_t>
class queue{

    public:
    std::vector<param_t> queue;   

    void push_back(param_t object){
        queue.push_back(object);
    }

    int size(){
        queue.size();
    }

    param_t pop_front(){
        if(queue.size() > 0){
            param_t p = queue[0];
            queue.erase(queue.begin());
            return p;
        } else throw std::out_of_range("The queue is empty.");
    }
};

class ThreadFactory{

    void convert(File file, std::string output, std::atomic<int>& threadCount){
        threadCount += 1;
        std::regex AudioRegex("audio");
        std::regex thumbnailRegex("image");
        std::regex outputRegex("output");

        std::string vid = file.folder + "/" + file.name + ".webm";
        std::string thumbnail = file.folder + "/" + file.name + ".webp";
        output = output;

        std::string ffmpeg = "ffmpeg -i \"audio\" -i \"image\" -c:a libmp3lame -q:a 2 -map 0:a -map 1:v -c:v copy -id3v2_version 3 -metadata:s:v title=\"Album cover\" -metadata:s:v comment=\"Cover (front)\" \"output\"";
        
        std::string result = std::regex_replace(ffmpeg, AudioRegex, vid);
        result = std::regex_replace(result, thumbnailRegex, thumbnail);

        system(result.c_str());
        threadCount -= 1;
    }
    
    void Factory(std::vector<File> files, std::string path, std::string outputFolder){
        queue<File> threadqueue;
        std::atomic<int> threadCountInUse(0);

        std::vector<std::thread> threadsInUse;

        //create queue

        for (int i = 0; i < files.size(); i++)
        {
            threadqueue.push_back(files[i]);
        }

        log("7")
        

        //loop the queue
        while (true)
        {
            if (threadCountInUse < 100)
            {
                try
                {
                    File f = threadqueue.pop_front();
                    threadsInUse.push_back(std::thread(&ThreadFactory::convert, this, f, outputFolder, std::ref(threadCountInUse)));  
                    //auto thread = std::thread(&ThreadFactory::test, this, 1);
                }
                catch(const std::out_of_range& e)
                {
                    break;
                }                
            }
        }
        
        for (int i = 0; i < threadsInUse.size(); i++)
        {
            threadsInUse[i].join();
        }
        

    }

    public:
    void startFactory(std::vector<File> files, std::string folder, std::string outputfolder){
        log(5)
        auto factory = std::thread(&ThreadFactory::Factory, this, files, folder, outputfolder);
        factory.join();
        log(6)
    }  
};

bool convertwebmp(std::string name, std::string folder, std::string output){
    
    std::regex AudioRegex("audio");
    std::regex thumbnailRegex("image");
    std::regex outputRegex("output");

    std::string vid = folder + "/" + name + ".webm";
    std::string thumbnail = folder + "/" + name + ".webp";
    output = output;

    std::string ffmpeg = "ffmpeg -i audio -i image -c:a libmp3lame -q:a 2 -map 0:a -map 1:v -c:v copy -id3v2_version 3 -metadata:s:v title=\"Album cover\" -metadata:s:v comment=\"Cover (front)\" output";
    
    std::string result = std::regex_replace(ffmpeg, AudioRegex, vid);
    result = std::regex_replace(result, thumbnailRegex, thumbnail);

    system(result.c_str());
    return true;
}


bool findFiles(std::vector<File> &files, std::string folder){
    for (const auto entry : std::filesystem::directory_iterator(folder))
    {
        files.push_back(File(entry.path().filename().stem().string(), entry.path().parent_path()));
    }
    
    return true;
}


int main(int, char**){
    std::vector<File> files;
    std::string folder = "/home/hiha/Music/test";
    log("1")
    findFiles(files, folder);
    log("2")

    ThreadFactory fac;
    log("3")
    fac.startFactory(files, folder, folder + "/mp3_output");
    log(4)

    return 0;
}
