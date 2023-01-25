#include <fstream>
#include "Dict.hpp"

void LZWcompress(const std::string& inputFile){
    Dict<std::string, int> dict;
    std::pair<std::string, int> p;
    const int buffer_size = 600;
    for(int i=0; i<256; i++){
        p = std::make_pair(i, (unsigned char) i);
        dict.insert(p);
    }

    std::ifstream input (inputFile, std::ifstream::binary);
    std::ofstream output ("compressed.txt", std::ifstream::binary);
    input.seekg(0, std::ios::end);
    int inputFileSize = input.tellg();
    input.seekg (0, input.beg);
    char * buffer = new char[buffer_size+1];
    if(output.is_open() && input.is_open()){
        while(true){
            input.read(buffer, buffer_size);
            int code = 256;
            std::string next;
            std::string toCompress(buffer);
            std::string current(1, toCompress[0]);
            for(int i=0; i<toCompress.size(); i++){
                if(i != toCompress.size()-1)
                    next += toCompress[i+1];
                if(dict.find(current + next)){
                    current += next;
                }
                else{
                    p = std::make_pair(current + next, code);
                    output << dict[current] << " ";
                    dict.insert(p);
                    current = next;
                    code++;
                }
                next.clear();
            }
            output << dict[current] << " ";
            if(input.eof())
                break;
        }
        int compressedFileSize = output.tellp();
        std::cout << "Kompresja udana." << std::endl << "Rozmiar oryginalnego pliku: " << inputFileSize << " bajtow."
                  << std::endl << "Rozmiar skompresowanego pliku: " << compressedFileSize << " bajtow." << std::endl
                  << "Wspolczynnik kompresji wynosi: " << (double)compressedFileSize/(double)inputFileSize*100 << "%" << std::endl;
        output.close();
        input.close();
        delete[] buffer;
    }
}

void LZWdecompress(std::string inputFile){
    Dict<std::string, std::string> dict;
    std::pair<std::string, std::string> p;
    for(int i=0; i<256; i++){
        p = std::make_pair(std::to_string(i), i);
        dict.insert(p);
    }
    std::ifstream input (inputFile, std::ifstream::binary);
    //std::ifstream input ("compressed2.txt", std::ifstream::binary);
    std::ofstream output ("decompressed.txt", std::ifstream::binary);

    if(output.is_open() && input.is_open()){
        std::string character;
        List<std::string> toDecode;
        int code = 256;
        while(input >> character){
            toDecode.push_back(character);
        }
        std::string prev = toDecode.pop_front();
        std::string current = dict[prev];
        std::string first;
        output << current;
        int size = toDecode.size();
        for(int i=0; i<size; i++){
            std::string next = toDecode.pop_front();
            if(!dict.find(next)){
                current = dict[prev] + first;
            }
            else{
                current = dict[next];
            }
            first = std::string(1,current[0]);
            p = std::make_pair(std::to_string(code), dict[prev] + first);
            dict.insert(p);
            prev = next;
            code++;
            output << current;
        }
        std::cout << "--------------------------------------------" << std::endl;
        std::cout << "Dekompresja udana." << std::endl;
        output.close();
        input.close();
    }
}

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cerr << "Bledna liczba argumentow" << std::endl;
        return 1;
    }

    LZWcompress(argv[1]);
    LZWdecompress("compressed.txt");

    return 0;
}