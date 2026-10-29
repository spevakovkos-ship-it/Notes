#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <random>
#include <chrono>
struct note {
    std::string text;
    std::string name;
}; 

void upload(note&& note) {
    std::ofstream file("data.txt",std::ios_base::app);

    if (!file.is_open())  
        return;
    file << '\n' <<note.name << ":" << note.text;
   
}

void find(const std::string& text) { 
    std::ifstream file("data.txt");

    std::string line;
    while (std::getline(file,line)) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) { 
            std::string noteName = line.substr(0,colonPos);
            std::string noteText = line.substr(colonPos + 1);

            if (noteText.find(text)!=std::string::npos) {
                std::cout << noteName << " - " << noteText << '\n';

            }
        }
    }
}

void findPinned(const std::string& text) { 
    std::ifstream file("pinned.txt");

    std::string line;
    while (std::getline(file,line)) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) { 
            std::string noteName = line.substr(0,colonPos);
            std::string noteText = line.substr(colonPos + 1);

            if (noteText.find(text)!=std::string::npos) {
                std::cout << noteName << " - " << noteText << '\n';
            }
        }
    }
}
note getPinned(const std::string& name) {
    note n;

    std::ifstream file("pinned.txt");

    std::string line;
    while (std::getline(file,line)) {

        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {

            std::string noteName = line.substr(0,colonPos);
            std::string text = line.substr(colonPos + 1);

            if (name == noteName) {
                n.name = noteName;
                n.text = text;
                return n;
            } 
        }
    }
    return n;
}
note get(const std::string& name) {
    note n;

    std::ifstream file("data.txt");

    std::string line;
    while (std::getline(file,line)) {

        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {

            std::string noteName = line.substr(0,colonPos);
            std::string text = line.substr(colonPos + 1);

            if (name == noteName) {
                n.name = noteName;
                n.text = text;
                return n;
            } 
        }
    }
    return n;
}
void change(note&& n, const std::string& newText) {
    std::ifstream inFile("data.txt");
    std::ofstream outFile("temp.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        size_t colonPos = line.find(':');
        
        if (colonPos != std::string::npos) {
            std::string noteName = line.substr(0, colonPos);
            
            if (noteName == n.name) {
                n.text = newText; 
                
                outFile << noteName << ":" << newText << "\n";
                continue; 
            }
        }
        
        outFile << line << "\n";
    }

    inFile.close();
    outFile.close();

    std::filesystem::remove("data.txt");
    std::filesystem::rename("temp.txt", "data.txt");
}
void changePinned(note&& n, const std::string& newText) {
    std::ifstream inFile("pinned.txt");
    std::ofstream outFile("temp.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        size_t colonPos = line.find(':');
        
        if (colonPos != std::string::npos) {
            std::string noteName = line.substr(0, colonPos);
            
            if (noteName == n.name) {
                n.text = newText; 
                
                outFile << noteName << ":" << newText << "\n";
                continue; 
            }
        }
        
        outFile << line << "\n";
    }

    inFile.close();
    outFile.close();

    std::filesystem::remove("pinned.txt");
    std::filesystem::rename("temp.txt", "pinned.txt");
}

void list() {
    
    std::ifstream file("data.txt");
    std::ifstream pinnedFile("pinned.txt");
    std::string pinnedLine;
    std::cout << "-------------------Pinned-------------------\n";
    bool oneInPinned = false;
    while (std::getline(pinnedFile,pinnedLine))  {
        size_t colonPos = pinnedLine.find(':');
        
        if (colonPos != std::string::npos) {
            oneInPinned = true;
            std::string pinnedNoteName = pinnedLine.substr(0, colonPos);
            std::string pinnedNoteText = pinnedLine.substr(colonPos + 1);

            std::cout << pinnedNoteName << " - " << pinnedNoteText << std::endl;
        }
    } 
    if (!oneInPinned) std::cout << "Pinned Is empty\n";
    std::cout << "-------------------Standard-----------------\n";
    std::string line;
    bool oneInStandard = false;
    while (std::getline(file,line))  {
        size_t colonPos = line.find(':');
        
        if (colonPos != std::string::npos) {
            oneInStandard = true;
            std::string noteName = line.substr(0, colonPos);
            std::string noteText = line.substr(colonPos + 1);

            std::cout << noteName << " - " << noteText << std::endl;
        }
    }
    if (!oneInStandard) std::cout << "Standard is empty\n";
}
void duplicateNote(const std::string& name,const std::string& newName) {
    note n = get(name);
    n.name = newName;
    upload(std::move(n));
}
void pin(note&& n) {
    std::ofstream outFile("pinned.txt",std::ios_base::app);

    outFile << n.name << ":" << n.text << '\n';   
 
 
}
void duplicatePinnedNote(const std::string& name,const std::string& newName) {
    note n = getPinned(name);
    n.name = newName;
    pin(std::move(n));
}
void removeNote(const std::string& name) {
    std::ifstream inFile("data.txt");
    std::ofstream outFile("temp.txt",std::ios_base::app);

    if (!inFile || !outFile) return;

    std::string line;
    while (std::getline(inFile,line))  {
        size_t colonPos = line.find(':');
        
        if (colonPos != std::string::npos) {
            std::string noteName = line.substr(0, colonPos);
            std::string noteText = line.substr(colonPos + 1);
            if (noteName == name) continue;
            
            outFile << noteName << ':' << noteText << '\n';
        }
    }
    inFile.close();
    outFile.close();
    std::filesystem::remove("data.txt");
    std::filesystem::rename("temp.txt","data.txt");
}
void removePinnedNote(const std::string& name) {
    std::ifstream inFile("pinned.txt");
    std::ofstream outFile("temp.txt",std::ios_base::app);

    if (!inFile || !outFile) return;

    std::string line;
    while (std::getline(inFile,line))  {
        size_t colonPos = line.find(':');
        
        if (colonPos != std::string::npos) {
            std::string noteName = line.substr(0, colonPos);
            std::string noteText = line.substr(colonPos + 1);
            if (noteName == name) continue;
            
            outFile << noteName << ':' << noteText << '\n';
        }
    }
    inFile.close();
    outFile.close();
    std::filesystem::remove("pinned.txt");
    std::filesystem::rename("temp.txt","pinned.txt");
}
void randomNote() {
    std::ifstream file("data.txt");
    int lineCount = 0;
    std::string line;

    while (std::getline(file, line)) {
        ++lineCount;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0,lineCount-1);
    file.clear();                
    file.seekg(0, std::ios::beg);
    int randPos = distr(gen);
    int i = 0;
    line = "";
    while (std::getline(file, line)) {
        if (i == randPos) {
            std::string name = line.substr(0,line.find(':'));
            note n = get(name);
            std::cout << n.name << " - " << n.text << std::endl;
            break;
        }
        ++i;
    }
    
}
void randomPinnedNote() {
    std::ifstream file("pinned.txt");
    int lineCount = 0;
    std::string line;

    while (std::getline(file, line)) {
        ++lineCount;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0,lineCount-1);
    file.clear();                
    file.seekg(0, std::ios::beg);
    int randPos = distr(gen);
    int i = 0;
    line = "";
    while (std::getline(file, line)) {
        if (i == randPos) {
            std::string name = line.substr(0,line.find(':'));
            note n = getPinned(name);
            std::cout << n.name << " - " << n.text << std::endl;
            break;
        }
        ++i;
    }
    

}

void renameNote(const std::string& name,const std::string& newName) {
    std::ifstream inFile("data.txt");
    std::ofstream outFile("temp.txt",std::ios_base::app);
    if (!inFile || !outFile) return;
    std::string line;
    while (std::getline(inFile,line))  {
        size_t colonPos = line.find(':');
        
        if (colonPos != std::string::npos) {
            std::string noteName = line.substr(0, colonPos);
            std::string noteText = line.substr(colonPos + 1);
            if (noteName == name) {
                outFile << newName << ':' << noteText;
                continue;
            }
            
            outFile << noteName << ':' << noteText << '\n';
        }
    }
    outFile.close();
    inFile.close();
    std::filesystem::remove("data.txt");
    std::filesystem::rename("temp.txt","data.txt");
}
void renamePinnedNote(const std::string& name,const std::string& newName) {
    std::ifstream inFile("pinned.txt");
    std::ofstream outFile("temp.txt",std::ios_base::app);
    if (!inFile || !outFile) return;
    std::string line;
    while (std::getline(inFile,line))  {
        size_t colonPos = line.find(':');
        
        if (colonPos != std::string::npos) {
            std::string noteName = line.substr(0, colonPos);
            std::string noteText = line.substr(colonPos + 1);
            if (noteName == name) {
                outFile << newName << ':' << noteText;
                continue;
            }
            
            outFile << noteName << ':' << noteText << '\n';
        }
    }
    outFile.close();
    inFile.close();
    std::filesystem::remove("pinned.txt");
    std::filesystem::rename("temp.txt","pinned.txt");
}
void count() {
    
    std::ifstream file("data.txt");
    int count = 0;
    std::string line;
    while (std::getline(file,line))  {
        if ((line.find(':')) != std::string::npos)
            ++count;
    }
    std::cout << count << std::endl;
}
void countPinned() {
    
    std::ifstream file("pinned.txt");
    int count = 0;
    std::string line;
    while (std::getline(file,line))  {
        if ((line.find(':')) != std::string::npos)
            ++count;
    }
    std::cout << count << std::endl;
}
void append(const std::string name,const std::string& value) {
    std::ifstream inFile("data.txt");
    std::ofstream outFile("temp.txt");

    std::string line;
    while (getline(inFile,line)) {
        
        size_t colonPos = line.find(':');

        if (colonPos != std::string::npos) {
            std::string noteName = line.substr(0,colonPos);
            std::string noteText = line.substr(colonPos + 1);

            if (noteName == name) {
                outFile << noteName << ':' << (noteText + value)<< '\n';
                continue;
            }
        }
        outFile << line << '\n';
        
    }

    inFile.close();
    outFile.close();

    std::filesystem::remove("data.txt");
    std::filesystem::rename("temp.txt","data.txt");
}
void appendPinned(const std::string name,const std::string& value) {
    std::ifstream inFile("pinned.txt");
    std::ofstream outFile("temp.txt");

    std::string line;
    while (getline(inFile,line)) {
        
        size_t colonPos = line.find(':');

        if (colonPos != std::string::npos) {
            std::string noteName = line.substr(0,colonPos);
            std::string noteText = line.substr(colonPos + 1);

            if (noteName == name) {
                outFile << noteName << ':' << (noteText + value)<< '\n';
                continue;
            }
        }
        outFile << line << '\n';
        
    }

    inFile.close();
    outFile.close();

    std::filesystem::remove("pinned.txt");
    std::filesystem::rename("temp.txt","pinned.txt");
}

void renameCin() {
    std::string name,newName;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,name);
    std::cout << "\nEnter newName of note " << name << ": ";
    std::getline(std::cin,newName);
    std::string mode;
    std::cout << "\nEnter mode [pinned,standard]: ";
    std::getline(std::cin,mode);
    if (mode == "pinned") {
        renamePinnedNote(name,newName);
    } else if (mode == "standard") {
        renameNote(name,newName);
    }
}
void duplicateNoteCin(){
    std::string name,newName;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,name);
    std::cout << "\nEnter name of duplicate note: ";
    std::getline(std::cin,newName);
    std::string mode;
    std::cout << "\nEnter mode [pinned,standard]: ";
    std::getline(std::cin,mode); 
    if (mode == "pinned") {
        duplicatePinnedNote(name,newName);
    } else if (mode == "standard") {
        duplicateNote(name,newName);
    }
}
void appendCin() {
    std::string name,value;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,name);
    std::cout << "\nEnter: ";
    std::getline(std::cin,value);
    std::string mode;
    std::cout << "\nEnter mode [pinned,standard]: ";
    std::getline(std::cin,mode);
    if (mode == "pinned") {
        appendPinned(name,value);
    } else if (mode == "standard") {
        append(name,value);
    }
}
void uploadCin() {
    note n;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,n.name);
    std::cout << "Enter text: ";
    std::getline(std::cin,n.text);
    upload(std::move(n));
}
void getCin() {
    std::string name;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,name);
    std::string mode;
    std::cout << "\nEnter mode [pinned,standard]: ";
    std::getline(std::cin,mode);
    note n;
    if (mode == "pinned") {
        n = getPinned(name);
        std::cout << n.text  << std::endl;
    } else if (mode == "standard") {
        n = get(name);
        std::cout << n.text << std::endl;
    }
   
}
void changeCin() {
    std::string name;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,name);
    std::string text;
    std::cout << "\nEnter new text: ";
    std::getline(std::cin,text);
    std::string mode;
    std::cout << "\nEnter mode [pinned,standard]: ";
    std::getline(std::cin,mode);
    if (mode == "standard"){
        note n = get(name);
        change(std::move(n),text);
    } else if (mode == "pinned") {
        note n = getPinned(name);
        changePinned(std::move(n),text);
    }
}
void removeCin() {
    std::string name;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,name);
    std::string mode;
    std::cout << "\nEnter mode [pinned,standard]: ";
    std::getline(std::cin,mode);
    if (mode == "standard") {
        removeNote(name);
    } else if (mode == "pinned") {
        removePinnedNote(name);
    }
    
}
void pinCin() {
    note n;
    std::cout << "\nEnter name of note:";
    std::getline(std::cin,n.name);
    std::cout << "\nEnter text:";
    std::getline(std::cin,n.text);
    pin(std::move(n));
}
void findCin() {
    std::cout << "\nEnter text: ";
    std::string text;
    std::getline(std::cin,text);
    std::string mode;
    std::cout << "\nEnter mode [pinned,standard]: ";
    std::getline(std::cin,mode);
    if (mode == "standard") {
        find(text);
    } else if (mode == "pinned") {
        findPinned(text);
    }
    
}


void help() {
    std::cout << "\n--------command--------action--------\n";
    std::cout << "|\033[32m list \033[0m| get all notes\n|\033[32m upload \033[0m| save note in file\n|\033[32m change\033[0m | change note text\n|\033[32m get \033[0m| get note by name\n|\033[32m remove \033[0m| remove note\n|\033[32m rename \033[0m| rename note\n|\033[32m help \033[0m| this message\n|\033[32m q \033[0m| quit\n|\033[32m count \033[0m| count all notes\n|\033[32m append \033[0m| append text to note\n|\033[32m pin \033[0m| pin note\n|\033[32m countPinned \033[0m| count all pinned notes\n|\033[32m randomPinned \033[0m| random pinned note\n";
}
std::string userInput() {
    std::string cmd;
    std::cout << "Enter you action: ";
    std::getline(std::cin,cmd);
    return cmd;
}
void executeCommand(const std::string& name) {
    using command = std::function<void()>;
    const static std::unordered_map<std::string,command> commands = {
        {"get",getCin},
        {"change",changeCin},
        {"upload",uploadCin},
        {"list",list},
        {"remove",removeCin},
        {"rename",renameCin},
        {"help",help},
        {"count",count},
        {"append",appendCin},
        {"random",randomNote},
        {"randomPinned",randomPinnedNote},
        {"duplicate",duplicateNoteCin},
        {"pin",pinCin},
        {"countPinned",countPinned},
        {"find",findCin},  
    };
   
    auto it = commands.find(name);

    if (it != commands.end()) {
        std::invoke(it->second);
    } else {
        std::cout << "Unknown command" << std::endl;
    }
}

int main() {
    while (true) { 
        std::string cmd = userInput();
        if (cmd == "q") break;
        executeCommand(cmd);
    }
    return 0;
}