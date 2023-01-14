#include <fstream>
#include "Dict.hpp"

std::string LZWcompress(std::string toCompress){

}

std::string LZWdecompress(std::string toDecompress){

}

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cerr << "Bledna liczba argumentow" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    std::ofstream output("compressed.txt");
    if(input.is_open() && output.is_open()){
        std::string line;
        while(getline(input, line)){
            output << LZWcompress(line) << std::endl;
        }
        std::cout << "Kompresja udana." << std::endl << "Rozmiar oryginalnego pliku: " << input.tellg() << " bajtow."
        << std::endl << "Rozmiar skompresowanego pliku: " << output.tellp() << " bajtow." << std::endl;
        input.close();
        output.close();
    }

    std::ifstream input2("compressed.txt");
    std::ofstream output2("decompressed.txt");
    if(input2.is_open() && output2.is_open()){
        std::string line;
        while(getline(input2, line)){
            output2 << LZWdecompress(line) << std::endl;
        }
        std::cout << "Dekompresja udana." << std::endl << "Rozmiar skompresowanego pliku: " << input.tellg() << " bajtow."
                  << std::endl << "Rozmiar zdekompresowanego pliku: " << output.tellp() << " bajtow." << std::endl;
        input2.close();
        output2.close();
    }

    return 0;
}