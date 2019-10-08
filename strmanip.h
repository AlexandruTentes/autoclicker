#ifndef STRMANIP_H_
#define STRMANIP_H_

#include <iostream>
#include <fstream>


//namespace for string manipulation functions

namespace string_manipulation
{

    //
    //  Implementation of classes
    //


    //class

    //end of classes


    //================
    //
    //  Implementing the struct Array
    //
    //================


    //
    //  Implementing custom structs
    //


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

    Array<std::string> * output = nullptr;
    Array<std::string> * buffer = nullptr;

    //end of global variables

    //================
    //
    //  Implementing the namespace's functions
    //
    //================


    //
    //  Implementing string manipulation functions
    //


    //function split() literally splits a string by a character and returns an Array of strings
    
    Array<std::string> & split(std::string arr, char c)
    {
        int size = 1;
        int i = 0;

        while(arr[i] != '\0')
        {
            if(arr[i] == c)
                size++;

            i++;
        }
            
        i = 0;
        std::string * aux = new std::string[size];
        int j = 0;

        while(arr[i] != '\0')
        {
            if(arr[i] != c)
                aux[j] += arr[i];
            else
            {
                if(aux[j] != "")
                    j++;

                if(arr[i + 1] == '\0')
                    j--;
            }

            i++;
        }

        output = new Array<std::string>(aux, j + 1);
        output->set_addr(output);
        buffer = output;
        
        return * output;
    }

    //end of split()

    //
    //  Implementing replace() function which searches all characters to be replaced and replaces them by the given character
    //

    //replace() function returns a string of the original given string but without all aparitions of a given character

    int size(std::string input_str)
    {
        int i = 0;

        while(input_str[i] != '\0')
            i++;

        return i;
    }

    std::string replace(std::string arr, std::string to_replace, std::string c)
    {
        std::string output = "";
        std::string aux_str = "";
        int i = 0, j = 0, k = 0, z = 0, aux_i = 0;
        bool start_replace = false;
        int to_replace_size;

        while(arr[i] != '\0')
        {
                aux_str = "";
                start_replace = false;

                if(arr[i] == to_replace[0])
                {
                    aux_i = i;

                    to_replace_size = size(to_replace);

                    while(aux_i - i < to_replace_size)
                    {
                        if(arr[aux_i] != '\0')
                            aux_str += arr[aux_i];
                        else
                        {
                            aux_str = "";
                            break;
                        }

                        aux_i++;
                    }

                    if(aux_str == to_replace)
                    {
                        i += to_replace_size - 1;
                        aux_str = "";
                        start_replace = true;
                    }
                }
            
            if(!start_replace)
                output += arr[i];

            if(start_replace)
                output += c;

            i++;
        }

        return output;
    }

    std::string replace(std::string arr, std::string to_replace[], int len, std::string c)
    {
        std::string output = "";
        std::string aux_str = "";
        int i = 0, j = 0, k = 0, z = 0, aux_i = 0;
        bool start_replace = false;
        int to_replace_size;

        while(arr[i] != '\0')
        {
            while(z < len)
            {
                aux_str = "";
                start_replace = false;

                if(arr[i] == to_replace[z][0])
                {
                    aux_i = i;

                    to_replace_size = size(to_replace[z]);

                    while(aux_i - i < to_replace_size)
                    {
                        if(arr[aux_i] != '\0')
                            aux_str += arr[aux_i];
                        else
                        {
                            aux_str = "";
                            break;
                        }

                        aux_i++;
                    }

                    if(aux_str == to_replace[z])
                    {
                        i += to_replace_size - 1;
                        aux_str = "";
                        start_replace = true;

                        break;
                    }
                }

                z++;
            }
            
            if(!start_replace)
                output += arr[i];

            if(start_replace)
                output += c;

            i++;
            z = 0;
        }

        return output;
    }

    std::string replace(std::string arr, std::string to_replace[], int len, std::string c[])
    {
        std::string output = "";
        std::string aux_str = "";
        int i = 0, j = 0, k = 0, z = 0, aux_i = 0;
        bool start_replace = false;
        int to_replace_size;

        while(arr[i] != '\0')
        {
            while(z < len)
            {
                aux_str = "";
                start_replace = false;

                if(arr[i] == to_replace[z][0])
                {
                    aux_i = i;

                    to_replace_size = size(to_replace[z]);

                    while(aux_i - i < to_replace_size)
                    {
                        if(arr[aux_i] != '\0')
                            aux_str += arr[aux_i];
                        else
                        {
                            aux_str = "";
                            break;
                        }

                        aux_i++;
                    }

                    if(aux_str == to_replace[z])
                    {
                        i += to_replace_size - 1;
                        aux_str = "";
                        start_replace = true;

                        break;
                    }
                }

                z++;
            }
            
            if(!start_replace)
                output += arr[i];

            if(start_replace)
                output += c[z];

            i++;
            z = 0;
        }

        return output;
    }

    //end of replace()

    //replace_duplicate()

    std::string replace_duplicate(std::string input_string, std::string to_replace, std::string replace_with)
    {
        int i = 0, j = 0, aux_i = 0;
        bool start_replace = false;
        std::string check_str = "";
        std::string output = "";

        while(input_string[i] != '\0')
        {
            start_replace = false;

            if(input_string[i] == to_replace[0])
                start_replace = true;

            if(start_replace)
            {
                aux_i = i;

                while(to_replace[j] != '\0')
                {
                    if(input_string[aux_i] != '\0' && input_string[aux_i] == to_replace[j])
                        check_str += to_replace[j];
                    else
                    {
                        check_str = "";
                        break;
                    }

                    aux_i++;
                    j++;
                }

                //if(check_str != "")
                    //i += j - 1;
            }

            if(!start_replace || check_str == "")
                output += input_string[i];

            if(check_str != "")
            {
                if(input_string[i + 1] != to_replace[0])
                    output += replace_with;

                check_str = "";
            }

            i++;
            j = 0;
        }

        return output;
    }

    //end of replace_duplicate()


    //
    //  file read/write functions
    //
    

    std::string read(std::string str)
    {
        std::string output = "";
        std::ifstream fout;
        fout.open(str);

        char c = fout.get();

        while(fout.good())
        {
            output += c;

            c = fout.get();
        }

        fout.close();

        return output;
    }


    void write(std::string file, std::string output, std::string mode)
    {
        std::ofstream fin;
        fin.open(file, ( mode == "trunc" ? std::fstream::trunc : ( mode == "app" ? std::fstream::app : std::fstream::out )));

        fin << output;

        fin.close();
    }


    //grep() function returns 1 if the key string matches the main string and 0 otherwise

    int grep(std::string str, std::string key)
    {
        int i = 0;
        int j = 0;
        bool check = false;
        int response = 0;

        while(str[i] != '\0')
        {
            if(str[i] == key[j])
            {
                check = true;
            }
            else
                if(key[j] != '\0')
                {
                    check = false;
                    j = 0;
                }

            i++;
            if(check & key[j] != '\0')
                j++;

            if(str[i] == '\0' && key[j] != '\0')
                response = 0;

            if(str[i] == '\0' && key[j] == '\0' && check)
                response = 1;
        }

        return response;
    }

    //end of fstream functions

    //
    //  MEMORY CLEAR FUNCTIONS
    //

    //  buffer clear function

    void clear()
    {
        delete buffer;
    }
    

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