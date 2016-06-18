
/*
Devoir 2 Question 6
Julien De Loor (julien.de-loor1@uqac.ca)
Algorithme de cryptage XOR très simple à cracker pour des humains 
Difficiles pour des aliens...
*/


#include <string>
#include <iostream>
#include <cassert>

bool is6bits_char(char c)
{
  return (c == ' ' ||     //26 letters + 26 letters + 10 numbers + 1 space = 63 characters
    (c >= '0' && c <= '9') ||
    (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z'));
}

char ascii_to_6bits(char c)
{
  assert(is6bits_char(c));
  if (c >= 'a' && c <= 'z')
  {
    return c - 'a' + 26 + 10 + 1;
  }
  else if (c >= 'A' && c <= 'Z')
  {
    return c - 'A' + 10 + 1;
  }
  else if (c >= '0' && c <= '9')
  {
    return c - '0' + 1;
  }
  return c - ' ';
}

static const char _6bitschar_table[63] = {
  ' ', // space
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', //numbers
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', //LETTERS
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' //letters
};

char _6bits_to_ascii(char c)
{
  assert(c >= 0 && c < 63);
  return _6bitschar_table[c];

}


bool check_6bits(const std::string& s)
{
  for (std::string::const_iterator it = s.cbegin(); it != s.cend(); ++it)
  {
    if (!(is6bits_char(*it))) 
    {
      return false;
    }
  }
  return true;
}


std::string xor(std::string key, const std::string& msg)
{
  while (key.size() < msg.size()) //On allonge artificiellement le clée
  {                               
    key += key; //Attention si on passe ici on ne respecte plus le principe de shanon
  }

  std::string res;
  res.reserve(msg.size());

  for (size_t i = 0; i < msg.size(); ++i)
  {
    /*
      J'ai décider de profiter du fait que l'ensemble des lettres et des chiffres soient codables 
      sur 6 bits pour permettre d'avoir une fonction xor qui renvois toujours des characteres visibles 
      dans la console.
    */
    char key_c = ascii_to_6bits(key[i]);   // key_c est compris entre [0 63] pour codé sur 6 bits 0b00xxxxxx
    char msg_c = ascii_to_6bits(msg[i]);  // msg_c est compris entre [0 63] pour codé sur 6 bits 0b00xxxxxx

    char c = key_c ^ msg_c;   // codé sur 6 bits 0b00xxxxxx
    res += _6bits_to_ascii(c);   // retour à l'ascii codé sur 8bits
  }

  return std::move(res);
}


int main(int argc, char** argv)
{
  std::cout << "Algo TP2 Question 6 Julien De Loor: julien.de-loor1@uqac.ca" << std::endl;
  std::cout << "This program can encrypt/decrypt ASCII characters (letters and numbers only) with XOR function." << std::endl;
  if (argc != 3)
  {
    std::cout << "usage: " << argv[0] << " <key> <msg>" << std::endl;
    std::cout << "<msg> will be crypted/decrypted" << std::endl;
    return 0;
  }

  std::string key = argv[1];
  std::string msg = argv[2];

  if (!check_6bits(key))
  {
    std::cerr << "error: key contains invalid character(s) (you must use [A-Z] or/and [a-z] or/and [0-9] or/and ' ' characters" << std::endl;
    return -1;
  }
  if (!check_6bits(msg))
  {
    std::cerr << "error: msg contains invalid character(s) (you must use [A-Z] or/and [a-z] or/and [0-9] or/and ' ' characters" << std::endl;
    return -1;
  }

  std::string out = xor(key, msg);

  if (!check_6bits(out))
  {
    std::cerr << "error: out contains invalid character(s) this should never happen !" << std::endl;
    return -1;
  }

  std::cout << "key: " << key << std::endl;
  std::cout << "msg: " << msg << std::endl;
  std::cout << "out: " << out << std::endl;

  return 0;
}