#pragma once

#include <sstream>

namespace CryptedVault
{
    template <class T>
    class CryptedVaultException : public std::exception
    {
        public:
            CryptedVaultException(T code, std::string_view details) 
                : details(details)
                , code(code) 
            { 
                stream << "Exception: " << code << " (" << details << ").";
                whatMessage = stream.str();
            }

            CryptedVaultException(T code) 
                : code(code)
            { 
                stream << "Exception: " << code << ".";
                whatMessage = stream.str();
            }

            const char *what() const throw ()
            {
                return  whatMessage.c_str();
            }

            T getCode() const { return code; }
            std::string getDetails() const { return details; }

        private:
            std::string details;
            T code;
            std::stringstream stream;
            std::string whatMessage;
    };
}