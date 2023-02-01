/*
 Narzedzie do kompresji oparte o algorytm LZW.
 ----------------------------------------------------------------------
 Autor: Michal Kosciolek
 ----------------------------------------------------------------------
*/

#include <fstream>
#include "Dict.hpp"

const int dict_size = 4096;

void LZWcompress(const std::string& inputFile){
    Dict<std::string, std::string> dict;
    std::pair<std::string, std::string> p;

    /* Tworzenie domyslej wersji slownika */
    for(int i=0; i<256; i++){
        p = std::make_pair(i, std::to_string(i));
        dict.insert(p);
    }

    std::ifstream input (inputFile);
    std::ofstream output ("compressed.bin", std::ios::out | std::ifstream::binary);

    /* Zapisywanie rozmiaru pliku do kompresji */
    input.seekg(0, std::ios::end);
    int inputFileSize = input.tellg();
    input.seekg (0, input.beg);

    if(output.is_open() && input.is_open()){

        /* Wczytywanie danych z pliku do listy */
        char ch;
        List<char> in;
        while (input >> std::noskipws >> ch) {
            in.push_back(ch);
        }

        int code = 256;
        std::string next;
        std::string current(1, in.pop_front());
        uint16_t number;

        while(in.size()){

            /* Resetowanie slownika do stanu domyslnego po przekroczeniu maksymalnego rozmiaru */
            if(code == dict_size){

                /* Wpisywanie kodu znaku do pliku */
                number = static_cast<uint16_t>(std::stoi(dict[current]));
                output.write(reinterpret_cast<char*>(&number), sizeof(number));

                dict.clear();
                for(int j=0; j<256; j++){
                    p = std::make_pair(j, std::to_string(j));
                    dict.insert(p);
                }

                code = 256;
                next.clear();
                current = std::string(1,in.pop_front());
            }

            if(in.size() != 0){
                next += in.pop_front();
            }
            if(dict.find(current + next)){
                current += next;
            }
            else{

                /* Wpisywanie kodu znaku do pliku */
                number = static_cast<uint16_t>(std::stoi(dict[current]));
                output.write(reinterpret_cast<char*>(&number), sizeof(number));

                /* Dodawanie nowego znaku do slownika */
                p = std::make_pair(current + next, std::to_string(code));
                dict.insert(p);

                current = next;
                code++;
            }
            next.clear();
        }

        /* Wpisywanie ostataniego kodu znaku do pliku */
        number = static_cast<uint16_t>(std::stoi(dict[current]));
        output.write(reinterpret_cast<char*>(&number), sizeof(number));

        /* Komunikaty */
        int compressedFileSize = output.tellp();
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "Kompresja udana." << std::endl << "Rozmiar oryginalnego pliku: " << inputFileSize << " bajtow."
                  << std::endl << "Rozmiar skompresowanego pliku: " << compressedFileSize << " bajtow." << std::endl
                  << "Wspolczynnik kompresji wynosi: " << (double)compressedFileSize/(double)inputFileSize*100 << "%" << std::endl;
        output.close();
        input.close();
    }
}

void LZWdecompress(const std::string& inputFile){
    Dict<std::string, std::string> dict;
    std::pair<std::string, std::string> p;

    //Tworzenie domyslej wersji slownika
    for(int i=0; i<256; i++){
        p = std::make_pair(std::to_string(i), i);
        dict.insert(p);
    }
    std::ifstream input (inputFile, std::ios::in | std::ifstream::binary);
    std::ofstream output ("decompressed.txt");

    if(output.is_open() && input.is_open()){
        std::string character;
        List<std::string> toDecode;
        int code = 256;

        //Wczytywanie danych do dekompresji
        uint16_t number;
        while(input.read(reinterpret_cast<char*>(&number), sizeof(number))){
            toDecode.push_back(std::to_string(number));
        }

        std::string prev = toDecode.pop_front();
        std::string current = dict[prev];
        std::string first;
        output << current;

        int size = toDecode.size();
        for(int i=0; i<size; i++){
            std::string next = toDecode.pop_front();

            //Resetowanie slownika do stanu domyslnego
            if(code == dict_size+1){
                dict.clear();
                for(int j=0; j<256; j++){
                    p = std::make_pair(std::to_string(j), j);
                    dict.insert(p);
                }
                code = 256;
            }

            if(!dict.find(next)){
                current = dict[prev] + first;
            }
            else{
                current = dict[next];
            }
            first = std::string(1,current[0]);

            /* Dodawanie nowego znaku do slownika */
            p = std::make_pair(std::to_string(code), dict[prev] + first);
            dict.insert(p);

            prev = next;
            code++;
            output << current;
        }
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "Dekompresja udana." << std::endl;
        std::cout << "---------------------------------------------" << std::endl;

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
    LZWdecompress("compressed.bin");

    return 0;
}