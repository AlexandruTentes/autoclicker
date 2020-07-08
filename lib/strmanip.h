#ifndef STRMANIP_H_
#define STRMANIP_H_

#include <iostream>
#include <fstream>

namespace string_manipulation
{
    //================
    //
    //  Implementing the struct Array
    //
    //================

    //Array struct creates a dynamic array of any type and stores it s size

    template <typename T>
    struct Array
    {
        Array<T> * addr = nullptr;
        T * array = nullptr;
        int size = 0;

        Array()
        {}

        Array(int size)
        {
            Array::size = size;
            array = new T[size];
        }

        Array(T * array, int size)
        {
            delete [] Array::array;
            Array::array = new T[size];
            Array::size = size;

            for(int i = 0; i < size; i++)
                Array::array[i] = array[i];

            delete [] array;
        }

        Array(T * array, int size, Array<T> * addr)
        {
            delete Array::addr;
            Array::addr = addr;

            delete [] Array::array;
            Array::array = new T[size];
            Array::size = size;

            for(int i = 0; i < size; i++)
                Array::array[i] = array[i];

            delete [] array;
        }

        void set_addr(Array<T> * addr)
        {
            delete Array::addr;
            Array::addr = addr;
        }

        void set(int size)
        {
            delete [] array;
            array = new T[size];
            Array::size = size;
        }

        void set(T * arr, int size)
        {
            delete [] array;
            array = arr;
            Array::size = size;
        }

        void set(Array<T> array)
        {
            int size = array.size;
            
            delete [] Array::array;
            Array::array = new T[size];
            Array::size = size;

            for(int i = 0; i < size; i++)
                Array::array[i] = array.array[i];
        }

        void resize(int size)
        {
            T * aux = new T[size];

            for(int i = 0; i < Array::size; i++)
                aux[i] = array[i];

            Array::size = size;
            delete [] array;

            array = aux;
        }

        void remove()
        {
            size = 0;
            delete [] array;
        }

        void print()
        {
            for(int i = 0; i < size; i++)
                std::cout << array[i] << ( i + 1 != size ? ", " : ";" );
        }

        void clear()
        {
            delete addr;
        }

        ~Array()
        {
            delete [] array;
        }
    };

    //end of structs

    //
    //  GLOBAL VARIABLES
    //

    //end of global variables

    //================
    //
    //  Implementing the namespace's functions
    //
    //================

    //function size() returns the size of a sctring

    extern int size(std::string input_str);

    //end of size()

    //function char_to_string() returns a string from a (char *). It's a very bad alternative

    extern std::string char_to_string(const char * input);
    extern std::string char_to_string(char * input);

    //end of size()

    //function split() literally splits a string by a character and returns an Array of strings
    
    extern Array<std::string> & split(std::string arr, char c);

    //end of split()

    //
    //  Implementing replace() function which searches all characters to be replaced and replaces them by the given character
    //

    //replace() function returns a string of the original given string but without all aparitions of a given character

    extern std::string replace(std::string arr, std::string to_replace, std::string c);

    //override replace() which replaces all substrings of to_replace[] with a given other string

    extern std::string replace(std::string arr, std::string to_replace[], int len, std::string c);

    //override of replace() which replaces to_replace[] with c[]

    extern std::string replace(std::string arr, std::string to_replace[], int len, std::string c[]);

    //end of replace()

    //replace_duplicate() replaces all duplicates of a string with another given one

    extern std::string replace_duplicate(std::string input_string, std::string to_replace, std::string replace_with);

    //end of replace_duplicate()

    //
    //  file read/write functions
    //

    extern std::string read(std::string str);

    //a write() function which writes to a give file and a given mode

    extern void write(std::string file, std::string output, std::string mode, bool newline);

    //end of read/write

    //grep() function returns 1 if the key string matches the main string and 0 otherwise

    extern int grep(std::string str, std::string key);
    //end of fstream functions

    //
    //  MEMORY CLEAR FUNCTIONS
    //

    //  buffer clear function

    extern void clear();

    //end of delete_duffer function

    //end of functions

    //
    //  OPERATOR DECLARATIONS
    //

    // the << declaration

    template <class T>
    std::ostream & operator << (std::ostream & os, Array<T> const & arr)
    {
        std::string output = "";
    
        for(int i = 0; i < arr.size; i++)
            output += arr.array[i] + ( i + 1 != arr.size ? ", " : ";" );

        return os << output;
    }

    //end of << declaration

    //end of operator declarations

}

#endif