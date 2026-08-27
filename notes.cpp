#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <filesystem>
#include <functional>
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
void list() {
    
    std::ifstream file("data.txt");

    std::string line;
    while (std::getline(file,line))  {
        size_t colonPos = line.find(':');
        
        if (colonPos != std::string::npos) {
            std::string noteName = line.substr(0, colonPos);
            std::string noteText = line.substr(colonPos + 1);

            std::cout << noteName << " - " << noteText << std::endl;
        }
    }
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
void renameCin() {
    std::string name,newName;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,name);
    std::cout << "\nEnter newName of note " << name << ": ";
    std::getline(std::cin,newName);
    renameNote(name,newName);
}

void appendCin() {
    std::string name,value;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,name);
    std::cout << "\nEnter: ";
    std::getline(std::cin,value);
    append(name,value);
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
    note n = get(name);
    std::cout << n.text << std::endl;
}
void changeCin() {
    std::string name;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,name);
    note n = get(name);
    std::string text;
    std::cout << "\nEnter new text: ";
    std::getline(std::cin,text);
    change(std::move(n),text);
}
void removeCin() {
    std::string name;
    std::cout << "\nEnter name of note: ";
    std::getline(std::cin,name);
    removeNote(name);
}
void help() {
    std::cout << "| list | get all notes\n| upload | save note in file\n| change | change note text\n| get | get note by name\n| remove | remove note\n| rename | rename note\n| help | this message\n| q | quit\n| count | count all notes\n| append | append text to note\n";
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
        {"append",appendCin}
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